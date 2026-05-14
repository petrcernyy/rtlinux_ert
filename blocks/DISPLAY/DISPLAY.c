#define S_FUNCTION_NAME DISPLAY
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_WIDTH = 0, P_HEIGHT, NPARAMS };

#define WIDTH           (ssGetSFcnParam(S, P_WIDTH))
#define HEIGHT          (ssGetSFcnParam(S, P_HEIGHT))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* WIDTH and HEIGHT */
    if (!mxIsDouble(WIDTH)  || mxIsComplex(WIDTH)  || mxGetNumberOfElements(WIDTH)  != 1 ||
        !mxIsDouble(HEIGHT) || mxIsComplex(HEIGHT) || mxGetNumberOfElements(HEIGHT) != 1) {
        ssSetErrorStatus(S, "WIDTH and HEIGHT must be real scalars.");
        return;
    } else {
        real_T w_val = *mxGetPr(WIDTH);
        real_T h_val = *mxGetPr(HEIGHT);
        
        if (w_val <= 0.0 || h_val <= 0.0) {
            ssSetErrorStatus(S, "WIDTH and HEIGHT must be positive.");
            return;
        }
    }
}

/* ----------------- S-Function boilerplate ----------------- */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    int_T w, h;

    /* Parameter initialization */
    ssSetNumSFcnParams(S, PCOUNT);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    /* Validate parameters before proceeding */
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) return;

    /* Mark parameters as non-tunable */
    for (i = 0; i < PCOUNT; i++) {
        ssSetSFcnParamNotTunable(S, i);
    }

    /* Input Port definitions (e.g., R, G, B channels) */
    if (!ssSetNumInputPorts(S, 3)) return;

    w = (int_T)(*mxGetPr(WIDTH));
    h = (int_T)(*mxGetPr(HEIGHT));

    for (i = 0; i < 3; i++) {
        ssSetInputPortMatrixDimensions(S, i, h, w);
        ssSetInputPortDataType(S, i, SS_UINT8);
        ssSetInputPortDirectFeedThrough(S, i, 1);
    }

    /* Output Port definitions */
    if (!ssSetNumOutputPorts(S, 0)) return;

    /* Explicitly initialize all work vectors and states to 0 */
    ssSetNumIWork(S, 0);
    ssSetNumRWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Execution options */
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* No simulation-time output; handled by TLC + C wrapper */
    UNUSED_ARG(S);
    UNUSED_ARG(tid);
}

static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S);
}

/* ----------------- RTW: write params for TLC ----------------- */
#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    real_T width  = *mxGetPr(WIDTH);
    real_T height = *mxGetPr(HEIGHT);

    if (!ssWriteRTWParamSettings(S, PCOUNT,
        SSWRITE_VALUE_NUM,  "Width",  width,
        SSWRITE_VALUE_NUM,  "Height", height))
    {
        /* Safe failure drop-through */
        ssSetErrorStatus(S, "Failed to write RTW parameters.");
    }
}
#endif /* MATLAB_MEX_FILE */

/* ----------------- C++ Interoperability Guard End ----------------- */
#ifdef __cplusplus
}
#endif

/* ----------------- Required Trailing Includes ----------------- */
#ifdef MATLAB_MEX_FILE
# include "simulink.c"
#else
# include "cg_sfun.h"
#endif