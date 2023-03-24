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
    bool hasEpsilon = is_epsilon_in(set);

    if (!hasEpsilon)
    {
        set->insert(set->begin(), "#");
    }

    return !hasEpsilon;
}

// Prints set
void Project2::print_set(vector<string>* set)
{
   for (int i = 0; i < set->size(); i++)
   {
        cout << set->at(i) << " ";
   }
}

// Prints set in a specified order
void Project2::print_set_in_order(vector<string>* set, vector<string>* order)
{
   string out1;
   for (int i = 0; i < order->size(); i++)
   {
        for (int j = 0; j < set->size(); j++)
        {
            if (order->at(i).compare(set->at(j)) == 0)
            {
                out1.append(set->at(j) + ", ");
            }
        }
   }
   string output = out1.substr(0, out1.size() -2 );
   cout << output;
}
//-----------------------------------------------------------------

// Prints out SYNTAX ERROR !!!
void Project2::syntax_error()
{
    cout << "SYNTAX ERROR !!!";
    exit(EXIT_FAILURE);
}

// Consumes input by callng GetToken
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

// Gets a set of nonterminals from ruleSet
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

// Gets a set of terminals from RHS of ruleSet
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

// Gets a set of terminals and nonterminals
vector<string> Project2::get_universe(vector<string> terminals, vector<string> nonterminals)
{
    vector<string> universe;

    universe.insert(universe.end(), "$"); // EOF
    universe.insert(universe.end(), "#"); // Epsilon

    combine_sets(&universe, &terminals);
    combine_sets(&universe, &nonterminals);

    return universe;
}

// Gets a set without epsilon from a given set
vector<string> Project2::get_set_without_epsilon(vector<string> set)
{
    vector<string> termMinusEps;
    for (int i = 0; i < terminals.size(); i++)
    {
        if (terminals.at(i).compare("#") != 0)
        {
            termMinusEps.insert(termMinusEps.end(), terminals.at(i));
        }
    }
    return termMinusEps;
}

// Get set of nonterminals in order of appearance in grammar
vector<string> Project2::get_nonterminal_order(vector<rule> rules, vector<string> nonterminals)
{
    vector<string> inOrder;
    for (int i = 0; i < rules.size(); i++) {
        if (str_is_in_set(&nonterminals, rules.at(i).LHS) && !str_is_in_set(&inOrder, rules.at(i).LHS)) {
            inOrder.insert(inOrder.end(), rules.at(i).LHS);
        }
        
        for (int j = 0; j < rules.at(i).RHS.size(); j++) {
            if (str_is_in_set(&nonterminals, rules.at(i).RHS.at(j)) && !str_is_in_set(&inOrder, rules.at(i).RHS.at(j))) {
                inOrder.insert(inOrder.end(), rules.at(i).RHS.at(j));
            }
        }
    }

    return inOrder;
}


/* Task Helpers */
//-----------------------------------------------------------------------------------------------------------------------
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

// Check if rules can be reached
bool* Project2::check_if_reachable(vector<rule> rulesGen)
{
    vector<string> nonterminals = get_nonterminals(rulesGen);
    bool symbolReach[nonterminals.size()];         // Checks if a given nonterminal is reachable
    bool* rulesReach = new bool [rulesGen.size()]; // Checks if a given rule is reachable

    // Initialize symbol reach, the first symbol is always reachable
    symbolReach[0] = true;
    for (int i = 1; i < nonterminals.size(); i++)
    {
        symbolReach[i] = false;
    }
    
    // Initialize rules reach (Non of the rules are reachable at the beginning)
    for (int i = 0; i < rulesGen.size(); i++)
    {
        rulesReach[i] = false;
    }
    
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

// Returns the first set of a Context-Free Grammar
vector<rule> Project2::get_first_set(vector<rule> rules, vector<string> nonterminals, vector<string> terminals)
{
    vector<string> universe = get_universe(terminals, nonterminals);

    vector<rule> first_set_universe;
    vector<rule> first_set_nonterminals;
    
    rule tempRule;
    for (int i = 0; i < universe.size(); i++)
    {
        tempRule.LHS = universe.at(i);
        first_set_universe.insert(first_set_universe.end(), tempRule);
    }
    
    /* Apply Rule 1 */
    first_set_universe.at(1).RHS.insert(first_set_universe.at(1).RHS.end(), first_set_universe.at(1).LHS);

    /* Apply Rule 2 */
    vector<string> termMinusEps = get_set_without_epsilon(terminals);
    for (int i = 2; i < termMinusEps.size() + 2; i++)
    {
        first_set_universe.at(i).RHS.insert(first_set_universe.at(i).RHS.end(), first_set_universe.at(i).LHS);
    }

    bool changed = false;
    do
    {
        changed = false;
        for (int i = 0; i < rules.size(); i++)
        {
            int indexLHS = get_index(&universe, rules.at(i).LHS);

            /* Apply Rule 5 */
            if (ruleSet.at(i).RHS.at(0).compare("#") == 0 && add_epsilon(&first_set_universe.at(indexLHS).RHS))
            {
                changed = true;
            }

            for (int j = 0; j < ruleSet.at(i).RHS.size(); j++)
            {
                int indexRHS = get_index(&universe, rules.at(i).RHS.at(0));

                /* Apply Rule 3 */
                if (combine_sets(&first_set_universe.at(indexLHS).RHS, &first_set_universe.at(indexRHS).RHS))
                {
                    changed = true;
                }

                bool foundEpsilon = false;
                for (int k = 0; k < ruleSet.at(i).RHS.size(); k++)
                {
                    indexRHS = get_index(&universe, rules.at(i).RHS.at(k));


                    if (is_epsilon_in(&first_set_universe.at(indexRHS).RHS))
                    {
                        /* Apply Rule 4 */
                        if (k < ruleSet.at(i).RHS.size() - 1)
                        {
                            indexRHS = get_index(&universe, rules.at(i).RHS.at(k + 1));
                            if (combine_sets(&first_set_universe.at(indexLHS).RHS, &first_set_universe.at(indexRHS).RHS))
                            {
                                changed = true;
                            }
                        }

                        else
                        {
                            foundEpsilon = true;
                        }
                    }

                    else
                    {
                        break;
                    }
                }

                /* Apply Rule 5 */
                if (foundEpsilon && add_epsilon(&first_set_universe.at(indexLHS).RHS))
                {
                    changed = true;
                }
            }
        }
    } while (changed);

    for (int i = 0; i < first_set_universe.size(); i++)
    {
        if (str_is_in_set(&nonterminals, first_set_universe.at(i).LHS))
        {
            first_set_nonterminals.insert(first_set_nonterminals.end(), first_set_universe.at(i));
        }
    }

    return first_set_nonterminals;
}

// Returns the follow set of a Context-Free Grammar
vector<rule> Project2::get_follow_set(vector<rule> rules, vector<rule> first_set)
{
    
}
//-----------------------------------------------------------------------------------------------------------------------

// Task 0 - Ariel Gutierrez
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
    vector<string> terminals    = get_terminals(ruleSet, nonterminals);
    vector<string> inOrder      = get_nonterminal_order(ruleSet, nonterminals);
    vector<string> termMinusEps = get_set_without_epsilon(terminals);

    print_set(&termMinusEps);
    print_set(&inOrder);
    //cout << "1\n";
}

// Task 2 - Ariel Gutierrez
void Project2::RemoveUselessSymbols()
{
    bool* rulesGenerate = check_if_generate(ruleSet); // Checks which rules generate

    vector<rule> generatingRules; // Contains only the rules that generate
    vector<rule> usefulRules;     // Contains only the rules that are useful (Generate and are Reachable)

    /* Add all rules that generate to another vector of rules */
    for (int i = 0; i < ruleSet.size(); i++)
    {
        if (rulesGenerate[i])
        {
            generatingRules.insert(generatingRules.end(), ruleSet.at(i));
        }
    }
    
    /* If the first symbol does not generate then none of the rules are useful */
    if(!generatingRules.empty() && generatingRules.at(0).LHS.compare(ruleSet.at(0).LHS) == 0)
    {
        bool* rulesReachable = check_if_reachable(generatingRules); // Checks which rules are reachable

        /* Add all rules that are reachable to another vector of rules */
        for (int i = 0; i < generatingRules.size(); i++)
        {
            if (rulesReachable[i])
            {
                usefulRules.insert(usefulRules.end(), generatingRules.at(i));
            }
        }
    }

    /* Print the useful rules */
    for (int i = 0; i < usefulRules.size(); i++)
    {
        rule Rule = usefulRules.at(i);

        cout << Rule.LHS << " -> ";
        print_set(&Rule.RHS);
        cout << "\n";
    }
    
    //cout << "2\n";
}

// Task 3 - Ariel Gutierrez & Salvador Gomez
void Project2::CalculateFirstSets()
{
    vector<string> nonterminals = get_nonterminals(ruleSet);
    vector<string> terminals    = get_terminals(ruleSet, nonterminals);
    vector<rule>   first_set    = get_first_set(ruleSet, nonterminals, terminals);

    vector<string> order = get_set_without_epsilon(terminals);
    order.insert(order.begin(), "#");

    vector<string> inOrder = get_nonterminal_order(ruleSet, nonterminals);

    for (int i = 0; i < inOrder.size(); i++)
    {
        for (int j = 0; j < first_set.size(); j++)
        {
            if (inOrder.at(i).compare(first_set.at(j).LHS) == 0)
            {
                cout << "FIRST(" << first_set.at(j).LHS << ") = { ";
                print_set_in_order(&first_set.at(j).RHS, &order);
                cout << " }\n";
            }
        }
    }

    //cout << "3\n";
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