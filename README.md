# AutomataSynchronizationChecker
SynchronizationChecker is a project for my master's thesis in UrFU.
There is implementation of Berlinkov's algorithm for automata synchronizing checking with linear expected time. (http://arxiv.org/abs/1304.5774)

That is Visual Studio 2013 solution, which includes 2 projects. SynchronizationChecker contains implementation of the main algorithm and required structures.
Tests project contains tests for crucial parts of the code.

After building of the code you can use SynchronizationChecker.exe to check if a particular automaton is synchronizing.
You have to write input automata in the input file using following template:

```
(Number of states) (Number of transitions) (Alphabet size)
(From state) (To state) (Letter) //transition description
(From state) (To state) (Letter)
...
(From state) (To state) (Letter)
```

Examples of input automata you can see in "SynchronizationChecker/input.txt".