#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"

void header_DO(FILE *fp_do)
{
    fprintf(fp_do,"Lat,Lon,Year,Day,Dev_Stage,tsum_emergence,Trans,EvaWater,EvaSoil,N_demand,P_demand\n");
}

void Output_Daily(FILE *fp_do)
{      
         fprintf(fp_do,"%7.2f\t%7.2f\t%4d\t%3d" // Lat, Lon, MeteoYear, MeteoDay
                "\t%4.2f\t%4.2f" // Crop stage, Crop tsum
                "\t%4.2f\t%4.2f\t%4.2f" // Water demand
                "\t%4.2f\t%4.2f\n", //N,P demand
                Latitude[Lat],
                Longitude[Lon],
                MeteoYear[Day],
                MeteoDay[Day],
                Crop->st.Development,
                Crop->TSumEmergence,
                WatBal->rt.Transpiration,
                //Penman.E0 * exp(-0.75 * KDiffuse * Crop->st.LAI),
                //max(0., Penman.ES0 * exp(-0.75 * KDiffuse * Crop->st.LAI)),
                WatBal->rt.EvapWater,
                WatBal->rt.EvapSoil,
                Crop->N_rt.Demand_lv + Crop->N_rt.Demand_st + Crop->N_rt.Demand_ro,
                Crop->P_rt.Demand_lv + Crop->P_rt.Demand_st + Crop->P_rt.Demand_ro);
}