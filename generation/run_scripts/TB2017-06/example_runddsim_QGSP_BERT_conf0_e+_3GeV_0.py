from DDSim.DD4hepSimulation import DD4hepSimulation
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
SIM.numberOfEvents = 1

SIM.skipNEvents = 0
SIM.outputFile = "/data_ilc/flc/jimenez/simulations/realisticbeam/data/ECAL_QGSP_BERT_conf0_e+_3GeV_0.slcio"

SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry/ECAL_CONF0.xml"
SIM.dumpSteeringFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/realisticbeam/steer/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True
