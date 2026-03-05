//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
//
// Code generated for Simulink model 'rpi_camera'.
//
// Model version                  : 1.399
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Mar  5 14:07:23 2026
//
// Target selection: rtlinux_ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "rpi_camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int stop_program = 0;
void sigint_handler(int sig)
{
  if (sig == SIGINT) {
    printf("\n[Main] Ctrl+C (SIGINT) detected. Stopping...\n");
  } else if (sig == SIGTERM) {
    printf("\n[Main] Kill command (SIGTERM) detected. Stopping...\n");
  } else {
    printf("\n[Main] Signal %d detected. Stopping...\n", sig);
  }

  stop_program = 1;
}

int main(void)
{
  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigint_handler);
  signal(SIGHUP, sigint_handler);

  // Initialize model
  rpi_camera_initialize();
  while (!stop_program) {
    usleep(100000);
  }

  printf("[Main] Terminating...\n");
  rpi_camera_terminate();
  printf("[Main] Done.\n");
  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
