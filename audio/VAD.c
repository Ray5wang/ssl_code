#include "VAD.h"
#include "wave.h"
#include "../dsplib/window.h"
#include "../dsplib/fft.h"
#include "../main.h"
#include "../dsplib/process.h"
#define DEBUG_VAD 1

//参数设置
float a01 = 0.5;
float a10 = 0.1;
float a00 =0.5;   // 1 - a01;
float a11 =0.9;   //1 - a10;

float threshold = 0.8; // Decision threshold of the likelihood ratio test
float win_dur = 0.256; // Window duration in seconds
float hop_dur = 0.128; //Hop duration in seconds
float num_noise = 1;   //Number of noise frames at the beginning of file
float argin = 1;       // Noise estimation (1: enable, 0: disable)

unsigned int win_size;
unsigned int hop_size;
unsigned int N_FFT;
unsigned int num_frames;

char *vad_results;
float *noise_var_temp;
float *G_MMSE;
float *A_MMSE;
float *hamming_win;
float *audio_frame;
int G_old;

//float 0.99;
struct my_complex audio_frame_fft;

/*******************************
*初始化VAD算法
*输入：音频数组
*输出: 无
*******************************/
void vad_init(float *audio){
    int i;
    unsigned int j;
    G_old = 1;
	win_size = FS * win_dur;                                     
	hop_size = FS * hop_dur;
	N_FFT = win_size;

	//hamming_win = hamming(win_size);生成汉明窗
    hamming_win = (float*) malloc(sizeof(float) * win_size);
    generateHanningWindow(hamming_win, win_size);
    
    audio_frame = (float*)malloc(sizeof(float)* win_size); //matlab中没有初始化这个，需要定义一个这个数组
    num_frames = (wav_length-win_size)/hop_size; //需要向下取整，还没处理
    vad_results = (char*) malloc(sizeof(char) * num_frames); //结果数组
    noise_var_temp = (float*) malloc(sizeof(float) * N_FFT);//噪声变量数组


    for (i=0;i<num_noise;i++){
       for(j=0;j<win_size;j++){
           audio_frame[j] = hamming_win[j]*audio[j];
       } 
    fftComputeOnce(myFFT,audio_frame ,audio_frame_fft.real, audio_frame_fft.Imag);
    point_multi_vad(audio_frame_fft,audio_frame_fft,noise_var_temp,win_size);

    A_MMSE = (float*) malloc(sizeof(float) * N_FFT);
    G_MMSE = (float*) malloc(sizeof(float) * N_FFT);
    }
}
/*******************************
*做一次VAD计算
*输入：
*输出:VAD表格vad_result
*******************************/
void vad_do_once(){
    unsigned int i;
	for(i=0;i<num_frames;i++){
        
    }
}
 /*******************************
  * * a * conj(b)
  * *输入：a b
  * *输出:result
  * *******************************/
void point_multi_vad(struct my_complex  data1,struct my_complex  data2, float *result ,int N){
    int i;
    for(i=0;i<N;i++){
    result[i] =  data1.real[i] * data2.real[i] + data1.Imag[i] * data2.Imag[i];
    result[i] = - data1.real[i] * data2.Imag[i] + data1.Imag[i] * data2.real[i];
    }
}


