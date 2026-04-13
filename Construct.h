#pragma once

#include <sstream>
#include "Settlement.h"

string** read_file(string filename, int n_settle, int n_attributes);
void construct_graph(string** data, int n_settle);