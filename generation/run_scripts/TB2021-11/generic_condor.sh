if [ ! -e steer ]; then
    mkdir steer
fi
if [ ! -e data ]; then
    mkdir data
fi
if [ ! -e log ]; then
    mkdir log
fi
if [ ! -e macros ]; then
    mkdir macros 
fi

particle=$1
energy=$2
conf=$3

geometry_folder="/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry_TB2021"
ilcsoft_path="/cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/"
local=$PWD
data_path="/data_ilc/flc/jimenez/simulations/TB2021/CONF${conf}/"

#macfile=$4
macfile=grid_-40-40_${particle}_${energy}GeV.mac

nevt=5000

cat > ${local}/macros/$macfile <<EOF

/gps/verbose 1
/gps/particle ${particle}
/gps/direction 0 0 1
/gps/pos/type Beam
/gps/pos/shape Circle
/gps/pos/centre -40 -40 0 mm
/gps/pos/sigma_x 7 mm
/gps/pos/sigma_y 7 mm
/gps/ang/rot1 0 0 1
/gps/ang/rot2 0 1 0

/gps/ene/type Mono
/gps/ene/mono ${energy} GeV
/run/beamOn ${nevt}

EOF


#physl=("QGSP_BERT" "FTFP_BERT")
physl=("QGSP_BERT")

#for energy in ${ens[@]}; do
for physlist in ${physl[@]}; do
  for it in {1..20}; do
    echo $conf $energy $particle $iter
    
    label=${physlist}_conf${conf}_${particle}_${energy}GeV_${it}
    echo $label
    
    scriptname=runddsim_${label}.py
    condorsh=runddsim_${label}.sh
    condorsub=runddsim_${label}.sub 
    condorfile=runddsim_${label}
    
    cat > ${local}/steer/$scriptname <<EOF
from DDSim.DD4hepSimulation import DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
# Number of events defined in macro file
#SIM.numberOfEvents = $nevt

SIM.skipNEvents = 0
#SIM.outputFile = "${local}/data/ECAL_${label}.slcio"
SIM.outputFile = "${data_path}/ECAL_${label}.slcio"

SIM.compactFile = "${geometry_folder}/ECAL_CONF${conf}.xml"
SIM.dumpSteeringFile = "${local}/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "${physlist}"
SIM.enableDetailedShowerMode=True
EOF
    
    # Here should add mac file creation
    
    #echo "Condor sh ${local}/steer/$condorsh"
    
    cat > ${local}/steer/$condorsh <<EOF
source ${ilcsoft_path}/init_ilcsoft.sh        
cp -r ${local}/steer/runddsim_${label}.* .
ddsim --enableG4GPS --macroFile ${local}/macros/${macfile} --steeringFile ${local}/steer/$scriptname
#&> ${local}/log/${label}.log
#tar czvf ${local}/data/ECAL_${label}.slcio.tar.gz ECAL_${label}.slcio 
#rm ${local}/log/errors_${condorfile}* ${local}/log/outfile_${condorfile}*
EOF
    
    cd ${local}/steer/
    /opt/exp_soft/cms/t3/t3submit -short $condorsh
    #rm ${condorsh} ${condorsub}
    cd -
    #break
  done
done

