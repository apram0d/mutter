/**
 *
 * @file  DisplayPcDpst.h
 * @brief  This file contains all the Display Pc Dpst related interface functions
 *
 */

#ifndef _DISPLAY_PCDPST_H_
#define _DISPLAY_PCDPST_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef IN
#define IN
#endif

#ifndef FALSE
#define FALSE 0 
#endif

#ifndef TRUE
#define TRUE 1 
#endif



#define DPST_BIN_COUNT 32   // Total number of segments in DPST
#define DPST_IET_LUT_LENGTH 33 // Total number of IET entries
#define LFP_DEFAULT_PANEL_GAMMA_IN_HUNDREDS 220 // Default LFP Panel Gamma Value in hundreds precision
#define DISPLAY_PC_DPST_MIN_AGGR_SETTING 0 // Lowest DPST aggressiveness setting in driver
#define DISPLAY_PC_DPST_MAX_AGGR_SETTING 5 // Highest DPST aggressiveness setting in driver

typedef enum _PIPE_ID  {
    NULL_PIPE = 0x7F,
    DPST_PIPE_ANY = 0x7E,
    DPST_PIPE_A = 0,
    DPST_PIPE_B = 1,
    DPST_PIPE_C = 2,
    DPST_PIPE_D = 3
} PIPE_ID;

typedef enum _DD_DPST_OPERATION
{
    DD_DPST_HISTOGRAM_ENABLE = 0,
    DD_DPST_HISTOGRAM_DISABLE,
    DD_DPST_HISTOGRAM_RESET,
    DD_DPST_HISTOGRAM_STATUS,
    DD_DPST_PROGRAM_DIET_REGS,     
} DD_DPST_OPERATION;

typedef enum _DPST_RESTRICTIONS_ENUM
{
    DPST_DISPLAY_OFF,
    DPST_PIPE_DISABLE,
    NUMBER_DPST_RESTRICTIONS
}DPST_RESTRICTIONS;


typedef enum _DD_DPST_BKLT_TYPE
{
    DD_BKLT_TYPE_UNDEFINED = 0,
    DD_CCFL_BACKLIGHT,
    DD_LED_BACKLIGHT,
    DD_MAX_BACKLIGHT_TYPE
} DD_DPST_BKLT_TYPE;

typedef struct _DD_DPST_ARGS
{
    PIPE_ID PipeId;
    DD_DPST_OPERATION DpstOpReq; // Operation Request
    bool IsSwDpst;
    bool IsProgramDiet;
    uint32_t DietFactor[DPST_IET_LUT_LENGTH];
    uint32_t Histogram[DPST_BIN_COUNT];
    uint32_t Backlight_level;
    uint32_t Aggressiveness_Level;
    uint32_t Resolution_X;
    uint32_t Resolution_Y;
}DD_DPST_ARGS;


void SetHistogramDataBin(DD_DPST_ARGS *DpstArgs);

//DD_DPST_ARGS CompArgs;
//DD_DPST_ARGS Set_BacklightLevel_DietFactor();
                 


#endif





