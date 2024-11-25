#include "dev_all.h"
#include "remote.h"

void My_Device_Init(void)
{
	My_Wheel_Init();
	
	My_Sd_Init();
	
	RC_Init(&rc);
}

