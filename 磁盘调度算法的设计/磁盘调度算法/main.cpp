//
//  main.cpp
//  c++磁盘调度算法的设计
//
//  Created by wejudging on 2018/12/9.
//  Copyright © 2018 wejudging. All rights reserved.
//


#include "stdio.h"
#include "stdlib.h"

void CopyL(int Sour[],int Dist[] ,int x); //数组Sour复制到数组Dist，复制到x个数
void SetDI(int DiscL[]);  //随机生成磁道数
void Print(int Pri[],int x);  //打印输出数组Pri
void DelInq(int Sour[],int x,int y);  //数组Sour把x位置的数删除，并把y前面的数向前移动，y后的数保持不变(即会出现2个y)
void FCFS(int Han,int DiscL[]);  //先来先服务算法(FCFS)
void SSTF(int Han,int DiscL[]);  //最短寻道时间优先算法(SSTF)
int SCAN(int Han,int DiscL[],int x,int y);  //扫描算法(SCAN)
void CSCAN(int Han,int DiscL[]);  //循环扫描算法(CSCAN)
void N_Step_SCAN(int Han1,int DiscL[]);  //N步扫描算法(NStepScan)
void PaiXu();  //寻道长度由低到高排序
void Pri();
int NAll=0;
int Best[5][2]; //用作寻道长度由低到高排序时存放的数组
int Limit=0; //输入寻找的范围磁道数i
int Jage;
float Aver=0;

int main()
{
   // int i;
    int DiscLine[10];  //声明准备要生成的随机磁道号的数组
    int Hand;  //磁道数
    int Con=1;
    int n;
    while(Con==1)
    {
        Jage=0;
        printf(" 请输入初始的磁道数(0<n<65536):");
        scanf("%d",&Hand);
        printf(" + 输入寻找的范围:");
        scanf("%d",&Limit);
        if(Limit<=0||Limit>65536){
            printf("超出范围!");
        }
        else{
            printf("             ╭═══════════════╮             \n");
            printf("             ║       操作系统课程实验       ║             \n");
            printf(" ╭═════┤         磁盘调度算法         ├═════╮ \n");
            printf(" ║          ║                              ║          ║ \n");
            printf(" ║          ╰═══════════════╯          ║ \n");
            printf(" ║               1.先来先服务算法(FCFS)                 ║ \n");
            printf(" ║                                                      ║ \n");
            printf(" ║               2.最短寻道时间优先算法(SSTF)           ║ \n");
            printf(" ║                                                      ║ \n");
            printf(" ║               3.扫描算法(SCAN)                       ║ \n");
            printf(" ║                                                      ║ \n");
            printf(" ║               4.循环扫描算法(CSCAN)                  ║ \n");
            printf(" ║                                                      ║ \n");
            printf(" ║                                                      ║ \n");
            printf(" ║  ╭───────────────────────╮  ║ \n");
            printf(" ╰═┤        请输入你的选择的算法(输入0离开)       ├═╯ \n");
            printf("     ╰───────────────────────╯     \n");
            scanf("%d",&n);
            if(n==0) exit(0);
            printf(" ");
            switch(n)
            {
                case 1:
                    SetDI(DiscLine);  //随机生成磁道数
                    FCFS(Hand,DiscLine); //先来先服务算法(FCFS)
                    break;
                case 2:
                    SetDI(DiscLine);  //随机生成磁道数
                    SSTF(Hand,DiscLine); //最短寻道时间优先算法(SSTF)
                    break;
                case 3:
                    SetDI(DiscLine);  //随机生成磁道数
                    SCAN(Hand,DiscLine,0,9);  //扫描算法(SCAN)
                    break;
                case 4:
                    SetDI(DiscLine);  //随机生成磁道数
                    CSCAN(Hand,DiscLine); //循环扫描算法(CSCAN)
                    break;
            }
            printf(" + 是否继续(按0结束,按1继续)?");
            scanf("%5d",&Con);
        }
        
    }
}

//数组Sour复制到数组Dist，复制到x个数
void CopyL(int Sour[],int Dist[] ,int x)
{
    int i;
    for(i=0;i<=x;i++)
    {
        Dist[i]=Sour[i];
    }
}
//打印输出数组Pri
void Print(int Pri[],int x)
{
    int i;
    for(i=0;i<=x;i++)
    {
        printf("%5d",Pri[i]);
    }
}
//随机生成磁道数
void SetDI(int DiscL[])
{
    int i;
    for(i=0;i<=9;i++)
    {
        DiscL[i]=rand()%Limit;//随机生成10个磁道号
    }
    printf("+ 需要寻找的磁道号:");
    Print(DiscL,9);  //输出随机生成的磁道号
    printf(" \n");
}
//数组Sour把x位置的数删除，并把y前面的数向前移动，y后的数保持不变(即会出现2个y)
void DelInq(int Sour[],int x,int y)
{
    int i;
    for(i=x;i<y;i++)
    {
        Sour[i]=Sour[i+1];
        x++;
    }
}
//先来先服务算法(FCFS)
void FCFS(int Han,int DiscL[])
{
    int RLine[10];  //将随机生成的磁道数数组Discl[]复制给数组RLine[]
    int i,k,All,Temp;  //Temp是计算移动的磁道距离的临时变量
    All=0;  //统计全部的磁道数变量
    k=9;  //限定10个的磁道数
    CopyL(DiscL,RLine,9);  //复制磁道号到临时数组RLine
    printf(" + 按照FCFS算法磁道的访问顺序为:");
    All=Han-RLine[0];
    for(i=0;i<=9;i++)
    {
        Temp=RLine[0]-RLine[1];//求出移动磁道数,前一个磁道数减去后一个磁道数得出临时的移动距离
        if(Temp<0)
            Temp=(-Temp);//移动磁道数为负数时,算出相反数作为移动磁道数
        printf("%5d",RLine[0]);
        All=Temp+All;//求全部磁道数的总和
        DelInq(RLine,0,k);//每个磁道数向前移动一位
        k--;
    }
    Best[Jage][1]=All;//Best[][1]存放移动磁道数
    Best[Jage][0]=1; //Best[][0]存放算法的序号为:1
    Jage++;//排序的序号加1
    Aver=((float) All)/10;//求平均寻道次数
    printf("\n");
    printf(" + 移动磁道数:<%5d> ",All);printf("\n");
    printf(" + 平均寻道长度:*%0.2f* \n",Aver);printf("\n");
}

//冒泡排序
int* Bubble(int pArr[],int nFirst,int nEnd)
{
    int nTemp=0;
    for(int i=nFirst;i<nEnd;++i)
    {
        for(int j=i;j<nEnd;++j)
        {
            if(pArr[i]>pArr[j])
            {
                nTemp=pArr[i];
                pArr[i]=pArr[j];
                pArr[j]=nTemp;
            }
        }
    }
    return pArr;
}

//最短寻道时间优先算法(SSTF)
void SSTF(int Han,int DiscL[])
{
    //int temp;
    int k=1,n=10;
    int l,r;
    int i,j,all=0;
    
    //将磁道号按递增排序
    DiscL=Bubble(DiscL,0,10);
    
    printf("\n+ 按照SSTF算法磁道的访问顺序为:：");
    
    //判断标志位Han左、右两边的偏移量大小
    if(DiscL[n-1]<=Han)//当前磁头位置大于最外围欲访问磁道
    {
        for(i=n-1;i>=0;i--)
        {
            printf("%d  ",DiscL[i]);
        }
        all=Han-DiscL[0];
    }
    else
        if(DiscL[0]>=Han)//当前磁头位置小于最里欲访问磁道
        {
            for(i=0;i<n;i++)
            {
                printf("%d  ",DiscL[i]);
            }
            all=DiscL[n-1]-Han;
        }
        else
        {
            while(DiscL[k]<Han)//确定当前磁道在已排的序列中的位置
            {
                k++;
            }
            l=k-1;//在磁头位置的前一个欲访问磁道
            r=k;//磁头欲访问磁道
            
            while((l>=0)&&(r<n))
            {
                if((Han-DiscL[l])<=(DiscL[r]-Han))//选择离磁头近的磁道
                {
                    printf("%d  ",DiscL[l]);
                    all+=Han-DiscL[l];
                    Han=DiscL[l];
                    l=l-1;
                }
                else
                {
                    printf("%d  ",DiscL[r]);
                    all+=DiscL[r]-Han;
                    Han=DiscL[r];
                    r=r+1;
                }
            }
            if(l==-1)//磁头位置里侧的磁道已访问完
            {
                for(j=r;j<n;j++)//访问磁头位置外侧的磁道
                {
                    printf("%d  ",DiscL[j]);
                }
                all+=DiscL[n-1]-DiscL[0];
            }
            if(r==n)//磁头位置外侧的磁道已访问完
            {
                for(j=k-1;j>-1;j--) //访问磁头位置里侧的磁道
                {
                    printf("%d  ",DiscL[j]);
                }
                all+=DiscL[n-1]-DiscL[0];
            }
        }
    printf("\n + 移动磁道数:<%5d> \n",all);
    printf(" + 平均寻道长度:*%0.2f* \n",all/10.0);
    
}

//扫描算法(SCAN)
int SCAN(int Han,int DiscL[],int x,int y)
{
    //int temp;
    int k=1,n=10;
    int l,r;
    int i,j,all=0;
    DiscL=Bubble(DiscL,x,y);
    //printf("按递增顺序排好的磁道为：");
    //for( i=0;i<n;i++)
    //{
    //  printf("%d ",DiscL[i]);
    //}
    
    //以下算法确定磁道访问顺序
    if(DiscL[n-1]<=Han) //磁头位置大于最外围欲访问磁道
    {
        for(i=n-1;i>=0;i--)
            printf("%d ",DiscL[i]);
        all=Han-DiscL[0];
    }
    else
        if(DiscL[0]>=Han) //磁头位置小于最里欲访问磁道
        {
            for(i=0;i<n;i++)
                printf("%d ",DiscL[i]);
            all=DiscL[n-1]-Han;
        }
        else                   //磁头位置在最里侧磁道与最外侧磁道之间
        {   int d;
            while(DiscL[k]<Han)
            {             //确定当前磁道在已排的序列中的位置
                k++;
            }
            l=k-1;//在磁头位置的前一个欲访问磁道
            r=k;   //磁头欲访问磁道
            printf("\n请输入当前磁头移动的方向 (0 表示向内 ，1表示向外) : ");
            scanf("%d",&d);  //确定磁头访问的方向
            
            printf("按照SCAN算法访问顺序为：");
            if(d==0||d==1)
            {
                if(d==0)    //磁头向内
                {
                    for(j=l;j>=0;j--)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    for(j=r;j<n;j++)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    all=Han-2*DiscL[0]+DiscL[n-1];
                }
                if(d==1)               //磁头向外
                {
                    for(j=r;j<n;j++)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    for(j=l;j>=0;j--)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    all=2*DiscL[n-1]-Han-DiscL[0];
                }
            }
            else
                printf("请输入0或1！");
        }
    
    printf("\n + 移动磁道数:<%5d> \n",all);
    printf(" + 平均寻道长度:*%0.2f* \n",all/10.0);
    return 0;
}
//循环扫描算法(CSCAN)
void CSCAN(int Han,int DiscL[])
{
   // int temp;
    int    l,r;
    int i,j,all=0;
    int k=1,n=10;
    //对访问磁道按由小到大顺序排列输出
    DiscL=Bubble(DiscL,0,10);
    //printf("按递增顺序排好的磁道为：");
    //for( i=0;i<n;i++)
    //{
    //  printf("%d ",DiscL[i]);
    //}
    
    if(DiscL[n-1]<=Han)//磁头位置大于最外围欲访问磁道
    {
        for(i=0;i<n;i++)
            printf("%d ",DiscL[i]);
        all=Han-2*DiscL[0]+DiscL[n-1];
    }
    else
        if(DiscL[0]>=Han)//磁头位置小于最里欲访问磁道
        {
            for(i=0;i<n;i++)
                printf("%d ",DiscL[i]);
            all=DiscL[n-1]-Han;
        }
        else //磁头位置在最里侧磁道与最外侧磁道之间
        {    int d;
            while(DiscL[k]<Han)
            {
                k++;
            }
            l=k-1;//在磁头位置的前一个欲访问磁道
            r=k;   //磁头欲访问磁道
            printf("\n请输入当前磁头移动的方向 (0 表示向内 ，1表示向外) : ");
            scanf("%d",&d);  //确定磁头访问的方向
            printf("按照CSCAN算法访问顺序为：");
            if(d==0||d==1)
            {
                if(d==1)    //磁头向外侧访问
                {
                    for(j=r;j<n;j++)//先访问外侧磁道再转向最里欲访问磁道
                    {
                        printf("%d ",DiscL[j]);
                    }
                    for(j=0;j<r;j++)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    all=2*DiscL[n-1]-Han-2*DiscL[0]+DiscL[l];
                }
                if(d==0)                //磁头向内侧访问
                {
                    for(j=r-1;j>=0;j--)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    for(j=n-1;j>=r;j--)
                    {
                        printf("%d ",DiscL[j]);
                    }
                    all=2*DiscL[n-1]-2*DiscL[0]+Han-DiscL[r];
                }
            }
            else
                printf("请输入0或1!");
        }
    
    printf("\n + 移动磁道数:<%5d> \n",all);
    printf(" + 平均寻道长度:*%0.2f* \n",all/10.0);
}
