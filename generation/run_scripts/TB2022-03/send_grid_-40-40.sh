#!/bin/bash

particle="e-"
conf="3"
#for energy in 1.0 1.4 1.8 2.2 2.6 3.0 3.4 3.8 4.2 4.6 5.2 5.6 6.0 # conf1
#for energy in 3.0 23.5 63.5 122.9 201.7 300.0 # conf1 - highE
for energy in 1.0 2.0 2.6 3.0 4.0 4.6 5.0 5.6 5.8 6.0
do
  #source generic_condor.sh $particle $energy $conf "grid_-40-40_"$particle$energy"GeV.mac"
  source generic_condor.sh $particle $energy $conf 
  #break
done
