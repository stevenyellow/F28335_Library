/*
 * led.c
 *
 *  Created on: 2010/12/3
 *      Author: YellowNew
 */
#include "leds.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init(void)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��

	//LED1�˿�����
	GpioCtrlRegs.GPBMUX2.bit.GPIO60=0;//����Ϊͨ��GPIO����
	GpioCtrlRegs.GPBDIR.bit.GPIO60=1;//����GPIO����Ϊ���
	GpioCtrlRegs.GPBPUD.bit.GPIO60=0;//ʹ��GPIO��������

	//LED2�˿�����
	GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO61=1;
	GpioCtrlRegs.GPBPUD.bit.GPIO61=0;

	EDIS;
}
