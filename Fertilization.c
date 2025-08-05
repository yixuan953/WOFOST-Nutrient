#include <stdio.h>
#include "wofost.h"
#include "extern.h"
#include "npcycling.h"

void IfFertilization(char* dateString)
{
    NPC->Fertilization = 0;

    int month, sow_day;
    sscanf(dateString, "%d-%d", &month, &sow_day);

    // Build a tm structure for the sowing date (in the current year)
    struct tm sow_date = {0};
    sow_date.tm_year = current_date.tm_year;
    sow_date.tm_mon = month - 1;    // tm_mon is 0-based
    sow_date.tm_mday = sow_day;

    // Normalize to correct tm_wday, tm_yday, etc.
    mktime(&sow_date);

    // Subtract 5 days
    sow_date.tm_mday -= 4;
    mktime(&sow_date);  // Normalize again after subtraction

    if (current_date.tm_year == sow_date.tm_year &&
        current_date.tm_mon == sow_date.tm_mon &&
        current_date.tm_mday == sow_date.tm_mday &&
        MeteoYear[Day] <= Meteo->EndYear)
    {
        NPC->Fertilization = 1;
    }
}

void GetPFertInput()
{
    Org_frac = 0.5;
    float InorgPInput;
    float ManurePInput;
    if (NPC->Fertilization == 1)
    {
        if(isnan(Inorg_P_appRate[Lon][Lat][Crop->Seasons - 1])){
            InorgPInput = 0.0;} 
            else{
                InorgPInput = Inorg_P_appRate[Lon][Lat][Crop->Seasons-1];
            }
        if(isnan(Manure_P_appRate[Lon][Lat][Crop->Seasons - 1])){
            ManurePInput= 0.0;} 
            else{
                ManurePInput = Manure_P_appRate[Lon][Lat][Crop->Seasons-1];
            }
        NPC->P_fert_input = InorgPInput + Org_frac * ManurePInput;
    } else{
        NPC->P_fert_input = 0;
    }
}