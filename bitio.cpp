#include "bitio.h"
#include <iostream>
using namespace std;

BitFileIn::BitFileIn(const char* path): ifstream(path,ios::binary)
{
    mask = 0x80;
    rack = 0;
}

BitFileIn::~BitFileIn()
{
    ifstream::close();
}

uint1 BitFileIn::BitInput(bool& flag)
{
    //Using flag to mark reaching the end
    int value;
    if (mask == 0x80) {
        this->read(reinterpret_cast<char*>(&rack), 1);
        if (this->eof()) {  //if reach end
            cerr << __func__ << "\tRead after the end!" << endl;
            system("pause");
            flag = true;  //indicate reach the end
            return 0;
        }
    }
    value = mask & rack;
    mask >>= 1;
    if (mask == 0) { mask = 0x80; }
    return ((value)?1:0);
}

uint4 BitFileIn::BitsInput(int count, bool& flag)
{
    uint4 mask = 1L << (count - 1);
    uint4 value = 0L;
    while (mask != 0) {
        if (this->BitInput(flag)) { value |= mask; }
        mask >>= 1;
    }
    return value;
}

BitFileOut::BitFileOut(const char* path): ofstream(path, ios::binary)
{
    mask = 0x80;
    rack = 0;
}

BitFileOut::~BitFileOut()
{
    this->close();
}

void BitFileOut::BitOutput(uint1 bit)
{
    if (bit != 0) { rack |= mask; }
    mask >>= 1;
    if (mask == 0) {
        this->write(reinterpret_cast<const char*>(&rack), 1);
        //*(this) << rack;
        rack = 0;
        mask = 0x80;
    }
}

void BitFileOut::BitsOutput(uint4 code, uint4 count)
{
    uint4 mask = 1 << (count - 1);
    while (mask != 0) {
        this->BitOutput((code & mask) ? 1 : 0);
        mask >>= 1;
    }
}

void BitFileOut::close()
{
    //output remaining bits
    if (mask != 0x80) {
        this->write(reinterpret_cast<const char*>(&rack), 1);
        //*(this) << rack;
    }
    ofstream::close();
}
