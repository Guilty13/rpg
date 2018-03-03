#include "quest.h"

std::ostream& operator <<(std::ostream& os, const TPath& o) {
    os << "< " << o.req << " ";
    if (o.time > 0) {
        os << "(" << o.time << " min) ";
    }
    os << "-> " << o.to << " >";
    return os;
}

std::ostream& operator <<(std::ostream& os, const TStage& o) {

    os << "'" << o.text << "' +" << o.reward << " -> ";
    for (auto& p: o.paths) {
        os << "\n  " << p << ", ";
    }
    if (o.paths.size() == 0) {
        os << "*";
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const TQuest& o) {
    for (size_t i = 0; i < o.stages.size(); i++) {
        os << i << ": " << o.stages[i] << endl;
    }
    return os;
}

TQuest TrainStrength() {
    return TQuestBuilder(TStage("You are in the training room"))
        .Path( TPath().Time(60.) , "begin_train" )
        .Path( TPath().Req(TReq().X("Agility", 1.).C(10.)).Time(50.) , "begin_train")
    .Stage("begin_train", TStage("Prepared for training"))
        .Path( TPath(), "poor_train" )
        .Path( TPath().Req(TReq().X("Strength", 1.).C(10.)), "power_train" )
    .Stage("poor_train", TStage("Got somewhat trained").RewardStats(TStats("Strength", 1.)))
    .Stage("power_train", TStage("Got awesomely trained").RewardStats(TStats("Strength", 2.)))
    .Done();
}

TQuest TrainAgility() {
    return TQuestBuilder(TStage("You are in the training room"))
        .Path( TPath().Time(60.) , "basic_train" )
        .Path( TPath().Req(TReq().X("Agility", 1.).C(10.)).Time(50.) , "basic_train")
        //.Path( TPath().Req(TReq().X("Agility", 1.).C(30.)).Time(45.) , "begin_train")
        //.Path( TPath().Req(TReq().X("Agility", 1.).C(100.)).Time(40.) , "begin_train")
    .Stage("basic_train", TStage("Got somewhat trained").RewardStats(TStats("Agility", 1.)))
        .Path( TPath(), "finish" )
        .Path( TPath().Req(TReq().X("Strength", 1.).C(10.)), "power_train" )
    .Stage("power_train", TStage("Got awesomely trained").RewardStats(TStats("Agility", 1.)))
        .Path( TPath(), "finish" )
    .Stage("finish", TStage("Done training"))
    .Done();
}

TQuestBook QuestBook;
