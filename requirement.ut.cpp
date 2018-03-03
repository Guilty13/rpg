#include "requirement.h"

using namespace std;

int main() {

    auto stats = TStats().Stat("Agility", 2.).Stat("Strength", 2.) + TStats().Stat("Agility", 1.).Stat("Fishing", 1.);
    cout << stats << endl;

    auto req1 = TRequirement().X("Strength", 1.).X("Agility", 2.).C(8);
    auto req2 = TRequirement().X("Strength", 2.).X("Agility", 1.).C(8);

    cout << req1 << ": " << req1.Check(stats) << endl;
    cout << req2 << ": " << req2.Check(stats) << endl;
}

