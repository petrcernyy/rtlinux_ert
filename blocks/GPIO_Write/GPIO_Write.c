#define S_FUNCTION_NAME GPIO_Write
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"

/* ----------------- Parameters ----------------- */
enum { P_IO_PIN = 0, NPARAMS };

#define IO_PIN          (ssGetSFcnParam(S, P_IO_PIN))
#define PCOUNT          NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* IO_PIN */
    if (!mxIsDouble(IO_PIN) || mxIsComplex(IO_PIN) || mxGetNumberOfElements(IO_PIN) != 1) {
        ssSetErrorStatus(S, "IO_PIN must be a real scalar.");
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
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

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
    real_T io_pin_val = *mxGetPr(IO_PIN);

    /* Write parameters to the .rtw file for TLC to read */
    if (!ssWriteRTWParamSettings(S, 1, 
            SSWRITE_VALUE_NUM, "io_pin", io_pin_val)) 
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
#include "simulink.c" 
#else
#include "cg_sfun.h" 
#endif