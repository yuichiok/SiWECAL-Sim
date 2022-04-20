# Generation of beam events and the SiW-ECAL

The generation is based on the [DD4hep](https://github.com/iLCSoft/lcgeo) models for detectors.

## Setup

The software here has been tested with LCIO: `source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh`

## Structure

- `geometry`: Contains the definition of the detector model geometries and configurations.
- `run_scripts`: A set of tools for running the simulations with different beam conditions. Includes scripts for sending batch jobs.

## Test beam configurations

A script to write the geometry description for a given TB year and configuration
```bash
./write_geom_desc.py TB2022-03_CONF1 > geometry/TB2022-03/ECAL_CONF1.xml 
```
### Configurations

Defined in `confs.py`. Per test beam session:

- 2017-06 (DESY):
  - CONF0: No Tungsten.
  - CONF[1,2,3]: Tungsten for different shower parts.
- 2021-11 (DESY):
  - CONF0: No Tungsten.
  - CONF1: Tungsten.
- 2022-03 (DESY):
  - CONF[0,2]: No Tungsten, different slab (Si) ordering.
  - CONF[1,3]: Same Tungsten configuration, different slab (Si) ordering.

### Visualization

Run 
```bash
geoDisplay generation/geometry/TB2022-03/ECAL_CONF0.xml # or any other geometry
```
There are some issues running this from LLR when one has zsh as default shell, but a workaround using x2go is:
```bash
source /cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/v02-00-01/init_ilcsoft.sh # still in zsh
bash
source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh # already in bash
```
Then run the `geoDisplay` as indicated above.

## Comments

A few minor issues that need to be fixed:
- Some problems with the software configuration with zsh. Better to use bash.
- The script to write configurations is written in python3 (but ilcsoft gives python2). Needs to be run in a separate environment.

## Previous work

This part of the repository is based on [Daniel Jeans' work](https://gitlab.cern.ch/calice/calice_dd4heptestbeamsim/-/tree/master/2017_SiECAL_DESY/).
