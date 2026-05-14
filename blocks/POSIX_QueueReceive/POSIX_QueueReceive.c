#define S_FUNCTION_NAME POSIX_QueueReceive
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <string.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_QUEUE_NAME = 0, P_BUFFER_ENABLE, P_BUFFER_SIZE, P_BUFFER_NAME, NPARAMS };

#define QUEUE_NAME      (ssGetSFcnParam(S, P_QUEUE_NAME))
#define BUFFER_ENABLE   (ssGetSFcnParam(S, P_BUFFER_ENABLE))
#define BUFFER_SIZE     (ssGetSFcnParam(S, P_BUFFER_SIZE))
#define BUFFER_NAME     (ssGetSFcnParam(S, P_BUFFER_NAME))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* QUEUE_NAME */
    if (!mxIsChar(QUEUE_NAME) || mxIsEmpty(QUEUE_NAME)) {
        ssSetErrorStatus(S, "QUEUE_NAME must be a non-empty string.");
        return;
    }

    /* BUFFER_ENABLE */
    if (!mxIsDouble(BUFFER_ENABLE) || mxIsComplex(BUFFER_ENABLE) || mxGetNumberOfElements(BUFFER_ENABLE) != 1) {
        ssSetErrorStatus(S, "BUFFER_ENABLE must be a real scalar.");
        return;
    }

    /* BUFFER_SIZE */
    if (!mxIsDouble(BUFFER_SIZE) || mxIsComplex(BUFFER_SIZE) || mxGetNumberOfElements(BUFFER_SIZE) != 1) {
        ssSetErrorStatus(S, "BUFFER_SIZE must be a real scalar.");
        return;
    }

    /* BUFFER_NAME */
    if (!mxIsChar(BUFFER_NAME) || mxIsEmpty(BUFFER_NAME)) {
        ssSetErrorStatus(S, "BUFFER_NAME must be a non-empty string.");
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
        return; /* Parameter mismatch will be reported by Simulink */
    }

    /* Validate parameters before proceeding */
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) return;

    /* Make parameters non-tunable */
    for (i = 0; i < PCOUNT; i++) {
        ssSetSFcnParamNotTunable(S, i);
    }

    /* Port definitions */
    if (!ssSetNumInputPorts(S, 0)) return;
    if (!ssSetNumOutputPorts(S, 2)) return;

    /* Set both ports to dynamically sized (cleaning up the original redundancy) */
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);

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
    char *q_str = mxArrayToString(QUEUE_NAME);
    char *b_str = mxArrayToString(BUFFER_NAME);
    real_T b_en;
    real_T b_size;

    /* Ensure both strings were allocated successfully */
    if (q_str != NULL && b_str != NULL) {
        b_en   = *mxGetPr(BUFFER_ENABLE);
        b_size = *mxGetPr(BUFFER_SIZE);

        /* Write parameters to the .rtw file for TLC to read */
        if (!ssWriteRTWParamSettings(S, 4,
                SSWRITE_VALUE_QSTR, "QueueName",    q_str,
                SSWRITE_VALUE_NUM,  "bufferEnable", b_en,
                SSWRITE_VALUE_NUM,  "bufferSize",   b_size,
                SSWRITE_VALUE_QSTR, "bufferName",   b_str)) 
        {
            /* Safe failure drop-through */
            ssSetErrorStatus(S, "Failed to write RTW parameters.");
        }
    } else {
        ssSetErrorStatus(S, "Failed to allocate memory for string parameters.");
    }

    /* Safely free both strings to prevent memory leaks */
    if (q_str) mxFree(q_str);
    if (b_str) mxFree(b_str);
}
#endif /* MATLAB_MEX_FILE */

/* ----------------- C++ Interoperability Guard End ----------------- */
#ifdef __cplusplus
}
#endif

/* ----------------- Required Trailing Includes ----------------- */
#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif