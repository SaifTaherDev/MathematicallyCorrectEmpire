#include "PtrStruct.h"
#include "Settlement.h"

map<string, int> Settlement::name_2_id = map<string, int>();
vector<Settlement> Settlement::graph = vector<Settlement>();
int Settlement::viable_mp = 257012; // Binary rep of 1s at { 2, 4, 5, 6, 7, 8, 9, 11, 13, 14, 15, 16, 17 } 
int Settlement::n_settle = 20;
int Settlement::n_attributes = 10;
int Settlement::re_h = 0; int Settlement::re_m = 0; int Settlement::re_b = 0;
int Settlement::ac_h = 0; int Settlement::ac_m = 0; int Settlement::ac_b = 0;
int Settlement::ac_eff = 10; int Settlement::mp_eff = 10;

Settlement::Settlement()
{
	eff = 100;
	id = -1;
	bit = -1;
	name = "NYC";
	type = 'Z';
	adj_list.clear();
	buildings.clear();
	buildings.reserve(6);
	adj_list.reserve(6);
}

Settlement::Settlement(int new_id, string new_name, char new_type, const vector<string>& adj_names, const vector<string>& buildings_names) : id(new_id), name(new_name), type(new_type)
{
	eff = 100;
	adj_list.reserve(6);
	buildings.reserve(6);

	bit = (1 << new_id);
	for (const string& s : adj_names)
		adj_list.push_back(name_2_id[s]);
	stroll_2 = 0;
	for (const string& s : buildings_names)
		if (s == "Quartermaster")
			buildings.push_back(B::QM);
		else if (s == "Aviary")
			buildings.push_back(B::AV);
		else if (s == "Barracks")
			buildings.push_back(B::BR);
		else if (s == "Farm")
			buildings.push_back(B::FA);
		else if (s == "Resource")
			buildings.push_back(B::RE);
		else
			buildings.push_back(B::UN);
}

void Settlement::populate_stroll()
{
	stroll_2.set(id);
	for (int i = 0; i < adj_list.size(); i++)
	{
		stroll_2.set(adj_list[i]);
		for (int j = 0; j < graph[adj_list[i]].adj_list.size(); j++)
			if (graph[adj_list[i]].adj_list[j] != id)
				stroll_2.set(graph[adj_list[i]].adj_list[j]);
	}
}

void Settlement::populate_all_strolls()
{
	for (int i = 0; i < n_settle; i++) graph[i].populate_stroll();
}

bool Settlement::can_flip() const
{
	for (int j = 0; j < buildings.size(); j++)
		if (buildings[j] == B::RE)
			return true;

	return false;
}

bool Settlement::flip_re(bool to_academy)
{
	bool was_flipped = false;
	for (int i = 0; i < buildings.size(); i++)
	{
		if (buildings[i] == B::RE)
		{
			switch (type)
			{
			case 'H':
				re_h--;
				if (to_academy) ac_h++;
				break;
			case 'M':
				re_m--;
				if (to_academy) ac_m++;
				break;
			case 'B':
				re_b--;
				if (to_academy) ac_b++;
				break;
			default:
				if (to_academy)
					throw runtime_error("Attempted to install academy in capital.");
			}

			was_flipped = true;
			buildings[i] = to_academy ? B::AC : B::MP;

			break;
		}
	}

	if (was_flipped)
	{
		if (to_academy)
		{
			for (int i = 0; i < n_settle; i++)
			{
				if (graph[i].type != type) continue;

				graph[i].eff += ac_eff;
			}
		}
		else
		{
			for (int i = 0; i < adj_list.size(); i++)
			{
				graph[adj_list[i]].eff += mp_eff;
			}
		}
	}

	return was_flipped;
}

int Settlement::re_count() const
{
	int curr_re = 0;
	for (int j = 0; j < buildings.size(); j++)
		if (buildings[j] == B::RE)
			curr_re++;

	return curr_re;
}

int Settlement::ac_count() const
{
	int curr_re = 0;
	for (int j = 0; j < buildings.size(); j++)
		if (buildings[j] == B::AC)
			curr_re++;

	return curr_re;
}

int Settlement::adj_re_count() const
{
	int total_re_adj = 0;

	for (int i = 0; i < adj_list.size(); i++)
		total_re_adj += graph[adj_list[i]].re_count();

	return total_re_adj;
}

int Settlement::reg_re_count() const
{
	int count = 0;
	for (int i = 0; i < n_settle; i++)
	{
		if (graph[i].type != type) continue;
		count += graph[i].re_count();
	}

	return count;
}

void Settlement::update_re_ac_counts()
{
	re_h = re_m = re_b = 0;
	ac_h = ac_m = ac_b = 0;
	int curr_re, curr_ac;

	for (int i = 0; i < n_settle; i++)
	{
		curr_re = graph[i].re_count();
		curr_ac = graph[i].ac_count();

		switch (graph[i].type)
		{
		case 'H':
			re_h += curr_re;
			ac_h += curr_ac;
			break;
		case 'M':
			re_m += curr_re;
			ac_m += curr_ac;
			break;
		case 'B':
			re_b += curr_re;
			ac_b += curr_ac;
			break;
		}
	}
}

pair<int, int> Settlement::min_prod_loss() const
{
	int min_index(-1), min_loss(2e9), curr_loss(2e9 + 1);
	bool ac_found = false, mp_found = false;
	for (int i = 0; i < buildings.size(); i++)
	{
		if (!ac_found && buildings[i] == B::AC)
		{
			if (type != 'C') curr_loss = reg_re_count() * ac_eff;
			else throw runtime_error("There is an academy in the capital.");

			ac_found = true;
		}
		else if (!mp_found && buildings[i] == B::MP)
		{
			curr_loss = adj_re_count() * mp_eff;
			mp_found = true;
		}
		else if (buildings[i] == B::RE)
		{
			curr_loss = eff;
		}

		if (curr_loss < min_loss)
		{
			min_loss = curr_loss;
			min_index = i;
		}
	}

	return { min_index, min_loss };
}

void Settlement::min_prod_change(B new_building, int override_index)
{
	int index = (override_index == -1) ? min_prod_loss().first: override_index;

	if (buildings[index] == B::AC)
	{
		if (type == 'H') ac_h--;
		else if (type == 'M') ac_m--;
		else if (type == 'B') ac_b--;

		for (int i = 0; i < n_settle; i++)
		{
			if (graph[i].type == type)
				graph[i].eff -= ac_eff;
		}
	}
	else if (buildings[index] == B::MP)
	{
		for (int i = 0; i < adj_list.size(); i++)
		{
			graph[adj_list[i]].eff -= mp_eff;
		}
	}
	else if (buildings[index] == B::RE)
	{
		switch (type)
		{
		case 'H':
			re_h--;
			break;
		case 'M':
			re_m--;
			break;
		case 'B':
			re_b--;
			break;
		}
	}
	else if (buildings[index] != B::RE)
	{
		throw runtime_error("Attempted to flip a core building.");
	}

	if (new_building == B::AC)
	{
		if (type == 'H') ac_h++;
		else if (type == 'M') ac_m++;
		else if (type == 'B') ac_b++;

		for (int i = 0; i < n_settle; i++)
		{
			if (graph[i].type == type)
				graph[i].eff += ac_eff;
		}
	}
	else if (new_building == B::MP)
	{
		for (int i = 0; i < adj_list.size(); i++)
		{
			graph[adj_list[i]].eff += mp_eff;
		}
	}
	else if (new_building == B::RE)
	{
		switch (type)
		{
		case 'H':
			re_h++;
			break;
		case 'M':
			re_m++;
			break;
		case 'B':
			re_b++;
			break;
		}
	}

	buildings[index] = new_building;
}

int Settlement::ac_util() const
{
	int total_re_type;

	switch (type)
	{
	case 'H':
		total_re_type = re_h;
		break;
	case 'M':
		total_re_type = re_m;
		break;
	case 'B':
		total_re_type = re_b;
		break;
	default:
		return -1;
	}

	int loss = (re_count() == 1) ? 2 * eff : eff;

	return ac_eff * (total_re_type - 1) - loss;
}

int Settlement::mp_util() const
{
	int loss = (re_count() == 1) ? 2 * eff : eff;
	return mp_eff * adj_re_count() - loss;
}

pair<int, int> Settlement::max_ac_mp_util(bool ac_util)
{
	int curr_util, max_index = -1, max_util = -2e9;
	for (int i = 0; i < n_settle; i++)
	{
		curr_util = ac_util ? graph[i].ac_util() : graph[i].mp_util();
		if (curr_util > max_util && graph[i].can_flip())
		{
			max_util = curr_util;
			max_index = i;
		}
	}

	return { max_index, max_util };
}

pair<B, pair<int, int>> Settlement::max_util()
{
	pair<int, int> max_ac = max_ac_mp_util(), max_mp = max_ac_mp_util(false);
	B type = max_ac.second > max_mp.second ? B::AC : B::MP;
	pair<int, int> max_pair = max_ac.second > max_mp.second ? max_ac : max_mp;

	return { type, max_pair };
}

void Settlement::set_up_unique(int n)
{
	if (n <= 0) return;

	vector<PtrStruct> ptr_vec;
	for (int i = 0; i < n_settle; i++) ptr_vec.push_back(PtrStruct(i, &Settlement::graph[i]));

	do
	{
		sort(ptr_vec.begin(), ptr_vec.end(), less_than_prod_loss());
		ptr_vec[0].s->min_prod_change(B::UN);
		n--;
	} while (n);
}

void Settlement::set_up_farms(int n)
{
	if (n <= 0) return;

	vector<PtrStruct> ptr_vec;
	for (int i = 0; i < n_settle; i++)
		if (graph[i].type == 'H')
			ptr_vec.push_back(PtrStruct(i, &Settlement::graph[i]));

	do
	{
		sort(ptr_vec.begin(), ptr_vec.end(), less_than_prod_loss());
		while (ptr_vec[0].n_farms == 2) ptr_vec.erase(ptr_vec.begin());
		ptr_vec[0].s->min_prod_change(B::FA);
		ptr_vec[0].n_farms++;
		n--;
	} while (n);
}

int Settlement::total_prod()
{
	int total = 0;

	for (int i = 0; i < graph.size(); i++)
	{
		if ((graph[i].type == 'M' || graph[i].type == 'B') && graph[i].re_count() >= 1) total += graph[i].eff; // To account for x2 production for first quarry/forge built in mountain/border settlements
		if (graph[i].type == 'H') total += 25 * graph[i].re_count();
		total += graph[i].eff * graph[i].re_count();
	}

	return total;
}

void Settlement::output_map()
{
	ofstream output("output.csv");
	string B_2_string[] = { "Quartermaster", "Aviary", "Barracks", "Farm", "Resource", "Unique", "Academy", "Marketplace" };

	for (int i = 0; i < n_settle; i++)
	{
		output << graph[i].name << ",";
		for (int j = 0; j < graph[i].buildings.size(); j++)
		{
			output << B_2_string[(int)graph[i].buildings[j]];
			output << ",";
			//if (j != graph[i].buildings.size() - 1) output << ",";
			//else output << "\n";
		}
		output << graph[i].eff << '\n';
	}
}