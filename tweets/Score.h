//
// Created by chris on 1/10/2019.
//

#ifndef CLUSTER_SCORE_H
#define CLUSTER_SCORE_H


class Score {
private:
    bool isSet;
    bool isNormalized;
    bool isValidate;
    double value;
public:
    Score(double score);

    Score();

    double getValue() const;

    void setValue(double value);

    bool isIsSet() const;

    void setIsSet(bool isSet);

    bool isIsNormalized() const;

    void setIsNormalized(bool isNormalized);

    bool isIsValidate() const;

    void setIsValidate(bool isValidate);

    void normalize(double value);


};


#endif //CLUSTER_SCORE_H
