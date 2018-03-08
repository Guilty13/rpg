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
    float minValue =-1000000.;
    float maxValue = 1000000.;
    float decay = -0.0004813522; // Value *= exp(Decay) per minute. -0.0004813522 => stat halves in 1 day

    TStatMeta() {}

    TStatMeta& Name(const string& name) {
        this->name = name;
        return *this;
    }
    
    TStatMeta& MinValue(const float& minValue) {
        this->minValue = minValue;
        return *this;
    }
    
    TStatMeta& MaxValue(const float& maxValue) {
        this->maxValue = maxValue;
        return *this;
    }
    
    TStatMeta& Decay(const float& decay) {
        this->decay = decay;
        return *this;
    }
    
 //   TStatMeta& Binary() {
//		return *this->Decay(0.).MaxValue(1.).MinValue(0.));
//	}
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
        Meta.push_back(TStatMeta().Name("Will"));
        Meta.push_back(TStatMeta().Name("Fishing"));
        //---------------------------------------
        Meta.push_back(TStatMeta().Name("Blackberry").Decay(-0.0004813522/3));
        Meta.push_back(TStatMeta().Name("Raspberry").Decay(-0.0004813522/3));
        Meta.push_back(TStatMeta().Name("Rounds").Decay(0.).MinValue(0.));
        //---------------------------------------
        Meta.push_back(TStatMeta().Name("Fatigue").MaxValue(0.).Decay(-0.0004813522 * 6));
        Meta.push_back(TStatMeta().Name("ReputationInVillage"));
        Meta.push_back(TStatMeta().Name("HeroOfVillage").Decay(0.).MaxValue(1.).MinValue(0.));
        Meta.push_back(TStatMeta().Name("GotSilverFish").Decay(0.).MaxValue(100.).MinValue(0.));
        //----------------------------------------

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

    void Clamp() {
        for (auto& p: Stats) {
            const auto& meta = StatsMeta[p.first];
            p.second = std::max(std::min(p.second, meta.maxValue), meta.minValue);
        }
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

