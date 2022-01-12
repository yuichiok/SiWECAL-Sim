Example standalone file:

ddsim --enableG4GPS --macroFile grid_-40-40_e-3GeV.mac --steeringFile example_runddsim_QGSP_BERT_conf0_e-_3GeV_0.py

The send_grid58.sh and generic_condor.sh scripts generate on the fly the scripts (bash, python and condor) for the launching of a set of runs using the condor_sub commands (HTC)

-40-40 stands for the beam position and condition during the Tungsten runs in TB2021 

Please check the send_grid58.sh (that calls the generic_condor.sh)

