# Context-Free Grammar Basics
This project was created by @ArielGGutierrez, and @Sgomez47-ASU for a university class focusing on programming languages.

The program reads a description of a context-free grammar and does the following operations based on the grammar:
- It prints the list of non-terminals followed by a list of terminals in the order that they appear in the grammar rules
- It finds useless symbols in the grammar and removes rules with useless symbols
- It calculates FIRST sets
- It calculates FOLLOW sets
- It determines if the grammar has a predictive parser

The descriptions of a context-free grammar must abide by the following CFG:
```
Grammar         -> Rule-list HASH
Rule-list       -> Rule Rule-list | Rule
Id-list         -> ID Id-list | ID
Rule            -> ID ARROW Right-hand-side STAR
Right-hand-side -> Id-list | ε

ID    = letter (letter + digit) *
STAR  = *
HASH  = #
ARROW = ->
```
You can look at example inputs and outputs in the tests folder.
