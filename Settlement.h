#pragma once

#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

enum class B { QM, AV, BR, FA, RE, UN, AC, MP, XX = -1 };

class Settlement
{
public:
	static map<string, int> name_2_id;
	static int n_settle, n_attributes, re_h, re_m, re_b, ac_h, ac_m, ac_b, ac_eff, mp_eff, viable_mp;
	static vector<Settlement> graph;

	int id, bit, eff;
	string name;
	char type;
	vector<int> adj_list;
	bitset<20> stroll_2;
	vector<B> buildings;

	Settlement();
	Settlement(int new_id, string new_name, char new_type, const vector<string>& adj_names, const vector<string>& buildings_names);

	void populate_stroll();
	static void populate_all_strolls();

	bool can_flip() const;
	bool flip_re(bool to_academy = true);

	int re_count() const;
	int ac_count() const;
	int adj_re_count() const;
	int reg_re_count() const;
	static void update_re_ac_counts();

	pair<int, int> min_prod_loss() const;
	void min_prod_change(B new_building, int override_index = -1);
	int ac_util() const;
	int mp_util() const;

	static pair<int, int> max_ac_mp_util(bool ac_util = true);
	static pair<B, pair<int, int>> max_util();

	static void set_up_unique(int n);
	static void set_up_farms(int n);

	static int total_prod();
	
	static void output_map();
};