#!/bin/bash

START_TIME=$(date +%s)

typos=( "wlp" )
periods=( "f" "h" )
soil="ec3"

mkdir -p ../Output

for typo in ${typos[@]}
do
	mkdir -p ../Output
	for period in ${periods[@]}
	do
		# run the first x  simulations in the background
		for nr in {1..30}
		do
			# change soil type
			sed -i "s/ec3/$soil/g" list-$period$nr.txt
			
			#run wofost
			./wofost-$typo list-$period$nr.txt meteo-$period.txt > /dev/null &
			
			# reset soil type
			sed -i "s/$soil/ec3/g" list-$period$nr.txt
			
			# wait until the number of simulations drops below x
			while [ ` ps axg | grep wofost-$typo | wc -l `  -eq "7" ] 
			do 
				sleep 5
			done
		done

		# wait untill al process have finished
		while [ ` ps axg | grep wofost-$typo | wc -l `  -gt "1" ] 
		do
			sleep 5
		done
	done
	mv ../Output ../Output-$typo-$soil
done

echo $typo $period  "Total time elapsed: $(date -ud "@$(($(date +%s) - $START_TIME))" +%T) (HH:MM:SS)"






