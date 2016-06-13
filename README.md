# A simple latex-like formula translator
* limited token kind
* simplify grammar
* rigid output format(html absolute position)

formular_translator support two kinds of input method:

1. read the formula code from stdin with a 
2. specify the formula file path as a parameter of main function

----------------------------------------------------------------

## Ubuntu 14.04

Use one command to build the project:

```bash
$ bash run.sh
```
This command compile the sources(which are needed), and execute the program.

The default input stream is set to the file: ```Project/input/formula.in```.
If the formula will be inputted when running the program,
run the command ```./bin/formula_translator```(assuming that the current directory is Project/).

----------------------------------------------------------------

## Windows 10

The source code in ```Project/windows/``` directory simply concatenate all the source files into one.
The source file may be out dated cause it will not be updated everytime some bugs fixed or some functions added.

----------------------------------------------------------------

### ***ATTENTION***

***The file should be remain unchanged, especially the ```input/```, ```output/``` and ```bin/``` directories.***
***If those three directories were changed or removed, the program may not function and break down.***
***The file path error detection is to be done.***
