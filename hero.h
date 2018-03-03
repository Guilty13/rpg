#include <iostream>
#include "quest.h"

struct TAction {
    enum TActionType {
        NONE = 0,
        TAKE_PATH,
        USE_ITEM,
    } type = NONE;

    union {
        struct {
            size_t path;
        };

        struct {
            size_t item;
            size_t usage;
        };
    };

    TAction() {}

    TAction& TakePath(size_t path) {
        type = TAKE_PATH;
        this->path = path;
    }
};

struct TActionSeq: vector<TAction>
{
    TActionSeq& TakePath(size_t path) {
        push_back(TAction().TakePath(path));
        return *this;
    }
};

struct THeroState {
    TStats stats;
    //TODO: Items
    float time = 0; 
};

std::ostream& operator <<(std::ostream& os, const THeroState& o) {
    os << o.time << ": " << o.stats;
}

struct THero {

    THeroState state;
    //TODO: bot settings

    void CompleteQuest(size_t quest, const TActionSeq& actions) {
        auto state = this->state;
        auto q = QuestBook[quest];
        size_t stage = 0;

        for (auto a: actions) {
            if (a.type == TAction::TAKE_PATH) {
                auto path = q.stages[stage].paths[a.path];
                path.req.Assert(state.stats);
                state.time += path.time;
                stage = path.to;
                state.stats += q.stages[stage].reward;
                // decay here?
            } else {
                throw "WTF!";
            }
        }
        if (q.stages[stage].paths.size() != 0) {
            cerr << "Not a final stage: " << stage << endl;
            throw "Assert!";
        }
        float dt = state.time - this->state.time;
        state.stats.Decay(dt);

        this->state = state;
    }

    void Progress() {
        //TODO: pick a better quest and walkthrough
        CompleteQuest(0, TActionSeq().TakePath(0).TakePath(0));
        cout << state << endl; 
        CompleteQuest(1, TActionSeq().TakePath(0).TakePath(0));
        cout << state << endl; 
    }
};

