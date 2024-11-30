#include "lqr.h"


/*Designed in Matlab*/


K_Matrix_t My_K_Matrix = {

.wheell_K =
{
  -3.535534, -2.033112, -3.752237, -0.268583, 1.732051, 0.246232, 
},
.wheelr_K =
{
  -3.535534, -2.033112, -3.752237, -0.268583, -1.732051, -0.246232, 
},

//.wheell_K =
//{
//  -3.692745, -3.287395, -8.559637, -0.590753, 3.692745, 0.369988, 
//},
//.wheelr_K =
//{
//  -3.692745, -3.287395, -8.559637, -0.590753, -3.692745, -0.369988, 
//},

.K_coefficient = 
{
  {0.00000, 0.00000, 0.00000, 0.00000, -2.73861, -0.37290, 0.63841, -0.40643, 0.08801, -2.07469, 
         0.19063, -0.37928, 0.83546, -0.04894, -7.17386, -0.05580, 0.04221, 0.22974, 0.00618, -0.77681, 
         0.00000, 0.00000, 0.00000, 0.00000, 2.73861, 0.00000, 0.00000, 0.00000, 0.00000, 0.29000},
	
  {0.00000, 0.00000, 0.00000, 0.00000, -2.73861, -0.37290, 0.63841, -0.40643, 0.08801, -2.07469, 
         0.19063, -0.37928, 0.83546, -0.04894, -7.17386, -0.05580, 0.04221, 0.22974, 0.00618, -0.77681, 
         0.00000, 0.00000, 0.00000, 0.00000, -2.73861, 0.00000, 0.00000, 0.00000, 0.00000, -0.29000}
}

};

/**
  * @brief  根据实时腿长拟合K矩阵，在腿长更新后调用(用枚举可以简化代码)
  * @param  K_Matrix_t* model, float l_angle_a, float r_angle_a
  * @retval None
  * @note   
  */
void K_Matrix_Fitting_Update(K_Matrix_t* model, float l_angle_a, float r_angle_a)
{
  /*左轮*/
  for(int i = 0; i < 6; i++)
  {
    int m = 5 * i;
    model->wheell_K[i] = model->K_coefficient[0][m + 0]*l_angle_a*l_angle_a*l_angle_a*l_angle_a + model->K_coefficient[0][m + 1]*l_angle_a*l_angle_a*l_angle_a + \
		                     model->K_coefficient[0][m + 2]*l_angle_a*l_angle_a + model->K_coefficient[0][m + 3]*l_angle_a + model->K_coefficient[0][m + 4];
  }
  /*右轮*/
  for(int i = 0; i < 6; i++)
  {
    int m = 5 * i;
    model->wheelr_K[i] = model->K_coefficient[1][m + 0]*r_angle_a*r_angle_a*r_angle_a*r_angle_a + model->K_coefficient[1][m + 1]*r_angle_a*r_angle_a*r_angle_a + \
		                     model->K_coefficient[1][m + 2]*r_angle_a*r_angle_a + model->K_coefficient[1][m + 3]*r_angle_a + model->K_coefficient[1][m + 4];
  }
}

