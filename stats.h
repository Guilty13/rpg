#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

struct TStatMeta {
    size_t id = 0;
    string name = "Null";
    float minValue = 0.;
    float maxValue = 1000000.;
    float decay = -0.0004813522; // Value *= exp(Decay) per minute. -0.0004813522 => stat halves in 1 day

    TStatMeta() {}

    TStatMeta& Name(const string& name) {
        this->name = name;
        return *this;
    }
};

struct TStatsMeta {
    vector<TStatMeta> Meta;

    map<string, size_t> IdByName;

    size_t FindIdByName(const string& name) {
        auto it = IdByName.find(name);
        if (it == IdByName.end()) {
            cerr << "unknown stat: " << name << endl;
            throw "RTFM";
        }
        return it->second;
    }
    
    const TStatMeta& operator [](int i) const {
        return Meta[i];
    }

    TStatsMeta() {
        Meta.push_back(TStatMeta());
        Meta.push_back(TStatMeta().Name("Agility"));
        Meta.push_back(TStatMeta().Name("Strength"));
        Meta.push_back(TStatMeta().Name("Fishing"));

        for (size_t i = 0; i < Meta.size(); i++) {
            Meta[i].id = i;
            IdByName[Meta[i].name] = i;
        }
    }
};

extern TStatsMeta StatsMeta;

struct TStats;
ostream& operator <<(ostream& os, const TStats& stats); 

struct TStats {

    map<size_t, float> Stats;
    
    TStats& operator +=(const TStats& s) {
        for (auto& p: s.Stats) {
            Stats[p.first] += p.second;
        }
        return *this;
    }

    TStats operator +(const TStats& s) const {
        // TODO: join?!
        auto r = *this;
        r += s;
        return r;
    }

    float operator[](size_t id) const {
        auto it = Stats.find(id);
        return it != Stats.end() ? it->second : 0.;
    }

    TStats() {}

    TStats(const string& name, float value) {
        Stat(name, value);
    }
    
    TStats& Stat(const string& name, float value) {
        Stats[StatsMeta.FindIdByName(name)] = value;
        return *this;
    }

    TStats& Decay(float dt) {
        for (auto& p: Stats) {
            auto meta = StatsMeta[p.first];
            p.second *= exp(meta.decay * dt);
        }

        return *this;
    }
};

