#include "dsplib/fft.h"
#include "stdio.h"
#define FrameSize 4096
#define MIC 16
#define MIC_PAIR 120
#define SAMPLES_PER_FRAME 512
#define SERCH_POINT 6480
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
float R[MIC_PAIR][FrameSize];
float srp_global[SERCH_POINT];
int TDOA_table[MIC_PAIR][SERCH_POINT];
/* function  */
void showfft(float *real ,float *imag);
void caculate_gccphat(struct mic_Array  *mic);
void srpphat( float R[MIC_PAIR][FrameSize] ,float *result);
void point_multi(struct mic_Array data1,struct mic_Array data2,struct mic_Array *result,int N);
void init_func();
int data_read(int file);
void process_datafft(float *real ,float *imag);
//int main(int argc, char* argv[])
int main()
{  
    init_func();
    data_read(0);
    data_read(1);
    caculate_gccphat(mic);
    srpphat(R,srp_global);
    showfft(srp_global,srp_global);
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
   // showfft(R[0],R[0]);
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
//最普通的算法，全局搜索
//***********************
void srpphat( float R[MIC_PAIR][FrameSize] ,float *result){
    int i,p;
    float srp_local=0;
    int  time_diff=0;
    int center = (FrameSize/2)-1;
    for(i=0;i<SERCH_POINT;i++){
        srp_local=0;
        for(p=0;p<MIC_PAIR;p++){
            time_diff=TDOA_table[p][i]+center;
    //        printf("%d\n",time_diff);
            srp_local = srp_local+R[p][time_diff];
        }
        result[i]=srp_local;
    }
}


//***********************
/*read the data     
 file=0 read  mic16.bin
 file=1 read  tdoa.bin
*/
//***********************
int data_read(int file){
  float num=0;
  int num_int=0;
  int i,j;
  FILE *fp;//判断命令行是否正确
            //按读方式打开由argv[1]指出的文件
  if((fp=fopen(binfileAddr[file],"r"))==NULL){
    printf("The file <%s> can not be opened.\n",binfileAddr[file]);//打开操作不成功
    return -1;//结束程序的执行
  }
  if (file==0){
      
    for(i=0;i<MIC;i++){
         for(j=0;j<FrameSize;j++){
             fread( &num, sizeof(float), 1, fp ); 
              mic[i].mic_signal[j]=num;    
         }
     }
  }
  else if(file==1){
      for(i=0;i<SERCH_POINT;i++){
        for(j=0;j<MIC_PAIR;j++){
            fread( &num_int, sizeof(int), 1, fp );
            TDOA_table[j][i]=num_int;
         }
      }    
  }
  fclose(fp); //关闭fp所指文件
  return 0;
 }
