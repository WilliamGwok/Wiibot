#ifndef __LQR_H
#define __LQR_H

typedef struct K_Matrix_struct_t
{
  double wheell_K[6];
  
  double wheelr_K[6];
  
  double K_coefficient[2][36];

}K_Matrix_t;

extern K_Matrix_t My_K_Matrix;

#endif


