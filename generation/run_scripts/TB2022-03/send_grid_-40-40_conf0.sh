#!/bin/bash

particle="e-"
#conf="0"
conf="2" # conf2, like conf0, has no W
for energy in 3.0
#for energy in 3.0 23.5 63.5 122.9 201.7 300.0
do
  #source generic_condor.sh $particle $energy $conf "grid_-40-40_"$particle$energy"GeV.mac"
  source generic_condor.sh $particle $energy $conf 
  #break
done
