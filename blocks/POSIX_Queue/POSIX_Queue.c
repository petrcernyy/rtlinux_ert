#define S_FUNCTION_NAME POSIX_Queue
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define QUEUE_NAME       (ssGetSFcnParam(S,0))
#define LENGHT        (ssGetSFcnParam(S,1))


static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 2);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch will be reported by Simulink */
	}

    ssSetSFcnParamNotTunable( S, 0);
    ssSetSFcnParamNotTunable( S, 1);

    if (!ssSetNumInputPorts(S, 0)) return;
    if (!ssSetNumOutputPorts(S, 0)) return;

    ssSetSimStateCompliance(S,HAS_NO_SIM_STATE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid){}

static void mdlTerminate(SimStruct *S){}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW

static void mdlRTW(SimStruct *S)
{
    char *str = mxArrayToString(QUEUE_NAME);

    if (!ssWriteRTWParamSettings(S, 2, 
                                 SSWRITE_VALUE_QSTR,"QueueName",str,
                                 SSWRITE_VALUE_NUM,"QueueLenght",
                                 (real_T) (*(mxGetPr(LENGHT)))
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