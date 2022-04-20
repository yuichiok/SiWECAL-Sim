 
from DDSim.DD4hepSimulation import DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
SIM.numberOfEvents = 100

SIM.skipNEvents = 0
SIM.outputFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2022/data/examples/ECAL_QGSP_BERT_conf1_e-_3GeV_0.slcio"
SIM.compactFile = "/grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry_TB2021/conf1_2022.xml"
SIM.dumpSteeringFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2021/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True

