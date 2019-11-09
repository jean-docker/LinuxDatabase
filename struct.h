/**
  *定义相关数据结构
  * */
#ifndef STRUCT_H
#define STRUCT_H
#include<iostream>
#include <string>

using namespace std;
#define RECORD_LENGTH   100     //每条记录属性个数
//#define int64   int         //后续删除
#define RECORD_SIZE_BYTE    (sizeof(int64_t)*(RECORD_LENGTH+1))
#define B_PLUS_TREE_INDEX_NODE_SIZE   4

/**
  *文件数据记录结构、单条记录
  * */
typedef struct Record {
    int64_t primary_key;                        //主键
    int64_t non_primary_array[RECORD_LENGTH];   //属性数组
}Record;

/**
  *索引记录节点信息结构
  * */
typedef struct IndexNode {
    int64_t primary_key;    //主键
    int64_t value;          //属性值
}IndexNode;

/**
  *B+树结构
  * */
typedef struct BPlusTreeNode{
    IndexNode index_nodes[B_PLUS_TREE_INDEX_NODE_SIZE];              //B+树阶数 B_PLUS_TREE_INDEX_NODE_SIZE -1
    int index_node_size;                                              //实际存储节点数
    struct BPlusTreeNode *childs[B_PLUS_TREE_INDEX_NODE_SIZE+1];    //子节点数组
    int child_size;                                                 //子节点数
    bool is_leaf;
    struct BPlusTreeNode *prev;
    struct BPlusTreeNode *next;
}BPlusTreeNode;



#endif // STRUCT_H
