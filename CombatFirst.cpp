#include "CombatFirst.h"
#include "PtrStruct.h"
#include "Settlement.h"

vector<int> optimize_quarter_aviary(bool include_self)
{
	int n_max = (1 << Settlement::n_settle), curr_bit, n_quarter_aviary, min_quarter_aviary = 999;
	vector<int> optimal; optimal.reserve(10000);
	vector<bool> covered(Settlement::n_settle, false);
	bool fully_covered;

	for (int i = 0; i < n_max; i++)
	{
		for (int j = 0; j < Settlement::n_settle; j++)
			covered[j] = false;
		n_quarter_aviary = 0;

		for (int j = 0; j < Settlement::n_settle; j++)
		{
			curr_bit = Settlement::graph[j].bit;
			if (curr_bit & i)
			{
				n_quarter_aviary++;
				for (int k = 0; k < Settlement::graph[j].adj_list.size(); k++) covered[Settlement::graph[j].adj_list[k]] = true;
				if (include_self) covered[Settlement::graph[j].id] = true;
			}
		}

		fully_covered = true;
		for (int j = 0; j < Settlement::n_settle; j++)
			if (!covered[j])
			{
				fully_covered = false;
				break;
			}

		if (!fully_covered) continue;

		if (n_quarter_aviary == min_quarter_aviary)
			optimal.push_back(i);
		else if (n_quarter_aviary < min_quarter_aviary)
		{
			optimal.clear();
			optimal.push_back(i);
			min_quarter_aviary = n_quarter_aviary;
		}

	}

	return optimal;
}

vector<int> optimize_barracks()
{
	int n_max = (1 << Settlement::n_settle), curr_bit, n_barracks, min_barracks = 999;
	vector<int> optimal; optimal.reserve(10000);
	vector<bool> covered(Settlement::n_settle, false);
	bool fully_covered;
	bitset<20> curr_combo, zero_bitset(0);

	for (int i = 0; i < n_max; i++)
	{
		curr_combo = bitset<20>(i);
		if (!curr_combo[8]) continue; // Capital has to have a Barracks
		for (int j = 0; j < Settlement::n_settle; j++)
			covered[j] = false;
		n_barracks = 0;
		fully_covered = true;

		for (int j = 0; j < Settlement::n_settle; j++)
		{
			curr_bit = Settlement::graph[j].bit;
			if (curr_bit & i)
			{
				if (Settlement::graph[j].type == 'H') // No Heartland settlement should have a Barracks (to maximize resource generation)
				{
					fully_covered = false;
					break;
				}
				n_barracks++;
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

		if (n_barracks == min_barracks)
			optimal.push_back(i);
		else if (n_barracks < min_barracks)
		{
			optimal.clear();
			optimal.push_back(i);
			min_barracks = n_barracks;
		}
	}

	return optimal;
}

void smart_greedy()
{
	vector<PtrStruct> ptr_vec, h_vec, m_vec, b_vec; ptr_vec.reserve(Settlement::n_settle);
	h_vec.reserve(Settlement::n_settle / 3 + 1); m_vec.reserve(Settlement::n_settle / 3 + 1); b_vec.reserve(Settlement::n_settle / 3 + 1);
	for (int i = 0; i < Settlement::n_settle; i++) ptr_vec.push_back(PtrStruct(i, &Settlement::graph[i]));
	sort(ptr_vec.begin(), ptr_vec.end(), less_than_mp());

	for (int i = 0; i < Settlement::n_settle; i++)
	{
		switch (ptr_vec[i].s->type)
		{
		case 'H':
			h_vec.push_back(ptr_vec[i]);
			break;
		case 'M':
			m_vec.push_back(ptr_vec[i]);
			break;
		case 'B':
			b_vec.push_back(ptr_vec[i]);
			break;
		default:
			continue;
		}
	}

	pair<B, pair<int, int>> curr_optimal = Settlement::max_util();
	char reg_type;
	vector<PtrStruct>* reg_vec;
	int min_avail;

	while (curr_optimal.second.second > 0)
	{
		min_avail = 0;

		if (curr_optimal.first == B::MP) Settlement::graph[curr_optimal.second.first].flip_re(false);
		else
		{
			reg_type = Settlement::graph[curr_optimal.second.first].type;
			if (reg_type == 'H') reg_vec = &h_vec;
			else if (reg_type == 'M') reg_vec = &m_vec;
			else if (reg_type == 'B') reg_vec = &b_vec;
			else throw runtime_error("Attempted to install academy in capital.");

			while (min_avail < (*reg_vec).size() && !(*reg_vec)[min_avail].s->can_flip())
				min_avail++;

			if (min_avail >= (*reg_vec).size())
			{
				cout << "\n Sth went wrong.";
				break;
			}
			(*reg_vec)[min_avail].s->flip_re();
		}

		curr_optimal = Settlement::max_util();
	}
}

void layered_greedy()
{
	pair<B, pair<int, int>> curr_optimal = Settlement::max_util();

	while (curr_optimal.second.second > 0)
	{
		Settlement::graph[curr_optimal.second.first].flip_re(curr_optimal.first == B::AC);
		curr_optimal = Settlement::max_util();
	}
}

void ac_mp_simple_greedy(bool is_ac)
{
	pair<int, int> curr_optimal = Settlement::max_ac_mp_util(is_ac);

	while (curr_optimal.second > 0)
	{
		Settlement::graph[curr_optimal.first].flip_re(is_ac);
		curr_optimal = Settlement::max_ac_mp_util(is_ac);
	}
}