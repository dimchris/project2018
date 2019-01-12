//
// Created by chris on 1/6/2019.
//

#ifndef CLUSTER_TWEET_H
#define CLUSTER_TWEET_H

#include<string>
#include<vector>
#include <map>
#include <set>

class User;
class Tweet {
private:
    long id;
    long rec;
    User *user;
    std::vector<std::string> bag;
    std::vector<std::string> cryptos;
    double totalScore;
    double alpha;
    std::set<Tweet *> neighbors;


public:
    Tweet(long id);

    double getAlpha() const;

    void setAlpha(double alpha);

    long getId() const;

    void setId(long id);

    const std::vector<std::string> &getBag() const;

    void setBag(const std::vector<std::string> &bag);

    double getTotalScore() const;

    void setTotalScore(double totalScore);

    const std::vector<std::string> &getCryptos() const;

    void setCryptos(const std::vector<std::string> &cryptos);

    long getRec() const;

    void setRec(long rec);

    User *getUser() const;

    void setUser(User *user);

    const std::set<Tweet *> &getNeighbors() const;

    void setNeighbors(const std::set<Tweet *> &neighbors);

    // add token to bag
    void addToken(std::string token);

    // calc sentiment given lexicon
    double calc_sentiment(std::map<std::string, std::string> *lexicon);

    //  check if crypto exist in
    bool hasCrypto(std::set<std::string> *crypto);

    // get ref cryptos
    void calcRefCryptos(std::map<std::string, std::set<std::string> *> &cryptos);

    // add neighbor
    void addNeighbor(Tweet *tweet);

    void clearNeighbors();


};


#endif //CLUSTER_TWEET_H
