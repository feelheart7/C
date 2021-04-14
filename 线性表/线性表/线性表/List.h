//
//  List.h
//  线性表
//
//  Created by weijiajin on 2021/4/9.
//

#ifndef List_h
#define List_h
using namespace std;
 ​
 class outOfRange{};
 class badSize{};
 template<class T>
 class List {
 public:
     // 清空线性表
     virtual void clear()=0;
     // 判空，表空返回true，非空返回false
     virtual bool empty()const=0;
     // 求线性表的长度
     virtual int size()const=0;
     // 在线性表中，位序为i[0..n]的位置插入元素value
     virtual void insert(int i,const T &value)=0;
     // 在线性表中，位序为i[0..n-1]的位置删除元素
     virtual void remove(int i)=0;
     // 在线性表中，查找值为value的元素第一次出现的位序
     virtual int search(const T&value)const=0;
     // 在线性表中，查找位序为i的元素并返回其值
     virtual T visit(int i)const=0;
     // 遍历线性表
     virtual void traverse()const=0;
     // 逆置线性表
     virtual void inverse()=0;
     virtual ~List(){};
 };
 ​

 /*自定义异常处理类*/

 class outOfRange :public exception {  //用于检查范围的有效性
 public:
     const char* what() const throw() {
         return "ERROR! OUT OF RANGE.\n";
     }
 };
 ​
 class badSize :public exception {   //用于检查长度的有效性
 public:
     const char* what() const throw() {
         return "ERROR! BAD SIZE.\n";
     }
 };
#endif /* List_h */
