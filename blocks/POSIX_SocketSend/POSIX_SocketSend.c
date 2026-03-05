#define S_FUNCTION_NAME POSIX_SocketSend
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define STATEMENT       (ssGetSFcnParam(S,0))
#define DATA_NUM        (ssGetSFcnParam(S,1))


static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 2);
    ssSetSFcnParamNotTunable(S, 0);
    ssSetSFcnParamNotTunable(S, 1);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return;
	}

    int len = (int) (*(mxGetPr(DATA_NUM)));

    if (!ssSetNumInputPorts(S, len)) return;
    if (!ssSetNumOutputPorts(S, 0)) return;
    for(int i = 0; i < len; i++) {
        ssSetInputPortWidth(S, i, DYNAMICALLY_SIZED);
        ssSetInputPortDirectFeedThrough(S, i, 1);
    }

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
    char *statement = mxArrayToString(STATEMENT);

    if (!ssWriteRTWParamSettings(S, 2, 
                                 SSWRITE_VALUE_QSTR,"Statement",statement,
                                 SSWRITE_VALUE_NUM,"data_num",
                                 (real_T) (*(mxGetPr(DATA_NUM))
                                 ))) {
        mxFree(statement);
        return;
    }
    mxFree(statement);
}

#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif