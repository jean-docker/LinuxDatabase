#include <iostream>
#include "dbrecord.h"
#include "bplustree.h"
#include "work.h"
using namespace std;
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
int main()
{
    DBRecord testRecord;
    Record record, *record_array=NULL;

    int NUMBER = 10;
//    Record record_array[NUMBER];


    BPlusTreeNode *b_plus_tree=NULL;
    BPlusTree testBPlusTree;

//    testRecord.createRecord((record), 0);
//    cout << record.primary_key << endl;
//    for(int i = 0;i < 100; i++){
//        cout << record.non_primary_array[i]<<" |";
//    }
//    testRecord.appendRecord(testRecord.getFilePath(), record);

//        testRecord.createRecordArray(&(record), 0, 1);
//        cout << record.primary_key << endl;
//        for(int i = 0;i < 100; i++){
//            cout << record.non_primary_array[i]<<" |";
//        }

    record_array = new Record[NUMBER];

//    testRecord.createRecordArray(record_array, 0, NUMBER);
//    cout << "create end"<<endl;
//    for(int j = 0; j < NUMBER; ++j) {
//        cout << "primari_key: "<<(record_array+j)->primary_key<<" # ";
//        for(int i = 0;i < 5; i++){
//            cout << (record_array+j)->non_primary_array[i]<<"| ";
//        }
//        cout << endl;
//    }


//    cout << "begin read"<<endl;
    int num = testRecord.readRecordArray(testRecord.getFilePath(), record_array, NUMBER);
//    cout << "end read"<<endl;

//    for (int i = 0;i<NUMBER;++i) {
//        cout<<"primary_key:"<<record_array[i].primary_key<<" | ";
//        for (int j=0;j<5;++j) {
//            cout<<record_array[i].non_primary_array[j]<<", ";
//        }
//        cout<<endl;
//    }

        for (int i = 0;i<NUMBER;++i) {
            cout<<record_array[i].non_primary_array[1]<<" ";
        }
        cout<<endl;

//    for(int i=0;i<num;++i){
//        cout << record_array[i].primary_key<<" |";
//    }
    cout << "--------------------------------"<<endl;
//    if(testRecord.readRecordByPrimaryKey(testRecord.getFilePath(), record, 1)){
//        cout << "readRecordByPrimaryKey success!"<<endl;
//    }

//    if(testRecord.appendRecordArray(testRecord.getFilePath(), record_array, NUMBER)){
//        cout << "records save success!"<<endl;
//    }

//    cout << "begin create tree"<<endl;
    b_plus_tree = testBPlusTree.createBPlusTree(record_array, NUMBER, 1);
//    cout << "end create tree"<<endl;

    show(b_plus_tree, 1, 1);

//    for (int i=0;i<b_plus_tree->index_node_size;++i) {
//        cout<<b_plus_tree->index_nodes[i].value<<" ";
//    }


    int64_t key_set[SEARCH_RESULT_SET_MAX_SIZE];
    int count = 0;
//    testBPlusTree.searchValueEqual(b_plus_tree, key_set, count, 25757);
//    for(int i=0; i<count; ++i){
//        cout<<" " <<key_set[i];
//    }
//    cout<<endl;


//    count = 0;
//    testBPlusTree.searchValueRange(b_plus_tree, key_set, count, 0, 5000);
//    for(int i=0; i<count; ++i){
//        cout<<" " <<key_set[i];
//    }
//    cout<<endl;

//    count = 0;
//    testBPlusTree.searchValueGreaterOrEqual(b_plus_tree, key_set, count, 1000);
//    for(int i=0; i<count; ++i){
//        cout<<" " <<key_set[i];
//    }
//    cout<<endl;

//    count = 0;
//    testBPlusTree.searchValueLessOrEqual(b_plus_tree, key_set, count, 29000);
//    for(int i=0; i<count; ++i){
//        cout<<" " <<key_set[i];
//    }
//    cout<<endl;

//    testBPlusTree.writeBPlusTree("./index", b_plus_tree, 1);
    BPlusTreeNode *p = testBPlusTree.readBPlusTree("./index", 1);
    cout <<"read B+ tree"<<endl;
    show(p, 1, 1);
    BPlusTree::deleteBPlusTree(p);


    DBRecord::deleteRecords(record_array);
    BPlusTree::deleteBPlusTree(b_plus_tree);

    Work *my_work = new Work();
    my_work->do_work();

    return 0;
}
