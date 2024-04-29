# Werid (PL/0 to C compiler front-end)

A fully-fledged <a href="https://en.wikipedia.org/wiki/PL/0">PL/0</a> to C compiler front-end. 
consists of lexer, parser and code-gen.

The Grammer for PL/0:

 ```CFG
 program = b "." .
 b     = [ "const" ident "=" number { "," ident "=" number } ";" ]
       [ "var" ident { "," ident } ";" ]
       { "procedure" ident ";" b ";" } st .
 st    = [ident ":=" expr
       | "call" ident
       | "begin" st { ";" st } "end"
       | "if" cond "then" st
       | "while" cond "do" st ] .
 cond  = "odd" expr
       | expr ( "=" | "#" | "<" | ">" ) e<Escape>xpr .
 expr  = [ "+" | "-" ] term { ( "+" | "-" ) term } .
 term  = factor { ( "*" | "/" ) factor } .
 factor= ident
       | number
       | "(" expr ")" .
 ident = "A-Za-z_" { "A-Za-z0-9_" } .
 number= "0" | "1" | "2" | "3" | "4" 
       | "5" | "6" | "7" | "8" | "9" .

```
<q>b: block, st: statement, cond: condition, expr: epression</q>
