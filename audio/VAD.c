#include "VAD.h"
#include "wave.h"
#include "../dsplib/window.h"
#include "../dsplib/fft.h"
#include "../main.h"
#include "../dsplib/process.h"
#define DEBUG_VAD 1


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
float *noise_var;
float *noise_var_orig;
float *noise_var_old;

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
	hop_size = FS * hop_dur;
	N_FFT = win_size;

    struct objFFT* vad_FFT;          //fft结构体
    vad_FFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(vad_FFT , N_FFT);

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
          // printf("%d haming_win is %f \n",j,hamming_win[j]);
       } 
    fftComputeOnce(vad_FFT,audio_frame ,audio_frame_fft.real, audio_frame_fft.Imag);
    point_multi_vad(audio_frame_fft,audio_frame_fft,noise_var_temp,win_size);
    }
    for(i=0;i<(int)N_FFT;i++){
    noise_var_orig[i] = noise_var_temp[i] / num_noise; 
    }
    noise_var_old = noise_var_orig;                      
    G_old = 1;
    A_MMSE = (float*) malloc(sizeof(float) * N_FFT);
    G_MMSE = (float*) malloc(sizeof(float) * N_FFT);
}
/*******************************
*做一次VAD计算
*输入：
*输出:VAD表格vad_result
*******************************/
void vad_do_once(float *audio ){
    int dur_start,dur_stop;
    unsigned int i,j;
    int dur;
	for(i=0;i<num_frames;i++){
    //dur = ((i-1)*hop_size+1):((i-1)*hop_size+win_size);//是上面计算噪声时候分的一段数据的一段
    dur_start = i*hop_size;
    dur_stop  = i*hop_size+win_size;
    dur = dur_start;
    j = 0;
    while(dur != dur_stop)
    audio_frame[j] = hamming_win[j] * audio[dur];              //加窗
    // audio_frame_fft = fft(audio_frame,N_FFT)';              //做fft
    point_multi_vad(audio_frame_fft,audio_frame_fft,noise_var_temp,win_size);

    noise_var = noise_var_orig;  
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
    printf("%dfft result : %f+%f\n",i,data1.real[i],data1.Imag[i]);
    }
}

