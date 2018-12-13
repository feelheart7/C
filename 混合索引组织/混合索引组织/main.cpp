//
//  main.cpp
//  混合索引组织
//
//  Created by wejudging on 2018/12/9.
//  Copyright © 2018 wejudging. All rights reserved.
//

/*
 模拟混合索引的原理；
 假设每个盘块16字节大小，每个盘块号占2字节：
 设计支持混合索引算法的索引节点的数据结构；编程模拟实现混合索引算法。
 测试：输入一个文件的长度，给出模拟分配占用的磁盘块的情况；输入一个需要访问的地址，计算该地址所在的盘块号。
 */
#include<stdio.h>
//#include<malloc.h>
#include<map>
#include<string.h>
#include<stdlib.h>
#include<time.h>
using namespace std;

//定义盘块的大小，单位为B
const int BLOCK_SIZE = 16;

//定义一个索引块中可以存放多少盘块；
const int MAX_NUMBER_IN_BLOCK = 8;

//定义盘块号的最大值
const int MAX_BLOCK_NUMBER = 5000;

//定义一次间址的索引块、二次和三次间址的最后一层索引快；
struct Index_block_three
{
    int blocks[MAX_NUMBER_IN_BLOCK];
};

//定义二次间址的第一个索引块，和三次间址的第二个索引块
struct Index_block_two
{
    Index_block_three *blocks[MAX_NUMBER_IN_BLOCK];
};

//定义三次间址的第一个索引块
struct Index_block_one
{
    Index_block_two *blocks[MAX_NUMBER_IN_BLOCK];
};

//定义混合索引的数据结构
struct Index_File
{
    //文件大小
    int fileSize;
    //定义10个直接地址项
    int addr[10];
    //定义一次间址的地指项
    Index_block_three *addr10;
    //定义二次间址的地址项
    Index_block_two *addr11;
    //定义三次间址的地址项
    Index_block_one *addr12;
};


//生成第三层索引，并并根据blocks赋值，
Index_block_three* indexBlockThree(int blocks[],int start,int end)
{
    Index_block_three * ans = (Index_block_three*)
          malloc(sizeof(Index_block_three));
    int j = 0;
    for(int i = start; i < end; i++)
    {
        ans->blocks[j++] = blocks[i];
        printf("%d ",ans->blocks[j - 1]);
    }
    putchar('\n');
    return ans;
}
//生成第二层索引
Index_block_two* indexBlockTwo(int blocks[],int start,int end)
{
    //计算生成几个第三层索引
    int num = (end - start) % MAX_NUMBER_IN_BLOCK == 0? (end - start) / MAX_NUMBER_IN_BLOCK : (end - start) / MAX_NUMBER_IN_BLOCK + 1;
    //生成第二层索引
    Index_block_two *ans = (Index_block_two *) malloc (sizeof(Index_block_two));
    //生成num第三层索引
    for(int i = 0; i < num; i++)
    {
        if(i != num -1)
            ans->blocks[i] = indexBlockThree(blocks,start + i * MAX_NUMBER_IN_BLOCK,start + (i + 1) * MAX_NUMBER_IN_BLOCK);
        else
            ans->blocks[i] = indexBlockThree(blocks,start + i * MAX_NUMBER_IN_BLOCK,end);
    }
    return ans;
}

Index_block_one* indexBlockOne(int blocks[],int start,int end)
{
    //计算二层索引块的个数
    int num = (end - start) % (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) == 0 ?
    (end - start) / (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) :
    (end - start) / (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) + 1 ;
    Index_block_one *ans = (Index_block_one*) malloc(sizeof(Index_block_one));
    //生成num个二层索引的块
    for(int i = 0; i < num; i++)
    {
        if(i != num - 1)
            ans->blocks[i] = indexBlockTwo(blocks,start + i * MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK,start + (i + 1) * MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK);
        else
            ans->blocks[i] = indexBlockTwo(blocks,start + i * MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK,end);
    }
    return ans;
}

//给定一个文件的长度，给出模拟分配占用的磁盘块的情况
Index_File* indexFile(int filesize)
{
    printf("文件大小为：%d B\n",filesize);
    //计算该文件需要多少盘块
    int block_num = filesize % BLOCK_SIZE == 0 ? filesize / BLOCK_SIZE : filesize / BLOCK_SIZE + 1;
    printf("共占 %d 个盘块\n",block_num);
    //定义保存所有盘块号的数组
    int *blocks = (int *) malloc(sizeof(int) * block_num);
    //初始化数组
    memset(blocks,0,sizeof(int) * block_num);
    
    //     for(int i = 0; i < block_num; i++)
    //        printf("%d ",blocks[i]);
    
    //模拟系统分配空闲盘块号
    //随机数种子
    //srand(time(NULL));
    //map标记，避免产生重复的盘块号
    map<int,int> flag;
    for(int i = 0; i < block_num; i++)
    {
        int temp = rand() % MAX_BLOCK_NUMBER;
        while(flag[temp] != 0)
        {
            temp = rand() % MAX_BLOCK_NUMBER;
        }
        flag[temp] = 1;
        blocks[i] = temp;
    }
    
    //     for(int i = 0; i < block_num; i++)
    //        printf("%d ",blocks[i]);
    
    Index_File *indexfile;
    indexfile = (Index_File*) malloc(sizeof(Index_File));
    indexfile->fileSize = filesize;
    //直接地址
    if(block_num <= 10)
    {
        printf("直接盘块号为：\n");
        for(int i = 0; i < block_num; i++)
        {
            indexfile->addr[i] = blocks[i];
            printf("%d ",indexfile->addr[i]);
        }
    }
    //一次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK + 10)
    {
        printf("直接盘块号为：\n");
        for(int i = 0; i < 10; i++)
        {
            indexfile->addr[i] = blocks[i];
            printf("%d ",indexfile->addr[i]);
        }
        printf("\n一次间址盘块号为：\n");
        indexfile->addr10 = indexBlockThree(blocks,10,block_num);
    }
    //二次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK * (MAX_NUMBER_IN_BLOCK + 1) + 10)
    {
        printf("直接盘块号为：\n");
        for(int i = 0; i < 10; i++)
        {
            indexfile->addr[i] = blocks[i];
            printf("%d ",indexfile->addr[i]);
        }
        printf("\n一次间址盘块号为：\n");
        indexfile->addr10 = indexBlockThree(blocks,10,MAX_NUMBER_IN_BLOCK + 10);
        printf("二次间址盘块号为：\n");
        indexfile->addr11 = indexBlockTwo(blocks,MAX_NUMBER_IN_BLOCK+10,block_num);
    }
    //三次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK + MAX_NUMBER_IN_BLOCK * (MAX_NUMBER_IN_BLOCK + 1) + 10)
    {
        printf("直接盘块号为：\n");
        for(int i = 0; i < 10; i++)
        {
            indexfile->addr[i] = blocks[i];
            printf("%d ",indexfile->addr[i]);
        }
        printf("\n一次间址盘块号为：\n");
        indexfile->addr10 = indexBlockThree(blocks,10,MAX_NUMBER_IN_BLOCK + 10);
        printf("二次间址盘块号为：\n");
        indexfile->addr11 = indexBlockTwo(blocks,MAX_NUMBER_IN_BLOCK+10,MAX_NUMBER_IN_BLOCK * (MAX_NUMBER_IN_BLOCK + 1) + 10);
        printf("三次间址盘块号为：\n");
        indexfile->addr12 = indexBlockOne(blocks,MAX_NUMBER_IN_BLOCK * (MAX_NUMBER_IN_BLOCK + 1) + 10,block_num);
    }
    return indexfile;
}


//给定地址addrss和文件indexfile，找到地址对应的块号；
int findBlock(int addrss, Index_File *indexfile)
{
    addrss++;
    int block_num = addrss % BLOCK_SIZE == 0 ? addrss / BLOCK_SIZE : addrss / BLOCK_SIZE + 1;
    printf("%d 在第 %d 个盘块\n",addrss - 1,block_num);
    int ans;
    if(block_num <= 10)
    {
        ans = indexfile->addr[block_num - 1];
    }
    //一次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK + 10)
    {
        block_num -= 10;
        //计算盘块号
        int index = block_num - 1;
        //查找盘块
        ans = indexfile->addr10->blocks[index];
    }
    //二次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK + MAX_NUMBER_IN_BLOCK + 10)
    {
        block_num -= MAX_NUMBER_IN_BLOCK + 10;
        //计算第三层索引块的序号
        int index_three = block_num % MAX_NUMBER_IN_BLOCK == 0 ? block_num / MAX_NUMBER_IN_BLOCK : block_num / MAX_NUMBER_IN_BLOCK + 1;
        //计算盘块号
        int index = block_num - ((index_three - 1) * MAX_NUMBER_IN_BLOCK);
        //查找盘块
        ans = indexfile->addr11->blocks[index_three - 1]->blocks[index - 1];
    }
    //三次间址
    else if(block_num <= MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK + MAX_NUMBER_IN_BLOCK * (MAX_NUMBER_IN_BLOCK + 1) + 10)
    {
        block_num -= MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK + MAX_NUMBER_IN_BLOCK + 10;
        //计算第二层索引块的序号
        int index_two = block_num % (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) == 0 ?
        block_num / (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) : block_num / (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK) + 1;
        block_num -= (index_two - 1) * (MAX_NUMBER_IN_BLOCK * MAX_NUMBER_IN_BLOCK);
        //计算第二层索引块的序号
        int index_three = block_num % MAX_NUMBER_IN_BLOCK == 0 ? block_num / MAX_NUMBER_IN_BLOCK : block_num / MAX_NUMBER_IN_BLOCK + 1;
        //计算盘块号
        int index = block_num - (index_three - 1) * MAX_NUMBER_IN_BLOCK;
        //查找盘块
        ans = indexfile->addr12->blocks[index_two - 1]->blocks[index_three - 1]->blocks[index - 1];
    }
    return ans;
}

int main()
{
    Index_File *file = indexFile(16*16*16);
    printf("\ninput the addrss\n");
    int addrss;
    scanf("%d",&addrss);
    printf("%d 所在的盘块号为 %d \n",addrss,findBlock(addrss,file));
    return 0;
}
