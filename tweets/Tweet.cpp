//
// Created by chris on 1/6/2019.
//

#include "Tweet.h"
#include <math.h>
#include <string>

long Tweet::getId() const {
    return id;
}

void Tweet::setId(long id) {
    Tweet::id = id;
}

const std::vector<std::string> &Tweet::getBag() const {
    return bag;
}

void Tweet::setBag(const std::vector<std::string> &bag) {
    Tweet::bag = bag;
}

Tweet::Tweet(long id) : id(id), totalScore(0), alpha(15.0), user(NULL) {}

void Tweet::addToken(std::string token) {
    bag.push_back(token);
}

double Tweet::calc_sentiment(std::map<std::string, std::string> *lexicon) {
    totalScore = 0.0;
    for (int i = 0; i < bag.size(); i++) {
        std::string score = (*lexicon)[bag.at(i)];
        if (score.length() > 0) {
            totalScore += std::stod(score);
        }
    }
    totalScore = totalScore / sqrt(totalScore * totalScore + alpha);
    return totalScore;
}

double Tweet::getAlpha() const {
    return alpha;
}

void Tweet::setAlpha(double alpha) {
    Tweet::alpha = alpha;
}

double Tweet::getTotalScore() const {
    return totalScore;
}

void Tweet::setTotalScore(double totalScore) {
    Tweet::totalScore = totalScore;
}

bool Tweet::hasCrypto(std::set<std::string> *crypto) {
    for (int i = 0; i < bag.size(); i++) {
        if (crypto->find(bag.at(i)) != crypto->end()) {
            return true;
        }
    }
    return false;
}

void Tweet::calcRefCryptos(std::map<std::string, std::set<std::string> *> &cryptos) {
    for (std::map<std::string, std::set<std::string> *>::iterator it = cryptos.begin(); it != cryptos.end(); ++it) {
        if (hasCrypto(it->second) == true) {
            this->cryptos.push_back(it->first);
        }
    }
}

const std::vector<std::string> &Tweet::getCryptos() const {
    return cryptos;
}

void Tweet::setCryptos(const std::vector<std::string> &cryptos) {
    Tweet::cryptos = cryptos;
}

long Tweet::getRec() const {
    return rec;
}

void Tweet::setRec(long rec) {
    Tweet::rec = rec;
}

User *Tweet::getUser() const {
    return user;
}

void Tweet::setUser(User *user) {
    Tweet::user = user;
}

const std::set<Tweet *> &Tweet::getNeighbors() const {
    return neighbors;
}

void Tweet::setNeighbors(const std::set<Tweet *> &neighbors) {
    Tweet::neighbors = neighbors;
}

void Tweet::addNeighbor(Tweet *tweet) {
    neighbors.insert(tweet);
}

void Tweet::clearNeighbors() {
    this->neighbors.clear();
}

