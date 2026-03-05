#define S_FUNCTION_NAME MQTT_Callback
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define ADDRESS       (ssGetSFcnParam(S,0))
#define CLIENTID        (ssGetSFcnParam(S,1))
#define TOPIC        (ssGetSFcnParam(S,2))
#define QOS        (ssGetSFcnParam(S,3))
#define DATA_N      (ssGetSFcnParam(S,4))
#define QUEUE_NAME       (ssGetSFcnParam(S,5))
#define LENGHT        (ssGetSFcnParam(S,6))

static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 7);
    ssSetSFcnParamNotTunable( S, 0);
    ssSetSFcnParamNotTunable( S, 1);
    ssSetSFcnParamNotTunable( S, 2);
    ssSetSFcnParamNotTunable( S, 3);
    ssSetSFcnParamNotTunable( S, 4);
    ssSetSFcnParamNotTunable( S, 5);
    ssSetSFcnParamNotTunable( S, 6);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch will be reported by Simulink */
	}
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
    char *str_address = mxArrayToString(ADDRESS);
    char *str_clientid = mxArrayToString(CLIENTID);
    char *str_topic = mxArrayToString(TOPIC);
    char *str_queue_name = mxArrayToString(QUEUE_NAME);

    if (!ssWriteRTWParamSettings(S, 7, 
                                 SSWRITE_VALUE_QSTR,"address",str_address,
                                 SSWRITE_VALUE_QSTR,"clientid",str_clientid,
                                 SSWRITE_VALUE_QSTR,"topic",str_topic,
                                 SSWRITE_VALUE_NUM,"qos",
                                 (real_T) (*(mxGetPr(QOS))),
                                 SSWRITE_VALUE_NUM,"data_n",
                                 (real_T) (*(mxGetPr(DATA_N))),
                                 SSWRITE_VALUE_QSTR,"queue_name",str_queue_name,
                                 SSWRITE_VALUE_NUM,"length",
                                 (real_T) (*(mxGetPr(LENGHT)))
                                 )) {
        mxFree(str_address);
        mxFree(str_clientid);
        mxFree(str_topic);
        mxFree(str_queue_name);
        return;
    }
    mxFree(str_address);
    mxFree(str_clientid);
    mxFree(str_topic);
    mxFree(str_queue_name);
    
}

#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif