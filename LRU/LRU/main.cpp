//
//  main.cpp
//  LRU
//
//  Created by wejudging on 2018/11/25.
//  Copyright © 2018 wejudging. All rights reserved.
//

#include <iostream>
#include  "main.h"
using namespace std;

int LRUCache::getValue(int key)
{
    CacheNode *p=p_cache_list_head->next;
    while(p->next!=NULL)
    {
        if(p->key == key) //catch node
        {
            detachNode(p);
            addToFront(p);
            return p->value;
        }
        p=p->next;
    }
    return -1;
}



bool LRUCache::putValue(int key,int value)
{
    CacheNode *p=p_cache_list_head->next;
    while(p->next!=NULL)
    {
        if(p->key == key)
        {
            p->value=value;
            getValue(key);
            return true;
        }
        p=p->next;
    }
    
    if(cache_real_size_ >= cache_size_)
    {
        cout << "释放" <<endl;
        p=p_cache_list_near->pre->pre;
        delete p->next;
        p->next=p_cache_list_near;
        p_cache_list_near->pre=p;
    }
    
    p=new CacheNode();//(CacheNode *)malloc(sizeof(CacheNode));
    
    if(p==NULL)
        return false;
    
    addToFront(p);
    p->key=key;
    p->value=value;
    
    cache_real_size_++;
    
    return true;
}


void LRUCache::displayNodes()
{
    CacheNode *p=p_cache_list_head->next;
    
    while(p->next!=NULL)
    {
        cout << " Key : " << p->key << " Value : " << p->value << endl;
        p=p->next;
        
    }
    cout << endl;
    
}


int main(int argc, const char * argv[]) {
    LRUCache a(3);
    a.putValue(1, 1);
    a.putValue(2, 2);
    a.putValue(3, 3);
    a.putValue(4, 4);
    cout<<a.getValue(1)<<endl;
    cout<<a.getValue(2)<<endl;
    cout<<a.getValue(3)<<endl;
    cout<<a.getValue(4)<<endl;
    a.displayNodes();
    return 0;
}
