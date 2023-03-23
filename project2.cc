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
void Project2::print_set(vector<string>* set)
{
   for (int i = 0; i < set->size(); i++)
   {
        cout << set->at(i) << " ";
   }
    //cout << "\n";
}
//-----------------------------------------------------------------
// Prints out SYNTAX ERROR
void Project2::syntax_error()
{
    cout << "SYNTAX ERROR !!!";
    exit(EXIT_FAILURE);
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

/* Parsing - Ariel Gutierrez */
//--------------------------------------------------
void Project2::parse_grammar()
{
    parse_rule_list();
    Token t = lexer.peek(1);
    if (t.token_type == HASH)
    {
        expect(HASH);
        Token t = lexer.peek(1);
        if (t.token_type == END_OF_FILE)
        {
            expect(END_OF_FILE);
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
    else
    {
        syntax_error();
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
                Rule.RHS.clear();
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
    else
    {
        syntax_error();
    }
}
//--------------------------------------------------

// Gets list of nonterminals from ruleSet
vector<string> Project2::get_nonterminals(vector<rule> rules)
{
    vector<string> nonterminals;

    for (int i = 0; i < rules.size(); i++)
    {
        if (!str_is_in_set(&nonterminals, rules.at(i).LHS))
        {
            nonterminals.insert(nonterminals.end(), rules.at(i).LHS);
        }
    }

    return nonterminals;
}

// Gets list of terminals from RHS of ruleSet
vector<string> Project2::get_terminals(vector<rule> rules, vector<string> nonterminals)
{
    vector<string> terminals;
    for (int i = 0; i < rules.size(); i++)
    {
        for (int j = 0; j < rules.at(i).RHS.size(); j++)
        {
            string str = rules.at(i).RHS.at(j);

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

    universe.insert(universe.end(), "$"); // EOF
    universe.insert(universe.end(), "#"); // Epsilon

    combine_sets(&universe, &terminals);
    combine_sets(&universe, &nonterminals);

    return universe;
}

// Checks if rules can generate
bool* Project2::check_if_generate(vector<rule> rules)
{
    vector<string> nonterminals = get_nonterminals(rules);
    vector<string> terminals    = get_terminals(rules, nonterminals);
    vector<string> universe     = get_universe(terminals, nonterminals);

    // Checks if a given Symbol generates
    bool symbolGen [universe.size()];
    for (int i = 0; i < universe.size(); i++)
    {
        symbolGen[i] = false;
    }

    // Checks if a given rule generates
    bool* ruleGen = new bool [rules.size()];
    for (int i = 0; i < rules.size(); i++)
    {
        ruleGen[i] = false;
    }

    // Terminals Generate
    for (int i = 0; i < terminals.size()+1; i++)
    {
        symbolGen[i+1] = true;
    }

    // Check if Nonterminals Generate
    bool changed = false;

    do
    {
        changed = false;
        // Iterate through rule list
        for (int i = 0; i < rules.size(); i++)
        {
            int count = 0;
            string lhs = rules.at(i).LHS;

            if (!ruleGen[i])
            {
                // Check if RHS rules generate
                for (int j = 0; j < rules.at(i).RHS.size(); j++)
                {
                    string rhs = rules.at(i).RHS.at(j);

                    // check if terminals/nonterminals generate
                    if (symbolGen[get_index(&universe, rhs)])
                    {
                        count++;
                    }

                    else
                    {
                        break;
                    }
                }

                // If all terminals and nonterminals generate, then the rule generates
                if (count == rules.at(i).RHS.size())
                {
                    symbolGen[get_index(&universe, lhs)] = true;
                    ruleGen[i] = true;
                    changed = true;
                }
            }
        }
        
    } while (changed);

    return ruleGen;
}

bool* Project2::check_if_reachable(vector<rule> rulesGen)
{
    vector<string> nonterminals = get_nonterminals(rulesGen);

    // Initialization
    bool symbolReach[nonterminals.size()];
    for (int i = 1; i < nonterminals.size(); i++)
    {
        symbolReach[i] = false;
    }
    symbolReach[0] = true;
    
    // Check if Nonterminal can be reached
    for (int i = 0; i < rulesGen.size(); i++)
    {
        string lhs = rulesGen.at(i).LHS;
        int indexLHS = get_index(&nonterminals, lhs);

        if (symbolReach[indexLHS])
        {
            for (int j = 0; j < rulesGen.at(i).RHS.size(); j++)
            {
                string rhs = rulesGen.at(i).RHS.at(j);
                int indexRHS = get_index(&nonterminals, rhs);

                if (indexRHS > 0)
                {
                    symbolReach[indexRHS] = true;
                }
            }
        }
    }

    bool* rulesReach = new bool [rulesGen.size()];
    for (int i = 0; i < rulesGen.size(); i++)
    {
        rulesReach[i] = false;
    }

    // Check if rules can be reached
    for (int i = 0; i < rulesGen.size(); i++)
    {
        int indexNonterminal = get_index(&nonterminals, rulesGen.at(i).LHS);

        if (symbolReach[indexNonterminal])
        {
            rulesReach[i] = true;
        }
    }

    return rulesReach;
}

// read grammar
void Project2::ReadGrammar()
{
    parse_grammar();

    for (int i = 0; i < ruleSet.size(); i++)
    {
        if (ruleSet.at(i).RHS.empty())
        {
            ruleSet.at(i).RHS.insert(ruleSet.at(i).RHS.end(), "#");
        }
    }

    //cout << "0\n";
}

// Task 1 - Salvador Gomez
void Project2::printTerminalsAndNoneTerminals()
{
    vector<string> nonterminals = get_nonterminals(ruleSet);
    vector<string> terminals = get_terminals(ruleSet, nonterminals);
    vector<string> inOrder;
    for (int i = 0; i < ruleSet.size(); i++) {
        if (str_is_in_set(&nonterminals, ruleSet.at(i).LHS) && !str_is_in_set(&inOrder, ruleSet.at(i).LHS)) {
            inOrder.insert(inOrder.end(), ruleSet.at(i).LHS);
        }
        
        for (int j = 0; j < ruleSet.at(i).RHS.size(); j++) {
            if (str_is_in_set(&nonterminals, ruleSet.at(i).RHS.at(j)) && !str_is_in_set(&inOrder, ruleSet.at(i).RHS.at(j))) {
                inOrder.insert(inOrder.end(), ruleSet.at(i).RHS.at(j));
            }
        }
    }

    vector<string> termMinusEps;
    for (int i = 0; i < terminals.size(); i++)
    {
        if (terminals.at(i).compare("#") != 0)
        {
            termMinusEps.insert(termMinusEps.end(), terminals.at(i));
        }
    }

    print_set(&termMinusEps);
    print_set(&inOrder);
    //cout << "1\n";
}

// Task 2 - Ariel Gutierrez
void Project2::RemoveUselessSymbols()
{
    bool* rulesGenerate = check_if_generate(ruleSet);

    vector<rule> generatingRules;
    vector<rule> usefulRules;

    for (int i = 0; i < ruleSet.size(); i++)
    {
        if (rulesGenerate[i])
        {
            generatingRules.insert(generatingRules.end(), ruleSet.at(i));
            
        }
    }
    
    bool* rulesReachable = check_if_reachable(generatingRules);

    for (int i = 0; i < generatingRules.size(); i++)
    {
        if (rulesReachable[i])
        {
            usefulRules.insert(usefulRules.end(), generatingRules.at(i));
        }
    }

    if (!usefulRules.empty() && usefulRules.at(0).LHS.compare(ruleSet.at(0).LHS) != 0) usefulRules.clear();

    for (int i = 0; i < usefulRules.size(); i++)
    {
        rule Rule = usefulRules.at(i);

        cout << Rule.LHS << " -> ";
        print_set(&Rule.RHS);
        cout << "\n";
    }
    
    //cout << "2\n";
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
    /*
        Reads the input grammar from standard input
        and represent it internally in data structures
        as described in project 2 presentation file
    */
    project2.ReadGrammar();  
    

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