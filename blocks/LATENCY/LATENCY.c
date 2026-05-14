#define S_FUNCTION_NAME LATENCY
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <string.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_NAME = 0, P_TICK_RES, P_BUFFER, NPARAMS };

#define NAME_PARAM      (ssGetSFcnParam(S, P_NAME))
#define TICK_RES_PARAM  (ssGetSFcnParam(S, P_TICK_RES))
#define BUFFER_PARAM    (ssGetSFcnParam(S, P_BUFFER))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* NAME_PARAM */
    if (!mxIsChar(NAME_PARAM) || mxIsEmpty(NAME_PARAM)) {
        ssSetErrorStatus(S, "Name must be a non-empty string.");
        return;
    }

    /* TICK_RES_PARAM */
    if (!mxIsDouble(TICK_RES_PARAM) || mxIsComplex(TICK_RES_PARAM) || mxGetNumberOfElements(TICK_RES_PARAM) != 1) {
        ssSetErrorStatus(S, "TickRes must be a real scalar.");
        return;
    }

    /* BUFFER_PARAM */
    if (!mxIsDouble(BUFFER_PARAM) || mxIsComplex(BUFFER_PARAM) || mxGetNumberOfElements(BUFFER_PARAM) != 1) {
        ssSetErrorStatus(S, "FrameBuffer must be a real scalar.");
        return;
    }
}

/* ----------------- S-Function boilerplate ----------------- */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;

    /* Parameter initialization */
    ssSetNumSFcnParams(S, PCOUNT);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    /* Validate parameters before proceeding */
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) return;

    /* Make parameters non-tunable (required for code generation) */
    for (i = 0; i < PCOUNT; i++) {
        ssSetSFcnParamNotTunable(S, i);
    }

    /* Port definitions (Sink Block) */
    if (!ssSetNumInputPorts(S, 0)) return;
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
    ssSetOptions(S,
        SS_OPTION_WORKS_WITH_CODE_REUSE |
        SS_OPTION_SFUNCTION_INLINED_FOR_RTW |
        SS_OPTION_CALL_TERMINATE_ON_EXIT);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Simulation-only output (ignored by CodeGen) */
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
    /* Get values from the Mask/Block */
    char *name_str = mxArrayToString(NAME_PARAM);
    real_T tick_val;
    real_T buff_val;

    if (name_str != NULL) {
        tick_val = *mxGetPr(TICK_RES_PARAM);
        buff_val = *mxGetPr(BUFFER_PARAM);

        /* Write them to the .rtw file for TLC to read */
        if (!ssWriteRTWParamSettings(S, 3,
                SSWRITE_VALUE_QSTR, "Name",        name_str,
                SSWRITE_VALUE_NUM,  "TickRes",     tick_val,
                SSWRITE_VALUE_NUM,  "FrameBuffer", buff_val))
        {
            /* Write failed, but we still drop down to safely free memory */
            ssSetErrorStatus(S, "ssWriteRTWParamSettings failed.");
        }
        mxFree(name_str);
    } else {
        ssSetErrorStatus(S, "Failed to read Name string.");
    }
}
#endif /* MATLAB_MEX_FILE */

/* ----------------- C++ Interoperability Guard End ----------------- */
#ifdef __cplusplus
}
#endif

/* ----------------- Required Trailing Includes ----------------- */
#ifdef MATLAB_MEX_FILE
#  include "simulink.c"
#else
#  include "cg_sfun.h"
#endif