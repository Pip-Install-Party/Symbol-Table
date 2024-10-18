# Component 3 - Recursive Descent Parser

This program is not yet functional. 

This program uses a series of deterministic finite state automata (DFAs) to remove comments from input file(s) and tokenize the contents before creating a concrete syntax tree (CST) using a recursive descent parsing technique.

Authored by: [Blake Marshall](https://github.com/officialblake), [Brandon Robinson](https://github.com/brandonuscg), [Holden Ea](https://github.com/holdenkea), [Rolando Yax](https://github.com/Ryax3), and [Jacob Sellers](https://github.com/JacobS999).

## Running the program

This project can be run via make.

```make```

```./parse.x```

If you have a Windows based machine, you will need to adjust the Makefile to generate a ```.exe``` executable, rather than ```.x```.

## Viewing the output 

After selecting "Recursive Descent Parser" from the menu and choosing a file, the program will generate an output file with the resutls. 

For example, if choosing ```"Tests/Program3/programming_assignment_3-test_file_3.c```, the output can be found in ```test_file_3_output.txt```.
