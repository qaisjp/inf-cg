#!/usr/bin/env python3

import argparse
from _io import TextIOWrapper
from typing import List, Pattern, Match
from util import eprint
from termcolor import colored
from collections import Counter
import re
import sys
import os
import shutil
import subprocess
import json

def parse_args():
    parser = argparse.ArgumentParser(prog='p144')
    parser.add_argument('--pbrt-folder', type=str, help='path to pbrt binaries folder', required=True)
    parser.add_argument('--ignore-errors', action='store_true', default=False)
    parser.add_argument('--incremental', action='store_true', default=False)
    parser.add_argument('--print-test-scenes', action='store_true', default=False)
    parser.add_argument('--force-remove-out', action='store_true', default=False)
    parser.add_argument('--quant', action='store_true', default=False, help="generate reference image and include quant.json")

    parser.add_argument('--baseline-integrator', type=str, default='path')
    parser.add_argument('--baseline-sampler', type=str, default='random')
    parser.add_argument('--scenes', nargs='+', type=open, required=True)
    parser.add_argument('--integrators', nargs='+', required=True, help="provide the rest of an integrator line as a single argument")
    parser.add_argument('--samplers', nargs='+', required=True, help="provide the rest of a sampler line as a single argument, excluding pixelsamples")
    parser.add_argument('--sample-counts', nargs='+', type=int, required=True)
    parser.add_argument('--cropwindow', type=str, metavar='<x0,x1,y0,y1>', help='Specify an image crop window (comma separated)', default="0,1,0,1")

    return parser.parse_args()

re_integrator = re.compile('^Integrator (.*)', re.MULTILINE)
re_sampler = re.compile('^Sampler (.*)', re.MULTILINE)
re_film = re.compile('^Film(.*)', re.MULTILINE)

def modify_scene(scene, integrator, sampler, sample_count, cropwindow="", test=False):
    if "pixelsamples" in sampler:
        eprint(colored("Given sampler should not contain 'pixelsamples'. This is the job of the application.", 'red'))
        eprint(colored("Sampler input: " + sampler, 'yellow'))
        sys.exit(1)
        return False

    films = re_film.findall(scene)
    if test:
        if len(films) != 1:
            eprint("Should be exactly 1 match for a Film")
            return False

        integrators = re_integrator.findall(scene)
        if len(integrators) != 1:
            eprint("Should be exactly 1 match for an Integrator")
            return False

        sampler_info = re_sampler.findall(scene)
        if len(sampler_info) != 1:
            eprint("Should be exactly 1 match for a Sampler")
            return False

    if " " not in integrator and not integrator.startswith('"') and not integrator.endswith('"'):
        integrator = '"' + integrator + '"'

    if " " not in sampler and not sampler.startswith('"') and not sampler.endswith('"'):
        sampler = '"' + sampler + '"'

    if cropwindow != "":
        cropwindow = ' "float cropwindow" [ ' + " ".join(cropwindow) + ' ]'

    scene = re_integrator.sub("Integrator " + integrator, scene)
    scene = re_sampler.sub("Sampler {} \"integer pixelsamples\" [{}]".format(sampler, sample_count), scene)
    scene = re_film.sub('Film "image" "string filename" "___p144out.exr" "integer xresolution" [400] "integer yresolution" [400]' + cropwindow, scene)

    with open('_lastscene.pbrt', 'w') as this:
        this.write(scene)

    return scene

def simplify_pbrt_input_param(s):
    return s.split(" ")[0].upper()

def catout_to_list(catout):
    l = []
    for line in catout.decode('utf-8').split("\n"):
        line = line.strip()
        if line == "":
            continue

        _, right = line.split(': ')
        assert right[0] == '('
        assert right[-1] == ')'
        tr, tg, tb = map(float, right[1:-1].split(' '))
        l.append((tr, tg, tb))
    return l

def catout_to_mse(catout, reference):
    mse = 0
    N = 0

    ours = catout_to_list(catout)
    assert len(ours) == len(reference)

    for tr, tg, tb in ours:
        rr, rg, rb = reference[N]
        r = abs(tr - rr)
        g = abs(tg - rg)
        b = abs(tb - rb)

        mse += r*r + g*g + b*b
        N += 1

    assert len(ours) == N
    return mse/N

def get_scene_refname(scene_filename):
    return "ref-" + os.path.basename(scene_filename) + ".exr"

def run_combinations(parser, scenes, scene_filenames, integrators, samplers, sample_counts):
    if os.path.isfile('out'):
        eprint("'out' is a file. we need to make a folder.")
        sys.exit(1)
    elif os.path.isdir('out'):
        if not parser.incremental and len(os.listdir('out')) != 0:
            eprint("folder 'out' needs to be empty. kill the directory? (YES/no) ", end='')
            if parser.force_remove_out:
                inp = 'y'
            else:
                inp = input().lower()
            if inp == 'yes' or inp == 'y':
                eprint('killing "out"... ', end='')
                shutil.rmtree('out')
                eprint('done!')
            else:
                eprint("aborting cuz your out folder is not killed")
                return

    os.makedirs('out', exist_ok=True)

    app_cwd = os.getcwd()
    pbrt_folder = os.path.abspath(parser.pbrt_folder)
    pbrt_exe = os.path.join(pbrt_folder, "pbrt")
    imgtool_exe = os.path.join(pbrt_folder, "imgtool")

    cropwindow = None
    if parser.cropwindow is not None:
        cropwindow = parser.cropwindow.replace(" ", "").split(",")

    mses = {}
    references = {}
    if parser.quant:
        eprint()
        eprint("--------------")
        eprint("---  QUANT  --")
        eprint("--------------")
        ref_sample_count = max(sample_counts) * 100
        eprint("Reference sample count is", ref_sample_count)
        eprint("Reference integrator is", parser.baseline_integrator)
        eprint("Reference sampler is", parser.baseline_sampler)

        for scene_id, scene_filename in enumerate(scene_filenames):
            scene_out = modify_scene(scenes[scene_id], parser.baseline_integrator, parser.baseline_sampler, ref_sample_count, cropwindow)
            out_filename = get_scene_refname(scene_filename)
            eprint("\n>>>> Generating", out_filename, "({} of {})".format(scene_id+1, len(scene_filenames)))
            outfile = os.path.join(os.path.join(app_cwd, 'out'), out_filename)
            outfile_exists = os.path.isfile(outfile)
            this_scene_folder = os.path.dirname(os.path.abspath(scene_filename))

            if not outfile_exists or (outfile_exists and not parser.incremental):
                tempfile = os.path.join(this_scene_folder, "___p144out.exr")
                subprocess.run([pbrt_exe], input=bytes(scene_out, 'utf-8'), shell=True, cwd=this_scene_folder, check=True)
                shutil.move(tempfile, outfile)

            catout = subprocess.run([imgtool_exe, "cat", outfile], stdout=subprocess.PIPE, check=True)

            l = catout_to_list(catout.stdout)
            references[out_filename] = l

            eprint("Generated reference image", out_filename)

    item_id = 0
    item_count = len(scene_filenames) * len(integrators) * len(samplers) * len(sample_counts)
    for scene_id, scene_filename in enumerate(scene_filenames):
        for integrator in integrators:
            for sampler in samplers:
                for sample_count in sample_counts:
                    scene_out = modify_scene(scenes[scene_id], integrator, sampler, sample_count, cropwindow)

                    out_filename = "out-" + os.path.basename(scene_filename) \
                        + "-i" + simplify_pbrt_input_param(integrator) \
                        + "-s" + simplify_pbrt_input_param(sampler) \
                        + "-n" + "{:03d}".format(sample_count) \
                        + ".exr"

                    this_scene_folder = os.path.dirname(os.path.abspath(scene_filename))

                    tempfile = os.path.join(this_scene_folder, "___p144out.exr")
                    outfile = os.path.join(os.path.join(app_cwd, 'out'), out_filename)
                    outfile_exists = os.path.isfile(outfile)

                    eprint()
                    # eprint(len(outfile) * "-")
                    # eprint(outfile)
                    # eprint(len(outfile) * "-")
                    # eprint(scene_out)
                    eprint(len(outfile) * "-")
                    eprint(outfile + " ({} of {})".format(item_id+1, item_count))
                    eprint(len(outfile) * "-")
                    eprint()

                    if not outfile_exists or (outfile_exists and not parser.incremental):
                        subprocess.run([pbrt_exe], input=bytes(scene_out, 'utf-8'), shell=True, cwd=this_scene_folder, check=True)
                        shutil.move(tempfile, outfile)

                    if parser.quant:
                        catout = subprocess.run([imgtool_exe, "cat", outfile], stdout=subprocess.PIPE, check=True)
                    if parser.quant:
                        mse = catout_to_mse(catout.stdout, references[get_scene_refname(scene_filename)])
                        mses[out_filename] = mse
                    else:
                        mse = "MSE NOT CALCULATED - SEE QUANT FLAG"

                    item_id += 1
                    eprint("^^^^^^ DONE - written to {} - mse: {}^^^^^^^^ - {} left".format(outfile, mse, item_count - item_id))

    if parser.quant:
        with open('out/quant.json', 'w') as f:
            f.write(json.dumps(mses, indent=4, sort_keys=True))


def main():
    parser = parse_args()

    scenes = []
    scene_filenames = []
    for f in parser.scenes:
        ftext = f.read()
        scenes.append(ftext)

        scene_out = modify_scene(ftext, "test_integrator", "test_sampler", "test_sample_count", test=True)
        if scene_out == False:
            eprint("Failed on", f.name)
            return

        if parser.print_test_scenes:
            eprint()
            eprint(len(f.name) * "-")
            eprint(f.name)
            eprint(len(f.name) * "-")
            eprint(scene_out)
            eprint()

        scene_filenames.append(f.name)
        f.close()

    if parser.print_test_scenes:
        eprint("Test scenes printed. Halting.")
        return

    integrators = list(set(parser.integrators))
    samplers = list(set(parser.samplers))
    sample_counts = sorted(set(parser.sample_counts))

    num_combinations = len(scenes) * len(integrators) * len(samplers) * len(sample_counts)

    if parser.ignore_errors:
        msg = "YOU ARE IGNORING ERRORS!"
        eprint(colored(len(msg) * "-", 'red', attrs=['bold']))
        eprint(colored(msg, 'red', attrs=['bold']))
        eprint(colored(len(msg) * "-", 'red', attrs=['bold']))

    sanity_check_failed = False

    if len(set(scene_filenames)) != 4:
        eprint(colored("- You need 4 *unique* scenes.", 'red'))
        sanity_check_failed = True
    if len(integrators) != 3:
        eprint(colored("- You need 3 *unique* integrators.", 'red'))
        sanity_check_failed = True
    if len(samplers) != 4:
        eprint(colored("- You need 3 *unique* samplers.", 'red'))
        sanity_check_failed = True
    if len(sample_counts) != 3:
        eprint(colored("- You need 3 *unique* sample counts.", 'red'))
        sanity_check_failed = True
    if num_combinations != 144:
        eprint(colored("- You need 144 *unique* combinations.", 'red'))
        sanity_check_failed = True

    if parser.ignore_errors or sanity_check_failed:
        eprint()

    eprint("Scenes ({}):\t\t{}".format(len(scenes), ", ".join(scene_filenames)))
    eprint("Integrators ({}):\t{}".format(len(integrators), ", ".join(integrators)))
    eprint("Samplers ({}):\t\t{}".format(len(samplers), ", ".join(samplers)))
    eprint("Sample counts ({}):\t{}".format(len(sample_counts), ", ".join(map(str, sample_counts))))
    eprint("Combinations:\t\t{}".format(num_combinations))

    if parser.ignore_errors and sanity_check_failed:
        eprint("\nSanity check failed.", colored("Ignoring.", 'red'))
    elif sanity_check_failed:
        eprint("\nSanity check failed.", colored("Aborting.", 'red'))
        return

    run_combinations(parser, scenes, scene_filenames, integrators, samplers, sample_counts)


if __name__ == "__main__":
    main()