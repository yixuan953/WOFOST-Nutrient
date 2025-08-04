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
    NPC->st_N_avail = N_total_dep[Lon][Lat][Day] + NPC->decomp_rt.SON_decomp;
}
