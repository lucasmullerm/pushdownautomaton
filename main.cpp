#include <fstream>
#include <sstream>

#include "push_down_automaton.h"

using namespace std;

int main(int argc, char const *argv[])
{
	vector<string> states;
	set<char> tape_alphabet;
	set<char> stack_alphabet;
	set<string> final_states;
	string initial_state;
	string tape;

	ifstream ifs("entrada.txt");
	string line;

	// read states
	getline(ifs, line);
	istringstream ss(line);
	string state;
	while (ss >> state)
	{
		states.push_back(state);
	}

	// read tape alphabet
	getline(ifs, line);
	ss = istringstream(line);
	char symbol;
	while (ss >> symbol)
	{
		tape_alphabet.insert(symbol);
	}

	// read stack_alphabet
	getline(ifs, line);
	ss = istringstream(line);
	while (ss >> symbol)
	{
		stack_alphabet.insert(symbol);
	}



	// read initial state
	getline(ifs, line);
	initial_state = line;

	// read final states
	getline(ifs, line);
	ss = istringstream(line);
	while (ss >> state)
	{
		final_states.insert(state);
	}

	// read tape
	getline(ifs, line);
	tape = line;

	// create automaton
	PushDownAutomaton pda(states, tape_alphabet, stack_alphabet, initial_state, final_states);

	// read transitions and insert in the automaton
	while (!ifs.eof())
	{
		getline(ifs, line);
		ss = istringstream(line);
		string from, to;
		char a, b, c;
		ss >> from >> to >> a >> b >> c;
		pda.addTransition(from, a, b, to, c);
	}

	// create output file and simulate the tape
	ofstream ofs("saida.txt");

	bool res = pda.compute(tape, ofs);

	if (res)
		ofs << endl << "Tape accepted!" << endl;
	else
		ofs << endl << "Tape not accepted!" << endl;

	ofs.close();
}