#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"
#include "npcycling.h"




/*-----------------------------------------------------------*/
/* function CalMaxTSMD                                       */
/* Purpose: Calculate the maximum topsoil mositure deficit   */
/* MaxTSMD is related to:                                    */
/*    1) Clay content                                        */
/*    2) If the soil is vegetated or not                     */
/*-----------------------------------------------------------*/


void CalMaxTSMD() {
    TopsoilDepth = 30.0;
    MaxTSMD = -((20.0 + 1.3 * clay_content[Lon][Lat] - 0.01 * clay_content[Lon][Lat] * clay_content[Lon][Lat]) * TopsoilDepth) / 23.;
}

/*---------------------------------------------------*/
/* function CalDecomp                                */
/* Purpose: Calculate the decomposition rate of SOC  */
/*---------------------------------------------------*/

void CalDecomp() {
    
    /* Calculate the modification factors */
    TopsoilDepth = 30.0;
    float AccTSMD;

    float m_temp;        // Modification factor for temperature
    float m_moisture;    // Modification factor for soil moisture
    float m_cover;       // Modification factor for soil cover
     
    m_temp = 47.91 / (1.0 + exp(106.06 / (DayTemp + 18.27)));

    AccTSMD = max(min(WatBal->st.TSMD,0), MaxTSMD);
    if (AccTSMD > 0.444 * MaxTSMD){
        m_moisture = 1.0;
    } else{
        m_moisture = 0.2 + 0.8 * (MaxTSMD-AccTSMD) / (MaxTSMD - 0.444*MaxTSMD);
    }

    if (Crop->Sowing > 0 && Crop->Emergence == 1){
        m_cover = 1.0;
    } else{
        m_cover = 0.6; 
    }
    
    /* Calculate the decomposition rate */
    NPC->decomp_rt.SOC_decomp = (bulk_density[Lon][Lat] * TopsoilDepth * SOC[Lon][Lat] * 1000)/365; // "/365" As the function is developed for annual decomposition
    NPC->decomp_rt.SON_decomp = NPC->decomp_rt.SOC_decomp * NC_ratio[Lon][Lat];
    NPC->decomp_rt.SOP_decomp = NPC->decomp_rt.SOC_decomp * PC_ratio[Lon][Lat];

}
