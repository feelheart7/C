//
//  main.h
//  C++
//
//  Created by wejudging on 2018/11/17.
//  Copyright © 2018 wejudging. All rights reserved.
//
#ifndef main_h
#define main_h
const int Max_process = 50;//最大进程数
const int Max_source = 50;//最大资源数

class bank
{
private:
    int available[Max_source];//可用资源数
    int max[Max_process][Max_source];//最大需求
    int allocation[Max_process][Max_source];//已分配资源数
    int need[Max_process][Max_source];//还需资源数
    int request[Max_process][Max_source];//进程需要资源数
    bool finish[Max_process];//判断系统是否有足够的资源分配
    int p[Max_process];//记录序列
    int m;//用来表示进程
    int n;//表示资源
    
public:
    void Init();//完成对变量的初始化
    bool Safe();//安全检测算法
    void Banker();//银行家算法
    void Display(int ,int );//显示进程与资源状态
 
    
};





#endif /* main_h */
