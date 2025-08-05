#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

#ifndef NPCYCLING_H
#define NPCYCLING_H

/* N, P Decomposition calculations*/
float MaxTSMD;      // [cm]
float TopsoilDepth; // [cm]

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
