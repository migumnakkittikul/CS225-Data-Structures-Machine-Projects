/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"
 #include <iostream>

BF::BF(uint64_t size, std::vector<hashFunction> hashList) : hv(hashList), bv(size, false){

    // Your code here 
}

BF::BF(const BF& other) : hv(other.hv), bv(other.bv.size()) {
    for (size_t i = 0; i < other.bv.size(); i++) {
        bv[i] = other.bv[i];
    }
}


BF::~BF(){
    //return;
}

void BF::add(const int & key){

    // Your code here 
    for (size_t i = 0; i < hv.size(); i++) {
        int hv2 = hv[i](key) % bv.size();
        bv[hv2] = 1;
    }
}

bool BF::contains(const int& key) const{
    // Your code here 

    //return false;
    for (size_t i = 0; i < hv.size(); i++) {
        int hv2 = hv[i](key) % bv.size();
        if (!bv[hv2]) {
            return false;
        }
    }
    return true;
}

void BF::bit_union(const BF& other){
    // Your code here 
    for (size_t i = 0; i < bv.size(); i++) {
        bv[i] = bv[i] || other.bv[i];
    }
}

void BF::intersect(const BF& other){
    // Your code here 
    for (size_t i = 0; i < bv.size(); i++) {
        bv[i] = bv[i] && other.bv[i];
    }
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here

    //return 9999.999;
    BF filter(size, hashList);
    for (size_t i = 0; i < inList.size(); i++) {
        filter.add(inList[i]);
    }
    
    int temp = 0;
    int notinlist = 0;

    for (int i = 0; i < static_cast<int>(max); i++) {
        bool isInList = false;
        for (size_t j = 0; j < inList.size(); j++) {
            if (inList[j] == i) {
                isInList = true;
                break;
            }
        }
        if (!isInList) {
            notinlist++;
            if (filter.contains(i)) {
                temp++;
            }
        }
    }

    if (notinlist > 0) {
        return static_cast<float>(temp) / notinlist;
    } else {
        return 0.0f;
    }

}

bool getBitFromArray(std::vector<char> bv, int index){
    // Your code here


    //return 'a';
    int byteIndex = index / 8;
    int bitIndex = 7 - (index % 8);
    return (bv[byteIndex] >> bitIndex) & 1;
}

bool getBitFromByte(char in, int index){
    // Your code here


    //return true;
    return (in >> (7 - index)) & 1;

}

