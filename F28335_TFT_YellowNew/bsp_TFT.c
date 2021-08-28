/**
 *  ********************************************************************************************
 *  @file                 bsp_TFT.c
 *  @file                 SK Electronics
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               TFT��ʾ�����ӿ�
 *  *******************************************************************************************
 *  @attention
 *  ʵ��ƽ̨��F28335   ���İ�
 * ���˲��ͣ�http://yellownew.cn 
 */
#include "bsp_TFT.h"
#include "bsp_SPI.h"
#include "lcdfont.h"
#include "pic.h"

extern void delay_1ms(Uint16 t);
extern Uint16 rdata;

void GPIO_TFT(void)//������Ӳ��SPI��ֻ����RES,BLK,DC��CS�ܽš����SPI��SCLK��SDA��Ҫ����
{
    EALLOW;

    /*����RES����ܽ�IO20*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO20=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO20=0;
    GpioCtrlRegs.GPADIR.bit.GPIO20=1;

    /*����DC����ܽ�IO21*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO21=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO21=0;
    GpioCtrlRegs.GPADIR.bit.GPIO21=1;

    /*����BLK����ܽ�IO22*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO22=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO22=0;
    GpioCtrlRegs.GPADIR.bit.GPIO22=1;

    /*����CS����ܽ�IO23*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO23=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO23=0;
    GpioCtrlRegs.GPADIR.bit.GPIO23=1;
    EDIS;
}


void TFT_Writ_Bus(Uint8 dat)
{
    TFT_CS_CLR();
    Spi_xmit(dat);
    TFT_CS_SET();
}

/**
 *  @brief                   TFT����д�����ݺ���
 *  @parameter               dat Ҫд�������
 *  @return_value               ��
 */
void TFT_WR_DATA8(Uint8 dat)
{

    TFT_Writ_Bus(dat<<8);

}

void TFT_WR_DATA16(Uint16 dat)
{
    TFT_Writ_Bus(dat);
    TFT_Writ_Bus(dat<<8);
}

/**
 *  @brief                   TFTд�������
 *  @parameter               dat Ҫд�������
 *  @return_value               ��
 */
void TFT_WR_REG(Uint8 dat)
{
    TFT_DC_CLR();//ѡ��д����
    TFT_Writ_Bus(dat<<8);
    TFT_DC_SET();//ѡ��д����
}

/**
 *  @brief                   ������ʼ�ͽ�����ַ
 *  @parameter               x1,x2�����е���ʼ�ͽ�����ַ��y1��y2�����е���ʼ�ͽ�����ַ
 *  @return_value               ��
 */
void TFT_Address_SET(Uint16 x1, Uint16 y1, Uint16 x2, Uint16 y2)
{
    TFT_WR_REG(0x2A);//�е�ַ����
    TFT_WR_DATA16(x1);
    TFT_WR_DATA16(x2);
    TFT_WR_REG(0x2B);//�е�ַ����
    TFT_WR_DATA16(y1);
    TFT_WR_DATA16(y2);
    TFT_WR_REG(0x2C);//������д
}

/**
 *  @brief                   ��ʼ��TFT����
 *  @parameter                  ��
 *  @return_value               ��
 */
void Init_TFT(void)
{
    Init_Spi();//��ʼ��SPI
    GPIO_TFT();//��ʼ��TFT���IO��

    TFT_RES_CLR();
    delay_1ms(100);
    TFT_RES_SET();
    delay_1ms(100);

    TFT_BLK_SET();
    delay_1ms(100);

    //************* Start Initial Sequence **********//
    TFT_WR_REG(0x11); //Sleep out
    delay_1ms(120);              //Delay 120ms
    //************* Start Initial Sequence **********//
    TFT_WR_REG(0xCF);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0xC1);
    TFT_WR_DATA8(0X30);
    TFT_WR_REG(0xED);
    TFT_WR_DATA8(0x64);
    TFT_WR_DATA8(0x03);
    TFT_WR_DATA8(0X12);
    TFT_WR_DATA8(0X81);
    TFT_WR_REG(0xE8);
    TFT_WR_DATA8(0x85);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x79);
    TFT_WR_REG(0xCB);
    TFT_WR_DATA8(0x39);
    TFT_WR_DATA8(0x2C);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x34);
    TFT_WR_DATA8(0x02);
    TFT_WR_REG(0xF7);
    TFT_WR_DATA8(0x20);
    TFT_WR_REG(0xEA);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0xC0); //Power control
    TFT_WR_DATA8(0x1D); //VRH[5:0]
    TFT_WR_REG(0xC1); //Power control
    TFT_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
    TFT_WR_REG(0xC5); //VCM control
    TFT_WR_DATA8(0x33);
    TFT_WR_DATA8(0x3F);
    TFT_WR_REG(0xC7); //VCM control
    TFT_WR_DATA8(0x92);
    TFT_WR_REG(0x3A); // Memory Access Control
    TFT_WR_DATA8(0x55);
    TFT_WR_REG(0x36); // Memory Access Control
    if(USE_HORIZONTAL==0)TFT_WR_DATA8(0x08);
    else if(USE_HORIZONTAL==1)TFT_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL==2)TFT_WR_DATA8(0x78);
    else TFT_WR_DATA8(0xA8);
    TFT_WR_REG(0xB1);
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x12);
    TFT_WR_REG(0xB6); // Display Function Control
    TFT_WR_DATA8(0x0A);
    TFT_WR_DATA8(0xA2);

    TFT_WR_REG(0x44);
    TFT_WR_DATA8(0x02);

    TFT_WR_REG(0xF2); // 3Gamma Function Disable
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0x26); //Gamma curve selected
    TFT_WR_DATA8(0x01);
    TFT_WR_REG(0xE0); //Set Gamma
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x22);
    TFT_WR_DATA8(0x1C);
    TFT_WR_DATA8(0x1B);
    TFT_WR_DATA8(0x08);
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x48);
    TFT_WR_DATA8(0xB8);
    TFT_WR_DATA8(0x34);
    TFT_WR_DATA8(0x05);
    TFT_WR_DATA8(0x0C);
    TFT_WR_DATA8(0x09);
    TFT_WR_DATA8(0x0F);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x00);
    TFT_WR_REG(0XE1); //Set Gamma
    TFT_WR_DATA8(0x00);
    TFT_WR_DATA8(0x23);
    TFT_WR_DATA8(0x24);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x10);
    TFT_WR_DATA8(0x07);
    TFT_WR_DATA8(0x38);
    TFT_WR_DATA8(0x47);
    TFT_WR_DATA8(0x4B);
    TFT_WR_DATA8(0x0A);
    TFT_WR_DATA8(0x13);
    TFT_WR_DATA8(0x06);
    TFT_WR_DATA8(0x30);
    TFT_WR_DATA8(0x38);
    TFT_WR_DATA8(0x0F);
    TFT_WR_REG(0x29); //Display on
}

/**
 *  @brief                   ��ָ�����������ɫ
 *  @parameter               xsta,ysta ��ʼ���ꣻxend,yend ��ֹ���ꣻcolor Ҫ������ɫ
 *  @return_value               ��
 */
void TFT_Fill(Uint16 xsta,Uint16 ysta,Uint16 xend,Uint16 yend,Uint16 color)
{
    Uint16 i,j;
    TFT_Address_SET(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
    for(i=ysta;i<yend;i++)
    {
        for(j=xsta;j<xend;j++)
        {
            TFT_WR_DATA16(color);
        }
    }
}

/**
 *  @brief                   ��ָ��λ�û���
 *  @parameter               x,y�������ꣻcolor �����ɫ
 *  @return_value               ��
 */
void TFT_DrawPoint(Uint16 x,Uint16 y,Uint16 color)
{
    TFT_Address_SET(x,y,x,y);//���ù��λ��
    TFT_WR_DATA16(color);
}

/**
 *  @brief                   ����
 *  @parameter               x1,y1 ��ʼ���ꣻx2,y2 ��ֹ���ꣻcolor �ߵ���ɫ
 *  @return_value               ��
 */
void TFT_DrawLine(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color)
{
    Uint16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1;//������������
    delta_y=y2-y1;
    uRow=x1;//���ߵ��������
    uCol=y1;
    if(delta_x>0)incx=1;//���õ�������
    else if (delta_x==0)incx=0;//��ֱ��
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//ˮƽ��
    else {incy=-1;delta_y=-delta_y;}
    if(delta_x>delta_y)distance=delta_x;//ѡȡ��������������
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        TFT_DrawPoint(uRow,uCol,color);//����
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}

/**
 *  @brief                   ������
 *  @parameter               x1,y1 ��ʼ���ꣻx2,y2 ��ֹ���ꣻcolor ���ε���ɫ
 *  @return_value               ��
 */
void TFT_DrawRectangle(Uint16 x1,Uint16 y1,Uint16 x2,Uint16 y2,Uint16 color)
{
    TFT_DrawLine(x1,y1,x2,y1,color);
    TFT_DrawLine(x1,y1,x1,y2,color);
    TFT_DrawLine(x1,y2,x2,y2,color);
    TFT_DrawLine(x2,y1,x2,y2,color);
}

/**
 *  @brief                   ��Բ
 *  @parameter               x0,y0 Բ�����ꣻr �뾶��color Բ����ɫ
 *  @return_value               ��
 */
void Draw_Circle(Uint16 x0,Uint16 y0,Uint8 r,Uint16 color)
{
    int a,b;
    a=0;b=r;
    while(a<=b)
    {
        TFT_DrawPoint(x0-b,y0-a,color);             //3
        TFT_DrawPoint(x0+b,y0-a,color);             //0
        TFT_DrawPoint(x0-a,y0+b,color);             //1
        TFT_DrawPoint(x0-a,y0-b,color);             //2
        TFT_DrawPoint(x0+b,y0+a,color);             //4
        TFT_DrawPoint(x0+a,y0-b,color);             //5
        TFT_DrawPoint(x0+a,y0+b,color);             //6
        TFT_DrawPoint(x0-b,y0+a,color);             //7
        a++;
        if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
        {
            b--;
        }
    }
}

/**
 *  @brief                   ��ʾ���ִ�
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��ִ���fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*s!=0)
    {
        if(sizey==12) TFT_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
        else if(sizey==16) TFT_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
        else if(sizey==24) TFT_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
        else if(sizey==32) TFT_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
        else return;
        s+=2;
        x+=sizey;
    }
}

/**
 *  @brief                   ��ʾ����12x12�ĺ���
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChinese12x12(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//���ֵ���Ŀ
    Uint16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

    HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont12[k].Index[0]==(*(s))+0xFF00)&&(tfont12[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ���ʽ
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//����ʽ
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//��һ����
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 *  @brief                   ��ʾ����16x16�ĺ���
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChinese16x16(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//������Ŀ
    Uint16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont16[k].Index[0]==(*(s))+0xFF00)&&(tfont16[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//���ӷ�ʽ
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//��һ����
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 *  @brief                   ��ʾ����24x24�ĺ���
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChinese24x24(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//������Ŀ
    Uint16 TypefaceNum;//һ���ַ���ռ�ֽڵĴ�С
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont24[k].Index[0]==(*(s))+0xFF00)&&(tfont24[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//���ӷ�ʽ
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//��һ����
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 *  @brief                   ��ʾ����32x32�ĺ���
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChinese32x32(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//������Ŀ
    Uint16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==(*(s))+0xFF00)&&(tfont32[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//���ӷ�ʽ
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//��һ����
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 *  @brief                   SPI������
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ѡ48��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_SPIChinese(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*s!=0)
    {
        TFT_ShowChineseSPI(x,y,s,fc,bc,sizey,mode);
        s+=2;
        x+=sizey;
    }
}

/**
 *  @brief                   ��ʾ����48x48�ĺ���
 *  @parameter               x,y ��ʾ���ꣻ*s Ҫ��ʾ�ĺ��֣�fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ѡ 48��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChineseSPI(Uint16 x,Uint16 y,Uint8 *s,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 i,j,m=0;
    Uint16 k;
    Uint16 HZnum;//������Ŀ
    Uint16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    Uint16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32SPI)/sizeof(typFNT_GBSPI);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32SPI[k].Index[0]==(*(s))+0xFF00)&&(tfont32SPI[k].Index[1]==(*(s+1))+0xFF00))
        {
            TFT_Address_SET(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
                    {
                        if(tfont32SPI[k].Msk[i]&(0x01<<j))TFT_WR_DATA16(fc);
                        else TFT_WR_DATA16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//���ӷ�ʽ
                    {
                        if(tfont32SPI[k].Msk[i]&(0x01<<j)) TFT_DrawPoint(x,y,fc);//��һ����
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}
/**
 *  @brief                   ��ʾ�����ַ�
 *  @parameter               x,y ��ʾ���ꣻnum Ҫ��ʾ���ַ���fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowChar(Uint16 x,Uint16 y,Uint8 num,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    Uint8 temp,sizex,t,m=0;
    Uint16 i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    Uint16 x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //�õ�ƫ�ƺ��ֵ
    TFT_Address_SET(x,y,x+sizex-1,y+sizey-1);  //���ù��λ��
    for(i=0;i<TypefaceNum;i++)
    {
        if(sizey==12)temp=ascii_1206[num][i];            //����6x12����
        else if(sizey==16)temp=ascii_1608[num][i];       //����8x16����
        else if(sizey==24)temp=ascii_2412[num][i];       //����12x24����
        else if(sizey==32)temp=ascii_3216[num][i];       //����16x32����
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//�ǵ���ģʽ
            {
                if(temp&(0x01<<t))TFT_WR_DATA16(fc);
                else TFT_WR_DATA16(bc);
                m++;
                if(m%sizex==0)
                {
                    m=0;
                    break;
                }
            }
            else//����ģʽ
            {
                if(temp&(0x01<<t))TFT_DrawPoint(x,y,fc);//��һ����
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/**
 *  @brief                   ��ʾ�ַ���
 *  @parameter               x,y ��ʾ���ꣻ*p Ҫ��ʾ���ַ�����fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey �ֺ� ��ѡ 16 24 32��mode 0�ǵ���ģʽ 1����ģʽ
 *  @return_value               ��
 */
void TFT_ShowString(Uint16 x,Uint16 y,const Uint8 *p,Uint16 fc,Uint16 bc,Uint8 sizey,Uint8 mode)
{
    while(*p!='\0')
    {
        TFT_ShowChar(x,y,*p,fc,bc,sizey,mode);
        x+=sizey/2;
        p++;
    }
}

/**
 *  @brief                   ��ʾָ��
 *  @parameter               m������nָ��
 *  @return_value               32λ
 */
Uint32 mypow(Uint8 m,Uint8 n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}

/**
 *  @brief                   ��ʾ��������
 *  @parameter               x,y ��ʾ���ꣻnum Ҫ��ʾ����������len Ҫ��ʾ��λ����fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey
 *  @return_value               ��
 */
void TFT_ShowIntNum(Uint16 x,Uint16 y,Uint16 num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey)
{
    Uint8 t,temp;
    Uint8 enshow=0;
    Uint8 sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                TFT_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
                continue;
            }else enshow=1;

        }
        TFT_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}

/**
 *  @brief                   ��ʾ��λС������
 *  @parameter               x,y ��ʾ���ꣻnum Ҫ��ʾ����������len Ҫ��ʾ��λ����fc �ֵ���ɫ��bc �ֵı�����ɫ��sizey
 *  @return_value               ��
 */
void TFT_ShowFloatNum1(Uint16 x,Uint16 y,float num,Uint8 len,Uint16 fc,Uint16 bc,Uint8 sizey)
{
    Uint8 t,temp,sizex;
    Uint16 num1;
    sizex=sizey/2;
    num1=num*100;
    for(t=0;t<len;t++)
    {
        temp=(num1/mypow(10,len-t-1))%10;
        if(t==(len-2))
        {
            TFT_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
            t++;
            len+=1;
        }
        TFT_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}

/**
 *  @brief                   ��ʾͼƬ
 *  @parameter               x,y ������ꣻlength ͼƬ���ȣ�width ͼƬ��ȣ�pic[] ͼƬ����
 *  @return_value               ��
 */
void TFT_ShowPicture(Uint16 x,Uint16 y,Uint16 length,Uint16 width,const Uint8 pic[])
{
    Uint16 i,j;
    Uint32 k=0;
    TFT_Address_SET(x,y,x+length-1,y+width-1);
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            TFT_WR_DATA8(pic[k*2]);
            TFT_WR_DATA8(pic[k*2+1]);
            k++;
        }
    }
}
