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
#include "Metrics/CosineMetric.h"
#include "cluster/assign/HypercubeAssignment.h"
#include "Hashing/RandomProjection.h"
#include "LSH/CosineLSHInt.h"
#include "HyperCube/EuclideanHyperCubeInt.h"
#include "HyperCube/CosineHyperCubeInt.h"
#include "Hashing/FFunctionCube.h"
#include "Hashing/RandomProjectionCube.h"

using namespace std;

int main(int argc, char *argv[]) {
    string file_input = "twitter_dataset_small_v2.csv";
    string file_configure = "cluster.cfg";
    string file_output = "output";
    string type_metric = "euclidean";
    string complete = "false";
    double epsilon = 0.000001;
    int r[] = {1, 1, 1, 1};
    int probes = 2;
    int m = 10;
    int w = 4;
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
        if (!config.get("number_of_probes").empty()) {
            probes = stoi(config.get("number_of_hash_tables"));
        }
        if (!config.get("number_of_max_points").empty()) {
            m = stoi(config.get("number_of_hash_tables"));
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
    // set metric
    Metric<double> *metric;
    if (type_metric == "euclidean") {
        metric = new EuclideanMetric<double>();
    } else {
        metric = new CosineMetric<double>();
    }
    // set hashfunctions
    // prepare the hashfunctions
    HashFunction<double> *hashFunctions[L];
    HashFunction<double> *hashFunctionsCube[1];
    for (int i = 0; i < L; i++) {
        if (type_metric == "euclidean") {
            hashFunctions[i] = new FFunction<double>(d, kk, tableSize / 8, r, w);
        } else {
            hashFunctions[i] = new RandomProjection<double>(d, kk);
        }
    }
    if (type_metric == "euclidean") {
        hashFunctionsCube[0] = new FFunctionCube<double>(d, kk, w);
    } else {
        hashFunctionsCube[0] = new RandomProjectionCube<double>(d, kk);
    }

    // set lsh
    LSH<double> *lsh;
    if (type_metric == "euclidean") {
        lsh = new EuclideanLSH<double>(d, L, tableSize / 8, hashFunctions, 0);
    } else {
        lsh = new CosineLSH<double>(d, L, tableSize / 8, hashFunctions, 0);
    }

    // set hc
    HyperCube<double> *hc;
    if (type_metric == "euclidean") {
        hc = new EuclideanHyperCube<double>(d, m, kk, probes, hashFunctions, 0);
    } else {
        tableSize = (int) pow(2, kk);
        hc = new CosineHyperCube<double>(d, m, kk, probes, hashFunctions, 0);
    }

    for (int n = 0; n < sample->size(); ++n) {
        lsh->addPoint(sample->at(n));
//        hc->addPoint(sample->at(n));
    }

    // inits
    ClusterInit<double> *inits[2] = {
            new RandomInit<double>(k),
            new KmeansppInit<double>(k, metric)
    };
    //Assigments
    Assignment<double> *assignments[3] = {
            new PAMAssignment<double>(metric),
            new LSHAssignment<double>(lsh),
            new HypercubeAssignment<double>(hc)
    };
    //updates
    Update<double> *updates[2] = {
            new VUpdate<double>(),
            new PAM<double>(metric)
    };


    //init clusters
    std::vector<Cluster<double> *> clusters(k);

    ClusterInit<double> *init;
    Update<double> *update;
    Assignment<double> *assignment;
    Clustering<double> *clustering;
    Eval<double> *eval = new EvalDef<double>(metric);

    // output file
    ofstream myfile;
    myfile.open(file_output);

    for (int i = 0; i < 2; ++i) {
        for (int j = 1; j < 3; ++j) {
            for (int l = 0; l < 2; ++l) {
                init = inits[i];
                assignment = assignments[j];
                update = updates[l];
                if (l == 1)
                    clustering = new ClusteringArb<double>(sample, init, assignment, update, eval, k, epsilon);
                else
                    clustering = new ClusteringVector<double>(sample, init, assignment, update, eval, k, epsilon);
                Timer timer = Timer();
                logger->log("Running Algorithm: " + to_string(i) + to_string(j) + to_string(l));
                timer.startTimer();
                clustering->run();
                double clustering_time = timer.stopTimer();
                logger->log("Elapsed time: " + to_string(clustering_time));
                timer.reset();

                // get clusters
                vector<Cluster<double> *> *clusters0 = clustering->getClusters();

                // silhouette
                Silhouette<double> *silhouette = new Silhouette<double>(clusters0, metric);
                double sil = silhouette->getMean();

                // output
                string algorithm = "";
                algorithm += std::to_string(init->getId());
                algorithm += std::to_string(assignment->getId());
                algorithm += std::to_string(update->getId());


                myfile << "Algorithm: " << to_string(i) + to_string(j) + to_string(l) << endl;

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

            }
        }
    }
    // close output file
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