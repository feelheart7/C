//
//  main.cpp
//  c++磁盘存储空间的管理模拟（UNIX存储管理的成组链接法的设计与实现
//
//  Created by wejudging on 2018/12/11.
//  Copyright © 2018 wejudging. All rights reserved.
//
#include <iostream>
#include <malloc/malloc.h>
using namespace std;
//用来分配内存的头文件
//******************************************************************************
const int BlockSize=512;//盘块的大小
const int GroupSize=5;  //盘块分组数量，初始化时用
//******************************************************************************
typedef struct
{
    int *top;  //指向盘块号栈的顶部，也就是栈的最上面(不是书上画的视觉的最上面)的元素的下一个地址
    int *base; //指向栈底，也就是装有盘块号栈的盘块的首地址
}Block;

Block DiskBlock[30];  //定义全局的系统盘块数组，用来模拟30个盘块的磁盘
Block MemoryBlock;    //定义内存盘块的堆栈，模拟运行在内存中的堆栈,这里为了直观，所以将内存盘块栈和系统盘块分别定义，但是这样增加了代码的长度
int GroupNumber;      //盘块分组的数量，和GroupSize不一样，它是随着分配和回收而可能发生动态变化的
bool CopyPattern;     //这里的CopyPattern的设置是为了后面分配和回收的算法共享Copy（）函数。
int SpareBlockNumber; //系统现有的空闲盘块数量（计数）
//******************************************************************************
bool InitBlock() //为盘块（包括内存盘块）分配空间，并进行部分初始化工作。
{
    for(int i=0;i<30;i++)
    {
        DiskBlock[i].base=(int *)malloc(BlockSize*sizeof(int));
        if(!DiskBlock[i].base)
        {
            cout<<"存储分配失败"<<endl;
            return false;
        }
        DiskBlock[i].top = DiskBlock[i].base+1;
        *(DiskBlock[i].base)=0;  //B.base指向的地址存放盘块数量，这里是初始化。
    }
    MemoryBlock.base=(int *)malloc(BlockSize*sizeof(int));
    if(!MemoryBlock.base)
    {
        cout<<"存储分配失败"<<endl;
        return false;
    }
    MemoryBlock.top = MemoryBlock.base+1;
    *(MemoryBlock.base)=0;  //B.base指向的地址存放盘块数量，这里是初始化。
    return true;
}
//******************************************************************************
static void InitialAllocation()//初始化系统磁盘的盘块的成组链接情况，这里为了简单直观起见，先按盘块号由小到大的顺序进行连接
{                              //程序运行中可以动态从控制台进行修改（分配或者回收）。
    GroupNumber=0;
    SpareBlockNumber=0;  //空闲盘块的数目初始置为0
    for(int j=0;j<30/GroupSize;j++)
    {
        for(int i = 0;i<GroupSize;i++)
        {
            if(j==0)
            {
                *(MemoryBlock.top)  = i;
                  MemoryBlock.top+=1;
                (*MemoryBlock.base)++;
                SpareBlockNumber++; //空闲盘块的数目加一
            }
            else
            {
                *((DiskBlock[(j-1)*GroupSize].top++))  = j*GroupSize+i;
                (*DiskBlock[(j-1)*GroupSize].base)++;
                SpareBlockNumber++; //空闲盘块的数目加一
            }
        }
        GroupNumber++;
    }
}
//******************************************************************************
bool Show()
{
    int FirstBlockID = 0;
    int GroupID=0;  //用来标识当前盘块组的序号
    bool IsFirstGroup=true;
    cout<< "***************************************************************\n";
    for(int i=0;i<GroupNumber;i++)
    {
        if(IsFirstGroup)
        {
            MemoryBlock.top=MemoryBlock.base+1;
            FirstBlockID=*(MemoryBlock.top);
            cout<< "---|------------------------Group "<< GroupID <<" has "<< *(MemoryBlock.base)<<" Block(s) as follows:\n";
            cout<< "--->";
            for(int i=0;i<*(MemoryBlock.base);i++)
                cout<<*(MemoryBlock.top++)<< "   ";
            cout<< endl;
            IsFirstGroup=false;
            GroupID++;
        }
        else
        {
            DiskBlock[FirstBlockID].top=DiskBlock[FirstBlockID].base+1;
            cout<< "---|------------------------Group "<< GroupID<<" has "<< *(DiskBlock[FirstBlockID].base)<<" Block(s) as follows:\n";
            cout<< "--->";
            for(int i=0;i<*(DiskBlock[FirstBlockID].base);i++)
                cout<<*(DiskBlock[FirstBlockID].top++)<< "   ";
            cout<< endl;
            FirstBlockID=*(DiskBlock[FirstBlockID].base+1);
            GroupID++;
        }
    }
    cout<< "***************************************************************\n";
    return true;
}
//******************************************************************************
bool Copy(bool CopyPattern,int &BlockID)//CopyPattern若是true就是分配时的复制
{                                       //（把当前组的最后剩下将要分配的的块的的内容复制到当前栈中）//若是true就是回收时的复制
    int i;       //（就是把当前栈中的内容复制到回收的这个盘块中，然后将回收的盘块号写到栈中，并当前栈中的base所指的计数值置为1）
    MemoryBlock.top=MemoryBlock.base;
    DiskBlock[BlockID].top=DiskBlock[BlockID].base;
    if(CopyPattern)
    {
        for(i=0;i<GroupSize+1;i++)//这里因为要复制base所指的计数值，所以要增加一次（比GroupSize）循环
            *(MemoryBlock.top++)=(*(DiskBlock[BlockID].top++));//循环复制
        GroupNumber--;
    }
    else
    {
        for(i=0;i<GroupSize+1;i++)
            *(DiskBlock[BlockID].top++)=*(MemoryBlock.top++);//循环复制
        GroupNumber++;
        MemoryBlock.top=MemoryBlock.base+1;//处理内存栈，令top回到base的下一个位置
        *(MemoryBlock.top++)=BlockID;//将盘块号写到top所指的位置，并使top自增
        *MemoryBlock.base=1;//将base所指的计数值置为1，表示当前组中有一个块
    }
    return true;
}
//******************************************************************************
bool RevokeBlock(int &BlockID)//回收盘块的算法
{
    if(*MemoryBlock.base>=GroupSize)//内存盘块栈满
    {
        CopyPattern=false;
        Copy(CopyPattern,BlockID);
        SpareBlockNumber++;  //空闲盘块的数目加一
    }
    else
    {
        *(MemoryBlock.top++)=BlockID; //将盘块号存入该栈（盘块）的top指针所指的单元里。并令top自增
        (*MemoryBlock.base)++;
        SpareBlockNumber++; ////空闲盘块的数目加一
    }
    return true;
}
//******************************************************************************
int  AssignBlock()//分配盘块
{   int BlockID;
    BlockID = *(--MemoryBlock.top);//把要分配的盘块号存在BlockID里
    if(*MemoryBlock.base==1)//内存盘块栈里的最后一个盘块
    {
        CopyPattern=true;
        Copy(CopyPattern,BlockID);//调用复制函数用来把要分配的盘块里的信息复制到内存盘块栈MemoryBlock里
        SpareBlockNumber--; //空闲盘块的数目减一
    }
    else
    {
        (*MemoryBlock.base)--;
        SpareBlockNumber--; //空闲盘块的数目减一
    }
    return BlockID;
}
//******************************************************************************
int main()
{
    int t=true;
    char Char;
    int Number;
    InitBlock();
    InitialAllocation();
    cout<< "System has 30 blocks and which are now all spare as we assumed. \n";
    cout<< "The 30 initial spare disk blocks are as follows:\n";
    Show();
    while(t)
    {
        cout<< "To assign( for file) block(s),you press key 'a'.\n";
        cout<< "To revoke(let file release) block(s),you press key 'r'.\n";
        cout<< "To end the programme,you press key 'e'.\n";
        cin>> Char;
        if(Char=='e')
            t=false;
        else if(Char=='a')
        {
            cout<<"How many block(s) do you want to request for the file?\n";
            cout<<"The number you input has to be less than "<<SpareBlockNumber<<".\n";
            cin>>Number;
            cout<<"The following block(s) are(is) assigned to you.\n";
            for(int i=0;i<Number;i++)
                cout<< AssignBlock()<< "  ";
            cout<<endl;
            cout<<"The current "<< SpareBlockNumber<< " spare block(s) are as follows:\n";
            Show();
        }
        else
        {
            cout<<"How many block(s) do you want to release?\n";
            cin>>Number;
            cout<<"Please input their each block's ID number.\n";
            int ID;
            for(int i=0;i<Number;i++)
            {
                cin>>ID;
                RevokeBlock(ID);
            }
            cout<< "Successfully revoked.\n";
            cout<<"The current "<< SpareBlockNumber<< " spare block(s) are as follows:\n";
            Show();
        }
    }
}


