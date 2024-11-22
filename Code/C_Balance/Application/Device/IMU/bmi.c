/**
 * @file        bmi.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        16-November-2020
 * @brief       BMI270.
 */

/* Includes ------------------------------------------------------------------*/
#include "bmi.h"
#include "math_support.h"
#include "arm_math.h"
/*!
 * @brief This function converts lsb to meter per second squared for 16 bit accelerometer at
 * range 2G, 4G, 8G or 16G.
 */
#define GRAVITY_EARTH  (9.80665f)

float inVSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

/**
  * @brief  ����̨����ת��Ϊ��������
  * @param[in]  (int16_t) q0,  q1,  q2,  q3,  x,  y,  z
  * @param[out] (float *) ggx, ggy, ggz, aax, aay, aaz
	*/
void Gyro2World(float q0,float q1,float q2,float q3,float x,float y,float z,float *ax,float *ay,float *az)
{
	
	float a,b,c,d;
	
	a = q0;
	b = q1;
	c = q2;
	d = q3;
	
	*ax = z*(2*a*c + 2*b*d) - y*(2*a*d - 2*b*c) + x*(a*a + b*b - c*c - d*d);
	*ay = x*(2*a*d + 2*b*c) - z*(2*a*b - 2*c*d) + y*(a*a - b*b + c*c - d*d);
	*az = y*(2*a*b + 2*c*d) - x*(2*a*c - 2*b*d) + z*(a*a - b*b - c*c + d*d);

}

/**
    @param
    @Kp
        Խ���ʾԽ���μ��ٶȣ������ٻζ�ʱ��yaw��Ƕȿ��ܻ�仯���߿���Ư�ơ�KpԽ�󣬳�ʼ����ʱ�������ȶ�Խ�졣
    @Ki
        ԽС�������ԽС
    @halfT
        �������ڵ�һ�룬����1ms����1����halfTΪ0.0005f
*/
float Kp = 0.1;//4
float norm;
float halfT = 0.0005f;
float vx, vy, vz;
float ex, ey, ez;
float gx,gy,gz,ax,ay,az;
float q0_init = 1, q1_init = 0, q2_init = 0, q3_init = 0;  //����̨�ڵ�yaw��pitch��Ϊ��ʱ����ʼֵ
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;  //����̨�ڵ�yaw��pitch��Ϊ��ʱ����ʼֵ
float q0temp,q1temp,q2temp,q3temp;
float gx_, gy_, gz_;
float q0_, q1_, q2_, q3_;
float q0temp_,q1temp_,q2temp_,q3temp_;
float a_sum;

float sintemp, sintemp_, costemp, costemp_;
/**
  * @brief  ����_��Ϊ�漰���ٶȼƵģ���_��Ϊ���漰���ٶȼƵģ����ڲ�ּ����ٶ�
  * @param  
  * @retval 
  */
uint8_t BMI_Get_EulerAngle(float *pitch,float *roll,float *yaw,\
                           float *pitch_,float *roll_,float *yaw_,\
													 float *ggx,float *ggy,float *ggz,\
													 float *aax,float *aay,float *aaz)
{
	/* ������ֵ��ֵ */
	gx = *ggx;
	gy = *ggy;
	gz = *ggz;
	
	/* ���ٶȼ�ֵ��ֵ */
	ax = *aax;
	ay = *aay;
	az = *aaz;
	
//	/* ���������ݵ�λת����to��ÿ�룩 */
//	gx = lsb_to_dps(gx,2000,bmi2_dev.resolution);
//	gy = lsb_to_dps(gy,2000,bmi2_dev.resolution);
//	gz = lsb_to_dps(gz,2000,bmi2_dev.resolution);
//	
//	/* ���������ݵ�λת����to����ÿ�룩 */
//	gx = gx * (double)0.017453;
//	gy = gy * (double)0.017453;
//	gz = gz * (double)0.017453;
	
	/* ���ݸ��ٶȽ����� */
	gx_ = gx;
	gy_ = gy;
	gz_ = gz;
	
	/* �ǶȽ���start */
	/* ���ٶȼ����ݼ�� */
	if(ax * ay *az != 0)
	{
		/* ���ٶȼ����ݵ�λת����to��ÿ���η��룩 */
//		ax = lsb_to_mps2(ax,2,bmi2_dev.resolution);
//		ay = lsb_to_mps2(ay,2,bmi2_dev.resolution);
//		az = lsb_to_mps2(az,2,bmi2_dev.resolution);

		norm = inVSqrt(ax*ax + ay*ay + az*az);
		ax = ax *norm;
		ay = ay *norm;
		az = az *norm;
		
		vx = 2*(q1*q3 - q0*q2);//-sin(Pitch) cos(K,i)
		vy = 2*(q0*q1 + q2*q3);//sin(Roll)cos(Pitch) cos(K,j)
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;//cos(Roll)cos(Pitch) cos(K,k)
		
		ex = (ay*vz - az*vy) ;
		ey = (az*vx - ax*vz) ;//���߷�����ٶ�
		ez = (ax*vy - ay*vx) ;
		
		gx = gx + Kp*ex;
		gy = gy + Kp*ey;
		gz = gz + Kp*ez;
	}
	
	q0temp=q0;
  q1temp=q1;
  q2temp=q2;
  q3temp=q3;
	
	q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
	q1 = q1temp + ( q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
	q2 = q2temp + ( q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
	q3 = q3temp + ( q0temp*gz + q1temp*gy -q2temp*gx)*halfT;
	
	norm = inVSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	
	*roll = atan2(2 * q2 * q3 + 2 * q0 * q1,q0*q0 - q1 * q1 -  q2 * q2 + q3 *q3)* 57.295773f;
	*pitch = -asin( 2 * q1 * q3 -2 * q0* q2)*57.295773f;
	*yaw =  -atan2(2*(q1*q2 + q0*q3),q0*q0 +q1*q1-q2*q2 -q3*q3)*57.295773f;  //װ��У��
	/* �ǶȽ���end */

	/* �ٶȽ���begin */
	q0temp_ = q0_init;
	q1temp_ = q1_init;
	q2temp_ = q2_init;
	q3temp_ = q3_init;
	
	q0_ = q0temp_ + (-q1temp_*gx_ - q2temp_*gy_ -q3temp_*gz_)*halfT;
	q1_ = q1temp_ + ( q0temp_*gx_ + q2temp_*gz_ -q3temp_*gy_)*halfT;
	q2_ = q2temp_ + ( q0temp_*gy_ - q1temp_*gz_ +q3temp_*gx_)*halfT;
	q3_ = q3temp_ + ( q0temp_*gz_ + q1temp_*gy_ -q2temp_*gx_)*halfT;
	
	norm = inVSqrt(q0_*q0_ + q1_*q1_ + q2_*q2_ + q3_*q3_);
	q0_ = q0_ * norm;
	q1_ = q1_ * norm;
	q2_ = q2_ * norm;
	q3_ = q3_ * norm;
	
//	*roll_ = atan2(2 * q2_ * q3_ + 2 * q0_ * q1_,q0_*q0_ - q1_ * q1_ -  q2_ * q2_ + q3_ *q3_)* 57.295773f;
//	*pitch_ = -asin( 2 * q1_ * q3_ -2 * q0_ * q2_)*57.295773f;
//	*yaw_ =  -atan2(2*(q1_*q2_ + q0_*q3_),q0_*q0_ +q1_*q1_-q2_*q2_ -q3_*q3_)*57.295773f;  //װ��У��
//*roll = atan2(2 * q2 * q3 + 2 * q0 * q1,q0*q0 - q1 * q1 -  q2 * q2 + q3 *q3)* 57.295773f;
	arm_atan2_f32(2 * q2 * q3 + 2 * q0 * q1, q0 * q0 - q1 * q1 -  q2 * q2 + q3 * q3, roll);
	
	//*pitch = -asin( 2 * q1 * q3 -2 * q0* q2)*57.295773f;
  //asin(x) = atan(x/sqrt(1-x*x))
	sintemp = 2 * q1 * q3 -2 * q0* q2;
	arm_sqrt_f32(1 - sintemp * sintemp, &costemp);
	arm_atan2_f32(sintemp, costemp, pitch);
	
	//*yaw =  atan2(2*(q1*q2 + q0*q3),q0*q0 +q1*q1-q2*q2 -q3*q3)*57.295773f;
	arm_atan2_f32(2 * (q1*q2 + q0*q3), q0*q0 +q1*q1-q2*q2 -q3*q3, yaw);
	
	*roll  *=  57.295773f;
	*pitch *= -57.295773f;
	*yaw   *=  57.295773f;
	/* �ٶȽ���end */
	
	return 0;
}



