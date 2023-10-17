# Example the command to generate events

Here the example command is presented.
This is more or less Yuichi's interpretation to the simulation program.
For the general tutorial, follow the guideline presented in [`README.md`](https://github.com/yuichiok/SiWECAL-Sim/blob/main/processors/README.md).

In general, we use (py3 env) to create a steering file, and use (ilcsoft env) for the Marlin execution.

Execute following (py3 env)
```shell
./run_scripts/prepare_Conversion.py \
--template steering/templates/Conversion.xml \
--output_filename steering/Conversion/TB2022-06_CONF6_mu-_conv_test.xml \
--LCIOInputFiles /sps/ilc/yokugawa/calice/Data/sim/TB2022-06/CONF6/lcio/ECAL_QGSP_BERT_conf6_mu-_10GeV_*.slcio \
--ConvAuxFile /sps/ilc/yokugawa/calice/Data/sim/TB2022-06/CONF6/build/TB2022-06_CONF6_mu-_10GeV_conv_test.root \
--MaxRecordNumber 5000 \
--tb_conf TB2022-06_CONF6 \
--MIPFitMode 1
```
This will prepare a steering file to convert GeV to MIP value.
Execute by (ilcsoft env)
```shell
Marlin steering/Conversion/TB2022-06_CONF6_mu-_conv_test.xml
```
Then GeV to MIP conversion ratio is now calculated.
Now prepare a steering file to run with these ratio setting.
```shell
./run_scripts/prepare_LCIO2Build.py \
--template steering/templates/LCIO2Build.xml \
--output_filename steering/LCIO2Build/TB2022-06_CONF6_e-_10GeV_test.xml \
--LCIOInputFiles /sps/ilc/yokugawa/calice/Data/sim/TB2022-06/CONF6/lcio/ECAL_QGSP_BERT_conf6_e-_10GeV_*.slcio \
--OutputBuildFile /sps/ilc/yokugawa/calice/Data/sim/TB2022-06/CONF6/build/ECAL_QGSP_BERT_conf6_e-_10GeV.root \
--MaxRecordNumber 5000
```
Then execute this steering file via Marlin. (ilcsoft env)
```shell
Marlin steering/Conversion/TB2022-06_CONF6_e-_10GeV_test.xml
```