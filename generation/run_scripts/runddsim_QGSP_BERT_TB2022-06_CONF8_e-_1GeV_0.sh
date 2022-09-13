source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03//init_ilcsoft.sh
cp -r /grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2022-06/steer//runddsim_QGSP_BERT_TB2022-06_CONF8_e-_1GeV_0.{py,sh} .
#This is run in /grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts
ddsim --enableG4GPS --macroFile /grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2022-06/macros/e-_1GeV_0.mac --steeringFile /grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2022-06/steer/runddsim_QGSP_BERT_TB2022-06_CONF8_e-_1GeV_0.py
