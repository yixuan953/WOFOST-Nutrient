#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"
#include "npcycling.h"


/*----------------------------------------------------------*/
/* function CalNutriAvail() [kg/ha]                         */
/* Purpose: Calculate the N, P availability for crop        */
/*----------------------------------------------------------*/

void CalNutriAvail() {
    float corr_para_P_uptake = 25.0;
    // The daily P availability is depend on the transpiration [cm] and P concentration [g/m3] in the soil solution
    NPC->st_P_avail = corr_para_P_uptake * NPC->p_st.cP_inorg * WatBal->rt.Transpiration * 0.1;

    // The daily N availability is dependent on deposition, decomposition, and the unsed fertilizer input
    NPC->st_N_avail = NPC->st_N_avail + NPC->N_fert_input + N_total_dep[Lon][Lat][Day] + NPC->decomp_rt.SON_decomp - Crop->N_rt.Uptake;

    // To calculate the annual balance
    NPC->n_st.N_dep += N_total_dep[Lon][Lat][Day]; 
    NPC->p_st.P_dep += P_total_dep[Lon][Lat][Day];

}

/*----------------------------------------------------------*/
/* function CalGaseousEmissions() [kg/ha]                   */
/* Purpose: Calculate N losses through gaseous emissions    */
/*----------------------------------------------------------*/

void CalGaseousEmissions() {
    NPC->n_st.Emission_N2O = (Urea_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] + Other_inorg_N_appRate[Lon][Lat][Crop->Seasons-1]) * EF_N2O_Inorg + 
                             (Manure_N_appRate[Lon][Lat][Crop->Seasons-1] + NPC->N_residue_afterHavest + NPC->N_residue_beforeSowing)* EF_N2O_Org;

    NPC->n_st.Emission_NH3 = Urea_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] * EF_NH3_Urea + 
                             Other_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] * EF_NH3_Inorg +
                             Manure_N_appRate[Lon][Lat][Crop->Seasons-1] * EF_NH3_Manure;
    
    NPC->n_st.Emission_NOx = (Manure_N_appRate[Lon][Lat][Crop->Seasons-1] + Urea_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] + Other_inorg_N_appRate[Lon][Lat][Crop->Seasons-1]) * EF_NOx[Lon][Lat][Crop->Seasons-1];

}

/*------------------------------------------------------*/
/* function CalNSurfRunoff() [kg/ha]                    */
/* Purpose: Calculate N losses through surface runoff   */
/*------------------------------------------------------*/

void CalNSurfRunoff() {
    NPC->n_st.N_loss_surf = (Manure_N_appRate[Lon][Lat][Crop->Seasons-1] 
        + Urea_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] 
        + Other_inorg_N_appRate[Lon][Lat][Crop->Seasons-1]) * L_runoff_max * min(f_precip_surf,f_texture); 
}


/*------------------------------------------------------*/
/* function CalNLeaching() [kg/ha]                      */
/* Purpose: Calculate N losses through leaching         */
/*------------------------------------------------------*/

void CalNLeaching() {
    f_root = 0.75;
    float Total_leach;
    Total_leach = NPC->n_st.N_surplus * L_leaching_max * min(f_precip_leaching, min(min(f_root, f_soc), f_temp));
    NPC->n_st.N_loss_leach = Total_leach * f_groundwater;
    NPC->n_st.N_loss_sub = Total_leach * (1-f_groundwater);
}

/*------------------------------------------------------------------------------------------------*/
/* function CalNSurplus() [kg/ha]                                                                 */
/* Purpose: Calculate N surplus after uptake, surface & subsurface runoff, and gaserous emissions */
/*------------------------------------------------------------------------------------------------*/

void CalNSurplus() {
     NPC->n_st.N_surplus = Manure_N_appRate[Lon][Lat][Crop->Seasons-1] + NPC->N_residue_beforeSowing + NPC->N_residue_afterHavest +
                           Urea_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] + Other_inorg_N_appRate[Lon][Lat][Crop->Seasons-1] + 
                           NPC->n_st.N_decomp + NPC->n_st.N_dep + NPC->n_st.N_fixation - 
                           NPC->n_st.Emission_N2O - NPC->n_st.Emission_NH3 - NPC->n_st.Emission_NOx -
                           Crop->N_st.Uptake - NPC->n_st.N_loss_surf;                             
}


/*----------------------------------------------------------*/
/* function CalDenitrification() [kg N/ha]                  */
/* Purpose: Calculate N denitrification (N2)                */
/*----------------------------------------------------------*/

void CalDenitrification() {
    NPC->n_st.N_loss_N2 = NPC->n_st.N_surplus - NPC->n_st.N_loss_leach;
}


/*------------------------------------------------------------*/
/* function CalNBalance()                             [kg/ha] */
/* Purpose: Integrate all of the N inputs, uptakes and losses */
/*------------------------------------------------------------*/

void CalNBalance(){
    CalLeachingFactors();

    CalGaseousEmissions();
    CalNSurfRunoff(); 

    CalNSurplus();
    CalNLeaching();
    CalDenitrification();
}

void InitilizeNPBalance(){

    NPC->st_N_avail = 0.;
    NPC->n_st.N_fert_input = 0.0;
    NPC->N_fert_input = 0.0;
    NPC->N_residue_afterHavest = 0.0;
    NPC->N_residue_beforeSowing = 0.0;

    NPC->n_st.Emission_N2O = 0.;
    NPC->n_st.Emission_NH3 = 0.;
    NPC->n_st.Emission_NOx = 0.;

    NPC->n_st.N_loss_surf = 0.;
    NPC->n_st.N_loss_sub = 0.;
    NPC->n_st.N_loss_leach = 0.;

    NPC->n_st.N_fixation = 0.;
    NPC->n_st.N_decomp = 0.;
    NPC->n_st.N_dep = 0.;

    NPC->n_st.N_surplus = 0.;
    NPC->n_st.N_loss_N2 = 0.;

    NPC->st_P_avail = 0.;
    NPC->p_st.P_fert_input = 0.0;
    NPC->P_fert_input = 0.0;

    NPC->p_st.PSubRunoff = 0.0;
    NPC->p_st.PSurfRunoff = 0.0;
    NPC->p_st.PLeaching = 0.0;

    NPC->p_st.P_decomp = 0.0;
    NPC->p_st.P_dep = 0.0;
    NPC->P_residue_afterHavest = 0.0;
    NPC->P_residue_beforeSowing = 0.0;

}
