#include <iostream>
#include "dbrecord.h"
using namespace std;

int main()
{
    DBRecord testRecord;
    Record record, *record_array=NULL;
    int NUMBER = 1000000;

//    testRecord.createRecord((record), 0);
//    cout << record.primary_key << endl;
//    for(int i = 0;i < 100; i++){
//        cout << record.non_primary_array[i]<<" |";
//    }
//    testRecord.appendRecord(testRecord.getFilePath(), record);

    record_array = new Record[NUMBER] ();
    testRecord.createRecordArray(record_array, 0, 0);
    cout << "create end"<<endl;

//    int num = testRecord.readRecordArray(testRecord.getFilePath(), record_array, 10);
//    for(int i=0;i<num;++i){
//        cout << record_array[i].primary_key<<" |";
//    }

//    if(testRecord.readRecordByPrimaryKey(testRecord.getFilePath(), record, 1)){
//        cout << "readRecordByPrimaryKey success!"<<endl;
//    }

    if(testRecord.appendRecordArray(testRecord.getFilePath(), record_array, NUMBER)){
        cout << "records save success!"<<endl;
    }


    if(record_array)
        delete [] record_array;

    return 0;
}
