#!/usr/bin/env python3
import numpy as np, pandas as pd, matplotlib.pyplot as plt, pickle

# https://stackoverflow.com/questions/33888973/get-values-from-matplotlib-axessubplot
def get_hist(ax):
    n,bins = [],[]
    for rect in ax.patches:
        ((x0, y0), (x1, y1)) = rect.get_bbox().get_points()
        n.append(y1-y0)
        bins.append(x0) # left edge of each bin
    bins.append(x1) # also get right edge of last bin

    return n,bins

def save_dict(filename, d):
    pickle_out = open(filename, 'wb')
    pickle.dump(d, pickle_out)
    pickle_out.close()
    return

def load_dict(filename):
    pickle_in = open(filename, 'rb')
    example_dict = pickle.load(pickle_in)
    pickle_in.close()
    return example_dict

def e_conf_str(in_str):
    for substr in in_str.split('/'):
        if 'conf' in substr and 'GeV' in substr:
            for subsubstr in substr.split('_'):
                if 'conf' in subsubstr: conf = subsubstr
                elif 'GeV' in subsubstr: e = subsubstr
                else: continue
    #return conf + ', ' + e
    return conf, e

def moliere_dict(df):
    radii = np.zeros(len(df.groupby(level=0)))
    for i, s in df.groupby(level=0):
        toten = np.sum(s['e'])
        avex = np.sum(s['posx'] * s['e']) / toten
        avey = np.sum(s['posy'] * s['e']) / toten

        s['rad'] = ((s['posx'] - avex) ** 2 + (s['posy'] - avey) ** 2) ** 0.5
        s = s.sort_values(by=['rad'])
        s = s.reset_index(drop=True)
        i_90 = np.where(np.cumsum(s['e']) > 0.9 * toten)[0][0]
        radii[i] = s['rad'][i_90]
    m_dict = {'radii': radii, 'avex': avex, 'avey': avey}
    return pd.DataFrame(m_dict)

def main():
    pass

if __name__ == '__main__':
    main()
