#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"

void header_AO(FILE *fp)
{
    fprintf(fp,"Lat,Lon,Year,Day,Storage,GrowthDay,N_Uptake,P_Uptake,N_grain,P_grain,N_residue,P_Residue\n");
}

void Output_Annual(FILE *fp)
{      
         fprintf(fp,"%7.2f\t%7.2f\t%4d\t%3d" // Lat, Lon, MeteoYear, MeteoDay
                "\t%4.2f\t%4.2d" // Crop stage
                "\t%4.2f\t%4.2f" //N,P uptake
                "\t%4.2f\t%4.2f\t%4.2f\t%4.2f\n",//N,P content in grains and residues
               Latitude[Lat],
               Longitude[Lon],
               MeteoYear[Day],
               MeteoDay[Day],
               Crop->st.storage, // Crop yield
               Crop->GrowthDay,
               Crop->N_st.Uptake,
               Crop->P_st.Uptake,
               Crop->N_st.storage,//For grains
               Crop->P_st.storage,//For grains
               Crop->st.leaves * Crop->prm.N_ResidualFrac_lv + Crop->st.stems * Crop->prm.N_ResidualFrac_st,
               Crop->st.leaves * Crop->prm.P_ResidualFrac_lv + Crop->st.stems * Crop->prm.P_ResidualFrac_st);
}