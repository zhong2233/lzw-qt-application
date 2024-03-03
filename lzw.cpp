#include "lzw.h"
#include <iostream>
using namespace std;

LZW_Dictionary::LZW_Dictionary()
{
    initDictionary();
}

LZW_Dictionary::~LZW_Dictionary()
{
    delete[] dict;
    delete[] d_stack;
}

void LZW_Dictionary::initDictionary()
{
    dict = new node[MAX_size + 1];
    d_stack = new uint1[MAX_size];
    for (int i = 0; i < 256; i++) {
        dict[i].suffix = i;
        dict[i].parent = -1;
        dict[i].firstchild = -1;
        dict[i].nextsibling = i + 1;
    }
    dict[255].nextsibling = -1;
    next_code = 256;
}

int LZW_Dictionary::InDict(uint1 character, int string_code)
{
    //-1代表string_code为空
    int sibling;
    if (string_code < 0) { return character; }
    sibling = dict[string_code].firstchild;

    while (sibling > -1) {
        if(dict[sibling].suffix == character){
            return sibling;  //查找所以子串
        }
        sibling = dict[sibling].nextsibling;  //子串的下一个兄弟.
    }
    return -1;  //字典中没有
}

void LZW_Dictionary::AddtoDict(uint1 character, int string_code)
{
    int firstsibling, nextsibling;
    if (0 > string_code) return;
    dict[next_code].suffix = character;
    dict[next_code].parent = string_code;
    dict[next_code].firstchild = -1;
    dict[next_code].nextsibling = -1;
    firstsibling = dict[string_code].firstchild;
    if (-1 < firstsibling) {	// the parent has child
        nextsibling = firstsibling;
        while (-1 < dict[nextsibling].nextsibling)
            nextsibling = dict[nextsibling].nextsibling;  //searching the last nextsibling
        dict[nextsibling].nextsibling = next_code;
    }
    else {// no child before, modify it to be the first
        dict[string_code].firstchild = next_code;
    }
    next_code++;  //important!!!!!
}

void LZW_Dictionary::PrintDict()
{
    //print dict after index of 255
    for (int i = 256; i < next_code; i++) {
        cout << i << ":  ";
        cout << dict[i].suffix << "  ";
        cout << dict[i].parent << "  ";
        cout << dict[i].firstchild << "  ";
        cout << dict[i].nextsibling << endl;
    }
}

int LZW_Dictionary::get_next_code()
{
    return next_code;
}

int LZW_Dictionary::DecodeString(int code, int last_code, int start)
{
    int count = 0;  //using d_stack to storage string
    if (code >= next_code) {  //case ABABA, the code is made by last code and it's fitst character
        count = 1;
        while (last_code >= 0) {
            start = 1;
            d_stack[count] = dict[last_code].suffix;
            last_code = dict[last_code].parent;
            count++;
        }
        d_stack[0] = d_stack[count - 1];
    }
    else {  //normal case
        while (code >= 0) {
            d_stack[count] = dict[code].suffix;
            code = dict[code].parent;
            count++;
        }
    }
    return count;
}

bool LZWencode(ifstream& file_in, BitFileOut& bit_out)
{
    LZW_Dictionary dictionary;
    int character(0), string_code(-1), index(0);
    uint4 file_length(0);

    if (!(file_in.is_open() && bit_out.is_open())) {
        cerr << __func__ << "\t Can not open file!" << endl;
        return false;
    }
    //get the length of file and output
    file_in.seekg(0, ios::end);
    file_length = file_in.tellg();
    file_in.seekg(0, 0);
    bit_out.BitsOutput(file_length, 4 * 8);
    //encode
    for (int current_len = 0; current_len < file_length; current_len++) {
        file_in.read(reinterpret_cast<char*>(&character), 1);
        index = dictionary.InDict(character, string_code);
        if (index >= 0) {
            string_code = index;
        }
        else {
            bit_out.BitsOutput((uint2)string_code, 16);
            if (dictionary.get_next_code() < MAX_size) {
                dictionary.AddtoDict(character, string_code);
            }
            string_code = character;
        }
    }
    bit_out.BitsOutput((uint2)string_code, 16);
    return true;
}

bool LZWdecode(ofstream& file_out, BitFileIn& bit_in)
{
    if (!(file_out.is_open() && bit_in.is_open())) {
        cerr << __func__ << "\t Can not open file!" << endl;
        return false;
    }
    LZW_Dictionary dic;
    bool is_end(false);
    int new_code(0), last_code(-1);
    int word_length(0);
    int character;
    int file_length = bit_in.BitsInput(4*8,is_end);
    if (is_end) {
        cerr << __func__ << "\t the number of bits incorrect!" << endl;
        return false;
    }
    //decode
    while (file_length > 0) {
        new_code = bit_in.BitsInput(16, is_end);
        if (is_end) {
            cerr << __func__ << "\t the number of bits incorrect!" << endl;
            return false;
        }
        word_length = dic.DecodeString(new_code, last_code);
        //add to dict
        if (dic.get_next_code() < MAX_size) {
            //Addtodict() will handle the case ABABA
            dic.AddtoDict(dic.d_stack[word_length - 1], last_code);
        }
        //out put bit stream to file
        while (0 < word_length) {
            word_length--;
            file_out.write(reinterpret_cast<const char*>(dic.d_stack + word_length), 1);
            file_length--;
        }
        last_code = new_code;
    }
    return true;
}

