#include "process.h"
float srp_global[SERCH_POINT];
const char  binfileAddr[2][12]={"./mic16.bin","tdoa1.bin"}; //bin文件的路径
struct mic_Array mic[MIC];
struct mic_Array ss[MIC_PAIR];
float R[MIC_PAIR][FrameSize];
int TDOA_table[MIC_PAIR][SERCH_POINT];

/*******************************
*求互相关函数的计算程序
*输入：需要做点积的两个虚数组 
*输出: 点积结果
*******************************/
void point_multi(struct mic_Array data1,struct mic_Array data2,struct mic_Array *result ,int N){
    int i;
    for(i=0;i<N;i++){
      result->mic_real[i] =  data1.mic_real[i] * data2.mic_real[i] + data1.mic_Imag[i] * data2.mic_Imag[i];
      result->mic_Imag[i] = - data1.mic_real[i] * data2.mic_Imag[i] + data1.mic_Imag[i] * data2.mic_real[i];
    }
}
/*******************************
*打印用的调试程序，显示出数组信息
*输入：数组
*******************************/
void showfft(float *real ,float *imag){
    int i;
    for (i=0;i<FrameSize;i++){
        printf("%f+%fj \n",real[i],imag[i]);
    }
}

/*******************************
*对fft结果进行归一化处理，就是
   X(m,:)= X(m,:)./abs(X(m,:)); 
*输入：需要归一化的虚部和实部数组 
*输出: 归一化结果
*******************************/
void process_datafft(float *real ,float *imag){
    int i;
    float phat;
    for (i=0;i<FrameSize;i++){
        phat=sqrt(real[i]*real[i]+imag[i]*imag[i]);
        real[i]=real[i]/phat;
        imag[i]=imag[i]/phat;
    }
}
        
/*******************************
*遍历搜索的srp-phat程序
*输入：互相关函数表格 
*输出:srp_global表格，就是每个遍
历点的srp-phat值
*******************************/
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


/*******************************
*计算互相关函数表格函数
*输入：mic 一帧数据
*输出：互相关函数表格
*
*result[][]中存放的事最终的输出结果
*******************************/
void caculate_gccphat(struct mic_Array *mic ,float result[MIC_PAIR][FrameSize]){
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
        ifftComputeOnce(myFFT, ss[p].mic_real,ss[p].mic_Imag ,result[p]);
    }
}

/*******************************
*获取表格的程序，这里用的是读预先
设计好的二进制文件，用于前期调试
*file=0 read  mic16.bin
*file=1 read  tdoa1.bin
*******************************/
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

/**********************************
*srp-phat 函数入口，暂时无输入输出
*之后应该有输入输出参数
*输入：mic的一帧数据 和 tdoa表格
*输出：srp_globe  表格 
**********************************/
void do_once_srp(){
    data_read(0);
    data_read(1);
    caculate_gccphat(mic,R);
    srpphat(R,srp_global);
    showfft(srp_global,srp_global);
}
