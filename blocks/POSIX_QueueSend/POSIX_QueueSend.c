#define S_FUNCTION_NAME POSIX_QueueSend
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define QUEUE_NAME       (ssGetSFcnParam(S,0))


static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 1);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch will be reported by Simulink */
	}

    ssSetSFcnParamNotTunable( S, 0);

    ssSetNumInputPorts( S, 1);
    ssSetInputPortWidth( S, 0, 1);
    ssSetNumOutputPorts( S, 0);

    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);

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
    char *str = mxArrayToString(QUEUE_NAME);

    if (!ssWriteRTWParamSettings(S, 1, 
                                 SSWRITE_VALUE_QSTR,"QueueName",str
                                 )) {
        mxFree(str);
        return;
    }
    mxFree(str);
    
}

#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif