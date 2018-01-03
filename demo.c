#include "dsplib/fft.h"
#include "stdio.h"
#define FrameSize 4096
#define MIC 16
#define MIC_PAIR 120
#define SAMPLES_PER_FRAME 512
struct objFFT* myFFT;
struct ParametersStruct* parament;

struct mic_Array{
    float loc_x;
    float loc_y;
    float loc_z;
    float mic_signal[FrameSize];
    float mic_real[FrameSize];
    float mic_Imag[FrameSize];
};

/*varlue*/
const char  binfileAddr[2][12]={"./mic16.bin","tdoa1.bin"}; //bin文件的路径
struct mic_Array mic[MIC];
struct mic_Array ss[MIC_PAIR];
float test[FrameSize];
/* function  */
void showfft(float *real ,float *imag);
void caculate_gccphat(struct mic_Array  *mic);
void sprphat();
void point_multi(struct mic_Array data1,struct mic_Array data2,struct mic_Array *result,int N);
void init_func();
int data_read(int file);
void process_datafft(float *real ,float *imag);
//int main(int argc, char* argv[])
int main()
{   
    init_func();
    data_read(0);
    caculate_gccphat(mic);
}


/* function  */
//*************************
// *init function */
//************************
//    
void init_func(){
    /*fft init start*/
    myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myFFT , FrameSize);
    /*fft init end*/
}

//*************************
/*cacullate the gcc-phat */
//************************
void caculate_gccphat(struct mic_Array *mic){
    int i,j;
    int p=0;
    float R[MIC_PAIR][FrameSize];

    printf("do fft ...\n");
    for(i=0;i<MIC;i++){  
        fftComputeOnce(myFFT,mic[i].mic_signal, mic[i].mic_real,  mic[i].mic_Imag);     
    }
    
    for(i=0;i<MIC;i++){
        process_datafft(mic[i].mic_real,mic[i].mic_Imag);
    }

   // Calculate the cross-power spectrum = fft(x1).*conj(fft(x2))
    for(i=0;i<MIC;i++){
        for(j=i+1;j<MIC;j++){
          point_multi(mic[i],mic[j],&ss[p],FrameSize); //ss = MIC[first_data]*MIC[j];
          p++;
        }
    }
    for(p=0;p<MIC_PAIR;p++){
        ifftComputeOnce(myFFT, ss[p].mic_real,ss[p].mic_Imag ,R[p]);
    }
   // printf("*************************");
    showfft(R[0],R[0]);
}

//********************************
/*cacullate the point multi of */
//********************************
void point_multi(struct mic_Array data1,struct mic_Array data2,struct mic_Array *result ,int N){
    int i;
    for(i=0;i<N;i++){
      result->mic_real[i] =  data1.mic_real[i] * data2.mic_real[i] + data1.mic_Imag[i] * data2.mic_Imag[i];
      result->mic_Imag[i] = - data1.mic_real[i] * data2.mic_Imag[i] + data1.mic_Imag[i] * data2.mic_real[i];
    }
}

//show the result of fft
void showfft(float *real ,float *imag){
    int i;
    for (i=0;i<FrameSize;i++){
        printf("%f+%fj \n",real[i],imag[i]);
    }
}

//*************************************
// /每个fft的数据除以phat，归一化处理*/
//*************************************    
void process_datafft(float *real ,float *imag){
    int i;
    float phat;
    for (i=0;i<FrameSize;i++){
        phat=sqrt(real[i]*real[i]+imag[i]*imag[i]);
        real[i]=real[i]/phat;
        imag[i]=imag[i]/phat;
    }
}
        
//***********************
/*caculate the spr-phat*/
//***********************
//p
//
void sprphat(){

}

//***********************
/*read the data 
 *micN   type:double      
 *tdoa                      */
//***********************
int data_read(int file){
  float num=0;
  int i,j;
  FILE *fp;//判断命令行是否正确
            //按读方式打开由argv[1]指出的文件
  if((fp=fopen(binfileAddr[file],"r"))==NULL){
    printf("The file <%s> can not be opened.\n",binfileAddr[file]);//打开操作不成功
    return -1;//结束程序的执行
  }
  for(i=0;i<MIC;i++){
    for(j=0;j<FrameSize;j++){
        fread( &num, sizeof(float), 1, fp ); 
        mic[i].mic_signal[j]=num;    
    }
  }
  fclose(fp); //关闭fp所指文件
  return 0;
 }
