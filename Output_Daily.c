#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"

void header_DO(FILE *fp_do)
{
    fprintf(fp_do,"Lat,Lon,Year,Day,Dev_Stage,Transpiration,EvaWater,EvaSoil,SoilMoisture,SurfaceRunoff,SubsurfaceRunoff,Percolation,TSMD,N_demand,P_demand,N_Uptake,P_Uptake,Lpool,Spool,Prepool,P_avail,P_Surf,P_Sub,P_Leaching\n");
}

void Output_Daily(FILE *fp_do)
{      
         fprintf(fp_do,"%7.2f,%7.2f,%4d,%3d," // Lat, Lon, MeteoYear, MeteoDay
                "%4.2f," // Crop stage
                "%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f," // Water demand
                "%4.2f,%4.2f,"   //N,P demand
                "%4.2f,%4.2f,"   //N,P uptake
                "%4.2f,%4.2f,%4.2f,%4.2f,"  //Soil P pools + availability
                "%4.2f,%4.2f,%4.2f\n",  //P losses
                Latitude[Lat],
                Longitude[Lon],
                MeteoYear[Day],
                MeteoDay[Day],
                Crop->st.Development,
                WatBal->rt.Transpiration,
                WatBal->rt.EvapWater,
                WatBal->rt.EvapSoil,
                WatBal->st.Moisture,
                WatBal->rt.Runoff,
                WatBal->rt.Loss,
                WatBal->rt.Percolation,
                WatBal->st.TSMD,
                Crop->N_rt.Demand_lv + Crop->N_rt.Demand_st + Crop->N_rt.Demand_ro,
                Crop->P_rt.Demand_lv + Crop->P_rt.Demand_st + Crop->P_rt.Demand_ro,
                Crop->N_rt.Uptake,
                Crop->P_rt.Uptake,
                NPC->p_st.LabileP,
                NPC->p_st.StableP,
                NPC->p_st.PrecP,
                NPC->st_P_avail,
                NPC->p_rt.PSurfRunoff,
                NPC->p_rt.PSubRunoff,
                NPC->p_rt.PLeaching);
}