#define S_FUNCTION_NAME PIGPIO_Write
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#define IO_PIN              (ssGetSFcnParam(S,0))

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);
	ssSetSFcnParamNotTunable( S, 0);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return;
	}
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if (!ssSetNumOutputPorts(S,0)) return;

    ssSetNumSampleTimes(S, 1);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid){}

static void mdlTerminate(SimStruct *S){}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW

static void mdlRTW(SimStruct *S)
{
    if (!ssWriteRTWParamSettings(S, 1, 
                                 SSWRITE_VALUE_NUM,"io_pin",
                                 (real_T) (*(mxGetPr(IO_PIN)))
                                 )) {
        return;
    }
}

#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif