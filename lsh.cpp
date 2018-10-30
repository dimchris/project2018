#include <iostream>
#include "Hashing/FFunction.h"
#include "Hashing/HashTable.h"
#include "FileUtils/CSVReader.h"
#include "LSH/LSH.h"
#include "LSH/EuclideanLSHInt.h"
#include "FileUtils/DataReader.h"
#include "FileUtils/QueryReader.h"
#include "UtilClasses/Parameters.h"
#include "Model/Neighbor.h"
#include "FileUtils/RecordWriter.h"
#include "UtilClasses/Timer.h"
#include "UtilClasses/MyLogger.h"
#include "Hashing/RandomProjection.h"
#include "LSH/CosineLSHInt.h"

int main(int argc, char *argv[]) {
    int k;
    int tableSize = 100;
    int r[] = {1, 1, 1, 1};
    int w = 4;
    int l;
    int d = 0;
    std::string inputFilename;
    std::string queriesFilename;
    std::string outputFilename;

    // get logger
    MyLogger *log = MyLogger::getInstance();
    log->setLevel(MyLogger::INFO);


    Parameters params(argc, argv);
    std::string param;
    inputFilename = params.getParam("-d");
    queriesFilename = params.getParam("-q");
    param = params.getParam("-k");
    if (param.empty())
        log->warn("setting to default k = 4");
    k = (param.empty() ? 4 : std::stoi(param));
    param = params.getParam("-L");
    l = (param.empty() ? 5 : std::stoi(param));
    if (param.empty())
        log->warn("setting to default l = 5");
    param = params.getParam("-o");
    outputFilename = param;

    if (inputFilename.empty() || outputFilename.empty() || queriesFilename.empty()) {
        log->error("input & output files are mandatory\n");
        return 0;
    }

    HashFunction<int> *hashFunctions[l];

    // work around to get the dimension from the file and tableSize
    DataReader<int> *csv0;
    try {
        csv0 = new DataReader<int>(inputFilename, ' ');
    } catch (std::runtime_error e) {
        log->error(e.what());
        log->error(inputFilename);
        return -1;
    }

    // get the metric
    std::string metric = *csv0->getMetric();
    if (metric.empty()) {
        log->info("setting metric to default: Euclidean");
        metric = "euclidean";
    } else {
        log->info(std::string("setting metric to: ") + std::string(metric));
    }
    if (metric != "euclidean" && metric != "cosine") {
        log->error("Not supported metric: ");
        log->error(metric);
        log->info("supported metrics: euclidean, cosine");
        return 0;
    }

    MyVector<int> *dataRecord;
    tableSize = 0;
    while ((dataRecord = csv0->getDataRecord())) {
        d = dataRecord->getSize();
        delete dataRecord;
        dataRecord = NULL;
        tableSize++;
    }

    csv0->close();
    delete csv0;
    csv0 = NULL;

    // prepare the hashfunctions
    for (int i = 0; i < l; i++) {
        if (metric == "euclidean")
            hashFunctions[i] = new FFunction<int>(d, k, tableSize, r, w);
        else
            hashFunctions[i] = new RandomProjection<int>(d, k);

    }

    //get the LSH
    LSH<int> *lhs = NULL;
    try {
        if (metric == "euclidean") {
            lhs = new EuclideanLSHInt(d, l, tableSize, hashFunctions, 0);
        } else {
            tableSize = (int) pow(2, k);
            lhs = new CosineLSHInt(d, l, tableSize, hashFunctions, 0);

        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    // get the csv reader
    DataReader<int> *csv;
    try {
        csv = new DataReader<int>(inputFilename, ' ');
    }catch (std::runtime_error e){
        log->error(e.what());
        log->error(inputFilename);
        return -1;
    }


    log->info("Reading input data..");
    //iterate through all records
    while (dataRecord = csv->getDataRecord()) {

        // add to lsh
        try {
            lhs->addPoint(dataRecord);
        } catch (std::exception e) {
            log->warn("Skipping item :");
            log->warn(dataRecord->getId());
            log->error(e.what());
        }

    }
    csv->close();
    delete csv;
    csv = NULL;

    log->info("Processing queries file..");
    //read queries
    QueryReader<int> *queries;
    try{
       queries = new QueryReader<int>(queriesFilename, ' ');
    }catch (std::runtime_error e){
        log->error(e.what());
        log->error(queriesFilename);
        return -1;
    }
    MyQuery<int> *query;
    std::set<Neighbor<int> *, Compare<int> > neighbors;
    Neighbor<int> *trueNeighbor;
    RecordWriter<int> *writer;
    try{
        writer = new RecordWriter<int>(outputFilename);
    }catch (std::runtime_error e){
        log->error(e.what());
        log->error(outputFilename);
        return -1;
    }
    Timer timer = Timer();
    Timer trueTimer = Timer();
    double elapseTime = 0.0;
    double trueElapseTime = 0.0;
    while (query = queries->getQuery()) {
        log->info(std::string("Processing query : ") + std::string(query->getVector()->getId()));

        // start timer
        timer.startTimer();
        neighbors = lhs->getNeighbors(query);

        // get run time
        elapseTime = timer.stopTimer();

        // start timer
        trueTimer.startTimer();
        trueNeighbor = lhs->trueNearest(query);

        // get run time
        trueElapseTime = trueTimer.stopTimer();

        //write record
        writer->writeRecord(query, neighbors, trueNeighbor, elapseTime, trueElapseTime);

        // clean memory
        delete trueNeighbor;
    }
    delete query;
    delete queries;
    queries = NULL;
    query = NULL;

    // write stats
    log->log("Writing output file..");
    writer->writeStats();
    delete writer;
    writer = NULL;

    // clear memory
    log->log("Cleaning memory..");
    delete log;
    delete lhs;
    for (int i = 0; i < l; i++) {
        delete hashFunctions[i];
    }
    log->destroy();
    log->log("DONE");
    return 0;
}