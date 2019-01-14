//
// Created by chris on 1/6/2019.
//

#ifndef CLUSTER_USER_H
#define CLUSTER_USER_H


#include "Tweet.h"
#include "Score.h"
#include "../Model/MyVector.h"

class User : public MyVector<double> {

private:
    long id;
    std::vector<Tweet *> tweets;
    std::map<std::string, Score> scores;
    std::set<std::string> cryptos;
    std::set<std::string> validate;
    double mean;
    bool isZero;
    std::set<User *> neighbors;
    std::set<std::string> unknownCryptos;

    static double sim(std::vector<double> *vector0, std::vector<double> *vector1);

    static double dotProduct(std::vector<double> *vector0, std::vector<double> *vector1);

    std::vector<double> getValidationVector();
public:

    User(long id);

    long getId() const;

    void setId(long id);

    const std::vector<Tweet *> &getTweets() const;

    void setTweets(const std::vector<Tweet *> &tweets);

    const std::map<std::string, Score> &getScores() const;

    void setScores(const std::map<std::string, Score> &scores);

    const std::set<std::string> &getCryptos() const;

    void setCryptos(const std::set<std::string> &cryptos);

    const std::set<std::string> &getUnknownCryptos() const;

    void setUnknownCryptos(const std::set<std::string> &unknownCryptos);

    double getMean() const;

    void setMean(double mean);

    bool isIsZero() const;

    void setIsZero(bool isZero);

    const std::set<User *> &getNeighbors() const;

    void setNeighbors(const std::set<User *> &neighbors);

    const std::set<std::string> &getValidate() const;

    void setValidate(const std::set<std::string> &validate);

    void addValidate(std::string);

    void addTweet(Tweet *tweet);

    void calcScores(std::map<std::string, std::set<std::string> *> &cryptos);

    void normalize();

    std::vector<double> getScoreVector();

    static double sim(User *user0, User *user1);

    void evalUnknown(std::vector<User *> &neighbors);

    void setScore(std::string key, double value);

    void addNeighbor(User *user);

    void clearNeighbors();

    // from MyVector

    void setId(const std::string &id) override;

    std::vector<double> *getVector() const override;

    void setVector(std::vector<double> *vector) override;

    int getSize() const override;

    std::vector<std::string> getRecommended(int total);

    void evalUnknown(std::vector<User *> &users, std::map<std::string, double> &sims);

    double mae(std::vector<User *> &users, std::string key);

    void setValidation(std::string key);

    void resetValidation();

    bool isZeroValidation();

};


#endif //CLUSTER_USER_H
