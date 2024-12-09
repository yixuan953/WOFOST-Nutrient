#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"

// includes are predefined computer code or settings (definitions)
// the ones with <> are the standard code comes with gcc (GNU Compiler Collection), the ones with "" are made by Iwan
int main(int argc, char **argv)
{

    FILE **files; // 声明一个指向FILE指针的指针，这%通常用于指向一个FILE指针数组，可以用来处理多个文件
    FILE *fptr;   // 声明一个指向FILE的指针，通常用于文件操作，如打开、读取、写入和关闭文件。

    SimUnit *initial = NULL; // 声明一个指向SimUnit类型的指针并初始化为NULL，SimUnit可能是一个用户定义的数据类型。
    Weather *head;           // 声明一个指向Weather类型的指针，可能用于链表的头指针，用来存储天气数据。
    Green *wipe;             // 声明一个指向Green类型的指针，Green可能是一个用户定义的数据类型。

    int CycleLength = 300; // 声明一个整型变量CycleLength并初始化为300，可能表示一个周期的长度。
    int NumberOfFiles;     // 声明一个整型变量NumberOfFiles，可能用来存储文件的数量。
    int Emergence;         // 声明一个整型变量Emergence，其具体用途不明。
    int i;                 // 声明一个整型变量i，通常用作循环计数。

    char list[MAX_STRING];      // 声明一个字符数组list，大小为MAX_STRING，可能用来存储一个字符串列表。
    char meteolist[MAX_STRING]; // 声明一个字符数组meteolist，大小为MAX_STRING，可能用来存储气象数据的列表。
    char name[MAX_STRING];      // 声明一个字符数组name，大小为MAX_STRING
    char name_old[MAX_STRING];  // 声明一个字符数组name_old，大小为MAX_STRING。

    Step = 1.; // 将Step变量赋值为1.0，Step的声明没有在这段代码中显示，可能在前面已经声明过。

    // 接下来是一系列的判断语句，用来进行错误检查和参数验证。
    if (argc != 3)
        exit(0); // 如果命令行参数的数量不等于3，则退出程序。
    if (strlen(argv[1]) >= MAX_STRING)
        exit(0); // 如果第一个命令行参数的长度大于或等于MAX_STRING，则退出程序。
    if (strlen(argv[2]) >= MAX_STRING)
        exit(0); // 同上，对第二个命令行参数进行检查。for protection

    // 将list和meteolist两个数组的内容设置为全0，通常是为了清空这些数组或初始化为默认状态。
    memset(list, '\0', MAX_STRING);
    memset(meteolist, '\0', MAX_STRING); // empty the memory string

    // 使用strncpy函数将命令行参数复制到list和meteolist数组中，只复制到各自字符串的长度。
    strncpy(list, argv[1], strlen(argv[1]));
    strncpy(meteolist, argv[2], strlen(argv[2]));

    /* Fill the crop, soil, site and management place holders*/ /* 填充作物、土壤、地点和管理信息的占位符 */
    NumberOfFiles = GetSimInput(list);

    /* Set the initial Grid address */ /* 设置初始的Grid地址 */
    initial = Grid;

    /* Get the meteo filenames and put them in the placeholder */ /* 获取气象文件的名称，并放入占位符中 */
    GetMeteoInput(meteolist);

    /* Allocate memory for the file pointers */      /* 为文件指针分配内存 */
    files = malloc(sizeof(**files) * NumberOfFiles); // 分配足够存储NumberOfFiles个FILE指针的内存

    /* Go back to the beginning of the list */ /* 返回列表的开始位置 */
    Grid = initial;

    /* Open the output files */                            /* 打开输出文件 */
    memset(name_old, '\0', MAX_STRING);                    // 将name_old数组初始化为全'\0'
    while (Grid)                                           // 遍历Grid链表
    {                                                      /* Make valgrind happy  */
        memset(name, '\0', MAX_STRING);                    // 将name数组初始化为全'\0'
        strncpy(name, Grid->output, strlen(Grid->output)); // 复制输出文件名到name数组中

        if (strcmp(name_old, name) != 0) // 如果当前文件名与上一个不同
        {
            // 打开新的文件并将文件指针存储在files数组中
            files[Grid->file] = fptr = fopen(name, "w");
            if (files[Grid->file] == NULL) // 如果文件打开失败
            {
                fprintf(stderr, "Cannot initialize output file %s.\n", name); // 打印错误信息到标准错误
                exit(0);                                                      // 退出程序
            }
            header(files[Grid->file]); // 写文件头
        }
        else
        {
            // 如果当前文件名与上一个相同，则复用之前的文件指针
            if (fptr != NULL)
                files[Grid->file] = fptr;
            else
            {
                fprintf(stderr, "Cannot initialize file pointer\n"); // 如果文件指针未初始化，打印错误信息
                exit(0);                                             // 退出程序
            }
        }

        // allocate memory for the statistical analysis // 为统计分析分配内存
        // Grid->twso = (float*) malloc((Meteo->EndYear - Meteo->StartYear + 1) * sizeof(float));
        for (i = 0; i <= Meteo->Seasons; i++)
        {
            // 初始化统计数据为0
            Grid->twso[i] = 0.0;
            Grid->length[i] = 0.0;
        }

        // 准备下一个输出文件名
        memset(name_old, '\0', MAX_STRING);                    // 将name_old数组重置为全'\0'
        strncpy(name_old, Grid->output, strlen(Grid->output)); // 将当前输出文件名设置为下次循环的旧文件名

        Grid->flag = 0;    // force end of simulations in endyear// 强制在结束年份结束模拟
        Grid = Grid->next; // 移动到链表的下一个元素
    }

    /* Go back to the beginning of the list */ /* 返回到列表的开始 */
    Grid = initial;

    while (Meteo)
    {
        /* Get the meteodata */ /* 获取气象数据 */
        if (GetMeteoData(Meteo) != 1)
        {
            fprintf(stderr, "Cannot get meteo data.\n");
            exit(0);
        }
        printf("running %d - %d\n", Meteo->StartYear, Meteo->EndYear);

        for (Lon = 0; Lon < Meteo->nlon; Lon++)
        {
            for (Lat = 0; Lat < Meteo->nlat; Lat++)
            {
                if (Mask[Lon][Lat] != 1) // 如果Mask对应的位置不为1，则跳过当前循环
                {
                    continue;
                }

                // Go back to the beginning of the list and rest grid value flag,and twso and length
                // 返回到列表的开始并重置Grid的标志、twso和length
                Grid = initial;
                while (Grid)
                {
                    Grid->flag = 0;

                    for (i = 0; i <= Meteo->Seasons; i++)
                    {
                        Grid->twso[i] = 0.0;
                        Grid->length[i] = 0.0;
                        Grid->crp->Seasons = 1;
                    }
                    Grid = Grid->next;
                }

                for (Day = 0; Day < Meteo->ntime; Day++)
                // assume that the series start January first // 假设系列从一月一日开始
                {

                    Grid = initial;

                    /* Set the date struct */ /* 设置日期结构 */
                    memset(&current_date, 0, sizeof(current_date));
                    current_date.tm_year = MeteoYear[Day] - 1900;
                    current_date.tm_mday = 0 + MeteoDay[Day];
                    mktime(&current_date);

                    while (Grid)
                    {
                        /* Get data, states and rates from the Grid structure and */
                        /* 从Grid结构获取数据、状态和速率，并放置在占位符中 */
                        /* put them in the place holders */
                        Crop = Grid->crp;
                        WatBal = Grid->soil;
                        Mng = Grid->mng;
                        Site = Grid->ste;

                        // Start     = Grid->start;/* 在播种或出苗时开始模拟 */
                        Emergence = Grid->emergence; /* Start simulation at sowing or emergence */

                        Temp = 0.5 * (Tmax[Lon][Lat][Day] + Tmin[Lon][Lat][Day]);
                        DayTemp = 0.5 * (Tmax[Lon][Lat][Day] + Temp);

                        /* Only simulate between start and end year */ /* 只在开始和结束年份之间模拟 */
                        if ((MeteoYear[Day] >= Meteo->StartYear && MeteoYear[Day] <= Meteo->EndYear) && (Meteo->Seasons >= Crop->Seasons))
                        {
                            /* Determine if the sowing already has occurred */ /* 确定播种是否已经发生 */
                            IfSowing(Grid->start);

                            /* If sowing has occurred than determine the emergence */ /* 如果播种已经发生，则确定出苗 */
                            if (Crop->Sowing >= 1 && Crop->Emergence == 0)
                            {
                                if (EmergenceCrop(Emergence))
                                {
                                    /* Initialize: set state variables */ /* 初始化：设置状态变量 */
                                    InitializeCrop();
                                    InitializeWatBal();
                                    InitializeNutrients();
                                }
                            }

                            if (Crop->Sowing >= 1 && Crop->Emergence == 1)
                            {
                                if (Crop->st.Development <= (Crop->prm.DevelopStageHarvest) && Crop->GrowthDay < CycleLength)
                                {
                                    Astro();
                                    CalcPenman();
                                    CalcPenmanMonteith();

                                    /* Calculate the evapotranspiration */ /* 计算蒸散发 */
                                    EvapTra();

                                    /* Set the rate variables to zero */ /* 将速率变量置零 */
                                    RatesToZero();

                                    /* Rate calculations */ /* 速率计算 */
                                    RateCalulationWatBal();
                                    Partioning();
                                    RateCalcultionNutrients();
                                    RateCalculationCrop();

                                    /* Write to the output files */ /* 写入输出文件 */
                                    // Output(output[Grid->file]);

                                    /* Calculate LAI */ /* 计算LAI */
                                    Crop->st.LAI = LeaveAreaIndex();

                                    /* State calculations */ /* 状态计算 */
                                    IntegrationCrop();
                                    IntegrationWatBal();
                                    IntegrationNutrients();

                                    Output(files[Grid->file]);

                                    /* Update the number of days that the crop has grown*/ /* 更新作物已经生长的天数 */
                                    Crop->GrowthDay++;
                                    
                                }
                                else
                                {
                                    /* Write to the output files */ /* 写入输出文件 */
                                    //Grid->twso[Crop->GrowthDay] = Crop->st.storage;
                                    //Grid->length[Crop->GrowthDay] = Crop->GrowthDay;
                                    //if (Meteo->Seasons == Crop->Seasons)
                                    //{
                                    //    Output(files[Grid->file]);
                                    //}

                                    /* Clean the LeaveProperties */ /* 清理LeaveProperties */
                                    while (Crop->LeaveProperties != NULL)
                                    {
                                        wipe = Crop->LeaveProperties;
                                        Crop->LeaveProperties = Crop->LeaveProperties->next;
                                        free(wipe); // 释放LeaveProperties节点占用的内存
                                    }

                                    Emergence = 0;
                                    Crop->TSumEmergence = 0;
                                    Crop->Emergence = 0;
                                    Crop->Sowing = 0;
                                    Crop->Seasons++;
                                }
                            }
                        }

                        /* Store the daily calculations in the Grid structure */ /* 将每天的计算结果存储在Grid结构中 */
                        Grid->crp = Crop;
                        Grid->soil = WatBal;
                        Grid->mng = Mng;
                        Grid->ste = Site;
                        Grid = Grid->next;
                    }
                }
            }
        }

        head = Meteo;
        Meteo = Meteo->next;
        CleanMeteo(head); // 清理已处理的Meteo数据
        free(head);       // 释放head指针占用的内存
    }

    /* Return to the beginning of the list */ /* 返回到列表的开始 */
    Grid = initial;

    /* Close the output files and free the allocated memory */ /* 关闭输出文件并释放分配的内存 */
    fptr = NULL;
    while (Grid)
    {
        if (files[Grid->file] != fptr)
        {
            if (Grid->file < NumberOfFiles)
            {
                fptr = files[Grid->file];
                fclose(files[Grid->file]); // 关闭文件指针
            }
        }
        Grid = Grid->next;
    }

    /* Go back to the beginning of the list */ /* 返回到列表的开始 */
    Grid = initial;
    Clean(Grid); // 清理Grid结构

    return 1;
}
