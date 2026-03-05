#define S_FUNCTION_NAME POSIX_QueueReceive
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define QUEUE_NAME       (ssGetSFcnParam(S,0))
#define BUFFER_ENABLE       (ssGetSFcnParam(S,1))
#define BUFFER_SIZE       (ssGetSFcnParam(S,2))
#define BUFFER_NAME       (ssGetSFcnParam(S,3))

static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 4);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch will be reported by Simulink */
	}

    ssSetSFcnParamNotTunable( S, 0);
    ssSetSFcnParamNotTunable( S, 1);
    ssSetSFcnParamNotTunable( S, 2);
    ssSetSFcnParamNotTunable( S, 3);

    ssSetNumInputPorts( S, 0);
    ssSetNumOutputPorts( S, 2);
    ssSetOutputPortWidth( S, 0, 1);

    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);

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
    char *name = mxArrayToString(BUFFER_NAME);

    if (!ssWriteRTWParamSettings(S, 4,
                                 SSWRITE_VALUE_QSTR,"QueueName",str,
                                 SSWRITE_VALUE_NUM,"bufferEnable",
                                 (real_T) (*(mxGetPr(BUFFER_ENABLE))),
                                 SSWRITE_VALUE_NUM,"bufferSize",
                                 (real_T) (*(mxGetPr(BUFFER_SIZE))),
                                 SSWRITE_VALUE_QSTR,"bufferName",name
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