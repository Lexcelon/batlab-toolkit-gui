#include "bldataobject.h"

batlabDataObject::batlabDataObject()
{
    data = (float*)_mm_malloc(1000*sizeof(float),16);
    size = 1000;

    for(int i = 0; i < 1000; i++) {
        data[i] = 0.0f;
    }
}

batlabDataObject::batlabDataObject(float * loc, int val)
{
    data = loc;
    size = val;
}


batlabDataObject::~batlabDataObject()
{

}

