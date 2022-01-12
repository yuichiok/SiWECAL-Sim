#!/usr/bin/env python3

# Run the following:
# conda activate digi2
# cd ~/LCIO/; . ./setup.sh; cd -

import numpy as np, matplotlib.pyplot as plt, pandas as pd
import uproot4 as ur, awkward1 as ak
import sys, os, argparse, pathlib

from pyLCIO.io import LcioReader
from pyLCIO.UTIL import LCRelationNavigator

version = 'v01'

flat_np = lambda array: ak.to_numpy(ak.flatten(array))
 
parser = argparse.ArgumentParser(description='From slcio to pandas for ECAL digi (v01)')
parser.add_argument('--filename', help='Input slcio digi file')
col_help='Collection (default=ecalSD_MIP, can change to SiEcalCollection)'
parser.add_argument('--collection', help=col_help, default='ecalSD_MIP')

def to_pandas(args):

    reader = LcioReader.LcioReader(args.filename)
    events = []
    counter = 0
    for event in reader:
        try:
            hits = event.getCollection(args.collection)
            this_hits = []
            for hit in hits:
                # If 'pos' list of len 3, pandas triplicates 1-valued 'e'
                # => v01: use single, flat pos[x/y/z]
                this_hits.append({'e': hit.getEnergy(),
                                  'posx': hit.getPosition()[0],
                                  'posy': hit.getPosition()[1],
                                  'posz': hit.getPosition()[2],
                                  })
            events.append(this_hits)
        except: counter += 1
    print(counter, 'exceptions')
    events = ak.Array(events)
    return ak.to_pandas(events)

def main():
    args = parser.parse_args()
    print('Filename:', args.filename, '\nCollection:', args.collection)

    ev = to_pandas(args)
    
    p = pathlib.Path(args.filename)
    v_path = 'results/' + version + '/'
    os.makedirs(v_path, exist_ok=True)
    pickle_file = v_path + str(p.stem) + '.pickle'
    ev.to_pickle(pickle_file, protocol=4)
    print('Pickled in', pickle_file)
    return

if __name__ == '__main__':
    main()

