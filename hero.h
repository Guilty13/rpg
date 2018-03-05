#include <iostream>
#include <deque>
#include <algorithm>
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
    //TODO: Items
    float time = 0; 
};

std::ostream& operator <<(std::ostream& os, const THeroState& o);

struct TOption {
    size_t quest;
    size_t stage = 0;
    TActionSeq actions;
    THeroState state;

    TOption(size_t quest, const THeroState& state)
    : quest(quest)
    , state(state)
    {}
};

inline float FunctionForLevel(float l) {
	if(l > 0) {
		return sqrt(l);
	} else {
		return sqrt(l*-1.);
	}
}

inline float Level(const TStats& stats) {
    float l = 0;
    for (auto& p: stats.Stats) {
        l += FunctionForLevel(p.second);
    }
    return l;
}

inline float DLevel(const TStats& s1, const TStats& s2) {
    float l = 0;
    for (auto& p: s2.Stats) {
        l += FunctionForLevel(p.second) - FunctionForLevel(s1[p.first]);
    }
    return l;
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
                state.stats.Decay(path.time);
                state.stats += q.stages[stage].reward;
            } else {
                cerr << "unknown action type: " << a.type << endl;
                throw "WTF!";
            }
        }
        if (q.stages[stage].paths.size() != 0) {
            cerr << "Not a final stage: " << stage << endl;
            throw "Assert!";
        }

        this->state = state;
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
            const auto& quest = QuestBook[opt.quest];
            const auto& stage = quest.stages[opt.stage];
            for (size_t i = 0; i < stage.paths.size(); i++) {
                const auto& path = stage.paths[i];
                if (path.req.Check(opt.state.stats)) {
                    TOption op = opt;
                    op.actions.TakePath(i);
                    op.state.time += path.time;
                    op.stage = path.to;
                    op.state.stats.Decay(path.time);
                    op.state.stats += quest.stages[path.to].reward;
                    search.push_back(op);
                    cerr << "Search: Q" << op.quest << ": " << op.actions << " S" << op.stage << "(" << quest.stages[op.stage].name << "): " << op.state << endl;
                }
            }
            if (stage.paths.size() == 0) {
                result.push_back(opt);
            }
        }

        for (auto op: result) {
            cerr << "Result: Q" << op.quest << ": " << op.actions << " S" << op.stage << ": " << op.state << endl;
        }

        std::sort(result.begin(), result.end(), [this](auto& x, auto& y) {
            //return Level(x.state.stats) < Level(y.state.stats); //FIMXE: dLevel/dt!
            return
                DLevel(state.stats, x.state.stats) / (x.state.time - state.time) < 
                DLevel(state.stats, y.state.stats) / (y.state.time - state.time);
        });

        auto op = result.back();
        cerr << "Best: Q" << op.quest << ": " << op.actions << " S" << op.stage << ": " << op.state << endl;

        CompleteQuest(result.back().quest, result.back().actions);
        //CompleteQuest(0, TActionSeq().TakePath(0).TakePath(0));
        //cout << state << endl; 
        //CompleteQuest(1, TActionSeq().TakePath(0).TakePath(0));
        cout << state << endl; 
    }
};

