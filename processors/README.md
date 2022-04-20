# Processors for digitization

## Notes on the processors

The main processors of interest for simulation and digitization are:

- `ConversionProcessor`: To make Raw energy -> MIP conversion.
- `ShapingProcessor`: Simulates the pulse shaping in the ASICs (in progress of adaptation for TB21/22).
- `ShapedConversion`: A new conversion to MIP after the effect of the shaping (in progress of adaptation for TB21/22).

There are other processors for misc tasks such as `(Digi)LCIO2BuildProcessor` for transforming LCIO ((Sim)CalorimeterHits) to the "build" format used in data.

## Setup and running

```
source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh # Or your favorite ilcsoft version
./build.sh
export MARLIN_DLL=$PWD/lib/libDigitization.so:$MARLIN_DLL
Marlin <path_to_steering_file.xml>
```

Under `run_scripts` there are per-processor `prepare_steering.py` (python3) scripts to help with the steering file writing, to ease multiple file processing. Input templates can be found under `steering/templates/`.

