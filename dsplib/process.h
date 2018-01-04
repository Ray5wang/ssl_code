#ifndef PROCESS_H
#define PROCESS_H
#include "fft.h"
#include <stdio.h>
#include "../main.h"

struct mic_Array{
    float loc_x;
    float loc_y;
    float loc_z;
    float mic_signal[FrameSize];
    float mic_real[FrameSize];
    float mic_Imag[FrameSize];
};

void showfft(float *real ,float *imag);
void caculate_gccphat(struct mic_Array *mic , float result[MIC_PAIR][FrameSize]);
void srpphat( float R[MIC_PAIR][FrameSize] ,float *result);
void point_multi(struct mic_Array data1,struct mic_Array data2,struct mic_Array *result,int N);
int  data_read(int file);
void process_datafft(float *real ,float *imag);
void do_once_srp();
#endif
