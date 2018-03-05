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
    return TQuestBuilder(TStage("init", "You are in the training room"))
        .Path( TPath().Req(TReq().X("Agility", 1.).X("Fatigue", 1.).C(10.)).Time(45.) , "begin_train")
        .Path( TPath().Time(60.) , "begin_train" )
    .Stage(TStage("begin_train", "Prepared for training"))
        .Path( TPath(), "poor_train" )
        .Path( TPath().Req(TReq().X("Strength", 1.).C(5.)), "power_train" )
    .Stage(TStage("poor_train", "Got somewhat trained").RewardStats(TStats("Strength", 1.)+TStats("Fatigue", -0.05)+TStats("Will", 1)))
    .Stage(TStage("power_train", "Got awesomely trained").RewardStats(TStats("Strength", 2.)+TStats("Fatigue", -0.05)+TStats("Will", 1.5)))
    .Done();
}

TQuest TrainAgility() {
    return TQuestBuilder(TStage("init", "You are in the training room"))
        .Path( TPath().Time(60.) , "basic_train" )
        .Path( TPath().Req(TReq().X("Agility", 1.).X("Fatigue", 1.).C(10.)).Time(45.) , "basic_train")
        //.Path( TPath().Req(TReq().X("Agility", 1.).C(30.)).Time(45.) , "begin_train")
        //.Path( TPath().Req(TReq().X("Agility", 1.).C(100.)).Time(40.) , "begin_train")
    .Stage(TStage("basic_train", "Got somewhat trained").RewardStats(TStats("Agility", 1.)+TStats("Fatigue", -0.05)))
        .Path( TPath(), "finish" )
        .Path( TPath().Req(TReq().X("Strength", 1.).C(10.)), "power_train" )
    .Stage(TStage("power_train", "Got awesomely trained").RewardStats(TStats("Agility", 1.)+TStats("Fatigue", -0.05)+TStats("Will", 2)))
        .Path( TPath(), "finish" )
    .Stage(TStage("finish", "Done training"))
    .Done();
}

TQuest TrainFishing() {
	return TQuestBuilder(TStage("init", "You are near the river"))
		.Path( TPath().Req(TReq().X("Fishing", 1.).X("Will", 0.3).X("Fatigue", 1.).C(10.)).Time(45.) , "begin_fishing")
        .Path( TPath().Time(60.) , "begin_fishing" )
    .Stage(TStage("begin_fishing", "Ready to fishing!!"))
		.Path( TPath().Req(TReq().X("ReputationInVillage", 1.).C(10.)).Time(15.), "find_boy" )
		.Path( TPath(), "do_alone" )
	.Stage(TStage("find_boy", "You fish with stupid boy! It would be funny... maybe.").RewardStats(TStats("ReputationInVillage", 1.5)+TStats("Fishing", 0.5)))
	.Stage(TStage("do_alone", "You fish alone? Village guys don't like it :-(").RewardStats(TStats("ReputationInVillage", -1.)+TStats("Fishing", 1.)))
	.Done();
}

TQuest HuntSilverFish() {
	return TQuestBuilder(TStage("init", "You are near the river"))
		.Path( TPath().Req(TReq().X("Fishing", 1).X("Will", 0.3).X("Fatigue", 1.).C(45.)).Time(120.) , "begin_hunting")
		.Path( TPath().Req(TReq().X("ReputationInVillage", 1).X("HeroOfVillage", 15.).C(45.)).Time(120.), "find_boyzz")
	.Stage(TStage("begin_hunting" , "You hunt alone, like Rexxar"))
		.Path( TPath().Time(60.), "end")
		.Path( TPath().Req(TReq().X("Fishing", 1).X("Will", 0.3).X("Fatigue", 1.).C(60.)), "end")
	.Stage(TStage("find_boyzz" , "You hunt with boyz, many boyz (or not)"))
		.Path( TPath().Time(60.), "end")
		.Path( TPath().Req(TReq().X("ReputationInVillage", 1).X("HeroOfVillage", 10.).C(50.)), "end")
	.Stage(TStage("end", "You did it").RewardStats(TStats("GotSilverFish", 100.)))
	.Done();
}

