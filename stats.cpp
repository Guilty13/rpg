#include "stats.h"

TStatsMeta StatsMeta;

ostream& operator <<(ostream& os, const TStats& stats) {
    os << "{ ";
    for (auto& p: stats.Stats) {
        os << StatsMeta[p.first].name << "=" << p.second << ", ";
    }
    os << "}";
}

