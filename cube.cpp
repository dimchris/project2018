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
#include "Hashing/RandomProjectionCube.h"
#include "HyperCube/CosineHyperCubeInt.h"
#include "Hashing/FFunctionCube.h"
#include "HyperCube/EuclideanHyperCubeInt.h"
#include <math.h>

int main(int argc, char *argv[]) {
    int tableSize = 100;
    int w = 4;
    int d = 0;
    int dd = 0;
    int m;
    int l = 1;
    int probes = 10;
    std::string inputFilename;
    std::string queriesFilename;
    std::string outputFilename;

    // get logger
    MyLogger *logger = MyLogger::getInstance();
    logger->setLevel(MyLogger::INFO);


    Parameters params(argc, argv);
    std::string param;
    inputFilename = params.getParam("-d");
    queriesFilename = params.getParam("-q");
    param = params.getParam("-k");
    if (param.empty()) {
        logger->warn("setting to default k = 3");
    }
    dd = (param.empty() ? 3 : std::stoi(param));
    param = params.getParam("-M");
    m = (param.empty() ? 10 : std::stoi(param));
    if (param.empty()) {
        logger->warn("setting to default M = 10");
    }
    param = params.getParam("-probes");
    probes = (param.empty() ? 2 : std::stoi(param));
    if (param.empty())
        logger->warn("setting to default probes = 2");
    param = params.getParam("-o");
    outputFilename = param;

    if (inputFilename.empty() || outputFilename.empty() || queriesFilename.empty()) {
        logger->error("input & output files are mandatory\n");
        return 0;
    }

    HashFunction<int> *hashFunctions[l];

    // work around to get the dimension from the file and tableSize
    DataReader<int> *csv0;
    try {
        csv0 = new DataReader<int>(inputFilename, ' ');
    } catch (std::runtime_error e) {
        logger->error(e.what());
        logger->error(inputFilename);
        return -1;
    }

    // get the metric
    std::string metric = *csv0->getMetric();
    if (metric.empty()) {
        logger->info("setting metric to default: Euclidean");
        metric = "euclidean";
    } else {
        logger->info(std::string("setting metric to: ") + std::string(metric));
    }
    if (metric != "euclidean" && metric != "cosine") {
        logger->error("Not supported metric: ");
        logger->error(metric);
        logger->info("supported metrics: euclidean, cosine");
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
    dd = dd>0?dd: (int) floor(log10(tableSize));
    csv0->close();
    delete csv0;
    csv0 = NULL;

    // prepare the hashfunctions
    for (int i = 0; i < l; i++) {
        if (metric == "euclidean") {
            hashFunctions[i] = new FFunctionCube<int>(d, dd, w);

        } else
            hashFunctions[i] = new RandomProjectionCube<int>(d, dd);

    }

    //get the LSH
    LSH<int> *lhs = NULL;
    try {
        if (metric == "euclidean") {
            lhs = new EuclideanHyperCubeInt(d, m, dd, probes, hashFunctions, 0);
        } else {
            tableSize = (int) pow(2, dd);
            lhs = new CosineHyperCubeInt(d, m, dd, probes, hashFunctions, 0);

        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    // get the csv reader
    DataReader<int> *csv;
    try {
        csv = new DataReader<int>(inputFilename, ' ');
    } catch (std::runtime_error e) {
        logger->error(e.what());
        logger->error(inputFilename);
        return -1;
    }


    logger->info("Reading input data..");
    //iterate through all records
    while (dataRecord = csv->getDataRecord()) {

        // add to lsh
        try {
            lhs->addPoint(dataRecord);
        } catch (std::exception e) {
            logger->warn("Skipping item :");
            logger->warn(dataRecord->getId());
            logger->error(e.what());
        }

    }
    csv->close();
    delete csv;
    csv = NULL;

    logger->info("Processing queries file..");
    //read queries
    QueryReader<int> *queries;
    try {
        queries = new QueryReader<int>(queriesFilename, ' ');
    } catch (std::runtime_error e) {
        logger->error(e.what());
        logger->error(queriesFilename);
        return -1;
    }
    MyQuery<int> *query;
    std::set<Neighbor<int> *, Compare<int> > neighbors;
    Neighbor<int> *trueNeighbor;
    RecordWriter<int> *writer;
    try {
        writer = new RecordWriter<int>(outputFilename);
    } catch (std::runtime_error e) {
        logger->error(e.what());
        logger->error(outputFilename);
        return -1;
    }
    Timer timer = Timer();
    Timer trueTimer = Timer();
    double elapseTime = 0.0;
    double trueElapseTime = 0.0;
    while (query = queries->getQuery()) {
        logger->info(std::string("Processing query : ") + std::string(query->getVector()->getId()));

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
    logger->log("Writing output file..");
    writer->writeStats();
    delete writer;
    writer = NULL;

    // clear memory
    logger->log("Cleaning memory..");
    delete logger;
    delete lhs;
    for (int i = 0; i < l; i++) {
        delete hashFunctions[i];
    }
    logger->destroy();
    delete logger;
    logger->log("DONE");
    return 0;
}