#include "Kalman_filter.h"
 
KF_Struct ax;
KF_Struct ay;
KF_Struct az;
KF_Struct grx;
KF_Struct gry;
KF_Struct grz;


void KF_Struct_Init(KF_Struct* KFS)
{
	KFS->x_last	=0;
	KFS->P_now	=0;
	KFS->P_last	=0.02;
	KFS->K		=0;
	KFS->Q_cov	=0.005;//过程激励噪声协方差,参数可调
	KFS->R_cov	=0.5;//测量噪声协方差，与仪器测量的性质有关，参数可调
	KFS->x_out	=0;
}

void Struct_arr_Init()
{
  KF_Struct_Init(&ax);
  KF_Struct_Init(&ay);
  KF_Struct_Init(&az);
  KF_Struct_Init(&grx);
  KF_Struct_Init(&gry);
  KF_Struct_Init(&grz);
}


/*
* @brief    卡尔曼滤波器
* @param    KFS:卡尔曼滤波器结构体指针
* @param    z:测量仪器的输入量
* @return   当前时刻的最优估计值
*/
float KMFilter(KF_Struct* KFS,float z)
{
	KFS->P_now = KFS->P_last + KFS->Q_cov;
  KFS->K = KFS->P_now / (KFS->P_now + KFS->R_cov );
  KFS->x_out = KFS->x_out + KFS->K * (z - KFS->x_out);
  KFS->P_last = (1 - KFS->K) * KFS->P_now;
    
  return KFS->x_out;
}
