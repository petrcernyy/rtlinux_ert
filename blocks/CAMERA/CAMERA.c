#define S_FUNCTION_NAME CAMERA
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "mex.h"     /* for mxArrayToString, mxGetScalar, etc. */

/* Param indices */
enum { P_DEVICE_ID = 0, P_WIDTH, P_HEIGHT, P_FORMAT, NPARAMS };

/* Param accessors */
#define DEVICE_ID   (ssGetSFcnParam(S, P_DEVICE_ID))
#define WIDTH_P     (ssGetSFcnParam(S, P_WIDTH))
#define HEIGHT_P    (ssGetSFcnParam(S, P_HEIGHT))
#define FORMAT_P    (ssGetSFcnParam(S, P_FORMAT))

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, NPARAMS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    /* Make params nontunable so codegen can treat sizes as constants */
    for (int i = 0; i < NPARAMS; ++i) ssSetSFcnParamTunable(S, i, 0);

    /* Basic checks: WIDTH/HEIGHT must be scalar numeric, FORMAT must be string */
    if (!mxIsDouble(WIDTH_P)  || mxIsComplex(WIDTH_P)  || mxGetNumberOfElements(WIDTH_P)  != 1 ||
        !mxIsDouble(HEIGHT_P) || mxIsComplex(HEIGHT_P) || mxGetNumberOfElements(HEIGHT_P) != 1) {
        ssSetErrorStatus(S, "WIDTH and HEIGHT must be scalar numerics.");
        return;
    }
    if (!mxIsChar(FORMAT_P)) {
        ssSetErrorStatus(S, "FORMAT must be a string (e.g., 'RGB888').");
        return;
    }

    if (!ssSetNumOutputPorts(S, 1)) return;

    /* Read sizes as numerics */
    const int_T W = (int_T)mxGetScalar(WIDTH_P);
    const int_T H = (int_T)mxGetScalar(HEIGHT_P);

    if (W <= 0 || H <= 0) {
        ssSetErrorStatus(S, "WIDTH and HEIGHT must be positive.");
        return;
    }

    /* Outport 0: uint8 vector of length W*H*3 */
    ssSetOutputPortWidth   (S, 0, W * H * 3);
    ssSetOutputPortDataType(S, 0, SS_UINT8);
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);

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
    UNUSED_ARG(S); UNUSED_ARG(tid);
}

static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S);
}

#if defined(MATLAB_MEX_FILE)
#  define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    char *format = mxArrayToString(FORMAT_P);
    if (!format) {
        ssSetErrorStatus(S, "Failed to read FORMAT string.");
        return;
    }

    const real_T dev   = mxGetScalar(DEVICE_ID);
    const real_T width = mxGetScalar(WIDTH_P);
    const real_T height= mxGetScalar(HEIGHT_P);

    if (!ssWriteRTWParamSettings(S, 4,
            SSWRITE_VALUE_NUM , "Device_Id", dev,
            SSWRITE_VALUE_NUM , "WIDTH"    , width,
            SSWRITE_VALUE_NUM , "HEIGHT"   , height,
            SSWRITE_VALUE_QSTR, "Format"   , format))
    {
        mxFree(format);
        ssSetErrorStatus(S, "ssWriteRTWParamSettings failed.");
        return;
    }
    mxFree(format);
}
#endif /* MATLAB_MEX_FILE */

#ifdef MATLAB_MEX_FILE
#  include "simulink.c"
#else
#  include "cg_sfun.h"
#endif
