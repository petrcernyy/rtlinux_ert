//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: rpi_camera_types.h
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
#ifndef rpi_camera_types_h_
#define rpi_camera_types_h_
#include "rtwtypes.h"
#include "invokeinterpreter.hpp"
#include "coder_bounded_array.h"

// Custom Type definition for MATLAB Function: '<S1>/MATLAB Function10'
#include "invokeinterpreter.hpp"
#ifndef struct_coder_TFLiteModel_rpi_camera_T
#define struct_coder_TFLiteModel_rpi_camera_T

struct coder_TFLiteModel_rpi_camera_T
{
  boolean_T matlabCodegenIsDeleted;
  real32_T Mean;
  real32_T StandardDeviation;
  boolean_T EnableProfiling;
  boolean_T Verbose;
  int32_T InternalNumThreads;
  invokeinterpreter * Network;
  real32_T PredictedTotalOutput[601];
};

#endif                                 // struct_coder_TFLiteModel_rpi_camera_T

// Custom Type definition for MATLAB Function: '<S1>/MATLAB Function10'
#ifndef struct_cell_wrap_6_rpi_camera_T
#define struct_cell_wrap_6_rpi_camera_T

struct cell_wrap_6_rpi_camera_T
{
  coder::bounded_array<char_T, 14U, 2U> f1;
};

#endif                                 // struct_cell_wrap_6_rpi_camera_T

// Forward declaration for rtModel
typedef struct tag_RTM_rpi_camera_T RT_MODEL_rpi_camera_T;

#endif                                 // rpi_camera_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
