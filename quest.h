#include <utility>
#include <iostream>
#include <unordered_map>

#include "requirement.h"

using TReq = TRequirement;

struct TPath {
    TReq req;
    size_t to;
    float time = 0.; //minutes

    TPath& Req(const TReq& req) {
        this->req = req;
        return *this;
    }

    TPath& Time(float time) {
        this->time = time;
        return *this;
    }
};

struct TStage {
    string name;
    TStats reward;
    vector<TPath> paths;
    string text;

    TStage(const string& name, const string& text = "")
    : name(name)
    , text(text)
    {}

    TStage& RewardStats(const TStats& reward) { //TODO: stats + items!
        this->reward = reward;
        return *this;
    }
};

struct TQuest {
    vector<TStage> stages;
};

struct TLink {
    string to;
    size_t stage;
    size_t path;
};

struct TQuestBuilder {
    TQuest quest;
    vector<TLink> links;

    unordered_map<string, size_t> StageByName;

    TQuestBuilder(const TStage& init = TStage("init")) {
        Stage(init);
    }

    TQuest Done() {

        for (auto l: links) {
            auto it = StageByName.find(l.to);
            if (it == StageByName.end()) {
                cerr << "unknown stage: " << l.to << endl;
                throw "OMG!";
            }
            quest.stages[l.stage].paths[l.path].to = it->second;
        }
        return quest;
    }
    
    TQuestBuilder& Path(const TPath& path, const string& to) {
        auto& paths = quest.stages.back().paths;
        paths.push_back(path);
        links.push_back({to, quest.stages.size() - 1, paths.size() - 1});
        return *this;
    }

    TQuestBuilder& Stage(const TStage& stage) {
        StageByName[stage.name] = quest.stages.size();
        quest.stages.push_back(stage);
        return *this;
    }

};

std::ostream& operator <<(std::ostream& os, const TPath&);
std::ostream& operator <<(std::ostream& os, const TStage&);
std::ostream& operator <<(std::ostream& os, const TQuest&);


TQuest TrainStrength();
TQuest TrainAgility();
//-------------------
TQuest TrainFishing();
TQuest HuntSilverFish();
TQuest HuntGoldenFish();
TQuest HelpStupidBoy();
TQuest BecomeAHeroOfVillage();
//--------------------


struct TQuestBook: vector<TQuest> {

    TQuestBook() {
        push_back(TrainStrength());
        push_back(TrainAgility());
        push_back(TrainFishing());
        push_back(HuntSilverFish());
        //Fishing...
    }

    //TODO: lookup methods...
};

extern TQuestBook QuestBook;
