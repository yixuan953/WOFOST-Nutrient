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

   } 
   
   else if (texture_class[Lon][Lat] > 11)                                 // Sand
   {
    KL = 1000;
    miu_SDis = 0.000002;
   }
   
   else                                                                   // Loam
   {
    KL = 1500;
    miu_SDis = 0.000044;
   }
}