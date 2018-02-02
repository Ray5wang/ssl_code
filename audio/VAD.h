
#ifndef VAD_H
#define VAD_H
#include "../dsplib/fft.h"
#include <stdio.h>
#include "../main.h"
struct my_complex{
    float real[win_size+1];
    float Imag[win_size+1];
};

void point_multi_vad(struct my_complex  data1,struct my_complex  data2,float *result ,int N);
void vad_do_once(float *audio);
void vad_init(float *audio);

       
#endif
