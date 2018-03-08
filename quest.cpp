#include "quest.h"

std::ostream& operator <<(std::ostream& os, const TPath& o) {
    os << "< " << o.req;
    if (o.time > 0) {
        os << " (" << o.time << " min)";
    }
    os << " +!" << o.reward << " +?" << o.boost;
    os << " -> " << o.to << " >";
    return os;
}

std::ostream& operator <<(std::ostream& os, const TStage& o) {

    os << "'" << o.text << "' +!" << o.reward << " +?" << o.boost << " -> ";
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

TQuest Rest() {
    return TQuestBuilder(TStage("init", "You decide to have a rest"))
        .Path( TPath().Time(60.), "done" )
    .Stage(TStage("done", "You rest for some time and are ready to go on")) // .RewardStats(TStats("Fatigue", 0.1)))
    .Done();
}

TQuest Sleep() {
    return TQuestBuilder(TStage("init", "You decide to have a sleep"))
        .Path( TPath().Time(300.), "done" )
    .Stage(TStage("done", "You awake fresh and ready for action").RewardStats(TStats("Fatigue", 3)))
    .Done();
}

TQuest GatherBerries() {
    return TQuestBuilder(TStage("init", "You decide to go gather berries"))
        .Path( TPath().Time(60.) , "forest" )
        .Path( TPath().Req(TReq().X("Agility").C(5.)).Time(50) , "forest")
        .Path( TPath().Req(TReq().X("Agility").C(15.)).Time(40) , "forest")
        .Path( TPath().Req(TReq().X("Agility").C(45.)).Time(30) , "forest")
    .Stage(TStage("forest", "You are in a forest"))
        .Path( TPath().RewardBoost("Rounds", 1), "gather")
        .Path( TPath().Req(TReq().X("Strength").X("Will").C(3)).RewardBoost("Rounds", 2), "gather")
        .Path( TPath().Req(TReq().X("Strength").X("Will").C(9)).RewardBoost("Rounds", 3), "gather")
        .Path( TPath().Req(TReq().X("Strength").X("Will").C(27)).RewardBoost("Rounds", 4), "gather")
    .Stage(TStage("gather", "You are gathering berries"))
        .Path( TPath().Req(TReq().X("Rounds").C(1)).RewardBoost("Rounds", -1).Time(10), "gather_blackberry" )
        .Path( TPath().Req(TReq().X("Rounds").C(1)).RewardBoost("Rounds", -1).Time(10), "gather_raspberry" )
        .Path( TPath(), "done" )
    .Stage(TStage("gather_blackberry", "You gather some blackberry").RewardStats(TStats("Blackberry", 3.)))
        .Path( TPath(), "gather" )
    .Stage(TStage("gather_raspberry", "You gather some raspberry").RewardStats(TStats("Raspberry", 3.)))
        .Path( TPath(), "gather" )
    .Stage(TStage("done", "You leave a forest"))
    .Done();
}

TQuest FightGoblin() {
    return TQuestBuilder(TStage("init", "You decide to go fight a goblin"))
        .Path( TPath().Req(TReq().X("Blackberry").C(1)).RewardStats("Blackberry", -1).RewardBoost("Strength", 5) , "init1")
        .Path( TPath().Req(TReq().X("Blackberry").C(3)).RewardStats("Blackberry", -3).RewardBoost("Strength", 12) , "init1")
        .Path( TPath().Req(TReq().X("Blackberry").C(5)).RewardStats("Blackberry", -5).RewardBoost("Strength", 17) , "init1")
        .Path( TPath(), "init1" )
    .Stage(TStage("init1"))
        .Path( TPath().Req(TReq().X("Raspberry").C(1)).RewardStats("Raspberry", -1).RewardBoost("Agility", 5) , "init2")
        .Path( TPath().Req(TReq().X("Raspberry").C(3)).RewardStats("Raspberry", -3).RewardBoost("Agility", 12) , "init2")
        .Path( TPath().Req(TReq().X("Raspberry").C(5)).RewardStats("Raspberry", -5).RewardBoost("Agility", 17) , "init2")
        .Path( TPath(), "init2")
   .Stage(TStage("init2"))
        .Path( TPath().Time(60), "fight")
   .Stage(TStage("fight", "Fight!"))
        .Path( TPath().Req(TReq().X("Strength", 1).X("Agility", 0.5).C(20)), "done")
        .Path( TPath().Req(TReq().X("Strength", 1).X("Agility", 0.5).C(30)).RewardStats("Will", 1), "done")
        .Path( TPath().Req(TReq().X("Strength", 1).X("Agility", 0.5).C(40)).RewardStats("Will", 3), "done")
        .Path( TPath().Req(TReq().X("Strength", 1).X("Agility", 0.5).C(50)).RewardStats("Will", 7), "done")
   .Stage(TStage("done", "Hooray!").RewardStats("Strength", 3).RewardStats("Agility", 3))
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
	.Stage(TStage("do_alone", "You fish alone? Village guys don't like it :-(").RewardStats(TStats("ReputationInVillage", -1.)+TStats("Fishing", 3.)))
	.Done();
}

TQuest HuntSilverFish() {
	return TQuestBuilder(TStage("init", "You are near the river"))
		.Path( TPath().Req(TReq().X("Fishing", 1).X("Will", 0.5).X("Fatigue", 1.).C(30.)).Time(120.) , "begin_hunting")
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

TQuestBook::TQuestBook() {
    push_back(Rest());
    push_back(Sleep());
    push_back(TrainStrength());
    push_back(TrainAgility());
    push_back(GatherBerries());
    push_back(FightGoblin());
    //push_back(TrainFishing());
    //push_back(HuntSilverFish());
    //Fishing...
}
