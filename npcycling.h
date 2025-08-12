#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

#ifndef NPCYCLING_H
#define NPCYCLING_H

/* N, P Decomposition calculations*/
float MaxTSMD;      // [cm]
float TopsoilDepth; // [cm]

/* Geseous emission factors for N losses*/
float EF_NH3_Urea;    // [-]
float EF_NH3_Inorg;   // [-]
float EF_NH3_Manure;  // [-]
float EF_N2O_Inorg;   // [-]
float EF_N2O_Org;     // [-]

/* Runoff factors for N losses */
float L_runoff_max;      // [-]
float f_precip_surf;     // Reduction factor for precipitation surplus
float f_texture;         // Reduction factor for soil texture
float f_groundwater;     // The fraction of leaching to groudwater

/* Leaching factors for N losses */
float L_leaching_max;    // [-]
float f_precip_leaching; // Reduction factor for precipitation surplus
float AveTemp;           // Average temperature of all of the simulation days
float f_temp;            // Reduction factor for average annual temperature
float f_soc;             // Reduction factor for soil organic carbon content (%)
float f_root;            // Reduction factor for root depth

/* P pool calculations */
float MaxLabileP;   // [mmol/kg]
float MinLabileP;   // [mmol/kg], = 0.001
float MaxStableP;   // [mmol/kg]

/* Soil texture related parameters */
float KL;         // Langmuire abasorption constant (m3/kg)
float miu_SDis;   // The rate constant for the transfer from the stable P pool to the soil solution

/* Fertilization related parameters */
float Org_frac; // Fraction of organic fertilizer inputs that cannot be uptaken by crops

#endif	// NPCYCLING_H
