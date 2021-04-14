//
//  main.cpp
//  操作系统 页式虚拟存储地址变换
//
//  Created by wejudging on 2018/11/25.
//  Copyright © 2018 wejudging. All rights reserved.
//

#include <iostream>
#include "main.h"
int main(int argc, const char * argv[]) {
    int lnumber,pnumber,write,dnumber;
    //int flag;
    unsigned laddress;
    int i;
    /*输入页表的信息，页号从0开始，依次编号，创建页表page*/
    printf("输入页表的信息，创建页表（若页号为－1，则结束输入）)\n");
    i=0;
    do
    {
        printf("输入页号和磁盘块号:");
        scanf("%d%d",&lnumber,&dnumber);
        page[i].lnumber=lnumber;
        page[i].flag=0;
        page[i].write=0;
        page[i].dnumber=dnumber;
        i++;
    }while(lnumber!=-1);
    page_length=i-1;//因为多输入了一个-1；
    printf("输入主存块号，主存块数要小于%d,(以－1结束）:",i-1);
    scanf("%d",&pnumber);
    m=0;
    head=0;
    while(pnumber!=-1)
    {
        if(m<=i)
        {
            page[m].pnumber=pnumber;
            page[m].flag=1;
            p[m]=m;//对数组P进行初始化，一开始程序的前m+1页调入内存
            m++;
        }
        scanf("%d",&pnumber);
    }
    printf("输入指令性质(1-修改,0-不需要，其他-结束程序运行)和逻辑地址:");
    scanf("%d%u",&write,&laddress);
    while(write==0||write==1)
    {
        command(laddress,write);/* 执行指令*/
        printf("输入指令性质(1-修改,0-不需要，其他-结束程序运行)和逻辑地址:");
        scanf("%d%u",&write,&laddress);
    }
    
    return 0;
}
