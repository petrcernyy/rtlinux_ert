/* Include files */

#include "rpi_camera_sfun.h"
#include "c9_rpi_camera.h"
#include <string.h>
#include "mwmathutil.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(S);
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

/* Forward Declarations */

/* Type Definitions */

/* Named Constants */
const int32_T CALL_EVENT = -1;

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static emlrtMCInfo c9_emlrtMCI = { 87, /* lineNo */
  33,                                  /* colNo */
  "eml_int_forloop_overflow_check",    /* fName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\lib\\matlab\\eml\\eml_int_forloop_overflow_check.m"/* pName */
};

static emlrtRSInfo c9_emlrtRSI = { 8,  /* lineNo */
  "Function-Call Subsystem/MATLAB Function10",/* fcnName */
  "#rpi_camera:86"                     /* pathName */
};

static emlrtRSInfo c9_b_emlrtRSI = { 28,/* lineNo */
  "Function-Call Subsystem/MATLAB Function10",/* fcnName */
  "#rpi_camera:86"                     /* pathName */
};

static emlrtRSInfo c9_c_emlrtRSI = { 64,/* lineNo */
  "loadTFLiteModel",                   /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\shared\\dlcoder_base\\dlcoder_base\\loadTFLiteModel.m"/* pathName */
};

static emlrtRSInfo c9_d_emlrtRSI = { 1,/* lineNo */
  "TFLiteModel",                       /* fcnName */
  "C:\\ProgramData\\MATLAB\\SupportPackages\\R2024b\\toolbox\\shared\\dlcoder_base\\supportpackages\\dl_tensorflow_lite\\+coder\\@T"
  "FLiteModel\\TFLiteModel.p"          /* pathName */
};

static emlrtRSInfo c9_e_emlrtRSI = { 1,/* lineNo */
  "predict",                           /* fcnName */
  "C:\\ProgramData\\MATLAB\\SupportPackages\\R2024b\\toolbox\\shared\\dlcoder_base\\supportpackages\\dl_tensorflow_lite\\+coder\\@T"
  "FLiteModel\\predict.p"              /* pathName */
};

static emlrtRSInfo c9_f_emlrtRSI = { 22,/* lineNo */
  "matlabCodegenHandle",               /* fcnName */
  "C:\\Program Files\\MATLAB\\R2024b\\toolbox\\eml\\eml\\+coder\\+internal\\matlabCodegenHandle.m"/* pathName */
};

static emlrtRSInfo c9_g_emlrtRSI = { 1,/* lineNo */
  "Function-Call Subsystem/MATLAB Function10",/* fcnName */
  "#rpi_camera:86"                     /* pathName */
};

static emlrtRTEInfo c9_emlrtRTEI = { 11,/* lineNo */
  18,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtRTEInfo c9_b_emlrtRTEI = { 38,/* lineNo */
  5,                                   /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtRTEInfo c9_c_emlrtRTEI = { 36,/* lineNo */
  5,                                   /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtRTEInfo c9_d_emlrtRTEI = { 43,/* lineNo */
  19,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtRTEInfo c9_e_emlrtRTEI = { 5,/* lineNo */
  12,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtDCInfo c9_emlrtDCI = { 33, /* lineNo */
  16,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c9_emlrtBCI = { 1,  /* iFirst */
  100,                                 /* iLast */
  33,                                  /* lineNo */
  16,                                  /* colNo */
  "boxes",                             /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtDCInfo c9_b_emlrtDCI = { 36,/* lineNo */
  23,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c9_b_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  43,                                  /* lineNo */
  30,                                  /* colNo */
  "name",                              /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtDCInfo c9_c_emlrtDCI = { 43,/* lineNo */
  32,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c9_c_emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  43,                                  /* lineNo */
  32,                                  /* colNo */
  "name",                              /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtDCInfo c9_d_emlrtDCI = { 43,/* lineNo */
  12,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c9_d_emlrtBCI = { 1,/* iFirst */
  20,                                  /* iLast */
  43,                                  /* lineNo */
  12,                                  /* colNo */
  "labelOut",                          /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtBCInfo c9_e_emlrtBCI = { 0,/* iFirst */
  90,                                  /* iLast */
  36,                                  /* lineNo */
  23,                                  /* colNo */
  "cocoLabels",                        /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtECInfo c9_emlrtECI = { -1, /* nDims */
  43,                                  /* lineNo */
  1,                                   /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86"                     /* pName */
};

static emlrtBCInfo c9_f_emlrtBCI = { 1,/* iFirst */
  100,                                 /* iLast */
  34,                                  /* lineNo */
  19,                                  /* colNo */
  "labels",                            /* aName */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  0                                    /* checkKind */
};

static emlrtDCInfo c9_e_emlrtDCI = { 34,/* lineNo */
  19,                                  /* colNo */
  "Function-Call Subsystem/MATLAB Function10",/* fName */
  "#rpi_camera:86",                    /* pName */
  1                                    /* checkKind */
};

static emlrtBCInfo c9_g_emlrtBCI = { 1,/* iFirst */
  307200,                              /* iLast */
  1,                                   /* lineNo */
  1,                                   /* colNo */
  "",                                  /* aName */
  "preprocessInput",                   /* fName */
  "C:\\ProgramData\\MATLAB\\SupportPackages\\R2024b\\toolbox\\shared\\dlcoder_base\\supportpackages\\dl_tensorflow_lite\\+coder\\@T"
  "FLiteModel\\preprocessInput.p",     /* pName */
  3                                    /* checkKind */
};

/* Function Declarations */
static void initialize_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void initialize_params_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void mdl_start_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance);
static void mdl_terminate_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void mdl_setup_runtime_resources_c9_rpi_camera
  (SFc9_rpi_cameraInstanceStruct *chartInstance);
static void mdl_cleanup_runtime_resources_c9_rpi_camera
  (SFc9_rpi_cameraInstanceStruct *chartInstance);
static void enable_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance);
static void disable_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance);
static void sf_gateway_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void ext_mode_exec_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void c9_update_jit_animation_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *
  chartInstance);
static void c9_do_animation_call_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *
  chartInstance);
static void set_sim_state_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_st);
static void c9_TFLiteModel_predict(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const emlrtStack *c9_sp, c9_coder_TFLiteModel *c9_this, uint8_T c9_varargin_1
  [307200], real32_T c9_varargout_1[400], real32_T c9_varargout_2[100], real32_T
  c9_varargout_3[100], real32_T *c9_varargout_4);
static void c9_permute(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_a[400], real32_T c9_b[400]);
static void c9_shiftdim(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_x[400], real32_T c9_b[400]);
static void c9_b_shiftdim(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_x[100], real32_T c9_b[100]);
static void c9_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, real32_T c9_y[4]);
static void c9_b_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, real32_T c9_y[4]);
static void c9_c_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, uint8_T c9_y[307200]);
static void c9_d_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, uint8_T c9_y
  [307200]);
static real_T c9_e_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier);
static real_T c9_f_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId);
static void c9_g_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, uint8_T c9_y[20]);
static void c9_h_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, uint8_T c9_y[20]);
static real32_T c9_i_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_nullptr, const char_T *c9_identifier);
static real32_T c9_j_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId);
static void c9_k_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, boolean_T *c9_svPtr,
  c9_cell_wrap_6 c9_y[91]);
static void c9_m_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, boolean_T
  *c9_svPtr, c9_cell_wrap_6 c9_y[91]);
static void c9_o_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, char_T c9_y_data[],
  int32_T c9_y_size[2]);
static void c9_array_cell_wrap_6_1x91s_Constru(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_cell_wrap_6 c9_pMatrix[91]);
static void c9_array_cell_wrap_6_Constructor(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_cell_wrap_6 *c9_pStruct);
static void c9_array_char_T_1x14_Constructor(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_coder_array_char_T_1x14 *c9_pArray);
static void c9_array_cell_wrap_6_1x91s_Destruc(SFc9_rpi_cameraInstanceStruct
  *chartInstance);
static void init_dsm_address_info(SFc9_rpi_cameraInstanceStruct *chartInstance);
static void init_simulink_io_address(SFc9_rpi_cameraInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
  sim_mode_is_external(chartInstance->S);
  chartInstance->c9_net_not_empty = false;
  chartInstance->c9_cocoLabels_not_empty = false;
  chartInstance->c9_net.matlabCodegenIsDeleted = true;
  chartInstance->c9_doneDoubleBufferReInit = false;
  chartInstance->c9_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void initialize_params_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
}

static void mdl_start_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance)
{
  c9_array_cell_wrap_6_1x91s_Constru(chartInstance, chartInstance->c9_cocoLabels);
  sim_mode_is_external(chartInstance->S);
}

static void mdl_terminate_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
  c9_coder_TFLiteModel *c9_b_obj;
  c9_coder_TFLiteModel *c9_obj;
  c9_obj = &chartInstance->c9_net;
  if (!c9_obj->matlabCodegenIsDeleted) {
    c9_obj->matlabCodegenIsDeleted = true;
    c9_b_obj = c9_obj;
    delete(c9_b_obj->Network);
  }
}

static void mdl_setup_runtime_resources_c9_rpi_camera
  (SFc9_rpi_cameraInstanceStruct *chartInstance)
{
  static const int32_T c9_postfixPredicateTree[3] = { 0, 1, -3 };

  static const int32_T c9_condTxtEndIdx[2] = { 1748, 1781 };

  static const int32_T c9_condTxtStartIdx[2] = { 1735, 1752 };

  static const uint32_T c9_decisionTxtEndIdx = 0U;
  static const uint32_T c9_decisionTxtStartIdx = 0U;
  sfSetAnimationVectors(chartInstance->S, &chartInstance->c9_JITStateAnimation[0],
                        &chartInstance->c9_JITTransitionAnimation[0]);
  covrtCreateStateflowInstanceData(chartInstance->c9_covrtInstance, 1U, 0U, 1U,
    26U);
  covrtChartInitFcn(chartInstance->c9_covrtInstance, 0U, false, false, false);
  covrtStateInitFcn(chartInstance->c9_covrtInstance, 0U, 0U, false, false, false,
                    0U, &c9_decisionTxtStartIdx, &c9_decisionTxtEndIdx);
  covrtTransInitFcn(chartInstance->c9_covrtInstance, 0U, 0, NULL, NULL, 0U, NULL);
  covrtEmlInitFcn(chartInstance->c9_covrtInstance, "", 4U, 0U, 1U, 0U, 3U, 0U,
                  0U, 0U, 0U, 0U, 2U, 1U);
  covrtEmlFcnInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 0U,
                     "c9_rpi_camera", 0, -1, 2008);
  covrtEmlIfInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 0U, 135, 150, -1,
                    1371, false);
  covrtEmlIfInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 1U, 1732, 1781,
                    1815, 1845, false);
  covrtEmlIfInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 2U, 1947, 1963,
                    1979, 2002, false);
  covrtEmlMCDCInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 0U, 1735, 1781,
                      2U, 0U, &c9_condTxtStartIdx[0], &c9_condTxtEndIdx[0], 3U,
                      &c9_postfixPredicateTree[0], false);
  covrtEmlRelationalInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 0U, 1735,
    1748, -1, 5U);
  covrtEmlRelationalInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 1U, 1752,
    1781, -1, 3U);
  covrtEmlRelationalInitFcn(chartInstance->c9_covrtInstance, 4U, 0U, 2U, 1950,
    1963, -1, 4U);
}

static void mdl_cleanup_runtime_resources_c9_rpi_camera
  (SFc9_rpi_cameraInstanceStruct *chartInstance)
{
  covrtDeleteStateflowInstanceData(chartInstance->c9_covrtInstance);
}

static void enable_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void sf_gateway_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
  static char_T c9_cv1[21] = "ssd_mobilenet.tflite";
  static char_T c9_cv42[14] = { 'b', 'a', 's', 'e', 'b', 'a', 'l', 'l', ' ', 'g',
    'l', 'o', 'v', 'e' };

  static char_T c9_cv12[13] = { 't', 'r', 'a', 'f', 'f', 'i', 'c', ' ', 'l', 'i',
    'g', 'h', 't' };

  static char_T c9_cv16[13] = { 'p', 'a', 'r', 'k', 'i', 'n', 'g', ' ', 'm', 'e',
    't', 'e', 'r' };

  static char_T c9_cv45[13] = { 't', 'e', 'n', 'n', 'i', 's', ' ', 'r', 'a', 'c',
    'k', 'e', 't' };

  static char_T c9_cv13[12] = { 'f', 'i', 'r', 'e', ' ', 'h', 'y', 'd', 'r', 'a',
    'n', 't' };

  static char_T c9_cv41[12] = { 'b', 'a', 's', 'e', 'b', 'a', 'l', 'l', ' ', 'b',
    'a', 't' };

  static char_T c9_cv66[12] = { 'p', 'o', 't', 't', 'e', 'd', ' ', 'p', 'l', 'a',
    'n', 't' };

  static char_T c9_cv69[12] = { 'd', 'i', 'n', 'i', 'n', 'g', ' ', 't', 'a', 'b',
    'l', 'e' };

  static char_T c9_cv84[12] = { 'r', 'e', 'f', 'r', 'i', 'g', 'e', 'r', 'a', 't',
    'o', 'r' };

  static char_T c9_cv14[11] = { 's', 't', 'r', 'e', 'e', 't', ' ', 's', 'i', 'g',
    'n' };

  static char_T c9_cv32[11] = { 'e', 'y', 'e', ' ', 'g', 'l', 'a', 's', 's', 'e',
    's' };

  static char_T c9_cv39[11] = { 's', 'p', 'o', 'r', 't', 's', ' ', 'b', 'a', 'l',
    'l' };

  static char_T c9_cv43[10] = { 's', 'k', 'a', 't', 'e', 'b', 'o', 'a', 'r', 'd'
  };

  static char_T c9_cv48[10] = { 'w', 'i', 'n', 'e', ' ', 'g', 'l', 'a', 's', 's'
  };

  static char_T c9_cv5[10] = { 'm', 'o', 't', 'o', 'r', 'c', 'y', 'c', 'l', 'e'
  };

  static char_T c9_cv79[10] = { 'c', 'e', 'l', 'l', ' ', 'p', 'h', 'o', 'n', 'e'
  };

  static char_T c9_cv90[10] = { 't', 'e', 'd', 'd', 'y', ' ', 'b', 'e', 'a', 'r'
  };

  static char_T c9_cv91[10] = { 'h', 'a', 'i', 'r', ' ', 'd', 'r', 'i', 'e', 'r'
  };

  static char_T c9_cv92[10] = { 't', 'o', 'o', 't', 'h', 'b', 'r', 'u', 's', 'h'
  };

  static char_T c9_cv93[10] = { 'h', 'a', 'i', 'r', ' ', 'b', 'r', 'u', 's', 'h'
  };

  static char_T c9_cv15[9] = { 's', 't', 'o', 'p', ' ', 's', 'i', 'g', 'n' };

  static char_T c9_cv38[9] = { 's', 'n', 'o', 'w', 'b', 'o', 'a', 'r', 'd' };

  static char_T c9_cv44[9] = { 's', 'u', 'r', 'f', 'b', 'o', 'a', 'r', 'd' };

  static char_T c9_cv80[9] = { 'm', 'i', 'c', 'r', 'o', 'w', 'a', 'v', 'e' };

  static char_T c9_cv24[8] = { 'e', 'l', 'e', 'p', 'h', 'a', 'n', 't' };

  static char_T c9_cv29[8] = { 'b', 'a', 'c', 'k', 'p', 'a', 'c', 'k' };

  static char_T c9_cv30[8] = { 'u', 'm', 'b', 'r', 'e', 'l', 'l', 'a' };

  static char_T c9_cv35[8] = { 's', 'u', 'i', 't', 'c', 'a', 's', 'e' };

  static char_T c9_cv56[8] = { 's', 'a', 'n', 'd', 'w', 'i', 'c', 'h' };

  static char_T c9_cv58[8] = { 'b', 'r', 'o', 'c', 'c', 'o', 'l', 'i' };

  static char_T c9_cv6[8] = { 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e' };

  static char_T c9_cv78[8] = { 'k', 'e', 'y', 'b', 'o', 'a', 'r', 'd' };

  static char_T c9_cv89[8] = { 's', 'c', 'i', 's', 's', 'o', 'r', 's' };

  static char_T c9_cv27[7] = { 'g', 'i', 'r', 'a', 'f', 'f', 'e' };

  static char_T c9_cv3[7] = { 'b', 'i', 'c', 'y', 'c', 'l', 'e' };

  static char_T c9_cv33[7] = { 'h', 'a', 'n', 'd', 'b', 'a', 'g' };

  static char_T c9_cv36[7] = { 'f', 'r', 'i', 's', 'b', 'e', 'e' };

  static char_T c9_cv60[7] = { 'h', 'o', 't', ' ', 'd', 'o', 'g' };

  static char_T c9_cv82[7] = { 't', 'o', 'a', 's', 't', 'e', 'r' };

  static char_T c9_cv85[7] = { 'b', 'l', 'e', 'n', 'd', 'e', 'r' };

  static char_T c9_cv9[7] = { 'u', 'n', 'k', 'n', 'o', 'w', 'n' };

  static char_T c9_cv2[6] = { 'p', 'e', 'r', 's', 'o', 'n' };

  static char_T c9_cv46[6] = { 'b', 'o', 't', 't', 'l', 'e' };

  static char_T c9_cv54[6] = { 'b', 'a', 'n', 'a', 'n', 'a' };

  static char_T c9_cv57[6] = { 'o', 'r', 'a', 'n', 'g', 'e' };

  static char_T c9_cv59[6] = { 'c', 'a', 'r', 'r', 'o', 't' };

  static char_T c9_cv68[6] = { 'm', 'i', 'r', 'r', 'o', 'r' };

  static char_T c9_cv70[6] = { 'w', 'i', 'n', 'd', 'o', 'w' };

  static char_T c9_cv72[6] = { 't', 'o', 'i', 'l', 'e', 't' };

  static char_T c9_cv75[6] = { 'l', 'a', 'p', 't', 'o', 'p' };

  static char_T c9_cv77[6] = { 'r', 'e', 'm', 'o', 't', 'e' };

  static char_T c9_cv10[5] = { 't', 'r', 'u', 'c', 'k' };

  static char_T c9_cv17[5] = { 'b', 'e', 'n', 'c', 'h' };

  static char_T c9_cv21[5] = { 'h', 'o', 'r', 's', 'e' };

  static char_T c9_cv22[5] = { 's', 'h', 'e', 'e', 'p' };

  static char_T c9_cv26[5] = { 'z', 'e', 'b', 'r', 'a' };

  static char_T c9_cv47[5] = { 'p', 'l', 'a', 't', 'e' };

  static char_T c9_cv51[5] = { 'k', 'n', 'i', 'f', 'e' };

  static char_T c9_cv52[5] = { 's', 'p', 'o', 'o', 'n' };

  static char_T c9_cv55[5] = { 'a', 'p', 'p', 'l', 'e' };

  static char_T c9_cv61[5] = { 'p', 'i', 'z', 'z', 'a' };

  static char_T c9_cv62[5] = { 'd', 'o', 'n', 'u', 't' };

  static char_T c9_cv64[5] = { 'c', 'h', 'a', 'i', 'r' };

  static char_T c9_cv65[5] = { 'c', 'o', 'u', 'c', 'h' };

  static char_T c9_cv76[5] = { 'm', 'o', 'u', 's', 'e' };

  static char_T c9_cv8[5] = { 't', 'r', 'a', 'i', 'n' };

  static char_T c9_cv87[5] = { 'c', 'l', 'o', 'c', 'k' };

  static char_T c9_cv11[4] = { 'b', 'o', 'a', 't' };

  static char_T c9_cv18[4] = { 'b', 'i', 'r', 'd' };

  static char_T c9_cv25[4] = { 'b', 'e', 'a', 'r' };

  static char_T c9_cv31[4] = { 's', 'h', 'o', 'e' };

  static char_T c9_cv37[4] = { 's', 'k', 'i', 's' };

  static char_T c9_cv40[4] = { 'k', 'i', 't', 'e' };

  static char_T c9_cv50[4] = { 'f', 'o', 'r', 'k' };

  static char_T c9_cv53[4] = { 'b', 'o', 'w', 'l' };

  static char_T c9_cv63[4] = { 'c', 'a', 'k', 'e' };

  static char_T c9_cv71[4] = { 'd', 'e', 's', 'k' };

  static char_T c9_cv73[4] = { 'd', 'o', 'o', 'r' };

  static char_T c9_cv81[4] = { 'o', 'v', 'e', 'n' };

  static char_T c9_cv83[4] = { 's', 'i', 'n', 'k' };

  static char_T c9_cv86[4] = { 'b', 'o', 'o', 'k' };

  static char_T c9_cv88[4] = { 'v', 'a', 's', 'e' };

  static char_T c9_cv19[3] = { 'c', 'a', 't' };

  static char_T c9_cv20[3] = { 'd', 'o', 'g' };

  static char_T c9_cv23[3] = { 'c', 'o', 'w' };

  static char_T c9_cv28[3] = { 'h', 'a', 't' };

  static char_T c9_cv34[3] = { 't', 'i', 'e' };

  static char_T c9_cv4[3] = { 'c', 'a', 'r' };

  static char_T c9_cv49[3] = { 'c', 'u', 'p' };

  static char_T c9_cv67[3] = { 'b', 'e', 'd' };

  static char_T c9_cv7[3] = { 'b', 'u', 's' };

  static char_T c9_cv74[2] = { 't', 'v' };

  invokeinterpreter * c9_networkObj;
  c9_cell_wrap_6 c9_r;
  c9_cell_wrap_6 c9_r1;
  c9_cell_wrap_6 c9_r10;
  c9_cell_wrap_6 c9_r11;
  c9_cell_wrap_6 c9_r12;
  c9_cell_wrap_6 c9_r13;
  c9_cell_wrap_6 c9_r14;
  c9_cell_wrap_6 c9_r15;
  c9_cell_wrap_6 c9_r16;
  c9_cell_wrap_6 c9_r17;
  c9_cell_wrap_6 c9_r18;
  c9_cell_wrap_6 c9_r19;
  c9_cell_wrap_6 c9_r2;
  c9_cell_wrap_6 c9_r20;
  c9_cell_wrap_6 c9_r21;
  c9_cell_wrap_6 c9_r22;
  c9_cell_wrap_6 c9_r23;
  c9_cell_wrap_6 c9_r24;
  c9_cell_wrap_6 c9_r25;
  c9_cell_wrap_6 c9_r26;
  c9_cell_wrap_6 c9_r27;
  c9_cell_wrap_6 c9_r28;
  c9_cell_wrap_6 c9_r29;
  c9_cell_wrap_6 c9_r3;
  c9_cell_wrap_6 c9_r30;
  c9_cell_wrap_6 c9_r31;
  c9_cell_wrap_6 c9_r32;
  c9_cell_wrap_6 c9_r33;
  c9_cell_wrap_6 c9_r34;
  c9_cell_wrap_6 c9_r35;
  c9_cell_wrap_6 c9_r36;
  c9_cell_wrap_6 c9_r37;
  c9_cell_wrap_6 c9_r38;
  c9_cell_wrap_6 c9_r39;
  c9_cell_wrap_6 c9_r4;
  c9_cell_wrap_6 c9_r40;
  c9_cell_wrap_6 c9_r41;
  c9_cell_wrap_6 c9_r42;
  c9_cell_wrap_6 c9_r43;
  c9_cell_wrap_6 c9_r44;
  c9_cell_wrap_6 c9_r45;
  c9_cell_wrap_6 c9_r46;
  c9_cell_wrap_6 c9_r47;
  c9_cell_wrap_6 c9_r48;
  c9_cell_wrap_6 c9_r49;
  c9_cell_wrap_6 c9_r5;
  c9_cell_wrap_6 c9_r50;
  c9_cell_wrap_6 c9_r51;
  c9_cell_wrap_6 c9_r52;
  c9_cell_wrap_6 c9_r53;
  c9_cell_wrap_6 c9_r54;
  c9_cell_wrap_6 c9_r55;
  c9_cell_wrap_6 c9_r56;
  c9_cell_wrap_6 c9_r57;
  c9_cell_wrap_6 c9_r58;
  c9_cell_wrap_6 c9_r59;
  c9_cell_wrap_6 c9_r6;
  c9_cell_wrap_6 c9_r60;
  c9_cell_wrap_6 c9_r61;
  c9_cell_wrap_6 c9_r62;
  c9_cell_wrap_6 c9_r63;
  c9_cell_wrap_6 c9_r64;
  c9_cell_wrap_6 c9_r65;
  c9_cell_wrap_6 c9_r66;
  c9_cell_wrap_6 c9_r67;
  c9_cell_wrap_6 c9_r68;
  c9_cell_wrap_6 c9_r69;
  c9_cell_wrap_6 c9_r7;
  c9_cell_wrap_6 c9_r70;
  c9_cell_wrap_6 c9_r71;
  c9_cell_wrap_6 c9_r72;
  c9_cell_wrap_6 c9_r73;
  c9_cell_wrap_6 c9_r74;
  c9_cell_wrap_6 c9_r75;
  c9_cell_wrap_6 c9_r76;
  c9_cell_wrap_6 c9_r77;
  c9_cell_wrap_6 c9_r78;
  c9_cell_wrap_6 c9_r79;
  c9_cell_wrap_6 c9_r8;
  c9_cell_wrap_6 c9_r80;
  c9_cell_wrap_6 c9_r81;
  c9_cell_wrap_6 c9_r82;
  c9_cell_wrap_6 c9_r83;
  c9_cell_wrap_6 c9_r84;
  c9_cell_wrap_6 c9_r85;
  c9_cell_wrap_6 c9_r86;
  c9_cell_wrap_6 c9_r87;
  c9_cell_wrap_6 c9_r88;
  c9_cell_wrap_6 c9_r89;
  c9_cell_wrap_6 c9_r9;
  c9_cell_wrap_6 c9_r90;
  c9_coder_TFLiteModel *c9_b_obj;
  c9_coder_TFLiteModel *c9_c_obj;
  c9_coder_TFLiteModel *c9_iobj_0;
  c9_coder_TFLiteModel *c9_modelObj;
  c9_coder_TFLiteModel *c9_obj;
  c9_coder_TFLiteModel *c9_this;
  emlrtStack c9_b_st;
  emlrtStack c9_st = { NULL,           /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  real_T c9_b_a;
  real_T c9_b_found;
  real_T c9_b_maxIdx;
  real_T c9_d;
  real_T c9_d_varargin_1;
  real_T c9_f_x;
  real_T c9_g_x;
  real_T c9_h_x;
  real_T c9_i_x;
  real_T c9_j_x;
  real_T c9_len;
  real_T c9_maxIdx;
  real_T c9_n;
  int32_T c9_subsa[3];
  int32_T c9_subsb[3];
  int32_T c9_name_size[2];
  int32_T c9_tmp_size[2];
  int32_T c9_b_idx;
  int32_T c9_b_k;
  int32_T c9_b_loop_ub;
  int32_T c9_b_varargin_1;
  int32_T c9_b_varargin_2;
  int32_T c9_c_idx;
  int32_T c9_c_k;
  int32_T c9_c_loop_ub;
  int32_T c9_c_maxIdx;
  int32_T c9_c_varargin_1;
  int32_T c9_c_varargin_2;
  int32_T c9_d_idx;
  int32_T c9_d_k;
  int32_T c9_d_varargin_2;
  int32_T c9_e_k;
  int32_T c9_f_k;
  int32_T c9_first;
  int32_T c9_g_k;
  int32_T c9_h_k;
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i10;
  int32_T c9_i100;
  int32_T c9_i101;
  int32_T c9_i102;
  int32_T c9_i103;
  int32_T c9_i104;
  int32_T c9_i105;
  int32_T c9_i106;
  int32_T c9_i107;
  int32_T c9_i108;
  int32_T c9_i109;
  int32_T c9_i11;
  int32_T c9_i110;
  int32_T c9_i111;
  int32_T c9_i112;
  int32_T c9_i113;
  int32_T c9_i114;
  int32_T c9_i115;
  int32_T c9_i116;
  int32_T c9_i117;
  int32_T c9_i118;
  int32_T c9_i119;
  int32_T c9_i12;
  int32_T c9_i120;
  int32_T c9_i121;
  int32_T c9_i122;
  int32_T c9_i123;
  int32_T c9_i124;
  int32_T c9_i125;
  int32_T c9_i13;
  int32_T c9_i14;
  int32_T c9_i15;
  int32_T c9_i16;
  int32_T c9_i17;
  int32_T c9_i18;
  int32_T c9_i19;
  int32_T c9_i2;
  int32_T c9_i20;
  int32_T c9_i21;
  int32_T c9_i22;
  int32_T c9_i23;
  int32_T c9_i24;
  int32_T c9_i25;
  int32_T c9_i26;
  int32_T c9_i27;
  int32_T c9_i28;
  int32_T c9_i29;
  int32_T c9_i3;
  int32_T c9_i30;
  int32_T c9_i31;
  int32_T c9_i32;
  int32_T c9_i33;
  int32_T c9_i34;
  int32_T c9_i35;
  int32_T c9_i36;
  int32_T c9_i37;
  int32_T c9_i38;
  int32_T c9_i39;
  int32_T c9_i4;
  int32_T c9_i40;
  int32_T c9_i41;
  int32_T c9_i42;
  int32_T c9_i43;
  int32_T c9_i44;
  int32_T c9_i45;
  int32_T c9_i46;
  int32_T c9_i47;
  int32_T c9_i48;
  int32_T c9_i49;
  int32_T c9_i5;
  int32_T c9_i50;
  int32_T c9_i51;
  int32_T c9_i52;
  int32_T c9_i53;
  int32_T c9_i54;
  int32_T c9_i55;
  int32_T c9_i56;
  int32_T c9_i57;
  int32_T c9_i58;
  int32_T c9_i59;
  int32_T c9_i6;
  int32_T c9_i60;
  int32_T c9_i61;
  int32_T c9_i62;
  int32_T c9_i63;
  int32_T c9_i64;
  int32_T c9_i65;
  int32_T c9_i66;
  int32_T c9_i67;
  int32_T c9_i68;
  int32_T c9_i69;
  int32_T c9_i7;
  int32_T c9_i70;
  int32_T c9_i71;
  int32_T c9_i72;
  int32_T c9_i73;
  int32_T c9_i74;
  int32_T c9_i75;
  int32_T c9_i76;
  int32_T c9_i77;
  int32_T c9_i78;
  int32_T c9_i79;
  int32_T c9_i8;
  int32_T c9_i80;
  int32_T c9_i81;
  int32_T c9_i82;
  int32_T c9_i83;
  int32_T c9_i84;
  int32_T c9_i85;
  int32_T c9_i86;
  int32_T c9_i87;
  int32_T c9_i88;
  int32_T c9_i89;
  int32_T c9_i9;
  int32_T c9_i90;
  int32_T c9_i91;
  int32_T c9_i92;
  int32_T c9_i93;
  int32_T c9_i94;
  int32_T c9_i95;
  int32_T c9_i96;
  int32_T c9_i97;
  int32_T c9_i98;
  int32_T c9_i99;
  int32_T c9_idx;
  int32_T c9_iindx;
  int32_T c9_k;
  int32_T c9_loop_ub;
  int32_T c9_value;
  int32_T c9_varargin_1;
  int32_T c9_varargin_2;
  int32_T c9_varargin_3;
  real32_T c9_boxes[400];
  real32_T c9_labels[100];
  real32_T c9_scores[100];
  real32_T c9_b_boxOut[4];
  real32_T c9_a;
  real32_T c9_a__1;
  real32_T c9_b_ex;
  real32_T c9_b_maxScore;
  real32_T c9_b_scoreOut;
  real32_T c9_b_x;
  real32_T c9_c_ex;
  real32_T c9_c_x;
  real32_T c9_d_ex;
  real32_T c9_d_x;
  real32_T c9_ex;
  real32_T c9_labelNum;
  real32_T c9_maxScore;
  real32_T c9_maxval;
  real32_T c9_x;
  char_T c9_cv[21];
  char_T c9_name_data[14];
  uint8_T c9_b_labelOut[20];
  uint8_T c9_tmp_data[14];
  boolean_T c9_b;
  boolean_T c9_b1;
  boolean_T c9_b2;
  boolean_T c9_b3;
  boolean_T c9_b4;
  boolean_T c9_b5;
  boolean_T c9_b6;
  boolean_T c9_b7;
  boolean_T c9_b_b;
  boolean_T c9_b_p;
  boolean_T c9_c_b;
  boolean_T c9_exitg1;
  boolean_T c9_p;
  c9_st.tls = chartInstance->c9_fEmlrtCtx;
  c9_b_st.prev = &c9_st;
  c9_b_st.tls = c9_st.tls;
  for (c9_i = 0; c9_i < 307200; c9_i++) {
    covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 0U, static_cast<real_T>((*
      chartInstance->c9_inputImgR)[c9_i]));
  }

  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c9_JITTransitionAnimation[0] = 0U;
  chartInstance->c9_sfEvent = CALL_EVENT;
  for (c9_i1 = 0; c9_i1 < 307200; c9_i1++) {
    chartInstance->c9_raw3D[c9_i1] = (*chartInstance->c9_inputImgR)[c9_i1];
  }

  covrtEmlFcnEval(chartInstance->c9_covrtInstance, 4U, 0, 0);
  if (covrtEmlIfEval(chartInstance->c9_covrtInstance, 4U, 0, 0,
                     !chartInstance->c9_net_not_empty)) {
    c9_b_st.site = &c9_emlrtRSI;
    c9_iobj_0 = &chartInstance->c9_net;
    c9_obj = c9_iobj_0;
    c9_modelObj = c9_obj;
    c9_modelObj->Mean = 127.5F;
    c9_modelObj->StandardDeviation = 127.5F;
    c9_modelObj->EnableProfiling = false;
    c9_modelObj->Verbose = false;
    c9_this = c9_modelObj;
    c9_modelObj = c9_this;
    c9_b_obj = c9_modelObj;
    c9_b_obj->InternalNumThreads = 6;
    c9_networkObj = new invokeinterpreter;
    for (c9_i8 = 0; c9_i8 < 21; c9_i8++) {
      c9_cv[c9_i8] = c9_cv1[c9_i8];
    }

    c9_c_obj = c9_modelObj;
    c9_value = c9_c_obj->InternalNumThreads;
    std::mem_fn(&invokeinterpreter::setup)(c9_networkObj, &c9_cv[0], c9_value);
    c9_modelObj->Network = c9_networkObj;
    for (c9_i9 = 0; c9_i9 < 601; c9_i9++) {
      c9_modelObj->PredictedTotalOutput[c9_i9] = 0.0F;
    }

    c9_modelObj->matlabCodegenIsDeleted = false;
    chartInstance->c9_net_not_empty = true;
    c9_r.f1.size[0] = 1;
    c9_r.f1.size[1] = 6;
    for (c9_i11 = 0; c9_i11 < 6; c9_i11++) {
      c9_r.f1.data[c9_i11] = c9_cv2[c9_i11];
    }

    c9_r1.f1.size[0] = 1;
    c9_r1.f1.size[1] = 7;
    for (c9_i12 = 0; c9_i12 < 7; c9_i12++) {
      c9_r1.f1.data[c9_i12] = c9_cv3[c9_i12];
    }

    c9_r2.f1.size[0] = 1;
    c9_r2.f1.size[1] = 3;
    for (c9_i13 = 0; c9_i13 < 3; c9_i13++) {
      c9_r2.f1.data[c9_i13] = c9_cv4[c9_i13];
    }

    c9_r3.f1.size[0] = 1;
    c9_r3.f1.size[1] = 10;
    for (c9_i15 = 0; c9_i15 < 10; c9_i15++) {
      c9_r3.f1.data[c9_i15] = c9_cv5[c9_i15];
    }

    c9_r4.f1.size[0] = 1;
    c9_r4.f1.size[1] = 8;
    for (c9_i17 = 0; c9_i17 < 8; c9_i17++) {
      c9_r4.f1.data[c9_i17] = c9_cv6[c9_i17];
    }

    c9_r5.f1.size[0] = 1;
    c9_r5.f1.size[1] = 3;
    for (c9_i19 = 0; c9_i19 < 3; c9_i19++) {
      c9_r5.f1.data[c9_i19] = c9_cv7[c9_i19];
    }

    c9_r6.f1.size[0] = 1;
    c9_r6.f1.size[1] = 5;
    for (c9_i20 = 0; c9_i20 < 5; c9_i20++) {
      c9_r6.f1.data[c9_i20] = c9_cv8[c9_i20];
    }

    c9_r7.f1.size[0] = 1;
    c9_r7.f1.size[1] = 5;
    for (c9_i23 = 0; c9_i23 < 5; c9_i23++) {
      c9_r7.f1.data[c9_i23] = c9_cv10[c9_i23];
    }

    c9_r8.f1.size[0] = 1;
    c9_r8.f1.size[1] = 4;
    for (c9_i25 = 0; c9_i25 < 4; c9_i25++) {
      c9_r8.f1.data[c9_i25] = c9_cv11[c9_i25];
    }

    c9_r9.f1.size[0] = 1;
    c9_r9.f1.size[1] = 13;
    for (c9_i27 = 0; c9_i27 < 13; c9_i27++) {
      c9_r9.f1.data[c9_i27] = c9_cv12[c9_i27];
    }

    c9_r10.f1.size[0] = 1;
    c9_r10.f1.size[1] = 12;
    for (c9_i28 = 0; c9_i28 < 12; c9_i28++) {
      c9_r10.f1.data[c9_i28] = c9_cv13[c9_i28];
    }

    c9_r11.f1.size[0] = 1;
    c9_r11.f1.size[1] = 11;
    for (c9_i29 = 0; c9_i29 < 11; c9_i29++) {
      c9_r11.f1.data[c9_i29] = c9_cv14[c9_i29];
    }

    c9_r12.f1.size[0] = 1;
    c9_r12.f1.size[1] = 9;
    for (c9_i33 = 0; c9_i33 < 9; c9_i33++) {
      c9_r12.f1.data[c9_i33] = c9_cv15[c9_i33];
    }

    c9_r13.f1.size[0] = 1;
    c9_r13.f1.size[1] = 13;
    for (c9_i35 = 0; c9_i35 < 13; c9_i35++) {
      c9_r13.f1.data[c9_i35] = c9_cv16[c9_i35];
    }

    c9_r14.f1.size[0] = 1;
    c9_r14.f1.size[1] = 5;
    for (c9_i38 = 0; c9_i38 < 5; c9_i38++) {
      c9_r14.f1.data[c9_i38] = c9_cv17[c9_i38];
    }

    c9_r15.f1.size[0] = 1;
    c9_r15.f1.size[1] = 4;
    for (c9_i41 = 0; c9_i41 < 4; c9_i41++) {
      c9_r15.f1.data[c9_i41] = c9_cv18[c9_i41];
    }

    c9_r16.f1.size[0] = 1;
    c9_r16.f1.size[1] = 3;
    for (c9_i44 = 0; c9_i44 < 3; c9_i44++) {
      c9_r16.f1.data[c9_i44] = c9_cv19[c9_i44];
    }

    c9_r17.f1.size[0] = 1;
    c9_r17.f1.size[1] = 3;
    for (c9_i46 = 0; c9_i46 < 3; c9_i46++) {
      c9_r17.f1.data[c9_i46] = c9_cv20[c9_i46];
    }

    c9_r18.f1.size[0] = 1;
    c9_r18.f1.size[1] = 5;
    for (c9_i48 = 0; c9_i48 < 5; c9_i48++) {
      c9_r18.f1.data[c9_i48] = c9_cv21[c9_i48];
    }

    c9_r19.f1.size[0] = 1;
    c9_r19.f1.size[1] = 5;
    for (c9_i51 = 0; c9_i51 < 5; c9_i51++) {
      c9_r19.f1.data[c9_i51] = c9_cv22[c9_i51];
    }

    c9_r20.f1.size[0] = 1;
    c9_r20.f1.size[1] = 3;
    for (c9_i53 = 0; c9_i53 < 3; c9_i53++) {
      c9_r20.f1.data[c9_i53] = c9_cv23[c9_i53];
    }

    c9_r21.f1.size[0] = 1;
    c9_r21.f1.size[1] = 8;
    for (c9_i56 = 0; c9_i56 < 8; c9_i56++) {
      c9_r21.f1.data[c9_i56] = c9_cv24[c9_i56];
    }

    c9_r22.f1.size[0] = 1;
    c9_r22.f1.size[1] = 4;
    for (c9_i57 = 0; c9_i57 < 4; c9_i57++) {
      c9_r22.f1.data[c9_i57] = c9_cv25[c9_i57];
    }

    c9_r23.f1.size[0] = 1;
    c9_r23.f1.size[1] = 5;
    for (c9_i58 = 0; c9_i58 < 5; c9_i58++) {
      c9_r23.f1.data[c9_i58] = c9_cv26[c9_i58];
    }

    c9_r24.f1.size[0] = 1;
    c9_r24.f1.size[1] = 7;
    for (c9_i59 = 0; c9_i59 < 7; c9_i59++) {
      c9_r24.f1.data[c9_i59] = c9_cv27[c9_i59];
    }

    c9_r25.f1.size[0] = 1;
    c9_r25.f1.size[1] = 3;
    for (c9_i60 = 0; c9_i60 < 3; c9_i60++) {
      c9_r25.f1.data[c9_i60] = c9_cv28[c9_i60];
    }

    c9_r26.f1.size[0] = 1;
    c9_r26.f1.size[1] = 8;
    for (c9_i61 = 0; c9_i61 < 8; c9_i61++) {
      c9_r26.f1.data[c9_i61] = c9_cv29[c9_i61];
    }

    c9_r27.f1.size[0] = 1;
    c9_r27.f1.size[1] = 8;
    for (c9_i62 = 0; c9_i62 < 8; c9_i62++) {
      c9_r27.f1.data[c9_i62] = c9_cv30[c9_i62];
    }

    c9_r28.f1.size[0] = 1;
    c9_r28.f1.size[1] = 4;
    for (c9_i63 = 0; c9_i63 < 4; c9_i63++) {
      c9_r28.f1.data[c9_i63] = c9_cv31[c9_i63];
    }

    c9_r29.f1.size[0] = 1;
    c9_r29.f1.size[1] = 11;
    for (c9_i64 = 0; c9_i64 < 11; c9_i64++) {
      c9_r29.f1.data[c9_i64] = c9_cv32[c9_i64];
    }

    c9_r30.f1.size[0] = 1;
    c9_r30.f1.size[1] = 7;
    for (c9_i65 = 0; c9_i65 < 7; c9_i65++) {
      c9_r30.f1.data[c9_i65] = c9_cv33[c9_i65];
    }

    c9_r31.f1.size[0] = 1;
    c9_r31.f1.size[1] = 3;
    for (c9_i66 = 0; c9_i66 < 3; c9_i66++) {
      c9_r31.f1.data[c9_i66] = c9_cv34[c9_i66];
    }

    c9_r32.f1.size[0] = 1;
    c9_r32.f1.size[1] = 8;
    for (c9_i67 = 0; c9_i67 < 8; c9_i67++) {
      c9_r32.f1.data[c9_i67] = c9_cv35[c9_i67];
    }

    c9_r33.f1.size[0] = 1;
    c9_r33.f1.size[1] = 7;
    for (c9_i68 = 0; c9_i68 < 7; c9_i68++) {
      c9_r33.f1.data[c9_i68] = c9_cv36[c9_i68];
    }

    c9_r34.f1.size[0] = 1;
    c9_r34.f1.size[1] = 4;
    for (c9_i69 = 0; c9_i69 < 4; c9_i69++) {
      c9_r34.f1.data[c9_i69] = c9_cv37[c9_i69];
    }

    c9_r35.f1.size[0] = 1;
    c9_r35.f1.size[1] = 9;
    for (c9_i70 = 0; c9_i70 < 9; c9_i70++) {
      c9_r35.f1.data[c9_i70] = c9_cv38[c9_i70];
    }

    c9_r36.f1.size[0] = 1;
    c9_r36.f1.size[1] = 11;
    for (c9_i71 = 0; c9_i71 < 11; c9_i71++) {
      c9_r36.f1.data[c9_i71] = c9_cv39[c9_i71];
    }

    c9_r37.f1.size[0] = 1;
    c9_r37.f1.size[1] = 4;
    for (c9_i72 = 0; c9_i72 < 4; c9_i72++) {
      c9_r37.f1.data[c9_i72] = c9_cv40[c9_i72];
    }

    c9_r38.f1.size[0] = 1;
    c9_r38.f1.size[1] = 12;
    for (c9_i73 = 0; c9_i73 < 12; c9_i73++) {
      c9_r38.f1.data[c9_i73] = c9_cv41[c9_i73];
    }

    c9_r39.f1.size[0] = 1;
    c9_r39.f1.size[1] = 14;
    for (c9_i74 = 0; c9_i74 < 14; c9_i74++) {
      c9_r39.f1.data[c9_i74] = c9_cv42[c9_i74];
    }

    c9_r40.f1.size[0] = 1;
    c9_r40.f1.size[1] = 10;
    for (c9_i75 = 0; c9_i75 < 10; c9_i75++) {
      c9_r40.f1.data[c9_i75] = c9_cv43[c9_i75];
    }

    c9_r41.f1.size[0] = 1;
    c9_r41.f1.size[1] = 9;
    for (c9_i76 = 0; c9_i76 < 9; c9_i76++) {
      c9_r41.f1.data[c9_i76] = c9_cv44[c9_i76];
    }

    c9_r42.f1.size[0] = 1;
    c9_r42.f1.size[1] = 13;
    for (c9_i77 = 0; c9_i77 < 13; c9_i77++) {
      c9_r42.f1.data[c9_i77] = c9_cv45[c9_i77];
    }

    c9_r43.f1.size[0] = 1;
    c9_r43.f1.size[1] = 6;
    for (c9_i78 = 0; c9_i78 < 6; c9_i78++) {
      c9_r43.f1.data[c9_i78] = c9_cv46[c9_i78];
    }

    c9_r44.f1.size[0] = 1;
    c9_r44.f1.size[1] = 5;
    for (c9_i79 = 0; c9_i79 < 5; c9_i79++) {
      c9_r44.f1.data[c9_i79] = c9_cv47[c9_i79];
    }

    c9_r45.f1.size[0] = 1;
    c9_r45.f1.size[1] = 10;
    for (c9_i80 = 0; c9_i80 < 10; c9_i80++) {
      c9_r45.f1.data[c9_i80] = c9_cv48[c9_i80];
    }

    c9_r46.f1.size[0] = 1;
    c9_r46.f1.size[1] = 3;
    for (c9_i81 = 0; c9_i81 < 3; c9_i81++) {
      c9_r46.f1.data[c9_i81] = c9_cv49[c9_i81];
    }

    c9_r47.f1.size[0] = 1;
    c9_r47.f1.size[1] = 4;
    for (c9_i82 = 0; c9_i82 < 4; c9_i82++) {
      c9_r47.f1.data[c9_i82] = c9_cv50[c9_i82];
    }

    c9_r48.f1.size[0] = 1;
    c9_r48.f1.size[1] = 5;
    for (c9_i83 = 0; c9_i83 < 5; c9_i83++) {
      c9_r48.f1.data[c9_i83] = c9_cv51[c9_i83];
    }

    c9_r49.f1.size[0] = 1;
    c9_r49.f1.size[1] = 5;
    for (c9_i84 = 0; c9_i84 < 5; c9_i84++) {
      c9_r49.f1.data[c9_i84] = c9_cv52[c9_i84];
    }

    c9_r50.f1.size[0] = 1;
    c9_r50.f1.size[1] = 4;
    for (c9_i85 = 0; c9_i85 < 4; c9_i85++) {
      c9_r50.f1.data[c9_i85] = c9_cv53[c9_i85];
    }

    c9_r51.f1.size[0] = 1;
    c9_r51.f1.size[1] = 6;
    for (c9_i86 = 0; c9_i86 < 6; c9_i86++) {
      c9_r51.f1.data[c9_i86] = c9_cv54[c9_i86];
    }

    c9_r52.f1.size[0] = 1;
    c9_r52.f1.size[1] = 5;
    for (c9_i87 = 0; c9_i87 < 5; c9_i87++) {
      c9_r52.f1.data[c9_i87] = c9_cv55[c9_i87];
    }

    c9_r53.f1.size[0] = 1;
    c9_r53.f1.size[1] = 8;
    for (c9_i88 = 0; c9_i88 < 8; c9_i88++) {
      c9_r53.f1.data[c9_i88] = c9_cv56[c9_i88];
    }

    c9_r54.f1.size[0] = 1;
    c9_r54.f1.size[1] = 6;
    for (c9_i89 = 0; c9_i89 < 6; c9_i89++) {
      c9_r54.f1.data[c9_i89] = c9_cv57[c9_i89];
    }

    c9_r55.f1.size[0] = 1;
    c9_r55.f1.size[1] = 8;
    for (c9_i90 = 0; c9_i90 < 8; c9_i90++) {
      c9_r55.f1.data[c9_i90] = c9_cv58[c9_i90];
    }

    c9_r56.f1.size[0] = 1;
    c9_r56.f1.size[1] = 6;
    for (c9_i91 = 0; c9_i91 < 6; c9_i91++) {
      c9_r56.f1.data[c9_i91] = c9_cv59[c9_i91];
    }

    c9_r57.f1.size[0] = 1;
    c9_r57.f1.size[1] = 7;
    for (c9_i92 = 0; c9_i92 < 7; c9_i92++) {
      c9_r57.f1.data[c9_i92] = c9_cv60[c9_i92];
    }

    c9_r58.f1.size[0] = 1;
    c9_r58.f1.size[1] = 5;
    for (c9_i93 = 0; c9_i93 < 5; c9_i93++) {
      c9_r58.f1.data[c9_i93] = c9_cv61[c9_i93];
    }

    c9_r59.f1.size[0] = 1;
    c9_r59.f1.size[1] = 5;
    for (c9_i94 = 0; c9_i94 < 5; c9_i94++) {
      c9_r59.f1.data[c9_i94] = c9_cv62[c9_i94];
    }

    c9_r60.f1.size[0] = 1;
    c9_r60.f1.size[1] = 4;
    for (c9_i95 = 0; c9_i95 < 4; c9_i95++) {
      c9_r60.f1.data[c9_i95] = c9_cv63[c9_i95];
    }

    c9_r61.f1.size[0] = 1;
    c9_r61.f1.size[1] = 5;
    for (c9_i96 = 0; c9_i96 < 5; c9_i96++) {
      c9_r61.f1.data[c9_i96] = c9_cv64[c9_i96];
    }

    c9_r62.f1.size[0] = 1;
    c9_r62.f1.size[1] = 5;
    for (c9_i97 = 0; c9_i97 < 5; c9_i97++) {
      c9_r62.f1.data[c9_i97] = c9_cv65[c9_i97];
    }

    c9_r63.f1.size[0] = 1;
    c9_r63.f1.size[1] = 12;
    for (c9_i98 = 0; c9_i98 < 12; c9_i98++) {
      c9_r63.f1.data[c9_i98] = c9_cv66[c9_i98];
    }

    c9_r64.f1.size[0] = 1;
    c9_r64.f1.size[1] = 3;
    for (c9_i99 = 0; c9_i99 < 3; c9_i99++) {
      c9_r64.f1.data[c9_i99] = c9_cv67[c9_i99];
    }

    c9_r65.f1.size[0] = 1;
    c9_r65.f1.size[1] = 6;
    for (c9_i100 = 0; c9_i100 < 6; c9_i100++) {
      c9_r65.f1.data[c9_i100] = c9_cv68[c9_i100];
    }

    c9_r66.f1.size[0] = 1;
    c9_r66.f1.size[1] = 12;
    for (c9_i101 = 0; c9_i101 < 12; c9_i101++) {
      c9_r66.f1.data[c9_i101] = c9_cv69[c9_i101];
    }

    c9_r67.f1.size[0] = 1;
    c9_r67.f1.size[1] = 6;
    for (c9_i102 = 0; c9_i102 < 6; c9_i102++) {
      c9_r67.f1.data[c9_i102] = c9_cv70[c9_i102];
    }

    c9_r68.f1.size[0] = 1;
    c9_r68.f1.size[1] = 4;
    for (c9_i103 = 0; c9_i103 < 4; c9_i103++) {
      c9_r68.f1.data[c9_i103] = c9_cv71[c9_i103];
    }

    c9_r69.f1.size[0] = 1;
    c9_r69.f1.size[1] = 6;
    for (c9_i104 = 0; c9_i104 < 6; c9_i104++) {
      c9_r69.f1.data[c9_i104] = c9_cv72[c9_i104];
    }

    c9_r70.f1.size[0] = 1;
    c9_r70.f1.size[1] = 4;
    for (c9_i105 = 0; c9_i105 < 4; c9_i105++) {
      c9_r70.f1.data[c9_i105] = c9_cv73[c9_i105];
    }

    c9_r71.f1.size[0] = 1;
    c9_r71.f1.size[1] = 2;
    for (c9_i106 = 0; c9_i106 < 2; c9_i106++) {
      c9_r71.f1.data[c9_i106] = c9_cv74[c9_i106];
    }

    c9_r72.f1.size[0] = 1;
    c9_r72.f1.size[1] = 6;
    for (c9_i107 = 0; c9_i107 < 6; c9_i107++) {
      c9_r72.f1.data[c9_i107] = c9_cv75[c9_i107];
    }

    c9_r73.f1.size[0] = 1;
    c9_r73.f1.size[1] = 5;
    for (c9_i108 = 0; c9_i108 < 5; c9_i108++) {
      c9_r73.f1.data[c9_i108] = c9_cv76[c9_i108];
    }

    c9_r74.f1.size[0] = 1;
    c9_r74.f1.size[1] = 6;
    for (c9_i109 = 0; c9_i109 < 6; c9_i109++) {
      c9_r74.f1.data[c9_i109] = c9_cv77[c9_i109];
    }

    c9_r75.f1.size[0] = 1;
    c9_r75.f1.size[1] = 8;
    for (c9_i110 = 0; c9_i110 < 8; c9_i110++) {
      c9_r75.f1.data[c9_i110] = c9_cv78[c9_i110];
    }

    c9_r76.f1.size[0] = 1;
    c9_r76.f1.size[1] = 10;
    for (c9_i111 = 0; c9_i111 < 10; c9_i111++) {
      c9_r76.f1.data[c9_i111] = c9_cv79[c9_i111];
    }

    c9_r77.f1.size[0] = 1;
    c9_r77.f1.size[1] = 9;
    for (c9_i112 = 0; c9_i112 < 9; c9_i112++) {
      c9_r77.f1.data[c9_i112] = c9_cv80[c9_i112];
    }

    c9_r78.f1.size[0] = 1;
    c9_r78.f1.size[1] = 4;
    for (c9_i113 = 0; c9_i113 < 4; c9_i113++) {
      c9_r78.f1.data[c9_i113] = c9_cv81[c9_i113];
    }

    c9_r79.f1.size[0] = 1;
    c9_r79.f1.size[1] = 7;
    for (c9_i114 = 0; c9_i114 < 7; c9_i114++) {
      c9_r79.f1.data[c9_i114] = c9_cv82[c9_i114];
    }

    c9_r80.f1.size[0] = 1;
    c9_r80.f1.size[1] = 4;
    for (c9_i115 = 0; c9_i115 < 4; c9_i115++) {
      c9_r80.f1.data[c9_i115] = c9_cv83[c9_i115];
    }

    c9_r81.f1.size[0] = 1;
    c9_r81.f1.size[1] = 12;
    for (c9_i116 = 0; c9_i116 < 12; c9_i116++) {
      c9_r81.f1.data[c9_i116] = c9_cv84[c9_i116];
    }

    c9_r82.f1.size[0] = 1;
    c9_r82.f1.size[1] = 7;
    for (c9_i117 = 0; c9_i117 < 7; c9_i117++) {
      c9_r82.f1.data[c9_i117] = c9_cv85[c9_i117];
    }

    c9_r83.f1.size[0] = 1;
    c9_r83.f1.size[1] = 4;
    for (c9_i118 = 0; c9_i118 < 4; c9_i118++) {
      c9_r83.f1.data[c9_i118] = c9_cv86[c9_i118];
    }

    c9_r84.f1.size[0] = 1;
    c9_r84.f1.size[1] = 5;
    for (c9_i119 = 0; c9_i119 < 5; c9_i119++) {
      c9_r84.f1.data[c9_i119] = c9_cv87[c9_i119];
    }

    c9_r85.f1.size[0] = 1;
    c9_r85.f1.size[1] = 4;
    for (c9_i120 = 0; c9_i120 < 4; c9_i120++) {
      c9_r85.f1.data[c9_i120] = c9_cv88[c9_i120];
    }

    c9_r86.f1.size[0] = 1;
    c9_r86.f1.size[1] = 8;
    for (c9_i121 = 0; c9_i121 < 8; c9_i121++) {
      c9_r86.f1.data[c9_i121] = c9_cv89[c9_i121];
    }

    c9_r87.f1.size[0] = 1;
    c9_r87.f1.size[1] = 10;
    for (c9_i122 = 0; c9_i122 < 10; c9_i122++) {
      c9_r87.f1.data[c9_i122] = c9_cv90[c9_i122];
    }

    c9_r88.f1.size[0] = 1;
    c9_r88.f1.size[1] = 10;
    for (c9_i123 = 0; c9_i123 < 10; c9_i123++) {
      c9_r88.f1.data[c9_i123] = c9_cv91[c9_i123];
    }

    c9_r89.f1.size[0] = 1;
    c9_r89.f1.size[1] = 10;
    for (c9_i124 = 0; c9_i124 < 10; c9_i124++) {
      c9_r89.f1.data[c9_i124] = c9_cv92[c9_i124];
    }

    c9_r90.f1.size[0] = 1;
    c9_r90.f1.size[1] = 10;
    for (c9_i125 = 0; c9_i125 < 10; c9_i125++) {
      c9_r90.f1.data[c9_i125] = c9_cv93[c9_i125];
    }

    chartInstance->c9_cocoLabels[0] = c9_r;
    chartInstance->c9_cocoLabels[1] = c9_r1;
    chartInstance->c9_cocoLabels[2] = c9_r2;
    chartInstance->c9_cocoLabels[3] = c9_r3;
    chartInstance->c9_cocoLabels[4] = c9_r4;
    chartInstance->c9_cocoLabels[5] = c9_r5;
    chartInstance->c9_cocoLabels[6] = c9_r6;
    chartInstance->c9_cocoLabels[7] = c9_r7;
    chartInstance->c9_cocoLabels[8] = c9_r8;
    chartInstance->c9_cocoLabels[9] = c9_r9;
    chartInstance->c9_cocoLabels[10] = c9_r10;
    chartInstance->c9_cocoLabels[11] = c9_r11;
    chartInstance->c9_cocoLabels[12] = c9_r12;
    chartInstance->c9_cocoLabels[13] = c9_r13;
    chartInstance->c9_cocoLabels[14] = c9_r14;
    chartInstance->c9_cocoLabels[15] = c9_r15;
    chartInstance->c9_cocoLabels[16] = c9_r16;
    chartInstance->c9_cocoLabels[17] = c9_r17;
    chartInstance->c9_cocoLabels[18] = c9_r18;
    chartInstance->c9_cocoLabels[19] = c9_r19;
    chartInstance->c9_cocoLabels[20] = c9_r20;
    chartInstance->c9_cocoLabels[21] = c9_r21;
    chartInstance->c9_cocoLabels[22] = c9_r22;
    chartInstance->c9_cocoLabels[23] = c9_r23;
    chartInstance->c9_cocoLabels[24] = c9_r24;
    chartInstance->c9_cocoLabels[25] = c9_r25;
    chartInstance->c9_cocoLabels[26] = c9_r26;
    chartInstance->c9_cocoLabels[27] = c9_r27;
    chartInstance->c9_cocoLabels[28] = c9_r28;
    chartInstance->c9_cocoLabels[29] = c9_r29;
    chartInstance->c9_cocoLabels[30] = c9_r30;
    chartInstance->c9_cocoLabels[31] = c9_r31;
    chartInstance->c9_cocoLabels[32] = c9_r32;
    chartInstance->c9_cocoLabels[33] = c9_r33;
    chartInstance->c9_cocoLabels[34] = c9_r34;
    chartInstance->c9_cocoLabels[35] = c9_r35;
    chartInstance->c9_cocoLabels[36] = c9_r36;
    chartInstance->c9_cocoLabels[37] = c9_r37;
    chartInstance->c9_cocoLabels[38] = c9_r38;
    chartInstance->c9_cocoLabels[39] = c9_r39;
    chartInstance->c9_cocoLabels[40] = c9_r40;
    chartInstance->c9_cocoLabels[41] = c9_r41;
    chartInstance->c9_cocoLabels[42] = c9_r42;
    chartInstance->c9_cocoLabels[43] = c9_r43;
    chartInstance->c9_cocoLabels[44] = c9_r44;
    chartInstance->c9_cocoLabels[45] = c9_r45;
    chartInstance->c9_cocoLabels[46] = c9_r46;
    chartInstance->c9_cocoLabels[47] = c9_r47;
    chartInstance->c9_cocoLabels[48] = c9_r48;
    chartInstance->c9_cocoLabels[49] = c9_r49;
    chartInstance->c9_cocoLabels[50] = c9_r50;
    chartInstance->c9_cocoLabels[51] = c9_r51;
    chartInstance->c9_cocoLabels[52] = c9_r52;
    chartInstance->c9_cocoLabels[53] = c9_r53;
    chartInstance->c9_cocoLabels[54] = c9_r54;
    chartInstance->c9_cocoLabels[55] = c9_r55;
    chartInstance->c9_cocoLabels[56] = c9_r56;
    chartInstance->c9_cocoLabels[57] = c9_r57;
    chartInstance->c9_cocoLabels[58] = c9_r58;
    chartInstance->c9_cocoLabels[59] = c9_r59;
    chartInstance->c9_cocoLabels[60] = c9_r60;
    chartInstance->c9_cocoLabels[61] = c9_r61;
    chartInstance->c9_cocoLabels[62] = c9_r62;
    chartInstance->c9_cocoLabels[63] = c9_r63;
    chartInstance->c9_cocoLabels[64] = c9_r64;
    chartInstance->c9_cocoLabels[65] = c9_r65;
    chartInstance->c9_cocoLabels[66] = c9_r66;
    chartInstance->c9_cocoLabels[67] = c9_r67;
    chartInstance->c9_cocoLabels[68] = c9_r68;
    chartInstance->c9_cocoLabels[69] = c9_r69;
    chartInstance->c9_cocoLabels[70] = c9_r70;
    chartInstance->c9_cocoLabels[71] = c9_r71;
    chartInstance->c9_cocoLabels[72] = c9_r72;
    chartInstance->c9_cocoLabels[73] = c9_r73;
    chartInstance->c9_cocoLabels[74] = c9_r74;
    chartInstance->c9_cocoLabels[75] = c9_r75;
    chartInstance->c9_cocoLabels[76] = c9_r76;
    chartInstance->c9_cocoLabels[77] = c9_r77;
    chartInstance->c9_cocoLabels[78] = c9_r78;
    chartInstance->c9_cocoLabels[79] = c9_r79;
    chartInstance->c9_cocoLabels[80] = c9_r80;
    chartInstance->c9_cocoLabels[81] = c9_r81;
    chartInstance->c9_cocoLabels[82] = c9_r82;
    chartInstance->c9_cocoLabels[83] = c9_r83;
    chartInstance->c9_cocoLabels[84] = c9_r84;
    chartInstance->c9_cocoLabels[85] = c9_r85;
    chartInstance->c9_cocoLabels[86] = c9_r86;
    chartInstance->c9_cocoLabels[87] = c9_r87;
    chartInstance->c9_cocoLabels[88] = c9_r88;
    chartInstance->c9_cocoLabels[89] = c9_r89;
    chartInstance->c9_cocoLabels[90] = c9_r90;
    chartInstance->c9_cocoLabels_not_empty = true;
  }

  for (c9_k = 0; c9_k < 320; c9_k++) {
    c9_b_k = c9_k;
    c9_varargin_1 = c9_b_k + 1;
    for (c9_c_k = 0; c9_c_k < 320; c9_c_k++) {
      c9_d_k = c9_c_k;
      c9_b_varargin_1 = c9_d_k + 1;
      c9_varargin_2 = c9_varargin_1;
      for (c9_e_k = 0; c9_e_k < 3; c9_e_k++) {
        c9_f_k = c9_e_k;
        c9_c_varargin_1 = c9_f_k + 1;
        c9_b_varargin_2 = c9_b_varargin_1;
        c9_varargin_3 = c9_varargin_2;
        c9_subsa[0] = c9_c_varargin_1;
        c9_subsa[1] = c9_b_varargin_2;
        c9_subsa[2] = c9_varargin_3;
        c9_subsb[0] = c9_subsa[2];
        c9_subsb[1] = c9_subsa[1];
        c9_subsb[2] = c9_subsa[0];
        chartInstance->c9_b_imgRestored[((c9_subsb[0] + 320 * (c9_subsb[1] - 1))
          + 102400 * (c9_subsb[2] - 1)) - 1] = chartInstance->c9_raw3D
          [((c9_subsa[0] + 3 * (c9_subsa[1] - 1)) + 960 * (c9_subsa[2] - 1)) - 1];
      }
    }
  }

  c9_i2 = 0;
  for (c9_i3 = 0; c9_i3 < 3; c9_i3++) {
    c9_i5 = 0;
    for (c9_i6 = 0; c9_i6 < 320; c9_i6++) {
      for (c9_i7 = 0; c9_i7 < 320; c9_i7++) {
        chartInstance->c9_imgRestored[(c9_i7 + c9_i5) + c9_i2] =
          chartInstance->c9_b_imgRestored[((c9_i7 + c9_i5) - c9_i2) + 204800];
      }

      c9_i5 += 320;
    }

    c9_i2 += 102400;
  }

  for (c9_i4 = 0; c9_i4 < 307200; c9_i4++) {
    chartInstance->c9_raw3D[c9_i4] = chartInstance->c9_imgRestored[c9_i4];
  }

  c9_b_st.site = &c9_b_emlrtRSI;
  c9_TFLiteModel_predict(chartInstance, &c9_b_st, &chartInstance->c9_net,
    chartInstance->c9_raw3D, c9_boxes, c9_labels, c9_scores, &c9_a__1);
  c9_x = c9_scores[0];
  c9_b_x = c9_x;
  c9_b = muSingleScalarIsNaN(c9_b_x);
  c9_p = !c9_b;
  if (c9_p) {
    c9_idx = 1;
  } else {
    c9_idx = 0;
    c9_g_k = 2;
    c9_exitg1 = false;
    while ((!c9_exitg1) && (c9_g_k < 101)) {
      c9_c_x = c9_scores[c9_g_k - 1];
      c9_d_x = c9_c_x;
      c9_b_b = muSingleScalarIsNaN(c9_d_x);
      c9_b_p = !c9_b_b;
      if (c9_b_p) {
        c9_idx = c9_g_k;
        c9_exitg1 = true;
      } else {
        c9_g_k++;
      }
    }
  }

  if (c9_idx == 0) {
    c9_ex = c9_scores[0];
    c9_idx = 1;
  } else {
    c9_first = c9_idx;
    c9_b_ex = c9_scores[c9_first - 1];
    c9_b_idx = c9_first;
    c9_i10 = c9_first;
    for (c9_h_k = c9_i10 + 1; c9_h_k < 101; c9_h_k++) {
      if (c9_b_ex < c9_scores[c9_h_k - 1]) {
        c9_b_ex = c9_scores[c9_h_k - 1];
        c9_b_idx = c9_h_k;
      }
    }

    c9_ex = c9_b_ex;
    c9_idx = c9_b_idx;
  }

  c9_c_ex = c9_ex;
  c9_c_idx = c9_idx;
  c9_d_ex = c9_c_ex;
  c9_d_idx = c9_c_idx;
  c9_maxval = c9_d_ex;
  c9_iindx = c9_d_idx;
  c9_maxScore = c9_maxval;
  c9_maxIdx = static_cast<real_T>(c9_iindx);
  c9_b_maxScore = c9_maxScore;
  c9_b_maxIdx = c9_maxIdx;
  c9_a = c9_b_maxScore;
  c9_b_scoreOut = c9_a * 100.0F;
  if (c9_b_maxIdx != static_cast<real_T>(static_cast<int32_T>
       (muDoubleScalarFloor(c9_b_maxIdx)))) {
    emlrtIntegerCheckR2012b(c9_b_maxIdx, &c9_emlrtDCI, &c9_st);
  }

  c9_i14 = static_cast<int32_T>(c9_b_maxIdx);
  if ((c9_i14 < 1) || (c9_i14 > 100)) {
    emlrtDynamicBoundsCheckR2012b(c9_i14, 1, 100, &c9_emlrtBCI, &c9_st);
  }

  c9_c_maxIdx = c9_i14 - 1;
  for (c9_i16 = 0; c9_i16 < 4; c9_i16++) {
    c9_b_boxOut[c9_i16] = c9_boxes[c9_c_maxIdx + 100 * c9_i16];
  }

  if (c9_b_maxIdx != static_cast<real_T>(static_cast<int32_T>
       (muDoubleScalarFloor(c9_b_maxIdx)))) {
    emlrtIntegerCheckR2012b(c9_b_maxIdx, &c9_e_emlrtDCI, &c9_st);
  }

  c9_i18 = static_cast<int32_T>(c9_b_maxIdx);
  if ((c9_i18 < 1) || (c9_i18 > 100)) {
    emlrtDynamicBoundsCheckR2012b(c9_i18, 1, 100, &c9_f_emlrtBCI, &c9_st);
  }

  c9_labelNum = c9_labels[c9_i18 - 1] + 1.0F;
  if (covrtEmlCondEval(chartInstance->c9_covrtInstance, 4U, 0, 0,
                       covrtRelationalopUpdateFcn
                       (chartInstance->c9_covrtInstance, 4U, 0U, 0U,
                        static_cast<real_T>(c9_labelNum), 1.0, -1, 5U,
                        static_cast<int32_T>(c9_labelNum >= 1.0F))) &&
      covrtEmlCondEval(chartInstance->c9_covrtInstance, 4U, 0, 1,
                       covrtRelationalopUpdateFcn
                       (chartInstance->c9_covrtInstance, 4U, 0U, 1U,
                        static_cast<real_T>(c9_labelNum), 91.0, -1, 3U,
                        static_cast<int32_T>(c9_labelNum <= 91.0F)))) {
    covrtEmlMcdcEval(chartInstance->c9_covrtInstance, 4U, 0, 0, true);
    covrtEmlIfEval(chartInstance->c9_covrtInstance, 4U, 0, 1, true);
    c9_d = c9_labelNum;
    if (c9_d != static_cast<real_T>(static_cast<int32_T>(muDoubleScalarFloor
          (c9_d)))) {
      emlrtIntegerCheckR2012b(c9_d, &c9_b_emlrtDCI, &c9_st);
    }

    c9_i22 = static_cast<int32_T>(static_cast<real32_T>(c9_d)) - 1;
    if ((c9_i22 < 0) || (c9_i22 > 90)) {
      emlrtDynamicBoundsCheckR2012b(c9_i22, 0, 90, &c9_e_emlrtBCI, &c9_st);
    }

    c9_i24 = c9_i22;
    c9_name_size[1] = chartInstance->c9_cocoLabels[c9_i24].f1.size[1];
    c9_loop_ub = chartInstance->c9_cocoLabels[c9_i24].f1.size[1] - 1;
    for (c9_i26 = 0; c9_i26 <= c9_loop_ub; c9_i26++) {
      c9_name_data[c9_i26] = chartInstance->c9_cocoLabels[c9_i24].f1.data[c9_i26];
    }
  } else {
    covrtEmlMcdcEval(chartInstance->c9_covrtInstance, 4U, 0, 0, false);
    covrtEmlIfEval(chartInstance->c9_covrtInstance, 4U, 0, 1, false);
    c9_name_size[1] = 7;
    for (c9_i21 = 0; c9_i21 < 7; c9_i21++) {
      c9_name_data[c9_i21] = c9_cv9[c9_i21];
    }
  }

  memset(&c9_b_labelOut[0], 0, 20U * sizeof(uint8_T));
  c9_c_varargin_2 = c9_name_size[1];
  c9_d_varargin_2 = c9_c_varargin_2;
  c9_n = static_cast<real_T>(c9_d_varargin_2);
  c9_d_varargin_1 = c9_n;
  c9_f_x = c9_d_varargin_1;
  c9_g_x = c9_f_x;
  c9_h_x = c9_g_x;
  c9_b_a = c9_h_x;
  c9_i_x = c9_b_a;
  c9_j_x = c9_i_x;
  c9_len = c9_j_x;
  c9_c_b = (c9_len < 1.0);
  c9_b1 = c9_c_b;
  c9_b2 = false;
  c9_b3 = (c9_b1 || c9_b2);
  if (c9_b3) {
    c9_i31 = 1;
    c9_i32 = 0;
  } else {
    c9_i30 = 1;
    if ((c9_i30 < 1) || (c9_i30 > c9_name_size[1])) {
      emlrtDynamicBoundsCheckR2012b(c9_i30, 1, c9_name_size[1], &c9_b_emlrtBCI,
        &c9_st);
    }

    c9_i31 = c9_i30;
    if (c9_len != static_cast<real_T>(static_cast<int32_T>(muDoubleScalarFloor
          (c9_len)))) {
      emlrtIntegerCheckR2012b(c9_len, &c9_c_emlrtDCI, &c9_st);
    }

    c9_i34 = static_cast<int32_T>(c9_len);
    if ((c9_i34 < 1) || (c9_i34 > c9_name_size[1])) {
      emlrtDynamicBoundsCheckR2012b(c9_i34, 1, c9_name_size[1], &c9_c_emlrtBCI,
        &c9_st);
    }

    c9_i32 = c9_i34;
  }

  c9_b4 = (c9_len < 1.0);
  c9_b5 = c9_b4;
  c9_b6 = false;
  c9_b7 = (c9_b5 || c9_b6);
  if (c9_b7) {
    c9_i36 = 0;
    c9_i37 = 0;
  } else {
    c9_i36 = 0;
    if (c9_len != static_cast<real_T>(static_cast<int32_T>(muDoubleScalarFloor
          (c9_len)))) {
      emlrtIntegerCheckR2012b(c9_len, &c9_d_emlrtDCI, &c9_st);
    }

    c9_i39 = static_cast<int32_T>(c9_len);
    if ((c9_i39 < 1) || (c9_i39 > 20)) {
      emlrtDynamicBoundsCheckR2012b(c9_i39, 1, 20, &c9_d_emlrtBCI, &c9_st);
    }

    c9_i37 = c9_i39;
  }

  c9_tmp_size[1] = (c9_i32 - c9_i31) + 1;
  c9_b_loop_ub = c9_i32 - c9_i31;
  for (c9_i40 = 0; c9_i40 <= c9_b_loop_ub; c9_i40++) {
    c9_tmp_data[c9_i40] = static_cast<uint8_T>(c9_name_data[(c9_i31 + c9_i40) -
      1]);
  }

  c9_i42 = c9_i37 - c9_i36;
  if (c9_i42 != c9_tmp_size[1]) {
    emlrtSubAssignSizeCheck1dR2017a(c9_i42, c9_tmp_size[1], &c9_emlrtECI, &c9_st);
  }

  c9_i43 = c9_i36 + 1;
  c9_c_loop_ub = c9_tmp_size[1] - 1;
  for (c9_i45 = 0; c9_i45 <= c9_c_loop_ub; c9_i45++) {
    c9_b_labelOut[(c9_i43 + c9_i45) - 1] = c9_tmp_data[c9_i45];
  }

  if (covrtEmlIfEval(chartInstance->c9_covrtInstance, 4U, 0, 2,
                     covrtRelationalopUpdateFcn(chartInstance->c9_covrtInstance,
        4U, 0U, 2U, static_cast<real_T>(c9_b_scoreOut), 60.0, -1, 4U,
        static_cast<int32_T>(c9_b_scoreOut > 60.0F)))) {
    c9_b_found = 1.0;
  } else {
    c9_b_found = 0.0;
  }

  for (c9_i47 = 0; c9_i47 < 307200; c9_i47++) {
    (*chartInstance->c9_displayImg)[c9_i47] = chartInstance->
      c9_b_imgRestored[c9_i47];
  }

  for (c9_i49 = 0; c9_i49 < 4; c9_i49++) {
    (*chartInstance->c9_boxOut)[c9_i49] = c9_b_boxOut[c9_i49];
  }

  *chartInstance->c9_scoreOut = c9_b_scoreOut;
  for (c9_i50 = 0; c9_i50 < 20; c9_i50++) {
    (*chartInstance->c9_labelOut)[c9_i50] = c9_b_labelOut[c9_i50];
  }

  *chartInstance->c9_found = c9_b_found;
  for (c9_i52 = 0; c9_i52 < 307200; c9_i52++) {
    covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 1U, static_cast<real_T>((*
      chartInstance->c9_displayImg)[c9_i52]));
  }

  for (c9_i54 = 0; c9_i54 < 4; c9_i54++) {
    covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 2U, static_cast<real_T>((*
      chartInstance->c9_boxOut)[c9_i54]));
  }

  covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 3U, static_cast<real_T>
                    (*chartInstance->c9_scoreOut));
  for (c9_i55 = 0; c9_i55 < 20; c9_i55++) {
    covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 4U, static_cast<real_T>((*
      chartInstance->c9_labelOut)[c9_i55]));
  }

  covrtSigUpdateFcn(chartInstance->c9_covrtInstance, 5U,
                    *chartInstance->c9_found);
}

static void ext_mode_exec_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
}

static void c9_update_jit_animation_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *
  chartInstance)
{
}

static void c9_do_animation_call_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
}

static const mxArray *get_sim_state_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct *
  chartInstance)
{
  const mxArray *c9_b_y = NULL;
  const mxArray *c9_c_y = NULL;
  const mxArray *c9_d_y = NULL;
  const mxArray *c9_e_y = NULL;
  const mxArray *c9_f_y = NULL;
  const mxArray *c9_g_y = NULL;
  const mxArray *c9_h_y = NULL;
  const mxArray *c9_st;
  const mxArray *c9_y = NULL;
  int32_T c9_iv[2];
  int32_T c9_u_size[2];
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i2;
  int32_T c9_loop_ub;
  char_T c9_u_data[14];
  c9_st = NULL;
  c9_st = NULL;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_createcellmatrix(6, 1), false);
  c9_b_y = NULL;
  sf_mex_assign(&c9_b_y, sf_mex_create("y", *chartInstance->c9_boxOut, 1, 0U, 1,
    0U, 2, 1, 4), false);
  sf_mex_setcell(c9_y, 0, c9_b_y);
  c9_c_y = NULL;
  sf_mex_assign(&c9_c_y, sf_mex_create("y", *chartInstance->c9_displayImg, 3, 0U,
    1, 0U, 3, 320, 320, 3), false);
  sf_mex_setcell(c9_y, 1, c9_c_y);
  c9_d_y = NULL;
  sf_mex_assign(&c9_d_y, sf_mex_create("y", chartInstance->c9_found, 0, 0U, 0,
    0U, 0), false);
  sf_mex_setcell(c9_y, 2, c9_d_y);
  c9_e_y = NULL;
  sf_mex_assign(&c9_e_y, sf_mex_create("y", *chartInstance->c9_labelOut, 3, 0U,
    1, 0U, 2, 1, 20), false);
  sf_mex_setcell(c9_y, 3, c9_e_y);
  c9_f_y = NULL;
  sf_mex_assign(&c9_f_y, sf_mex_create("y", chartInstance->c9_scoreOut, 1, 0U, 0,
    0U, 0), false);
  sf_mex_setcell(c9_y, 4, c9_f_y);
  c9_g_y = NULL;
  if (!chartInstance->c9_cocoLabels_not_empty) {
    sf_mex_assign(&c9_g_y, sf_mex_create("y", NULL, 0, 0U, 1, 0U, 2, 0, 0),
                  false);
  } else {
    for (c9_i = 0; c9_i < 2; c9_i++) {
      c9_iv[c9_i] = 1 + 90 * c9_i;
    }

    sf_mex_assign(&c9_g_y, sf_mex_createcellarray(2, c9_iv), false);
    for (c9_i1 = 0; c9_i1 < 91; c9_i1++) {
      c9_u_size[1] = chartInstance->c9_cocoLabels[c9_i1].f1.size[1];
      c9_loop_ub = chartInstance->c9_cocoLabels[c9_i1].f1.size[1] - 1;
      for (c9_i2 = 0; c9_i2 <= c9_loop_ub; c9_i2++) {
        c9_u_data[c9_i2] = chartInstance->c9_cocoLabels[c9_i1].f1.data[c9_i2];
      }

      c9_h_y = NULL;
      sf_mex_assign(&c9_h_y, sf_mex_create("y", &c9_u_data, 10, 0U, 1, 0U, 2, 1,
        c9_u_size[1]), false);
      sf_mex_setcell(c9_g_y, c9_i1, c9_h_y);
    }
  }

  sf_mex_setcell(c9_y, 5, c9_g_y);
  sf_mex_assign(&c9_st, c9_y, false);
  return c9_st;
}

static void set_sim_state_c9_rpi_camera(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_st)
{
  c9_cell_wrap_6 c9_rv[91];
  const mxArray *c9_u;
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i2;
  int32_T c9_i3;
  real32_T c9_fv[4];
  uint8_T c9_uv1[20];
  chartInstance->c9_doneDoubleBufferReInit = true;
  c9_u = sf_mex_dup(c9_st);
  c9_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 0)),
                      "boxOut", c9_fv);
  for (c9_i = 0; c9_i < 4; c9_i++) {
    (*chartInstance->c9_boxOut)[c9_i] = c9_fv[c9_i];
  }

  c9_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 1)),
                        "displayImg", chartInstance->c9_uv);
  for (c9_i1 = 0; c9_i1 < 307200; c9_i1++) {
    (*chartInstance->c9_displayImg)[c9_i1] = chartInstance->c9_uv[c9_i1];
  }

  *chartInstance->c9_found = c9_e_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c9_u, 2)), "found");
  c9_g_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 3)),
                        "labelOut", c9_uv1);
  for (c9_i2 = 0; c9_i2 < 20; c9_i2++) {
    (*chartInstance->c9_labelOut)[c9_i2] = c9_uv1[c9_i2];
  }

  *chartInstance->c9_scoreOut = c9_i_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c9_u, 4)), "scoreOut");
  c9_k_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 5)),
                        "cocoLabels", &chartInstance->c9_cocoLabels_not_empty,
                        c9_rv);
  for (c9_i3 = 0; c9_i3 < 91; c9_i3++) {
    chartInstance->c9_cocoLabels[c9_i3] = c9_rv[c9_i3];
  }

  sf_mex_destroy(&c9_u);
  sf_mex_destroy(&c9_st);
}

static void c9_TFLiteModel_predict(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const emlrtStack *c9_sp, c9_coder_TFLiteModel *c9_this, uint8_T c9_varargin_1
  [307200], real32_T c9_varargout_1[400], real32_T c9_varargout_2[100], real32_T
  c9_varargout_3[100], real32_T *c9_varargout_4)
{
  invokeinterpreter * c9_networkObj;
  c9_cell_7 c9_tmpBuffer1;
  c9_cell_8 c9_buffer;
  c9_coder_TFLiteModel *c9_obj;
  emlrtStack c9_st;
  real_T c9_count;
  int32_T c9_channel;
  int32_T c9_height;
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i10;
  int32_T c9_i11;
  int32_T c9_i12;
  int32_T c9_i13;
  int32_T c9_i14;
  int32_T c9_i2;
  int32_T c9_i3;
  int32_T c9_i4;
  int32_T c9_i5;
  int32_T c9_i6;
  int32_T c9_i7;
  int32_T c9_i8;
  int32_T c9_i9;
  int32_T c9_numThreads;
  int32_T c9_value;
  int32_T c9_width;
  real32_T c9_totalPythonOutput[601];
  real32_T c9_fv[400];
  real32_T c9_outputReshaped[400];
  real32_T c9_a[100];
  real32_T c9_outputScaleVector[4];
  real32_T c9_outputZeroPointVector[4];
  real32_T c9_b;
  real32_T c9_b_a;
  real32_T c9_b_channel;
  real32_T c9_b_height;
  real32_T c9_b_width;
  real32_T c9_b_x;
  real32_T c9_f;
  real32_T c9_f1;
  real32_T c9_inputScaleVector;
  real32_T c9_inputZeroPointVector;
  real32_T c9_meanVal;
  real32_T c9_stdVal;
  real32_T c9_x;
  boolean_T c9_enableProfiling;
  boolean_T c9_verbose;
  c9_st.prev = c9_sp;
  c9_st.tls = c9_sp->tls;
  c9_st.site = &c9_e_emlrtRSI;
  c9_obj = c9_this;
  c9_value = c9_obj->InternalNumThreads;
  c9_st.site = &c9_e_emlrtRSI;
  c9_networkObj = c9_this->Network;
  c9_verbose = c9_this->Verbose;
  c9_enableProfiling = c9_this->EnableProfiling;
  c9_numThreads = c9_value;
  c9_meanVal = c9_this->Mean;
  c9_stdVal = c9_this->StandardDeviation;
  c9_inputScaleVector = 1.0F;
  c9_inputZeroPointVector = 0.0F;
  for (c9_i = 0; c9_i < 4; c9_i++) {
    c9_outputScaleVector[c9_i] = 1.0F;
  }

  memset(&c9_outputZeroPointVector[0], 0, sizeof(real32_T) << 2);
  std::mem_fn(&invokeinterpreter::setVerbose)(c9_networkObj, c9_verbose);
  std::mem_fn(&invokeinterpreter::setProfiling)(c9_networkObj,
    c9_enableProfiling);
  std::mem_fn(&invokeinterpreter::setNumThreads)(c9_networkObj, c9_numThreads);
  std::mem_fn(&invokeinterpreter::setInputMean)(c9_networkObj, c9_meanVal);
  std::mem_fn(&invokeinterpreter::setInputStdDeviation)(c9_networkObj, c9_stdVal);
  std::mem_fn(&invokeinterpreter::setInputScale)(c9_networkObj,
    &c9_inputScaleVector);
  std::mem_fn(&invokeinterpreter::setInputZeroPoint)(c9_networkObj,
    &c9_inputZeroPointVector);
  std::mem_fn(&invokeinterpreter::setOutputScale)(c9_networkObj,
    &c9_outputScaleVector[0]);
  std::mem_fn(&invokeinterpreter::setOutputZeroPoint)(c9_networkObj,
    &c9_outputZeroPointVector[0]);
  std::mem_fn(&invokeinterpreter::setSkipNormalization)(c9_networkObj, false);
  std::mem_fn(&invokeinterpreter::setSkipInputQuantization)(c9_networkObj, true);
  std::mem_fn(&invokeinterpreter::setSkipOutputDequantization)(c9_networkObj,
    true);
  c9_st.site = &c9_e_emlrtRSI;
  c9_count = 1.0;
  for (c9_height = 0; c9_height < 320; c9_height++) {
    c9_b_height = static_cast<real32_T>(c9_height) + 1.0F;
    for (c9_width = 0; c9_width < 320; c9_width++) {
      c9_b_width = static_cast<real32_T>(c9_width) + 1.0F;
      for (c9_channel = 0; c9_channel < 3; c9_channel++) {
        c9_b_channel = static_cast<real32_T>(c9_channel) + 1.0F;
        c9_i4 = static_cast<int32_T>(c9_count);
        if ((c9_i4 < 1) || (c9_i4 > 307200)) {
          emlrtDynamicBoundsCheckR2012b(c9_i4, 1, 307200, &c9_g_emlrtBCI, &c9_st);
        }

        chartInstance->c9_preprocessedInp[c9_i4 - 1] = static_cast<real32_T>
          (c9_varargin_1[((static_cast<int32_T>(c9_b_height) + 320 * (
              static_cast<int32_T>(c9_b_width) - 1)) + 102400 * (static_cast<
             int32_T>(c9_b_channel) - 1)) - 1]);
        c9_count++;
      }
    }
  }

  std::mem_fn(&invokeinterpreter::predict)(c9_this->Network,
    &chartInstance->c9_preprocessedInp[0], 307200,
    &c9_this->PredictedTotalOutput[0]);
  for (c9_i1 = 0; c9_i1 < 601; c9_i1++) {
    c9_totalPythonOutput[c9_i1] = c9_this->PredictedTotalOutput[c9_i1];
  }

  for (c9_i2 = 0; c9_i2 < 400; c9_i2++) {
    c9_outputReshaped[c9_i2] = c9_totalPythonOutput[c9_i2];
  }

  c9_permute(chartInstance, c9_outputReshaped, c9_fv);
  for (c9_i3 = 0; c9_i3 < 400; c9_i3++) {
    c9_tmpBuffer1.f1[c9_i3] = c9_fv[c9_i3];
  }

  for (c9_i5 = 0; c9_i5 < 601; c9_i5++) {
    c9_totalPythonOutput[c9_i5] = c9_this->PredictedTotalOutput[c9_i5];
  }

  for (c9_i6 = 0; c9_i6 < 100; c9_i6++) {
    c9_a[c9_i6] = c9_totalPythonOutput[c9_i6 + 400];
  }

  for (c9_i7 = 0; c9_i7 < 100; c9_i7++) {
    c9_tmpBuffer1.f2[c9_i7] = c9_a[c9_i7];
  }

  for (c9_i8 = 0; c9_i8 < 601; c9_i8++) {
    c9_totalPythonOutput[c9_i8] = c9_this->PredictedTotalOutput[c9_i8];
  }

  for (c9_i9 = 0; c9_i9 < 100; c9_i9++) {
    c9_a[c9_i9] = c9_totalPythonOutput[c9_i9 + 500];
  }

  for (c9_i10 = 0; c9_i10 < 100; c9_i10++) {
    c9_tmpBuffer1.f3[c9_i10] = c9_a[c9_i10];
  }

  for (c9_i11 = 0; c9_i11 < 601; c9_i11++) {
    c9_totalPythonOutput[c9_i11] = c9_this->PredictedTotalOutput[c9_i11];
  }

  c9_f = c9_totalPythonOutput[600];
  c9_b_a = c9_f;
  c9_f1 = c9_b_a;
  c9_tmpBuffer1.f4 = c9_f1;
  c9_shiftdim(chartInstance, c9_tmpBuffer1.f1, c9_buffer.f1);
  for (c9_i12 = 0; c9_i12 < 400; c9_i12++) {
    c9_varargout_1[c9_i12] = c9_buffer.f1[c9_i12];
  }

  c9_b_shiftdim(chartInstance, c9_tmpBuffer1.f2, c9_buffer.f2);
  for (c9_i13 = 0; c9_i13 < 100; c9_i13++) {
    c9_varargout_2[c9_i13] = c9_buffer.f2[c9_i13];
  }

  c9_b_shiftdim(chartInstance, c9_tmpBuffer1.f3, c9_buffer.f3);
  for (c9_i14 = 0; c9_i14 < 100; c9_i14++) {
    c9_varargout_3[c9_i14] = c9_buffer.f3[c9_i14];
  }

  c9_x = c9_tmpBuffer1.f4;
  c9_b_x = c9_x;
  c9_b = c9_b_x;
  c9_buffer.f4 = c9_b;
  *c9_varargout_4 = c9_buffer.f4;
}

static void c9_permute(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_a[400], real32_T c9_b[400])
{
  int32_T c9_subsa[3];
  int32_T c9_subsb[3];
  int32_T c9_b_k;
  int32_T c9_b_varargin_1;
  int32_T c9_c_k;
  int32_T c9_d_k;
  int32_T c9_k;
  int32_T c9_varargin_1;
  int32_T c9_varargin_2;
  for (c9_k = 0; c9_k < 100; c9_k++) {
    c9_b_k = c9_k;
    c9_varargin_1 = c9_b_k + 1;
    for (c9_c_k = 0; c9_c_k < 4; c9_c_k++) {
      c9_d_k = c9_c_k;
      c9_b_varargin_1 = c9_d_k + 1;
      c9_varargin_2 = c9_varargin_1;
      c9_subsa[0] = c9_b_varargin_1;
      c9_subsa[1] = c9_varargin_2;
      c9_subsb[1] = c9_subsa[1];
      c9_subsb[2] = c9_subsa[0];
      c9_b[(c9_subsb[1] + 100 * (c9_subsb[2] - 1)) - 1] = c9_a[(c9_subsa[0] +
        ((c9_subsa[1] - 1) << 2)) - 1];
    }
  }
}

static void c9_shiftdim(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_x[400], real32_T c9_b[400])
{
  int32_T c9_subsb[3];
  int32_T c9_b_k;
  int32_T c9_b_varargin_1;
  int32_T c9_b_varargin_2;
  int32_T c9_c_k;
  int32_T c9_d_k;
  int32_T c9_k;
  int32_T c9_varargin_1;
  int32_T c9_varargin_2;
  int32_T c9_varargin_3;
  for (c9_k = 0; c9_k < 4; c9_k++) {
    c9_b_k = c9_k;
    c9_varargin_1 = c9_b_k + 1;
    for (c9_c_k = 0; c9_c_k < 100; c9_c_k++) {
      c9_d_k = c9_c_k;
      c9_b_varargin_1 = c9_d_k + 1;
      c9_varargin_2 = c9_varargin_1;
      c9_b_varargin_2 = c9_b_varargin_1;
      c9_varargin_3 = c9_varargin_2;
      c9_subsb[0] = c9_b_varargin_2;
      c9_subsb[1] = c9_varargin_3;
      c9_b[(c9_subsb[0] + 100 * (c9_subsb[1] - 1)) - 1] = c9_x[(c9_b_varargin_2
        + 100 * (c9_varargin_3 - 1)) - 1];
    }
  }
}

static void c9_b_shiftdim(SFc9_rpi_cameraInstanceStruct *chartInstance, real32_T
  c9_x[100], real32_T c9_b[100])
{
  int32_T c9_subsb[2];
  int32_T c9_b_k;
  int32_T c9_k;
  int32_T c9_varargin_1;
  int32_T c9_varargin_2;
  for (c9_k = 0; c9_k < 100; c9_k++) {
    c9_b_k = c9_k;
    c9_varargin_1 = c9_b_k + 1;
    c9_varargin_2 = c9_varargin_1;
    c9_subsb[0] = c9_varargin_2;
    c9_b[c9_subsb[0] - 1] = c9_x[c9_varargin_2 - 1];
  }
}

const mxArray *sf_c9_rpi_camera_get_eml_resolved_functions_info()
{
  const mxArray *c9_nameCaptureInfo = NULL;
  const char_T *c9_data[5] = {
    "789cc554cb4ec240141d0c4636280be3d21f20690c71e54ac460501e152a1bc6e0d04ea161daa9d34161c717e8c28fe21ffc0713972eed8329d0a429c1087773"
    "7b723af79c7b3a2948556a2900c021082a7710f4acc0f3be07562bcaa762baa87d905e3927f8f77957a9c5f19807c042260e4f6ad4342c6471656263c0b043c9",
    "0bd67c463708560c13b79641dd436679890a814779cfa5015687ad9109d8c0593824cb20cce33b66dff49a79bcc5e4918bf09d4aa3f458ba8032a37d86cc6bc4"
    "11ac15956af10ab646b64d1997913a447decc066e1ac70de839c52d2a363e80c10c31ad4884a35ccba3de460e804476c7144235d8e2d87329dd0d72e31388679",
    "ff7578a994ab2eacb98040db1d64a85cb217fb4fffb8ff69c2fe82f7dd48867b09988588c475cfa4e4c5e07ff7d0cfd3867ea215e74794d0fbd9504fccff48d0"
    "137ca752dfc5e7cf8bc4613e881c8ac8bd4b0092733f5a3387b8ff421664fc3e9b7da6b6a9373dce7c6d534fd4aef4c631f3d6bdc727317ab9083fbc69dc57db",
    "77cf7d793278686376ab93a65a5ef8901374927c8018fcdff37f0148a0b77f", "" };

  c9_nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&c9_data[0], 1832U, &c9_nameCaptureInfo);
  return c9_nameCaptureInfo;
}

static void c9_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, real32_T c9_y[4])
{
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_nullptr);
}

static void c9_b_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, real32_T c9_y[4])
{
  int32_T c9_i;
  real32_T c9_fv[4];
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_fv, 0, 1, 0U, 1, 0U, 2, 1, 4);
  for (c9_i = 0; c9_i < 4; c9_i++) {
    c9_y[c9_i] = c9_fv[c9_i];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_c_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, uint8_T c9_y[307200])
{
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_nullptr);
}

static void c9_d_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, uint8_T c9_y
  [307200])
{
  int32_T c9_i;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), chartInstance->c9_b_uv, 1, 3, 0U,
                1, 0U, 3, 320, 320, 3);
  for (c9_i = 0; c9_i < 307200; c9_i++) {
    c9_y[c9_i] = chartInstance->c9_b_uv[c9_i];
  }

  sf_mex_destroy(&c9_u);
}

static real_T c9_e_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier)
{
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_y = c9_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId);
  sf_mex_destroy(&c9_nullptr);
  return c9_y;
}

static real_T c9_f_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId)
{
  real_T c9_d;
  real_T c9_y;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), &c9_d, 1, 0, 0U, 0, 0U, 0);
  c9_y = c9_d;
  sf_mex_destroy(&c9_u);
  return c9_y;
}

static void c9_g_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, uint8_T c9_y[20])
{
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_nullptr);
}

static void c9_h_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, uint8_T c9_y[20])
{
  int32_T c9_i;
  uint8_T c9_c_uv[20];
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_c_uv, 1, 3, 0U, 1, 0U, 2, 1,
                20);
  for (c9_i = 0; c9_i < 20; c9_i++) {
    c9_y[c9_i] = c9_c_uv[c9_i];
  }

  sf_mex_destroy(&c9_u);
}

static real32_T c9_i_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_nullptr, const char_T *c9_identifier)
{
  emlrtMsgIdentifier c9_thisId;
  real32_T c9_y;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_y = c9_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId);
  sf_mex_destroy(&c9_nullptr);
  return c9_y;
}

static real32_T c9_j_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId)
{
  real32_T c9_f;
  real32_T c9_y;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), &c9_f, 0, 1, 0U, 0, 0U, 0);
  c9_y = c9_f;
  sf_mex_destroy(&c9_u);
  return c9_y;
}

static void c9_k_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_nullptr, const char_T *c9_identifier, boolean_T *c9_svPtr,
  c9_cell_wrap_6 c9_y[91])
{
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = const_cast<const char_T *>(c9_identifier);
  c9_thisId.fParent = NULL;
  c9_thisId.bParentIsCell = false;
  c9_m_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nullptr), &c9_thisId,
                        c9_svPtr, c9_y);
  sf_mex_destroy(&c9_nullptr);
}

static void c9_m_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, boolean_T
  *c9_svPtr, c9_cell_wrap_6 c9_y[91])
{
  emlrtMsgIdentifier c9_thisId;
  int32_T c9_iv[2];
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i2;
  char_T c9_str[11];
  boolean_T c9_bv[2];
  c9_thisId.fParent = c9_parentId;
  c9_thisId.bParentIsCell = true;
  if (mxIsEmpty(c9_u)) {
    *c9_svPtr = false;
    for (c9_i1 = 0; c9_i1 < 91; c9_i1++) {
      c9_y[c9_i1].f1.size[0] = 1;
      c9_y[c9_i1].f1.size[1] = 0;
    }
  } else {
    *c9_svPtr = true;
    for (c9_i = 0; c9_i < 2; c9_i++) {
      c9_iv[c9_i] = 1 + 90 * c9_i;
    }

    memset(&c9_bv[0], 0, sizeof(boolean_T) << 1);
    sf_mex_check_cell(c9_parentId, c9_u, 2U, c9_iv, c9_bv);
    for (c9_i2 = 0; c9_i2 < 91; c9_i2++) {
      emlrtMexSnprintf(&c9_str[0], (size_t)11U, "%d", c9_i2 + 1);
      c9_thisId.fIdentifier = &c9_str[0];
      c9_o_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, c9_i2)),
                            &c9_thisId, c9_y[c9_i2].f1.data, c9_y[c9_i2].f1.size);
    }
  }

  sf_mex_destroy(&c9_u);
}

static void c9_o_emlrt_marshallIn(SFc9_rpi_cameraInstanceStruct *chartInstance,
  const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId, char_T c9_y_data[],
  int32_T c9_y_size[2])
{
  static boolean_T c9_bv1[2] = { false, true };

  int32_T c9_iv[2];
  int32_T c9_tmp_size[2];
  int32_T c9_i;
  int32_T c9_i1;
  int32_T c9_i2;
  int32_T c9_loop_ub;
  char_T c9_tmp_data[14];
  boolean_T c9_bv[2];
  for (c9_i = 0; c9_i < 2; c9_i++) {
    c9_iv[c9_i] = 1 + 13 * c9_i;
  }

  c9_tmp_size[0] = sf_mex_get_dimension(c9_u, 0);
  c9_tmp_size[1] = sf_mex_get_dimension(c9_u, 1);
  for (c9_i1 = 0; c9_i1 < 2; c9_i1++) {
    c9_bv[c9_i1] = c9_bv1[c9_i1];
  }

  sf_mex_import_vs(c9_parentId, sf_mex_dup(c9_u), &c9_tmp_data, 1, 10, 0U, 1, 0U,
                   2, c9_bv, c9_iv, c9_tmp_size);
  c9_y_size[0] = 1;
  c9_y_size[1] = c9_tmp_size[1];
  c9_loop_ub = c9_tmp_size[1] - 1;
  for (c9_i2 = 0; c9_i2 <= c9_loop_ub; c9_i2++) {
    c9_y_data[c9_i2] = c9_tmp_data[c9_i2];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_array_cell_wrap_6_1x91s_Constru(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_cell_wrap_6 c9_pMatrix[91])
{
  int32_T c9_i;
  for (c9_i = 0; c9_i < 91; c9_i++) {
    c9_array_cell_wrap_6_Constructor(chartInstance, &c9_pMatrix[c9_i]);
  }
}

static void c9_array_cell_wrap_6_Constructor(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_cell_wrap_6 *c9_pStruct)
{
  c9_array_char_T_1x14_Constructor(chartInstance, &c9_pStruct->f1);
}

static void c9_array_char_T_1x14_Constructor(SFc9_rpi_cameraInstanceStruct
  *chartInstance, c9_coder_array_char_T_1x14 *c9_pArray)
{
  c9_pArray->size[0] = 0;
  c9_pArray->size[1] = 0;
}

static void c9_array_cell_wrap_6_1x91s_Destruc(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
}

static void init_dsm_address_info(SFc9_rpi_cameraInstanceStruct *chartInstance)
{
}

static void init_simulink_io_address(SFc9_rpi_cameraInstanceStruct
  *chartInstance)
{
  chartInstance->c9_covrtInstance = (CovrtStateflowInstance *)
    sfrtGetCovrtInstance(chartInstance->S);
  chartInstance->c9_fEmlrtCtx = (void *)sfrtGetEmlrtCtx(chartInstance->S);
  chartInstance->c9_displayImg = (uint8_T (*)[307200])
    ssGetOutputPortSignal_wrapper(chartInstance->S, 1);
  chartInstance->c9_inputImgR = (uint8_T (*)[307200])
    ssGetInputPortSignal_wrapper(chartInstance->S, 0);
  chartInstance->c9_boxOut = (real32_T (*)[4])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c9_scoreOut = (real32_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c9_labelOut = (uint8_T (*)[20])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 4);
  chartInstance->c9_found = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 5);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SFunction Glue Code */
void sf_c9_rpi_camera_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(739169023U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(639886725U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3513775484U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(268435969U);
}

mxArray *sf_c9_rpi_camera_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,1);
  mxSetCell(mxcell3p, 0, mxCreateString("coder.internal.tflite.TFLiteAPI"));
  return(mxcell3p);
}

mxArray *sf_c9_rpi_camera_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("late");
  mxArray *fallbackReason = mxCreateString("ir_vars");
  mxArray *hiddenFallbackType = mxCreateString("");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("net");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c9_rpi_camera_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c9_rpi_camera(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  mxArray *mxVarInfo = sf_mex_decode(
    "eNpjYPT0ZQACPiB+wcrAwAakOYCYiQECWKF8RgaIHCNcnAUurgDEJZUFqSDx4qJkzxQgnZeYC+Y"
    "nllZ45qXlg823YECYz4bFfEYk8zmh4hDwwZ4y/SIO6PpZCPhPAMpPyq/wLy2BhA8snAbOHypgfz"
    "gQ8AcXmj9A/JTM4oKcxErP3HSEPwbeP0pExQsrmn9A/LT80ryUQeMPCaL8wYHmDxA/JzEpNQeYw"
    "gaHP2TI9kdxcn5RKtX8IeBAmX6I/eTkk+T85HwfUJwUI8y5wIC/XGREKxeZkMqTHIZMhgIs7sHm"
    "HyY0/wjAPNMQ5wChE6gSLiAAAJClNEI="
    );
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c9_rpi_camera_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static const char* sf_get_instance_specialization(void)
{
  return "sBaASZ2fUUlJP2aEfRK7HV";
}

static void sf_opaque_initialize_c9_rpi_camera(void *chartInstanceVar)
{
  initialize_params_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*)
    chartInstanceVar);
  initialize_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c9_rpi_camera(void *chartInstanceVar)
{
  enable_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c9_rpi_camera(void *chartInstanceVar)
{
  disable_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c9_rpi_camera(void *chartInstanceVar)
{
  sf_gateway_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c9_rpi_camera(SimStruct* S)
{
  return get_sim_state_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct *)
    sf_get_chart_instance_ptr(S));     /* raw sim ctx */
}

static void sf_opaque_set_sim_state_c9_rpi_camera(SimStruct* S, const mxArray
  *st)
{
  set_sim_state_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*)
    sf_get_chart_instance_ptr(S), st);
}

static void sf_opaque_cleanup_runtime_resources_c9_rpi_camera(void
  *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_rpi_camera_optimization_info();
    }

    mdl_cleanup_runtime_resources_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*)
      chartInstanceVar);
    ((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar)->
      ~SFc9_rpi_cameraInstanceStruct();
    utFree(chartInstanceVar);
    if (ssGetUserData(S)!= NULL) {
      sf_free_ChartRunTimeInfo(S);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_mdl_start_c9_rpi_camera(void *chartInstanceVar)
{
  mdl_start_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
  if (chartInstanceVar) {
    sf_reset_warnings_ChartRunTimeInfo(((SFc9_rpi_cameraInstanceStruct*)
      chartInstanceVar)->S);
  }
}

static void sf_opaque_mdl_terminate_c9_rpi_camera(void *chartInstanceVar)
{
  mdl_terminate_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c9_rpi_camera(SimStruct *S)
{
  mdlProcessParamsCommon(S);
  if (sf_machine_global_initializer_called()) {
    initialize_params_c9_rpi_camera((SFc9_rpi_cameraInstanceStruct*)
      sf_get_chart_instance_ptr(S));
  }
}

const char* sf_c9_rpi_camera_get_post_codegen_info(void)
{
  int i;
  const char* encStrCodegen [28] = {
    "eNrtWc1vG0UUX0dJ1Fb9Qqqgh6pUiANSRYAUxIcQJPFH69ZpTOwEqRjCePetPcrszHZm1ok59cY",
    "NDr1wQhz4D7gjbuWEOKKeOHJB4gZH3uyunWXjxrvrfuTASmv77e5v3vzevK9ZW6X6uoXHWTx/v2",
    "JZi/h9As85KzoWYrmUOKPr89b7sdw8ZVl2n0hd566w8h+2cKAHvBW4Lt3PieWB1ySSeKqAXk482",
    "AQlWKCp4PkmT7kLEriNA/hC6lx6FfUCRvluLeC20aw+7lO73+qLgDlrOCBxNjgbPkqvH+gmaqxQ",
    "CbauATi6L0XQ69cY6R1tBan3yn2wd1Xg5baVAt0KfENVrQdMU59BdR/sOleaoBXUFL4tTTSU9X5",
    "uD6GqNUILz2eU8Oy27hPVAh+9Q8OW7+DnRqDRehn1or4u5UQLSQmreqxsPDwjtslwnuvo1iy3nb",
    "WnK9ANej3Ke8a6MvCAI3/0kwy2cstiAJL0YIPnjEHDrrofLvDYL7PHYN24ZaEYDLxoVVQhbKi3O",
    "kALqYJ6azYvE8ZUPmxb+A0YAAv1V4gmBbCR/hxgpajTFttEmijIGUkBp3cDiLFlwR2afYUHKVSY",
    "cG9j8swAp55xKXDQzOOpjwea5pOB0sIrYxhVGo2M+g5j61yDdIkNmXOfJFQBTjj0q5x6HapIlxk",
    "0WkmHLDOPALww1FJuwCt7Qu6ijfMm2QNbmUjIhwanBxXQECaMKnr3NmFBxjl7CjOccY8thRkrn1",
    "7EmvgpBLaJ3QfH1BPKYB2UGUBlzs9YR1aR7YDqYQWULamfNZICBQ4WEmOl9tCHLb7LxR6vSeG14",
    "o7gCL8CwKxBJMeysIZlSQ5rOPlss5Zwt208q0h75hHNSNf4xnXgWFkMV1MJiY1RVeXYuuGEZsG2",
    "6BdY2rmiSmM7NayGMeCE/ejFUr5+9LlYvjaOpbpqS1wpgmWYY6e02g2bPWhTD8ILLYI9RSTGh9H",
    "7unWg9+Tc0Xrn8FepIM6aEbeSwM1PsM+ZBO58LNvv7kif7tiYWCSZoP90BjuP9H83n299RvP5Ou",
    "z5MS3v64NG/MDThYf+zbUJEXRdXLEB+oM5XBqtVSsp3DaSV0vcGgvmlvkdJv4WJn7ZTwQMSwohn",
    "5+m2POrlD2N/El9o/xp+b1OU4oeVkVTzTvrq+3G6lon7pebxN41Caazufz68pvdjhaCdcV+R2Ek",
    "gNNxmNn+yJ0uUdBREcQfQRy2g1GhhHSZ2NthVEPnavh4Z6Vda6AY9pgdHweitl7yIx7NKTwup3h",
    "cjvdgcomaWskJW9KuUbZk6ITrcMhP0uOOjuS4I7v+PGU+91O4+6Fdbz8Ls14dWaBzNTJBZ2QCY9",
    "yYzzsJPuem+P2Z+PqDBw9Ls+DvXTjx5yz4pJ/Pir8xZT2fT62nkXevb3zU2L51t9cc9re2Qd502",
    "aZdi8Z7JTFeacJ4Sb8q8nxRXNE6kMzLZ1J65lO4xdhGv3zb+Pv7Hx48fG3erp378c6Xs+j/ZjFf",
    "Xj4by5dG+8Bxpzw41EwWXX+1RlZbd5bdrS12s7lMqu7mrbdvbEfj/bFw9HwXU/MdXb+Cpw6zE44",
    "v7bqTKCUkiN41pP19cYo9Tv7H3//6cDb8hZU0fn4Kv/OxjKlsw7yqOPuIuH26PF5eydJvnErxOB",
    "Xth3xGhnWvdzgenx2flzKty0KKj5FdEXDn2PC4mInHiRQPI2NjHr6BOB48LhXmoWwh4bHxOL8yG",
    "z5bXz4pTmxhi4ZZE3Uwzq9Wvjw+l8gnzKKWP2E+k/jMpfiMejfr3mcr0ffnj8Uus+53/sc9uX1n",
    "ln1fqSBuYcZ95tPCzcrvSfexx+35ovvB48bjqPx0egKvUmrc48rrt5z148VY/mD8X0O5T5kz4W1",
    "nfLsBxJ109ynxuzfl+VLq+Sf1XiTvvP/JuS4vxHLVrEv8J+yda6s4n6GiUb8wutyU5v+/8S0JRE",
    "1+N/0s6lvW/uh0ah2MvEe5I/bUq28sv7U8S738Fyzi5I4=",
    ""
  };

  static char newstr [1997] = "";
  newstr[0] = '\0';
  for (i = 0; i < 28; i++) {
    strcat(newstr, encStrCodegen[i]);
  }

  return newstr;
}

static void mdlSetWorkWidths_c9_rpi_camera(SimStruct *S)
{
  const char* newstr = sf_c9_rpi_camera_get_post_codegen_info();
  sf_set_work_widths(S, newstr);
  ssSetChecksum0(S,(4148993742U));
  ssSetChecksum1(S,(3670520740U));
  ssSetChecksum2(S,(1180894255U));
  ssSetChecksum3(S,(2270871055U));
}

static void mdlRTW_c9_rpi_camera(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlSetupRuntimeResources_c9_rpi_camera(SimStruct *S)
{
  SFc9_rpi_cameraInstanceStruct *chartInstance;
  chartInstance = (SFc9_rpi_cameraInstanceStruct *)utMalloc(sizeof
    (SFc9_rpi_cameraInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  memset(chartInstance, 0, sizeof(SFc9_rpi_cameraInstanceStruct));
  chartInstance = new (chartInstance) SFc9_rpi_cameraInstanceStruct;
  chartInstance->chartInfo.chartInstance = chartInstance;
  if (ssGetSampleTime(S, 0) == CONTINUOUS_SAMPLE_TIME && ssGetOffsetTime(S, 0) ==
      0 && sfHasContStates(S)> 0 &&
      !supportsLegacyBehaviorForPersistentVarInContinuousTime(S)) {
    sf_error_out_about_continuous_sample_time_with_persistent_vars(S);
  }

  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c9_rpi_camera;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c9_rpi_camera;
  chartInstance->chartInfo.mdlStart = sf_opaque_mdl_start_c9_rpi_camera;
  chartInstance->chartInfo.mdlTerminate = sf_opaque_mdl_terminate_c9_rpi_camera;
  chartInstance->chartInfo.mdlCleanupRuntimeResources =
    sf_opaque_cleanup_runtime_resources_c9_rpi_camera;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c9_rpi_camera;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c9_rpi_camera;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c9_rpi_camera;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c9_rpi_camera;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c9_rpi_camera;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c9_rpi_camera;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c9_rpi_camera;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartEventFcn = NULL;
  chartInstance->S = S;
  chartInstance->chartInfo.dispatchToExportedFcn = NULL;
  sf_init_ChartRunTimeInfo(S, &(chartInstance->chartInfo), false, 0);
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  mdl_setup_runtime_resources_c9_rpi_camera(chartInstance);
}

void c9_rpi_camera_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_SETUP_RUNTIME_RESOURCES:
    mdlSetupRuntimeResources_c9_rpi_camera(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c9_rpi_camera(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c9_rpi_camera(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c9_rpi_camera_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
