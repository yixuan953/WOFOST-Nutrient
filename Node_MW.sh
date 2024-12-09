#!/bin/bash
#-----------------------------Mail address-----------------------------
#SBATCH --mail-user=mengru.wang@wur.nl
#SBATCH --mail-type=FAIL
#-----------------------------Output files-----------------------------
#SBATCH --output=output_%j.txt
#SBATCH --error=error_output_%j.txt
#-----------------------------Other information------------------------


#-----------------------------Required resources-----------------------
#SBATCH --time=100000
#SBATCH --mem=16000


#-----------------------------Environment, Operations and Job steps----
#load modules
./wofost-nwlp list_GFDL-ESM4_wwh-hist.txt meteolist_GFDL-ESM4_hist.txt &
./wofost-nwlp list_GFDL-ESM4_wwh-fut.txt meteolist_GFDL-ESM4_ssp585.txt &
./wofost-nwlp list_IPSL-CM6A-LR_wwh-hist.txt meteolist_IPSL-CM6A-LR_hist.txt &
./wofost-nwlp list_IPSL-CM6A-LR_wwh-fut.txt meteolist_IPSL-CM6A-LR_ssp585.txt &
./wofost-nwlp list_MPI-ESM1-2-HR_wwh-hist.txt meteolist_MPI-ESM1-2-HR_hist.txt &
./wofost-nwlp list_MPI-ESM1-2-HR_wwh-fut.txt meteolist_MPI-ESM1-2-HR_ssp585.txt &
./wofost-nwlp list_MRI-ESM2-0_wwh-hist.txt meteolist_MRI-ESM2-0_hist.txt &
./wofost-nwlp list_MRI-ESM2-0_wwh-fut.txt meteolist_MRI-ESM2-0_ssp585.txt &
./wofost-nwlp list_UKESM1-0-LL_wwh-hist.txt meteolist_UKESM1-0-LL_hist.txt &
./wofost-nwlp list_UKESM1-0-LL_wwh-fut.txt meteolist_UKESM1-0-LL_ssp585.txt

