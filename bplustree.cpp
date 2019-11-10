#include "bplustree.h"
#include<iostream>
BPlusTree::BPlusTree()
{

}
BPlusTree::~BPlusTree(){

}


BPlusTreeNode* BPlusTree::createBPlusTree(const Record *record_array,  int number, int col){
    if(number <=0 || col < 1 || col > 100 || NULL == record_array)
        return NULL;
    BPlusTreeNode *root = newBPlusNode();
    if(!root)
        return NULL;
//    std::cout<<"insert begin ";
    for (int num = 0; num < number; ++num) {
//        std::cout<<"insert "<<num<<std::endl;
        insertIndexNode(root, root, newIndexNode(record_array[num], col));
    }

    return root;
}

/**
 * @brief 创建B+树节点，默认为叶节点，索引、孩子数均为0
 * @return
 */
BPlusTreeNode* BPlusTree::newBPlusNode(){
    BPlusTreeNode *node = new BPlusTreeNode;
    if(!node)
        return NULL;
    node->is_leaf = true;
    node->next = node->prev = NULL;
    node->index_node_size = node->child_size = 0;
    return  node;
}

//插入索引节点
void BPlusTree::insertIndexNode(BPlusTreeNode* &root, BPlusTreeNode* &current_node, IndexNode *index_node){
    if(NULL == root || NULL == current_node || NULL == index_node)
        return;
    int index = -1;
    if(true == current_node->is_leaf){  //叶子节点直接插入
        index = insertIndexNodeToLeaf(current_node, index_node);
        //当前节点索引数量达到阶数，分裂
        if(B_PLUS_TREE_INDEX_NODE_SIZE == current_node->index_node_size){
            splitLeafNode(root, current_node);
        }
    }
    else {  //非叶节点  找到节点X <= 插入值，插入到X右子树
        int index = current_node->index_node_size-1;
        for(; index >= 0; --index){
            if(index_node->value >= current_node->index_nodes[index].value){
                insertIndexNode(root, current_node->childs[index+1], index_node);
                break;
            }
        }
        if(index < 0)
            insertIndexNode(root, current_node->childs[0], index_node);
    }
}


IndexNode * BPlusTree::newIndexNode(const Record &record, int col){
//    if(col < 1 || col > 100)
//        return NULL;
    IndexNode *p_index_node = new IndexNode;
    p_index_node->primary_key = record.primary_key;
    p_index_node->value = record.non_primary_array[col];

    return p_index_node;
}


/**
 * @brief 叶子节点添加一个索引节点，返回插入位置
 * @param root
 * @param index_node
 * @return
 */
int BPlusTree::insertIndexNodeToLeaf(BPlusTreeNode *&current_node, IndexNode *&index_node){
    int index = current_node->index_node_size;  //从后往前扫
    for(; index > 0; --index){
        if(current_node->index_nodes[index-1].value > index_node->value) {
            current_node->index_nodes[index].primary_key = current_node->index_nodes[index-1].primary_key;
            current_node->index_nodes[index].value = current_node->index_nodes[index-1].value;
        }
        else
            break;
    }
    current_node->index_nodes[index].value = index_node->value;
    current_node->index_nodes[index].primary_key = index_node->primary_key;
    current_node->index_node_size += 1;         //节点索引数 +1

//    std::cout<<"inserted one:"<<index_node->primary_key<<"---"<<index_node->value<<std::endl;
    return index;
}

/**
 * @brief 非叶节点上添加索引节点（不包含指向文件记录的primary_key），返回插入位置
 * @param current_node
 * @param index_node
 * @return
 */
int BPlusTree::insertIndexNodeToNonLeaf(BPlusTreeNode *&current_node, IndexNode *index_node){
    int index = current_node->index_node_size;  //从后往前扫
    for(; index > 0; --index){
        if(current_node->index_nodes[index-1].value >= index_node->value) {
            current_node->index_nodes[index].value = current_node->index_nodes[index-1].value;
        }
        else
            break;
    }
    current_node->index_nodes[index].value = index_node->value;
    current_node->index_node_size += 1;         //节点索引数 +1
    return index;
}

/**
 * @brief 查询父节点，返回父节点指针（没有返回NULL）
 * @param root
 * @param current_node
 * @return
 */
BPlusTreeNode *BPlusTree::parent(BPlusTreeNode *&root, BPlusTreeNode *&current_node, int &p_index){
    BPlusTreeNode *p_parent=NULL;
    if(root == current_node){
        p_index = -1;
        return NULL;
    }
    for (int index = 0; index < root->child_size; ++index) {
        if(root->childs[index] == current_node){
           p_index = index;
           return root;
        }
        else {
            //子节点最大值 < 查询节点最小值，即，查询节点在右侧
            if(root->childs[index]->index_nodes[root->childs[index]->index_node_size-1].value < current_node->index_nodes[0].value)
                continue;
            //子节点最小值 > 查询节点最大值，即，查询节点在左侧
            if(root->childs[index]->index_nodes[0].value > current_node->index_nodes[current_node->index_node_size-1].value) {
                break;
            }
            p_parent = parent(root->childs[index], current_node, p_index);
            if(NULL != p_parent){
                break;
            }
        }
    }
    return p_parent;
}


void BPlusTree::splitLeafNode(BPlusTreeNode *&root, BPlusTreeNode *&current_node){
    int parent_pos=-1;
    BPlusTreeNode *p_parent = parent(root, current_node, parent_pos);  //当前节点对应父节点的孩子数组位置（下标）
    int index;

std::cout<<"split parent="<<p_parent<<" pos="<<parent_pos<<std::endl;
    //当前节点分裂成两份 0~M/2-1 、 M/2 ~ M-1
    int second_index = B_PLUS_TREE_INDEX_NODE_SIZE/2;
    BPlusTreeNode *right_brother = newBPlusNode();
    while(second_index < B_PLUS_TREE_INDEX_NODE_SIZE) {
        //复制节点信息
        right_brother->index_nodes[right_brother->index_node_size].value = current_node->index_nodes[second_index].value;
        right_brother->index_nodes[right_brother->index_node_size].primary_key = current_node->index_nodes[second_index].primary_key;
        right_brother->index_node_size += 1;    //新节点索引数+1
        current_node->index_node_size -= 1;     //当前节点索引数-1

        second_index += 1;
    }
    right_brother->next = current_node->next;   //右兄弟
    right_brother->prev = current_node;     //左兄弟
    current_node->next = right_brother;

    //将分裂的新节点插入当前节点的父节点(非叶节点)
    if(NULL == p_parent){
        std::cout<<"no parent!!"<<std::endl;
        //无父节点 新建父节点
        p_parent = newBPlusNode();
        p_parent->is_leaf = false;
        p_parent->index_nodes[0].value = right_brother->index_nodes[0].value;
        p_parent->childs[0] = current_node;
        p_parent->childs[1] = right_brother;
        p_parent->index_node_size = 1;
        p_parent->child_size = 2;
        root = p_parent;

    }else {
        //存在父节点
        index = p_parent->index_node_size;
        std::cout<<"parent index="<<index<<std::endl;
        for(; index > parent_pos; --index){
            p_parent->index_nodes[index].value = p_parent->index_nodes[index-1].value;  //索引值后移
            p_parent->childs[index+1] = p_parent->childs[index];                        //子节点指针后移
        }
        p_parent->index_nodes[parent_pos].value = right_brother->index_nodes[0].value;
        p_parent->childs[parent_pos+1] = right_brother;
        p_parent->index_node_size += 1;
        p_parent->child_size += 1;

//        show(root, 1, 1);
        //检查父节点插入索引后是否需要分裂
        if(p_parent->index_node_size == B_PLUS_TREE_INDEX_NODE_SIZE)
            splitNonLeafNode(root, p_parent);
    }
//    show(root, 1, 1);
}

void BPlusTree::splitNonLeafNode(BPlusTreeNode *&root, BPlusTreeNode *&current_node){
    int parent_pos=-1;
    BPlusTreeNode *p_parent = parent(root, current_node, parent_pos);  //当前节点对应父节点的孩子数组位置（下标）
    int index;

std::cout<<"nonleaf split parent="<<p_parent<<" pos="<<parent_pos<<std::endl;

    //当前节点分裂成两份 0~M/2-1 、 M/2 ~ M-1
    int second_index = B_PLUS_TREE_INDEX_NODE_SIZE/2;
    BPlusTreeNode *right_brother = newBPlusNode();
    right_brother->is_leaf = false;
    while(second_index < B_PLUS_TREE_INDEX_NODE_SIZE) {
        //复制节点信息
        right_brother->index_nodes[right_brother->index_node_size].value = current_node->index_nodes[second_index].value;
        right_brother->index_node_size += 1;    //新节点索引数+1
        current_node->index_node_size -= 1;     //当前节点索引数-1
        //孩子指针
        right_brother->childs[right_brother->child_size] = current_node->childs[second_index];
        current_node->childs[second_index] = NULL;
        right_brother->child_size += 1;
        current_node->child_size -= 1;
        second_index += 1;
    }
    right_brother->next = current_node->next;   //右兄弟
    right_brother->prev = current_node;     //左兄弟
    current_node->next = right_brother;

    //将分裂的新节点插入当前节点的父节点(非叶节点)
    if(NULL == p_parent){
        //无父节点
        p_parent = newBPlusNode();
        p_parent->is_leaf = false;
        p_parent->index_nodes[0].value = right_brother->index_nodes[0].value;
        p_parent->childs[0] = current_node;
        p_parent->childs[1] = right_brother;
        p_parent->index_node_size = 1;
        p_parent->child_size = 2;

        if(root == current_node)
            root = p_parent;
        std::cout<<"root ===="<<root->is_leaf<<std::endl;
//        show(root, 11, 1);
    }else {
        //存在父节点
        index = p_parent->index_node_size;
        for(; index > parent_pos; --index){
            p_parent->index_nodes[index].value = p_parent->index_nodes[index-1].value;  //索引值后移
            p_parent->childs[index+1] = p_parent->childs[index];                        //子节点指针后移
        }
        p_parent->index_nodes[parent_pos].value = right_brother->index_nodes[0].value;
        p_parent->childs[parent_pos+1] = right_brother;
        p_parent->index_node_size += 1;
        p_parent->child_size += 1;

        //检查父节点插入索引后是否需要分裂
        if(p_parent->index_node_size == B_PLUS_TREE_INDEX_NODE_SIZE)
            splitNonLeafNode(root, p_parent);
    }
}

//---------------------search-----------------------------------
void BPlusTree::searchValueEqual(BPlusTreeNode *&root, int64_t *key_set, int &count, const int64_t &value){
    if(NULL == root || count >= SEARCH_RESULT_SET_MAX_SIZE)
        return;
    BPlusTreeNode *p_node = root;
    if(p_node->is_leaf){    //叶节点取值
        for (int index=0; index<p_node->index_node_size; ++index) {
            if(count >= SEARCH_RESULT_SET_MAX_SIZE)
                return;
            if(p_node->index_nodes[index].value == value){
                key_set[count] = p_node->index_nodes[index].primary_key;
                count += 1;
            }else if (p_node->index_nodes[index].value > value) {
                break;
            }
        }
    }else { //非叶节点
        int index=0;
        for( ; index<p_node->index_node_size; ++index){
            if(p_node->index_nodes[index].value >= value){
                searchValueEqual(p_node->childs[index], key_set, count, value);
                break;
            }
        }
        if(index == p_node->index_node_size)
            searchValueEqual(p_node->childs[index], key_set, count, value);
        else {
            searchValueEqual(p_node->childs[index+1], key_set, count, value);
        }
    }
}

void BPlusTree::searchValueRange(BPlusTreeNode *&root, int64_t *key_set, int &count, const int64_t &min_value, const int64_t &max_value){
    if(NULL == root || count >= SEARCH_RESULT_SET_MAX_SIZE)
        return;
    BPlusTreeNode *p_node = root;
    if(p_node->is_leaf){    //叶节点取值
        for (int index=0; index<p_node->index_node_size; ++index) {
            if(count >= SEARCH_RESULT_SET_MAX_SIZE || p_node->index_nodes[index].value > max_value)
                return;
            if(p_node->index_nodes[index].value >= min_value){
                key_set[count] = p_node->index_nodes[index].primary_key;
                count += 1;
            }
        }
    }else { //非叶节点
        int index=0;
        for( ; index<p_node->index_node_size; ++index){
            if(count >= SEARCH_RESULT_SET_MAX_SIZE || p_node->index_nodes[index].value > max_value)
                break;
            if(p_node->index_nodes[index].value >= min_value){
                searchValueRange(p_node->childs[index], key_set, count, min_value, max_value);
            }
        }
        if(index == p_node->index_node_size)
            searchValueRange(p_node->childs[index], key_set, count, min_value, max_value);
        else if(p_node->index_nodes[index].value > max_value && count < SEARCH_RESULT_SET_MAX_SIZE)
            searchValueRange(p_node->childs[index], key_set, count, min_value, max_value);
    }
}

void BPlusTree::searchValueGreaterOrEqual(BPlusTreeNode *&root, int64_t *key_set, int &count, const int64_t &min_value){
    if(NULL == root || count >= SEARCH_RESULT_SET_MAX_SIZE)
        return;
    BPlusTreeNode *p_node = root;
    if(p_node->is_leaf){    //叶节点取值
        for (int index=0; index<p_node->index_node_size; ++index) {
            if(count >= SEARCH_RESULT_SET_MAX_SIZE)
                return;
            if(p_node->index_nodes[index].value >= min_value){
                key_set[count] = p_node->index_nodes[index].primary_key;
                count += 1;
            }
        }
    }else { //非叶节点
        if(p_node->index_nodes[0].value >= min_value)
            searchValueGreaterOrEqual(p_node->childs[0], key_set, count, min_value);
        int index=0;
        for(; index<p_node->index_node_size; ++index){
            if(count >= SEARCH_RESULT_SET_MAX_SIZE)
                break;
            if(p_node->index_nodes[index].value >= min_value){
                if(count == 0 && index > 0)
                    searchValueGreaterOrEqual(p_node->childs[index], key_set, count, min_value);
                searchValueGreaterOrEqual(p_node->childs[index+1], key_set, count, min_value);
            }
        }
        if(count == 0 && index == p_node->index_node_size){ //非叶节点 最后一个索引值 < min_value
            searchValueGreaterOrEqual(p_node->childs[index], key_set, count, min_value);
        }
    }
}

void BPlusTree::searchValueLessOrEqual(BPlusTreeNode *&root, int64_t *key_set, int &count, const int64_t &max_value){
    if(NULL == root || count >= SEARCH_RESULT_SET_MAX_SIZE)
        return;
    BPlusTreeNode *p_node = root;
    if(p_node->is_leaf){    //叶节点取值
        for (int index=0; index<p_node->index_node_size; ++index) {
            if(count >= SEARCH_RESULT_SET_MAX_SIZE)
                return;
            if(p_node->index_nodes[index].value <= max_value){
                key_set[count] = p_node->index_nodes[index].primary_key;
                count += 1;
            }
        }
    }else { //非叶节点
        int index=0;
        for(; index<p_node->index_node_size; ++index){
            if(count >= SEARCH_RESULT_SET_MAX_SIZE)
                break;
            if(p_node->index_nodes[index].value > max_value){
                searchValueLessOrEqual(p_node->childs[index], key_set, count, max_value);
                break;
            }
            if(p_node->index_nodes[index].value <= max_value){
                searchValueLessOrEqual(p_node->childs[index], key_set, count, max_value);
            }
        }
//        if(count == 0 && p_node->index_nodes[0].value > max_value){
//            searchValueLessOrEqual(p_node->childs[0], key_set, count, max_value);
//        }

        if(p_node->index_nodes[p_node->index_node_size-1].value <= max_value)
            searchValueLessOrEqual(p_node->childs[p_node->child_size-1], key_set, count, max_value);
    }
}

//--------------------------------------------------------
/**
 * @brief 静态方法
 * @param root
 */
void BPlusTree::deleteBPlusTree(BPlusTreeNode *&root){
    if(NULL == root)
        return;
    for(int index = 0; index < root->child_size; ++index){
        deleteBPlusTree(root->childs[index]);
    }
    delete root;
    root = NULL;
}
