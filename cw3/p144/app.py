#!/usr/bin/env python3

import argparse
from _io import TextIOWrapper
from typing import List
from util import eprint
from termcolor import colored
from collections import Counter

def parse_args():
    parser = argparse.ArgumentParser(prog='p144')
    parser.add_argument('--scenes', nargs='+', type=open, required=True)
    parser.add_argument('--integrators', nargs='+', required=True)
    parser.add_argument('--samplers', nargs='+', required=True)
    parser.add_argument('--sample-counts', nargs='+', type=int, required=True)

    parser.add_argument('--ignore-errors', action='store_true', default=False)
    return parser.parse_args()

def main():
    parser = parse_args()

    scenes: List[TextIOWrapper] = parser.scenes
    scene_filenames: List[str] = list(map(lambda f: f.name, scenes))
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

    if num_combinations != 144:
        eprint(colored("- You need 144 *unique* combinations.", 'red'))
        sanity_check_failed = True
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


if __name__ == "__main__":
    main()