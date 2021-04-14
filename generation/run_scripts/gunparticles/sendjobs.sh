#!/bin/bash

for particle in "e+" 
do
    energy=3
    conf=0
    source generic_condor.sh $particle $energy $conf
    for energy in 1 2 3 4 5 5.8
    do
	for conf in 1 2 3
	do
	    source generic_condor.sh $particle $energy $conf
	done
    done
done

for particle in "mu-"
do
    for energy in 0.4 4 40 400
    do
        for conf in 0 1 2 3
        do
            source generic_condor.sh $particle $energy $conf
        done
    done
done

	    
