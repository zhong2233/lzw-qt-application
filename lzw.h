#ifndef LZW_H
#define LZW_H
#endif
#pragma once
#include "bitio.h"
#include <iostream>
#include <fstream>
#define MAX_size 65535
using namespace std;

typedef unsigned char uint1;
typedef unsigned short uint2;
typedef unsigned int uint4;

bool LZWencode(ifstream& file_in, BitFileOut& bit_out);
bool LZWdecode(ofstream& file_out, BitFileIn& bit_in);

class LZW_Dictionary
{
public:
    LZW_Dictionary();
    ~LZW_Dictionary();
    void initDictionary();
    int InDict(uint1 character, int string_code);
    void AddtoDict(uint1 character, int string_code);
    void PrintDict();
    int get_next_code();
    int DecodeString(int code, int last_code, int start = 0);

    uint1* d_stack;

private:
    typedef struct {
        int suffix;
        int parent;
        int firstchild;
        int nextsibling;
    }node;
    node* dict;
    uint4 next_code;
};
