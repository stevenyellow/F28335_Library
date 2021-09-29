#include "mpuiic.h"
#include "public.h"


//��ʼ��IIC
void MPU_IIC_Init(void)
{
       EALLOW;
       SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��
       GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;     //����
       GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;     // ����˿�
       GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;    // IO��
       GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3;   // ��ͬ��

       GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     //����
       GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;     // ����˿�
       GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;    // IO��
       GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;   // ��ͬ��

       EDIS;

 
}
//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
    MPU_SDA_OUT();     //sda�����
    MPU_SDA_SETH;
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SDA_SETL;//START:when CLK is high,DATA change form high to low
    DELAY_US(6);
    MPU_SCL_SETL;//ǯסI2C���ߣ�׼�����ͻ��������

}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda�����


	MPU_SCL_SETL;
	MPU_SDA_SETL;//STOP:when CLK is high DATA change form low to high
	MPU_SCL_SETH;
    DELAY_US(6);
    MPU_SDA_SETH;//����I2C���߽����ź�
    DELAY_US(6);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
    unsigned char tempTime=0;
    MPU_SDA_SETH;
    DELAY_US(1);
    MPU_SDA_IN();      //SDA����Ϊ����
    MPU_SCL_SETH;
    DELAY_US(1);
    while(MPU_READ_SDA)
    {
        tempTime++;
        if(tempTime>250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_SCL_SETL;//ʱ�����0
    return 0;
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{
    MPU_SCL_SETL;
    MPU_SDA_OUT();//sda�����
    MPU_SDA_SETL;
    DELAY_US(2);
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SCL_SETL;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
    MPU_SCL_SETL;
    MPU_SDA_OUT();//sda�����
    MPU_SDA_SETH;
    DELAY_US(2);
    MPU_SCL_SETH;
    DELAY_US(5);
    MPU_SCL_SETL;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
    unsigned char t;
    MPU_SDA_OUT();//sda�����
    MPU_SCL_SETL;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>0) //0x80  1000 0000
            MPU_SDA_SETH;
        else
            MPU_SDA_SETL;
        txd<<=1;
        DELAY_US(2);   //��TEA5767��������ʱ���Ǳ����
        MPU_SCL_SETH;
        DELAY_US(2);
        MPU_SCL_SETL;
        DELAY_US(2);
    }
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MPU_SDA_IN();      //SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        MPU_SCL_SETL;
        DELAY_US(2);
        MPU_SCL_SETH;
        receive<<=1;
        if(MPU_READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK
    return receive;
}


























