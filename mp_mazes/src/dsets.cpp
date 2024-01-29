#include "dsets.h"
#include <iostream>
#include <vector>


void DisjointSets::addelements(int num){ //fill the vector with -1 up until the specified index
    // root.resize(num);
    // root[num-1] = -1;
    for (int i = 0; i < num; i++){ // use for loop to set the elements to -1 throughout the vector
        root.push_back(-1);
    }
}


int DisjointSets::find(int elem){
    if(root[elem] < 0){ // base case for recursion, when the root element is found (root elemet is negative)
        return elem;
    }
    else{
        root[elem] = find(root[elem]); // use recursion to loop until found the root element, try to flatten the tree
        return root[elem];
    }
}


void DisjointSets::setunion(int a, int b){
    int roota = find(a); // determine the roots of tree a and b
    int rootb = find(b);

    if(size(roota) >= size(rootb)){ // if tree a is bigger, add tree b to tree a
        root[roota] = -1 * (abs(root[roota]) + abs(root[rootb])); // update the size of the set
        root[rootb] = roota; // link root b to root a
    }
    else{ // the same thing but tree b is bigger
        root[rootb] = -1 *(abs(root[roota]) + abs(root[rootb]));
        root[roota] = rootb;
    }

}


int DisjointSets::size(int elem){
    // int temp = root[elem];
    // return -1 * root[temp];
    return -1 * root[find(elem)]; // get the element at index of the root for size
}


bool DisjointSets::sameset(int a, int b){
    if (find(a) == find(b)){
        return true;
    }
    else{
        return false;
    }

}