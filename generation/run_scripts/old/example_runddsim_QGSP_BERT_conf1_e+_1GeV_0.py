 
from DDSim.DD4hepSimulation import DD4hepSimulation
#from SystemOfUnits import mm, GeV, MeV
from g4units import GeV, mm, MeV

SIM = DD4hepSimulation()

SIM.runType = "batch"
SIM.numberOfEvents = 10

SIM.skipNEvents = 0
SIM.outputFile = "ECAL_QGSP_BERT_conf1_e+_1GeV_0.slcio"

SIM.compactFile = "/lhome/ific/a/airqui/SiWECAL/SiWECAL-Sim/generation/geometry//ECAL_CONF1.xml"
SIM.dumpSteeringFile = "dumpSteering.xml"

SIM.field.eps_min = 1*mm
SIM.part.minimalKineticEnergy = 0.3*MeV
SIM.physicsList = "QGSP_BERT"
SIM.enableDetailedShowerMode=True

SIM.enableGun = True
SIM.gun.energy = 1*GeV
SIM.gun.particle = "e+"
SIM.gun.position = "45*mm,45*mm,-1000*mm"
#SIM.gun.isotrop
SIM.gun.direction = "0,0,1"

