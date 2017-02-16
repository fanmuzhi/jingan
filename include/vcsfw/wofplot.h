/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * Implement the VCSFW_CMD_WOF_BAELINE VCSFW_CMD_WOF_SIGNAL command
 *
 */

#define VCSFW_CMD_WOF_BASELINE 254
#define VCSFW_CMD_WOF_SIGNAL   255

/* set how many gain will scan */
#define WOF_GAIN_MAX_IDX 5 

#define  WOF_BASELINE   74

typedef struct wof_amp_gain_s
{
    uint8_t    amp2;  
    uint8_t    amp3;
    uint8_t    amp4;
    uint8_t    reserved;    
    uint16_t   value;
}wof_amp_gain_t;


//static wof_amp_gain_t   wof_gain_setting[WOF_GAIN_MAX_IDX] = 
//{
//    {.amp2=0,   .amp3=4,    .amp4=1,  .value=0x821},    /* Normal Gain */
//    {.amp2=0,   .amp3=2,    .amp4=0,  .value=0x810},    /* Lower Gain  */
//    {.amp2=0,   .amp3=3,    .amp4=0,  .value=0x818},    /* Low Gain    */
//    {.amp2=0,   .amp3=5,    .amp4=0,  .value=0x828},    /* High Gain   */
//    {.amp2=1,   .amp3=1,    .amp4=0,  .value=0x848},    /* Higer Gain  */
//};

//typedef struct vcsfw_cmd_wofbaseline_s
//{
//    uint8_t  size;
//    wof_amp_gain_t  wof_gain_setting[WOF_GAIN_MAX_IDX]; 
//}vcsfw_cmd_wofbaseline_t;

typedef struct vcsfw_reply_wofbaseline_s
{
    uint16_t    gains[WOF_GAIN_MAX_IDX];
    uint16_t    offsets[WOF_GAIN_MAX_IDX];
}vcsfw_reply_wofbaseline_t;

typedef struct vcsfw_cmd_wofsignal_s
{
    uint16_t    gains[WOF_GAIN_MAX_IDX];
    uint16_t    offsets[WOF_GAIN_MAX_IDX];
}vcsfw_cmd_wofsignal_t;
