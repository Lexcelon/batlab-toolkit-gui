#include "batlabcell.h"

batlabCell::batlabCell()
{

}

batlabCell::batlabCell(uchar key)
{
    unit = key >> 2;
    cell = key & 0x03;
}

batlabCell::~batlabCell()
{

}

