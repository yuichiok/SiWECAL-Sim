Here we will locate the useful processors (as digitisation, mip calibration etc)

# Folder structure
- `src/` --> for source code
- `include/` --> for headers
- Rest of folders as defined in [iLCSoft/Marlin](https://github.com/iLCSoft/Marlin).
    
# Install/compile:

(example)
source correct_version_path/init_ilcsoft.sh
cd yourpath/SiWECAL-Sim/processors/
mkdir build
cd build
cmake -C $ILCSoft/ILCSoft.CMake ..
make install

```
source correct_version_path/init_ilcsoft.sh
cd yourpath/SiWECAL-Sim/processors/
mkdir build
cd build
cmake -C $ILCSoft/ILCSoft.CMake ..
make install
```

## Installing Marlin

Follow instructions at  [iLCSoft/Marlin](https://github.com/iLCSoft/Marlin). For easily running the processors here, copy the existing example/template on your Marlin installation using the dedicated script (e.g. `./copy_new_Processor.sh mymarlin ConversionProcessor`) then copy the source and header files from your clone of this repository to the new template.


01/2022 A. Irles, F. Jiménez Morales
