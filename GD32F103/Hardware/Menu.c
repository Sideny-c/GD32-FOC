/*
#include "menu.h"
#include "OLED.h"
char ExitMenu_flag=0;

void Motor_Control(void);

//----------------------------------   主菜单   -------------------------------
// 一级菜单
MENU_PRMT MainMenu_Prmt;
MENU_TABLE MainMenu_Table[] =
{
      {" Motor_Ctrl",Motor_Control, NULL},
};

// 二级菜单1  行进电机参数调节
MENU_PRMT Motor_Prmt;
MENU_TABLE Motor_Table[] =
{
        {" 1.Acc_KP", Menu_Null,(float*)&P_acc_pid.kp},
        {" 2.Acc_KI", Menu_Null,(float*)&P_acc_pid.ki},
        {" 3.Acc_KD", Menu_Null,(float*)&P_acc_pid.kd},
        {" 4.Ang_KP", Menu_Null,(float*)&P_angle_pid.kp},
        {" 5.Ang_KI", Menu_Null,(float*)&P_angle_pid.ki},


};


void Menu_Process(char *menuName, MENU_PRMT *prmt, MENU_TABLE *table, char num)
{
  int key;
  Site_t site;
  char page;    //显示菜单需要的页数
  char txt[32];

  if(num - PAGE_DISP_NUM <= 0)
      page = 1;
  else
  {
      page = num - PAGE_DISP_NUM + 1;
      num = PAGE_DISP_NUM;
  }
  // 显示项数和页数设置
  Menu_PrmtInit(prmt, num, page);

  do
  {
      //tft180_show_string(0, 0, menuName, RGB565_WHITE, RGB565_BLACK);
      OLED_ShowString(0,0,menuName,12); // 显示菜单标题
      // 显示分割线
//      for(uint8 y=0; y < OLED_H; y++)
//      {
//          lcd_showchar(OLED_W-1, y,'|', BLACK,WHITE);//要改
//      }
      // 显示菜单项
      Menu_Display(table, prmt->PageNo, prmt->DispNum, prmt->Cursor);

      key = Key_Scan();                       // 获取按键

      if ( Menu_Move(prmt, key) == 0 )           // 菜单移动，按下确认键
      {
        // 判断此菜单项有无需要调节的参数，有则进入参数调节
        if(table[prmt->Index].DebugParam != NULL && table[prmt->Index].ItemHook == Menu_Null)
        {
              site.x = (OLED_W-OLED_NUM)/8;   site.y = (1 + prmt->Cursor) ;
							OLED_ShowNum(site.x,site.y,(float)*(table[prmt->Index].DebugParam),5,12);
              adjustParam(site, table[prmt->Index].DebugParam, 4, u16RED, u16BLACK);
              
        }
        // 不是参数调节的话就执行菜单函数
        else
        {
              table[prmt->Index].ItemHook();         // 执行相应项
        }
      }
  }while (prmt->ExitMark == 0 && ExitMenu_flag == 0);
  TFTSPI_CLS(u16BLACK);
  delayms(10);
}


void Menu_PrmtInit(MENU_PRMT *prmt, char num, char page)
{
    prmt->ExitMark = 0;       //清除退出菜单标志
    prmt->Cursor   = 0;       //光标清零
    prmt->PageNo   = 0;       //页清零
    prmt->Index    = 0;       //索引清零
    prmt->DispNum  = num;     //页最多显示项目数
    prmt->MaxPage  = page;    //最多页数
}


void Menu_Display(MENU_TABLE *menuTable, char pageNo, char dispNum, char cursor)
{
    char i;
    Site_t site;
    char txt[32];
    for (i=0; i<dispNum; i++)
    {
      if (cursor == i)
      {
          // 反白显示当前光标选中菜单项 
          site.x = 0;    site.y = (i+1);//
          TFTSPI_P8X16Str(site.x,site.y,menuTable[pageNo+i].MenuName,u16RED,u16WHITE);
          // 若此菜单项有需要调的参数，则显示该参数 
          if(menuTable[pageNo+i].DebugParam != NULL)
          {
            site.x = (OLED_W-OLED_NUM)/8;  //数字坐标
            float num_t = (*(menuTable[pageNo+i].DebugParam));
					  tft180_show_uint(site.x,site.y,num_t,8,RGB565_WHITE, RGB565_BLACK);
          }
      }
      else
      {
          // 正常显示其余菜单项 
          site.x = 0;    site.y = (i+1);
          TFTSPI_P8X16Str(site.x,site.y,menuTable[pageNo+i].MenuName,u16WHITE,u16BLACK);
          //若此菜单项有需要调的参数，则显示该参数 
          if(menuTable[pageNo+i].DebugParam != NULL)
          {
              site.x = (OLED_W-OLED_NUM)/8;
              float num_t = (*(menuTable[pageNo+i].DebugParam));
             // OLED_NUM(site, num_t, GREEN, WHITE);
             tft180_show_uint(site.x,site.y,num_t,8,RGB565_WHITE, RGB565_BLACK);

          }
      }
    }
}


char Menu_Move(MENU_PRMT *prmt, int key)
{
  char rValue = 1;
  switch (key)
  {
    case KEY_UP:                   // 向上
    {
      if (prmt->Cursor != 0)              // 光标不在顶端
      {
          prmt->Cursor--;                 // 光标上移
      }
      else                                // 光标在顶端
      {
          if (prmt->PageNo != 0)          // 页面没有到最小
          {
              prmt->PageNo--;             // 向上翻
          }
          else
          {
              prmt->Cursor = prmt->DispNum-1;    // 光标到底
              prmt->PageNo = prmt->MaxPage-1;    // 最后页
          }
      }
      break;
    }

    case KEY_DOWN:                   // 向下
    {
      if (prmt->Cursor < prmt->DispNum-1)        // 光标没有到底，移动光标
      {
          prmt->Cursor++;                        // 光标向下移动
      }
      else                                       // 光标到底
      {
          if (prmt->PageNo < prmt->MaxPage-1)    // 页面没有到底，页面移动
          {
                prmt->PageNo++;                    // 下翻一页
          }
          else                                   // 页面和光标都到底，返回开始页
          {
                prmt->Cursor = 0;
                prmt->PageNo = 0;
          }
      }
      break;
    }

    case KEY_MID:                   // 确认
    {
            prmt->Index = prmt->Cursor + prmt->PageNo;   //计算执行项的索引
            rValue = 0;
            break;
    }

    case KEY_LEFT:                   // 左键返回上级菜单
    {
            //prmt->Cursor = 0;
            //prmt->PageNo = 0;
            prmt->ExitMark = 1;
            break;
    }

    case KEY_RIGHT:                   // 右键跳到底部
    {
            prmt->Cursor = prmt->DispNum-1;             // 光标到底
            prmt->PageNo = prmt->MaxPage-1;             // 最后页
            break;
    }

    default:break;
  }
  return rValue;                    // 返回执行索引
}

void Menu_Null(void)
{
    delayms(100);
}

void adjustParam(Site_t site, float *param, char max_param_bit, unsigned short Color, unsigned short bkColor)
{
    char key=-1;
    float add_INTS=1;
    char txt[32];
    while(1)
    {
        key=Key_Scan();
        if(add_INTS<=1)
            add_INTS=1;
        if(add_INTS>=1000)
            add_INTS=1000;
        switch(key)
        {
           case(KEY_UP):
              add_INTS*=10;
              //(*param)++;
              tft180_show_uint(site.x,site.y, (uint32)(*param), max_param_bit, Color, bkColor);

              break;

           case(KEY_DOWN):
              add_INTS/=10;
              //(*param)--;
              tft180_show_uint(site.x,site.y, (uint32)(*param), max_param_bit, Color, bkColor);

              break;

           case(KEY_LEFT):
               (*param)-=add_INTS;
              if((*param) < 0)(*param)=0;
              tft180_show_uint(site.x,site.y, (uint32)(*param), max_param_bit, Color, bkColor);

              break;

          case(KEY_RIGHT):
              (*param)+=add_INTS;
              //(*param) += 10;
              tft180_show_uint(site.x,site.y, (uint32)(*param), max_param_bit, Color, bkColor);

              break;

          case(KEY_MID):  //中间的键
               return ;
          default:
          break;
        }
        if(*param <= 0)  *param = 0;
        if(*param >= 65535) *param = 0;
    }
}


void MainMenu_Set(void)
{
  FTSPI_CLS(u16BLACK);

  char menuNum = sizeof(MainMenu_Table)/sizeof(MainMenu_Table[0]);    // 菜单项数

  Menu_Process("  -> Setting <- ", &MainMenu_Prmt, MainMenu_Table, menuNum);

}

*/
/******************************************************************************
* FunctionName   : Motor_Control()
* Description    : 行进电机设置
* EntryParameter : None
* ReturnValue    : None
*******************************************************************************//*
void Motor_Control(void)
{
    TFTSPI_CLS(u16BLACK);
    char menuNum;
    menuNum = sizeof(Motor_Table)/sizeof(Motor_Table[0]);         // 菜单项数
    Menu_Process(" Motor_Ctrl", &Motor_Prmt, Motor_Table, menuNum);
}
*/