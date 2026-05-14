#define S_FUNCTION_NAME POSIX_Thread
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* ----------------- Parameters ----------------- */
#define TASK_NAME       (ssGetSFcnParam(S,0))
#define TICK_RES        (ssGetSFcnParam(S,1))
#define POLICY          (ssGetSFcnParam(S,2))
#define PRIORITY        (ssGetSFcnParam(S,3))
#define LOCK_MEMORY     (ssGetSFcnParam(S,4))
#define AFFINITY0       (ssGetSFcnParam(S,5))
#define AFFINITY1       (ssGetSFcnParam(S,6))
#define AFFINITY2       (ssGetSFcnParam(S,7))
#define AFFINITY3       (ssGetSFcnParam(S,8))
#define PCOUNT          9

/* ----------------- C++ Interoperability Guard ----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- Parameter checks ----------------- */
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    /* TASK_NAME */
    if (!mxIsChar(TASK_NAME) || mxIsEmpty(TASK_NAME)) {
        ssSetErrorStatus(S, "TaskName must be a non-empty string.");
        return;
    }

    /* TICK_RES */
    if (!mxIsDouble(TICK_RES) || mxIsComplex(TICK_RES) || mxGetNumberOfElements(TICK_RES) != 1) {
        ssSetErrorStatus(S, "TickRes must be a real scalar (seconds).");
        return;
    } else {
        real_T ts = *mxGetPr(TICK_RES);
        if (!(ts > 0.0) || !mxIsFinite(ts)) {
            ssSetErrorStatus(S, "TickRes must be > 0.");
            return;
        }
    }

    /* POLICY */
    if (!mxIsChar(POLICY) || mxIsEmpty(POLICY)) {
        ssSetErrorStatus(S, "SchedPolicy must be a string: 'SCHED_OTHER', 'SCHED_FIFO', or 'SCHED_RR'.");
        return;
    } else {
        char *pol = mxArrayToString(POLICY);
        if (pol != NULL) { /* Added NULL check for safety */
            if (strcmp(pol, "SCHED_OTHER") != 0 &&
                strcmp(pol, "SCHED_FIFO")  != 0 &&
                strcmp(pol, "SCHED_RR")    != 0) {
                mxFree(pol);
                ssSetErrorStatus(S, "SchedPolicy must be 'SCHED_OTHER', 'SCHED_FIFO', or 'SCHED_RR'.");
                return;
            }
            mxFree(pol);
        } else {
            ssSetErrorStatus(S, "Memory allocation error when reading SchedPolicy.");
            return;
        }
    }

    /* PRIORITY */
    if (!mxIsDouble(PRIORITY) || mxIsComplex(PRIORITY) || mxGetNumberOfElements(PRIORITY) != 1) {
        ssSetErrorStatus(S, "Priority must be a real scalar.");
        return;
    } else {
        double pr = *mxGetPr(PRIORITY);
        if (!mxIsFinite(pr) || pr < 0.0) {
            ssSetErrorStatus(S, "Priority must be >= 0.");
            return;
        }
    }
}

/* ----------------- S-Function boilerplate ----------------- */
static void mdlInitializeSizes(SimStruct *S)
{
    int i;

    /* Parameter initialization */
    ssSetNumSFcnParams(S, PCOUNT);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Simulink will flag mismatch, return immediately */
        return;
    }

    /* Validate parameters before proceeding */
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) return;

    for (i = 0; i < PCOUNT; i++) {
        ssSetSFcnParamNotTunable(S, i);
    }

    /* Port definitions */
    ssSetNumInputPorts(S, 0);
    
    if (!ssSetNumOutputPorts(S, 1)) return; /* Added safety check */
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortDataType(S, 0, SS_FCN_CALL);

    /* Work vectors and states */
    ssSetNumIWork(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumPWork(S, 1); /* Assuming this will hold thread context later */
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Execution options */
    ssSupportsMultipleExecInstances(S, 1);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE |
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    real_T ts = *mxGetPr(TICK_RES);
    ssSetSampleTime(S, 0, ts);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetCallSystemOutput(S, 0); /* function-call output */
}

#define MDL_START
static void mdlStart(SimStruct *S)
{
    /* Initialize PWork to NULL to ensure clean termination guards */
    void **pwork = ssGetPWork(S);
    if (pwork != NULL) {
        pwork[0] = NULL;
    }
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    ssCallSystemWithTid(S, 0, tid);
}

static void mdlTerminate(SimStruct *S) 
{
    /* Guard against memory leaks if PWork is dynamically allocated */
    void **pwork = ssGetPWork(S);
    if (pwork != NULL && pwork[0] != NULL) {
        /* Example: free(pwork[0]); */
        pwork[0] = NULL; /* Clear the pointer */
    }
}

/* ----------------- RTW: write params for TLC ----------------- */
#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    char *taskName   = mxArrayToString(TASK_NAME);
    char *policy     = mxArrayToString(POLICY);
    real_T tickRes   = *mxGetPr(TICK_RES);
    real_T priority  = *mxGetPr(PRIORITY);
    real_T lockMem   = *mxGetPr(LOCK_MEMORY);
    real_T affinity0 = *mxGetPr(AFFINITY0);
    real_T affinity1 = *mxGetPr(AFFINITY1);
    real_T affinity2 = *mxGetPr(AFFINITY2);
    real_T affinity3 = *mxGetPr(AFFINITY3);

    /* Check for NULL allocations before writing to RTW */
    if (taskName != NULL && policy != NULL) {
        if (!ssWriteRTWParamSettings(S, 9,
            SSWRITE_VALUE_QSTR, "TaskName",     taskName,
            SSWRITE_VALUE_NUM,  "TickRes",      tickRes,
            SSWRITE_VALUE_QSTR, "SchedPolicy",  policy,
            SSWRITE_VALUE_NUM,  "Priority",     priority,
            SSWRITE_VALUE_NUM,  "LockMemory",   lockMem,
            SSWRITE_VALUE_NUM,  "Affinity0",    affinity0,
            SSWRITE_VALUE_NUM,  "Affinity1",    affinity1,
            SSWRITE_VALUE_NUM,  "Affinity2",    affinity2,
            SSWRITE_VALUE_NUM,  "Affinity3",    affinity3))
        {
            /* Write failed, but we still drop down to safely free memory */
        }
    } else {
        ssSetErrorStatus(S, "Failed to allocate memory for RTW parameters.");
    }

    if (taskName) mxFree(taskName);
    if (policy)   mxFree(policy);
}
#endif /* MATLAB_MEX_FILE */

/* ----------------- C++ Interoperability Guard End ----------------- */
#ifdef __cplusplus
}
#endif

/* ----------------- Required Trailing Includes ----------------- */
#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif