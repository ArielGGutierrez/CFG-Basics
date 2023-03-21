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

#include "project2.h"

using namespace std;

struct rule
{
    string LHS;
    vector<string> RHS;
};

/* Set Operations */
//-----------------------------------------------------------------
bool project2::combine_sets(vector<string>* dstSet, vector<string>* srcSet)
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

bool project2::is_epsilon_in(vector<string>* set)
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

bool project2::add_epsilon(vector<string>* set)
{
    bool hasChanged = is_epsilon_in(set);

    if (hasChanged)
    {
        set->insert(set->end(), "#");
    }

    return hasChanged;
}

void project2::print_set_in_order(vector<string>* set, vector<string>* order)
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

void project2::syntax_error()
{
    cout << "Syntax Error DUMMY";
}

void project2::expect(TokenType token)
{
    Token t = lexer.peek(1);
    if (token == t.token_type)
    {
        t = lexer.GetToken();
    }
    else
    {
        syntax_error();
    }
}

/* Parsing */
//--------------------------------------------------
void project2::parse_grammar()
{
    parse_rule_list();
    Token t = lexer.peek(1);
    if (t.token_type == HASH)
    {
        expect(HASH);
        expect(END_OF_FILE);
    }

    else
    {
        syntax_error();
    }
}

void project2::parse_rule_list()
{
    parse_rule();
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        parse_rule_list();
    }
}

void project2::parse_id_list()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        Rule.RHS.insert(Rule.RHS.end(), t.lexeme);
        expect(ID);
        t = lexer.peek(1);
        if (t.token_type == ID
        {
            parse_id_list();
        }
    }
}

void project2::parse_rule()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        Rule.LHS = t.lexeme;
        expect(ID);
        t = lexer.peek(1);
        if (t.token_type == ARROW)
        {
            expect(ARROW);
            parse_rhs();
            t = lexer.peek(1);
            if (t.token_type == STAR)
            {
                ruleSet.insert(ruleSet.end(), Rule);
                expect(STAR);
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

void project2::parse_rhs()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        parse_id_list();
    }
    else if (t.token_type == STAR)
    {
        return;
    }
}
//--------------------------------------------------

vector<string> project2::get_nonterminals()
{
    vector<string> nonterminals;

    for (int i = 0; i < ruleSet.size; i++)
    {
        nonterminals.insert(nonterminals.end(), ruleSet.at(i).LHS);
    }

    return nonterminals;
}

vector<string> project2::get_terminals(vector<string> nonterminals)
{
    vector<string> terminals;
    for (int i = 0; i < ruleSet.size(); i++)
    {
        for (int j = 0; j < ruleSet.at(i).RHS.size(); j++)
        {
            string str = ruleSet.at(i).RHS.at(j);

            int k;
            bool isTerminal = true;
            bool isInList = false;

            for (k = 0; k < nonterminals.size(); k++)
            {
                if (str.compare(nonterminals.at(k)) == 0)
                {
                    isTerminal = false;
                    break
                }
            }

            if (isTerminal)
            {
                for (k = 0; k < terminals.size(); k++)
                {
                    if (str.compare(terminals.at(k)) == 0)
                    {
                        isInList = true;
                        break;
                    }
                }
            }

            if (isTerminal && !isInList)
            {
                terminals.insert(terminals.end(), str);
            }
        }
    }

    return terminals;
}

vector<string> project2::get_universe(vector<string> nonterminals, vector<string> terminals)
{
    //vector<string> nonterminals = get_nonterminals();
    //vector<string> terminals    = get_terminals(nonterminals);
    vector<string> universe;

    universe.insert(universe.end(), "#"); // Epsilon
    universe.insert(universe.end(), "$"); // EOF

    combine_sets(&universe, &terminals);
    combine_sets(&universe, &nonterminals);

    return universe;
}
/*
int* project2::check_if_generate()
{
    vector<string> nonterminals = get_nonterminals();
    vector<string> terminals    = get_terminals(nonterminals);
    vector<string> universe     = get_universe(nonterminals, terminals);
    bool done = false;

    int generates[universe.size()];
    generates[0] = 1;  // Epsilon Generates
    generates[1] = -1; // EOF Doesn't Generate

    do
    {

    } while (!done);

    return generates;
}
*/
// read grammar
void project2::ReadGrammar()
{
    parse_grammar();
    cout << "0\n";
}

// Task 1
void project2::printTerminalsAndNoneTerminals()
{
    cout << "1\n";
}

// Task 2
void project2::RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void project2::CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void project2::CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void project2::CheckIfGrammarHasPredictiveParser()
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

