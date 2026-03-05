#define S_FUNCTION_NAME POSIX_Socket
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define SOCKET_NAME       (ssGetSFcnParam(S,0))
#define BACKLOG        (ssGetSFcnParam(S,1))
#define TICK_RES        (ssGetSFcnParam(S,2))

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return;
        }

    ssSetSFcnParamNotTunable( S, 0);
    ssSetSFcnParamNotTunable( S, 1);
    ssSetSFcnParamNotTunable( S, 2);

    ssSetNumInputPorts( S, 0);
    ssSetNumOutputPorts( S, 1);
    ssSetOutputPortWidth( S, 0, 1);

    ssSetOutputPortDataType(  S, 0, SS_FCN_CALL);
    ssSetNumIWork(            S, 1);
    ssSetNumRWork(            S, 0);
    ssSetNumPWork(            S, 1);
    ssSetNumSampleTimes(      S, 1);
    ssSetNumContStates(       S, 0);
    ssSetNumDiscStates(       S, 0);
    ssSetNumModes(            S, 0);
    ssSetNumNonsampledZCs(    S, 0);

    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetCallSystemOutput(S, 0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    ssCallSystemWithTid(S, 0, tid);
}

static void mdlTerminate(SimStruct *S){}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW

static void mdlRTW(SimStruct *S)
{
    char *sck_name = mxArrayToString(SOCKET_NAME);

    if (!ssWriteRTWParamSettings(S, 3, 
                                 SSWRITE_VALUE_QSTR,"SocketName",sck_name,
                                 SSWRITE_VALUE_NUM,"Backlog",
                                 (real_T) (*(mxGetPr(BACKLOG))),
                                 SSWRITE_VALUE_NUM,"TickRes",
                                 (real_T) (*(mxGetPr(TICK_RES)))
                                 )) {
        mxFree(sck_name);
        return;
    }
    mxFree(sck_name);
    
}

#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif