from DDSim.DD4hepSimulation import DD4hepSimulation
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "run"
SIM.numberOfEvents = 1

SIM.skipNEvents = 0
#SIM.outputFile = "/data_ilc/flc/jimenez/simulations/realisticbeam/data/ECAL_QGSP_BERT_conf1_e+_3GeV_0.slcio"
SIM.outputFile = "/data_ilc/flc/jimenez/simulations/TB2017-06/CONF1/lcio/ECAL_QGSP_BERT_conf1_e+_3GeV_0.slcio"

#SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry/ECAL_CONF1.xml"
#SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry/TB2017-06/ECAL_CONF1.xml"
SIM.compactFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/geometry/TB2017-06/ECAL_CONF1_newGeo.xml"
#SIM.dumpSteeringFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/realisticbeam/steer/dumpSteering.xml"
#SIM.dumpSteeringFile = "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/realisticbeam/steer/dumpSteering.xml"
SIM.dumpSteeringFile = "/grid_mnt/vol_home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/generation/run_scripts/TB2017-06/dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True
