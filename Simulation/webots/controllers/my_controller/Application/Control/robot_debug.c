#include "robot_debug.h"



void Debug_State_Var(void)
{
  printf("SV: s: %f, sd1: %f, thetab: %f, thetabd1: %f, phi: %f, phid1: %f.\n",\
         My_State_Var.s, My_State_Var.sd1, My_State_Var.thetab, My_State_Var.thetabd1,\
         My_State_Var.phi, My_State_Var.phid1);
}

void Debug_LQR(void)
{
  K_Matrix_t *K = &My_K_Matrix;

  double s_k = -K->wheell_K[0] * My_State_Var.s;
  double sd1_k = -K->wheell_K[1] * My_State_Var.sd1;
  double thetab_k = -K->wheell_K[2] * My_State_Var.thetab;
  double thetabd1_k = -K->wheell_K[3] * My_State_Var.thetabd1;
  double phi_k = -K->wheell_K[4] * My_Yaw_Zero_Point_Process(My_State_Var.phi);
  double phid1_k = -K->wheell_K[5] * My_State_Var.phid1;

  double tol = s_k + sd1_k + thetab_k + thetabd1_k + phi_k + phid1_k;


  printf("LQR: s: %f, sd1: %f, thetab: %f, thetabd1: %f, phi: %f, phid1: %f, total: %f\n.",\
         s_k, sd1_k, thetab_k, thetabd1_k,\
         phi_k, phid1_k, tol);
}














