#!/usr/bin/env python3

import pandas as pd, numpy as np, matplotlib.pyplot as plt
import argparse, sys, os, pathlib
from utils import e_conf_str, moliere_dict

plt.rcParams['font.size'] = '16'
plt.rcParams['savefig.dpi'] = 300

mkdir = lambda filename: os.makedirs(filename, exist_ok=True)
pd_df = lambda series: pd.DataFrame(series)
flat_np = lambda df: df.to_numpy().flatten()
h2d = np.histogram2d
npu = np.unique
roustr = lambda x, n: str(np.round(x, n))

parser = argparse.ArgumentParser()
parser.add_argument('--filename', help='input pickled pandas df')
parser.add_argument('--norm_hist', action='store_true', help='Use histo weighted by number of hits', default=False)

version = 'v01'

def errorfun(data):
    # Vincent: delta (E) / E: 1% /sqrt(E) (+) 14%
    mask = data > 0
    ans = np.zeros_like(data)
    #ans[mask] = data[mask] * np.sqrt(  (0.01 / np.sqrt(data[mask]))**2 + 0.14**2)
    ans[mask] = data[mask] * np.sqrt(  (0.1 / np.sqrt(data[mask]))**2 + 0.046**2)
    return ans


def old_df_bins(df):
    # Unfortunately deprecated as not always all cells are hit
    # Then, unique doesn't accurately recover all cells, in particular conf0
    xy_bins = []
    for pos in ('posx', 'posy'):
        bins = npu(df[pos])
        bins = np.concatenate(([bins[0] - (bins[1] - bins[0]) / 2],
                               bins[1:] + np.diff(bins) / 2))
        xy_bins.append(bins)
    return xy_bins

def df_bins(df):
    # Unfortunately deprecated as not always all cells are hit
    # Then, unique doesn't accurately recover all cells, in particular conf0
    xy_bins = []
    for pos in ('posx', 'posy'):
        bins = np.linspace(-88., 88., 33)
        bins = np.concatenate(([bins[0] - (bins[1] - bins[0]) / 2],
                               bins[1:] + np.diff(bins) / 2))
        xy_bins.append(bins)
    return xy_bins



def plot_xy_sum(args, df):
    # 2d histogram of energy 
    H, xed, yed = h2d(flat_np(df['posx']), flat_np(df['posy']),
                      #weights=flat_np(df['e']), bins=32)
                      weights=flat_np(df['e']), bins=df_bins(df))
    
    #pdb.set_trace()
    fig, axs = plt.subplots(1, 3, figsize=(15, 5))
    conf, e = e_conf_str(args.basepath)
    fig.suptitle(conf + ', ' + e)
    title_str = 'Sum hit energies'
    if 'digi' in args.basepath: title_str += ' (digi)'
    axs[0].set_title(title_str)
    X, Y = np.meshgrid(xed, yed)
    im = axs[0].pcolormesh(X, Y, H)
    fig.colorbar(im, ax=axs[0], fraction=0.046, pad=0.04)
    axs[0].set_aspect('equal')
    
    # Moliere radius distribution
    m_dict = moliere_dict(df)
    radii = m_dict['radii']
    radii.hist(ax=axs[1], bins=50, grid=False)
    axs[1].set_title('Moliere Radius distribution')
    axs[1].set_xlabel(r'$R_M$')

    # Energy distribution
    df['e'].hist(ax=axs[2], bins=np.linspace(0, 0.003, 200), grid=False)
    axs[2].set_title('All hit energy')
    axs[2].set_xlabel('(Raw) Energy [GeV]')
    
    savepath = args.basepath + 'sum_energy_xy.png'
    plt.tight_layout()
    fig.savefig(savepath)
    print('Saved in', savepath)
    return

def plot_xy_layers(args, df):
    
    zvals = npu(df['posz'])
    fig, axs = plt.subplots(4, 4, figsize=(16, 16))
    conf, e = e_conf_str(args.basepath)
    title_str = 'Energy in cells per layer ' + conf + ', ' + e + '\n XY in [mm], "truth" average energy [GeV] from sim'
    fig.suptitle(title_str)
    sum_H = []
    sum_Herr = []

    # Transversal plots
    for iz, zval in enumerate(zvals):
        zmask = df['posz'] == zval
        #layer_nhits = np.sum(zmask)
        H, xed, yed = h2d(flat_np(df['posx'][zmask]),
                          flat_np(df['posy'][zmask]),
                          weights=flat_np(df['e'][zmask]),
                          bins=df_bins(df))
                          #bins=32)
        X, Y = np.meshgrid(xed, yed)
        if args.norm_hist: H = H / len(df)
        Herr = errorfun(H)
        iax = axs.flat[iz]
        im = iax.pcolormesh(X, Y, H)
        fig.colorbar(im, ax=iax, fraction=0.046, pad=0.04)
        #fig.colorbar(im, ax=iax)
        iax.set_aspect('equal')
        iax.set_title('Layer ' + str(iz) + ' (z = ' + roustr(zval - 6.1, 0) + ' mm)')
        sum_H.append(np.sum(H))
        sum_Herr.append(np.sqrt(np.sum(Herr**2)))
    #print('Binning to be checked!!')
    
    # Longitudinal profile, to be reimplemented
    # # Long profile plot
    # gs = axs[2, 1].get_gridspec()
    # for dax in axs[-1, 1:]: dax.remove()
    # axbig = fig.add_subplot(gs[-1, 1:])
    # #axbig.plot(zvals, sum_H, marker='.', ls='', markersize=20)
    # axbig.errorbar(zvals, sum_H, yerr=sum_Herr, marker='.', ls='', markersize=20)
    # axbig.set_title('Longitudinal')
    # axbig.set_xlabel('z position [mm]')
    # axbig.set_ylabel('Energy in layer')
    
    savepath = args.basepath + 'layer_energy_xy.png'
    fig.tight_layout()
    fig.savefig(savepath)
    print('Saved in', savepath)
    return

def main():
    args = parser.parse_args()
    df = pd.read_pickle(args.filename)
    p = pathlib.Path(args.filename)
    args.basepath = str(p.parent.absolute()) + '/plots/' + str(p.stem) + '/'
    print('Base path', args.basepath)
    mkdir(args.basepath)

    plot_xy_sum(args, df)
    plot_xy_layers(args, df)

if __name__ == '__main__':
    main()
