
/*
#ifndef _MENU_H_
#define _MENU_H_

#include "gd32f10x.h"
#include <stdio.h>
#include <stdint.h>

typedef struct{
        unsigned char x;
        unsigned char y;
}Site_t;

typedef struct{
    char ExitMark;     // �˳��˵�(0-���˳���1-�˳�)
    char Cursor;       // ���ֵ(��ǰ���λ��)
    char PageNo;       // �˵�ҳ(��ʾ��ʼ��)
    char Index;        // �˵�����(��ǰѡ��Ĳ˵���)
    char DispNum;      // ��ʾ����(ÿҳ�������ڲ˵���)
    char MaxPage;      // ���ҳ��(����ж�������ʾҳ)
}MENU_PRMT;      // �˵�����


typedef struct{
    char *MenuName;        // �˵���Ŀ����
    void(*ItemHook)(void);  // Ҫ���еĲ˵�����
    float *DebugParam;     // Ҫ���ԵĲ���
}MENU_TABLE;     // �˵�ִ��

#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}
#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])
#define AT                  AT_IMAGE
#define PI                 (3.1415926535898)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))



#define PAGE_DISP_NUM 3 
#define OLED_NUM 97  
#define OLED_W 128//Һ��X�����
#define OLED_H 32//Һ��Y�����

void Menu_Process(char *menuName, MENU_PRMT *prmt, MENU_TABLE *table, char num);
void Menu_PrmtInit(MENU_PRMT *prmt, char num, char page);
void Menu_Display(MENU_TABLE *menuTable, char pageNo, char dispNum, char cursor);
char Menu_Move(MENU_PRMT *prmt, int key);
void Menu_Null(void);
void adjustParam(Site_t site, float *param, char max_param_bit, unsigned short Color, unsigned short bkColor);
void MainMenu_Set(void);


#endif
*/