//
// Created by chris on 1/10/2019.
//

#include <exception>
#include <stdexcept>
#include "Score.h"

double Score::getValue() const {
    return value;
}

void Score::setValue(double value) {
    this->isSet = true;
    Score::value = value;
}

bool Score::isIsSet() const {
    return isSet;
}

void Score::setIsSet(bool isSet) {
    Score::isSet = isSet;
}

Score::Score(double score) : isSet(true), value(score), isNormalized(false), isValidate(false) {}

Score::Score() : isSet(false), value(0.0) {}

bool Score::isIsNormalized() const {
    return isNormalized;
}

void Score::setIsNormalized(bool isNormalized) {
    Score::isNormalized = isNormalized;
}

void Score::normalize(double value) {
    this->value = value;
    this->isNormalized = true;
}

bool Score::isIsValidate() const {
    return isValidate;
}

void Score::setIsValidate(bool isValidate) {
    Score::isValidate = isValidate;
}
