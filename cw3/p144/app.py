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

def parse_args():
    parser = argparse.ArgumentParser(prog='p144')
    parser.add_argument('--pbrt-exe', type=str, help='path to pbrt exe', required=True)
    parser.add_argument('--ignore-errors', action='store_true', default=False)
    parser.add_argument('--print-test-scenes', action='store_true', default=False)

    parser.add_argument('--scenes', nargs='+', type=open, required=True)
    parser.add_argument('--integrators', nargs='+', required=True, help="provide the rest of an integrator line as a single argument")
    parser.add_argument('--samplers', nargs='+', required=True, help="provide the rest of a sampler line as a single argument, excluding pixelsamples")
    parser.add_argument('--sample-counts', nargs='+', type=int, required=True)

    return parser.parse_args()

re_integrator: Pattern[str] = re.compile('^Integrator (.*)', re.MULTILINE)
re_sampler: Pattern[str] = re.compile('^Sampler (.*)', re.MULTILINE)
re_film: Pattern[str] = re.compile('^Film(.*)', re.MULTILINE)

def modify_scene(scene, integrator, sampler, sample_count, test=False):
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

    scene = re_integrator.sub("Integrator " + integrator, scene)
    scene = re_sampler.sub("Sampler {} \"integer pixelsamples\" [{}]".format(sampler, sample_count), scene)
    scene = re_film.sub('Film "image" "string filename" "___p144out.exr" "integer xresolution" [400] "integer yresolution" [400]', scene)

    return scene

def simplify_pbrt_input_param(s):
    return s.split(" ")[0].upper()

def run_combinations(parser, scenes, scene_filenames, integrators, samplers, sample_counts):
    if os.path.isfile('out'):
        eprint("'out' is a file. we need to make a folder.")
        sys.exit(1)
    elif os.path.isdir('out'):
        if len(os.listdir('out')) != 0:
            eprint("folder 'out' needs to be empty. kill the directory? (YES/no) ", end='')
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
    pbrt_exe = os.path.abspath(parser.pbrt_exe)

    for scene_id, scene_filename in enumerate(scene_filenames):
        for integrator in integrators:
            for sampler in samplers:
                for sample_count in sample_counts:
                    scene_out = modify_scene(scenes[scene_id], integrator, sampler, sample_count)

                    out_filename = "out-" + os.path.basename(scene_filename) \
                        + "-i" + simplify_pbrt_input_param(integrator) \
                        + "-s" + simplify_pbrt_input_param(sampler) \
                        + "-n" + str(sample_count)

                    this_scene_folder = os.path.dirname(os.path.abspath(scene_filename))

                    outfile = os.path.join(os.path.join(app_cwd, 'out'), out_filename+'.exr')

                    eprint()
                    eprint(len(outfile) * "-")
                    eprint(outfile)
                    eprint(len(outfile) * "-")
                    eprint(scene_out)
                    eprint(len(outfile) * "-")
                    eprint(outfile)
                    eprint(len(outfile) * "-")
                    eprint()
                    subprocess.run([pbrt_exe], input=bytes(scene_out, 'utf-8'), shell=True, cwd=this_scene_folder)

                    shutil.move(os.path.join(this_scene_folder, "___p144out.exr"), outfile)
                    eprint("^^^^^^ DONE - written to {} ^^^^^^^^".format(outfile))



def main():
    parser = parse_args()

    scenes: List[str] = []
    scene_filenames: List[str] = []
    f: TextIOWrapper
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

    integrators: List[str] = list(set(parser.integrators))
    samplers: List[str] = list(set(parser.samplers))
    sample_counts: List[int] = sorted(set(parser.sample_counts))

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