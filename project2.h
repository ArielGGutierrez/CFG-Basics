#ifndef __PROJECT2__H__
#define __PROJECT2__H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

struct rule
{
	string LHS;
	vector<string> RHS;
};

class Project2
{
	private:
		
	public:
		LexicalAnalyzer lexer;
		std::vector<rule> ruleSet;
		struct rule Rule;

		std::vector<string> terminals;
		std::vector<string> nonTerminals;

		int  get_index(vector<string>* universe, string str);
		bool str_is_in_set(vector<string>* srcSet, string str);
		bool combine_sets(vector<string>* dstSet, vector<string>* srcSet);
		bool is_epsilon_in(vector<string>* set);
		bool add_epsilon(vector<string>* set);
		void print_set_in_order(vector<string>* set, vector<string>* order);

		void syntax_error();
		void expect(TokenType token);

		void parse_grammar();
		void parse_rule_list();
		void parse_id_list();
		void parse_rule();
		void parse_rhs();

		vector<string> get_nonterminals(vector<rule> rules);
		vector<string> get_terminals(vector<rule> rules, vector<string> nonterminals);
		vector<string> get_universe(vector<string> nonterminals, vector<string> terminals);

		bool* check_if_generate(vector<rule> rules);
		bool* check_if_reachable(vector<string> rulesGen);

		void ReadGrammar();
		void printTerminalsAndNoneTerminals();
		void RemoveUselessSymbols();
		void CalculateFirstSets();
		void CalculateFollowSets();
		void CheckIfGrammarHasPredictiveParser();
};

#endif  //__PROJECT2__H__