#!/bin/bash
# If run on hpc this script has to be adapted
# This script is developed for a standalone linux server


START_TIME=$(date +%s)

typos=( "pot" "wlp" "nwlp" )
periods=( "ssp585" "hist" )
#periods=( "hist" )
model=( "IPSL-CM6A-LR" "MPI-ESM1-2-HR" "MRI-ESM2-0" "UKESM1-0-LL" "GFDL-ESM4")
#model=( "IPSL-CM6A-LR" )

for typo in ${typos[@]}
do
	mkdir -p ../Results/Output
	for period in ${periods[@]}
	do		
		for m in ${model[@]}
		do
			#run wofost
			echo ./wofost-$typo 'list_'$m'_'$period'.txt' 'meteolist_'$m'_'$period'.txt' 
			./wofost-$typo 'list_'$m'_'$period'.txt' 'meteolist_'$m'_'$period'.txt' > /dev/null &
				
		done
		while [ ` ps axg | grep wofost-$typo | wc -l `  -gt "1" ] 
		do 
			sleep 5
		done
	done
	mv ../Results/Output ../Results/Output'-'$typo
done

echo $typo $period  "Total time elapsed: $(date -ud "@$(($(date +%s) - $START_TIME))" +%T) (HH:MM:SS)"
