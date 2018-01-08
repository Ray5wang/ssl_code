#include "dsplib/fft.h"
#include "dsplib/process.h"
#include "dsplib/tdoa.h"
#include "stdio.h"
/*varlue*/
int TDOA_table[MIC_PAIR][SERCH_POINT];      //tdoa表格数据
struct mic_Array mic[MIC];                  //输入mic数据
const char  binfileAddr[2][12]={"./mic16.bin","tdoa1.bin"}; //bin文件的路径
/* function  */
void init_func();
int data_read(int file);
//int main(int argc, char* argv[])
int main()
{   int I;
    init_func();
    data_read(0);
   // data_read(1);
    tdoa_table_full(TDOA_table);
    I = do_once_srp(mic,TDOA_table);
    printf("I is %d\n",I);
}
  
/*******************************
 * 初始化函数
 * 在这里对需要初始化的函数进行
 * 初始化操作
*******************************/
void init_func(){
    myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myFFT , FrameSize);
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

