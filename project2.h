#ifndef __PROJECT2__H__
#define __PROJECT2__H__

#include <vector>
#include <string>

class project2
{
	private:
		LexicalAnalyzer lexer;
		vector<rule> ruleSet;
		struct rule Rule;

		vector<string> terminals;
		vector<string> nonTerminals;
	public:
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

		vector<string> get_nonterminals();
		vector<string> get_terminals(vector<string> nonterminals);
		vector<string> get_universe(vector<string> nonterminals, vector<string> terminals);

		void ReadGrammar();
		void printTerminalsAndNoneTerminals();
		void RemoveUselessSymbols();
		void CalculateFirstSets();
		void CalculateFollowSets();
		void CheckIfGrammarHasPredictiveParser();
};