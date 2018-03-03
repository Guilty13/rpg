#include "requirement.h"

using namespace std;

ostream& operator <<(ostream& os, const TRequirement& req) {
    if (req.xs.size() == 0) {
        cout << "[ ]";
    } else {

        cout << "[ ";
        for (auto& p: req.xs) {
            cout << " + " << p.second << "*" << StatsMeta[p.first].name;
        }
        cout << " >= " << req.c << " ] ";
    }
    return os;
}

