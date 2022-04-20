#!/bin/bash

particle="e-"
conf="0"
for energy in 3.0
do
  #source generic_condor.sh $particle $energy $conf "grid_-40-40_"$particle$energy"GeV.mac"
  source generic_condor_angle.sh $particle $energy $conf 
  #break
done
