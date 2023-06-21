#!/bin/bash

cd ../

for particle in e- mu-
do
  for energy in 10.0 20.0 40.0 60.0 80.0 100.0 150.0 200.0
  do
    srun -N1 -t 1-0 -o submit_jobs/slurm_out/slurm-%j.out --partition=htc \
    Marlin steering/Conversion/TB2022-06_CONF6_${particle}_${energy}GeV.xml
  done
done

cd -