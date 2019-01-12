//
// Created by chris on 1/6/2019.
//

#include "User.h"
#include <math.h>
#include <iostream>

User::User(long id) : id(id), mean(0.0) {}

long User::getId() const {
    return id;
}

void User::setId(long id) {
    User::id = id;
}

const std::vector<Tweet *> &User::getTweets() const {
    return tweets;
}

void User::setTweets(const std::vector<Tweet *> &tweets) {
    User::tweets = tweets;
}

void User::addTweet(Tweet *tweet) {
    tweets.push_back(tweet);
}

void User::calcScores(std::map<std::string, std::set<std::string> *> &cryptos) {
    // init scores
    for (std::map<std::string, std::set<std::string> *>::iterator it = cryptos.begin(); it != cryptos.end(); ++it) {
        unknownCryptos.insert(it->first);
        Score score();
        scores[it->first] = Score(); // init scores
    }
    for (int i = 0; i < tweets.size(); i++) {
        // get tweets cryptos
        std::vector<std::string> tweet_cryptos = tweets.at(i)->getCryptos();

        // for each tweet add crypto and score
        for (int j = 0; j < tweet_cryptos.size(); j++) {
            this->cryptos.insert(tweet_cryptos.at(j));
            unknownCryptos.erase(tweet_cryptos.at(j));
            scores[tweet_cryptos.at(j)].setValue(
                    scores[tweet_cryptos.at(j)].getValue() + tweets.at(i)->getTotalScore());
        }
    }
}

const std::map<std::string, Score> &User::getScores() const {
    return scores;
}

void User::setScores(const std::map<std::string, Score> &scores) {
    User::scores = scores;
}

const std::set<std::string> &User::getCryptos() const {
    return cryptos;
}

void User::setCryptos(const std::set<std::string> &cryptos) {
    User::cryptos = cryptos;
}

void User::normalize() {
    // find mean score
    int total = 0;
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        total++;
        mean += it->second.getValue();
    }
    mean /= total;

    // normalize values
    this->isZero = true;
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        if(it->second.getValue() != 0.0){
            isZero = false;
        }
        if (!it->second.isIsSet())
            it->second.normalize(0.0);
    }
}

double User::sim(User *user0, User *user1) {
    return sim(user0->getScoreVector(), user1->getScoreVector());
}

std::vector<double> User::getScoreVector() {
    std::vector<double> scoresVector;
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        scoresVector.push_back(it->second.getValue());
    }

    return scoresVector;
}

double User::sim(std::vector<double> vector0, std::vector<double> vector1) {
    if (vector0.size() != vector1.size()) {
        throw std::invalid_argument("incompatible dimensions");
    }
    return dotProduct(&vector0, &vector1) / (sqrt(dotProduct(&vector0, &vector0)) * sqrt(dotProduct(&vector1, &vector1)));
}

double User::dotProduct(std::vector<double> *vector0, std::vector<double> *vector1) {
    if (vector0->size() != vector1->size()) {
        throw std::invalid_argument("incompatible dimensions");
    }
    double total = 0.0;
    for (int i = 0; i < vector0->size(); ++i) {
        total += vector0->at(i) * vector1->at(i);
    }

    return total;
}

const std::set<std::string> &User::getUnknownCryptos() const {
    return unknownCryptos;
}

void User::setUnknownCryptos(const std::set<std::string> &unknownCryptos) {
    User::unknownCryptos = unknownCryptos;
}

void User::evalUnknown(std::vector<User *> users) {
    for (std::set<std::string>::iterator it = unknownCryptos.begin(); it != unknownCryptos.end(); ++it) {
        // predict score
        double sum = 0.0;
        double z = 0;
        for (int i = 0; i < users.size(); ++i) {
            double sim0 = sim(this, users.at(i));
            sum += sim0 * (users.at(i)->getScores().at(*it).getValue() - users.at(i)->mean);
            z += sim0;
        }
        z = 1 / z;

        // set score
        setScore(*it, mean + z * sum);

        // test print score
        std::cout << *it << ":" << scores[*it].getValue() << std::endl;
    }
}

void User::setScore(std::string key, double value) {
    scores[key] = Score(value);
}

double User::getMean() const {
    return mean;
}

void User::setMean(double mean) {
    User::mean = mean;
}

bool User::isIsZero() const {
    return isZero;
}

void User::setIsZero(bool isZero) {
    User::isZero = isZero;
}

const std::set<User *> &User::getNeighbors() const {
    return neighbors;
}

void User::setNeighbors(const std::set<User *> &neighbors) {
    User::neighbors = neighbors;
}

void User::addNeighbor(User *user) {
    this->neighbors.insert(user);
}

void User::clearNeighbors() {
    this->neighbors.clear();
}
