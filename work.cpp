#include "work.h"

Work::Work()
{
    my_DBrecord = new DBRecord ();
    my_B_plus_tree = new BPlusTree ();
    db_file_path = "./record_file.bat";
    index_file_path = "./index";
}

Work::~Work(){
    if(my_DBrecord)
        delete my_DBrecord;
    if(my_B_plus_tree)
        delete my_B_plus_tree;
}

void Work::do_work(){
    int func;
    do{
        func = insertOrSearch();
    }while(func != 0);
}

int Work::insertOrSearch(){
    int func;
    cout << "Please input function(0 quit, 1 insert, 2 search): ";
    cin >> func;
    switch (func) {
    case 0:
        break;
    case 1:
        insertOneOrArray();
        break;
    case 2:
        search();
        break;
    default:
        insertOrSearch();   //错误输入 循环
    }
    return func;
}
//-------------------search----------------------------------------------
int Work::search(){
     int func;
    cout << "Please input(0 quit, 1 Equal, 2 Less, 3 Greater, 4 Between): ";
    cin >> func;
    switch (func) {
    case 0:
        break;
    case 1:
        searchEqual();
        break;
    case 2:
        searchLess();
        break;
    case 3:
        searchGreater();
        break;
    case 4:
        searchBetween();
        break;
    default:
        search();
    }
}

void Work::searchEqual(){
    int col;
    int64_t value;
    cout<<"input search col and value: ";
    cin>>col>>value;
    if(col < 1 || col > RECORD_LENGTH)
        return;
//    int64_t sum = my_DBrecord->getPrimaryKeyFromFile(db_file_path); //获取记录数
//    if(sum <= 0)
//        return;
//    DBRecord *record_array = new DBRecord[sum];
    //读取索引
    BPlusTreeNode *b_plus_tree = my_B_plus_tree->readBPlusTree(index_file_path, col-1);
    int count = 0;
    Record record;
    int64_t key_set[SEARCH_RESULT_SET_MAX_SIZE];
    my_B_plus_tree->searchValueEqual(b_plus_tree, key_set, count, value);
    for (int index = 0; index < count; ++index) {
        if(my_DBrecord->readRecordByPrimaryKey(db_file_path, record, key_set[index]))
            showRecord(record);
    }

    BPlusTree::deleteBPlusTree(b_plus_tree);
}

void Work::searchLess(){
    int col;
    int64_t max_value;
    cout<<"input search col and max_value: ";
    cin>>col>>max_value;
    if(col < 1 || col > RECORD_LENGTH)
        return;
    //读取索引
    BPlusTreeNode *b_plus_tree = my_B_plus_tree->readBPlusTree(index_file_path, col-1);
    int count = 0;
    Record record;
    int64_t key_set[SEARCH_RESULT_SET_MAX_SIZE];
    my_B_plus_tree->searchValueLessOrEqual(b_plus_tree, key_set, count, max_value);
    for (int index = 0; index < count; ++index) {
        if(my_DBrecord->readRecordByPrimaryKey(db_file_path, record, key_set[index]))
            showRecord(record);
    }

    BPlusTree::deleteBPlusTree(b_plus_tree);
}

void Work::searchGreater(){
    int col;
    int64_t min_value;
    cout<<"input search col and min_value: ";
    cin>>col>>min_value;
    if(col < 1 || col > RECORD_LENGTH)
        return;
    //读取索引
    BPlusTreeNode *b_plus_tree = my_B_plus_tree->readBPlusTree(index_file_path, col-1);
    int count = 0;
    Record record;
    int64_t key_set[SEARCH_RESULT_SET_MAX_SIZE];
    my_B_plus_tree->searchValueGreaterOrEqual(b_plus_tree, key_set, count, min_value);
    for (int index = 0; index < count; ++index) {
        if(my_DBrecord->readRecordByPrimaryKey(db_file_path, record, key_set[index]))
            showRecord(record);
    }

    BPlusTree::deleteBPlusTree(b_plus_tree);
}

void Work::searchBetween(){
    int col;
    int64_t min_value, max_value;
    cout<<"input search col and min_value and max_value: ";
    cin>>col>>min_value>>max_value;
    if(col < 1 || col > RECORD_LENGTH || min_value > max_value)
        return;
    //读取索引
    BPlusTreeNode *b_plus_tree = my_B_plus_tree->readBPlusTree(index_file_path, col-1);
    int count = 0;
    Record record;
    int64_t key_set[SEARCH_RESULT_SET_MAX_SIZE];
    my_B_plus_tree->searchValueRange(b_plus_tree, key_set, count, min_value, max_value);
    for (int index = 0; index < count; ++index) {
        if(my_DBrecord->readRecordByPrimaryKey(db_file_path, record, key_set[index]))
            showRecord(record);
    }

    BPlusTree::deleteBPlusTree(b_plus_tree);
}

//--------------------------------insert----------------------------------

int Work::insertOneOrArray(){
    int func;
    cout << "Please input(0 quit, 1 insert one, 2 insert array): ";
    cin >> func;
    switch (func) {
    case 0:
        break;
    case 1:
        insertOneRecord();
        break;
    case 2:
        insertArray();
        break;
    default:
        insertOneOrArray();   //错误输入 循环
    }
    return func;
}

void Work::insertOneRecord(){
//    Record *record = new Record;
    Record record;
    //产生一条记录
    my_DBrecord->createRecord(&record, my_DBrecord->getPrimaryKeyFromFile(db_file_path));
    my_DBrecord->appendRecord(db_file_path, record);

//    更新索引  XX
    showRecord(record);
}

void Work::insertArray(){
    int sum;
    cout<<"how many recors you want to insert: ";
    cin>>sum;
    if(sum<=0)
        return;
    Record *record_array = new Record[sum];
    my_DBrecord->createRecordArray(record_array, my_DBrecord->getPrimaryKeyFromFile(db_file_path), sum);
    my_DBrecord->appendRecordArray(db_file_path, record_array, sum);
    showRecordArray(record_array, sum);
    delete [] record_array;
}

//---------------show---------------------------------------
void Work::showRecord(const Record &record){
    for (int index = 0; index < RECORD_LENGTH; ++index) {
        cout<<record.non_primary_array[index]<<"| ";
    }
    cout<<endl;
}
void Work::showRecordArray(Record *record_array, int number){
    if(record_array == NULL)
        return;
    for(int count = 0; count < number; ++count){
        showRecord(*(record_array+count));
    }
}
