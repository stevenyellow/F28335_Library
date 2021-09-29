#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "public.h"


//IO��������
#define MPU_SDA_IN()  {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;EDIS;}
#define MPU_SDA_OUT() {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;EDIS;}

//IO��������	 
#define MPU_SCL_SETL  (GpioDataRegs.GPACLEAR.bit.GPIO10=1)
#define MPU_SCL_SETH  (GpioDataRegs.GPASET.bit.GPIO10=1)

#define MPU_SDA_SETL  (GpioDataRegs.GPACLEAR.bit.GPIO11=1)
#define MPU_SDA_SETH  (GpioDataRegs.GPASET.bit.GPIO11=1)



#define MPU_READ_SDA  (GpioDataRegs.GPADAT.bit.GPIO11)		//����SDA

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















