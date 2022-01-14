source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03//init_ilcsoft.sh        
cp -r /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/steer/runddsim_QGSP_BERT_conf1_e-_5.6GeV_0.* .
ddsim --enableG4GPS --macroFile /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/macros/grid_-40-40_e-_5.6GeV.mac --steeringFile /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/steer/runddsim_QGSP_BERT_conf1_e-_5.6GeV_0.py
#&> /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/log/QGSP_BERT_conf1_e-_5.6GeV_0.log
#tar czvf /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/data/ECAL_QGSP_BERT_conf1_e-_5.6GeV_0.slcio.tar.gz ECAL_QGSP_BERT_conf1_e-_5.6GeV_0.slcio 
#rm /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/log/errors_runddsim_QGSP_BERT_conf1_e-_5.6GeV_0* /home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/log/outfile_runddsim_QGSP_BERT_conf1_e-_5.6GeV_0*
