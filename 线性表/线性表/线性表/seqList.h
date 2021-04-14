//
//  seqList.h
//  线性表
//
//  Created by weijiajin on 2021/4/9.
//

#ifndef seqList_h
#define seqList_h
#include "List.h"
#include<iostream>
 using namespace std;
 ​
 //celemType为顺序表存储的元素类型
 template <class elemType>
 class seqList: public List<elemType> {
 private:
     // 利用数组存储数据元素
     elemType *data;
     // 当前顺序表中存储的元素个数
     int curLength;
     // 顺序表的最大长度
     int maxSize;
     // 表满时扩大表空间
     void resize();
 public:
     // 构造函数
     seqList(int initSize = 10);
     // 拷贝构造函数
     seqList(seqList & sl);
     // 析构函数
     ~seqList()  {delete [] data;}
     // 清空表，只需修改curLength
     void clear()  {curLength = 0;}
     // 判空
     bool empty()const{return curLength == 0;}
     // 返回顺序表的当前存储元素的个数
     int size() const  {return curLength;}
     // 在位置i上插入一个元素value，表的长度增1
     void insert(int i,const elemType &value);
     // 删除位置i上的元素value，若删除位置合法，表的长度减1
     void remove(int i);
     // 查找值为value的元素第一次出现的位序
     int search(const elemType &value) const ;
     // 访问位序为i的元素值，“位序”0表示第一个元素，类似于数组下标
     elemType visit(int i) const;
     // 遍历顺序表
     void traverse() const;
     // 逆置顺序表
     void inverse();
     bool Union(seqList<elemType> &B);
 };

#endif /* seqList_h */
