#include "tdoa.h"

float serch_r[SERCH_r_num]= {800 ,1600 ,2300 ,2900 ,3400 ,3800};

/**********************
*创建tdoa表格
*使用的是最简单的遍历法      		
**********************/
void mk_tdoa_table(int TDOA_table[MIC_PAIR][SERCH_POINT]){
	int  i,j,k;
	int s=0;
	float th_s,ph_s;
    float th_ns,ph_ns;    //遍历的角度
	float d1,d2;
	int tdoa;

	//每次遍历的弧度单位
	th_s=(2*M_PI)/SERCH_theta_num;
	ph_s=(M_PI)/(2*SERCH_phi_num);

	for(i=0;i<SERCH_r_num;i++){
        th_ns=0;
		for(j=0;j<SERCH_theta_num;j++){
            ph_ns=0;
			for(k=0;k<SERCH_phi_num;k++){
			serch_space_phi[s][0]=th_ns;
			serch_space_phi[s][1]=ph_ns;
			serch_space_phi[s][2]=serch_r[i];
		    sph_to_cart(serch_space_phi[s],serch_space_plot[s]);
			s++;
			ph_ns = ph_ns + ph_s ;
           // printf("ph_ns is %f\n",ph_ns);
			}
			th_ns = th_ns +th_s;
           // printf("th_ns is %f\n",th_ns);
		}
		
	}

    
	for(i=0;i<SERCH_POINT;i++){
        s=0;
        printf("x:%f y:%f z:%f\n",serch_space_plot[i][0],serch_space_plot[i][1],serch_space_plot[i][2]);
		for(j=0;j<MIC;j++){
			for(k=j+1;k<MIC;k++){
				d1=calulate_distance(serch_space_plot[i],mic_loc[k]);
				d2=calulate_distance(serch_space_plot[i],mic_loc[j]);
				d1 = d2 - d1;
                if (d1==0){
                   // printf("ERROR :dd is zero \n");
                    TDOA_table[s][i]=0;
                }
                else{
				tdoa = (int)((d1/C)*FS);
				TDOA_table[s][i]=tdoa;
                }
			}   
        }  
	}
}

/**********************
*将球坐标转换成直角坐标
sph[0]  ---> theta 水平角度
sph[1]  ---> phi   垂直角度
sph[2]  ---> r	   长度
转换后：
sph[0]  ---> x 
sph[1]  ---> y   
sph[2]  ---> z	   
**********************/
void sph_to_cart(float *sph ,float *plot){
	plot[0]=sph[2]*cos(sph[1])*cos(sph[0]);
	plot[1]=sph[2]*cos(sph[1])*sin(sph[0]);
	plot[2]=sph[2]*sin(sph[1]);
  //  printf("z  theta is %f %f\n",sph[1],plot[2]);
}
/**********************
*计算两个坐标之间的距离
输入：两个点的极坐标
输出：两点之间的距离
**********************/
float calulate_distance(float *x1 ,float *x2){
	return sqrt((x1[0]-x2[0])*(x1[0]-x2[0])+(x1[2]-x2[2])*(x1[2]-x2[2])+(x1[1]-x2[1])*(x1[1]-x2[1]));
}
