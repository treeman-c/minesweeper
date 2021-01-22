#include <graphics.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define MAX 100    //ջԪ��������
//using namespace ege;

//ȫ�ֱ���������
PIMAGE thunder_all;  //ɨ�������һ����ͼƬָ��
PIMAGE thunder_num[10]; //ɨ�׵ĸ�������ͼƬָ�� ������ͼ��
PIMAGE thunder_ini; //ɨ�׳�ʼ��ʾ��ͼƬָ��
PIMAGE thunder_is;

typedef struct th  //�׽ṹ��
{
    int is_th;  //�Ƿ�Ϊ��
    int is_click;//�Ƿ񱻵��
    int num;//�ܱ߶��ٸ���
}TH;

typedef struct position
{
    int r;  //��
    int c;  //��
}Position;
typedef struct stack_th
{
    int top; ///ջ��ָ��
    Position menber[MAX];  ///Ԫ��ָ��
}STACK_TH;

STACK_TH stack_a;///ȫ�ֱ���

Position stack_pop(){   ///Ԫ�س�ջ,�˺����޷���ֵ
    Position a=stack_a.menber[stack_a.top] ;
    stack_a.top--;
    return a;
}
void stack_push(Position buf){  ///Ԫ����ջ
    stack_a.menber[++stack_a.top] = buf;
}
int stack_empty(){  ///�п�,���ջΪ�յĻ�����1,���򷵻�0
    return stack_a.top == -1;
}
void stack_clear(){ ///���ջ
    stack_a.top = -1;
}

void Init()
{
    thunder_all=newimage();
    getimage(thunder_all,"/res/1.jpg");//��ȡͼƬ��Դ
    thunder_ini=newimage();
    getimage(thunder_ini,"/res/backgro.jpg");
    thunder_is=newimage();
    getimage(thunder_is,"/res/thunder.jpg");
    int num=0;
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            thunder_num[num]=newimage();        //��ȡͼƬָ���ڴ�
            getimage(thunder_num[num],thunder_all,125*j,180*i,130,180);//��һ��ͼƬָ���и�ɨ������ĸ���ͼƬ
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


void customize(int r,int c,int thun) //�У��У��׸���
{
    cleardevice();//�����Ļ
    TH scope[r][c];    //rxc��Χ
    int thunder=thun;  //����thun����
    srand((unsigned)time(NULL));//�������
    for(int i=0;i<r*c;i++)   //�������Ӧ�׸���������ʼ���ṹ������
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
    for(int i=0;i<r*c;i++)  //�˿����㷨������λ��
    {
        TH temp;
        int a=rand()%(r*c);
        temp=scope[i/c][i%c];
        scope[i/c][i%c]=scope[a/c][a%c];
        scope[a/c][a%c]=temp;
    }
    for(int i=0;i<r*c;i++) //�����ܱ�����
    {
        if(i/c-1>=0&&i%c>=0&&scope[i/c-1][i%c].is_th)   //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c-1>=0&&i%c-1>=0&&scope[i/c-1][i%c-1].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c-1>=0&&i%c+1<c&&scope[i/c-1][i%c+1].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c>=0&&i%c-1>=0&&scope[i/c][i%c-1].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c-1>=0&&scope[i/c+1][i%c-1].is_th)//����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c>=0&&i%c+1<c&&scope[i/c][i%c+1].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c+1<c&&scope[i/c+1][i%c+1].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
        if(i/c+1<r&&i%c>=0&&scope[i/c+1][i%c].is_th) //����
        {
            scope[i/c][i%c].num++;
        }
    }

    resizewindow(20*c,20*r); //���ɶ�Ӧ��ɨ�׽���
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            putimage(20*j,20*i,20,20,thunder_ini,0,0,420,290);
        }
    }

    flushmouse();//��������Ϣ������
    mouse_msg msg = {0};
    time_t ST=time(NULL);  //��Ϸ��ʼʱ��
    while(true)
    {
        msg=getmouse();
        if(msg.is_left()&&msg.is_down())  //����������
        {
            if(scope[msg.y/20][msg.x/20].is_th)  //����
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
                MessageBox(NULL,TEXT("���Ѳ��ף���Ϸ����"),TEXT("��Ϸ����"),MB_OK);
                break;
            }
            else  if(scope[msg.y/20][msg.x/20].is_click==0)//δ�����ִ��
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
                    if(scope[a.r][a.c].num==0)//��������ܱ�����
                    {
                        if(a.r-1>=0&&a.c>=0)   //����
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
                        if(a.r-1>=0&&a.c-1>=0) //����
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
                        if(a.r-1>=0&&a.c+1<c) //����
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
                        if(a.r>=0&&a.c-1>=0) //����
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
                        if(a.r+1<r&&a.c-1>=0)//����
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
                        if(a.r>=0&&a.c+1<c) //����
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
                        if(a.r+1<r&&a.c+1<c) //����
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
                        if(a.r+1<r&&a.c>=0) //����
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
                if(!scope[i][j].is_click) //�鿴��ʣδ����������Ƿ�ﵽʤ������
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
            sprintf(s,"�ѻ�ʤ����ʱ��%.0lf��",t);
            MessageBox(NULL,s,TEXT("��Ϸ����"),MB_OK);
            break;
        }

    }
    Sleep(2000);//ͣ������
    cleardevice();//����
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
        LOGFONTA TT;//������ʽ
        getfont(&TT);
        strcpy(TT.lfFaceName, " ����");
        TT.lfQuality = ANTIALIASED_QUALITY;
        TT.lfWeight=FW_BOLD;
        setfont(&TT);
        settextjustify(CENTER_TEXT,CENTER_TEXT,NULL);//x���У�y���У�ͼƬָ��
        setfillstyle(EMPTY_FILL,GREEN,NULL);
        bar3d(50, 100, 140, 120, 0, 0);//�����ο��
        bar3d(50, 180, 140, 200, 0, 0);
        bar3d(50, 260, 140, 280, 0, 0);
        setbkmode(TRANSPARENT);//���ֱ���͸��
        outtextrect(50,30,60,100,"ɨ��",NULL);
        outtextrect(50,100,80,20,"�����Ѷ�",NULL);
        outtextrect(50,180,80,20,"�м��Ѷ�",NULL);
        outtextrect(50,260,80,20,"�߼��Ѷ�",NULL);

        msg=getmouse();
        //while(msg.is_down()){msg=getmouse();}
        if(msg.x>=50&&msg.x<=140&&msg.y>=100&&msg.y<=120&&msg.is_left())
        {
            customize(9,9,10);//����
        }
        else if(msg.x>=50&&msg.x<=140&&msg.y>=180&&msg.y<=200&&msg.is_left())
        {
             customize(16,16,40);//�м�
        }
        else if(msg.x>=50&&msg.x<=140&&msg.y>=260&&msg.y<=280&&msg.is_left())
        {
             customize(16,30,99);//�߼�
        }
        flushmouse();//��������Ϣ������
    }

	closegraph();
	delimage(thunder_all);//����ͼ��
	delimage(thunder_ini);
	delimage(thunder_is);
	for(int i=0;i<10;i++)
        delimage(thunder_num[i]);

	return 0;
}
