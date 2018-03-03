#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct TStatMeta {
    size_t id = 0;
    string name = "Null";
    float minValue = 0.;
    float maxValue = 1000000.;
    float decay = -0.023104906; // Value /= exp(Decay) per day. -0.023104906 => 0.5 per month

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

struct TStats {

    map<size_t, float> Stats;
    
    TStats operator +(const TStats& s) const {
        TStats r;
        //TODO: join!
        r.Stats = Stats;
        for (auto p: s.Stats) {
            r.Stats[p.first] += p.second;
        }
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
};

ostream& operator <<(ostream& os, const TStats& stats); 

