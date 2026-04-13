#pragma once

#include "Settlement.h"

struct PtrStruct
{
	int id;
	Settlement* s;
	int n_farms;

	PtrStruct(int id, Settlement* s) : id(id), s(s) { n_farms = 0; }
};

struct less_than_mp
{
	inline bool operator() (const PtrStruct& struct1, const PtrStruct& struct2)
	{
		int s1_viable_deg(0), s2_viable_deg(0);

		for (int i = 0; i < struct1.s->adj_list.size(); i++)
			if (Settlement::graph[struct1.s->adj_list[i]].bit & Settlement::viable_mp)
				s1_viable_deg++;

		for (int i = 0; i < struct2.s->adj_list.size(); i++)
			if (Settlement::graph[struct2.s->adj_list[i]].bit & Settlement::viable_mp)
				s2_viable_deg++;

		return s1_viable_deg < s2_viable_deg;
	}
};

struct less_than_eff
{
	inline bool operator() (const PtrStruct& struct1, const PtrStruct& struct2)
	{
		return struct1.s->eff < struct2.s->eff;
	}
};

struct less_than_prod_loss
{
	inline bool operator() (const PtrStruct& struct1, const PtrStruct& struct2)
	{
		return struct1.s->min_prod_loss().second < struct2.s->min_prod_loss().second;
	}
};