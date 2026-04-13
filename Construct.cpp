#include "Construct.h"

string** read_file(string filename, int n_settle, int n_attributes)
{
	ifstream input(filename); input.ignore(3);
	string line;
	int i(0), j(0);
	string** data = new string * [n_settle];
	for (int i = 0; i < n_settle; i++) data[i] = new string[n_attributes];

	while (getline(input, line))
	{
		j = 0;
		for (const char& c : line)
		{
			if (c != ',') data[i][j] += c;
			else j++;
		}

		i++;
	}

	return data;
}

void construct_graph(string** data, int n_settle)
{
	Settlement::graph.reserve(n_settle);
	vector<string> adj_names, buildings_names; adj_names.reserve(6); buildings_names.reserve(6);
	stringstream adj_names_stream;
	string curr_adj;

	for (int i = 0; i < n_settle; i++) Settlement::name_2_id[data[i][1]] = stoi(data[i][0]);

	for (int i = 0; i < n_settle; i++)
	{
		adj_names.clear();
		buildings_names.clear();
		adj_names_stream = stringstream(data[i][3]);
		while (adj_names_stream >> curr_adj) adj_names.push_back(curr_adj);

		for (int j = 0; j < 6; j++)
			buildings_names.push_back(data[i][4 + j]);

		Settlement::graph.push_back(Settlement(stoi(data[i][0]), data[i][1], data[i][2][0], adj_names, buildings_names));
	}
}