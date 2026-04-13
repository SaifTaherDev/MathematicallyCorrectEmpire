#include <cmath>
#include <algorithm>
#include "Settlement.h"
#include "PtrStruct.h"
#include "Construct.h"
#include "CombatFirst.h"
#include "EconFirst.h"

int main()
{
	string** data = read_file("MCE_QM_AV_BR_second.csv", Settlement::n_settle, Settlement::n_attributes);
	construct_graph(data, Settlement::n_settle);
	Settlement::populate_all_strolls();
	Settlement::update_re_ac_counts();

	//vector<int> optimal_quarter = optimize_quarter_aviary(); // returns { 198832, 199072 }
	vector<int> optimal_quarter_locations = { 4, 5, 7, 11, 16, 17 };
	//vector<int> optimal_aviary = optimize_quarter_aviary(true); // returns { 164392, 164512 } 
	vector<int> optimal_aviary_locations = { 5, 7, 9, 15, 17 };
	// vector<int> optimal_barracks_combos = optimize_barracks(); // returns { 18688, 131336, 131392, 133376, 264448 }
	vector<int> optimal_barracks_locations = { 8, 11, 18 };

	//smart_greedy(); // 13270 with 10 farms and QM/AV/BR pre-set up
	layered_greedy(); // 13130 with 10 farms and QM/AV/BR pre-set up
	//ac_mp_simple_greedy(true);
	//ac_mp_simple_greedy(false); // academy first then mp; 12790 with 10 farms and QM/AV/BR pre-set up
	//ac_mp_simple_greedy(false);
	//ac_mp_simple_greedy(true); // mp first then academy; 12430 with 10 farms and QM/AV/BR pre-set up

	// (barracks allowed in heartland):
	// layered → QM → AV → BR → FA → UN = 13650; after accounting for bonuses: 16300
	// academy first then mp → QM → AV → BR → FA → UN = 13200
	// mp first then academy → QM → AV → BR → FA → UN = 12670

	//vector<int> qm_second = optimize_quarter_aviary_second();
	//int qm = qm_second[0];
	int qm = 199072;
	for (int i = 0; i < 20; i++)
	{
		if (Settlement::graph[i].bit & qm)
			Settlement::graph[i].min_prod_change(B::QM);
	}

	//vector<int> av_second = optimize_quarter_aviary_second(true);
	//int av = av_second[0];
	int av = 164512;
	for (int i = 0; i < 20; i++)
	{
		if (Settlement::graph[i].bit & av)
			Settlement::graph[i].min_prod_change(B::AV);
	}

	//vector<int> br_second = optimize_barracks_second(false);
	//int br = br_second[0];
	int br = 65920;
	//int br = 131392;
	for (int i = 0; i < 20; i++)
	{
		if (Settlement::graph[i].id != 8 && Settlement::graph[i].bit & br)
			Settlement::graph[i].min_prod_change(B::BR);
	}

	Settlement::set_up_farms(10);
	Settlement::set_up_unique(4);

	cout << Settlement::total_prod() << endl; //17130
	

	Settlement::output_map();

	return 0;
}
