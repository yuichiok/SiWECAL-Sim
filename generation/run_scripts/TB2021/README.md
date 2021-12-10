Example standalone file:

ddsim --enableG4GPS --macroFile grid58_e3GeV.mac --steeringFile example_runddsim_QGSP_BERT_conf1_e+_1GeV_0.py


The send_grid58.sh and generic_condor.sh scripts generate on the fly the scripts (bash, python and condor) for the launching of a set of runs using the condor_sub commands (HTC)

grid58 stands for one of the beam position and condition during the TB2017

Plese check the send_grid58.sh (that calls the generic_condor.sh)

