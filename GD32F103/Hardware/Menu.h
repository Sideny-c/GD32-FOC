
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
    char ExitMark;     // 退出菜单(0-不退出，1-退出)
    char Cursor;       // 光标值(当前光标位置)
    char PageNo;       // 菜单页(显示开始项)
    char Index;        // 菜单索引(当前选择的菜单项)
    char DispNum;      // 显示项数(每页可以现在菜单项)
    char MaxPage;      // 最大页数(最大有多少种显示页)
}MENU_PRMT;      // 菜单参数


typedef struct{
    char *MenuName;        // 菜单项目名称
    void(*ItemHook)(void);  // 要运行的菜单函数
    float *DebugParam;     // 要调试的参数
}MENU_TABLE;     // 菜单执行

#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}
#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])
#define AT                  AT_IMAGE
#define PI                 (3.1415926535898)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))



#define PAGE_DISP_NUM 3 
#define OLED_NUM 97  
#define OLED_W 128//液晶X方宽度
#define OLED_H 32//液晶Y方宽度

void Menu_Process(char *menuName, MENU_PRMT *prmt, MENU_TABLE *table, char num);
void Menu_PrmtInit(MENU_PRMT *prmt, char num, char page);
void Menu_Display(MENU_TABLE *menuTable, char pageNo, char dispNum, char cursor);
char Menu_Move(MENU_PRMT *prmt, int key);
void Menu_Null(void);
void adjustParam(Site_t site, float *param, char max_param_bit, unsigned short Color, unsigned short bkColor);
void MainMenu_Set(void);


#endif
*/