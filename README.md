# Werid (PL/0 to C compiler front-end)

A fully-fledged <a href="https://en.wikipedia.org/wiki/PL/0">PL/0</a> to C compiler front-end. 
consists of lexer, parser and code-gen.

## Make

Just run:
```
make
```

#### The Grammer for PL/0:

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

TODO:
 [ ] Implement the Symbol table and scoping into parser.
 [ ] all procedures return void expect main, the parser should
     keep track of wethear main or not.
 [ ] allow no nested procedure.

### /TMP

given the following PL/0 code at `test/isprime.pl`:

<img width="363" alt="Screenshot 2024-06-06 at 21 10 52" src="https://github.com/metflekx/weird/assets/106245122/c992dd89-716a-4949-8ae6-ee39232f8160">

```Shell
apt-install update && apt install clang-format && ./weird test/isprime.pl | clang-format
```

Will write the following to the stdout:

<img width="380" alt="Screenshot 2024-06-06 at 21 14 45" src="https://github.com/metflekx/weird/assets/106245122/18f041e6-60dc-41bb-a092-bca69f8ce795">
