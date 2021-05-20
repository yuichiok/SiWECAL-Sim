#!/bin/bash

particle="e-"
energy=3
conf=0
source generic_condor.sh $particle $energy $conf "grid58_e3GeV.mac"

particle="e+"
source generic_condor.sh $particle $energy $conf "grid58_p3GeV.mac"

particle="mu-" 
energy=0.4
source generic_condor.sh $particle $energy $conf "grid58_mu04GeV.mac"

energy=4
source generic_condor.sh $particle $energy $conf "grid58_mu4GeV.mac"

energy=40
source generic_condor.sh $particle $energy $conf "grid58_mu40GeV.mac"

energy=400
source generic_condor.sh $particle $energy $conf "grid58_mu400GeV.mac"




	    
