#include <iostream>
#include "Hashing/FFunction.h"
#include "Hashing/HashTable.h"
#include "FileUtils/CSVReader.h"
#include "LHS/LHS.h"
#include "LHS/EuclideanLHSInt.h"
#include "FileUtils/DataReader.h"
#include "FileUtils/QueryReader.h"

int main() {
    int d = 128;
    int k = 4;
    int tableSize = 100;
    int r[] = {1, 1, 1, 1};
    int w = 4;
    int l = 5;
    HashFunction<int> *hashFunctions[l];

    // prepare the hashfunctions
    for (int i = 0; i < l; i++) {
        hashFunctions[i] = new FFunction<int>(d, k, tableSize, r, w);
    }
    //get the LSH

    EuclideanLHSInt *lhs;
    try {
        lhs = new EuclideanLHSInt(d, l, tableSize, hashFunctions, 0);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        delete lhs;
        return 0;
    }

    // get the input file name
    std::string inputFilename = "input_small";
    std::string queriesFilename = "query_small";

    // get the csv reader
    DataReader<int> csv = DataReader<int>(inputFilename, ' ');

    std::cout<<*csv.getMetric()<<std::endl;

    //iterate through all records
    MyVector<int> *dataRecord;
    while (dataRecord  = csv.getDataRecord()) {

        // add to lsh
        try{
            lhs->addPoint(dataRecord);
        }catch (std::exception e){
            //TODO: handle exception
            return 0;
        }

    }
    
    //read queries
    QueryReader<int> queries = QueryReader<int>(queriesFilename, ' ');
    MyQuery<int> *query;
    while(query = queries.getQuery()){
        std::set<MyVector<int > *> *neighbors = lhs->getNeighbors(query);
    }
    


    // clear memory
    delete lhs;
    for (int i = 0; i < l; i++) {
        delete hashFunctions[i];
    }

    std::cout << "end !";


    return 0;
}