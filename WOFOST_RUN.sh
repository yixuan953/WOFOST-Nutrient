#!/bin/bash
#-----------------------------Mail address-----------------------------
#SBATCH --mail-user=yixuan.zhou@wur.nl
#SBATCH --mail-type=FAIL
#-----------------------------Output files-----------------------------
#SBATCH --output=/lustre/nobackup/WUR/ESG/zhou111/WOFOST-NPcycling/HPC_Report/output_%j.txt
#SBATCH --error=/lustre/nobackup/WUR/ESG/zhou111/WOFOST-NPcycling/HPC_Report/error_output_%j.txt
#-----------------------------Other information------------------------

#-----------------------------Required resources-----------------------
#SBATCH --time=200
#SBATCH --mem=250000

#-----------------------------Environment, Operations and Job steps----
#load modules
module load netcdf
#----------------------------Run WOFOST simulations-----------------------
# echo "Current directory: $(pwd)"

# ./wofost-NP list_wnl_Rhine_maize.txt meteolist_WFDE5_Rhine_maize.txt fertlist_Rhine_maize.txt irrilist_Rhine_maize.txt
# ./wofost-NP list_wnl_Yangtze_maize.txt meteolist_WFDE5_Yangtze_maize.txt fertlist_Yangtze_maize.txt irrilist_Yangtze_maize.txt
# ./wofost-NP list_wnl_Indus_maize.txt meteolist_WFDE5_Indus_maize.txt fertlist_Indus_maize.txt irrilist_Indus_maize.txt
# ./wofost-NP list_wnl_LaPlata_maize.txt meteolist_WFDE5_LaPlata_maize.txt fertlist_LaPlata_maize.txt irrilist_LaPlata_maize.txt

./wofost-NP list_wnl_Yangtze_mainrice.txt meteolist_WFDE5_Yangtze_mainrice.txt fertlist_Yangtze_mainrice.txt irrilist_Yangtze_mainrice.txt
#./wofost-NP list_wnl_Indus_mainrice.txt meteolist_WFDE5_Indus_mainrice.txt fertlist_Indus_mainrice.txt irrilist_Indus_mainrice.txt


# ./wofost-NP list_wnl_LaPlata_soybean.txt meteolist_WFDE5_LaPlata_soybean.txt fertlist_LaPlata_soybean.txt irrilist_LaPlata_soybean.txt

# ./wofost-NP list_wnl_Yangtze_winterwheat.txt meteolist_WFDE5_Yangtze_winterwheat.txt fertlist_Yangtze_winterwheat.txt irrilist_Yangtze_winterwheat.txt

# ./wofost-NP list_Yangtze_secondrice.txt meteolist_WFDE5_Yangtze_secondrice.txt fertlist_Yangtze_secondrice.txt irrilist_Yangtze_secondrice.txt

# ./wofost-NP list_Yangtze_soybean.txt meteolist_WFDE5_Yangtze_soybean.txt fertlist_Yangtze_soybean.txt irrilist_Yangtze_soybean.txt
# ./wofost-NP list_LaPlata_soybean.txt meteolist_WFDE5_LaPlata_soybean.txt fertlist_LaPlata_soybean.txt irrilist_LaPlata_soybean.txt

# ./wofost-NP list_Yangtze_maize.txt meteolist_WFDE5_Yangtze_maize.txt fertlist_Yangtze_maize.txt irrilist_Yangtze_maize.txt
# ./wofost-NP list_LaPlata_maize.txt meteolist_WFDE5_LaPlata_maize.txt fertlist_LaPlata_maize.txt irrilist_LaPlata_maize.txt
# ./wofost-NP list_Indus_maize.txt meteolist_WFDE5_Indus_maize.txt fertlist_Indus_maize.txt irrilist_Indus_maize.txt
# ./wofost-NP list_Rhine_maize.txt meteolist_WFDE5_Rhine_maize.txt fertlist_Rhine_maize.txt irrilist_Rhine_maize.txt

# ./wofost-NP list_Yangtze_winterwheat.txt meteolist_WFDE5_Yangtze_winterwheat.txt fertlist_Yangtze_winterwheat.txt irrilist_Yangtze_winterwheat.txt
# ./wofost-NP list_LaPlata_winterwheat.txt meteolist_WFDE5_LaPlata_winterwheat.txt fertlist_LaPlata_winterwheat.txt irrilist_LaPlata_winterwheat.txt
# ./wofost-NP list_Indus_winterwheat.txt meteolist_WFDE5_Indus_winterwheat.txt fertlist_Indus_winterwheat.txt irrilist_Indus_winterwheat.txt
# ./wofost-NP list_Rhine_winterwheat.txt meteolist_WFDE5_Rhine_winterwheat.txt fertlist_Rhine_winterwheat.txt irrilist_Rhine_winterwheat.txt