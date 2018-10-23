#include <iostream>
#include "FFunction.h"
#include "HashTable.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    int d = 2;
    int k = 4;
    int tableSize = 10;
    int r[]= {1,1,1,1};
    int w = 4;
    FFunction<int> fFunction(d,k,tableSize,r,w);
    std::vector<int> vector0;
    vector0.push_back(1);
    vector0.push_back(2);

    std::vector<int> vector;
    vector.push_back(50);
    vector.push_back(200);

    HashTable<int> hashTable(tableSize, d, &fFunction);
    hashTable.addPoint(&vector);
    hashTable.addPoint(&vector0);

    const std::list<std::vector<int >* > table = hashTable.getBucket(fFunction.phi(vector0));

    return 0;
}