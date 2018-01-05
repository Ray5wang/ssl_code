
#ifndef TDOA_H
#define TDOA_H
#include <stdio.h>
#include <math.h>
#include "../main.h"
/*var*/
//float serch_r[SERCH_r_num]= {800 ,1600 ,2300 ,2900 ,3400 ,3800};//r方向的搜索点
float serch_space_plot[SERCH_POINT][3];
float serch_space_phi[SERCH_POINT][3];
/*function*/
void mk_tdoa_table(int TDOA_table[MIC_PAIR][SERCH_POINT]);
void sph_to_cart(float *x1 ,float *x2);
float calulate_distance(float *x1 ,float *x2);
#endif
