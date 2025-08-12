#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"

void header_AO(FILE *fp_ao)
{
    fprintf(fp_ao,"Lat,Lon,Year,Day,Storage,GrowthDay,N_decomp,N_dep,N_fix,N_fert,NH3,N2O,NOx,N2,N_surf,N_sub,N_leach,N_uptake,P_decomp,P_dep,P_fert,LabileP,StableP,PrecP,P_surf,P_sub,P_LEACH,P_uptake\n");
}

void Output_Annual(FILE *fp_ao)
{      
         fprintf(fp_ao,"%7.2f,%7.2f,%4d,%3d,"         // Lat, Lon, MeteoYear, MeteoDay
                "%4.2f, %4.2d,"                       // Crop yield and GrowthDays
                "%4.2f, %4.2f, %4.2f, %4.2f,"         // N inputs
                "%4.2f, %4.2f, %4.2f, %4.2f,"         // N losses through gaseous emission
                "%4.2f, %4.2f, %4.2f, %4.2f,"         // N losses through water fluxes and uptakes 
                "%4.2f, %4.2f, %4.2f,"                // P inputs
                "%4.2f, %4.2f, %4.2f,"                // P pools
                "%4.2f, %4.2f, %4.2f, %4.2f\n",        // P losses through water fluxes and uptakes 
               Latitude[Lat],
               Longitude[Lon],
               MeteoYear[Day],
               MeteoDay[Day],
               // Yield and growing days
               Crop->st.storage, // Crop yield = Grid->twso[Crop->GrowthDay]
               Crop->GrowthDay,
               // N balance
               NPC->n_st.N_decomp,NPC->n_st.N_dep,NPC->n_st.N_fixation,NPC->n_st.N_fert_input,
               NPC->n_st.Emission_NH3,NPC->n_st.Emission_N2O,NPC->n_st.Emission_NOx,NPC->n_st.N_loss_N2,
               NPC->n_st.N_loss_surf,NPC->n_st.N_loss_sub,NPC->n_st.N_loss_leach,Crop->N_st.Uptake,
               // P dynamics
               NPC->p_st.P_decomp,NPC->p_st.P_dep,NPC->p_st.P_fert_input,
               NPC->p_st.LabileP,NPC->p_st.StableP,NPC->p_st.PrecP,
               NPC->p_st.PSurfRunoff,NPC->p_st.PSubRunoff,NPC->p_st.PLeaching,Crop->P_st.Uptake);
}