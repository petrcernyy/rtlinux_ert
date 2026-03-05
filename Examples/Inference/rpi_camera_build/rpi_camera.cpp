//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: rpi_camera.cpp
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
#include "rpi_camera_types.h"
#include "rtwtypes.h"
#include "rpi_camera_private.h"
#include <cmath>
#include <cstring>
#include "invokeinterpreter.hpp"

// Block signals (default storage)
B_rpi_camera_T rpi_camera_B;

// Block states (default storage)
DW_rpi_camera_T rpi_camera_DW;

// Real-time model
RT_MODEL_rpi_camera_T rpi_camera_M_{ };

RT_MODEL_rpi_camera_T *const rpi_camera_M{ &rpi_camera_M_ };

// Forward declaration for local functions
static void rpi_camera_permute(const real32_T a[400], real32_T b[400]);
static void rpi_camera_shiftdim(const real32_T x[400], real32_T b[400]);
static void rpi_camera_shiftdim_l(const real32_T x[100], real32_T b[100]);
static void rpi_camera_TFLiteModel_predict(coder_TFLiteModel_rpi_camera_T
  *b_this, const uint8_T varargin_1[307200], real32_T varargout_1[400], real32_T
  varargout_2[100], real32_T varargout_3[100], real32_T *varargout_4);

// --- Cyclictest Variables ---
static struct timespec jitter_start_time = { 0, 0 };

static int jitter_is_first_step = 1;
static unsigned long long jitter_step_counter = 0;

// --- Buffering ---
static double *jitter_latency_buffer = NULL;
static int jitter_sample_count = 0;
static const int jitter_max_samples = 1000000;
enum {
  CAM_WIDTH = 320,
  CAM_HEIGHT = 320,
  CAM_CH = 3,
  CAM_FRAMEBYTES = 307200
};

static uint8_t cam_frame_buf[CAM_FRAMEBYTES];
static int frameCounter = 0;
static double fps = 0.0;
static struct timespec fps_start_time;

// --- GLOBALS ---
static int fbfd = -1;
static char *fbp = NULL;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static int is_fb_ready = 0;

// --- INIT FUNCTION ---
void InitFB(void)
{
  if (is_fb_ready)
    return;

  // 1. Open Device
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    printf("[Display] Error: cannot open /dev/fb0\n");
    return;
  }

  // 2. Get Screen Info
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    printf("[Display] Error reading fixed info.\n");
    return;
  }

  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    printf("[Display] Error reading variable info.\n");
    return;
  }

  // 3. Map to Memory
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((long)fbp == -1) {
    printf("[Display] Error: failed to map framebuffer.\n");
    return;
  }

  // 4. Clear Screen to Black
  memset(fbp, 0, screensize);
  printf("[Display] Initialized: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres,
         vinfo.bits_per_pixel);
  is_fb_ready = 1;
}

// --- CLEANUP ---
void CleanupFB()
{
  if (fbp && fbp != (char*)-1) {
    munmap(fbp, screensize);
    fbp = NULL;
  }

  if (fbfd >= 0) {
    close(fbfd);
    fbfd = -1;
  }

  is_fb_ready = 0;
}

// --- STREAM FUNCTION (FAST) ---
void StreamFrame(const uint8_t *R, const uint8_t *G, const uint8_t *B)
{
  // Safety Check
  if (!is_fb_ready || !fbp)
    return;
  int src_W = 320;
  int src_H = 320;
  int dest_W = vinfo.xres;
  int dest_H = vinfo.yres;
  int bpp = vinfo.bits_per_pixel / 8;

  // Scaling Loop
  for (int y = 0; y < dest_H; ++y) {
    int src_y = (y * src_H) / dest_H;
    long int dest_row_offset = y * finfo.line_length;
    for (int x = 0; x < dest_W; ++x) {
      int src_x = (x * src_W) / dest_W;
      int idx = src_x * src_H + src_y; // Column Major
      long int location = dest_row_offset + x * bpp;
      if (vinfo.bits_per_pixel == 16) {
        uint8_t r = R[idx];
        uint8_t g = G[idx];
        uint8_t b = B[idx];
        unsigned short int t = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((unsigned short int*)(fbp + location)) = t;
      } else if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = B[idx];
        *(fbp + location + 1) = G[idx];
        *(fbp + location + 2) = R[idx];
        *(fbp + location + 3) = 255;
      }
    }
  }
}

static struct gpiod_chip *chip_17 = NULL;
static struct gpiod_line_request *req_17 = NULL;
pthread_t Camera_t;
volatile int Camera_running = 1;

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC                   1000000000L
#endif

// Definition of task Camera
void *Camera(void *arg)
{
  int64_t period_ns = 33000000;
  struct timespec next_wakeup_time;
  struct timespec interval = {
    .tv_sec = (time_t)(period_ns / NSEC_PER_SEC),
    .tv_nsec = (long)(period_ns % NSEC_PER_SEC)
  };

  clock_gettime(CLOCK_MONOTONIC, &next_wakeup_time);
  while (Camera_running) {
    // Wait until next shot.
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_wakeup_time, NULL);

    // Call the system : <Root>/Function-Call Subsystem

    // Output and update for function-call system: '<Root>/Function-Call Subsystem' 
    {
      static const char_T sd[21]{ "ssd_mobilenet.tflite" };

      invokeinterpreter * networkObj;
      cell_wrap_6_rpi_camera_T ab;
      cell_wrap_6_rpi_camera_T ac;
      cell_wrap_6_rpi_camera_T ad;
      cell_wrap_6_rpi_camera_T b;
      cell_wrap_6_rpi_camera_T bb;
      cell_wrap_6_rpi_camera_T bc;
      cell_wrap_6_rpi_camera_T bd;
      cell_wrap_6_rpi_camera_T c;
      cell_wrap_6_rpi_camera_T cb;
      cell_wrap_6_rpi_camera_T cc;
      cell_wrap_6_rpi_camera_T cd;
      cell_wrap_6_rpi_camera_T d;
      cell_wrap_6_rpi_camera_T db;
      cell_wrap_6_rpi_camera_T dc;
      cell_wrap_6_rpi_camera_T dd;
      cell_wrap_6_rpi_camera_T e;
      cell_wrap_6_rpi_camera_T eb;
      cell_wrap_6_rpi_camera_T ec;
      cell_wrap_6_rpi_camera_T ed;
      cell_wrap_6_rpi_camera_T f;
      cell_wrap_6_rpi_camera_T fb;
      cell_wrap_6_rpi_camera_T fc;
      cell_wrap_6_rpi_camera_T fd;
      cell_wrap_6_rpi_camera_T g;
      cell_wrap_6_rpi_camera_T gb;
      cell_wrap_6_rpi_camera_T gc;
      cell_wrap_6_rpi_camera_T gd;
      cell_wrap_6_rpi_camera_T h;
      cell_wrap_6_rpi_camera_T hb;
      cell_wrap_6_rpi_camera_T hc;
      cell_wrap_6_rpi_camera_T hd;
      cell_wrap_6_rpi_camera_T i;
      cell_wrap_6_rpi_camera_T ib;
      cell_wrap_6_rpi_camera_T ic;
      cell_wrap_6_rpi_camera_T id;
      cell_wrap_6_rpi_camera_T j_p;
      cell_wrap_6_rpi_camera_T jb;
      cell_wrap_6_rpi_camera_T jc;
      cell_wrap_6_rpi_camera_T jd;
      cell_wrap_6_rpi_camera_T k;
      cell_wrap_6_rpi_camera_T kb;
      cell_wrap_6_rpi_camera_T kc;
      cell_wrap_6_rpi_camera_T kd;
      cell_wrap_6_rpi_camera_T l;
      cell_wrap_6_rpi_camera_T lb;
      cell_wrap_6_rpi_camera_T lc;
      cell_wrap_6_rpi_camera_T ld;
      cell_wrap_6_rpi_camera_T m;
      cell_wrap_6_rpi_camera_T mb;
      cell_wrap_6_rpi_camera_T mc;
      cell_wrap_6_rpi_camera_T md;
      cell_wrap_6_rpi_camera_T n;
      cell_wrap_6_rpi_camera_T nb;
      cell_wrap_6_rpi_camera_T nc;
      cell_wrap_6_rpi_camera_T nd;
      cell_wrap_6_rpi_camera_T o;
      cell_wrap_6_rpi_camera_T ob;
      cell_wrap_6_rpi_camera_T oc;
      cell_wrap_6_rpi_camera_T od;
      cell_wrap_6_rpi_camera_T p;
      cell_wrap_6_rpi_camera_T pb;
      cell_wrap_6_rpi_camera_T pc;
      cell_wrap_6_rpi_camera_T pd;
      cell_wrap_6_rpi_camera_T q;
      cell_wrap_6_rpi_camera_T qb;
      cell_wrap_6_rpi_camera_T qc;
      cell_wrap_6_rpi_camera_T qd;
      cell_wrap_6_rpi_camera_T r;
      cell_wrap_6_rpi_camera_T rb;
      cell_wrap_6_rpi_camera_T rc;
      cell_wrap_6_rpi_camera_T s;
      cell_wrap_6_rpi_camera_T sb;
      cell_wrap_6_rpi_camera_T sc;
      cell_wrap_6_rpi_camera_T t;
      cell_wrap_6_rpi_camera_T tb;
      cell_wrap_6_rpi_camera_T tc;
      cell_wrap_6_rpi_camera_T u;
      cell_wrap_6_rpi_camera_T ub;
      cell_wrap_6_rpi_camera_T uc;
      cell_wrap_6_rpi_camera_T v;
      cell_wrap_6_rpi_camera_T vb;
      cell_wrap_6_rpi_camera_T vc;
      cell_wrap_6_rpi_camera_T w;
      cell_wrap_6_rpi_camera_T wb;
      cell_wrap_6_rpi_camera_T wc;
      cell_wrap_6_rpi_camera_T x;
      cell_wrap_6_rpi_camera_T xb;
      cell_wrap_6_rpi_camera_T xc;
      cell_wrap_6_rpi_camera_T y_p;
      cell_wrap_6_rpi_camera_T yb;
      cell_wrap_6_rpi_camera_T yc;
      real_T tmp;
      int32_T b_k;
      int32_T bb_p;
      int32_T d_p;
      int32_T displayImg_tmp;
      int32_T displayImg_tmp_p;
      int32_T i_p;
      int32_T iindx;
      int32_T j;
      int32_T kb_p;
      int32_T l_p;
      int32_T o_p;
      int32_T pixelIndex;
      int32_T pixelIndexCpy;
      int32_T qY;
      int32_T qY_e;
      int32_T qY_p;
      int32_T theChar;
      real32_T boxes[400];
      real32_T b_x[100];
      real32_T labels[100];
      real32_T a__1;
      real32_T b_x_p;
      char_T rd[21];
      char_T name_data[14];
      uint8_T valR_idx_0;
      uint8_T valR_idx_1;
      uint8_T y;
      boolean_T tmp_e;
      boolean_T tmp_p;
      static const char_T td[6]{ 'p', 'e', 'r', 's', 'o', 'n' };

      static const char_T yd[7]{ 'u', 'n', 'k', 'n', 'o', 'w', 'n' };

      static const char_T ud[7]{ 'b', 'i', 'c', 'y', 'c', 'l', 'e' };

      static const char_T wd[10]{ 'm', 'o', 't', 'o', 'r', 'c', 'y', 'c', 'l',
        'e' };

      static const char_T xd[8]{ 'a', 'i', 'r', 'p', 'l', 'a', 'n', 'e' };

      static const char_T be[5]{ 't', 'r', 'a', 'i', 'n' };

      static const char_T ce[5]{ 't', 'r', 'u', 'c', 'k' };

      static const char_T ee[13]{ 't', 'r', 'a', 'f', 'f', 'i', 'c', ' ', 'l',
        'i', 'g', 'h', 't' };

      static const char_T fe[12]{ 'f', 'i', 'r', 'e', ' ', 'h', 'y', 'd', 'r',
        'a', 'n', 't' };

      static const char_T ge[11]{ 's', 't', 'r', 'e', 'e', 't', ' ', 's', 'i',
        'g', 'n' };

      static const char_T he[9]{ 's', 't', 'o', 'p', ' ', 's', 'i', 'g', 'n' };

      static const char_T ie[13]{ 'p', 'a', 'r', 'k', 'i', 'n', 'g', ' ', 'm',
        'e', 't', 'e', 'r' };

      static const char_T je[5]{ 'b', 'e', 'n', 'c', 'h' };

      static const char_T ne[5]{ 'h', 'o', 'r', 's', 'e' };

      static const char_T oe[5]{ 's', 'h', 'e', 'e', 'p' };

      static const char_T qe[8]{ 'e', 'l', 'e', 'p', 'h', 'a', 'n', 't' };

      static const char_T se[5]{ 'z', 'e', 'b', 'r', 'a' };

      static const char_T te[7]{ 'g', 'i', 'r', 'a', 'f', 'f', 'e' };

      static const char_T ve[8]{ 'b', 'a', 'c', 'k', 'p', 'a', 'c', 'k' };

      static const char_T we[8]{ 'u', 'm', 'b', 'r', 'e', 'l', 'l', 'a' };

      static const char_T ye[11]{ 'e', 'y', 'e', ' ', 'g', 'l', 'a', 's', 's',
        'e', 's' };

      static const char_T af[7]{ 'h', 'a', 'n', 'd', 'b', 'a', 'g' };

      static const char_T cf[8]{ 's', 'u', 'i', 't', 'c', 'a', 's', 'e' };

      static const char_T df[7]{ 'f', 'r', 'i', 's', 'b', 'e', 'e' };

      static const char_T ff[9]{ 's', 'n', 'o', 'w', 'b', 'o', 'a', 'r', 'd' };

      static const char_T gf[11]{ 's', 'p', 'o', 'r', 't', 's', ' ', 'b', 'a',
        'l', 'l' };

      static const char_T jf[12]{ 'b', 'a', 's', 'e', 'b', 'a', 'l', 'l', ' ',
        'b', 'a', 't' };

      static const char_T kf[14]{ 'b', 'a', 's', 'e', 'b', 'a', 'l', 'l', ' ',
        'g', 'l', 'o', 'v', 'e' };

      static const char_T lf[10]{ 's', 'k', 'a', 't', 'e', 'b', 'o', 'a', 'r',
        'd' };

      static const char_T mf[9]{ 's', 'u', 'r', 'f', 'b', 'o', 'a', 'r', 'd' };

      static const char_T nf[13]{ 't', 'e', 'n', 'n', 'i', 's', ' ', 'r', 'a',
        'c', 'k', 'e', 't' };

      static const char_T of[6]{ 'b', 'o', 't', 't', 'l', 'e' };

      static const char_T pf[5]{ 'p', 'l', 'a', 't', 'e' };

      static const char_T qf[10]{ 'w', 'i', 'n', 'e', ' ', 'g', 'l', 'a', 's',
        's' };

      static const char_T tf[5]{ 'k', 'n', 'i', 'f', 'e' };

      static const char_T uf[5]{ 's', 'p', 'o', 'o', 'n' };

      static const char_T wf[6]{ 'b', 'a', 'n', 'a', 'n', 'a' };

      static const char_T xf[5]{ 'a', 'p', 'p', 'l', 'e' };

      static const char_T yf[8]{ 's', 'a', 'n', 'd', 'w', 'i', 'c', 'h' };

      static const char_T ag[6]{ 'o', 'r', 'a', 'n', 'g', 'e' };

      static const char_T bg[8]{ 'b', 'r', 'o', 'c', 'c', 'o', 'l', 'i' };

      static const char_T cg[6]{ 'c', 'a', 'r', 'r', 'o', 't' };

      static const char_T dg[7]{ 'h', 'o', 't', ' ', 'd', 'o', 'g' };

      static const char_T eg[5]{ 'p', 'i', 'z', 'z', 'a' };

      static const char_T fg[5]{ 'd', 'o', 'n', 'u', 't' };

      static const char_T hg[5]{ 'c', 'h', 'a', 'i', 'r' };

      static const char_T ig[5]{ 'c', 'o', 'u', 'c', 'h' };

      static const char_T jg[12]{ 'p', 'o', 't', 't', 'e', 'd', ' ', 'p', 'l',
        'a', 'n', 't' };

      static const char_T lg[6]{ 'm', 'i', 'r', 'r', 'o', 'r' };

      static const char_T mg[12]{ 'd', 'i', 'n', 'i', 'n', 'g', ' ', 't', 'a',
        'b', 'l', 'e' };

      static const char_T ng[6]{ 'w', 'i', 'n', 'd', 'o', 'w' };

      static const char_T pg[6]{ 't', 'o', 'i', 'l', 'e', 't' };

      static const char_T sg[6]{ 'l', 'a', 'p', 't', 'o', 'p' };

      static const char_T tg[5]{ 'm', 'o', 'u', 's', 'e' };

      static const char_T ug[6]{ 'r', 'e', 'm', 'o', 't', 'e' };

      static const char_T vg[8]{ 'k', 'e', 'y', 'b', 'o', 'a', 'r', 'd' };

      static const char_T wg[10]{ 'c', 'e', 'l', 'l', ' ', 'p', 'h', 'o', 'n',
        'e' };

      static const char_T xg[9]{ 'm', 'i', 'c', 'r', 'o', 'w', 'a', 'v', 'e' };

      static const char_T ah[7]{ 't', 'o', 'a', 's', 't', 'e', 'r' };

      static const char_T ch[12]{ 'r', 'e', 'f', 'r', 'i', 'g', 'e', 'r', 'a',
        't', 'o', 'r' };

      static const char_T dh[7]{ 'b', 'l', 'e', 'n', 'd', 'e', 'r' };

      static const char_T fh[5]{ 'c', 'l', 'o', 'c', 'k' };

      static const char_T hh[8]{ 's', 'c', 'i', 's', 's', 'o', 'r', 's' };

      static const char_T ih[10]{ 't', 'e', 'd', 'd', 'y', ' ', 'b', 'e', 'a',
        'r' };

      static const char_T jh[10]{ 'h', 'a', 'i', 'r', ' ', 'd', 'r', 'i', 'e',
        'r' };

      static const char_T kh[10]{ 't', 'o', 'o', 't', 'h', 'b', 'r', 'u', 's',
        'h' };

      static const char_T lh[10]{ 'h', 'a', 'i', 'r', ' ', 'b', 'r', 'u', 's',
        'h' };

      boolean_T exitg1;

      // S-Function (JITTER): '<S1>/JITTER'
      {
        struct timespec now;
        long long period_ns = (long long)(33 * 1000000.0);// Convert ms to ns
        double latency_us = 0.0;

        // 1. Get current absolute kernel time
        clock_gettime(CLOCK_MONOTONIC, &now);
        if (jitter_is_first_step) {
          // Initialize the "Perfect Timeline" anchor point
          jitter_start_time = now;
          jitter_is_first_step = 0;
          jitter_step_counter = 0;
          latency_us = 0.0;
        } else {
          // 2. Increment step counter
          jitter_step_counter++;

          // 3. Calculate where we SHOULD be (The "Perfect" time)
          // Target = Start + (Steps * Period)
          long long total_offset_ns = jitter_step_counter * period_ns;
          long long target_sec = jitter_start_time.tv_sec + (total_offset_ns /
            1000000000LL);
          long long target_nsec = jitter_start_time.tv_nsec + (total_offset_ns %
            1000000000LL);

          // Handle nsec rollover if start_time.tv_nsec was large
          if (target_nsec >= 1000000000LL) {
            target_sec++;
            target_nsec -= 1000000000LL;
          }

          // 4. Calculate Latency (Now - Ideal)
          long long diff_sec = now.tv_sec - target_sec;
          long long diff_nsec = now.tv_nsec - target_nsec;
          long long latency_ns = (diff_sec * 1000000000LL) + diff_nsec;

          // Convert to microseconds for easier reading
          latency_us = (double)latency_ns / 1000.0;
        }

        // 5. Store in buffer
        if (jitter_latency_buffer && jitter_sample_count < jitter_max_samples) {
          jitter_latency_buffer[jitter_sample_count++] = latency_us;
        }
      }

      // S-Function (CAMERA): '<S1>/CAMERA1'
      size_t sz = 0;
      const uint8_t *src = camera_wrapper_get_latest(&sz);
      if (src && sz >= CAM_FRAMEBYTES) {
        memcpy(cam_frame_buf, src, CAM_FRAMEBYTES);
      }

      {
        uint8_T *y0 = (uint8_T *)&rpi_camera_B.CAMERA1[0];
        memcpy(y0, cam_frame_buf, CAM_FRAMEBYTES);
      }

      // MATLAB Function: '<S1>/MATLAB Function10'
      if (!rpi_camera_DW.net_not_empty) {
        rpi_camera_DW.net.Mean = 127.5F;
        rpi_camera_DW.net.StandardDeviation = 127.5F;
        rpi_camera_DW.net.EnableProfiling = false;
        rpi_camera_DW.net.Verbose = false;
        rpi_camera_DW.net.InternalNumThreads = -1;
        networkObj = new invokeinterpreter;
        for (b_k = 0; b_k < 21; b_k++) {
          rd[b_k] = sd[b_k];
        }

        std::mem_fn(&invokeinterpreter::setup)(networkObj, &rd[0],
          rpi_camera_DW.net.InternalNumThreads);
        rpi_camera_DW.net.Network = networkObj;
        std::memset(&rpi_camera_DW.net.PredictedTotalOutput[0], 0, 601U * sizeof
                    (real32_T));
        rpi_camera_DW.net.matlabCodegenIsDeleted = false;
        rpi_camera_DW.net_not_empty = true;
        b.f1.size[0] = 1;
        b.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          b.f1.data[b_k] = td[b_k];
        }

        c.f1.size[0] = 1;
        c.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          c.f1.data[b_k] = ud[b_k];
        }

        d.f1.size[0] = 1;
        d.f1.size[1] = 3;
        d.f1.data[0] = 'c';
        d.f1.data[1] = 'a';
        d.f1.data[2] = 'r';
        e.f1.size[0] = 1;
        e.f1.size[1] = 10;
        for (b_k = 0; b_k < 10; b_k++) {
          e.f1.data[b_k] = wd[b_k];
        }

        f.f1.size[0] = 1;
        f.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          f.f1.data[b_k] = xd[b_k];
        }

        g.f1.size[0] = 1;
        g.f1.size[1] = 3;
        g.f1.data[0] = 'b';
        g.f1.data[1] = 'u';
        g.f1.data[2] = 's';
        h.f1.size[0] = 1;
        h.f1.size[1] = 5;
        i.f1.size[0] = 1;
        i.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          h.f1.data[b_k] = be[b_k];
          i.f1.data[b_k] = ce[b_k];
        }

        j_p.f1.size[0] = 1;
        j_p.f1.size[1] = 4;
        j_p.f1.data[0] = 'b';
        j_p.f1.data[1] = 'o';
        j_p.f1.data[2] = 'a';
        j_p.f1.data[3] = 't';
        k.f1.size[0] = 1;
        k.f1.size[1] = 13;
        for (b_k = 0; b_k < 13; b_k++) {
          k.f1.data[b_k] = ee[b_k];
        }

        l.f1.size[0] = 1;
        l.f1.size[1] = 12;
        for (b_k = 0; b_k < 12; b_k++) {
          l.f1.data[b_k] = fe[b_k];
        }

        m.f1.size[0] = 1;
        m.f1.size[1] = 11;
        for (b_k = 0; b_k < 11; b_k++) {
          m.f1.data[b_k] = ge[b_k];
        }

        n.f1.size[0] = 1;
        n.f1.size[1] = 9;
        for (b_k = 0; b_k < 9; b_k++) {
          n.f1.data[b_k] = he[b_k];
        }

        o.f1.size[0] = 1;
        o.f1.size[1] = 13;
        for (b_k = 0; b_k < 13; b_k++) {
          o.f1.data[b_k] = ie[b_k];
        }

        p.f1.size[0] = 1;
        p.f1.size[1] = 5;
        q.f1.size[0] = 1;
        q.f1.size[1] = 4;
        q.f1.data[0] = 'b';
        q.f1.data[1] = 'i';
        q.f1.data[2] = 'r';
        q.f1.data[3] = 'd';
        r.f1.size[0] = 1;
        r.f1.size[1] = 3;
        s.f1.size[0] = 1;
        s.f1.size[1] = 3;
        r.f1.data[0] = 'c';
        s.f1.data[0] = 'd';
        r.f1.data[1] = 'a';
        s.f1.data[1] = 'o';
        r.f1.data[2] = 't';
        s.f1.data[2] = 'g';
        t.f1.size[0] = 1;
        t.f1.size[1] = 5;
        u.f1.size[0] = 1;
        u.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          p.f1.data[b_k] = je[b_k];
          t.f1.data[b_k] = ne[b_k];
          u.f1.data[b_k] = oe[b_k];
        }

        v.f1.size[0] = 1;
        v.f1.size[1] = 3;
        v.f1.data[0] = 'c';
        v.f1.data[1] = 'o';
        v.f1.data[2] = 'w';
        w.f1.size[0] = 1;
        w.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          w.f1.data[b_k] = qe[b_k];
        }

        x.f1.size[0] = 1;
        x.f1.size[1] = 4;
        x.f1.data[0] = 'b';
        x.f1.data[1] = 'e';
        x.f1.data[2] = 'a';
        x.f1.data[3] = 'r';
        y_p.f1.size[0] = 1;
        y_p.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          y_p.f1.data[b_k] = se[b_k];
        }

        ab.f1.size[0] = 1;
        ab.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          ab.f1.data[b_k] = te[b_k];
        }

        bb.f1.size[0] = 1;
        bb.f1.size[1] = 3;
        bb.f1.data[0] = 'h';
        bb.f1.data[1] = 'a';
        bb.f1.data[2] = 't';
        cb.f1.size[0] = 1;
        cb.f1.size[1] = 8;
        db.f1.size[0] = 1;
        db.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          cb.f1.data[b_k] = ve[b_k];
          db.f1.data[b_k] = we[b_k];
        }

        eb.f1.size[0] = 1;
        eb.f1.size[1] = 4;
        eb.f1.data[0] = 's';
        eb.f1.data[1] = 'h';
        eb.f1.data[2] = 'o';
        eb.f1.data[3] = 'e';
        fb.f1.size[0] = 1;
        fb.f1.size[1] = 11;
        for (b_k = 0; b_k < 11; b_k++) {
          fb.f1.data[b_k] = ye[b_k];
        }

        gb.f1.size[0] = 1;
        gb.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          gb.f1.data[b_k] = af[b_k];
        }

        hb.f1.size[0] = 1;
        hb.f1.size[1] = 3;
        hb.f1.data[0] = 't';
        hb.f1.data[1] = 'i';
        hb.f1.data[2] = 'e';
        ib.f1.size[0] = 1;
        ib.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          ib.f1.data[b_k] = cf[b_k];
        }

        jb.f1.size[0] = 1;
        jb.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          jb.f1.data[b_k] = df[b_k];
        }

        kb.f1.size[0] = 1;
        kb.f1.size[1] = 4;
        kb.f1.data[0] = 's';
        kb.f1.data[1] = 'k';
        kb.f1.data[2] = 'i';
        kb.f1.data[3] = 's';
        lb.f1.size[0] = 1;
        lb.f1.size[1] = 9;
        for (b_k = 0; b_k < 9; b_k++) {
          lb.f1.data[b_k] = ff[b_k];
        }

        mb.f1.size[0] = 1;
        mb.f1.size[1] = 11;
        for (b_k = 0; b_k < 11; b_k++) {
          mb.f1.data[b_k] = gf[b_k];
        }

        nb.f1.size[0] = 1;
        nb.f1.size[1] = 4;
        nb.f1.data[0] = 'k';
        nb.f1.data[1] = 'i';
        nb.f1.data[2] = 't';
        nb.f1.data[3] = 'e';
        ob.f1.size[0] = 1;
        ob.f1.size[1] = 12;
        for (b_k = 0; b_k < 12; b_k++) {
          ob.f1.data[b_k] = jf[b_k];
        }

        pb.f1.size[0] = 1;
        pb.f1.size[1] = 14;
        for (b_k = 0; b_k < 14; b_k++) {
          pb.f1.data[b_k] = kf[b_k];
        }

        qb.f1.size[0] = 1;
        qb.f1.size[1] = 10;
        for (b_k = 0; b_k < 10; b_k++) {
          qb.f1.data[b_k] = lf[b_k];
        }

        rb.f1.size[0] = 1;
        rb.f1.size[1] = 9;
        for (b_k = 0; b_k < 9; b_k++) {
          rb.f1.data[b_k] = mf[b_k];
        }

        sb.f1.size[0] = 1;
        sb.f1.size[1] = 13;
        for (b_k = 0; b_k < 13; b_k++) {
          sb.f1.data[b_k] = nf[b_k];
        }

        tb.f1.size[0] = 1;
        tb.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          tb.f1.data[b_k] = of[b_k];
        }

        ub.f1.size[0] = 1;
        ub.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          ub.f1.data[b_k] = pf[b_k];
        }

        vb.f1.size[0] = 1;
        vb.f1.size[1] = 10;
        for (b_k = 0; b_k < 10; b_k++) {
          vb.f1.data[b_k] = qf[b_k];
        }

        wb.f1.size[0] = 1;
        wb.f1.size[1] = 3;
        wb.f1.data[0] = 'c';
        wb.f1.data[1] = 'u';
        wb.f1.data[2] = 'p';
        xb.f1.size[0] = 1;
        xb.f1.size[1] = 4;
        xb.f1.data[0] = 'f';
        xb.f1.data[1] = 'o';
        xb.f1.data[2] = 'r';
        xb.f1.data[3] = 'k';
        yb.f1.size[0] = 1;
        yb.f1.size[1] = 5;
        ac.f1.size[0] = 1;
        ac.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          yb.f1.data[b_k] = tf[b_k];
          ac.f1.data[b_k] = uf[b_k];
        }

        bc.f1.size[0] = 1;
        bc.f1.size[1] = 4;
        bc.f1.data[0] = 'b';
        bc.f1.data[1] = 'o';
        bc.f1.data[2] = 'w';
        bc.f1.data[3] = 'l';
        cc.f1.size[0] = 1;
        cc.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          cc.f1.data[b_k] = wf[b_k];
        }

        dc.f1.size[0] = 1;
        dc.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          dc.f1.data[b_k] = xf[b_k];
        }

        ec.f1.size[0] = 1;
        ec.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          ec.f1.data[b_k] = yf[b_k];
        }

        fc.f1.size[0] = 1;
        fc.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          fc.f1.data[b_k] = ag[b_k];
        }

        gc.f1.size[0] = 1;
        gc.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          gc.f1.data[b_k] = bg[b_k];
        }

        hc.f1.size[0] = 1;
        hc.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          hc.f1.data[b_k] = cg[b_k];
        }

        ic.f1.size[0] = 1;
        ic.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          ic.f1.data[b_k] = dg[b_k];
        }

        jc.f1.size[0] = 1;
        jc.f1.size[1] = 5;
        kc.f1.size[0] = 1;
        kc.f1.size[1] = 5;
        lc.f1.size[0] = 1;
        lc.f1.size[1] = 4;
        lc.f1.data[0] = 'c';
        lc.f1.data[1] = 'a';
        lc.f1.data[2] = 'k';
        lc.f1.data[3] = 'e';
        mc.f1.size[0] = 1;
        mc.f1.size[1] = 5;
        nc.f1.size[0] = 1;
        nc.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          jc.f1.data[b_k] = eg[b_k];
          kc.f1.data[b_k] = fg[b_k];
          mc.f1.data[b_k] = hg[b_k];
          nc.f1.data[b_k] = ig[b_k];
        }

        oc.f1.size[0] = 1;
        oc.f1.size[1] = 12;
        for (b_k = 0; b_k < 12; b_k++) {
          oc.f1.data[b_k] = jg[b_k];
        }

        pc.f1.size[0] = 1;
        pc.f1.size[1] = 3;
        pc.f1.data[0] = 'b';
        pc.f1.data[1] = 'e';
        pc.f1.data[2] = 'd';
        qc.f1.size[0] = 1;
        qc.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          qc.f1.data[b_k] = lg[b_k];
        }

        rc.f1.size[0] = 1;
        rc.f1.size[1] = 12;
        for (b_k = 0; b_k < 12; b_k++) {
          rc.f1.data[b_k] = mg[b_k];
        }

        sc.f1.size[0] = 1;
        sc.f1.size[1] = 6;
        tc.f1.size[0] = 1;
        tc.f1.size[1] = 4;
        tc.f1.data[0] = 'd';
        tc.f1.data[1] = 'e';
        tc.f1.data[2] = 's';
        tc.f1.data[3] = 'k';
        uc.f1.size[0] = 1;
        uc.f1.size[1] = 6;
        vc.f1.size[0] = 1;
        vc.f1.size[1] = 4;
        vc.f1.data[0] = 'd';
        vc.f1.data[1] = 'o';
        vc.f1.data[2] = 'o';
        vc.f1.data[3] = 'r';
        wc.f1.size[0] = 1;
        wc.f1.size[1] = 2;
        wc.f1.data[0] = 't';
        wc.f1.data[1] = 'v';
        xc.f1.size[0] = 1;
        xc.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          sc.f1.data[b_k] = ng[b_k];
          uc.f1.data[b_k] = pg[b_k];
          xc.f1.data[b_k] = sg[b_k];
        }

        yc.f1.size[0] = 1;
        yc.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          yc.f1.data[b_k] = tg[b_k];
        }

        ad.f1.size[0] = 1;
        ad.f1.size[1] = 6;
        for (b_k = 0; b_k < 6; b_k++) {
          ad.f1.data[b_k] = ug[b_k];
        }

        bd.f1.size[0] = 1;
        bd.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          bd.f1.data[b_k] = vg[b_k];
        }

        cd.f1.size[0] = 1;
        cd.f1.size[1] = 10;
        for (b_k = 0; b_k < 10; b_k++) {
          cd.f1.data[b_k] = wg[b_k];
        }

        dd.f1.size[0] = 1;
        dd.f1.size[1] = 9;
        for (b_k = 0; b_k < 9; b_k++) {
          dd.f1.data[b_k] = xg[b_k];
        }

        ed.f1.size[0] = 1;
        ed.f1.size[1] = 4;
        ed.f1.data[0] = 'o';
        ed.f1.data[1] = 'v';
        ed.f1.data[2] = 'e';
        ed.f1.data[3] = 'n';
        fd.f1.size[0] = 1;
        fd.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          fd.f1.data[b_k] = ah[b_k];
        }

        gd.f1.size[0] = 1;
        gd.f1.size[1] = 4;
        gd.f1.data[0] = 's';
        gd.f1.data[1] = 'i';
        gd.f1.data[2] = 'n';
        gd.f1.data[3] = 'k';
        hd.f1.size[0] = 1;
        hd.f1.size[1] = 12;
        for (b_k = 0; b_k < 12; b_k++) {
          hd.f1.data[b_k] = ch[b_k];
        }

        id.f1.size[0] = 1;
        id.f1.size[1] = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          id.f1.data[b_k] = dh[b_k];
        }

        jd.f1.size[0] = 1;
        jd.f1.size[1] = 4;
        jd.f1.data[0] = 'b';
        jd.f1.data[1] = 'o';
        jd.f1.data[2] = 'o';
        jd.f1.data[3] = 'k';
        kd.f1.size[0] = 1;
        kd.f1.size[1] = 5;
        for (b_k = 0; b_k < 5; b_k++) {
          kd.f1.data[b_k] = fh[b_k];
        }

        ld.f1.size[0] = 1;
        ld.f1.size[1] = 4;
        ld.f1.data[0] = 'v';
        ld.f1.data[1] = 'a';
        ld.f1.data[2] = 's';
        ld.f1.data[3] = 'e';
        md.f1.size[0] = 1;
        md.f1.size[1] = 8;
        for (b_k = 0; b_k < 8; b_k++) {
          md.f1.data[b_k] = hh[b_k];
        }

        nd.f1.size[0] = 1;
        nd.f1.size[1] = 10;
        od.f1.size[0] = 1;
        od.f1.size[1] = 10;
        pd.f1.size[0] = 1;
        pd.f1.size[1] = 10;
        qd.f1.size[0] = 1;
        qd.f1.size[1] = 10;
        for (b_k = 0; b_k < 10; b_k++) {
          nd.f1.data[b_k] = ih[b_k];
          od.f1.data[b_k] = jh[b_k];
          pd.f1.data[b_k] = kh[b_k];
          qd.f1.data[b_k] = lh[b_k];
        }

        rpi_camera_DW.cocoLabels[0] = b;
        rpi_camera_DW.cocoLabels[1] = c;
        rpi_camera_DW.cocoLabels[2] = d;
        rpi_camera_DW.cocoLabels[3] = e;
        rpi_camera_DW.cocoLabels[4] = f;
        rpi_camera_DW.cocoLabels[5] = g;
        rpi_camera_DW.cocoLabels[6] = h;
        rpi_camera_DW.cocoLabels[7] = i;
        rpi_camera_DW.cocoLabels[8] = j_p;
        rpi_camera_DW.cocoLabels[9] = k;
        rpi_camera_DW.cocoLabels[10] = l;
        rpi_camera_DW.cocoLabels[11] = m;
        rpi_camera_DW.cocoLabels[12] = n;
        rpi_camera_DW.cocoLabels[13] = o;
        rpi_camera_DW.cocoLabels[14] = p;
        rpi_camera_DW.cocoLabels[15] = q;
        rpi_camera_DW.cocoLabels[16] = r;
        rpi_camera_DW.cocoLabels[17] = s;
        rpi_camera_DW.cocoLabels[18] = t;
        rpi_camera_DW.cocoLabels[19] = u;
        rpi_camera_DW.cocoLabels[20] = v;
        rpi_camera_DW.cocoLabels[21] = w;
        rpi_camera_DW.cocoLabels[22] = x;
        rpi_camera_DW.cocoLabels[23] = y_p;
        rpi_camera_DW.cocoLabels[24] = ab;
        rpi_camera_DW.cocoLabels[25] = bb;
        rpi_camera_DW.cocoLabels[26] = cb;
        rpi_camera_DW.cocoLabels[27] = db;
        rpi_camera_DW.cocoLabels[28] = eb;
        rpi_camera_DW.cocoLabels[29] = fb;
        rpi_camera_DW.cocoLabels[30] = gb;
        rpi_camera_DW.cocoLabels[31] = hb;
        rpi_camera_DW.cocoLabels[32] = ib;
        rpi_camera_DW.cocoLabels[33] = jb;
        rpi_camera_DW.cocoLabels[34] = kb;
        rpi_camera_DW.cocoLabels[35] = lb;
        rpi_camera_DW.cocoLabels[36] = mb;
        rpi_camera_DW.cocoLabels[37] = nb;
        rpi_camera_DW.cocoLabels[38] = ob;
        rpi_camera_DW.cocoLabels[39] = pb;
        rpi_camera_DW.cocoLabels[40] = qb;
        rpi_camera_DW.cocoLabels[41] = rb;
        rpi_camera_DW.cocoLabels[42] = sb;
        rpi_camera_DW.cocoLabels[43] = tb;
        rpi_camera_DW.cocoLabels[44] = ub;
        rpi_camera_DW.cocoLabels[45] = vb;
        rpi_camera_DW.cocoLabels[46] = wb;
        rpi_camera_DW.cocoLabels[47] = xb;
        rpi_camera_DW.cocoLabels[48] = yb;
        rpi_camera_DW.cocoLabels[49] = ac;
        rpi_camera_DW.cocoLabels[50] = bc;
        rpi_camera_DW.cocoLabels[51] = cc;
        rpi_camera_DW.cocoLabels[52] = dc;
        rpi_camera_DW.cocoLabels[53] = ec;
        rpi_camera_DW.cocoLabels[54] = fc;
        rpi_camera_DW.cocoLabels[55] = gc;
        rpi_camera_DW.cocoLabels[56] = hc;
        rpi_camera_DW.cocoLabels[57] = ic;
        rpi_camera_DW.cocoLabels[58] = jc;
        rpi_camera_DW.cocoLabels[59] = kc;
        rpi_camera_DW.cocoLabels[60] = lc;
        rpi_camera_DW.cocoLabels[61] = mc;
        rpi_camera_DW.cocoLabels[62] = nc;
        rpi_camera_DW.cocoLabels[63] = oc;
        rpi_camera_DW.cocoLabels[64] = pc;
        rpi_camera_DW.cocoLabels[65] = qc;
        rpi_camera_DW.cocoLabels[66] = rc;
        rpi_camera_DW.cocoLabels[67] = sc;
        rpi_camera_DW.cocoLabels[68] = tc;
        rpi_camera_DW.cocoLabels[69] = uc;
        rpi_camera_DW.cocoLabels[70] = vc;
        rpi_camera_DW.cocoLabels[71] = wc;
        rpi_camera_DW.cocoLabels[72] = xc;
        rpi_camera_DW.cocoLabels[73] = yc;
        rpi_camera_DW.cocoLabels[74] = ad;
        rpi_camera_DW.cocoLabels[75] = bd;
        rpi_camera_DW.cocoLabels[76] = cd;
        rpi_camera_DW.cocoLabels[77] = dd;
        rpi_camera_DW.cocoLabels[78] = ed;
        rpi_camera_DW.cocoLabels[79] = fd;
        rpi_camera_DW.cocoLabels[80] = gd;
        rpi_camera_DW.cocoLabels[81] = hd;
        rpi_camera_DW.cocoLabels[82] = id;
        rpi_camera_DW.cocoLabels[83] = jd;
        rpi_camera_DW.cocoLabels[84] = kd;
        rpi_camera_DW.cocoLabels[85] = ld;
        rpi_camera_DW.cocoLabels[86] = md;
        rpi_camera_DW.cocoLabels[87] = nd;
        rpi_camera_DW.cocoLabels[88] = od;
        rpi_camera_DW.cocoLabels[89] = pd;
        rpi_camera_DW.cocoLabels[90] = qd;
      }

      for (b_k = 0; b_k < 320; b_k++) {
        for (i_p = 0; i_p < 320; i_p++) {
          displayImg_tmp = i_p * 3 + b_k * 960;
          displayImg_tmp_p = 320 * i_p + b_k;
          rpi_camera_B.displayImg[displayImg_tmp_p] =
            rpi_camera_B.CAMERA1[displayImg_tmp];
          rpi_camera_B.displayImg[displayImg_tmp_p + 102400] =
            rpi_camera_B.CAMERA1[displayImg_tmp + 1];
          rpi_camera_B.displayImg[displayImg_tmp_p + 204800] =
            rpi_camera_B.CAMERA1[displayImg_tmp + 2];
        }
      }

      for (b_k = 0; b_k < 320; b_k++) {
        for (i_p = 0; i_p < 3; i_p++) {
          std::memcpy(&rpi_camera_B.InsertText5[b_k * 320 + i_p * 102400],
                      &rpi_camera_B.displayImg[(b_k * 320 + i_p * -102400) +
                      204800], 320U * sizeof(uint8_T));
        }
      }

      rpi_camera_TFLiteModel_predict(&rpi_camera_DW.net,
        rpi_camera_B.InsertText5, boxes, labels, b_x, &a__1);
      if (!std::isnan(b_x[0])) {
        b_k = 0;
      } else {
        b_k = -1;
        i_p = 2;
        exitg1 = false;
        while ((!exitg1) && (i_p < 101)) {
          if (!std::isnan(b_x[i_p - 1])) {
            b_k = i_p - 1;
            exitg1 = true;
          } else {
            i_p++;
          }
        }
      }

      if (b_k + 1 == 0) {
        a__1 = b_x[0];
        iindx = 0;
      } else {
        a__1 = b_x[b_k];
        iindx = b_k;
        for (i_p = b_k + 2; i_p < 101; i_p++) {
          b_x_p = b_x[i_p - 1];
          if (a__1 < b_x_p) {
            a__1 = b_x_p;
            iindx = i_p - 1;
          }
        }
      }

      rpi_camera_B.scoreOut = a__1 * 100.0F;
      if (labels[iindx] + 1.0F >= 1.0F) {
        if (labels[iindx] + 1.0F <= 91.0F) {
          b_k = static_cast<int32_T>(labels[iindx] + 1.0F) - 1;
          displayImg_tmp_p = rpi_camera_DW.cocoLabels[b_k].f1.size[1];
          displayImg_tmp = rpi_camera_DW.cocoLabels[b_k].f1.size[1];
          for (i_p = 0; i_p < displayImg_tmp; i_p++) {
            name_data[i_p] = rpi_camera_DW.cocoLabels[b_k].f1.data[i_p];
          }
        } else {
          displayImg_tmp_p = 7;
          for (b_k = 0; b_k < 7; b_k++) {
            name_data[b_k] = yd[b_k];
          }
        }
      } else {
        displayImg_tmp_p = 7;
        for (b_k = 0; b_k < 7; b_k++) {
          name_data[b_k] = yd[b_k];
        }
      }

      for (i_p = 0; i_p < 20; i_p++) {
        rpi_camera_B.labelOut[i_p] = 0U;
      }

      for (b_k = 0; b_k < displayImg_tmp_p; b_k++) {
        rpi_camera_B.labelOut[b_k] = static_cast<uint8_T>(name_data[b_k]);
      }

      // S-Function (FPS_TRACKER): '<S1>/FPS1'
      struct timespec now;
      clock_gettime(CLOCK_MONOTONIC, &now);
      frameCounter++;
      double elapsed_sec = (now.tv_sec - fps_start_time.tv_sec) +
        (now.tv_nsec - fps_start_time.tv_nsec) / 1e9;
      if (elapsed_sec >= 1.0) {
        fps = frameCounter / elapsed_sec;
        frameCounter = 0;
        fps_start_time = now;
        rpi_camera_B.FPS1 = fps;
      }

      // S-Function (sviptextrender): '<S1>/Insert Text3'
      // put the pen at the beginning of the string
      b_k = 2;

      // S-Function (sviptextrender): '<S1>/Insert Text3'
      // copy input to output since Simulink isn't giving us in-place operation
      std::memcpy(&rpi_camera_B.img_out[0], &rpi_camera_B.displayImg[0], 307200U
                  * sizeof(uint8_T));

      // S-Function (sviptextrender): '<S1>/Insert Text3'
      while (b_k - 1 > 0) {
        // copy location to int32 container
        // update the color/intensity
        rpi_camera_DW.InsertText3_VarHolder[0] = rpi_camera_B.FPS1;
        MWVIP_snprintf_wrapper(&rpi_camera_DW.InsertText3_RepBuf[0U],
          &rpi_camera_DW.InsertText3_StringDataDW[0],
          &rpi_camera_DW.InsertText3_VarHolder[0U], b_k, 0, false, 161);
        displayImg_tmp_p = MWVIP_strlen(&rpi_camera_DW.InsertText3_RepBuf[0]);
        b_k--;

        // put the pen at the beginning of the string
        qY_p = 1;
        qY = 1;
        for (l_p = 0; l_p < displayImg_tmp_p; l_p++) {
          if (rpi_camera_DW.InsertText3_RepBuf[l_p] == 10) {
            qY += 15;
            qY_p = 1;
          } else {
            theChar = rpi_camera_DW.InsertText3_RepBuf[l_p];
            kb_p = static_cast<int32_T>
              (rpi_camera_ConstP.InsertText3_GlyIndexes[theChar]);
            if ((rpi_camera_ConstP.InsertText3_GlyWidths[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText3_GlyHeights[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText3_GlyXAdvances[theChar] == 0)) {
              theChar = 32;
              kb_p = 0;
            }

            qY_e = rpi_camera_ConstP.InsertText3_GlyWidths[theChar];
            bb_p = rpi_camera_ConstP.InsertText3_GlyHeights[theChar];
            o_p = (qY_p + rpi_camera_ConstP.InsertText3_GlyLBearings[theChar]) -
              1;
            d_p = (qY - rpi_camera_ConstP.InsertText3_GlyTBearings[theChar]) +
              11;
            pixelIndexCpy = (o_p * 320 + d_p) - 1;
            for (i_p = 0; i_p < qY_e; i_p++) {
              pixelIndex = pixelIndexCpy;
              for (j = 0; j < bb_p; j++) {
                pixelIndex++;
                if ((o_p >= -i_p) && (o_p < 320 - i_p) && (d_p >= -j) && (d_p <
                     320 - j)) {
                  y = rpi_camera_ConstP.InsertText3_GlyData[(j *
                    rpi_camera_ConstP.InsertText3_GlyWidths[theChar] + kb_p) +
                    i_p];
                  if (y != 0) {
                    if (y == 255) {
                      // just the text color
                      valR_idx_0 = MAX_uint8_T;
                      valR_idx_1 = 0U;
                      y = 0U;
                    } else {
                      // on the edge, need to figure out for nice anti-aliasing
                      valR_idx_0 = static_cast<uint8_T>(static_cast<uint32_T>
                        ((255 - y) * rpi_camera_B.img_out[pixelIndex] +
                         static_cast<int32_T>(y + 1U) * 255) >> 8);
                      valR_idx_1 = static_cast<uint8_T>(static_cast<uint32_T>
                        ((255 - y) * rpi_camera_B.img_out[pixelIndex + 102400]) >>
                        8);
                      y = static_cast<uint8_T>(static_cast<uint32_T>((255 - y) *
                        rpi_camera_B.img_out[pixelIndex + 204800]) >> 8);
                    }

                    rpi_camera_B.img_out[pixelIndex] = valR_idx_0;
                    rpi_camera_B.img_out[pixelIndex + 102400] = valR_idx_1;
                    rpi_camera_B.img_out[pixelIndex + 204800] = y;
                  }
                }
              }

              pixelIndexCpy += 320;
            }

            qY_p += rpi_camera_ConstP.InsertText3_GlyXAdvances[theChar];
          }
        }
      }

      // MATLAB Function: '<S1>/MATLAB Function' incorporates:
      //   MATLAB Function: '<S1>/MATLAB Function10'

      if (rpi_camera_B.scoreOut > 60.0F) {
        a__1 = std::round(boxes[iindx + 100] * 320.0F);
        if (a__1 < 2.14748365E+9F) {
          if (a__1 >= -2.14748365E+9F) {
            displayImg_tmp = static_cast<int32_T>(a__1);
          } else {
            displayImg_tmp = MIN_int32_T;
          }
        } else {
          displayImg_tmp = MAX_int32_T;
        }

        a__1 = std::round(boxes[iindx] * 320.0F);
        if (a__1 < 2.14748365E+9F) {
          if (a__1 >= -2.14748365E+9F) {
            displayImg_tmp_p = static_cast<int32_T>(a__1);
          } else {
            displayImg_tmp_p = MIN_int32_T;
          }
        } else {
          displayImg_tmp_p = MAX_int32_T;
        }

        a__1 = std::round(boxes[iindx + 300] * 320.0F);
        if (a__1 < 2.14748365E+9F) {
          if (a__1 >= -2.14748365E+9F) {
            b_k = static_cast<int32_T>(a__1);
          } else {
            b_k = MIN_int32_T;
          }
        } else {
          b_k = MAX_int32_T;
        }

        if ((b_k >= 0) && (displayImg_tmp < b_k - MAX_int32_T)) {
          qY_p = MAX_int32_T;
        } else if ((b_k < 0) && (displayImg_tmp > b_k - MIN_int32_T)) {
          qY_p = MIN_int32_T;
        } else {
          qY_p = b_k - displayImg_tmp;
        }

        a__1 = std::round(boxes[iindx + 200] * 320.0F);
        if (a__1 < 2.14748365E+9F) {
          if (a__1 >= -2.14748365E+9F) {
            b_k = static_cast<int32_T>(a__1);
          } else {
            b_k = MIN_int32_T;
          }
        } else {
          b_k = MAX_int32_T;
        }

        if ((b_k >= 0) && (displayImg_tmp_p < b_k - MAX_int32_T)) {
          qY = MAX_int32_T;
        } else if ((b_k < 0) && (displayImg_tmp_p > b_k - MIN_int32_T)) {
          qY = MIN_int32_T;
        } else {
          qY = b_k - displayImg_tmp_p;
        }

        rpi_camera_B.xy[0] = displayImg_tmp;
        if (displayImg_tmp_p < -2147483628) {
          rpi_camera_B.xy[1] = MIN_int32_T;
        } else {
          rpi_camera_B.xy[1] = displayImg_tmp_p - 20;
        }

        tmp_e = ((displayImg_tmp_p < 0) && (qY < MIN_int32_T - displayImg_tmp_p));
        if (tmp_e) {
          b_k = MIN_int32_T;
        } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                    - displayImg_tmp_p)) {
          b_k = MAX_int32_T;
        } else {
          b_k = displayImg_tmp_p + qY;
        }

        tmp_p = ((displayImg_tmp < 0) && (qY_p < MIN_int32_T - displayImg_tmp));
        if (tmp_p) {
          i_p = MIN_int32_T;
        } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T - displayImg_tmp))
        {
          i_p = MAX_int32_T;
        } else {
          i_p = displayImg_tmp + qY_p;
        }

        for (l_p = 0; l_p < 2; l_p++) {
          tmp = static_cast<real_T>(displayImg_tmp_p) + static_cast<real_T>(l_p);
          if (tmp < 2.147483648E+9) {
            iindx = static_cast<int32_T>(tmp);
          } else {
            iindx = MAX_int32_T;
          }

          tmp = static_cast<real_T>(b_k) - static_cast<real_T>(l_p);
          if (tmp >= -2.147483648E+9) {
            bb_p = static_cast<int32_T>(tmp);
          } else {
            bb_p = MIN_int32_T;
          }

          tmp = static_cast<real_T>(displayImg_tmp) + static_cast<real_T>(l_p);
          if (tmp < 2.147483648E+9) {
            kb_p = static_cast<int32_T>(tmp);
          } else {
            kb_p = MAX_int32_T;
          }

          tmp = static_cast<real_T>(i_p) - static_cast<real_T>(l_p);
          if (tmp >= -2.147483648E+9) {
            theChar = static_cast<int32_T>(tmp);
          } else {
            theChar = MIN_int32_T;
          }

          if (iindx <= 320) {
            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              d_p = 0;
              qY_e = 0;
            } else {
              d_p = displayImg_tmp - 1;
            }

            qY_e -= d_p;
            for (o_p = 0; o_p < qY_e; o_p++) {
              rpi_camera_B.img_out[(iindx + 320 * (d_p + o_p)) - 1] =
                MAX_uint8_T;
            }

            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              d_p = 0;
              qY_e = 0;
            } else {
              d_p = displayImg_tmp - 1;
            }

            qY_e -= d_p;
            for (o_p = 0; o_p < qY_e; o_p++) {
              rpi_camera_B.img_out[(iindx + 320 * (d_p + o_p)) + 102399] = 0U;
            }

            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              d_p = 0;
              qY_e = 0;
            } else {
              d_p = displayImg_tmp - 1;
            }

            qY_e -= d_p;
            for (o_p = 0; o_p < qY_e; o_p++) {
              rpi_camera_B.img_out[(iindx + 320 * (d_p + o_p)) + 204799] = 0U;
            }
          }

          if (bb_p <= 320) {
            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              o_p = 0;
              qY_e = 0;
            } else {
              o_p = displayImg_tmp - 1;
            }

            qY_e -= o_p;
            for (iindx = 0; iindx < qY_e; iindx++) {
              rpi_camera_B.img_out[(bb_p + 320 * (o_p + iindx)) - 1] =
                MAX_uint8_T;
            }

            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              o_p = 0;
              qY_e = 0;
            } else {
              o_p = displayImg_tmp - 1;
            }

            qY_e -= o_p;
            for (iindx = 0; iindx < qY_e; iindx++) {
              rpi_camera_B.img_out[(bb_p + 320 * (o_p + iindx)) + 102399] = 0U;
            }

            if (tmp_p) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp > 0) && (qY_p > MAX_int32_T
                        - displayImg_tmp)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp + qY_p;
            }

            if (displayImg_tmp > qY_e) {
              o_p = 0;
              qY_e = 0;
            } else {
              o_p = displayImg_tmp - 1;
            }

            qY_e -= o_p;
            for (iindx = 0; iindx < qY_e; iindx++) {
              rpi_camera_B.img_out[(bb_p + 320 * (o_p + iindx)) + 204799] = 0U;
            }
          }

          if (kb_p <= 320) {
            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              bb_p = 0;
              qY_e = 0;
            } else {
              bb_p = displayImg_tmp_p - 1;
            }

            qY_e -= bb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(bb_p + kb_p * 320) + -320], 255,
                          static_cast<uint32_T>((((qY_e + bb_p) + kb_p * 320) -
                bb_p) - kb_p * 320) * sizeof(uint8_T));
            }

            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              bb_p = 0;
              qY_e = 0;
            } else {
              bb_p = displayImg_tmp_p - 1;
            }

            qY_e -= bb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(bb_p + kb_p * 320) + 102080], 0,
                          static_cast<uint32_T>((((qY_e + bb_p) + kb_p * 320) -
                bb_p) - kb_p * 320) * sizeof(uint8_T));
            }

            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              bb_p = 0;
              qY_e = 0;
            } else {
              bb_p = displayImg_tmp_p - 1;
            }

            qY_e -= bb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(bb_p + kb_p * 320) + 204480], 0,
                          static_cast<uint32_T>((((qY_e + bb_p) + kb_p * 320) -
                bb_p) - kb_p * 320) * sizeof(uint8_T));
            }
          }

          if (theChar <= 320) {
            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              kb_p = 0;
              qY_e = 0;
            } else {
              kb_p = displayImg_tmp_p - 1;
            }

            qY_e -= kb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(kb_p + theChar * 320) + -320],
                          255, static_cast<uint32_T>((((qY_e + kb_p) + theChar *
                320) - kb_p) - theChar * 320) * sizeof(uint8_T));
            }

            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              kb_p = 0;
              qY_e = 0;
            } else {
              kb_p = displayImg_tmp_p - 1;
            }

            qY_e -= kb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(kb_p + theChar * 320) + 102080],
                          0, static_cast<uint32_T>((((qY_e + kb_p) + theChar *
                320) - kb_p) - theChar * 320) * sizeof(uint8_T));
            }

            if (tmp_e) {
              qY_e = MIN_int32_T;
            } else if ((displayImg_tmp_p > 0) && (qY > MAX_int32_T
                        - displayImg_tmp_p)) {
              qY_e = MAX_int32_T;
            } else {
              qY_e = displayImg_tmp_p + qY;
            }

            if (displayImg_tmp_p > qY_e) {
              kb_p = 0;
              qY_e = 0;
            } else {
              kb_p = displayImg_tmp_p - 1;
            }

            qY_e -= kb_p;
            if (qY_e - 1 >= 0) {
              std::memset(&rpi_camera_B.img_out[(kb_p + theChar * 320) + 204480],
                          0, static_cast<uint32_T>((((qY_e + kb_p) + theChar *
                320) - kb_p) - theChar * 320) * sizeof(uint8_T));
            }
          }
        }
      } else {
        rpi_camera_B.xy[0] = 1000;
        rpi_camera_B.xy[1] = 1000;
      }

      // End of MATLAB Function: '<S1>/MATLAB Function'

      // MATLAB Function: '<S1>/MATLAB Function9'
      if (rpi_camera_B.xy[0] > 2147483607) {
        rpi_camera_B.xyO[0] = MAX_int32_T;
      } else {
        rpi_camera_B.xyO[0] = rpi_camera_B.xy[0] + 40;
      }

      rpi_camera_B.xyO[1] = rpi_camera_B.xy[1];

      // End of MATLAB Function: '<S1>/MATLAB Function9'

      // S-Function (sviptextrender): '<S1>/Insert Text4'
      // put the pen at the beginning of the string
      b_k = 21;
      displayImg_tmp = 0;

      // S-Function (sviptextrender): '<S1>/Insert Text4'
      // copy input to output since Simulink isn't giving us in-place operation
      std::memcpy(&rpi_camera_B.InsertText4[0], &rpi_camera_B.img_out[0],
                  307200U * sizeof(uint8_T));

      // S-Function (sviptextrender): '<S1>/Insert Text4'
      while (b_k - 1 > 0) {
        // copy location to int32 container
        // update the color/intensity
        for (i_p = 0; i_p < 20; i_p++) {
          rpi_camera_DW.InsertText4_VarHolder[i_p] = rpi_camera_B.labelOut[i_p +
            displayImg_tmp];
        }

        MWVIP_snprintf_wrapper(&rpi_camera_DW.InsertText4_RepBuf[0U],
          &rpi_camera_DW.InsertText4_StringDataDW[0],
          &rpi_camera_DW.InsertText4_VarHolder[0U], b_k, 3, true, 25);
        i_p = MWVIP_strlen(&rpi_camera_B.labelOut[displayImg_tmp]) + 1;
        displayImg_tmp += i_p;
        b_k -= i_p;
        displayImg_tmp_p = MWVIP_strlen(&rpi_camera_DW.InsertText4_RepBuf[0]);

        // put the pen at the beginning of the string
        qY_p = rpi_camera_B.xyO[0];
        qY = rpi_camera_B.xyO[1];
        for (l_p = 0; l_p < displayImg_tmp_p; l_p++) {
          if (rpi_camera_DW.InsertText4_RepBuf[l_p] == 10) {
            qY += 15;
            qY_p = rpi_camera_B.xyO[0];
          } else {
            theChar = rpi_camera_DW.InsertText4_RepBuf[l_p];
            kb_p = static_cast<int32_T>
              (rpi_camera_ConstP.InsertText4_GlyIndexes[theChar]);
            if ((rpi_camera_ConstP.InsertText4_GlyWidths[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText4_GlyHeights[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText4_GlyXAdvances[theChar] == 0)) {
              theChar = 32;
              kb_p = 0;
            }

            qY_e = rpi_camera_ConstP.InsertText4_GlyWidths[theChar];
            bb_p = rpi_camera_ConstP.InsertText4_GlyHeights[theChar];
            o_p = (qY_p + rpi_camera_ConstP.InsertText4_GlyLBearings[theChar]) -
              1;
            d_p = (qY - rpi_camera_ConstP.InsertText4_GlyTBearings[theChar]) +
              11;
            pixelIndexCpy = (o_p * 320 + d_p) - 1;
            for (i_p = 0; i_p < qY_e; i_p++) {
              pixelIndex = pixelIndexCpy;
              for (j = 0; j < bb_p; j++) {
                pixelIndex++;
                if ((o_p >= -i_p) && (o_p < 320 - i_p) && (d_p >= -j) && (d_p <
                     320 - j) && (rpi_camera_ConstP.InsertText4_GlyData[(j *
                      rpi_camera_ConstP.InsertText4_GlyWidths[theChar] + kb_p) +
                                  i_p] != 0)) {
                  rpi_camera_B.InsertText4[pixelIndex] = MAX_uint8_T;
                  rpi_camera_B.InsertText4[pixelIndex + 102400] = MAX_uint8_T;
                  rpi_camera_B.InsertText4[pixelIndex + 204800] = 0U;
                }
              }

              pixelIndexCpy += 320;
            }

            qY_p += rpi_camera_ConstP.InsertText4_GlyXAdvances[theChar];
          }
        }
      }

      // S-Function (sviptextrender): '<S1>/Insert Text5'
      // put the pen at the beginning of the string
      b_k = 2;

      // S-Function (sviptextrender): '<S1>/Insert Text5' incorporates:
      //   S-Function (sviptextrender): '<S1>/Insert Text4'

      // copy input to output since Simulink isn't giving us in-place operation
      std::memcpy(&rpi_camera_B.InsertText5[0], &rpi_camera_B.InsertText4[0],
                  307200U * sizeof(uint8_T));

      // S-Function (sviptextrender): '<S1>/Insert Text5'
      while (b_k - 1 > 0) {
        // copy location to int32 container
        // update the color/intensity
        rpi_camera_DW.InsertText5_VarHolder[0] = rpi_camera_B.scoreOut;
        MWVIP_snprintf_wrapper(&rpi_camera_DW.InsertText5_RepBuf[0U],
          &rpi_camera_DW.InsertText5_StringDataDW[0],
          &rpi_camera_DW.InsertText5_VarHolder[0U], b_k, 1, false, 161);
        displayImg_tmp_p = MWVIP_strlen(&rpi_camera_DW.InsertText5_RepBuf[0]);
        b_k--;

        // put the pen at the beginning of the string
        qY_p = rpi_camera_B.xy[0];
        qY = rpi_camera_B.xy[1];
        for (l_p = 0; l_p < displayImg_tmp_p; l_p++) {
          if (rpi_camera_DW.InsertText5_RepBuf[l_p] == 10) {
            qY += 15;
            qY_p = rpi_camera_B.xy[0];
          } else {
            theChar = rpi_camera_DW.InsertText5_RepBuf[l_p];
            kb_p = static_cast<int32_T>
              (rpi_camera_ConstP.InsertText5_GlyIndexes[theChar]);
            if ((rpi_camera_ConstP.InsertText5_GlyWidths[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText5_GlyHeights[theChar] == 0) ||
                (rpi_camera_ConstP.InsertText5_GlyXAdvances[theChar] == 0)) {
              theChar = 32;
              kb_p = 0;
            }

            qY_e = rpi_camera_ConstP.InsertText5_GlyWidths[theChar];
            bb_p = rpi_camera_ConstP.InsertText5_GlyHeights[theChar];
            o_p = (qY_p + rpi_camera_ConstP.InsertText5_GlyLBearings[theChar]) -
              1;
            d_p = (qY - rpi_camera_ConstP.InsertText5_GlyTBearings[theChar]) +
              11;
            pixelIndexCpy = (o_p * 320 + d_p) - 1;
            for (i_p = 0; i_p < qY_e; i_p++) {
              pixelIndex = pixelIndexCpy;
              for (j = 0; j < bb_p; j++) {
                pixelIndex++;
                if ((o_p >= -i_p) && (o_p < 320 - i_p) && (d_p >= -j) && (d_p <
                     320 - j)) {
                  y = rpi_camera_ConstP.InsertText5_GlyData[(j *
                    rpi_camera_ConstP.InsertText5_GlyWidths[theChar] + kb_p) +
                    i_p];
                  if (y != 0) {
                    if (y == 255) {
                      // just the text color
                      valR_idx_0 = MAX_uint8_T;
                      valR_idx_1 = 0U;
                      y = MAX_uint8_T;
                    } else {
                      // on the edge, need to figure out for nice anti-aliasing
                      displayImg_tmp = static_cast<int32_T>(y + 1U) * 255;
                      valR_idx_0 = static_cast<uint8_T>(static_cast<uint32_T>
                        ((255 - y) * rpi_camera_B.InsertText5[pixelIndex] +
                         displayImg_tmp) >> 8);
                      valR_idx_1 = static_cast<uint8_T>(static_cast<uint32_T>
                        ((255 - y) * rpi_camera_B.InsertText5[pixelIndex +
                         102400]) >> 8);
                      y = static_cast<uint8_T>(static_cast<uint32_T>((255 - y) *
                        rpi_camera_B.InsertText5[pixelIndex + 204800] +
                        displayImg_tmp) >> 8);
                    }

                    rpi_camera_B.InsertText5[pixelIndex] = valR_idx_0;
                    rpi_camera_B.InsertText5[pixelIndex + 102400] = valR_idx_1;
                    rpi_camera_B.InsertText5[pixelIndex + 204800] = y;
                  }
                }
              }

              pixelIndexCpy += 320;
            }

            qY_p += rpi_camera_ConstP.InsertText5_GlyXAdvances[theChar];
          }
        }
      }

      // MATLAB Function: '<S1>/MATLAB Function7' incorporates:
      //   S-Function (sviptextrender): '<S1>/Insert Text5'

      std::memcpy(&rpi_camera_B.r[0], &rpi_camera_B.InsertText5[0], 102400U *
                  sizeof(uint8_T));
      std::memcpy(&rpi_camera_B.g[0], &rpi_camera_B.InsertText5[102400], 102400U
                  * sizeof(uint8_T));
      std::memcpy(&rpi_camera_B.b[0], &rpi_camera_B.InsertText5[204800], 102400U
                  * sizeof(uint8_T));

      // S-Function (DISPLAY): '<S1>/DISPLAY'
      StreamFrame((const uint8_t *)&rpi_camera_B.r[0], (const uint8_t *)
                  &rpi_camera_B.g[0], (const uint8_t *)&rpi_camera_B.b[0]);

      // UnitDelay: '<S1>/Unit Delay'
      rpi_camera_B.UnitDelay = rpi_camera_DW.UnitDelay_DSTATE;

      // S-Function (PIGPIO_Write): '<S1>/PIGPIO_Write'
      if (req_17) {
        int val = (rpi_camera_B.UnitDelay != 0) ? 1 : 0;
        gpiod_line_request_set_value(req_17, 17, val);
      }

      // Update for UnitDelay: '<S1>/Unit Delay' incorporates:
      //   MATLAB Function: '<S1>/MATLAB Function10'

      rpi_camera_DW.UnitDelay_DSTATE = (rpi_camera_B.scoreOut > 60.0F);
    }

    // calculate next shot: t += interval; normalize
    next_wakeup_time.tv_nsec += interval.tv_nsec;
    next_wakeup_time.tv_sec += interval.tv_sec;
    if (next_wakeup_time.tv_nsec >= NSEC_PER_SEC) {
      next_wakeup_time.tv_nsec -= NSEC_PER_SEC;
      next_wakeup_time.tv_sec += 1;
    }
  }
}

// Function for MATLAB Function: '<S1>/MATLAB Function10'
static void rpi_camera_permute(const real32_T a[400], real32_T b[400])
{
  for (int32_T k{0}; k < 100; k++) {
    int32_T b_tmp;
    b_tmp = k << 2;
    b[k] = a[b_tmp];
    b[k + 100] = a[b_tmp + 1];
    b[k + 200] = a[b_tmp + 2];
    b[k + 300] = a[b_tmp + 3];
  }
}

// Function for MATLAB Function: '<S1>/MATLAB Function10'
static void rpi_camera_shiftdim(const real32_T x[400], real32_T b[400])
{
  std::memcpy(&b[0], &x[0], 400U * sizeof(real32_T));
}

// Function for MATLAB Function: '<S1>/MATLAB Function10'
static void rpi_camera_shiftdim_l(const real32_T x[100], real32_T b[100])
{
  std::memcpy(&b[0], &x[0], 100U * sizeof(real32_T));
}

// Function for MATLAB Function: '<S1>/MATLAB Function10'
static void rpi_camera_TFLiteModel_predict(coder_TFLiteModel_rpi_camera_T
  *b_this, const uint8_T varargin_1[307200], real32_T varargout_1[400], real32_T
  varargout_2[100], real32_T varargout_3[100], real32_T *varargout_4)
{
  real_T count;
  int32_T height;
  int32_T preprocessedInp_tmp;
  int32_T width;
  real32_T tmp[400];
  real32_T outputScaleVector[4];
  real32_T outputZeroPointVector[4];
  real32_T inputScaleVector;
  real32_T inputZeroPointVector;
  inputScaleVector = 1.0F;
  inputZeroPointVector = 0.0F;
  outputScaleVector[0] = 1.0F;
  outputZeroPointVector[0] = 0.0F;
  outputScaleVector[1] = 1.0F;
  outputZeroPointVector[1] = 0.0F;
  outputScaleVector[2] = 1.0F;
  outputZeroPointVector[2] = 0.0F;
  outputScaleVector[3] = 1.0F;
  outputZeroPointVector[3] = 0.0F;
  std::mem_fn(&invokeinterpreter::setVerbose)(b_this->Network, b_this->Verbose);
  std::mem_fn(&invokeinterpreter::setProfiling)(b_this->Network,
    b_this->EnableProfiling);
  std::mem_fn(&invokeinterpreter::setNumThreads)(b_this->Network,
    b_this->InternalNumThreads);
  std::mem_fn(&invokeinterpreter::setInputMean)(b_this->Network, b_this->Mean);
  std::mem_fn(&invokeinterpreter::setInputStdDeviation)(b_this->Network,
    b_this->StandardDeviation);
  std::mem_fn(&invokeinterpreter::setInputScale)(b_this->Network,
    &inputScaleVector);
  std::mem_fn(&invokeinterpreter::setInputZeroPoint)(b_this->Network,
    &inputZeroPointVector);
  std::mem_fn(&invokeinterpreter::setOutputScale)(b_this->Network,
    &outputScaleVector[0]);
  std::mem_fn(&invokeinterpreter::setOutputZeroPoint)(b_this->Network,
    &outputZeroPointVector[0]);
  std::mem_fn(&invokeinterpreter::setSkipNormalization)(b_this->Network, false);
  std::mem_fn(&invokeinterpreter::setSkipInputQuantization)(b_this->Network,
    true);
  std::mem_fn(&invokeinterpreter::setSkipOutputDequantization)(b_this->Network,
    true);
  count = 1.0;
  for (height = 0; height < 320; height++) {
    for (width = 0; width < 320; width++) {
      preprocessedInp_tmp = 320 * width + height;
      rpi_camera_B.preprocessedInp[static_cast<int32_T>(count) - 1] =
        varargin_1[preprocessedInp_tmp];
      rpi_camera_B.preprocessedInp[static_cast<int32_T>(count + 1.0) - 1] =
        varargin_1[preprocessedInp_tmp + 102400];
      rpi_camera_B.preprocessedInp[static_cast<int32_T>(count + 2.0) - 1] =
        varargin_1[preprocessedInp_tmp + 204800];
      count += 3.0;
    }
  }

  std::mem_fn(&invokeinterpreter::predict)(b_this->Network,
    &rpi_camera_B.preprocessedInp[0], 307200, &b_this->PredictedTotalOutput[0]);
  rpi_camera_permute(&b_this->PredictedTotalOutput[0], tmp);
  rpi_camera_shiftdim(tmp, varargout_1);
  rpi_camera_shiftdim_l(&b_this->PredictedTotalOutput[400], varargout_2);
  rpi_camera_shiftdim_l(&b_this->PredictedTotalOutput[500], varargout_3);
  *varargout_4 = b_this->PredictedTotalOutput[600];
}

// Model step function
void rpi_camera_step(void)
{
  // (no output/update code required)
}

// Model initialize function
void rpi_camera_initialize(void)
{
  // SystemInitialize for S-Function (POSIX_Thread): '<Root>/POSIX_Thread' incorporates:
  //   SubSystem: '<Root>/Function-Call Subsystem'


  // System initialize for function-call system: '<Root>/Function-Call Subsystem' 
  {
    int32_T i;

    // Start for S-Function (JITTER): '<S1>/JITTER'
    {
      jitter_latency_buffer = (double *)malloc(jitter_max_samples * sizeof
        (double));
    }

    // Start for S-Function (CAMERA): '<S1>/CAMERA1'
    CameraConfig cfg;
    cfg.deviceIndex = 0;
    cfg.cameraId = "";                 // use deviceIndex
    cfg.width = 320;
    cfg.height = 320;
    cfg.pixelFormat = "RGB888";
    (void)camera_wrapper_init(&cfg);
    usleep(1000000);                   // warm-up

    // Start for S-Function (sviptextrender): '<S1>/Insert Text3'
    for (i = 0; i < 10; i++) {
      rpi_camera_DW.InsertText3_StringDataDW[i] =
        rpi_camera_ConstP.InsertText3_StrArray[i];
    }

    // End of Start for S-Function (sviptextrender): '<S1>/Insert Text3'

    // Start for S-Function (sviptextrender): '<S1>/Insert Text4'
    rpi_camera_DW.InsertText4_StringDataDW[0] = 37U;
    rpi_camera_DW.InsertText4_StringDataDW[1] = 115U;
    rpi_camera_DW.InsertText4_StringDataDW[2] = 0U;

    // Start for S-Function (sviptextrender): '<S1>/Insert Text5'
    for (i = 0; i < 5; i++) {
      rpi_camera_DW.InsertText5_StringDataDW[i] =
        rpi_camera_ConstP.InsertText5_StrArray[i];
    }

    // End of Start for S-Function (sviptextrender): '<S1>/Insert Text5'

    // Start for S-Function (DISPLAY): '<S1>/DISPLAY'

    // CALL INIT HERE (Before the loop starts!)
    InitFB();

    // Start for S-Function (PIGPIO_Write): '<S1>/PIGPIO_Write'
    if (!chip_17) {
      const char *chip_path = "/dev/gpiochip0";
      unsigned int offset = 17;
      struct gpiod_line_settings *settings;
      struct gpiod_line_config *line_cfg;
      struct gpiod_request_config *req_cfg;
      chip_17 = gpiod_chip_open(chip_path);
      if (!chip_17) {
        printf("[Error] Failed to open %s for Pin %d\n", chip_path, offset);
      } else {
        settings = gpiod_line_settings_new();
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
        line_cfg = gpiod_line_config_new();
        gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
        req_cfg = gpiod_request_config_new();
        char consumer_name[32];
        snprintf(consumer_name, sizeof(consumer_name), "simulink_pin_%d", offset);
        gpiod_request_config_set_consumer(req_cfg, consumer_name);
        req_17 = gpiod_chip_request_lines(chip_17, req_cfg, line_cfg);
        if (!req_17) {
          printf("[Error] Failed to request line for Pin %d\n", offset);
        }

        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        gpiod_request_config_free(req_cfg);
      }
    }

    // SystemInitialize for MATLAB Function: '<S1>/MATLAB Function10'
    rpi_camera_DW.net.matlabCodegenIsDeleted = true;
  }

  mlockall(MCL_CURRENT | MCL_FUTURE);
  pthread_attr_t Camera_attr;
  struct sched_param Camera_sp;
  int Camera_policy = SCHED_FIFO;
  pthread_attr_init(&Camera_attr);
  pthread_attr_setinheritsched(&Camera_attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&Camera_attr, Camera_policy);
  memset(&Camera_sp, 0, sizeof(Camera_sp));
  Camera_sp.sched_priority = (int)99.0;
  pthread_attr_setschedparam(&Camera_attr, &Camera_sp);
  pthread_create(&Camera_t, &Camera_attr, Camera, NULL);
  pthread_attr_destroy(&Camera_attr);
  cpu_set_t Camera_set;
  CPU_ZERO(&Camera_set);
  CPU_SET(1, &Camera_set);
  CPU_SET(2, &Camera_set);
  CPU_SET(3, &Camera_set);
  pthread_setaffinity_np(Camera_t, sizeof(Camera_set), &Camera_set);

  // End of SystemInitialize for S-Function (POSIX_Thread): '<Root>/POSIX_Thread' 
}

// Model terminate function
void rpi_camera_terminate(void)
{
  // Terminate for S-Function (POSIX_Thread): '<Root>/POSIX_Thread'
  Camera_running = 0;

  //pthread_join(Camera_t, NULL);

  // End of Terminate for S-Function (POSIX_Thread): '<Root>/POSIX_Thread'

  // Terminate for S-Function (POSIX_Thread): '<Root>/POSIX_Thread' incorporates:
  //   SubSystem: '<Root>/Function-Call Subsystem'


  // Termination for function-call system: '<Root>/Function-Call Subsystem'

  // Terminate for S-Function (JITTER): '<S1>/JITTER'
  if (jitter_latency_buffer != NULL) {
    FILE *f = fopen("jitter_cyclictest.csv", "w");
    if (f) {
      printf("\nRT_LATENCY: Saving %d samples to jitter_cyclictest.csv...\n",
             jitter_sample_count);
      fprintf(f, "Step,Latency_US\n");
      for (int i = 0; i < jitter_sample_count; i++) {
        fprintf(f, "%d,%.3f\n", i, jitter_latency_buffer[i]);
      }

      fclose(f);
      printf("RT_LATENCY: Save Complete.\n");
    }

    free(jitter_latency_buffer);
    jitter_latency_buffer = NULL;
  }

  // Terminate for S-Function (CAMERA): '<S1>/CAMERA1'
  camera_wrapper_stop();

  // Terminate for MATLAB Function: '<S1>/MATLAB Function10'
  if (!rpi_camera_DW.net.matlabCodegenIsDeleted) {
    rpi_camera_DW.net.matlabCodegenIsDeleted = true;
    delete(rpi_camera_DW.net.Network);
  }

  // End of Terminate for MATLAB Function: '<S1>/MATLAB Function10'
  // Terminate for S-Function (DISPLAY): '<S1>/DISPLAY'
  CleanupFB();

  // Terminate for S-Function (PIGPIO_Write): '<S1>/PIGPIO_Write'
  if (req_17) {
    gpiod_line_request_release(req_17);
    req_17 = NULL;
  }

  if (chip_17) {
    gpiod_chip_close(chip_17);
    chip_17 = NULL;
  }

  // End of Terminate for S-Function (POSIX_Thread): '<Root>/POSIX_Thread'
}

const char_T* RT_MODEL_rpi_camera_T::getErrorStatus() const
{
  return (errorStatus);
}

void RT_MODEL_rpi_camera_T::setErrorStatus(const char_T* const volatile
  aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//
