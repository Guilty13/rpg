#include "hero.h"

using namespace std;

std::ostream& operator <<(std::ostream& os, const THeroState& o) {
    os << o.time << ": " << o.stats;
    return os;
}

ostream& operator <<(ostream& os, const TActionSeq& o) {
    for (auto a: o) {
        if (a.type == TAction::TAKE_PATH) {
            os << " => " << a.path;
        }
    }
    return os;
}

ostream& operator <<(ostream& os, const TQuestState& o) {
    os << "S" << o.stage << "(" << o.Stage().name << "): " << static_cast<const THeroState&>(o) << " + " << o.boost;
    return os;
}

ostream& operator <<(ostream& os, const TOption& o) {
    os << "Q" << o.state.quest << ": " << o.actions << " " << o.state;
    return os;
}
