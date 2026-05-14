#define S_FUNCTION_NAME CAMERA
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"
#include <string.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
enum { P_DEVICE_ID = 0, P_WIDTH, P_HEIGHT, P_FORMAT, NPARAMS };

#define DEVICE_ID   (ssGetSFcnParam(S, P_DEVICE_ID))
#define WIDTH_P     (ssGetSFcnParam(S, P_WIDTH))
#define HEIGHT_P    (ssGetSFcnParam(S, P_HEIGHT))
#define FORMAT_P    (ssGetSFcnParam(S, P_FORMAT))
#define PCOUNT      NPARAMS

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* DEVICE_ID */
    if (!mxIsDouble(DEVICE_ID) || mxIsComplex(DEVICE_ID) || mxGetNumberOfElements(DEVICE_ID) != 1) {
        ssSetErrorStatus(S, "DEVICE_ID must be a scalar numeric.");
        return;
    }

    /* WIDTH and HEIGHT */
    if (!mxIsDouble(WIDTH_P)  || mxIsComplex(WIDTH_P)  || mxGetNumberOfElements(WIDTH_P)  != 1 ||
        !mxIsDouble(HEIGHT_P) || mxIsComplex(HEIGHT_P) || mxGetNumberOfElements(HEIGHT_P) != 1) {
        ssSetErrorStatus(S, "WIDTH and HEIGHT must be scalar numerics.");
        return;
    } else {
        const int_T W = (int_T)mxGetScalar(WIDTH_P);
        const int_T H = (int_T)mxGetScalar(HEIGHT_P);
        if (W <= 0 || H <= 0) {
            ssSetErrorStatus(S, "WIDTH and HEIGHT must be positive.");
            return;
        }
    }

    /* FORMAT */
    if (!mxIsChar(FORMAT_P) || mxIsEmpty(FORMAT_P)) {
        ssSetErrorStatus(S, "FORMAT must be a non-empty string (e.g., 'RGB888').");
        return;
    }
}

/* ----------------- S-Function boilerplate ----------------- */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    int_T W, H;

    /* Parameter initialization */
    ssSetNumSFcnParams(S, PCOUNT);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    /* Validate parameters before proceeding */
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) return;

    /* Make params nontunable so codegen can treat sizes as constants */
    for (i = 0; i < PCOUNT; i++) {
        ssSetSFcnParamNotTunable(S, i);
    }

    /* Port definitions */
    ssSetNumInputPorts(S, 0);
    
    if (!ssSetNumOutputPorts(S, 1)) return;

    /* Read sizes as numerics for port width */
    W = (int_T)mxGetScalar(WIDTH_P);
    H = (int_T)mxGetScalar(HEIGHT_P);

    /* Outport 0: uint8 vector of length W*H*3 */
    ssSetOutputPortWidth(S, 0, W * H * 3);
    ssSetOutputPortDataType(S, 0, SS_UINT8);
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);

    /* Explicitly initialize all work vectors and states to 0 */
    ssSetNumIWork(S, 0);
    ssSetNumRWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Execution options */
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
    /* No simulation-time output here; TLC will inline camera code at RTW */
    UNUSED_ARG(S); 
    UNUSED_ARG(tid);
}

static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S);
}

/* ----------------- RTW: write params for TLC ----------------- */
#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    char *format = mxArrayToString(FORMAT_P);
    real_T dev;
    real_T width;
    real_T height;

    if (format != NULL) {
        dev    = mxGetScalar(DEVICE_ID);
        width  = mxGetScalar(WIDTH_P);
        height = mxGetScalar(HEIGHT_P);

        if (!ssWriteRTWParamSettings(S, 4,
                SSWRITE_VALUE_NUM , "Device_Id", dev,
                SSWRITE_VALUE_NUM , "WIDTH"    , width,
                SSWRITE_VALUE_NUM , "HEIGHT"   , height,
                SSWRITE_VALUE_QSTR, "Format"   , format))
        {
            /* Write failed, but we still drop down to safely free memory */
            ssSetErrorStatus(S, "ssWriteRTWParamSettings failed.");
        }
        mxFree(format);
    } else {
        ssSetErrorStatus(S, "Failed to read FORMAT string.");
    }
}
#endif /* MATLAB_MEX_FILE */

/* ----------------- C++ Interoperability Guard End ----------------- */
#ifdef __cplusplus
}
#endif

/* ----------------- Required Trailing Includes ----------------- */
#ifdef MATLAB_MEX_FILE
#  include "simulink.c"
#else
#  include "cg_sfun.h"
#endif