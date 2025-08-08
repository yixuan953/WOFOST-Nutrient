#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"
#include "npcycling.h"

/* ---------------------------------------------------------------------------------*/
/*  function CalSoilTexturePara()                                                   */
/*  Purpose: Calculate soil texture related parameters for N, P cycling             */
/* ---------------------------------------------------------------------------------*/

void CalSoilTexturePara()
{  
   if (texture_class[Lon][Lat] < 4 || texture_class[Lon][Lat] == 8)       // Clay
   {
    KL = 2000;
    miu_SDis = 0.000044;
    L_leaching_max = 0.5;
   } 
   
   else if (texture_class[Lon][Lat] > 11)                                 // Sand
   {
    KL = 1000;
    miu_SDis = 0.000002;
    L_leaching_max = 1.0;
   }
   
   else                                                                   // Loam
   {
    KL = 1500;
    miu_SDis = 0.000044;
    L_leaching_max = 0.75;
   }
}

/* --------------------------------------------------------------------------------*/
/*  function CalEmissionFactor()                                                   */
/*  Purpose: Calculate emission factors for N gaseous emission simulations         */
/* --------------------------------------------------------------------------------*/

void CalEmissionFactor()
{  
   // EF_NOx is dependent on annual precipitation and is saved with the fertilizer .nc files

   // NH3 emission factors depend on fertilizer types
   EF_NH3_Urea   = 0.15;
   EF_NH3_Inorg  = 0.02;
   EF_NH3_Manure = 0.20;

   // N2O emission factors depend on annual precipitatio
   if (Crop->Seasons > 1)   // Then we will use the water balance from last year
   {
      if (WatBal->st.PreSurplus > 0)                                  // P + Irr > ET: Moist climate
      {
         EF_N2O_Inorg = 0.016;
         EF_N2O_Org = 0.006;
      } else                                                          // P + Irr <= ET: Dry climate 
      {
         EF_N2O_Inorg = 0.005;
         EF_N2O_Org = 0.005;         
      }
   } else{
      if (Climate_Zone[Lon][Lat] == 1 || Climate_Zone[Lon][Lat] == 3) // P > ET: Moist climate
      {
         EF_N2O_Inorg = 0.016;
         EF_N2O_Org = 0.006;
      } else                                                          // P <= ET: Dry climate
      {
         EF_N2O_Inorg = 0.005;
         EF_N2O_Org = 0.005;         
      }

   }  

}

/* ---------------------------------------------------------------------------------*/
/*  function CalRunoffFactors()                                                     */
/*  Purpose: Calculate factor related to N losses through the surface runoff        */
/* ---------------------------------------------------------------------------------*/
void CalRunoffFactors(){

   // L_runoff_max
   if (slope[Lon][Lat]<0.08)       // 8%
   {
      L_runoff_max = 0.10;
   } else if (slope[Lon][Lat]<0.15){
      L_runoff_max = 0.20;
   } else if (slope[Lon][Lat]<0.25){
      L_runoff_max = 0.35;
   } else {
      L_runoff_max = 0.50;
   }

   // f_precip_surplus 
   if (WatBal->st.PreSurplus < 5) // 50 mm
   {
      f_precip_surf = 0.25;
   } else if (WatBal->st.PreSurplus < 10){
      f_precip_surf = 0.50;
   } else if (WatBal->st.PreSurplus < 30){
      f_precip_surf = 0.75;
   } else {
      f_precip_surf = 1.0;
   }

   // f_texture
   if (clay_content[Lon][Lat] < 18) // %
   {
      f_texture = 0.25;
   } else if (clay_content[Lon][Lat] < 34){
      f_texture = 0.75;
   } else if (clay_content[Lon][Lat] < 60){
      f_texture = 0.9;
   } else {
      f_texture = 1.0;
   }

}


/* ---------------------------------------------------------------------------*/
/*  function CalLeachingactors()                                              */
/*  Purpose: Calculate factor related to N losses through the leaching        */
/* ---------------------------------------------------------------------------*/
void CalLeachingFactors(){
   // L_leaching_max i calculated in the the function CalSoilTexturePara()

   // f_precip_leaching 
   if (texture_class[Lon][Lat] < 4 || texture_class[Lon][Lat] == 8) { // Clay
      if (WatBal->st.PreSurplus < 5) // 50 mm
      {
         f_precip_leaching = 0.25;
      } else if (WatBal->st.PreSurplus < 10){
         f_precip_leaching = 0.75;
      } else if (WatBal->st.PreSurplus < 30){
         f_precip_leaching = 1.0;
      } else {
         f_precip_leaching = 0.5;
      }
   } else {
      if (WatBal->st.PreSurplus < 5) // 50 mm                        // Sand & Loam
      {
         f_precip_leaching = 0.25;
      } else if (WatBal->st.PreSurplus < 10){
         f_precip_leaching = 0.50;
      } else if (WatBal->st.PreSurplus < 30){
         f_precip_leaching = 0.75;
      } else {
         f_precip_leaching = 1.0;
      }
   }

   // f_temp
   if (AveTemp < 5) {f_temp = 1.0;} 
   else if (AveTemp < 15) {f_temp = 0.75;} 
   else {f_temp = 0.5;}

   // f_soc
   if (SOC[Lon][Lat] < 1.0) // %
   {
      f_soc = 1.0;
   } else if (SOC[Lon][Lat] < 2.0){
      f_soc = 0.9;
   } else if (SOC[Lon][Lat] < 6.0){
      f_soc = 0.75;
   } else {
      f_soc = 0.5;
   }
}