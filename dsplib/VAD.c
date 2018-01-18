#include "VAD.h"

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

//float 0.99;

/*******************************
*求互相关函数的计算程序
*输入：需要做点积的两个虚数组 
*输出: 点积结果
*******************************/


