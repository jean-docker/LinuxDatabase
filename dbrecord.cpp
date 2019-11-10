#include "dbrecord.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <fstream>

void DBRecord::deleteRecords(Record *record_array){
    if(NULL == record_array)
        return;
    delete [] record_array;
    record_array = NULL;
}

DBRecord::DBRecord()
{
    record_file = NULL;
    last_primary_key = 0;
    my_file_path = "./record_file.bat";
    srand((unsigned)time(NULL));    //随机
}
DBRecord::~DBRecord(){
    if(record_file)
        delete record_file;
}

/**
 * @brief 产生一条随机数据记录
 * @param my_record
 * @param primary_key
 */
void DBRecord::createRecord(Record *my_record, const int64_t &primary_key){
    my_record->primary_key = primary_key + 1;
//    srand((unsigned)time(NULL));
//    std::cout<<my_record.primary_key<<" ";
    for (int index = 0; index < RECORD_LENGTH; ++index) {
        my_record->non_primary_array[index] = rand();    //随机数
//        std::cout<<my_record->non_primary_array[index]<<"-";
    }
//    std::cout<<std::endl;
}

/**
 * @brief 产生一组随机数据记录
 * @param record_array
 * @param primary_key
 * @param number
 */
void DBRecord::createRecordArray(Record *record_array, int64_t primary_key, const int &number){
//    number = sizeof (*record_array) / sizeof (Record);
    Record *p_record = record_array;
    for (int index = 0; index < number; ++index) {
        createRecord(p_record, primary_key);
        primary_key += 1;
//        std::cout<<p_record->primary_key<<" ";
//        for (int j=0;j<RECORD_LENGTH;++j) {
//            std::cout<<p_record->non_primary_array[j]<<" ";
//        }
//        std::cout<<std::endl;
        p_record++;
    }
}

/**
 * @brief 获取记录键值（数据按键值存储，返回最后一条记录键值）
 * @param file_path
 * @return
 */
int64_t DBRecord::getPrimaryKeyFromFile(const std::string &file_path){
    std::fstream fs(file_path, std::fstream::in | std::fstream::binary);
    if(!fs.is_open()){
        last_primary_key = 0;
        return 0;
    }
    //文件最后一条记录
    fs.seekg(0, fs.end);
    last_primary_key = fs.tellg();
    fs.close();

    return last_primary_key;
}

//保存数据记录
bool DBRecord::appendRecord(const std::string &file_path, Record &record){
    std::fstream fs(file_path, std::fstream::out | std::fstream::binary | std::fstream::app);
    if(fs.is_open()){
        std::cout << "papendRecord open file success!"<<std::endl;
        fs.write((char*)(&record), RECORD_SIZE_BYTE);
        fs.close();
        last_primary_key = record.primary_key;
        return true;
    }
    return false;
}

//保存数据记录数组
bool DBRecord::appendRecordArray(const std::string &file_path, Record *record_array, const int &number){
    std::fstream fs(file_path, std::fstream::out | std::fstream::binary | std::fstream::app);
    if(fs.is_open()){
        int index=0;
        for ( ;index<number; ++index) {
            fs.write((char*)(record_array+index), RECORD_SIZE_BYTE);
        }
        fs.close();
        last_primary_key = record_array[index].primary_key;
    }
    return true;
}


int DBRecord::readRecordArray(const std::string &file_path, Record *record_array, const int &number){
    if(number <= 0)
        return false;
    std::fstream fs(file_path, std::fstream::in | std::fstream::binary);;
    if(!fs.is_open()){
        std::cout << "readRecordArray open file error!!!"<<std::endl;
        return false;
    }
//    record_array = new Record[number]();
    int index=0;
    while(index<number) {
        fs.read((char*)(record_array+index), RECORD_SIZE_BYTE);
        if(fs.eof())
            break;  //防止多读一条无效记录
//        std::cout<<"read one record: "<<record_array[index].primary_key<<std::endl;
//        for(int i = 0;i < 100; i++){
//            std::cout << record_array[index].non_primary_array[i]<<" |";
//        }
//        std::cout<<std::endl;
        ++index;
    }
    fs.close();
    return index;
}

bool DBRecord::readRecordByPrimaryKey(const std::string &file_path, Record &record, const int64_t &primary_key){
    if(primary_key < 1)
        return false;
    std::fstream fs(file_path, std::fstream::in | std::fstream::binary);
    if(!fs.is_open()){
        std::cout << "readRecordArray open file error!!!"<<std::endl;
        return false;
    }
    //相对文件开始位置偏移 RECORD_SIZE_BYTE*(primary_key-1) 个字节
    fs.seekg(RECORD_SIZE_BYTE*(primary_key-1), fs.beg);
    fs.read((char*)(&record), RECORD_SIZE_BYTE);
    fs.close();
    return true;
}
