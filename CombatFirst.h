#pragma once

#include "Settlement.h"

vector<int> optimize_quarter_aviary(bool include_self = false);
vector<int> optimize_barracks();

void smart_greedy();
void layered_greedy();
void ac_mp_simple_greedy(bool is_ac);
void backflip_greedy();
