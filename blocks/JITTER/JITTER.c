#define S_FUNCTION_NAME JITTER
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

// Define Parameter Indices
#define NAME_PARAM      (ssGetSFcnParam(S,0))
#define TICK_RES_PARAM  (ssGetSFcnParam(S,1))
#define BUFFER_PARAM    (ssGetSFcnParam(S,2))
#define NPARAMS 3

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, NPARAMS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    // Make parameters non-tunable (required for code generation)
    for (int i = 0; i < NPARAMS; ++i) {
        ssSetSFcnParamTunable(S, i, 0);
    }

    // --- NO PORTS (Sink Block) ---
    ssSetNumOutputPorts(S, 0);
    ssSetNumInputPorts(S, 0);
    // -----------------------------

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
    // Simulation-only output (ignored by CodeGen)
    UNUSED_ARG(S); UNUSED_ARG(tid);
}

static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S);
}

// =================================================================
// CRITICAL SECTION: This function passes "40" to the Code Generator
// =================================================================
#if defined(MATLAB_MEX_FILE)
#  define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    // Get values from the Mask/Block
    char *name_str   = mxArrayToString(NAME_PARAM);
    real_T tick_val  = *mxGetPr(TICK_RES_PARAM);
    real_T buff_val  = *mxGetPr(BUFFER_PARAM);

    // Write them to the .rtw file for TLC to read
    if (!ssWriteRTWParamSettings(S, 3,
            SSWRITE_VALUE_QSTR, "Name",        name_str,
            SSWRITE_VALUE_NUM,  "TickRes",     tick_val,
            SSWRITE_VALUE_NUM,  "FrameBuffer", buff_val))
    {
        ssSetErrorStatus(S, "ssWriteRTWParamSettings failed.");
        return;
    }
    
    if (name_str) mxFree(name_str);
}
#endif 
// =================================================================

#ifdef MATLAB_MEX_FILE
#  include "simulink.c"
#else
#  include "cg_sfun.h"
#endif