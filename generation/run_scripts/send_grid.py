#!/usr/bin/env python3

import argparse, subprocess, os

# Energies in TB2021-11
ENERGIES = ['1.0', '1.4', '1.8', '2.2', '2.6', '3.0', '3.4', '3.8', '4.2', '4.6', '5.2', '5.6', '6.0']

mkdir = lambda filename: os.makedirs(filename, exist_ok=True)

parser = argparse.ArgumentParser()
parser.add_argument('--energies', nargs='+', help='input pickled pandas df', default=ENERGIES)
parser.add_argument('--particle', help='Particle', default='e-')
parser.add_argument('--conf', help='Conf as feined in confs.py', required=True)

if __name__ == "__main__":
    args = parser.parse_args()
    for energy in args.energies:
        subprocess.run(['source', args.particle, energy, args.conf])
