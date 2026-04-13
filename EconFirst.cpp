#include "EconFirst.h"

vector<int> optimize_quarter_aviary_second(bool include_self)
{
	int n_max = (1 << Settlement::n_settle), curr_bit, curr_prod_lost, min_prod_lost = 2e9;
	vector<int> optimal; optimal.reserve(10000);
	vector<bool> covered(Settlement::n_settle, false);
	bool fully_covered;
	vector<int> eff_delta(Settlement::n_settle);
	pair<int, int> curr_pair;
	vector<int> delayed_queue; delayed_queue.reserve(20);

	for (int i = 0; i < n_max; i++)
	{
		for (int j = 0; j < Settlement::n_settle; j++)
			covered[j] = false;
		curr_prod_lost = 0;
		for (int j = 0; j < Settlement::n_settle; j++)
			eff_delta[j] = 0;
		delayed_queue.clear();
		fully_covered = true;

		for (int j = 0; j < Settlement::n_settle; j++)
		{
			curr_bit = Settlement::graph[j].bit;
			if (curr_bit & i)
			{
				curr_pair = Settlement::graph[j].min_prod_loss();

				if (Settlement::graph[j].buildings[curr_pair.first] == B::AC)
				{
					for (int k = 0; k < Settlement::n_settle; k++)
					{
						if (Settlement::graph[k].type == Settlement::graph[j].type)
							eff_delta[k] += Settlement::ac_eff;
					}

					curr_prod_lost += curr_pair.second;
				}
				else if (Settlement::graph[j].buildings[curr_pair.first] == B::MP)
				{
					for (int k = 0; k < Settlement::graph[j].adj_list.size(); k++)
					{
						eff_delta[Settlement::graph[j].adj_list[k]] += Settlement::mp_eff;
					}

					curr_prod_lost += curr_pair.second;
				}
				else // Replaced structure is RE
				{
					delayed_queue.push_back(j);
				}
				
				for (int k = 0; k < Settlement::graph[j].adj_list.size(); k++) covered[Settlement::graph[j].adj_list[k]] = true;
				if (include_self) covered[Settlement::graph[j].id] = true;
			}
		}

		for (int j = 0; j < Settlement::n_settle; j++)
			if (!covered[j])
			{
				fully_covered = false;
				break;
			}

		if (!fully_covered) continue;

		for (int j = 0; j < delayed_queue.size(); j++)
			curr_prod_lost += (Settlement::graph[delayed_queue[j]].eff - eff_delta[delayed_queue[j]]);

		if (curr_prod_lost == min_prod_lost)
			optimal.push_back(i);
		else if (curr_prod_lost < min_prod_lost)
		{
			optimal.clear();
			optimal.push_back(i);
			min_prod_lost = curr_prod_lost;
		}

	}

	return optimal;
}

vector<int> optimize_barracks_second(bool exclude_heartland)
{
	int n_max = (1 << Settlement::n_settle), curr_bit, curr_prod_lost, min_prod_lost = 2e9;
	vector<int> optimal; optimal.reserve(10000);
	vector<bool> covered(Settlement::n_settle, false);
	bool fully_covered;
	bitset<20> curr_combo, zero_bitset(0);
	vector<int> eff_delta(Settlement::n_settle);
	pair<int, int> curr_pair;
	vector<int> delayed_queue; delayed_queue.reserve(20);

	for (int i = 0; i < n_max; i++)
	{
		curr_combo = bitset<20>(i);
		if (!curr_combo[8]) continue; // Capital has to have a Barracks

		for (int j = 0; j < Settlement::n_settle; j++)
			covered[j] = false;
		curr_prod_lost = 0;
		for (int j = 0; j < Settlement::n_settle; j++)
			eff_delta[j] = 0;
		delayed_queue.clear();
		fully_covered = true;

		for (int j = 0; j < Settlement::n_settle; j++)
		{
			curr_bit = Settlement::graph[j].bit;
			if (curr_bit & i)
			{
				if (exclude_heartland && Settlement::graph[j].type == 'H') // No Heartland settlement should have a Barracks (to maximize resource generation)
				{
					fully_covered = false;
					break;
				}
				curr_pair = Settlement::graph[j].min_prod_loss();

				if (Settlement::graph[j].buildings[curr_pair.first] == B::AC)
				{
					for (int k = 0; k < Settlement::n_settle; k++)
					{
						if (Settlement::graph[k].type == Settlement::graph[j].type)
							eff_delta[k] += Settlement::ac_eff;
					}

					curr_prod_lost += curr_pair.second;
				}
				else if (Settlement::graph[j].buildings[curr_pair.first] == B::MP)
				{
					for (int k = 0; k < Settlement::graph[j].adj_list.size(); k++)
					{
						eff_delta[Settlement::graph[j].adj_list[k]] += Settlement::mp_eff;
					}

					curr_prod_lost += curr_pair.second;
				}
				else // Replaced structure is RE
				{
					delayed_queue.push_back(j);
				}
			}
		}

		if (!fully_covered) continue;

		for (int j = 0; j < Settlement::n_settle; j++)
			if ((Settlement::graph[j].stroll_2 & curr_combo) == zero_bitset)
			{
				fully_covered = false;
				break;
			}

		if (!fully_covered) continue;

		for (int j = 0; j < delayed_queue.size(); j++)
			curr_prod_lost += (Settlement::graph[delayed_queue[j]].eff - eff_delta[delayed_queue[j]]);

		if (curr_prod_lost == min_prod_lost)
			optimal.push_back(i);
		else if (curr_prod_lost < min_prod_lost)
		{
			optimal.clear();
			optimal.push_back(i);
			min_prod_lost = curr_prod_lost;
		}
	}

	return optimal;
}