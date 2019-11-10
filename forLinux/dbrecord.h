#ifndef DBRECORD_H
#define DBRECORD_H
#include "struct.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <fstream>
/**
 * @brief 文件记录操作（产生随机数据记录、读取、保存数据）
 */

class DBRecord
{
public:
    static void deleteRecords(Record *record_array);
    static DBRecord *newRecords(int number);
public:
    DBRecord();
    ~DBRecord();

    //产生一条随机数据记录
    void createRecord(Record *my_record, const int64_t &primary_key);
    //产生一组(number个)随机数据记录
    void createRecordArray(Record *record_array, int64_t primary_key, const int &number);
    //获取记录键值（数据按键值存储，返回最后一条记录键值）
    int64_t getPrimaryKeyFromFile(const std::string &file_path);
    //保存一个数据记录
    bool appendRecord(const std::string &file_path, Record &record);
    //保存数据记录数组
    bool appendRecordArray(const std::string &file_path, Record *record_array, const int &number);

    int readRecordArray(const std::string &file_path, Record *record_array, const int &number);
    bool readRecordByPrimaryKey(const std::string &file_path, Record &record, const int64_t &primary_key);


    std::string getFilePath(){ return my_file_path; }
    int64_t getLastPrimaryKey(){ return last_primary_key; }
private:
    FILE *record_file;     //数据记录文件指针
    int64_t last_primary_key; //数据最后一条记录的主键值

    std::string my_file_path;
};

#endif // DBRECORD_H
