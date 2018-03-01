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
float *my_gamma;
float *Lambda;
float *Y_mag;
int G_old;
float Lambda_mean;
float weight;
float diff;

float *noise_var;
float *noise_var_orig;
float *noise_var_old;
float *noise_var_prev;

float *frame_var;
float *audio_frame;
float *xi;
float *v;
//
float *hamming_win;
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
    noise_var_orig = (float*) malloc(sizeof(float) * N_FFT);//噪声变量数组
    noise_var_temp = (float*) malloc(sizeof(float) * N_FFT);//噪声变量数组


    for (i=0;i<num_noise;i++){
       for(j=i*hop_size;j<i*hop_size+win_size;j++){
           audio_frame[j] = hamming_win[j]*audio[j];
       } 
    fftComputeOnce(vad_FFT,audio_frame ,audio_frame_fft.real, audio_frame_fft.Imag);
    for(j=0;j<N_FFT;j++){
    }
    //和自己的共轭相乘
    point_multi_vad(audio_frame_fft,audio_frame_fft,noise_var_temp,win_size);
    }

    for(i=0;i<(int)N_FFT;i++){
    noise_var_orig[i] = noise_var_temp[i] / num_noise; 
    }

    noise_var_old = noise_var_orig;                      
    G_old = 1;

    //初始化一些变量
    A_MMSE = (float*) malloc(sizeof(float) * N_FFT);
    G_MMSE = (float*) malloc(sizeof(float) * N_FFT);

    my_gamma = (float*) malloc(sizeof(float) * N_FFT);
    xi = (float*) malloc(sizeof(float) * N_FFT);
    v = (float*) malloc(sizeof(float) * N_FFT);
    Lambda = (float*) malloc(sizeof(float) * N_FFT);
}
/*******************************
*做一次VAD计算
*输入：
*输出:VAD表格vad_result
*******************************/
void vad_do_once(float *audio){
    int dur_start,dur_stop;
    unsigned int i,j;
    int dur;
    int nn;
    struct objFFT* vad_FFT;          //fft结构体
    vad_FFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(vad_FFT , N_FFT);
	for(i=0;i<num_frames;i++){
    //dur = ((i-1)*hop_size+1):((i-1)*hop_size+win_size);//是上面计算噪声时候分的一段数据的一段
    dur_start = i*hop_size;
    dur_stop  = i*hop_size+win_size;
    dur = dur_start;
    j = 0;
    printf("run2222????????????? \n");
    while(dur != dur_stop){
        audio_frame[j] = hamming_win[j] * audio[dur];              //加窗
        fftComputeOnce(vad_FFT,audio_frame ,audio_frame_fft.real, audio_frame_fft.Imag);
       //audio_frame_fft = fft(audio_frame,N_FFT)';              //做fft
        point_multi_vad(audio_frame_fft,audio_frame_fft,frame_var,win_size);
        noise_var = noise_var_orig; 
    }
    printf("1 is start \n");
    //***************一****************
    if(argin){
        nn = 1;
        noise_var_prev = noise_var_orig;
         while (nn <= 10){
             for(j=0;j<win_size;j++){
             my_gamma[j] = (frame_var[j]/noise_var[j]); 
             }
             do_abs(Y_mag,&audio_frame_fft); 
             if (i == 0){
                for(j=0;j<win_size;j++){
                xi[j] = alpha + (1-alpha) *((my_gamma[j]-1 > 0)?my_gamma[j]:0);
                }
             }
             else{
                for(j=0;j<win_size;j++){
                xi[j] = alpha * ( (A_MMSE[i]*A_MMSE[i]) / noise_var_old[j] )  + (1-alpha) * ((my_gamma[j]-1 > 0)?my_gamma[j]:0);
                }
             }
             printf("1 is end \n");
              //***************二****************
              for(j=0;j<N_FFT;j++){
                  v[j] = (xi[j] * my_gamma[j]) / (1 + xi[j]);
                //这里bessel还没有解决
                //  G_MMSE[j] = (sqrt(M_PI)/2) * (sqrt(v[j])/my_gamma[j]) * exp(v[j]/-2)  * ((1+v[j]) * besseli(0,v[j]/2) + v[j] * besselj(1,v[j]/2));
                  if(isnan(G_MMSE[j])){
                      G_MMSE[j]=1;
                  }
                  if(isinf(G_MMSE[j])){
                      G_MMSE[j]=1;
                  }

                  A_MMSE[j] = G_MMSE[j] * Y_mag[j]; 
              }
              printf("2 is end \n");
              //********************三*************
              for(j=0;j<N_FFT;j++){
              Lambda[j] = 1/(1+xi[j]) + exp(my_gamma[j]*xi[j]/(1+xi[j]));
              Lambda_mean = sum_vad(Lambda)/N_FFT; 
              }
              printf("3 is end \n");
              //********************四*************
              weight = Lambda_mean / (1 + Lambda_mean);
              if(isnan(weight))
                weight = 1;
              for(j=0;j<N_FFT;j++){
              noise_var[j] = weight*noise_var_orig[j] + (1-weight)*frame_var[j];
              }
              diff = abs(sum_vad(noise_var) - sum_vad(noise_var_prev));
              if (diff < 0.000001)
                nn = 10;
              nn = nn + 1;
              noise_var_prev = noise_var;
              printf("4 is end \n");
         }
    }
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
   // result[i] = - data1.real[i] * data2.Imag[i] + data1.Imag[i] * data2.real[i];
    printf("%d point_multi : %f\n",i,result[i]);
    }
}
/*******************************
 * * * abs (x)
 * * *输入：复数
 * * *输出:result
 * * *******************************/
void do_abs(float *result , struct my_complex* x){
    int i=0;
    while (i != win_size){
       result[i] = sqrt(x->Imag[i]*x->Imag[i]+x->real[i]*x->real[i]);
       x++;
       i++;
    }
}

 /*******************************
  * * * * sum_vad (x)
  * * * *输入：累加数组
  * * * *输出: 累加和
  * * * *******************************/

float sum_vad(float *num){
    float result=0;
    while(*num){
        result += *num;
        num++;
    }
    return result;
}
