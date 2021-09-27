# Nabd ("Nabbed")

## Description

A simple, functional esoteric programming language.

The name is an acronym standing for "Not A B.f. Derivative"

## Modules

Code is organized into modules and the module file directories can be passed into the compiler with `-I<folder name>` like headers in gcc.

Modules can either be nabd code *or* C++ headers with a corresponding object file (determined by file extension).

For reference, look at the standard library implementation as that is a C++ library.

## Instructions

I've placed spaces, but they're not required

| Instruction | Description |
|:-:|:-:|
| `$ _ $` | Reads code from the module within dollar signs. If a module contains `$`, an escape sequence (`\$`) can be used |
| `_ = _ > _ .` | Define a function with name (first \_) and parameter (second \_) of some type (third \_) returning an expression (last \_). Polymorphism is allowed. |
| `_ ( _ )` | Call function (first \_) with parameter (second \_) |
| `[ _ :> _ , _ , _ , ... ]` | Expression denoting a list with type first \_ of comma separated sub expressions |
| `'_'` | Expression denoting a string, just like in other programming languages |
| `0d_#` | Expression denoting a decimal number. |
| `0x_#` | Enter a number as hex |
| `{ _ :> _ , _ :> _ }` | Expression denoting a tuple |
| `! _ ? _ : _` | If an expression converted to a number is > 0, then expression is first, otherwise second

Types:
 - `String` - a string
 - `Number` - a decimal number
 - `Tuple` - two objects of different types packed together
 - `List` - a collection of objects of the same types

Note: all types can be converted to each other and will not error out. For instance, if a string is implicitly cast as a number, or the cast fails to convert it properly to a number, the first character will be converted to its ascii representation

Code is organized into functions, and the first function called is `main`, which takes a list of strings as an argument

## Examples

__Hello World:__

`$std$main=args>print('Hello, world!\n').`

__Truth Machine:__

`$std$i1InfI0Stop=!loop>loop?i1InfI0Stop(print(0d1#)):print(0d0#).main=args>i1InfI0Stop(parseNum(input(0d0#))).`

or

```
$std$
i1InfI0Stop = loop >
    ! loop ? i1Inf0Stop(print(0d1#)) : print(0d0#).
main = args >
    i1InfI0Stop(
        parseNum(input(0d0#))).
```

## Standard Library Functions

These are built-in functionality to add a lot to the language.
It will be increased overtime

 - `print` - prints the value passed in to stdout and returns the string printed to console
 - `input` - read a string from stdin. Does nothing with parameter
 - `len` - returns the length of what's passed in. For tuples it's always 2, for numbers it's 1, and for lists and strings it's the actual length
 - `swap` - takes the first item of the tuple parameter and interprets it as a number and returns a copy of the second item with its first-parameter-th index replaced by the third element. If the index is outside the second parameter, then it simply returns the original parameter.
 - `int` - takes input and attempts to convert it to an integer or defaults to implicit cast if it fails
 - `gt` - takes two inputs via a tuple, compares them, and returns 1 if param 1 \> param 2 or -1 otherwise
 - `ls` - same as greater but with \<
 - `eq` - same as greater but with =
 - `gte` - same as greater but with \>=
 - `lse` - same as greater but with \<=
 - `ne` - same as greater but with !=
 - `parseNum` - don't just cast to a number, but try to actually parse a number out
 - `seedRandom` - takes a dummy parameter and seeds a random given the current time. Returns 0
 - `random` - given a tuple with range min to max, return a random number
 - `ignore` - takes a tuple and returns the second. Can be used to emulate do syntax in Haskell
 - `dup` - duplicates an item and creates a tuple of the two
