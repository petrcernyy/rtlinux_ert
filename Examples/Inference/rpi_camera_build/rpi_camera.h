//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: rpi_camera.h
//
// Code generated for Simulink model 'rpi_camera'.
//
// Model version                  : 1.399
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Mar  5 14:07:23 2026
//
// Target selection: rtlinux_ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef rpi_camera_h_
#define rpi_camera_h_
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <gpiod.h>
#include <pthread.h>
#include <sched.h>
#include "rtwtypes.h"
#include "camera_wrapper.h"
#include "vipdrawtext_rt.h"
#include "rpi_camera_types.h"

// Block signals (default storage)
struct B_rpi_camera_T {
  real32_T preprocessedInp[307200];
  uint8_T CAMERA1[307200];             // '<S1>/CAMERA1'
  uint8_T InsertText5[307200];
  real_T FPS1;                         // '<S1>/FPS1'
  real_T UnitDelay;                    // '<S1>/Unit Delay'
  real32_T scoreOut;                   // '<S1>/MATLAB Function'
  int32_T xyO[2];                      // '<S1>/MATLAB Function9'
  int32_T xy[2];                       // '<S1>/MATLAB Function'
  uint8_T r[102400];                   // '<S1>/MATLAB Function7'
  uint8_T g[102400];                   // '<S1>/MATLAB Function7'
  uint8_T b[102400];                   // '<S1>/MATLAB Function7'
  uint8_T displayImg[307200];          // '<S1>/MATLAB Function10'
  uint8_T img_out[307200];             // '<S1>/MATLAB Function'
  uint8_T labelOut[20];                // '<S1>/MATLAB Function'
  uint8_T InsertText4[307200];         // '<S1>/Insert Text4'
};

// Block states (default storage) for system '<Root>'
struct DW_rpi_camera_T {
  coder_TFLiteModel_rpi_camera_T net;  // '<S1>/MATLAB Function10'
  cell_wrap_6_rpi_camera_T cocoLabels[91];// '<S1>/MATLAB Function10'
  real_T UnitDelay_DSTATE;             // '<S1>/Unit Delay'
  real_T InsertText3_VarHolder[2];     // '<S1>/Insert Text3'
  void *POSIX_Thread_PWORK;            // '<Root>/POSIX_Thread'
  real32_T InsertText5_VarHolder[2];   // '<S1>/Insert Text5'
  int32_T InsertText3_TxtLoc[2];       // '<S1>/Insert Text3'
  int32_T InsertText4_TxtLoc[2];       // '<S1>/Insert Text4'
  int32_T InsertText5_TxtLoc[2];       // '<S1>/Insert Text5'
  uint32_T InsertText3_TxtSelctn;      // '<S1>/Insert Text3'
  uint32_T InsertText4_TxtSelctn;      // '<S1>/Insert Text4'
  uint32_T InsertText5_TxtSelctn;      // '<S1>/Insert Text5'
  int_T POSIX_Thread_IWORK;            // '<Root>/POSIX_Thread'
  uint8_T InsertText3_RepBuf[161];     // '<S1>/Insert Text3'
  uint8_T InsertText3_StringDataDW[10];// '<S1>/Insert Text3'
  uint8_T InsertText4_RepBuf[25];      // '<S1>/Insert Text4'
  uint8_T InsertText4_VarHolder[21];   // '<S1>/Insert Text4'
  uint8_T InsertText4_StringDataDW[3]; // '<S1>/Insert Text4'
  uint8_T InsertText5_RepBuf[161];     // '<S1>/Insert Text5'
  uint8_T InsertText5_StringDataDW[5]; // '<S1>/Insert Text5'
  boolean_T net_not_empty;             // '<S1>/MATLAB Function10'
};

// Constant parameters (default storage)
struct ConstP_rpi_camera_T {
  // Computed Parameter: InsertText3_GlyLBearings
  //  Referenced by: '<S1>/Insert Text3'

  int32_T InsertText3_GlyLBearings[256];

  // Computed Parameter: InsertText3_GlyTBearings
  //  Referenced by: '<S1>/Insert Text3'

  int32_T InsertText3_GlyTBearings[256];

  // Computed Parameter: InsertText4_GlyLBearings
  //  Referenced by: '<S1>/Insert Text4'

  int32_T InsertText4_GlyLBearings[256];

  // Computed Parameter: InsertText4_GlyTBearings
  //  Referenced by: '<S1>/Insert Text4'

  int32_T InsertText4_GlyTBearings[256];

  // Computed Parameter: InsertText5_GlyLBearings
  //  Referenced by: '<S1>/Insert Text5'

  int32_T InsertText5_GlyLBearings[256];

  // Computed Parameter: InsertText5_GlyTBearings
  //  Referenced by: '<S1>/Insert Text5'

  int32_T InsertText5_GlyTBearings[256];

  // Computed Parameter: InsertText3_GlyIndexes
  //  Referenced by: '<S1>/Insert Text3'

  uint32_T InsertText3_GlyIndexes[256];

  // Computed Parameter: InsertText4_GlyIndexes
  //  Referenced by: '<S1>/Insert Text4'

  uint32_T InsertText4_GlyIndexes[256];

  // Computed Parameter: InsertText5_GlyIndexes
  //  Referenced by: '<S1>/Insert Text5'

  uint32_T InsertText5_GlyIndexes[256];

  // Computed Parameter: InsertText3_GlyWidths
  //  Referenced by: '<S1>/Insert Text3'

  uint16_T InsertText3_GlyWidths[256];

  // Computed Parameter: InsertText3_GlyHeights
  //  Referenced by: '<S1>/Insert Text3'

  uint16_T InsertText3_GlyHeights[256];

  // Computed Parameter: InsertText3_GlyXAdvances
  //  Referenced by: '<S1>/Insert Text3'

  uint16_T InsertText3_GlyXAdvances[256];

  // Computed Parameter: InsertText4_GlyWidths
  //  Referenced by: '<S1>/Insert Text4'

  uint16_T InsertText4_GlyWidths[256];

  // Computed Parameter: InsertText4_GlyHeights
  //  Referenced by: '<S1>/Insert Text4'

  uint16_T InsertText4_GlyHeights[256];

  // Computed Parameter: InsertText4_GlyXAdvances
  //  Referenced by: '<S1>/Insert Text4'

  uint16_T InsertText4_GlyXAdvances[256];

  // Computed Parameter: InsertText5_GlyWidths
  //  Referenced by: '<S1>/Insert Text5'

  uint16_T InsertText5_GlyWidths[256];

  // Computed Parameter: InsertText5_GlyHeights
  //  Referenced by: '<S1>/Insert Text5'

  uint16_T InsertText5_GlyHeights[256];

  // Computed Parameter: InsertText5_GlyXAdvances
  //  Referenced by: '<S1>/Insert Text5'

  uint16_T InsertText5_GlyXAdvances[256];

  // Computed Parameter: InsertText3_StrArray
  //  Referenced by: '<S1>/Insert Text3'

  uint8_T InsertText3_StrArray[10];

  // Computed Parameter: InsertText3_GlyData
  //  Referenced by: '<S1>/Insert Text3'

  uint8_T InsertText3_GlyData[1629];

  // Computed Parameter: InsertText4_GlyData
  //  Referenced by: '<S1>/Insert Text4'

  uint8_T InsertText4_GlyData[4885];

  // Computed Parameter: InsertText5_StrArray
  //  Referenced by: '<S1>/Insert Text5'

  uint8_T InsertText5_StrArray[5];

  // Computed Parameter: InsertText5_GlyData
  //  Referenced by: '<S1>/Insert Text5'

  uint8_T InsertText5_GlyData[1521];
};

// Real-time Model Data Structure
struct tag_RTM_rpi_camera_T {
  const char_T * volatile errorStatus;
  const char_T* getErrorStatus() const;
  void setErrorStatus(const char_T* const volatile aErrorStatus);
};

// Block signals (default storage)
#ifdef __cplusplus

extern "C"
{

#endif

  extern struct B_rpi_camera_T rpi_camera_B;

#ifdef __cplusplus

}

#endif

// Block states (default storage)
extern struct DW_rpi_camera_T rpi_camera_DW;

// Constant parameters (default storage)
extern const ConstP_rpi_camera_T rpi_camera_ConstP;

#ifdef __cplusplus

extern "C"
{

#endif

  // Model entry point functions
  extern void rpi_camera_initialize(void);
  extern void rpi_camera_step(void);
  extern void rpi_camera_terminate(void);

#ifdef __cplusplus

}

#endif

// Real-time Model object
#ifdef __cplusplus

extern "C"
{

#endif

  extern RT_MODEL_rpi_camera_T *const rpi_camera_M;

#ifdef __cplusplus

}

#endif

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'rpi_camera'
//  '<S1>'   : 'rpi_camera/Function-Call Subsystem'
//  '<S2>'   : 'rpi_camera/Function-Call Subsystem/MATLAB Function'
//  '<S3>'   : 'rpi_camera/Function-Call Subsystem/MATLAB Function10'
//  '<S4>'   : 'rpi_camera/Function-Call Subsystem/MATLAB Function7'
//  '<S5>'   : 'rpi_camera/Function-Call Subsystem/MATLAB Function9'

#endif                                 // rpi_camera_h_

//
// File trailer for generated code.
//
// [EOF]
//
