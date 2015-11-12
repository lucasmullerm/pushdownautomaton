#ifndef PUSH_DOWN_AUTOMATON_H_
#define PUSH_DOWN_AUTOMATON_H_

#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <set>

using namespace std;

class PushDownAutomaton
{
private:
	set<char> alphabet_;
	set<char> stack_alphabet_;
	string initial_state_;
	set<string> final_states_;
	struct Transition
	{
		string from;
		string to;
		char tape_symbol;
		char pull_symbol;
		char push_symbol;
		Transition(string from, char tape_symbol, char pull_symbol, string to, char push_symbol) 
			: from(from), tape_symbol(tape_symbol), pull_symbol(pull_symbol), to(to), push_symbol(push_symbol) {
		}
		string toString();
	};
	map<string, vector<Transition> > states_;
	struct SimulationState
	{
		string state;
		stack<char> memory;
		int tape_position;
		PushDownAutomaton& automaton;
		SimulationState(string state, stack<char> memory, int tape_position, PushDownAutomaton& pda)
			: automaton(pda), state(state), memory(memory), tape_position(tape_position) {
		}
		vector<pair<Transition, SimulationState> > next(char head, ofstream& ofs);
		string toString();
	};
	bool isFinal(string state);
	char epsilon = 'e';
	char dollar = '$';

public:
	PushDownAutomaton(vector<string> states, set<char> alphabet, set<char> stack_alphabet,
		string initial_state, set<string> final_states);
	~PushDownAutomaton();
	void addTransition(string from, char tape_symbol, char pull_symbol, string to, char push_symbol);
	bool compute(string tape, ofstream& ofs);
};

#endif //PUSH_DOWN_AUTOMATON_H_

