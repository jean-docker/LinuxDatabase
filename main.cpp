#include <iostream>
#include "dbrecord.h"
using namespace std;

int main()
{
    DBRecord testRecord;
    Record record, *record_array;

//    testRecord.createRecord((record), 0);
//    cout << record.primary_key << endl;
//    for(int i = 0;i < 100; i++){
//        cout << record.non_primary_array[i]<<" |";
//    }
//    testRecord.appendRecord(testRecord.getFilePath(), record);

//    record_array = new Record[1000000] ();
//    testRecord.createRecordArray(record_array, 0, 0);
//    cout << "end";

//    int num = testRecord.readRecordArray(testRecord.getFilePath(), record_array, 10);
//    for(int i=0;i<num;++i){
//        cout << record_array[i].primary_key<<" |";
//    }

    if(testRecord.readRecordByPrimaryKey(testRecord.getFilePath(), record, 1)){
        cout << "readRecordByPrimaryKey success!"<<endl;
    }
    return 0;
}
