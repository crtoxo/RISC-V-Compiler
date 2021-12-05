# hw4 report

|||
|-:|:-|
|Name|洪珩鈞|
|ID|0716316|

## How much time did you spend on this project

12 hours.

## Project overview

I have specified rules in parser.y related to expression node which hadn't been done in previous homework. I have also add a "dump table function" to determine whether to dump out the symbol table or not. In the most critical part, I have implemented semantic analysis in post order way so that if erro is detected in higher level scope, the lower level semantic analysis will not be executed.

## What is the hardest you think in this project

The criteria of redeclaration and fetching the type of an expression.

## Feedback to T.A.s

The spec can specify more detail on which entry should be dropped if redeclaration occurs. For instance, funcSymbolRedecl() in second test case is dropped but the symbol table it creates still remains.
