#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "struct.h"

class BPlusTree
{
public:
    BPlusTree();
    ~BPlusTree();

public:
    static void deleteBPlusTree(BPlusTreeNode *root);


    //创建B+树
    BPlusTreeNode* createBPlusTree(const Record *record_array,  int number, int col);
    BPlusTreeNode* newBPlusNode();

    //插入索引节点
    void insertIndexNode(BPlusTreeNode *root, BPlusTreeNode *current_node, IndexNode *index_node);
    IndexNode* newIndexNode(const Record &record, int col);

    //查询

    void show(BPlusTreeNode *root, int level, int num){
        if(!root)
            return;
        cout<<"level="<<level<<",num="<<num<<"  ";
        for (int i=0;i<root->index_node_size;++i) {
            cout<<root->index_nodes[i].value<<" ";
        }
        cout<<endl<<"----------------------------------------"<<endl;

        for (int i=0;i<root->child_size;++i) {
            show(root->childs[i], level+1, i+1);
        }
    }
private:
    int insertIndexNodeToLeaf(BPlusTreeNode *current_node, IndexNode *index_node);
    int insertIndexNodeToNonLeaf(BPlusTreeNode *current_node, IndexNode *index_node);
    BPlusTreeNode *parent(BPlusTreeNode *root, const BPlusTreeNode *current_node, int &p_index);
    void splitLeafNode(BPlusTreeNode *root, BPlusTreeNode *current_node);
    void splitNonLeafNode(BPlusTreeNode *root, BPlusTreeNode *current_node);
};

#endif // BPLUSTREE_H
