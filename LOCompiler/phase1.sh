#!/usr/bin/env swipl

:- initialization top.

:- use_module('../BootCompiler/lo').

eval :-
        current_prolog_flag(argv, Argv),
        main(Argv).

top :-
        catch(eval, E, (print_message(error, E), fail)),
        halt.
top :-
        halt(1).
