//
// Created by chris on 10/30/2018.
//

#ifndef CUBE_FFUNCTIONCUBE_H
#define CUBE_FFUNCTIONCUBE_H


#include "FFunction.h"

template<class T>
class FFunctionCube : public FFunction<T> {
public:
    FFunctionCube(int d, int dd, int w) : FFunction<T>(d, dd, (int) pow(2, dd), NULL, w) {}

    int phi(std::vector<T> *p) override {
        assert(p->size() == HashFunction<T>::d);
        int result = 0;
        int th = Functions::lmod(FFunction<T>::h(*p, *(FFunction<T>::v[1]), FFunction<T>::tau[1]), 2);
        assert(th < 2 && th >= 0);
        result = th;
        for (int i = 1; i < FFunction<T>::k; i++) {
            result = result << 1;
            th = Functions::lmod(FFunction<T>::h(*p, *(FFunction<T>::v[i]), FFunction<T>::tau[i]), 2);
            result |= th;
            assert(result<(pow(2,FFunction<T>::k)));
        }
        return result;
    }
};


#endif //CUBE_FFUNCTIONCUBE_H
