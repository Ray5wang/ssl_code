#include "process.h"
float srp_global[SERCH_POINT];
struct mic_Array ss[MIC_PAIR];
float R[MIC_PAIR][FrameSize];


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
void srpphat( float R[MIC_PAIR][FrameSize] ,int TDOA_table[MIC_PAIR][SERCH_POINT],float *result){
    int i,p;
    float srp_local=0;
    int  time_diff=0;
    int center = (FrameSize/2)-1;
    for(i=0;i<SERCH_POINT;i++){
        srp_local=0;
        for(p=0;p<MIC_PAIR;p++){
            time_diff=TDOA_table[p][i]+center;
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

/**********************************
*srp-phat 函数入口，暂时无输入输出
*之后应该有输入输出参数
*输入：mic的一帧数据 和 tdoa表格
*输出：srp  表格 最大值的坐标点
**********************************/
int  do_once_srp(struct mic_Array *mic ,int TDOA_table[MIC_PAIR][SERCH_POINT]){
    int i;
    int max_loc;
    float max_srp;

    printf("start srp-phat");
    caculate_gccphat(mic,R);
    srpphat(R, TDOA_table,srp_global);
    //找到srp最大值
    max_loc=0;
    max_srp=srp_global[0];
    for(i=0;i<SERCH_POINT;i++){
        if(srp_global[i] > max_srp){
            max_srp = srp_global[i];
            max_loc = i;
        }
    }
    return max_loc;
}
