#define S_FUNCTION_NAME PIGPIO_I2C_CONFIG
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#define ADDR       (ssGetSFcnParam(S,0))

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);
    ssSetSFcnParamNotTunable( S, 0);
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
    real_T Address = (real_T) mxGetScalar(ADDR);
    
    if (!ssWriteRTWParamSettings(S, 1, 
                                 SSWRITE_VALUE_NUM,"addr", Address
                                 )) {
        return;
    }
}





#endif /* mdlRTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif