//
// Created by chris on 1/6/2019.
//

#include "User.h"
#include <math.h>
#include <iostream>
#include <functional>

User::User(long id) : MyVector(std::to_string(id), NULL), id(id), mean(0.0) {
    std::vector<double> *vector = new std::vector<double>();
    setVector(vector); // init parent vector
}

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
        if (it->second.getValue() != 0.0) {
            isZero = false;
        }
        if (!it->second.isIsSet())
            it->second.normalize(mean);
    }
    // copy vector to MyVector vector
    const std::vector<double> &vector0 = getScoreVector();
    for (int i = 0; i < vector0.size(); ++i) {
        MyVector::vector->push_back(vector0[i]);
    }
    MyVector::size = vector0.size();
}

double User::sim(User *user0, User *user1) {
    return sim(user0->getVector(), user1->getVector());
}

std::vector<double> User::getScoreVector() {
    std::vector<double> scoresVector;
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        scoresVector.push_back(it->second.getValue());
    }

    return scoresVector;
}

double User::sim(std::vector<double> *vector0, std::vector<double> *vector1) {
    if (vector0->size() != vector1->size()) {
        throw std::invalid_argument("incompatible dimensions");
    }
    return dotProduct(vector0, vector1) /
           (sqrt(dotProduct(vector0, vector0)) * sqrt(dotProduct(vector1, vector1)));
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

void User::evalUnknown(std::vector<User *> &users) {
    if (users.size() == 0) {
        return;
    }
    for (std::set<std::string>::iterator it = unknownCryptos.begin(); it != unknownCryptos.end(); ++it) {
        // predict score
        double sum = 0.0;
        double z = 0;
        for (int i = 0; i < users.size(); ++i) {
            if (users.at(i) == this)
                continue;
            double sim0 = sim(this, users.at(i));
            double user_score;
            // get the real value and not the predicted
            // if the real value then the crypto is in the cryptos vector
            // else use the mean value
            if (users.at(i)->getScores().at(*it).isIsSet())
                user_score = users.at(i)->getScores().at(*it).getValue();
            else
                user_score = users.at(i)->getMean();
            sum += sim0 * (user_score - users.at(i)->mean);
            z += sim0;
        }
        z = 1 / z;

        // set score
        setScore(*it, mean + z * sum);
    }
}

void User::evalUnknown(std::vector<User *> &users, std::map<std::string, double> &sims) {
    if (users.size() == 0) {
        return;
    }
    for (std::set<std::string>::iterator it = unknownCryptos.begin(); it != unknownCryptos.end(); ++it) {
        // predict score
        double sum = 0.0;
        double z = 0;
        for (int i = 0; i < users.size(); ++i) {
            if (users.at(i) == this)
                continue;
            // get the sim from pre calc sims
            double sim0 = sims["" + std::to_string(this->id) + "_" + std::to_string(users.at(i)->id)];
            double user_score;
            // get the real value and not the predicted
            // if the real value then the crypto is in the cryptos vector
            // else use the mean value
            if (users.at(i)->getScores().at(*it).isIsSet())
                user_score = users.at(i)->getScores().at(*it).getValue();
            else
                user_score = users.at(i)->getMean();
            sum += sim0 * (user_score - users.at(i)->mean);
            z += sim0;
        }
        z = 1 / z;

        // set score
        setScore(*it, mean + z * sum);
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

void User::setId(const std::string &id) {
    MyVector::setId(id);
}

std::vector<double> *User::getVector() const {
    return MyVector::getVector();
}

void User::setVector(std::vector<double> *vector) {
    MyVector::setVector(vector);
}

int User::getSize() const {
    return MyVector::getSize();
}

std::vector<std::string> User::getRecommended(int total) {
    std::vector<std::string> recommended;
    typedef std::function<bool(std::pair<std::string, Score>, std::pair<std::string, Score>)> Comparator;
    Comparator compFunctor =
            [](std::pair<std::string, Score> elem1, std::pair<std::string, Score> elem2) {
                return elem1.second.getValue() > elem2.second.getValue();
            };
    std::set<std::pair<std::string, Score>, Comparator> cryptos(
            scores.begin(), scores.end(), compFunctor);
    int ctotal = 0;
    for (std::pair<std::string, Score> element : cryptos) {
        // if score is negative then return --all remaining will be negative
//        if(element.second.getValue() < 0)
//            return recommended;
        if (unknownCryptos.find(element.first) != unknownCryptos.end()) {
            recommended.push_back(element.first);
            ctotal++;
            // break when reach total recommendations
            if (ctotal == total)
                return recommended;
        }
    }
    return recommended;

}

const std::set<std::string> &User::getValidate() const {
    return validate;
}

void User::setValidate(const std::set<std::string> &validate) {
    User::validate = validate;
}

void User::addValidate(std::string crypto) {
    this->validate.insert(crypto);
}

double User::mae(std::vector<User *> &users, std::string key) {
    if (users.size() == 0) {
        return 0.0;
    }
    // predict score
    double sum = 0.0;
    double z = 0;
    for (int i = 0; i < users.size(); ++i) {
        if (users.at(i) == this)
            continue;
        std::vector<double> validationVector0 = getValidationVector();
        std::vector<double> validationVector1 = users.at(i)->getValidationVector();
        double sim0 = sim(&validationVector0, &validationVector1);
        double user_score;
        // get the real value and not the predicted
        // if the real value then the crypto is in the cryptos vector
        // else use the mean value
        if (!users.at(i)->getScores().at(key).isIsValidate() || users.at(i)->getScores().at(key).isIsSet())
            user_score = users.at(i)->getScores().at(key).getValue();
        else
            user_score = users.at(i)->getMean();
        sum += sim0 * (user_score - users.at(i)->mean);
        z += sim0;
    }
    z = 1 / z;

    return abs(mean + z * sum - getScores().at(key).getValue());
}

std::vector<double> User::getValidationVector() {
    std::vector<double> validationVector;
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        if (it->second.isIsValidate() || !it->second.isIsSet())
            validationVector.push_back(mean);
        else
            validationVector.push_back(it->second.getValue());
    }
    return validationVector;
}

void User::resetValidation() {
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        it->second.setIsValidate(false);
    }
}

void User::setValidation(std::string key) {
    this->scores.at(key).setIsValidate(true);
}

bool User::isZeroValidation() {
    for (std::map<std::string, Score>::iterator it = scores.begin(); it != scores.end(); ++it) {
        if (it->second.isIsSet() && !it->second.isIsValidate() && it->second.getValue() != 0)
            return false;
    }

    return true;

}
