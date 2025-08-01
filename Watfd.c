#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "penman.h"
#include "wofost.h"
#include "npcycling.h"

/*------------------------------------------------*/
/* function InitializeWatBal                      */
/* Purpose: Initialize the water balance (WatBal) */
/*------------------------------------------------*/

void InitializeWatBal()
{ 
    float KDiffuse;
    float AssRootDepth = 10.;
    
    /* Crop Growth has not started yet */
    WatBal->SoilMaxRootingDepth = 0.;
    
    /* Assume no water stress at initialization */
    WatBal->WaterStress = 1.;
    
    /* Set the infiltration of the previous day to zero */
    WatBal->InfPreviousDay = 0.;
    
    /* Check initial soil moisture. It cannot be larger than the              */
    /* saturated soil moisture SoilMoistureSAT or smaller than SoilMoistureWP */
    /* Unit: [-]*/
    if (Site->MaxInitSoilM < WatBal->ct.MoistureWP)  
            Site->MaxInitSoilM = WatBal->ct.MoistureWP;
    if (Site->MaxInitSoilM > WatBal->ct.MoistureSAT) 
            Site->MaxInitSoilM = WatBal->ct.MoistureSAT;
    
    /* Set initial surface storage */
    WatBal->st.SurfaceStorage = Site->SurfaceStorage;
    
    /* Initial soil moisture for a rice crop, unit: -*/
    if (Crop->prm.Airducts) Site->MaxInitSoilM = WatBal->ct.MoistureSAT; 
    
    if(Crop->Sowing == 0){
        WatBal->st.Moisture = limit(WatBal->ct.MoistureWP, Site->MaxInitSoilM, 
                WatBal->ct.MoistureWP + Site->InitSoilMoisture/AssRootDepth);
    } else{
        WatBal->st.Moisture = limit(WatBal->ct.MoistureWP, Site->MaxInitSoilM, 
                WatBal->ct.MoistureWP + Site->InitSoilMoisture/Crop->st.RootDepth);
    }

    
    /* Initial moisture amount in rootable zone, unit: cm */
    if(Crop->Sowing == 0){
       WatBal->st.RootZoneMoisture = WatBal->st.Moisture * AssRootDepth;
    } else{
       WatBal->st.RootZoneMoisture = WatBal->st.Moisture * Crop->st.RootDepth;
    }   
    
    /*  Soil evaporation, days since last rain */
    WatBal->DaysSinceLastRain = 1.;
    if (WatBal->st.Moisture <= (WatBal->ct.MoistureWP + 
            0.5 * (WatBal->ct.MoistureFC - WatBal->ct.MoistureWP))) 
            WatBal->DaysSinceLastRain = 5.;
    
    /* Moisture amount between rooted zone and max.rooting depth, unit: cm  */
    if (Crop->Sowing == 0){
        WatBal->st.MoistureLOW  = limit (0., WatBal->ct.MoistureSAT
                *(Crop->prm.MaxRootingDepth - AssRootDepth), 
                Site->InitSoilMoisture + Crop->prm.MaxRootingDepth * WatBal->ct.MoistureWP - 
                WatBal->st.RootZoneMoisture);
    } else {
        WatBal->st.MoistureLOW  = limit (0., WatBal->ct.MoistureSAT
                *(Crop->prm.MaxRootingDepth - Crop->st.RootDepth), 
                Site->InitSoilMoisture + Crop->prm.MaxRootingDepth * WatBal->ct.MoistureWP - 
                WatBal->st.RootZoneMoisture);
    }

    /* Initial Precipitation surplus and TSMD */
    WatBal->st.PreSurplus = 0;
    WatBal->st.TSMD = 0;

    KDiffuse = Afgen(Crop->prm.KDiffuseTb, &(Crop->st.Development));
    WatBal->rt.EvapSoil = max(0., Penman.ES0 * exp(-0.75 * KDiffuse * Crop->st.LAI));
   
}

/*---------------------------------------------------*/
/* function RateCalulationWatBal                     */
/* Purpose: Calculate the rate of the WatBal struct  */
/*---------------------------------------------------*/


void RateCalulationWatBal() {
   
    float Available;
    float CMaxSoilEvap;
    float Perc1, Perc2;
    float WaterEq;
    float WELOW;
    int Irri_time_index; // As the time step of irrigation data is monthly instead of daily
    float RINPRE; // Preliminary infiltration rate 
    float AssRootDepth = 10.;
    
    Irri_time_index = ((Crop->Seasons-1) * 12 + MeteoMonth[Day])-1; 
    // ---- Compute IRRITimeStep ----
    if (WatBal->MoistureStress < 0.95 && Irri_time_count[Lon][Lat] != Irri_time_index) {    
        WatBal->rt.Irrigation = Irrigation_Rate[Lon][Lat][Irri_time_index];
        Irri_time_count[Lon][Lat] = Irri_time_index; // To mark that for this time step (monthly), irrigation has been applied
        
        /* Check if irrigation data is well input*/
        // printf("Irri_time_index=%d Lon=%d Lat=%d Season=%d Year=%d Month=%d DOY=%d MoistureStress=%.2f WatIrr=%.2f Irr=%.2f\n",
        //    Irri_time_index, Lon, Lat, Crop->Seasons, MeteoYear[Day], MeteoMonth[Day], MeteoDay[Day],
        //    WatBal->MoistureStress, WatBal->rt.Irrigation, Irrigation_Rate[Lon][Lat][Irri_time_index]);
    } else {
        WatBal->rt.Irrigation = 0.0;
    }
    // End of the new code

    /* How irrigation rate was filld before */
    // WatBal->rt.Irrigation = List(Mng->Irrigation);
    
    /* If surface storage > 1 cm  */
    if (WatBal->st.SurfaceStorage > 1.) 
    {
        WatBal->rt.EvapWater = Evtra.MaxEvapWater;
    }
    else 
    {
        if (WatBal->InfPreviousDay >= 1.) 
        {
            //If infiltration >= 1cm on previous day assume maximum soil evaporation
            WatBal->rt.EvapSoil = Evtra.MaxEvapSoil;
            WatBal->DaysSinceLastRain = 1.;
        }
        else 
        {
            WatBal->DaysSinceLastRain++;
            CMaxSoilEvap = Evtra.MaxEvapSoil*(sqrt(WatBal->DaysSinceLastRain) - 
                    sqrt(WatBal->DaysSinceLastRain - 1));
            WatBal->rt.EvapSoil = min(Evtra.MaxEvapSoil, CMaxSoilEvap + 
                    WatBal->InfPreviousDay);
        }
    }
    
    /* Preliminary infiltration rate */
    if (WatBal->st.SurfaceStorage <= 0.1) 
    {
        /* Without surface storage */
        if (Site->InfRainDependent) WatBal->rt.Infiltration = 
               (1. - Site->NotInfiltrating * Afgen(Site->NotInfTB, &Rain[Lon][Lat][Day])) * 
               Rain[Lon][Lat][Day] + WatBal->rt.Irrigation + WatBal->st.SurfaceStorage/ Step;
        else
            RINPRE = (1. - Site->NotInfiltrating) * Rain[Lon][Lat][Day] + 
                WatBal->rt.Irrigation + WatBal->st.SurfaceStorage/ Step;
    }
    else 
    {
        /* Surface storage, infiltration limited by maximum percolation */
        /* rate root zone */
        Available = WatBal->st.SurfaceStorage + (Rain[Lon][Lat][Day] * 
                (1.-Site->NotInfiltrating) + WatBal->rt.Irrigation 
                 - WatBal->rt.EvapSoil) * Step;
        RINPRE = min(WatBal->ct.MaxPercolRTZ * Step, 
                Available) / Step;
    }


    /* I added this to calculate the loss and percolation when no crop is planted*/
    if (Crop->Sowing == 0 || Crop->Emergence == 0){
        /* Equilibrium amount of soil moisture in rooted zone*/
        WaterEq = WatBal->ct.MoistureFC * AssRootDepth;
        WatBal->rt.Transpiration = 0;
        
        /* Percolation from rooted zone to subsoil equals amount of   */
        /* Excess moisture in rooted zone (not to exceed conductivity)*/
        Perc1 = limit(0., WatBal->ct.MaxPercolRTZ, 
                (WatBal->st.RootZoneMoisture - WaterEq) / Step - 
                WatBal->rt.Transpiration - WatBal->rt.EvapSoil);
        
        /* Loss of water at the lower end of the maximum root zone */
        /* Equilibrium amount of soil moisture below rooted zone   */
        WELOW = WatBal->ct.MoistureFC * (Crop->prm.MaxRootingDepth - AssRootDepth);
        WatBal->rt.Loss  = limit (0., WatBal->ct.MaxPercolSubS, 
                (WatBal->st.MoistureLOW - WELOW)/Step + Perc1);
        
        /* For rice water losses are limited to K0/20 */
        if (Crop->prm.Airducts) 
            WatBal->rt.Loss = min(WatBal->rt.Loss, 0.05*WatBal->ct.K0);
        
        /* Percolation not to exceed uptake capacity of subsoil */
        Perc2 = ((Crop->prm.MaxRootingDepth - AssRootDepth) * WatBal->ct.MoistureSAT - 
                WatBal->st.MoistureLOW) / Step + WatBal->rt.Loss;

    } else{

        /* Equilibrium amount of soil moisture in rooted zone*/
        WaterEq = WatBal->ct.MoistureFC * Crop->st.RootDepth;
        
        /* Percolation from rooted zone to subsoil equals amount of   */
        /* Excess moisture in rooted zone (not to exceed conductivity)*/
        Perc1 = limit(0., WatBal->ct.MaxPercolRTZ, 
                (WatBal->st.RootZoneMoisture - WaterEq) / Step - 
                WatBal->rt.Transpiration - WatBal->rt.EvapSoil);
        
        /* Loss of water at the lower end of the maximum root zone */
        /* Equilibrium amount of soil moisture below rooted zone   */
        WELOW = WatBal->ct.MoistureFC * (Crop->prm.MaxRootingDepth - Crop->st.RootDepth);
        WatBal->rt.Loss  = limit (0., WatBal->ct.MaxPercolSubS, 
                (WatBal->st.MoistureLOW - WELOW)/Step + Perc1);
        
        /* For rice water losses are limited to K0/20 */
        if (Crop->prm.Airducts) 
            WatBal->rt.Loss = min(WatBal->rt.Loss, 0.05*WatBal->ct.K0);
        
        /* Percolation not to exceed uptake capacity of subsoil */
        Perc2 = ((Crop->prm.MaxRootingDepth - Crop->st.RootDepth) * WatBal->ct.MoistureSAT - 
                WatBal->st.MoistureLOW) / Step + WatBal->rt.Loss;
    }
    
    WatBal->rt.Percolation = min(Perc1, Perc2);  
           
    
    /* Adjustment of the infiltration rate */
    if (Crop->Sowing == 0 && Crop->Emergence == 0){
        WatBal->rt.Infiltration = max(0, min(RINPRE,
        (WatBal->ct.MoistureSAT - WatBal->st.Moisture) * AssRootDepth/Step + 
        WatBal->rt.Transpiration + WatBal->rt.EvapSoil + WatBal->rt.Percolation));
    } else{
        WatBal->rt.Infiltration = max(0, min(RINPRE,
        (WatBal->ct.MoistureSAT - WatBal->st.Moisture) * Crop->st.RootDepth/Step + 
        WatBal->rt.Transpiration + WatBal->rt.EvapSoil + WatBal->rt.Percolation));        
    }
            
    /* Rates of change in amounts of moisture W and WLOW */
    WatBal->rt.RootZoneMoisture = -WatBal->rt.Transpiration - WatBal->rt.EvapSoil -  
            WatBal->rt.Percolation + WatBal->rt.Infiltration;
    WatBal->rt.MoistureLOW = WatBal->rt.Percolation - WatBal->rt.Loss;  
    
}


/*-----------------------------------------------------*/
/* function IntegrationWatBal                          */
/* Purpose: integrate the waterbalance rates over time */
/* and calculate the water content in the rooted zone  */
/*-----------------------------------------------------*/

void IntegrationWatBal() 
{
    float PreSurfaceStorage;
    float WaterRootExt;
    float AssRootDepth = 10.;
    float BundHeight = 15.;
    float DrainageHeight = 1.5;
    
    WatBal->st.Transpiration += WatBal->rt.Transpiration;
    WatBal->st.EvapWater     += WatBal->rt.EvapWater;
    WatBal->st.EvapSoil      += WatBal->rt.EvapSoil;
    
    WatBal->st.Rain += Rain[Lon][Lat][Day];
    WatBal->st.Infiltration += WatBal->rt.Infiltration;
    WatBal->st.Irrigation   += WatBal->rt.Irrigation;

    WatBal->st.PreSurplus = WatBal->rt.Irrigation + Rain[Lon][Lat][Day] - WatBal->rt.EvapWater - WatBal->rt.EvapSoil - WatBal->rt.Transpiration;
    
    if (Crop->Sowing > 1.0 && Crop->Emergence == 1.0){
        WatBal->st.TSMD = max(min(WatBal->st.TSMD,0), MaxTSMD) +  WatBal->st.PreSurplus;     // Vegetated soil  
    } else{
        WatBal->st.TSMD = max(min(WatBal->st.TSMD,0), MaxTSMD/1.8) +  WatBal->st.PreSurplus; // Bare soil
    }

    /* Surface storage and runoff */
    PreSurfaceStorage = WatBal->st.SurfaceStorage + (Rain[Lon][Lat][Day] + 
            WatBal->rt.Irrigation - WatBal->rt.EvapWater - 
            WatBal->rt.Infiltration) * Step;

    /* For rice, the MaxSurface Storage would change during the growing period*/
    if (Crop->prm.Airducts){
        if (Crop->st.Development < 0.01 || (Crop->st.Development > 0.49 && Crop->st.Development < 0.72) || Crop->st.Development > 1.6){
            Site->MaxSurfaceStorage = DrainageHeight;
            // The rice field is drained:
            // 1: When fertilizers are applied in the field before transplanting (Crop->Sowing == 0)
            // 2: In the end of tillering and before flowering (0.49 < DVS < 0.72)
            // 3: Before harvest (DVC > 1.8)
        } else{
            Site->MaxSurfaceStorage = BundHeight;
        }
    } 
    
    WatBal->st.SurfaceStorage = min(PreSurfaceStorage, Site->MaxSurfaceStorage);   

    WatBal->rt.Runoff = PreSurfaceStorage - WatBal->st.SurfaceStorage;
    WatBal->st.Runoff += WatBal->rt.Runoff;
    
    /* Water amount in the rooted zone */
    WatBal->st.RootZoneMoisture += WatBal->rt.RootZoneMoisture * Step;
    if (WatBal->st.RootZoneMoisture < 0.) 
    {
       WatBal->st.EvapSoil += WatBal->st.RootZoneMoisture;
       WatBal->st.RootZoneMoisture = 0.;
    }
    
    /* Total percolation and loss of water by deep leaching */
    WatBal->st.Percolation += WatBal->rt.Percolation * Step;
    WatBal->st.Loss        += WatBal->rt.Loss * Step;     
    
    WatBal->st.MoistureLOW += WatBal->rt.MoistureLOW;
   
    /* Change of root zone subsystem boundary                  */
    /* Calculation of amount of soil moisture in new root zone */
    if (Crop->st.RootDepth - Crop->st.RootDepth_prev > 0.001) 
    {
        
        /* water added to root zone by root growth, in cm   */
        WaterRootExt = WatBal->st.MoistureLOW *
                (Crop->st.RootDepth - Crop->st.RootDepth_prev) / 
                (Crop->prm.MaxRootingDepth - Crop->st.RootDepth_prev);
        
        WaterRootExt = min(WaterRootExt,WatBal->st.MoistureLOW);
                
        WatBal->st.MoistureLOW -= WaterRootExt;

        /* Total water addition to root zone by root growth  */
        WatBal->st.WaterRootExt += WaterRootExt;

        /* Amount of soil moisture in extended root zone */
        WatBal->st.RootZoneMoisture += WaterRootExt;
    }

    /* Mean soil moisture content in rooted zone */
     if (Crop->Sowing == 0 || Crop->Emergence == 0) {  // assuming depth must be positive
            WatBal->st.Moisture = WatBal->st.RootZoneMoisture / AssRootDepth;  
    } else{
            WatBal->st.Moisture = WatBal->st.RootZoneMoisture / Crop->st.RootDepth;  
    }
    
    /* Store the infiltration rate of the previous day */
     WatBal->InfPreviousDay = WatBal->rt.Infiltration;
     
}