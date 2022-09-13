from DDSim.DD4hepSimulation import  DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
# Number of events defined in macro file

SIM.skipNEvents = 0
SIM.outputFile = "/data_ilc/flc/jimenez/simulations/TB2022-06/CONF9/lcio/ECAL_QGSP_BERT_conf9_mu-_1GeV_0.slcio"

SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry//TB2022-06/ECAL_CONF9.xml"
SIM.dumpSteeringFile = "/grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2022-06/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True