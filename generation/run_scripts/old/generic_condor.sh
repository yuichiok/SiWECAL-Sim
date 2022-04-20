particle=$1
energy=$2
conf=$3

geometry_folder="/lhome/ific/a/airqui/SiWECAL/SiWECAL-Sim/generation/geometry/"
ilcsoft_path="/cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-01/"
local=$PWD

physl=("QGSP_BERT" "FTFP_BERT")


nevt=2

#for energy in ${ens[@]}; do
for physlist in ${physl[@]}; do
for it in {0..1}; do


echo $conf $energy $particle $iter


label=${physlist}_conf${conf}_${particle}_${energy}GeV_${it}

echo $label

scriptname=runddsim_${label}.py
condorsh=runddsim_${label}.sh
condorsub=runddsim_${label}.sub 
condorfile=runddsim_${label}

if [ ! -e steer ]; then
    mkdir steer
fi
if [ ! -e data ]; then
    mkdir data
fi
if [ ! -e log ]; then
    mkdir log
fi

cat > ${local}/steer/$scriptname <<EOF
 
from DDSim.DD4hepSimulation import DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "batch"
SIM.numberOfEvents = $nevt

SIM.skipNEvents = 0
SIM.outputFile = "${local}/data/ECAL_${label}.slcio"

SIM.compactFile = "${geometry_folder}/ECAL_CONF${conf}.xml"
SIM.dumpSteeringFile = "${local}/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "${physlist}"
SIM.enableDetailedShowerMode=True

SIM.enableGun = True
SIM.gun.energy = ${energy}*GeV
SIM.gun.particle = "${particle}"
SIM.gun.position = "45*mm,45*mm,-1000*mm"
#SIM.gun.isotrop
SIM.gun.direction = "0,0,1"

EOF

cat > ${local}/steer/$condorsh <<EOF
source ${ilcsoft_path}/init_ilcsoft.sh        
cp -r ${local}/steer/runddsim_${label}.* .

ddsim --steeringFile ${local}/steer/$scriptname 
#&> ${local}/log/${label}.log
#tar czvf ${local}/data/ECAL_${label}.slcio.tar.gz ECAL_${label}.slcio 
#rm ${local}/log/errors_${condorfile}* ${local}/log/outfile_${condorfile}*

EOF

cat > ${local}/steer/$condorsub <<EOF
# Unix submit description file
# kt_xNAMEfile.sub -- simple Marlin job

executable              = ${condorfile}.sh
log                     = ../log/${condorfile}.log
output                  = ../log/outfile_${condorfile}.txt
error                   = ../log/errors_${condorfile}.txt
should_transfer_files   = Yes
when_to_transfer_output = ON_EXIT
queue 1
EOF

cd ${local}/steer/
condor_submit $condorsub
#rm ${condorsh} ${condorsub}
cd -


done
done

