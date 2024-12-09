#!/bin/bash
#-----------------------------Mail address-----------------------------
#SBATCH --mail-user=yixuan.zhou@wur.nl
#SBATCH --mail-type=FAIL
#-----------------------------Output files-----------------------------
#SBATCH --output=home/WUR/zhou111/output_%j.txt
#SBATCH --error=home/WUR/zhou111/error_output_%j.txt
#-----------------------------Other information------------------------


#-----------------------------Required resources-----------------------
#SBATCH --time=200
#SBATCH --mem=250000


#-----------------------------Environment, Operations and Job steps----
#load modules
module load netcdf
#----------------------------Run WOFOST simulations-----------------------
# echo "Current directory: $(pwd)"
./wofost list_GFDL-ESM4_hist.txt meteolist_GFDL-ESM4_hist.txt 
#./wofost-pot list_GFDL-ESM4_ssp585.txt meteolist_GFDL-ESM4_ssp585.txt 
#./wofost-pot list_IPSL-CM6A-LR_hist.txt meteolist_IPSL-CM6A-LR_hist.txt 
#./wofost-pot list_IPSL-CM6A-LR_ssp585.txt meteolist_IPSL-CM6A-LR_ssp585.txt 
#./wofost-pot list_MPI-ESM1-2-HR_hist.txt meteolist_MPI-ESM1-2-HR_hist.txt 
#./wofost-pot list_MPI-ESM1-2-HR_ssp585.txt meteolist_MPI-ESM1-2-HR_ssp585.txt 
#./wofost-pot list_MRI-ESM2-0_hist.txt meteolist_MRI-ESM2-0_hist.txt 
#./wofost-pot list_MRI-ESM2-0_ssp585.txt meteolist_MRI-ESM2-0_ssp585.txt 
#./wofost-pot list_UKESM1-0-LL_hist.txt meteolist_UKESM1-0-LL_hist.txt 
#./wofost-pot list_UKESM1-0-LL_ssp585.txt meteolist_UKESM1-0-LL_ssp585.txt