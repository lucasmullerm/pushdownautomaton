#include "push_down_automaton.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define DEBUG

// Inner class methods

inline void debug(ofstream& ofs)
{
	ofs << "im here!!!!!!!!";
	ofs.flush();
}

vector<pair<PushDownAutomaton::Transition, PushDownAutomaton::SimulationState> > PushDownAutomaton::SimulationState::next(char head, ofstream& ofs)
{
	vector<pair<PushDownAutomaton::Transition, PushDownAutomaton::SimulationState> > result;

	for (Transition t : automaton.states_[state])
	{


		// if not epsilon transition and head does not match transition go to next iteration
		if (t.tape_symbol != automaton.epsilon && t.tape_symbol != head)
		{
			continue;
		}

		// new state
		stack<char> new_memory(memory);
		string new_state = t.to;
		int new_tape_position = t.tape_symbol == automaton.epsilon ? tape_position : tape_position + 1;

		// check stack top, if matches pop, if epsilon transition keeps going, else next iteration
		if (!memory.empty() && t.pull_symbol == memory.top())
		{
			new_memory.pop();
		}
		else if (t.pull_symbol != automaton.epsilon)
		{
			continue;
		}

		// push into the stack
		if (t.push_symbol != automaton.epsilon)
		{
			new_memory.push(t.push_symbol);
		}
		// push new simulation state
		result.push_back(make_pair(t, PushDownAutomaton::SimulationState(new_state, new_memory, new_tape_position, automaton)));
	}
	return result;
}

string PushDownAutomaton::SimulationState::toString()
{
	ostringstream oss;
	oss << "State: {state: " << state << " stack: {top: " << (memory.empty() ? ' ' : memory.top()) << " size: " << memory.size() << "} tape_position: " << tape_position << "}";
	return oss.str();
}

// class methods

bool PushDownAutomaton::isFinal(string state)
{ 
	return final_states_.count(state) > 0;
}

PushDownAutomaton::PushDownAutomaton(vector<string> states, set<char> alphabet, set<char> stack_alphabet,
	string initial_state, set<string> final_states) 
	: alphabet_(alphabet), stack_alphabet_(stack_alphabet), initial_state_(initial_state), final_states_(final_states)
{
	for (unsigned i = 0; i < states.size(); i++)
	{
		states_[states[i]] = vector<Transition>();
	}
	stack_alphabet_.insert(dollar);
	stack_alphabet_.insert(epsilon);
	alphabet_.insert(epsilon);
}

PushDownAutomaton::~PushDownAutomaton()
{

}

void PushDownAutomaton::addTransition(string from, char tape_symbol, char pull_symbol, string to, char push_symbol)
{
	if (!states_.count(from) || !states_.count(to) || !alphabet_.count(tape_symbol) ||
		!stack_alphabet_.count(pull_symbol) || !stack_alphabet_.count(push_symbol))
	{
		return;
	}

	Transition t(from, tape_symbol, pull_symbol, to, push_symbol);
	states_[from].push_back(t);
}

bool PushDownAutomaton::compute(string tape, ofstream& ofs)
{
#ifdef DEBUG
	ofs << "BFS Simulation using the tape: " << tape << endl << endl;
#endif // DEBUG

	if (tape.size() == 0)
	{
#ifdef DEBUG
		ofs << "fita em branco" << endl;
#endif // DEBUG

		return isFinal(initial_state_);
	}

	queue<SimulationState> simulation_queue;
	simulation_queue.push(SimulationState(initial_state_, stack<char>(), 0, *this));

	while (!simulation_queue.empty())
	{
		SimulationState current = simulation_queue.front();
		simulation_queue.pop();

#ifdef DEBUG
		ofs << "--pop: " << current.toString() << endl;
#endif // DEBUG

		char head = current.tape_position < (int) tape.size() ? tape[current.tape_position] : epsilon;
		vector<pair<Transition, SimulationState> > nextStates = current.next(head, ofs);
		for (auto next : nextStates)
		{
			Transition& t = next.first;
			SimulationState& ss = next.second;

#ifdef DEBUG
			ofs <<"\t>" << t.toString() << " => ";
			ofs << ss.toString() << endl;
#endif // DEBUG

			// check if computation has stopped
			if (ss.tape_position == tape.size() && isFinal(ss.state) && ss.memory.empty())
			{

#ifdef DEBUG
				ofs << "-----pop: " << ss.toString() << endl;
#endif // DEBUG
				return true;
			}
			simulation_queue.push(ss);
		}
	}
	return false;
}

string PushDownAutomaton::Transition::toString()
{
	ostringstream oss;
	oss << "Transition: [" << from << "->" << to << " " << tape_symbol << "," << pull_symbol << "/" << push_symbol << "]";
	return oss.str();
}
