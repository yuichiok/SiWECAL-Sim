#!/bin/bash

particle="e-"
conf="6"
for energy in 10.0 20.0 40.0 60.0 80.0 100.0 150.0 200.0
do
  #source generic_condor.sh $particle $energy $conf "grid_-40-40_"$particle$energy"GeV.mac"
  source generic_condor.sh $particle $energy $conf 
  #break
done
