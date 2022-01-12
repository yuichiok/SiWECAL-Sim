 
from DDSim.DD4hepSimulation import DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
SIM.numberOfEvents = 3

SIM.skipNEvents = 0
#SIM.outputFile = "/lhome/ific/a/airqui/SiWECAL/SiWECAL-Sim/generation/run_scripts/realisticbeam/data/ECAL_QGSP_BERT_conf0_e+_3GeV_0.slcio"
SIM.outputFile = "/data_ilc/flc/jimenez/simulations/TB2021/data/ECAL_QGSP_BERT_conf1_e-_3GeV_0.slcio"
#SIM.compactFile = "/lhome/ific/a/airqui/SiWECAL/SiWECAL-Sim/generation/geometry//ECAL_CONF0.xml"
SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry_TB2021/ECAL_CONF1.xml"
#SIM.dumpSteeringFile = "/lhome/ific/a/airqui/SiWECAL/SiWECAL-Sim/generation/run_scripts/TB2021/steer/dumpSteering.xml"
SIM.dumpSteeringFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True

