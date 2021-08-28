/**
 *  ********************************************************************************************
 *  @file                 bsp_spi.c
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2020-xx-xx
 *  @brief               SPIͨ��Ӧ�ú����ӿ�
 *  *******************************************************************************************
 *  @attention
 *  ʵ��ƽ̨��F28335   ���İ�
 * ���˲��ͣ�http://yellownew.cn 
 */
#include "bsp_spi.h"
/**
 *  @brief                   ��ʼ��SPI����
 *  @parameter                  ��
 *  @return_value               ��
 */
extern Uint16 rdata;

void Init_Spi(void)
{
    InitSpiaGpio();
    EALLOW;

    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.all = 0x0057;
    SpiaRegs.SPICTL.all = 0x0016;
    SpiaRegs.SPISTS.all=0;
    SpiaRegs.SPIBRR = 0x0003;
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

    EDIS;
    IER |= M_INT6;
    PieCtrlRegs.PIEIER6.bit.INTx1 = 1;
}

void Spi_xmit(Uint16 a)
{
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG);
    SpiaRegs.SPITXBUF = a;
    while(!SpiaRegs.SPISTS.bit.INT_FLAG);
    rdata = SpiaRegs.SPIRXBUF;
}
