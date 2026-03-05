#define S_FUNCTION_NAME DISPLAY
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"

#define WIDTH           (ssGetSFcnParam(S,0))
#define HEIGHT          (ssGetSFcnParam(S,1))

#define NINPUTS 2

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, NINPUTS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    // Mark parameters as non-tunable (required for RTW params)
    for (int i = 0; i < NINPUTS; ++i) {
        ssSetSFcnParamTunable(S, i, false);
    }

    // Check scalar, real params (0..2: width, height, fps)
    for (int i = 0; i < NINPUTS-1; ++i) {
        const mxArray *p = ssGetSFcnParam(S, i);
        if (!mxIsDouble(p) || mxIsComplex(p) || mxGetNumberOfElements(p) != 1) {
            ssSetErrorStatus(S, "Width, Height, and FPS must be real scalars.");
            return;
        }
    }

    // Validate width and height
    real_T width_val  = *mxGetPr(WIDTH);
    real_T height_val = *mxGetPr(HEIGHT);

    if (width_val <= 0 || height_val <= 0) {
        ssSetErrorStatus(S, "Width and height must be positive.");
        return;
    }

    int_T width  = (int_T) width_val;
    int_T height = (int_T) height_val;

    ssSetNumInputPorts(S, 3);
    for (int i = 0; i < 3; ++i) {
        ssSetInputPortMatrixDimensions(S, i, height, width);
        ssSetInputPortDataType(S, i, SS_UINT8);
        ssSetInputPortDirectFeedThrough(S, i, 1);
    }

    ssSetNumOutputPorts(S, 0);
    ssSetNumSampleTimes(S, 1);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    UNUSED_ARG(S);
    UNUSED_ARG(tid);
    // No simulation-time output; handled by TLC + C wrapper
}

static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S);
}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    real_T width  = *mxGetPr(WIDTH);
    real_T height = *mxGetPr(HEIGHT);

    if (!ssWriteRTWParamSettings(S, NINPUTS,
        SSWRITE_VALUE_NUM,  "Width",  width,
        SSWRITE_VALUE_NUM,  "Height", height))
    {
        ssSetErrorStatus(S, "Failed to write RTW parameters.");
        return;
    }
}
#endif

#ifdef MATLAB_MEX_FILE
# include "simulink.c"
#else
# include "cg_sfun.h"
#endif
