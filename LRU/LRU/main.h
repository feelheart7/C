//
//  main.h
//  LRU
//
//  Created by wejudging on 2018/11/25.
//  Copyright © 2018 wejudging. All rights reserved.
//

#ifndef main_h
#define main_h


typedef struct _Node_{
    
    int key;    //键
    int value;  //数据
    
    struct _Node_ *next;  //下一个节点
    struct _Node_ *pre;   //上一个节点
    
}CacheNode;

class LRUCache{
    
public:
    
    LRUCache(int cache_size=10);  //构造函数，默认cache大小为10
    ~LRUCache();     //析构函数
    
    int getValue(int key);             //获取值
    bool putValue(int key,int value);  //写入或更新值
    void displayNodes();               //显示所有节点
    
    
private:
    
    int cache_size_;                   //cache长度
    int cache_real_size_;              //目前使用的长度
    CacheNode *p_cache_list_head;      //头节点指针
    CacheNode *p_cache_list_near;      //尾节点指针
    void detachNode(CacheNode *node);  //分离节点
    void addToFront(CacheNode *node);  //将节点插入到第一个
    
};

LRUCache::LRUCache(int cache_size)
{
    cache_size_=cache_size;
    cache_real_size_=0;
    p_cache_list_head=new CacheNode();
    p_cache_list_near=new CacheNode();
    p_cache_list_head->next=p_cache_list_near;
    p_cache_list_head->pre=NULL;
    p_cache_list_near->pre=p_cache_list_head;
    p_cache_list_near->next=NULL;
    
}

LRUCache::~LRUCache()
{
    CacheNode *p;
    p=p_cache_list_head->next;
    while(p!=NULL)
    {
        delete p->pre;
        p=p->next;
    }
    
    delete p_cache_list_near;
    
}

void LRUCache::detachNode(CacheNode *node)
{
    node->pre->next=node->next;
    node->next->pre=node->pre;
}

void LRUCache::addToFront(CacheNode *node)
{
    node->next=p_cache_list_head->next;
    p_cache_list_head->next->pre=node;
    p_cache_list_head->next=node;
    node->pre=p_cache_list_head;
}
#endif /* main_h */
