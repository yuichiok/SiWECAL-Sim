This folder is the first version of TB2017 sim (gun particle)

Example standalone file:

ddsim --steeringFile example_runddsim_QGSP_BERT_conf1_e+_1GeV_0.py


The sendjobs.sh and generic_condor.sh scripts generate on the fly the scripts (bash, python and condor) for the launching of a set of runs using the condor_sub commands (HTC)

Check the sendjobs.sh (that calls the generic_condor.sh)
