#define S_FUNCTION_NAME POSIX_Socket
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <string.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_SOCKET_NAME = 0, P_BACKLOG, P_TICK_RES, NPARAMS };

#define SOCKET_NAME     (ssGetSFcnParam(S, P_SOCKET_NAME))
#define BACKLOG         (ssGetSFcnParam(S, P_BACKLOG))
#define TICK_RES        (ssGetSFcnParam(S, P_TICK_RES))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* SOCKET_NAME */
    if (!mxIsChar(SOCKET_NAME) || mxIsEmpty(SOCKET_NAME)) {
        ssSetErrorStatus(S, "SOCKET_NAME must be a non-empty string.");
        return;
    }

    /* BACKLOG */
    if (!mxIsDouble(BACKLOG) || mxIsComplex(BACKLOG) || mxGetNumberOfElements(BACKLOG) != 1) {
        ssSetErrorStatus(S, "BACKLOG must be a real scalar.");
        return;
    }

    /* TICK_RES */
    if (!mxIsDouble(TICK_RES) || mxIsComplex(TICK_RES) || mxGetNumberOfElements(TICK_RES) != 1) {
        ssSetErrorStatus(S, "TICK_RES must be a real scalar.");
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
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortDataType(S, 0, SS_FCN_CALL); /* Function-call output */

    /* Work vectors and states */
    ssSetNumIWork(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumPWork(S, 1); /* PWork included for potential socket context */
    ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Execution options */
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1.0); /* Hardcoded to 1 based on original logic */
    ssSetOffsetTime(S, 0, 0.0);
    ssSetCallSystemOutput(S, 0); /* Function-call port 0 */
}

#define MDL_START
static void mdlStart(SimStruct *S)
{
    /* Initialize PWork to NULL to ensure clean termination guards */
    void **pwork = ssGetPWork(S);
    if (pwork != NULL) {
        pwork[0] = NULL;
    }
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Execute the connected function-call subsystem */
    ssCallSystemWithTid(S, 0, tid);
}

static void mdlTerminate(SimStruct *S)
{
    void **pwork = ssGetPWork(S);
    if (pwork != NULL && pwork[0] != NULL) {
        pwork[0] = NULL;
    }
}

/* ----------------- RTW: write params for TLC ----------------- */
#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    char *sck_name = mxArrayToString(SOCKET_NAME);
    real_T backlog_val;
    real_T tick_res_val;

    if (sck_name != NULL) {
        backlog_val  = *mxGetPr(BACKLOG);
        tick_res_val = *mxGetPr(TICK_RES);

        /* Write parameters to the .rtw file for TLC to read */
        if (!ssWriteRTWParamSettings(S, 3, 
                SSWRITE_VALUE_QSTR, "SocketName", sck_name,
                SSWRITE_VALUE_NUM,  "Backlog",    backlog_val,
                SSWRITE_VALUE_NUM,  "TickRes",    tick_res_val)) 
        {
            /* Safe failure drop-through */
            ssSetErrorStatus(S, "Failed to write RTW parameters.");
        }
        mxFree(sck_name); /* Safely free string to prevent memory leaks */
    } else {
        ssSetErrorStatus(S, "Failed to allocate memory for SOCKET_NAME string.");
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