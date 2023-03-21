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

/* Set Operations */
//-----------------------------------------------------------------
// Gets the index that a given string would be in a given universe
int Project2::get_index(vector<string>* universe, string str)
{
    for (int i = 0; i < universe->size(); i++)
    {
        if (str.compare(universe->at(i)) == 0)
        {
            return i;
        }
    }

    return -1;

}

// Checks if a given element is part of a given set
bool Project2::str_is_in_set(vector<string>* srcSet, string str)
{
    for (int i = 0; i < srcSet->size(); i++)
    {
        if (str.compare(srcSet->at(i)) == 0)
        {
            return true;
        }
    }

    return false;
}

// Adds elements of a source set to a destination set and returns true if this resulted in a change
bool Project2::combine_sets(vector<string>* dstSet, vector<string>* srcSet)
{
    bool hasChanged = false;
    for (int i = 0; i < srcSet->size(); i++)
    {
        if (srcSet->at(i).compare("#") != 0 && !str_is_in_set(dstSet, srcSet->at(i)))
        {
            hasChanged = true;
            dstSet->insert(dstSet->end(),srcSet->at(i));
        }
    }
    return hasChanged;
}

// Checks if epsilon is in a given set
bool Project2::is_epsilon_in(vector<string>* set)
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

// Adds epsilon to a given set
bool Project2::add_epsilon(vector<string>* set)
{
    bool hasChanged = is_epsilon_in(set);

    if (hasChanged)
    {
        set->insert(set->end(), "#");
    }

    return hasChanged;
}

// Prints set according to a specified order
void Project2::print_set_in_order(vector<string>* set, vector<string>* order)
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
// Prints out SYNTAX ERROR
void Project2::syntax_error()
{
    cout << "Syntax Error DUMMY";
}

// Calls getToken
void Project2::expect(TokenType token)
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
void Project2::parse_grammar()
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

void Project2::parse_rule_list()
{
    parse_rule();
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        parse_rule_list();
    }
}

void Project2::parse_id_list()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        Rule.RHS.insert(Rule.RHS.end(), t.lexeme);
        expect(ID);
        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            parse_id_list();
        }
    }
}

void Project2::parse_rule()
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

void Project2::parse_rhs()
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

// Gets list of nonterminals from ruleSet
vector<string> Project2::get_nonterminals()
{
    vector<string> nonterminals;

    for (int i = 0; i < ruleSet.size(); i++)
    {
        nonterminals.insert(nonterminals.end(), ruleSet.at(i).LHS);
    }

    return nonterminals;
}

// Gets list of terminals from RHS of ruleSet
vector<string> Project2::get_terminals(vector<string> nonterminals)
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
                    break;
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

// Gets list of terminals and nonterminals
vector<string> Project2::get_universe(vector<string> terminals, vector<string> nonterminals)
{
    vector<string> universe;

    universe.insert(universe.end(), "#"); // Epsilon
    universe.insert(universe.end(), "$"); // EOF

    combine_sets(&universe, &terminals);
    combine_sets(&universe, &nonterminals);

    return universe;
}

// Checks if rules can generate
bool* Project2::check_if_generate()
{
    vector<string> nonterminals = get_nonterminals();
    vector<string> terminals    = get_terminals(nonterminals);
    vector<string> universe     = get_universe(terminals, nonterminals);
    bool changed = false;

    bool generates[universe.size()];
    generates[0] = true;  // Epsilon Generates
    generates[1] = false; // EOF Doesn't Generate

    // Terminals Generate
    for (int i = 0; i < terminals.size(); i++)
    {
        generates[i + 2] = true;
    }

    // Check if Nonterminals Generate
    do
    {
        // Iterate through rule list
        for (int i = terminals.size() - 1; i < ruleSet.size(); i++)
        {
            int count = 0;

            // Check if RHS rules generate
            for (int j = 0; j < ruleSet.at(i).RHS.size(); j++)
            {
                
                // If we have the rule A -> epsilon, it generates
                if (ruleSet.at(i).RHS.at(j).size() == 0)
                {
                    generates[get_index(&universe, ruleSet.at(i).LHS)] = true;
                    changed = true;
                    break;
                }

                // If not we need to know if the terminals/nonterminals generate
                else if (generates[get_index(&universe, ruleSet.at(i).LHS)] == true)
                {
                    count++;
                }
            }
            // If all terminals and nonterminals generate, then the rule generates
            if (count == ruleSet.at(i).RHS.size())
            {
                generates[get_index(&universe, ruleSet.at(i).LHS)] = true;
                changed = true;
            }
        }
    } while (changed);

    return generates;
}

// read grammar
void Project2::ReadGrammar()
{
    parse_grammar();
    cout << "0\n";
}

// Task 1
void Project2::printTerminalsAndNoneTerminals()
{
    vector<string> nonterminals = get_nonterminals();
    vector<string> terminals = get_terminals();
    vector<string> inOrder;
    for (int i = 0; i < ruleSet.size(); i++) {
        for (int j = 0; j < ruleSet.at(i).RHS.size(); j++) {
            if (str_is_in_set(&nonterminals, ruleSet.at(i).RHS.at(j))) {
                inOrder.insert(inOrder.end(), ruleSet.at(i).RHS.at(j));
            }
        }
    }
        print_set_in_order(&terminals, &terminals);
        print_set_in_order(&inOrder, &inOrder);
    cout << "1\n";
}

// Task 2
void Project2::RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void Project2::CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void Project2::CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void Project2::CheckIfGrammarHasPredictiveParser()
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

    Project2 project2;
    project2.ReadGrammar();  // Reads the input grammar from standard input
                             // and represent it internally in data structures
                             // ad described in project 2 presentation file

    switch (task) {
        case 1: 
            project2.printTerminalsAndNoneTerminals();
            break;

        case 2: 
            project2.RemoveUselessSymbols();
            break;

        case 3: 
            project2.CalculateFirstSets();
            break;

        case 4: 
            project2.CalculateFollowSets();
            break;

        case 5: 
            project2.CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}
