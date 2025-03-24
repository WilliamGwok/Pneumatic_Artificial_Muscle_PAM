#ifndef _KALMAN_FILTER_H
#define _KALMAN_FILTER_H

typedef struct Kalman_Filter{
	float x_last;
	float P_now;
	float P_last;
	float K;
	float R_cov;
	float Q_cov;
  float x_out;
}KF_Struct; 

void Struct_arr_Init(void);
float KMFilter(KF_Struct* KFS,float z);



#endif