//
// Created by chris on 11/25/2018.
//
#include <string>
#include "UtilClasses/MyLogger.h"
#include "UtilClasses/Parameters.h"
#include "FileUtils/ConfigReader.h"
#include "Model/MyVector.h"
#include "FileUtils/DataReader.h"
#include "cluster/init/KmeansppInit.h"
#include "Metrics/EuclideanMetric.h"
#include <vector>
#include "cluster/Cluster.h"
#include "cluster/assign/LloydAssignment.h"
#include "cluster/update/VUpdate.h"
#include "cluster/evaluate/Eval.h"
#include "cluster/evaluate/EvalDef.h"
#include "cluster/ClusteringVector.h"
#include "cluster/init/RandomInit.h"
#include "cluster/update/PAM.h"
#include "cluster/assign/PAMAssignment.h"
#include "cluster/ClusteringArb.h"
#include "cluster/Silhouette.h"
#include "UtilClasses/Timer.h"
#include "cluster/assign/LSHAssignment.h"
#include "LSH/EuclideanLSH.h"
#include "Hashing/FFunction.h"

using namespace std;

int main(int argc, char *argv[]) {
    string file_input = "twitter_dataset_small_v2.csv";
    string file_configure = "cluster.cfg";
    string file_output = "output";
    string type_metric = "euclidean";
    string complete = "false";
    double epsilon = 0.000001;
    // get logger
    MyLogger *logger = MyLogger::getInstance();
    logger->setLevel(MyLogger::INFO);

    // get the parameters
    Parameters params(argc, argv);
    std::string param;
    file_input = params.getParam("-i");
    file_configure = params.getParam("-c");
    file_output = params.getParam("-o");
    type_metric = params.getParam("-m");
    complete = params.getParam("-complete");

    // check parameters
    if (file_input.empty() || file_configure.empty() || file_output.empty()) {
        logger->error("input & output files are mandatory\n");
        return 0;
    }
    if (type_metric != "euclidean" && type_metric != "cosine") {
        logger->error("Not supported metric: ");
        logger->error(type_metric);
        logger->info("supported metrics: euclidean, cosine");
        return 0;
    }

    // get configuration
    int k = 0;
    int kk = 4; // default value
    int L = 5; // default value
    try {
        ConfigReader config = ConfigReader(file_configure);
        if (config.get("number_of_clusters").empty()) {
            logger->error("Could not get number_of_clusters. Check configurarion file");
            return -1;
        }
        k = stoi(config.get("number_of_clusters"));
        if (!config.get("number_of_hash_functions").empty()) {
            kk = stoi(config.get("number_of_hash_functions"));
        }
        if (!config.get("number_of_hash_tables").empty()) {
            L = stoi(config.get("number_of_hash_tables"));
        }
    } catch (std::runtime_error) {
        logger->error("could not find config file:");
        logger->error(file_configure);
        return -1;
    }
    //get the input
    vector<MyVector<double> *> *sample = new vector<MyVector<double> *>();

    DataReader<double> *csv0;
    try {
        csv0 = new DataReader<double>(file_input, ',', false);
    } catch (std::runtime_error e) {
        logger->error(e.what());
        logger->error(file_input);
        return -1;
    }

    MyVector<double> *dataRecord;
    int tableSize = 0;
    int d = 0;
    while ((dataRecord = csv0->getDataRecord())) {
        sample->push_back(dataRecord);
        d = dataRecord->getSize();
        dataRecord = NULL;
        tableSize++;
    }

    // init
    EuclideanMetric<double> *metric = new EuclideanMetric<double>();

    ClusterInit<double> *init = new RandomInit<double>(k);
    //init clusters
    std::vector<Cluster<double> *> clusters(k);
//    Assignment<double> *assign = new LloydAssignment<double>(metric);
    int r[] = {1, 1, 1, 1};
    int w = 4;
    HashFunction<double> *hashFunctions[kk];
    for (int i = 0; i < L; i++) {
        hashFunctions[i] = new FFunction<double>(d, kk, tableSize/8, r, w);
    }
    LSH<double> *lsh = new EuclideanLSH<double>(d, L, tableSize/8, hashFunctions, 0);
    for (int m = 0; m < sample->size(); ++m) {
        lsh->addPoint(sample->at(m));
    }

    Assignment<double> *assign = new LSHAssignment<double>(lsh);

    Eval<double> *eval = new EvalDef<double>(metric);
//    Update<double> *update = new VUpdate<double>();
    Update<double> *update = new PAM<double>(metric);

    Clustering<double> *clustering = new ClusteringArb<double>(sample, init, assign, update, eval, k, epsilon);

    Timer timer = Timer();
    timer.startTimer();
    clustering->run();
    double clustering_time = timer.stopTimer();

    vector<Cluster<double> *> *clusters0 = clustering->getClusters();

    Silhouette<double> *silhouette = new Silhouette<double>(clusters0, metric);
    double sil = silhouette->getMean();

    // output
    string algorithm = "";
    algorithm += std::to_string(init->getId());
    algorithm += std::to_string(assign->getId());
    algorithm += std::to_string(update->getId());

    ofstream myfile;
    myfile.open(file_output);
    myfile << "Algorithm: " << algorithm << endl;

    // each cluster
    for (int j = 0; j < clusters0->size(); ++j) {
        myfile << "CLUSTER-" << j << " {" << endl;

        myfile << "size: " << clusters0->at(j)->getAssigned()->size() << "," << endl;

        myfile << "centroid: ";
        if (update->getId() == 2)
            myfile << clusters0->at(j)->getCentroid()->getId() << "," << endl;
        else {
            myfile << endl << "[" << endl;
            for (int i = 0; i < clusters0->at(j)->getCentroid()->getSize(); ++i) {
                myfile << clusters0->at(j)->getCentroid()->getVector()->at(i);
                if (i != clusters0->at(j)->getCentroid()->getSize() - 1)
                    myfile << ", ";
            }
            myfile << "]" << endl;
        }

    }
    // total statistics
    myfile << "clustering_time: " << clustering_time << endl;
    myfile << "Silhouette: [";
    for (int l = 0; l < clusters0->size(); ++l) {
        myfile << clusters0->at(l)->getMeanSil() << ",";
    }
    myfile << sil << "]" << endl;
    myfile.close();
    // if complete
    if (complete == "true") {
        for (int j = 0; j < clusters0->size(); ++j) {
            myfile << "CLUSTER-" << j << " {";
            for (int i = 0; i < clusters0->at(j)->getAssigned()->size(); ++i) {
                set<MyVector<double> *, Compare<double >> *assigned = clusters0->at(j)->getAssigned();
                typename std::set<MyVector<double> *, Compare<double> >::iterator it;
                for (it = assigned->begin(); it != assigned->end(); ++it) {
                    myfile << (*it)->getId();
                    it++;
                    if (it != assigned->end())
                        myfile << ",";
                    it--;
                }
                myfile << "}" << endl;
            }

        }

    }

    myfile.close();




    // clean memory
    logger->log("Cleaning memory..");
    // delete sample
    for (int i = 0; i < sample->size(); i++)
        delete sample->at(i);
    logger->log("DONE");
    logger->destroy();
    delete logger;
    return 0;
}