/**
  *定义相关数据结构
  * */
#ifndef STRUCT_H
#define STRUCT_H
#include<stdio.h>
#include <string>

#define RECORD_LENGTH   100     //每条记录属性个数
//#define int64   int         //后续删除
#define RECORD_SIZE_BYTE    (sizeof(int64_t)*(RECORD_LENGTH+1))

typedef struct Record {
    int64_t primary_key;
    int64_t non_primary_array[RECORD_LENGTH];
}Record;



#endif // STRUCT_H
