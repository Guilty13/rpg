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

