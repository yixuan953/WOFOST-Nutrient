#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "wofost.h"

void header(FILE *fp)
{
    fprintf(fp,"Lat,Lon,Year,Day,Storage,N_residue_uptake,P_residue_uptake,Rain,Inf,Perc,Trans,Loss,Runoff,RM,N_uptake,P_uptake,N_demand,P_demand,N_avil,P_avail\n");
}

void Output(FILE *fp)
{      
         fprintf(fp,"%7.2f\t%7.2f\t%4d\t%3d" // Lat, Lon, MeteoYear, MeteoDay
                "\t%4.2f\t%4.2f\t%4.2f" // Crop stage
                "\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f" // Water balance
                "\t%4.2f\t%4.2f\t%4.2f\t%4.2f" //N,P uptake and demand
                "\t%4.2f\t%4.2f\n", //Site conditions
               Latitude[Lat],
               Longitude[Lon],
               MeteoYear[Day],
               MeteoDay[Day],
        //Crop->st.Development,
        //Crop->st.leaves,
        //Crop->st.stems,
               Crop->st.storage,
               Crop->N_rt.Uptake_lv + Crop->N_rt.Uptake_st + Crop->N_rt.Uptake_ro,
               Crop->P_rt.Uptake_lv + Crop->P_rt.Uptake_st + Crop->P_rt.Uptake_ro, 
        //Crop->st.roots,
        //Crop->st.LAI,
               Rain[Lon][Lat][Day],
               // WatBal->WaterStress,
               // WatBal->st.Moisture,
               WatBal->rt.Infiltration,
               WatBal->rt.Percolation,
               WatBal->rt.Transpiration,
               WatBal->rt.Loss,
               WatBal->rt.Runoff,
               WatBal->st.RootZoneMoisture,
//        Crop->N_st.Indx,
//        Crop->P_st.Indx,
//        Crop->K_st.Indx,
//        Site->st_N_tot,
//        Site->st_P_tot,
//        Site->st_K_tot,
               Crop->N_rt.Uptake,
               Crop->P_rt.Uptake,
//        Crop->K_rt.Uptake,
               Crop->N_rt.Demand_lv + Crop->N_rt.Demand_st + Crop->N_rt.Demand_ro,
               Crop->P_rt.Demand_lv + Crop->P_rt.Demand_st + Crop->P_rt.Demand_ro,
//        Crop->K_rt.Demand_lv + Crop->K_rt.Demand_st + Crop->K_rt.Demand_ro);

               Site->st_N_tot,
               Site->st_P_tot);
}