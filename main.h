
#ifndef MAIN_H
#define MAIN_H
/**********************
*config the srp-phat*/
/**********************/
#define FrameSize 4096			//每一帧数据个数
#define MIC 16					//麦克风数量
#define MIC_PAIR 120
#define SERCH_theta_num 180		//水平角方向搜索步长
#define SERCH_phi_num  6		//垂直角方向搜索步长  暂时假设垂直方向只要0~90度
#define SERCH_r_num   6         //r方向搜索点数
#define SERCH_POINT 6480
#define FS  16000        		//采样率
#define C  340000        		//声速
#ifndef M_PI
    #define M_PI    3.1415926535897932384626433832795f
#endif


//float serch_r[SERCH_r_num]= {800 ,1600 ,2300 ,2900 ,3400 ,3800};//r方向的搜索点

/**********************
*要用的变量          */			
/**********************/
struct objFFT* myFFT;          //fft结构体

#endif

