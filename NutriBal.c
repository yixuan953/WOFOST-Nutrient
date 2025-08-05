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

    // The daily N availability is dependent on deposition, decomposition, and the unsed fertilizer input
    NPC->st_N_avail = N_total_dep[Lon][Lat][Day] + NPC->decomp_rt.SON_decomp;

    // The daily P availability is depend on the transpiration [cm] and P concentration [g/m3] in the soil solution
    NPC->st_P_avail = WatBal->rt.Transpiration * NPC->p_st.cP_inorg * 0.1;

}

