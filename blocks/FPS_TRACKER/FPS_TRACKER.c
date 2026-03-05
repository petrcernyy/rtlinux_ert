#define S_FUNCTION_NAME FPS_TRACKER
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    ssSetNumInputPorts(S, 0);

    ssSetNumOutputPorts(S, 1);
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);

    ssSetNumSampleTimes(S, 1);

    ssSetOptions(S,
        SS_OPTION_WORKS_WITH_CODE_REUSE |
        SS_OPTION_SFUNCTION_INLINED_FOR_RTW |
        SS_OPTION_EXCEPTION_FREE_CODE |
        SS_OPTION_CALL_TERMINATE_ON_EXIT);
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
# define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    // No RTW parameters in this S-function
}
#endif

#ifdef MATLAB_MEX_FILE
# include "simulink.c"
#else
# include "cg_sfun.h"
#endif
