# Code Description

## EBNF Definition

The following EBNF definition represents the structure of the code. The starting rule is `translation_unit`, and it is designed to be LL(1) compatible:

```
translation_unit ::= { function | enum | class } 

enum ::= "enum" identifier "{" enum_body "}"

enum_body ::=  enum_item {, enum_item }

enum_item ::= identifier [ "(" [type] {,type} ")" ]

class ::= "class" identifier "{" class_body "}"

class_body ::=  { ("public" | "private") ( class_variable_member | method )  }

class_variable_member ::=  ("let" | "const") [static] identifier ":" type = expression ";" 


method ::= "fn" ["static"] identifier "(" [parameter_list] ")" ["->" type] "{" statements "}"


function ::= "fn" identifier "(" ")" "->" type "{" statements "}"

statements ::= (statement)*

statement ::= variable_declaration | constant_declaration | loop | return_statement | identifier ( assignment_expression | call_expression)

assignment_expression := ("="|"+="|"-=") expression ";"

call_expression := "(" [ expression ] { "," expression } ")"

variable_declaration ::= "let" identifier ":" type "=" expression ";"

constant_declaration ::= "const" identifier ":" type "=" expression ";"

loop ::= "while" "(" condition ")" "{" statements "}"

return_statement ::= "return" expression ";"

expression ::= integer | string_literal | array_initialization | binary_expression

array_initialization ::= "[" expression ";" expression "]"

binary_expression ::= identifier [ operator expression ] [ call_expression ]

condition ::= binary_expression

identifier ::= /[a-zA-Z_][a-zA-Z0-9_]*/

integer ::= /\d+/

string_literal ::= /".*"/

operator ::= "<" | "=" | "+" | "*" | "/"

type ::= "i64" | "string" | "[i64;" identifier "]"
```
