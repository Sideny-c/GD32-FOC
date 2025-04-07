/*
#include "menu.h"
#include "OLED.h"
char ExitMenu_flag=0;

void Motor_Control(void);

//----------------------------------   ���˵�   -------------------------------
// һ���˵�
MENU_PRMT MainMenu_Prmt;
MENU_TABLE MainMenu_Table[] =
{
      {" Motor_Ctrl",Motor_Control, NULL},
};

// �����˵�1  �н������������
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
  char page;    //��ʾ�˵���Ҫ��ҳ��
  char txt[32];

  if(num - PAGE_DISP_NUM <= 0)
      page = 1;
  else
  {
      page = num - PAGE_DISP_NUM + 1;
      num = PAGE_DISP_NUM;
  }
  // ��ʾ������ҳ������
  Menu_PrmtInit(prmt, num, page);

  do
  {
      //tft180_show_string(0, 0, menuName, RGB565_WHITE, RGB565_BLACK);
      OLED_ShowString(0,0,menuName,12); // ��ʾ�˵�����
      // ��ʾ�ָ���
//      for(uint8 y=0; y < OLED_H; y++)
//      {
//          lcd_showchar(OLED_W-1, y,'|', BLACK,WHITE);//Ҫ��
//      }
      // ��ʾ�˵���
      Menu_Display(table, prmt->PageNo, prmt->DispNum, prmt->Cursor);

      key = Key_Scan();                       // ��ȡ����

      if ( Menu_Move(prmt, key) == 0 )           // �˵��ƶ�������ȷ�ϼ�
      {
        // �жϴ˲˵���������Ҫ���ڵĲ�������������������
        if(table[prmt->Index].DebugParam != NULL && table[prmt->Index].ItemHook == Menu_Null)
        {
              site.x = (OLED_W-OLED_NUM)/8;   site.y = (1 + prmt->Cursor) ;
							OLED_ShowNum(site.x,site.y,(float)*(table[prmt->Index].DebugParam),5,12);
              adjustParam(site, table[prmt->Index].DebugParam, 4, u16RED, u16BLACK);
              
        }
        // ���ǲ������ڵĻ���ִ�в˵�����
        else
        {
              table[prmt->Index].ItemHook();         // ִ����Ӧ��
        }
      }
  }while (prmt->ExitMark == 0 && ExitMenu_flag == 0);
  TFTSPI_CLS(u16BLACK);
  delayms(10);
}


void Menu_PrmtInit(MENU_PRMT *prmt, char num, char page)
{
    prmt->ExitMark = 0;       //����˳��˵���־
    prmt->Cursor   = 0;       //�������
    prmt->PageNo   = 0;       //ҳ����
    prmt->Index    = 0;       //��������
    prmt->DispNum  = num;     //ҳ�����ʾ��Ŀ��
    prmt->MaxPage  = page;    //���ҳ��
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
          // ������ʾ��ǰ���ѡ�в˵��� 
          site.x = 0;    site.y = (i+1);//
          TFTSPI_P8X16Str(site.x,site.y,menuTable[pageNo+i].MenuName,u16RED,u16WHITE);
          // ���˲˵�������Ҫ���Ĳ���������ʾ�ò��� 
          if(menuTable[pageNo+i].DebugParam != NULL)
          {
            site.x = (OLED_W-OLED_NUM)/8;  //��������
            float num_t = (*(menuTable[pageNo+i].DebugParam));
					  tft180_show_uint(site.x,site.y,num_t,8,RGB565_WHITE, RGB565_BLACK);
          }
      }
      else
      {
          // ������ʾ����˵��� 
          site.x = 0;    site.y = (i+1);
          TFTSPI_P8X16Str(site.x,site.y,menuTable[pageNo+i].MenuName,u16WHITE,u16BLACK);
          //���˲˵�������Ҫ���Ĳ���������ʾ�ò��� 
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
    case KEY_UP:                   // ����
    {
      if (prmt->Cursor != 0)              // ��겻�ڶ���
      {
          prmt->Cursor--;                 // �������
      }
      else                                // ����ڶ���
      {
          if (prmt->PageNo != 0)          // ҳ��û�е���С
          {
              prmt->PageNo--;             // ���Ϸ�
          }
          else
          {
              prmt->Cursor = prmt->DispNum-1;    // ��굽��
              prmt->PageNo = prmt->MaxPage-1;    // ���ҳ
          }
      }
      break;
    }

    case KEY_DOWN:                   // ����
    {
      if (prmt->Cursor < prmt->DispNum-1)        // ���û�е��ף��ƶ����
      {
          prmt->Cursor++;                        // ��������ƶ�
      }
      else                                       // ��굽��
      {
          if (prmt->PageNo < prmt->MaxPage-1)    // ҳ��û�е��ף�ҳ���ƶ�
          {
                prmt->PageNo++;                    // �·�һҳ
          }
          else                                   // ҳ��͹�궼���ף����ؿ�ʼҳ
          {
                prmt->Cursor = 0;
                prmt->PageNo = 0;
          }
      }
      break;
    }

    case KEY_MID:                   // ȷ��
    {
            prmt->Index = prmt->Cursor + prmt->PageNo;   //����ִ���������
            rValue = 0;
            break;
    }

    case KEY_LEFT:                   // ��������ϼ��˵�
    {
            //prmt->Cursor = 0;
            //prmt->PageNo = 0;
            prmt->ExitMark = 1;
            break;
    }

    case KEY_RIGHT:                   // �Ҽ������ײ�
    {
            prmt->Cursor = prmt->DispNum-1;             // ��굽��
            prmt->PageNo = prmt->MaxPage-1;             // ���ҳ
            break;
    }

    default:break;
  }
  return rValue;                    // ����ִ������
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

          case(KEY_MID):  //�м�ļ�
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

  char menuNum = sizeof(MainMenu_Table)/sizeof(MainMenu_Table[0]);    // �˵�����

  Menu_Process("  -> Setting <- ", &MainMenu_Prmt, MainMenu_Table, menuNum);

}

*/
/******************************************************************************
* FunctionName   : Motor_Control()
* Description    : �н��������
* EntryParameter : None
* ReturnValue    : None
*******************************************************************************//*
void Motor_Control(void)
{
    TFTSPI_CLS(u16BLACK);
    char menuNum;
    menuNum = sizeof(Motor_Table)/sizeof(Motor_Table[0]);         // �˵�����
    Menu_Process(" Motor_Ctrl", &Motor_Prmt, Motor_Table, menuNum);
}
*/