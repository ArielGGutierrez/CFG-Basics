/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "lexer.h"

using namespace std;

LexicalAnalyzer lexer;

struct rule
{
    string LHS;
    vector<string> RHS;
};

vector<rule> ruleSet;
struct rule Rule;

vector<string> terminals;
vector<string> nonTerminals;

/* Set Operations */
//-----------------------------------------------------------------
bool combine_sets(vector<string>* dstSet, vector<string>* srcSet) 
{
    bool hasChanged = false;
    for (int i = 0; i < srcSet->size(); i++)
    {
        if (srcSet->at(i).compare("#") != 0)
        {
            bool isPresent = false;
            for (int j = 0; i < dstSet->size(); j++)
            {
                if (srcSet->at(i).compare(dstSet->at(j)) == 0)
                {
                    isPresent = true;
                }
            }

            if (!isPresent)
            {
                hasChanged = true;
                dstSet->insert(dstSet->end(),srcSet->at(i));
            }
        }
    }
    return hasChanged;
}

bool is_epsilon_in(vector<string>* set)
{
    for (int i = 0; i < set->size(); i++)
    {
        if(set->at(i).compare("#") == 0)
        {
            return true;
        }
    }

    return false;
}

bool add_epsilon(vector<string>* set)
{
    bool hasChanged = is_epsilon_in(set);

    if (hasChanged)
    {
        set->insert(set->end(), "#");
    }

    return hasChanged;
}

void print_set_in_order(vector<string>* set, vector<string>* order)
{
    for (int i = 0; i < order->size(); i++)
    {
        for (int j = 0; j < set->size(); j++)
        {
            if (order->at(i).compare(set->at(j)) == 0)
            {
                cout << order->at(i) << " ";
            }
        }
    }

    cout << "\n";
}
//-----------------------------------------------------------------

void expect(string token)
{
    Token t = lexer.peek(1);
    if (token == t)
    {
        t = lexer.GetToken();
    }
    else
    {
        syntax_error();
    }
}

void syntax_error()
{
    cout << "Syntax Error DUMMY";
}

/* Parsing */
//--------------------------------------------------
void parse_grammar()
{
    parse_rule_list();
    Token t = lexer.peek(1);
    if (t.token_type == "HASH")
    {
        expect("HASH");
        expect("END_OF_FILE");
    }

    else
    {
        syntax_error();
    }
}

void parse_rule_list()
{
    parse_rule();
    Token t = lexer.peek(1);
    if (t.token_type == "ID")
    {
        parse_rule_list();
    }
}

void parse_id_list()
{
    Token t = lexer.peek(1);
    if (t.token_type == "ID")
    {
        Rule.RHS.insert(Rule.RHS.end(), t.lexeme);
        expect("ID");
        t = lexer.peek(1);
        if (t.token_type == "ID")
        {
            parse_id_list();
        }
    }
}

void parse_rule()
{
    Token t = lexer.peek(1);
    if (t.token_type == "ID")
    {
        Rule.LHS = t.lexeme;
        expect("ID");
        t = lexer.peek(1);
        if (t.token_type == "ARROW")
        {
            expect("ARROW");
            parse_rhs();
            t = lexer.peek(1);
            if (t.token_type == "STAR")
            {
                ruleSet.insert(ruleSet.end(), Rule);
                expect("STAR");
                return;
            }
            else
            {
                syntax_error();
            }

        }
        else
        {
            syntax_error();
        }
    }
    else
    {
        syntax_error();
    }
}

void parse_rhs()
{
    Token t = lexer.peek(1);
    if (t.token_type == "ID")
    {
        parse_id_list();
    }
    else if (t.token_type == "STAR")
    {
        return;
    }
}
//--------------------------------------------------
/*
vector<string> get_nonterminals()
{
    vector<string> nonterminals;

    for (int i = 0; i < ruleSet.size; i++)
    {
        nonterminals.insert(nonterminals.end(), ruleSet.at(i).LHS);
    }

    return nonterminals;
}

vector<string> get_terminals(vector<string> nonterminals)
{
    vector<string> terminals;
    for (int i = 0; i < ruleSet.size(); i++)
    {
        for (int j = 0; j < ruleSet.at(i).RHS.size(); j++)
        {
            string str = ruleSet.at(i).RHS.at(j);
        }
        //nonterminals.insert(nonterminals.end(), ruleSet.at(i).LHS);
    }

    return terminals;
}

int[] check_if_generate()
{
    vector<string> nonterminals = get_nonterminals();
    bool done = false;

    int generates[nonterminals.size()];

    do
    {



    } while (!done);

    return generates;
}
*/
// read grammar
void ReadGrammar()
{
    parse_grammar();
    cout << "0\n";
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    cout << "1\n";
}

// Task 2
void RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
    */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

