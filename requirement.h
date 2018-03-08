#include <iostream>
#include "stats.h"

struct TRequirement;
std::ostream& operator <<(std::ostream& os, const TRequirement&);

struct TRequirement {

    map<size_t, float> xs;
    float c = 0.;
    // x1*s1 + x2*s2 + ... >= c

    TRequirement() {}

    TRequirement& X(size_t id, float value) {
        xs[id] += value;
        return *this;
    }

    TRequirement& X(const string name, float value = 1.) {
        return X(StatsMeta.FindIdByName(name), value);
    }


    TRequirement& C(float c) {
        this->c = c;
        return *this;
    }

    bool Check(const TStats& stats, bool assert = false) const {
        float v = 0.;
        for (auto& p: xs) {
            v += p.second * stats[p.first];
        }
        if (v >= c) {
            return true;
        }
        if (assert) {
            cerr << "Check failed!: " << *this << " " << stats << endl;
            throw "Assert";
        }
        return false;
    }

    bool Check(const TStats& stats1, const TStats& stats2, bool assert = false) const {
        //TODO: no tmp object?
        auto stats = stats1 + stats2;
        stats.Clamp();
        return Check(stats, assert); 
    }
};

