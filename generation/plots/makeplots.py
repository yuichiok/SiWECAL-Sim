# initialize environment:
#  export PYTHONPATH=${LCIO}/src/python:${ROOTSYS}/lib

from pyLCIO.io import LcioReader
from pyLCIO.UTIL import LCRelationNavigator
import ROOT
ROOT.gROOT.SetBatch()
import string


rfile = ROOT.TFile("./ecalhits.root","recreate")

en=1

h_en={}
h_toten={}
h_rad={}
h_xy={}
h_molr={}

for conf in (0,1):
    h_en[conf]={}
    h_toten[conf]={}
    h_rad[conf]={}
    h_xy[conf]={}
    h_molr[conf]={}

    for en in (40,80):

        lab='conf'+str(conf)+'_mu-_'+str(en)+'GeV'
        fname='data/ECAL_'+lab+'.slcio'

        print fname

        reader = LcioReader.LcioReader(fname)

        lab=string.replace(lab,'-','')

        h_toten[conf][en] = ROOT.TH1F("toten_"+lab,'toten'+lab,500,0,0.05)
        h_en[conf][en]    = ROOT.TH1F("hiten_"+lab,'hiten'+lab,500,0,0.0025)
        h_rad[conf][en]   = ROOT.TH1F("hitrad_"+lab,'hitrad'+lab,500,0,50.)
        h_molr[conf][en]   = ROOT.TH1F("molr_"+lab,'molr'+lab,100,0,100.)
        h_xy[conf][en]    = ROOT.TH2F("hitxy_"+lab,'hitxy'+lab,25,-100,100,25,-100,100)

        for nevent in range(reader.getNumberOfEvents()):
            event = reader.next()
            try:
                hits = event.getCollection("SiEcalCollection")
                toten=0
                avex=0
                avey=0
                for p in hits:
                    energy=p.getEnergy()
                    hpx = p.getPosition()[0]
                    hpy = p.getPosition()[1]
                    toten=toten+energy

                    avex=avex+hpx*energy
                    avey=avey+hpy*energy

                    h_en[conf][en].Fill(energy)
                    h_xy[conf][en].Fill(hpx, hpy, energy)
                h_toten[conf][en].Fill(toten)

                avex=avex/toten
                avey=avey/toten
                allhitdist={}
                for p in hits:
                    energy=p.getEnergy()
                    hpx = p.getPosition()[0]
                    hpy = p.getPosition()[1]
                    rad = ((hpx-avex)**2 + (hpy-avey)**2)**0.5
                    h_rad[conf][en].Fill(rad, energy)

                    while rad in allhitdist.keys():
                        rad=rad+0.001
                    allhitdist[rad]=energy

                dd = allhitdist.keys()
                dd.sort()

                sume=0.
                molrad=0.
                for dist in dd:
                    sume = sume+allhitdist[dist]
                    if sume > 0.9*toten:
                        molrad=dist
                        break

                h_molr[conf][en].Fill(molrad)

            except:
                aa=1

rfile.Write()

cc=ROOT.TCanvas()
cc.Print('ecalhits.ps[')

for hh in (h_toten, h_en, h_rad, h_molr):
    cc.Clear()
    cc.Divide(2,2)
    for conf in (1,2,3):
        cc.cd(conf)
        icol=1
        hmax=0
        for en in hh[conf].keys():
            hh[conf][en].SetLineColor(icol)
            icol=icol+1
            tmax=hh[conf][en].GetMaximum()
            if tmax>hmax:
                hmax=tmax

        ff=True
        for en in hh[conf].keys():
            if ff:
                hh[conf][en].SetMaximum(hmax*1.1)
                hh[conf][en].Draw()
                ff=False
            else:
                hh[conf][en].Draw('same')

    cc.Print('ecalhits.ps')


cc.Clear()
cc.Divide(6,3)

hh=h_xy

print hh
print hh.keys()
for kk in hh.keys():
    print hh[kk].keys()

for conf in (1,2,3):
    ic=1
    for en in hh[conf].keys():
        cc.cd((conf-1)*6 + ic )
        ic=ic+1
        hh[conf][en].Draw('col')

cc.Print('ecalhits.ps')


cc.Print('ecalhits.ps]')

rfile.Write()
rfile.Close()
