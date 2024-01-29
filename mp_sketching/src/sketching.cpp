#include "sketching.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

template <typename T>
void sortvector(std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); i++) { // use bubble sort to arrange the elements in the vector
        for (size_t j = 0; j < vec.size() - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

std::string binarytostring(uint64_t number) { // convert binary to string
    std::string bstring = ""; 
    for (int j = 63; j >= 0; j--) {
        if (number & (1ULL << j)) {
            bstring += '1';
        } else {
            bstring += '0';
        }
    }
    return bstring;
}


std::vector<uint64_t> kminhash(std::vector<int> inList, unsigned k, hashFunction h) {
    std::set<uint64_t> storehash2; // change to use set to avoid timeout
    std::vector<uint64_t> storehash;

    for (size_t i = 0; i < inList.size(); i++){
        storehash2.insert(h(inList[i])); // put hash function on every element and put it into new vector
    }

    //sortvector(storehash); // sort the values in the vector from smallest to largest


    std::vector<uint64_t> removedup; // create a new vector for removing duplicates
    // for (size_t i = 0; i < storehash.size(); i++){ // iterate through the vector
    //     if (i == 0 || storehash[i] != storehash[i-1]){ // if it's the first element or the current and next element are not the same, add it into the vector
    //         removedup.push_back(storehash[i]);
    //     }
    // }


    for (auto it = storehash2.begin(); it != storehash2.end(); it++){
        removedup.push_back(*it);
    }
    
    std::vector<uint64_t> final; // create another vector to store final results
    for (size_t i = 0; i < k; i++){ // iterate to the kth value
        if (i < removedup.size()){ // check that we don't exceed the size
            final.push_back(removedup[i]);
        }
        else{ // if there are not enough elements, fill it with global max int
            final.push_back(GLOBAL_MAX_INT);
        }
    }

    return final;

}


std::vector<uint64_t> khash_minhash(std::vector<int> inList, std::vector<hashFunction> hv) {
    
    std::vector<uint64_t> storehash(hv.size(), GLOBAL_MAX_INT); // create vector
    // for (size_t i = 0; i < hv.size(); i++){
    //     storehash.push_back(GLOBAL_MAX_INT);
    // }

    for (size_t j = 0; j < inList.size(); j++){ // apply hash functions to each element
        for (size_t k = 0; k < hv.size(); k++){
            uint64_t hashval = hv[k](inList[j]);
            if (storehash[k] > hashval){ // choose only the minimum
                storehash[k] = hashval;
            }
        }
    }

    return storehash;

}


std::vector<uint64_t> kpartition_minhash(std::vector<int> inList, int part_bits, hashFunction h) {
    std::vector<uint64_t> partivec(pow(2, part_bits), GLOBAL_MAX_INT);

    for (size_t i = 0; i < inList.size(); i++) { // apply hash functions and convert binary to string
        uint64_t hashval = h(inList[i]);
        std::string hashStr = binarytostring(hashval);

        uint64_t parti = 0;
        for (int j = 0; j < part_bits; ++j) { 
            parti <<= 1; // left shift by one
            if (hashStr[j] == '1') { // if current bit == 1
                parti = parti | 1; // apply or operation
            }
        }

        if (hashval < partivec[parti]) { // get the min value
            partivec[parti] = hashval;
        }
    }

    return partivec;
}


float minhash_jaccard(std::vector<uint64_t> mh1, std::vector<uint64_t> mh2) {
    
    mh1.erase(std::remove(mh1.begin(), mh1.end(), GLOBAL_MAX_INT), mh1.end()); 
    mh2.erase(std::remove(mh2.begin(), mh2.end(), GLOBAL_MAX_INT), mh2.end());

    // sortvector(mh1);
    // sortvector(mh2);

    // for (size_t i = 0; i < mh1.size(); i++) {
    //     for (size_t j = 0; j < mh2.size(); j++) {
    //         if (mh1[i] != mh2[j] || mh1[i] == GLOBAL_INT_MAX || mh2[j] == GLOBAL_INT_ MAX){
    //             intersect.push_back(mh1[i]);
    //         }
    //     }
    //     if (mh1[i] == mh2[i]){
    //         break;
    //     }
    // }

    std::vector<uint64_t> intersect;
    for (size_t i = 0; i < mh1.size(); i++) {
        for (size_t j = 0; j < mh2.size(); j++) {
            if (mh1[i] == mh2[j]) { // check for intersection
                if (intersect.empty() || intersect.back() != mh1[i]) { // check for duplicate
                    intersect.push_back(mh1[i]);
                }
                break;
            }
        }
    }
    
    return static_cast<float>(intersect.size()) / static_cast<float>(mh1.size() + mh2.size() - intersect.size()); // find size of union from intersection=, formula provided from lecture slides
}




int minhash_cardinality(std::vector<uint64_t> mh, unsigned k) {
    return static_cast<int>(std::ceil(((static_cast<double>(k) / (static_cast<double>(mh[k - 1]) / static_cast<double>(GLOBAL_MAX_INT))) - 1))); // formula from lecture k/N+1
}


float exact_jaccard(std::vector<int> raw1, std::vector<int> raw2) {
    
    sortvector(raw1);
    sortvector(raw2);

    raw1.erase(std::unique(raw1.begin(), raw1.end()), raw1.end());
    raw2.erase(std::unique(raw2.begin(), raw2.end()), raw2.end());

     // for (size_t i = 0; i < mh1.size(); i++) {
    //     for (size_t j = 0; j < mh2.size(); j++) {
    //         if (mh1[i] != mh2[j]){
    //             intersect.push_back(mh1[i]);
    //         }
    //     }
    //     if (mh1[i] == mh2[i]){
    //         break;
    //     }
    // }


    std::vector<int> intersect;
    for (size_t i = 0; i < raw1.size(); i++) { // loop through every element
        for (size_t j = 0; j < raw2.size(); j++) {
            if (raw1[i] == raw2[j]) { // check intersection
                if (intersect.empty() || intersect.back() != raw1[i]) { // avoid duplicate
                    intersect.push_back(raw1[i]);
                }
                break;
            }
        }
    }

    return static_cast<float>(intersect.size()) / static_cast<float>(raw1.size() + raw2.size() - intersect.size()); // used formula for union provided from the slides
}



int exact_cardinality(std::vector<int> raw) {

    sortvector(raw);
    
    int count = 0;
    for (size_t i = 1; i < raw.size(); i++){
        if (raw[i] != raw[i-1]){
            count++;
        }
    }

    return count + 1;
}

unsigned tilesize(unsigned size, unsigned numTiles){ //calculate the size of tile
    unsigned tsize = size / numTiles;
    if (size % numTiles != 0){ // conditional statement to include additional pixels if there are remainders
        tsize = tsize + 1;
    }
    return tsize;
}


MM::MM(const cs225::PNG& input, unsigned numTiles, unsigned k, hashFunction h) {
    tilevector.resize(numTiles * numTiles); // resize the vector to be able to store 
    numTiles2 = numTiles; // declare numTiles in the constructor to be able to use it at getminhash

    auto Wtile = tilesize(input.width(), numTiles); // call the helper function to get the width of tile
    auto Htile = tilesize(input.height(), numTiles); // same but for height

    unsigned y = 0;
    while (y < numTiles) {
        unsigned x = 0;
        while (x < numTiles) {
            std::vector<int> lval; // create a vector to store luminance value
            unsigned startX = x * Wtile; // get the starting position
            unsigned lastX = (x + 1) * Wtile; // ending position at the beginning of next tile
            if (lastX > input.width()) { // if next tile exceeds the bound, make the end bound equal to the last one
                lastX = input.width();
            }

            while (startX < lastX) {
                //vector<int> storepixel;
                //int getpixel = static_cast<int>(pixel.h * 360 + pixel.s * 100 + pixel.l * 100);
                //storepixel.push_back(getpixel);
                unsigned startY = y * Htile;
                unsigned lastY = (y + 1) * Htile;
                if (lastY > input.height()) {
                    lastY = input.height();
                }

                while (startY < lastY) {
                    lval.push_back((int)(input.getPixel(startX, startY).l * 255)); // times 255 for luminous range and push into vector
                    startY++;
                }
                startX++;
            }
            // for (int a = 0; a < k; a++){
            //     auto temp = storepixel[a];
            //     storepixel.pop_back();
            // }
            tilevector[y * numTiles + x] = kminhash(lval, k, h); // apply minhash at position x y of the tile
            x++;
        }
        y++;
    }
}




std::vector<uint64_t> MM::getMinHash(unsigned width, unsigned height) const {
    return tilevector[width+height*numTiles2]; // get the position of vector and x y
}


int MM::countMatchTiles(const MM & other, float threshold) const {
    
    int count = 0;
    for (unsigned j = 0; j < numTiles2; j++) { // loop through width and height
        for (unsigned i = 0; i < numTiles2; i++) {
            std::vector<uint64_t> thishash = getMinHash(i, j); // get the hash of this one we want to compare
            std::vector<uint64_t> otherhash = other.getMinHash(i, j); // get the hash of the other that we want to compare with

            auto similarity = minhash_jaccard(thishash, otherhash); // apply minhash_jaccard
            if (similarity >= threshold) { // add count if they are similar more than a threshold
                count++;
            }
        }
    }

    return count;
}


MM getpng(const std::string& filePath, unsigned numTiles, unsigned k, hashFunction h) { // helper function to read from file
    cs225::PNG png;
    png.readFromFile(filePath);
    return MM(png, numTiles, k, h);
}


std::vector<std::tuple<int, int, int>> build_minhash_graph(std::vector<std::string> flist, unsigned numTiles, unsigned k, hashFunction h, float threshold) {
    std::vector<std::tuple<int, int, int>> graphtup; // vector tuple for edges of graph

    for (int i = 0; i < static_cast<int>(flist.size()); i++) { // first loop
        MM pic1 = getpng(flist[i], numTiles, k, h);

        for (int j = i + 1; j < static_cast<int>(flist.size()); j++) { // i+1 to loop the next
            MM pic2 = getpng(flist[j], numTiles, k, h);

            int temp = pic1.countMatchTiles(pic2, threshold); // compare
            std::tuple<int, int, int> arm(i, j, temp);
            graphtup.push_back(arm); // add edge to graph
        }
    }
    return graphtup;
}

