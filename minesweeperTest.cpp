#include <graphics.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define MAX 100    //栈元素最大个数
//using namespace ege;

//全局变量定义区
PIMAGE thunder_all;  //扫雷所需的一整张图片指针
PIMAGE thunder_num[10]; //扫雷的各个数字图片指针 （含空图）
PIMAGE thunder_ini; //扫雷初始显示的图片指针
PIMAGE thunder_is;

typedef struct th  //雷结构体
{
    int is_th;  //是否为雷
    int is_click;//是否被点击
    int num;//周边多少个雷
}TH;

typedef struct position
{
    int r;  //行
    int c;  //列
}Position;
typedef struct stack_th
{
    int top; ///栈顶指针
    Position menber[MAX];  ///元素指针
}STACK_TH;

STACK_TH stack_a;///全局变量

Position stack_pop(){   ///元素出栈,此函数无返回值
    Position a=stack_a.menber[stack_a.top] ;
    stack_a.top--;
    return a;
}
void stack_push(Position buf){  ///元素入栈
    stack_a.menber[++stack_a.top] = buf;
}
int stack_empty(){  ///判空,如果栈为空的话返回1,否则返回0
    return stack_a.top == -1;
}
void stack_clear(){ ///清空栈
    stack_a.top = -1;
}

void Init()
{
    thunder_all=newimage();
    getimage(thunder_all,"/res/1.jpg");//获取图片资源
    thunder_ini=newimage();
    getimage(thunder_ini,"/res/backgro.jpg");
    thunder_is=newimage();
    getimage(thunder_is,"/res/thunder.jpg");
    int num=0;
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            thunder_num[num]=newimage();        //获取图片指针内存
            getimage(thunder_num[num],thunder_all,125*j,180*i,130,180);//从一个图片指针切割扫雷所需的各个图片
            num++;
        }
    }
    PIMAGE temp=newimage();
    temp=thunder_num[9];
    for(int i=9;i>=1;i--)
    {
        thunder_num[i]=thunder_num[i-1];
    }
    thunder_num[0]=temp;
}


void customize(int r,int c,int thun) //行，列，雷个数
{
    cleardevice();//清除屏幕
    TH scope[r][c];    //rxc范围
    int thunder=thun;  //共有thun个雷
    srand((unsigned)time(NULL));//随机种子
    for(int i=0;i<r*c;i++)   //先置入对应雷个数，并初始化结构体数据
    {
        if(i<thunder)
        {
            scope[i/c][i%c].is_th=1;
            scope[i/c][i%c].is_click=0;
            scope[i/c][i%c].num=0;
        }
        else
        {
            scope[i/c][i%c].is_th=0;
            scope[i/c][i%c].is_click=0;
            scope[i/c][i%c].num=0;
        }
    }
    for(int i=0;i<r*c;i++)  //扑克牌算法打乱雷位置
    {
        TH temp;
        int a=rand()%(r*c);
        temp=scope[i/c][i%c];
        scope[i/c][i%c]=scope[a/c][a%c];
        scope[a/c][a%c]=temp;
    }
    for(int i=0;i<r*c;i++) //计算周边雷数
    {
        if(i/c-1>=0&&i%c>=0&&scope[i/c-1][i%c].is_th)   //上中
        {
            scope[i/c][i%c].num++;
        }
        if(i/c-1>=0&&i%c-1>=0&&scope[i/c-1][i%c-1].is_th) //上左
        {
            scope[i/c][i%c].num++;
        }
        if(i/c-1>=0&&i%c+1<c&&scope[i/c-1][i%c+1].is_th) //上右
        {
            scope[i/c][i%c].num++;
        }
        if(i/c>=0&&i%c-1>=0&&scope[i/c][i%c-1].is_th) //左中
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c-1>=0&&scope[i/c+1][i%c-1].is_th)//左下
        {
            scope[i/c][i%c].num++;
        }
        if(i/c>=0&&i%c+1<c&&scope[i/c][i%c+1].is_th) //右中
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c+1<c&&scope[i/c+1][i%c+1].is_th) //右下
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c>=0&&scope[i/c+1][i%c].is_th) //下中
        {
            scope[i/c][i%c].num++;
        }
    }

    resizewindow(20*c,20*r); //生成对应的扫雷界面
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            putimage(20*j,20*i,20,20,thunder_ini,0,0,420,290);
        }
    }

    flushmouse();//清空鼠标信息缓冲区
    mouse_msg msg = {0};
    time_t ST=time(NULL);  //游戏开始时间
    while(true)
    {
        msg=getmouse();
        if(msg.is_left()&&msg.is_down())  //鼠标左键按下
        {
            if(scope[msg.y/20][msg.x/20].is_th)  //踩雷
            {
                scope[msg.y/20][msg.x/20].is_click=1;
                for(int i=0;i<r;i++)
                {
                    for(int j=0;j<c;j++)
                    {
                         if(!scope[i][j].is_th)
                         {
                             putimage(20*j,20*i,20,20,thunder_num[scope[i][j].num],0,0,130,180);
                         }
                         else
                         {
                             putimage(20*j,20*i,20,20,thunder_is,0,0,610,680);
                         }
                    }
                }
                MessageBox(NULL,TEXT("你已踩雷！游戏结束"),TEXT("游戏结束"),MB_OK);
                break;
            }
            else  if(scope[msg.y/20][msg.x/20].is_click==0)//未点击则执行
            {
                stack_clear();
                Position te;
                te.r=msg.y/20;
                te.c=msg.x/20;
                stack_push(te);
                while(!stack_empty())
                {
                    Position a=stack_pop();
                    scope[a.r][a.c].is_click=1;
                    if(scope[a.r][a.c].num==0)//点击区域周边无雷
                    {
                        if(a.r-1>=0&&a.c>=0)   //上中
                        {
                            if(scope[a.r-1][a.c].num==0&&scope[a.r-1][a.c].is_click==0)
                            {
                                Position b;
                                b.c=a.c;
                                b.r=a.r-1;
                                stack_push(b);
                            }
                            scope[a.r-1][a.c].is_click=1;
                            putimage(20*(a.c),20*(a.r-1),20,20,thunder_num[scope[a.r-1][a.c].num],0,0,130,180);
                        }
                        if(a.r-1>=0&&a.c-1>=0) //上左
                        {
                            if(scope[a.r-1][a.c-1].num==0&&scope[a.r-1][a.c-1].is_click==0)
                            {
                                Position b;
                                b.c=a.c-1;
                                b.r=a.r-1;
                                stack_push(b);
                            }

                            scope[a.r-1][a.c-1].is_click=1;
                            putimage(20*(a.c-1),20*(a.r-1),20,20,thunder_num[scope[a.r-1][a.c-1].num],0,0,130,180);
                        }
                        if(a.r-1>=0&&a.c+1<c) //上右
                        {
                            if(scope[a.r-1][a.c+1].num==0&&scope[a.r-1][a.c+1].is_click==0)
                            {
                                Position b;
                                b.c=a.c+1;
                                b.r=a.r-1;
                                stack_push(b);
                            }

                            scope[a.r-1][a.c+1].is_click=1;
                            putimage(20*(a.c+1),20*(a.r-1),20,20,thunder_num[scope[a.r-1][a.c+1].num],0,0,130,180);
                        }
                        if(a.r>=0&&a.c-1>=0) //左中
                        {
                            if(scope[a.r][a.c-1].num==0&&scope[a.r][a.c-1].is_click==0)
                            {
                                Position b;
                                b.c=a.c-1;
                                b.r=a.r;
                                stack_push(b);
                            }

                            scope[a.r][a.c-1].is_click=1;
                            putimage(20*(a.c-1),20*(a.r),20,20,thunder_num[scope[a.r][a.c-1].num],0,0,130,180);
                        }
                        if(a.r+1<r&&a.c-1>=0)//左下
                        {
                            if(scope[a.r+1][a.c-1].num==0&&scope[a.r+1][a.c-1].is_click==0)
                            {
                                Position b;
                                b.c=a.c-1;
                                b.r=a.r+1;
                                stack_push(b);
                            }

                            scope[a.r+1][a.c-1].is_click=1;
                            putimage(20*(a.c-1),20*(a.r+1),20,20,thunder_num[scope[a.r+1][a.c-1].num],0,0,130,180);
                        }
                        if(a.r>=0&&a.c+1<c) //右中
                        {
                            if(scope[a.r][a.c+1].num==0&&scope[a.r][a.c+1].is_click==0)
                            {
                                Position b;
                                b.c=a.c+1;
                                b.r=a.r;
                                stack_push(b);
                            }

                            scope[a.r][a.c+1].is_click=1;
                            putimage(20*(a.c+1),20*(a.r),20,20,thunder_num[scope[a.r][a.c+1].num],0,0,130,180);
                        }
                        if(a.r+1<r&&a.c+1<c) //右下
                        {
                            if(scope[a.r+1][a.c+1].num==0&&scope[a.r+1][a.c+1].is_click==0)
                            {
                                Position b;
                                b.c=a.c+1;
                                b.r=a.r+1;
                                stack_push(b);
                            }

                            scope[a.r+1][a.c+1].is_click=1;
                            putimage(20*(a.c+1),20*(a.r+1),20,20,thunder_num[scope[a.r+1][a.c+1].num],0,0,130,180);
                        }
                        if(a.r+1<r&&a.c>=0) //下中
                        {
                            if(scope[a.r+1][a.c].num==0&&scope[a.r+1][a.c].is_click==0)
                            {
                                Position b;
                                b.c=a.c;
                                b.r=a.r+1;
                                stack_push(b);
                            }

                            scope[a.r+1][a.c].is_click=1;
                            putimage(20*(a.c),20*(a.r+1),20,20,thunder_num[scope[a.r+1][a.c].num],0,0,130,180);
                        }
                    }
                    putimage(20*(a.c),20*(a.r),20,20,thunder_num[scope[a.r][a.c].num],0,0,130,180);
                }
            }
        }
        int temp=0;
        for(int i=0;i<r;i++)
        {
            for(int j=0;j<c;j++)
            {
                if(!scope[i][j].is_click) //查看所剩未点击的内容是否达到胜利条件
                {
                    temp++;
                }
            }
        }
        if(temp==thunder)
        {
            char s[15];
            time_t ET=time(NULL);
            double t=difftime(ET, ST);
            sprintf(s,"已获胜，用时：%.0lf秒",t);
            MessageBox(NULL,s,TEXT("游戏结束"),MB_OK);
            break;
        }

    }
    Sleep(2000);//停留两秒
    cleardevice();//清屏
}



int main()
{
    Init();
    mouse_msg msg = {0};
    initgraph(180,340,0);

    for ( ; is_run(); delay_fps(60))
    {
        resizewindow(180,340);
        putimage(0,0,180,340,thunder_ini,0,0,420,290);
        LOGFONTA TT;//字体样式
        getfont(&TT);
        strcpy(TT.lfFaceName, " 黑体");
        TT.lfQuality = ANTIALIASED_QUALITY;
        TT.lfWeight=FW_BOLD;
        setfont(&TT);
        settextjustify(CENTER_TEXT,CENTER_TEXT,NULL);//x居中，y居中，图片指针
        setfillstyle(EMPTY_FILL,GREEN,NULL);
        bar3d(50, 100, 140, 120, 0, 0);//画矩形框架
        bar3d(50, 180, 140, 200, 0, 0);
        bar3d(50, 260, 140, 280, 0, 0);
        setbkmode(TRANSPARENT);//文字背景透明
        outtextrect(50,30,60,100,"扫雷",NULL);
        outtextrect(50,100,80,20,"初级难度",NULL);
        outtextrect(50,180,80,20,"中级难度",NULL);
        outtextrect(50,260,80,20,"高级难度",NULL);

        msg=getmouse();
        //while(msg.is_down()){msg=getmouse();}
        if(msg.x>=50&&msg.x<=140&&msg.y>=100&&msg.y<=120&&msg.is_left())
        {
            customize(9,9,10);//初级
        }
        else if(msg.x>=50&&msg.x<=140&&msg.y>=180&&msg.y<=200&&msg.is_left())
        {
             customize(16,16,40);//中级
        }
        else if(msg.x>=50&&msg.x<=140&&msg.y>=260&&msg.y<=280&&msg.is_left())
        {
             customize(16,30,99);//高级
        }
        flushmouse();//清空鼠标信息缓冲区
    }

	closegraph();
	delimage(thunder_all);//销毁图像
	delimage(thunder_ini);
	delimage(thunder_is);
	for(int i=0;i<10;i++)
        delimage(thunder_num[i]);

	return 0;
}
