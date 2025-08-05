#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"
#include "npcycling.h"

/* ---------------------------------------------------------------------------------*/
/*  function CalMaxPPoolSize()                                                      */
/*  Purpose: Calculate the maximum size for labile and stable P pool [mmol/kg]      */
/* ---------------------------------------------------------------------------------*/
void CalMaxPPoolSize()
{
   MaxLabileP = Al_Fe_ox[Lon][Lat]/6;
   MaxStableP = Al_Fe_ox[Lon][Lat]/3; 
}


/* --------------------------------------------------------------------*/
/*  function InitializeSoilPPool()                                     */
/*  Purpose: Initialize soil P pool for soil P dynamics [mmol/kg]      */
/* --------------------------------------------------------------------*/
void InitializeSoilPPool()
{  

   NPC->p_st.PrecP = 0.0; // Precipitation P pool: As a buffer of soil P pool
   NPC->p_st.LabileP = limit(MinLabileP, MaxLabileP, P_Olsen[Lon][Lat] * 8/3);   // Labile P pool as a function of P oxlate (= Olsen_P * 8)
   NPC->p_st.StableP = min(MaxStableP, P_Olsen[Lon][Lat] * 16/3);                // Stable P pool as a function of P oxlate (= Olsen_P * 8)
   NPC->p_st.cP_inorg = 0.0;
   NPC->p_st.cP_tot = 0.0;
   
   // Initialize the P losses;
   NPC->p_rt.PLeaching = 0.0;
   NPC->p_rt.PSurfRunoff = 0.0;
   NPC->p_rt.PSubRunoff = 0.0;
   
   NPC->p_rt.PrecP_L = 0.0;     // Changes of the precipitation P pool due to the exchange between the labile and precipitation pool
   NPC->p_rt.PrecP_S = 0.0;     // Changes of the precipitation P pool due to the exchange between the stable and precipitation pool 
   NPC->p_rt.PdisL = 0.0;       // Changes of the labile P pool
   NPC->p_rt.PdisL_corr = 0.0;  // Corrected changes of the labile P pool
   NPC->p_rt.PdisS = 0.0;       // Changes of the stable P pool
   NPC->p_rt.PdisS_corr = 0.0;  // Corrected changes of the stable P pool

}

/* --------------------------------------------------------------------------------------------*/
/*  function CalPConcentration()                                                               */
/*  Purpose: Calculate P concentration (inorganic & total) in the soil solution [g/m3]         */
/* --------------------------------------------------------------------------------------------*/
void CalPConcentration()
{
  NPC->p_st.cP_inorg = 1000 * NPC->p_st.LabileP/(KL * (MaxLabileP - NPC->p_st.LabileP));
  NPC->p_st.cP_tot = 1.1 * NPC->p_st.cP_inorg + 0.1 * exp(-NPC->p_st.cP_inorg);
}

/* ---------------------------------------------------------------------------------------*/
/*  function CalPLeaching()                                                               */
/*  Purpose: Calculate daily P losses through the leaching[kg P/ha]                       */
/* ---------------------------------------------------------------------------------------*/
void CalPLeaching()
{
  NPC->p_rt.PLeaching = NPC->p_st.cP_tot * WatBal->rt.Percolation * 0.1;
}

/* ---------------------------------------------------------------------------------------*/
/*  function CalPSurfRunoff()                                                             */
/*  Purpose: Calculate daily P losses through the surface runoff [kg P/ha]                */
/* ---------------------------------------------------------------------------------------*/
void CalPSurfRunoff()
{
  NPC->p_rt.PSurfRunoff = NPC->p_st.cP_tot * WatBal->rt.Runoff * 0.1;
}

/* ---------------------------------------------------------------------------------------*/
/*  function CalPSubRunoff()                                                              */
/*  Purpose: Calculate daily P losses through the subsurface runoff [kg P/ha]             */
/* ---------------------------------------------------------------------------------------*/
void CalPSubRunoff()
{
  NPC->p_rt.PSubRunoff = NPC->p_st.cP_tot * WatBal->rt.Loss * 0.1;
}


/* ---------------------------------------------------------------------------------------------------*/
/*  function CalDisS()                                                                                */
/*  Purpose: Calculate the daily P exchange from the soil solution to the stable pool [mmol kg-1 d-1] */
/* ---------------------------------------------------------------------------------------------------*/
void CalPdisS()
{
  float n = 0.26;               // Parameter in the Freundlich constant calculation equation
  float KF = MaxStableP/(pow(90,n)); // Freundlich constant of the stable pool
  float miu_DisS = 0.0014;      // The rate constant for the transfer from the soil solution to the stable P pool
  
  // Initial PdisS depends on cP_tot
  if (KF >= pow(NPC->p_st.cP_inorg,NPC->p_st.StableP))
  {
    NPC->p_rt.PdisS = miu_DisS * (KF * NPC->p_st.cP_inorg - NPC->p_st.StableP); // From soil solution to stable P pool
  } else 
  {
    NPC->p_rt.PdisS = miu_SDis * (KF * NPC->p_st.cP_inorg - NPC->p_st.StableP); // From the stable P pool to soil solution
  }  
  
  // Correct the NPC->p_rt.PdisS
  if (NPC->p_rt.PdisS < 0 && NPC->p_st.PrecP < fabsf(NPC->p_rt.PdisS))
  {
    NPC->p_rt.PdisS_corr = NPC->p_rt.PdisS + NPC->p_rt.PdisS;
  } else
  {
    NPC->p_rt.PdisS_corr = 0;
  }
  
}


/* -----------------------------------------------------------------------------------------------------*/
/*  function CalDisL()                                                                                  */
/*  Purpose: Calculate the daily P exchange from the labile P pool to the soil solution [mmol kg-1 d-1] */
/* -----------------------------------------------------------------------------------------------------*/
void CalPdisL()
{
   TopsoilDepth = 30.0;           // [cm]
   float Pacc;                    // [kgP/ha]
   float MolarMassP = 31.0;       // [mg/mmol]
   
   if (Crop->Sowing <1) // When the crop is now sowed or transplanted, there is no crop uptake
   {
    Pacc = NPC->P_fert_input + P_total_dep[Lon][Lat][Day] - NPC->decomp_rt.SOP_decomp - NPC->p_rt.PSurfRunoff - NPC->p_rt.PSubRunoff - NPC->p_rt.PLeaching;
   } else 
   {
    Pacc = NPC->P_fert_input + P_total_dep[Lon][Lat][Day] - Crop->N_rt.Uptake - NPC->decomp_rt.SOP_decomp - NPC->p_rt.PSurfRunoff - NPC->p_rt.PSubRunoff - NPC->p_rt.PLeaching;
   }
   
   NPC->p_rt.PdisL = (100 * Pacc/(MolarMassP * TopsoilDepth * bulk_density[Lon][Lat])) - NPC->p_rt.PdisS; // Unit: [mmol P/kg soil]

}


/* ----------------------------------------------------------------------------------------------------------------------------------------*/
/*  function CalPrecipChangeS()                                                                                                            */
/*  Purpose: Calculate the changes of the precipitation P pool due to exachanges between the stable and precipitation pool [mmol kg-1 d-1] */
/* ----------------------------------------------------------------------------------------------------------------------------------------*/
void CalPrecipChangeS()
{

  if (NPC->p_rt.PdisS < 0.0)        // Stable P pool is supplied by the precipitation P pool
  {
    //When the precipitation P pool is not enough to supply the stable P pool
    if (NPC->p_st.PrecP < fabsf(NPC->p_rt.PrecP_S))
    {
      NPC->p_rt.PrecP_S = -NPC->p_st.PrecP; 
    } else 
    {
      NPC->p_rt.PrecP_S = NPC->p_rt.PdisS;
    }
    
  } 
  else if (NPC->p_rt.PdisS >=0.0)   // Stable P pool is supplied by the soil solution 
  {
    // If the stable P pool will exceed the maximum size after being supplied by the soil solution
    if ((NPC->p_st.StableP + NPC->p_rt.PdisS) > MaxStableP)   
    {
      NPC->p_rt.PrecP_S = NPC->p_st.StableP + NPC->p_rt.PdisS - MaxStableP;
    } else 
    {
      NPC->p_rt.PrecP_S = 0;
    }
  }
}


/* ------------------------------------------------------------------------------------------*/
/*  function CorrectPdisL()                                                                  */
/*  Purpose: Correct the PdisL according to changes in precipitation P pool  [mmol kg-1 d-1] */
/* ------------------------------------------------------------------------------------------*/
void CorrectPdisL()  
{ 
  // When the soil solution needs to supply the labile P pool
  if (NPC->p_rt.PdisL < 0)  
  {
    // When the precipitation pool does not have enough P to suuply the labile P pool
    if ((NPC->p_st.PrecP + NPC->p_rt.PrecP_S) < fabsf(NPC->p_rt.PdisL))
    {
      NPC->p_rt.PdisL_corr = NPC->p_rt.PdisL + NPC->p_st.PrecP + NPC->p_rt.PrecP_S;
    } 
    else 
    {
      NPC->p_rt.PdisL_corr = NPC->p_rt.PdisL; 
    }
  } 
  else 
  {
    NPC->p_rt.PdisL_corr = NPC->p_rt.PdisL; 
  }
}


/* ----------------------------------------------------------------------------------------------------------------------------------------*/
/*  function CalPrecipChangeL()                                                                                                            */
/*  Purpose: Calculate the changes of the precipitation P pool due to exachanges between the labile and precipitation pool [mmol kg-1 d-1] */
/* ----------------------------------------------------------------------------------------------------------------------------------------*/
void CalPrecipChangeL()
{
  MinLabileP = 0.001; // [mmol/kg]
  
  // The P is flowing from soil solution to the labile P pool
  if (NPC->p_rt.PdisL >= 0)
  { 
    // If the changes of labile P pool exceed the maximum value, the exceeded part will flow to the precipitation pool
    if ((NPC->p_rt.PdisL + NPC->p_st.LabileP) > MaxLabileP)
    {
      NPC->p_rt.PrecP_L = NPC->p_st.LabileP + NPC->p_rt.PdisL - MaxLabileP;
    }
    // If the changes of the labile P pool still cannot reach the minimum value
    else if ((NPC->p_rt.PdisL + NPC->p_st.LabileP) < MinLabileP)
    {
      // If the lacking amount is lower than the current precipitation P pool
      if ((fabsf(NPC->p_st.LabileP + NPC->p_rt.PdisL - MinLabileP))<(NPC->p_st.PrecP + NPC->p_rt.PrecP_S))
      {
        NPC->p_rt.PrecP_L = NPC->p_st.PrecP + NPC->p_rt.PrecP_S - MinLabileP;
      } else 
      {
        NPC->p_rt.PrecP_L = -(NPC->p_st.PrecP + NPC->p_rt.PrecP_S);
      }
    }
    else 
    {
      NPC->p_rt.PrecP_L = 0;
    }

  }
  
  // P is flowing from labile P pool to the soil solution
  else if (NPC->p_rt.PdisL < 0)
  {
    // The preicpitation pool is not enough to supply the P transfer from the labile pool to the soil solution
    if ((NPC->p_st.PrecP + NPC->p_rt.PrecP_S) < fabsf(NPC->p_rt.PdisL))
    {
      NPC->p_rt.PrecP_L = -(NPC->p_st.PrecP + NPC->p_rt.PrecP_S);
    }
    else 
    {
      NPC->p_rt.PrecP_L = NPC->p_rt.PdisL;
    }
  }

}

/* -----------------------------------------------------------------------*/
/*  function UpdateStableP()                                              */
/*  Purpose: Calculate the changes of stable P pool       [mmol kg-1 d-1] */
/* -----------------------------------------------------------------------*/
void UpdateStableP()
{
  NPC->p_st.StableP = max((NPC->p_st.StableP + NPC->p_rt.PdisS_corr), MaxStableP);
}

/* ------------------------------------------------------------------------*/
/*  function UpdatePrecipP()                                               */
/*  Purpose: Calculate the changes of precipitation P pool [mmol kg-1 d-1] */
/* ------------------------------------------------------------------------*/
void UpdatePrecipP() 
{
  NPC->p_st.PrecP = NPC->p_st.PrecP + NPC->p_rt.PrecP_L + NPC->p_rt.PrecP_S;
}

/* -----------------------------------------------------------------------*/
/*  function UpdateLabileP()                                              */
/*  Purpose: Calculate the changes of labile P pool       [mmol kg-1 d-1] */
/* -----------------------------------------------------------------------*/
void UpdateLabileP()
{
  MinLabileP = 0.001;
  NPC->p_st.LabileP = limit(MinLabileP, MaxLabileP, NPC->p_st.LabileP + NPC->p_rt.PdisL_corr);
}

void CalPPoolDynamics()
{
  // P losses
  CalPSurfRunoff();
  CalPSubRunoff();
  CalPLeaching();

  // Langmuir equations
  CalPdisS();
  CalPrecipChangeS();
  CalPdisL();
  CorrectPdisL();
  CalPrecipChangeL();  
  
  // Update P pools
  UpdateStableP();
  UpdatePrecipP();
  UpdateLabileP();

}
