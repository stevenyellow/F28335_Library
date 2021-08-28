/*
 * iic.c
 *
 *  Created on: 2021/4/23
 *      Author: YellowNEW
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "iic.h"


/*******************************************************************************
* �� �� ��         : IIC_Init
* ��������		   : IIC��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IICA_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;	  	//����
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;   	// ����˿�
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;  	// IO��
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3; 	// ��ͬ��

	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	  	//����
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;   	// ����˿�
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  	// IO��
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;   // ��ͬ��
	EDIS;
}


/*******************************************************************************
* �� �� ��         : IIC_Start
* ��������		   : ����IIC��ʼ�ź�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Start(void)
{

	IIC_SDA_SETH;    //�Ƚ�SDA�ø�
	IIC_SCL_SETH;    //�ٽ�SCL�õ�
	DELAY_US(5);     //��ʱһС��ʱ���ȶ�һ��
	IIC_SDA_SETL;    //��SCLΪ�ߵ�ƽ�ڼ�SDA�ɸ߱�������ʹ�ź�
	DELAY_US(6);     //��ʱһС��ʱ���ȶ�һ��
	IIC_SCL_SETL;    //ǯסI2C���ߣ�׼�����ͻ��������
}

/*******************************************************************************
* �� �� ��         : IIC_Stop
* ��������		   : ����IICֹͣ�ź�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Stop(void)
{

	IIC_SDA_SETL;//�Ƚ�SDA�õ�
	IIC_SCL_SETH;//Ȼ���ڽ�SCL��Ϊ�ߵ�ƽ
 	DELAY_US(6); //��ʱһ��ʱ���ȶ�һ��
 	IIC_SDA_SETH;//Ȼ����SCLΪ�ߵ�ƽ�ڼ佫SDA�õ͵�ƽʵ��ֹͣ�ź�
	DELAY_US(6); //��ʱһС��ʱ���ȶ�һ��
}

/*******************************************************************************
* �� �� ��         : IIC_Wait_Ack
* ��������		   : �ȴ�Ӧ���źŵ���
* ��    ��         : ��
* ��    ��         : 1������Ӧ��ʧ��
        			 0������Ӧ��ɹ�
*******************************************************************************/
unsigned char IIC_Wait_Ack(void)
{
	unsigned char tempTime=0;
	IIC_SDA_SETH;//������SDA�ͷţ�����SDA��Ϊ�ߵ�ƽ
	DELAY_US(1);
	IIC_SCL_SETH;//������SCL�øߵȴ��ӻ�����SDA����һ��Ӧ���ź�
	DELAY_US(1);
	while(READ_SDA)//�����ȡ��READ_SDA��Ϊ�͵�ƽ˵���ӻ���Ӧ��Ӧ��
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_Stop();//����ӻ�û�н�SDA��Ϊ�͵�ƽ˵��û�в���Ӧ���ʱ������Ҫ����ֹͣ�ź�
			return 1;
		}
	}
	IIC_SCL_SETL;//ʱ�����0
	return 0;
}
/*
 * ��ΪOLED����ͨ������д����������ȡ��ȡ�����������ĺ����ڱ��ι����в�û��ʹ��
 */

/*******************************************************************************
* �� �� ��         : IIC_Ack
* ��������		   : ����ACKӦ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_SETL;
	IIC_SDA_SETL;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* �� �� ��         : IIC_NAck
* ��������		   : ����NACK��Ӧ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_SETL;
	IIC_SDA_SETH;
	DELAY_US(2);
	IIC_SCL_SETH;
	DELAY_US(5);
	IIC_SCL_SETL;
}

/*******************************************************************************
* �� �� ��         : IIC_Send_Byte
* ��������		   : IIC����һ���ֽ�
* ��    ��         : txd������һ���ֽ�
* ��    ��         : ��
*******************************************************************************/
void IIC_Send_Byte(unsigned char txd)
{
	unsigned char t;
	IIC_SCL_SETL;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        if(txd&0x80) //0x80  1000 0000
        	IIC_SDA_SETH;
		else
			IIC_SDA_SETL;
        txd<<=1;
        DELAY_US(2);
        IIC_SCL_SETH;//ʱ�����߽����ݴ������
		DELAY_US(2);
		IIC_SCL_SETL;//ʱ���ٴ�����׼��������һ��SDA������
		DELAY_US(2);
    }
}

/*******************************************************************************
* �� �� ��       : IIC_Read_Byte
* ��������     : IIC��һ���ֽ�
* ��    ��         : ack=1ʱ������ACK��ack=0������nACK
* ��    ��         : Ӧ����Ӧ��
*******************************************************************************/
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
    for(i=0;i<8;i++ )
	{
    	IIC_SCL_SETL;
    	DELAY_US(2);
        IIC_SCL_SETH;
        receive<<=1;
        if(READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

