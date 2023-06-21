#!/bin/bash

# particle="e-"
# particle="mu-"
conf="6"
template_file=steering/templates/LCIO2Build.xml

cd ../

for particle in e- mu-
do
  for energy in 10.0 20.0 40.0 60.0 80.0 100.0 150.0 200.0
  do
    ./run_scripts/prepare_LCIO2Build.py \
      --template ${template_file} \
      --output_filename steering/Conversion/TB2022-06_CONF6_${particle}_${energy}GeV.xml \
      --LCIOInputFiles data/ECAL_QGSP_BERT_conf6_${particle}_${energy}GeV_{1..20}.slcio \
      --OutputBuildFile build_root/ECAL_QGSP_BERT_conf6_${particle}_${energy}GeV_build.root \
      --MaxRecordNumber 5000
  done
done

cd -