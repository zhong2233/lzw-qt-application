#ifndef BITIO_H
#define BITIO_H

#endif // BITIO_H
#pragma once
#ifndef  __bitio__
#define  __bitio__

#include <fstream>
using namespace std;

typedef unsigned char uint1;
typedef unsigned short uint2;
typedef unsigned int uint4;

class BitFileIn: public ifstream
{
public:
    BitFileIn(const char* path);
    ~BitFileIn();
    uint1 BitInput(bool& flag);
    uint4 BitsInput(int count, bool& flag);

private:
    uint1 mask;
    uint1 rack;
};

class BitFileOut : public ofstream
{
public:
    BitFileOut(const char* path);
    ~BitFileOut();
    void BitOutput(uint1 bit);
    void BitsOutput(uint4 code, uint4 count);
    void close();
private:
    uint1 mask;
    uint1 rack;
};

#endif // ! __bitio__
