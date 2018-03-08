#include <iostream>
#include <deque>
#include <algorithm>
#include "quest.h"

struct TAction {
    enum TActionType {
        NONE = 0,
        TAKE_PATH,
        ENTER_SUB_QUEST,
        //LEAVE_SUB_QUEST - automatic
    } type = NONE;

    union {
        struct {
            size_t path;
        };

        struct {
            size_t quest;
        };
    };

    TAction() {}

    TAction& TakePath(size_t path) {
        type = TAKE_PATH;
        this->path = path;
        return *this;
    }
};

struct TActionSeq: vector<TAction>
{
    TActionSeq& TakePath(size_t path) {
        push_back(TAction().TakePath(path));
        return *this;
    }

};

std::ostream& operator <<(std::ostream& os, const TActionSeq& o);

struct THeroState {
    TStats stats;
    float time = 0; 
};

std::ostream& operator <<(std::ostream& os, const THeroState& o);

struct TQuestState: THeroState {
    size_t quest;
    size_t stage = 0;
    TStats boost;

    TQuestState(size_t quest, const THeroState& state)
    : THeroState(state)
    , quest(quest)
    {}

    bool TryTakeAction(const TAction& action, bool assert = false) {
        const auto& quest = QuestBook[this->quest];
        const auto& stage = quest.stages[this->stage];
        if (action.type == TAction::TAKE_PATH) {
            const auto& path = stage.paths[action.path];
            if (!path.req.Check(stats, boost, assert)) {
                return false;
            }
            time += path.time;
            this->stage = path.to;
            stats += path.reward;
            boost += path.boost;
            //cerr << "QQ: " << boost << endl;
            stats.Decay(path.time);
            stats += quest.stages[path.to].reward;
            boost += quest.stages[path.to].boost;
            stats.Clamp();
            boost.Clamp(); 
            return true;
        } else {
            cerr << "unknown action type: " << action.type << endl;
            throw "WTF!";
        }
    }

    bool IsFinalStage(bool assert  = false) {
        const auto& quest = QuestBook[this->quest];
        const auto& stage = quest.stages[this->stage];
        if (stage.paths.size() == 0) {
            return true;
        }
        if (assert) {
            cerr << "Not a final stage: Q" << quest << ": S" << stage << endl;
            throw "Assert!";
        }
        return false;
    }

    const TStage& Stage() const {
        return QuestBook[quest].stages[stage];
    }
};

std::ostream& operator <<(std::ostream& os, const TQuestState& o);

struct TOption {
    TQuestState state;
    TActionSeq actions;

    TOption(size_t quest, const THeroState& state)
    : state(quest, state)
    {}


};

std::ostream& operator <<(std::ostream& os, const TOption& o);

inline float Level(float l) {
    if(l > 0) {
        return sqrt(l);
    } else {
        return -1*sqrt(l*-1.);
    }
}

inline float Level(const TStats& stats) {
    float l = 0;
    for (auto& p: stats.Stats) {
        l += Level(p.second);
    }
    return l;
}

inline float DLevel(const TStats& s1, const TStats& s2) {
    float l = 0;
    for (auto& p: s2.Stats) {
        l += Level(p.second) - Level(s1[p.first]);
    }
    return l;
}


struct THero {

    THeroState state;
    //TODO: bot settings
    

    void CompleteQuest(size_t quest, const TActionSeq& actions) {
        TQuestState state(quest, this->state);
        auto q = QuestBook[quest];

        for (auto a: actions) {
            state.TryTakeAction(a, true);
        }
        state.IsFinalStage(true);

        this->state = state; // discard boosts
    }

    void CompleteQuest(const TOption& opt) {
        CompleteQuest(opt.state.quest, opt.actions);
    }

    void Progress() {
        //TODO: pick a better quest and walkthrough

        deque<TOption> search;
        vector<TOption> result;

        for (size_t q = 0; q < QuestBook.size(); q++) {
            search.push_back(TOption(q, state));
        }

        while (search.size()) {
            auto opt = search.front();
            search.pop_front();

            const auto& stage = opt.state.Stage();
            for (size_t i = 0; i < stage.paths.size(); i++) {
                TOption op = opt;

                op.actions.TakePath(i);
                if (op.state.TryTakeAction(op.actions.back())) {
                    search.push_back(op);
                    cerr << "Search: " << op << endl;
                }
            }
            if (opt.state.IsFinalStage()) {
                result.push_back(opt);
            }
        }

        for (auto op: result) {
            cerr << "Result: " << op << endl;
        }

        std::sort(result.begin(), result.end(), [this](auto& x, auto& y) {
            //return Level(x.state.stats) < Level(y.state.stats); //FIMXE: dLevel/dt!
            return
                DLevel(state.stats, x.state.stats) / (x.state.time - state.time) < 
                DLevel(state.stats, y.state.stats) / (y.state.time - state.time);
        });

        auto op = result.back();
        cerr << "Best: " << op << endl;

        CompleteQuest(result.back());
        //CompleteQuest(0, TActionSeq().TakePath(0).TakePath(0));
        //cout << state << endl; 
        //CompleteQuest(1, TActionSeq().TakePath(0).TakePath(0));
        cout << state << endl; 
    }
};

