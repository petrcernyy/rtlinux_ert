#define S_FUNCTION_NAME POSIX_SocketSend
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <string.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_STATEMENT = 0, P_DATA_NUM, NPARAMS };

#define STATEMENT       (ssGetSFcnParam(S, P_STATEMENT))
#define DATA_NUM        (ssGetSFcnParam(S, P_DATA_NUM))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* STATEMENT */
    if (!mxIsChar(STATEMENT) || mxIsEmpty(STATEMENT)) {
        ssSetErrorStatus(S, "STATEMENT must be a non-empty string.");
        return;
    }

    /* DATA_NUM */
    if (!mxIsDouble(DATA_NUM) || mxIsComplex(DATA_NUM) || mxGetNumberOfElements(DATA_NUM) != 1) {
        ssSetErrorStatus(S, "DATA_NUM must be a real scalar.");
        return;
    } else {
        real_T val = *mxGetPr(DATA_NUM);
        if (val < 0.0) {
            ssSetErrorStatus(S, "DATA_NUM must be a non-negative integer.");
            return;
        }
    }
}

/* ----------------- S-Function boilerplate ----------------- */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    int len;

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

    /* Port definitions (Dynamically sized based on parameter) */
    len = (int)(*mxGetPr(DATA_NUM));
    
    if (!ssSetNumInputPorts(S, len)) return;
    
    for(i = 0; i < len; i++) {
        ssSetInputPortWidth(S, i, DYNAMICALLY_SIZED);
        ssSetInputPortDirectFeedThrough(S, i, 1);
    }

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
    char *statement = mxArrayToString(STATEMENT);
    real_T data_num_val;

    if (statement != NULL) {
        data_num_val = *mxGetPr(DATA_NUM);

        /* Write parameters to the .rtw file for TLC to read */
        if (!ssWriteRTWParamSettings(S, 2, 
                SSWRITE_VALUE_QSTR, "Statement", statement,
                SSWRITE_VALUE_NUM,  "data_num",  data_num_val)) 
        {
            /* Safe failure drop-through */
            ssSetErrorStatus(S, "Failed to write RTW parameters.");
        }
        mxFree(statement); /* Safely free string to prevent memory leaks */
    } else {
        ssSetErrorStatus(S, "Failed to allocate memory for STATEMENT string.");
    }
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