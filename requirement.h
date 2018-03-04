#include <iostream>
#include "stats.h"

struct TRequirement;
std::ostream& operator <<(std::ostream& os, const TRequirement&);

struct TRequirement {

    map<size_t, float> xs;
    float c;
    // x1*s1 + x2*s2 + ... >= c

    TRequirement() {}

    TRequirement& X(size_t id, float value) {
        xs[id] += value;
        return *this;
    }

    TRequirement& X(const string name, float value) {
        return X(StatsMeta.FindIdByName(name), value);
    }


    TRequirement& C(float c) {
        this->c = c;
        return *this;
    }

    bool Check(const TStats& stats) const {
        float v = 0.;
        for (auto& p: xs) {
            v += p.second * stats[p.first];
        }
        return v >= c;
    }

    void Assert(const TStats& stats) const {
        if (!Check(stats)) {
            cerr << "Check failed!: " << *this << " " << stats << endl;
            throw "Assert";
        }
    }
};

