#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct stata{
    int id;
    double value; // ne constanta 
    double defaultValue;
    double degradeKoef;
};

bool checkStata(std::vector<stata> stats,stata it){ // esli bol'she - true, men'she - false; esli net takoi stati, ber'otsa *defaultValue*
		bool che = false;
		for(int i = 0; i < stats.size(); i++){
			if(it.id == stats[i].id) {
				if(it.value <= stats[i].value) {
					che = true;
				}
				return che;
			}
		}
		if(it.value <= it.defaultValue) {
			che = true;
		}
		return che;
}

struct option{
	std::vector<stata> stataRequire; // nuzno stol'ko ili bol'she
	std::vector<stata> stataReward; // delta statov
	std::vector<int> id_to; // -1 eto konets
};

struct quest{
	double time_cost;
	std::vector<option> options;
};

struct hero{
	std::vector<stata> stats;
	int findStataByID(int id) {
		for(int i = 0; i < stats.size(); i++) {
			if(stats[i].id == id) {
				return i;
			}
		}
		return -1;
	}
	double time;
};

void printStata(stata f) {
	std::cout << "stataid = " << f.id << std::endl;
}

void printStats(std::vector<stata> fs) {
	for(int i = 0; i < fs.size(); i++) {
		printStata(fs[i]);
	}
}


void printOption(option op){
	std::cout << "statsRequire :" << std::endl;
	printStats(op.stataRequire);
	std::cout << "statsReward :" << std::endl;
	printStats(op.stataReward);
	std::cout << "connected to : --" << std::endl;
}

int completeQuest(quest q, hero h, std::vector<int> deWay){
	option oNow = q.options[0];
	std::vector<stata> deltaStata;
	for(int i = 0; i < deWay.size(); i++) {
		for(int s = 0; s < oNow.stataRequire.size(); s++) {
			if(!checkStata(h.stats, oNow.stataRequire[s])) {
				for(int i = 0; i < deltaStata.size(); i++) { // vozvrascheniye statov
					int ud = h.findStataByID(deltaStata[i].id);
					h.stats[ud].value -= deltaStata[i].value;  
				}
				return 2; // neprohod po statam
			}
		}
		for(int w = 0; w < oNow.stataReward.size(); w++) { // dobavleniye statov
			int id = h.findStataByID(oNow.stataReward[w].id);
			if(id == -1) {
				stata newStata = oNow.stataReward[w];
				deltaStata.push_back(oNow.stataReward[w]);
				newStata.value = newStata.value + newStata.defaultValue;
				h.stats.push_back(newStata);
			} else {
				deltaStata.push_back(oNow.stataReward[w]);
				h.stats[id].value += oNow.stataReward[w].value;
			}
		}
		// ya leniviy, proverka na vectori
		if(oNow.id_to[deWay[i] ] == -1  ) { return 0;} //to chto dolzno bit'
		oNow = q.options[oNow.id_to[deWay[i] ] ];
	}
	for(int i = 0; i < deltaStata.size(); i++) { // vozvrascheniye statov
		int ud = h.findStataByID(deltaStata[i].id);
		h.stats[ud].value -= deltaStata[i].value;  
	}
	return 0; // put' korotkiy
}

quest quest1(){
	quest q;
	q.time_cost = 20;
	option o;
	stata agility;
	agility.id=0;
	o.stataReward.push_back(agility);
	o.id_to.push_back(-1);
	q.options.push_back(o);
	return q;
}

int main() {
	
	return 0;
}
