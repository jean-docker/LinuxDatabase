#include <iostream>
#include "dbrecord.h"
#include "bplustree.h"
#include "work.h"
using namespace std;

int main()
{
    Work *my_work = new Work();
    my_work->do_work();

	delete my_work;
    return 0;
}
