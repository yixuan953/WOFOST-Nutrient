#ifndef WOFOST_H
#define WOFOST_H

#include <time.h>
#include <math.h>

#define NR_VARIABLES_CRP	67
#define NR_TABLES_CRP   	22
#define NR_VARIABLES_SITE       12
#define NR_TABLES_SITE          1
#define NR_VARIABLES_SOIL       12
#define NR_VARIABLES_SOIL_USED  6
#define NR_TABLES_SOIL          2
#define NR_VARIABLES_MANAGEMENT 9
#define NR_TABLES_MANAGEMENT    4
#define NUMBER_OF_TABLES        31
#define MAX_STRING             2048
#define METEO_LENGTH           36600 //max 100 years 
#define FERT_LENGTH            100 //max 100 years 
#define IRRI_LENGTH            1200 //max 100 years 
#define DOMAIN_LENGTH          15000   //max 0.5 degree

struct tm current_date;

typedef struct TBL {
	float x;
	float y;
	struct TBL *next;
	} TABLE;
        
typedef struct TBLD {
	int month;
	int day;
        float amount;
	struct TBLD *next;
	} TABLE_D;

typedef struct MANAGEMENT {
        /** Tables for fertilizer application and recovery fraction **/
        TABLE_D *N_Fert_table;
        TABLE_D *P_Fert_table;
        TABLE_D *K_Fert_table;
        TABLE_D *Irrigation;
        
        float N_Mins;
        float NRecoveryFrac;
        float P_Mins;
        float PRecoveryFrac;
        float K_Mins; 
        float KRecoveryFrac;
        float N_Uptake_frac;
        float P_Uptake_frac;
        float K_Uptake_frac;
        } Management;
Management *Mng;

typedef struct CONSTANTS {
        float MaxEvapWater;
        float MoistureFC;
        float MoistureWP;
        float MoistureSAT;
        float CriticalSoilAirC;
        float MaxPercolRTZ;
        float MaxPercolSubS;
        float MaxSurfaceStorge;
        float K0;
        } Constants;

typedef struct PARAMETERS {
          /** Tables for the Crop simulations **/
        TABLE *Roots;
        TABLE *Stems;
        TABLE *Leaves;
        TABLE *Storage;

        TABLE *VernalizationRate;
        TABLE *DeltaTempSum;
        TABLE *SpecificLeaveArea;
        TABLE *SpecificStemArea;
        TABLE *KDiffuseTb;
        TABLE *EFFTb;
        TABLE *MaxAssimRate; 
        TABLE *FactorAssimRateTemp;
        TABLE *FactorGrossAssimTemp;
        TABLE *FactorSenescence;
        TABLE *DeathRateStems;
        TABLE *DeathRateRoots; 
        
        /** Tables to account for the atmospheric CO2 concentration **/
        TABLE *CO2AMAXTB;
        TABLE *CO2EFFTB;
        TABLE *CO2TRATB;

        /** Tables for the maximum nutrient content in leaves as a function of DVS **/
        TABLE *N_MaxLeaves;
        TABLE *P_MaxLeaves;
        TABLE *K_MaxLeaves;

        /** Static Variables  **/
        /**  Emergence  **/
        float TempBaseEmergence;
        float TempEffMax;
        float TSumEmergence;                      	     

        /**  Phenology  **/
        int   IdentifyAnthesis; 
        float OptimumDaylength;	            
        float CriticalDaylength;
        float SatVernRequirement;
        float BaseVernRequirement;
        float TempSum1;       
        float TempSum2; 
        float InitialDVS;
        float DevelopStageHarvest;

        /** Initial Values  **/
        float InitialDryWeight;
        float RelIncreaseLAI;

        /**  Green Area  **/
        float SpecificPodArea;
        float LifeSpan;
        float TempBaseLeaves;

        /** Conversion assimilates into biomass **/
        float ConversionLeaves;
        float ConversionStorage;
        float ConversionRoots;
        float ConversionStems;

        /** Maintenance Respiration **/
        float Q10;
        float RelRespiLeaves;
        float RelRespiStorage;
        float RelRespiRoots;
        float RelRespiStems;

        /** Death Rates  **/
        float MaxRelDeathRate;

        /** Water Use  **/
        float CorrectionTransp;
        float CropGroupNumber;
        float Airducts;

        /** Rooting **/
        float InitRootingDepth;
        float MaxIncreaseRoot;
        float MaxRootingDepth;

        /** Nutrients **/
        float DyingLeaves_NPK_Stress; 
        float DevelopmentStageNLimit; 
        float DevelopmentStageNT;
        float FracTranslocRoots;  
        float Opt_N_Frac;   
        float Opt_P_Frac;   
        float Opt_K_Frac;   
        float N_MaxRoots;   
        float N_MaxStems;   
        float P_MaxRoots;   
        float P_MaxStems;   
        float K_MaxRoots;   
        float K_MaxStems;   
        float NitrogenStressLAI;   
        float NLUE;   
        float Max_N_storage; 
        float Max_P_storage; 
        float Max_K_storage; 
        float N_lv_partitioning;  
        float NutrientStessSLA;   
        float N_ResidualFrac_lv;  
        float N_ResidualFrac_st;  
        float N_ResidualFrac_ro;  
        float P_ResidualFrac_lv;  
        float P_ResidualFrac_st;  
        float P_ResidualFrac_ro;  
        float K_ResidualFrac_lv;  
        float K_ResidualFrac_st;   
        float K_ResidualFrac_ro;   
        float TCNT;   
        float TCPT;   
        float TCKT;   
        float N_fixation;
        float Frac_translocation;
        } Parameters;


typedef struct STATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float Rain;
        float RootZoneMoisture;
        float Runoff;
        float SurfaceStorage;
        float Transpiration;
        float WaterRootExt;
        float PreSurplus;
        float TSMD;
        } States;
        

typedef struct RATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float RootZoneMoisture;
        float Runoff;
        float Transpiration;
        float WaterRootExt;
        float PreSurplus;
        } Rates;
        
 
typedef struct NUTRIENT_RATES {
        float roots;
        float stems;
        float leaves;
        float storage;
        float Demand_lv;
        float Demand_st;
        float Demand_ro;
        float Demand_so;
        float Supply;
        float Transloc;
        float Transloc_lv;
        float Transloc_st;
        float Transloc_ro;
        float Uptake;
        float Uptake_lv;
        float Uptake_st;
        float Uptake_ro;
        float death_lv;
        float death_st;
        float death_ro;
        } nutrient_rates;
        

typedef struct NUTRIENT_STATES {
        float roots;
        float stems;
        float leaves;
        float storage;
        float Max_lv;
        float Max_st;
        float Max_ro;
        float Max_so;
        float Optimum_lv;
        float Optimum_st;
        float Indx;
        float Uptake;
        float Uptake_lv;
        float Uptake_st;
        float Uptake_ro;
        float death_lv;
        float death_st;
        float death_ro;
        float Avail;
        float Avail_lv;
        float Avail_st;
        float Avail_ro;
        
        } nutrient_states;
        

typedef struct GROWTH_RATES {
        float roots;
        float stems;
        float leaves;
        float LAIExp;
        float storage;
        float Development;
        float RootDepth;
        float vernalization;
} growth_rates;

typedef struct GROWTH_STATES {
        float roots;
        float stems;
        float leaves;
        float LAI;
        float LAIExp;
        float storage;
        float Development;
        float RootDepth;
        float RootDepth_prev;
        float vernalization;
        } growth_states;

typedef struct DYING_STATES {
        float roots;
        float stems;
        float leaves;
        } dying_states; 
        
typedef struct DYING_RATES {
        float roots;
        float stems;
        float leaves;
        } dying_rates; 


typedef struct GREEN {
	float weight;
	float age;
	float area;
	struct GREEN *next;
	} Green;
        

typedef struct PLANT {
        int Emergence;
        int Sowing;
        int Seasons;
        int GrowthDay;     
        float NPK_Indx;
        float NutrientStress;
        float DaysOxygenStress;
        float TSumEmergence;
        float fac_ro;
        float fac_lv;
        float fac_st;
        float fac_so;
        float rt_DevPrev;
        
        Parameters prm;
        
        growth_rates  rt;
        growth_states st;
        dying_rates   drt;
        dying_states  dst;
        
        nutrient_states N_st;
	nutrient_states P_st;
        nutrient_states K_st;
        
        nutrient_rates N_rt;
	nutrient_rates P_rt;
        nutrient_rates K_rt;            
        
        Green *LeaveProperties;
	} Plant;    
Plant *Crop; /* Place holder for the current crop simulations */


typedef struct SOIL {
        float DaysSinceLastRain;
        float SoilMaxRootingDepth;
        float WaterStress;
        float MoistureStress;
        float InfPreviousDay;
        
        /* Tables for Soil */
        TABLE *VolumetricSoilMoisture;
        TABLE *HydraulicConductivity; /* currently not used */
        
        Constants ct;
        States st;
        Rates rt;
        } Soil;
Soil *WatBal; /* Place holder for the current water balance simulations */


typedef struct FIELD {
        /* Water related parameters */
        float FlagGroundWater;
        float InfRainDependent;
        float FlagDrains;
        float MaxSurfaceStorage;     
        float InitSoilMoisture;
        float GroundwaterDepth;
        float DD;
        float SoilLimRootDepth;
        float NotInfiltrating;
        float SurfaceStorage;
        float MaxInitSoilM;
        
        /* Mineral states and rates */
        float st_N_tot;
        float st_P_tot;
        float st_K_tot;

        float st_N_mins;
        float st_P_mins;
        float st_K_mins;

        float rt_N_tot;
        float rt_P_tot;
        float rt_K_tot;

        float rt_N_mins;
        float rt_P_mins;
        float rt_K_mins;
        
        /** Table for the fraction of precipitation that does not infiltrate **/
        TABLE *NotInfTB;
        } Field;
Field *Site; /* Place holder for the current site simulations */

/* Define dthe NP_cycling related structures*/

typedef struct DECOMPOSITION_RATE {
        float SOC_decomp;
        float SON_decomp;
        float SOP_decomp;
        } decomposition_rates; 

typedef struct P_RATE {
        float PrecP_S; // Changes due to the stable P pool
        float PrecP_L; // Changes due to the labile P pool
        float PdisS;
        float PdisS_corr;
        float PdisL;
        float PdisL_corr;
        float PLeaching;
        float PSurfRunoff;
        float PSubRunoff;
        } p_rates; 

typedef struct P_STATE {
        float LabileP;
        float StableP;
        float PrecP;
        float cP_inorg;  // g/m3 = mg/L
        float cP_tot;    // g/m3 = mg/L
        } p_states; 

typedef struct N_STATE {
        float Emission_NH3;
        float Emission_N2O;
        float Emission_NOx;

        float N_loss_surf;
        float N_loss_sub;
        float N_loss_leach;

        float N_fixation;
        float N_decomp;
        float N_dep;

        float N_surplus;
        float N_loss_N2;
        } n_states; 

typedef struct NP_CYCLING {
        
        /* Related parameters */
        int Fertilization;      // Check if fertilizer is applied on a certain date or not
        
        /* Related states and rates */
        decomposition_rates decomp_rt;   // Decomposition
        p_rates p_rt;           // Soil P pool: [mmol P / kg soil]
        p_states p_st;          // Soil P pool: [mmol P / kg soil]
        n_states n_st;          // Variables related to annual N budges [kg N/ha]

        float P_residue_afterHavest;   // P residue input after harvest [kg/ha]
        float P_residue_beforeSowing;  // P residue input before sowing [kg/ha]

        float N_residue_afterHavest;   // N residue input after harvest [kg/ha]
        float N_residue_beforeSowing;  // N residue input before sowing [kg/ha]

        float P_fert_input;     // The P fertilizer input that is available for crop uptakes
        float N_fert_input;     // The N fertilizer input that is available for crop uptakes
        
        /* Other variables */
        float st_N_avail;       // kg N/ha
        float st_P_avail;       // kg P/ha
 
        /* Table related to N, P cycling */    
        } NPCycling;
NPCycling *NPC; 


/* Place holder for a simulation unit */
typedef struct SIMUNIT {
        Plant *crp;
        Field *ste;
        Management *mng;
        Soil  *soil;
        NPCycling *npc;

        int emergence;
        int file_DO;
        int file_AO;
        int flag;
        char start[MAX_STRING];
        char output_daily[MAX_STRING];
        char output_annual[MAX_STRING];

        /* Statistics */
        float twso[100000];
        float length[100000];
                
        struct SIMUNIT *next;
        } SimUnit; 
SimUnit *Grid;

enum{
    WEATHER_TMIN,
    WEATHER_TMAX,
    WEATHER_RADIATION,
    WEATHER_RAIN,
    WEATHER_WINDSPEED,
    WEATHER_VAPOUR,
    WEATHER_NDEP,
    WEATHER_PDEP,
    WEATHER_NTYPES
};

typedef struct WEATHER {
        char mask[MAX_STRING];
        char file[WEATHER_NTYPES][MAX_STRING];
        char type[WEATHER_NTYPES][MAX_STRING];
        char var[WEATHER_NTYPES][MAX_STRING];
        int StartYear;
        int EndYear;
        int Seasons;
        size_t nlat;
        size_t nlon;
        size_t ntime;
        struct WEATHER *next;
        } Weather;
Weather *Meteo; /* Place holder for the meteo filenames and lat/lon */

/** Meteorological Variables  **/
int Station, Year;
int MeteoYear[METEO_LENGTH];
int MeteoMonth[METEO_LENGTH];
int MeteoDay[METEO_LENGTH];
int FertYear[FERT_LENGTH];
int IrriYear[FERT_LENGTH];
int IrriMonth[IRRI_LENGTH];
float CO2;
double Longitude[DOMAIN_LENGTH], Latitude[DOMAIN_LENGTH];

// --- I added these variables to the original crop mask.nc file ----
//float **HA; // Harvest area
float **Sow_date; // Average sowing date: This will replace the sowing date
float **TSUM1; // TSUM1
float **TSUM2; // TSUM2
float **SOC; // soil organic carbon, percent weight [%]
float **bulk_density; // Top soil bulk density [kg/dm3]
float **clay_content; // percent [%]
float **texture_class;
float **slope; //[-]
float **Climate_Zone; // To calculate the EF_N2O 
float **NC_ratio; // [-]
float **PC_ratio; //[-]
float **Al_Fe_ox; //[mmol/kg]
float **P_Olsen; //[mmol/kg]
// --------------------------------------------------------------------
float **Altitude;
float **AngstA;
float **AngstB;
float ***Tmin;
float ***Tmax;
float ***Radiation;
float ***Rain;
float ***Windspeed;
float ***Vapour;
// --- I added these variables as "Meteo" input ----
float ***N_total_dep;
float ***P_total_dep;
// --- ---------------------------------------- ----

/* Time step */
float Step;


// ------- HERE I created a new data structure to input fertilizatin -----
enum{
        FERT_RESRATIO,
        FERT_N_MANURE,
        FERT_P_MANURE,
        FERT_N_UREA,
        FERT_N_OTHERINOG,
        FERT_P_INORG,
        FERT_EFNOX,
        FERT_NTYPES
    };


typedef struct FERT {
        char mask[MAX_STRING];
        char file[FERT_NTYPES][MAX_STRING];
        char type[FERT_NTYPES][MAX_STRING];
        char var[FERT_NTYPES][MAX_STRING];
        int StartYear;
        int EndYear;
        int Seasons;
        size_t nlat;
        size_t nlon;
        size_t ntime;
        struct FERT *next;
        } Nutri;
Nutri *Fert; /* The name of the structure FERT*/

float ***EF_NOx; // Emission factor of NOX [-]
float ***Manure_N_appRate; // [kg/ha]
float ***Urea_inorg_N_appRate; // [kg/ha]
float ***Other_inorg_N_appRate; // [kg/ha]
float ***Manure_P_appRate; // [kg/ha]
float ***Inorg_P_appRate; // [kg/ha]
float ***Res_return_ratio; // [-]

// ------- HERE I created a new data structure to input irrigation -----
enum{
        IRRI_RATE,
        IRRI_NTYPES
    };


typedef struct IRRIWATER {
        char mask[MAX_STRING];
        char file[IRRI_NTYPES][MAX_STRING];
        char type[IRRI_NTYPES][MAX_STRING];
        char var[IRRI_NTYPES][MAX_STRING];
        int StartYear;
        int EndYear;
        int Seasons;
        size_t nlat;
        size_t nlon;
        size_t ntime;
        struct IRRIWATER *next;
        } Water_Irri;
Water_Irri *Irri; /* The name of the structure IRRIWATER*/

float ***Irrigation_Rate; //  [-]
int **Irri_time_count;

#endif	// 