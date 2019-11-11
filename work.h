#ifndef WORK_H
#define WORK_H
#include <iostream>
#include <fstream>
#include "struct.h"
#include "dbrecord.h"
#include "bplustree.h"

using namespace std;
class Work
{
public:
    Work();
    ~Work();

    void do_work();
    int insertOrSearch();

    int search();
    void searchEqual();
    void searchLess();
    void searchGreater();
    void searchBetween();

    int insertOneOrArray();

    void insertOneRecord();
    void insertArray();

    //显示记录
    void showRecord(const Record &record);
    void showRecordArray(Record *record_array, int number);

    //创建索引 B+树
    BPlusTreeNode *createBPlusTree(string db_file_path, int col);

private:
    DBRecord *my_DBrecord;
    BPlusTree *my_B_plus_tree;

    string db_file_path;
    string index_file_path;
};

#endif // WORK_H
