/*https://stackoverflow.com/questions/15550553/flex-regular-expression-literal-char*/
%{
#define EXIT_OK 0
#define EXIT_FAIL 1

#include <iostream>
#include <vector>
#include "token.h"

std::vector<Token> *tokens_ptr;

extern "C" int yylex();

// global variables
int identifiers = 0;
int lines = 0;
%}
letter [a-zA-Z]
letter_ {letter}|_
digit [0-9]

%%
int {}
if {}
else {}
while {}
return {}
char {}
'([^'\\\n]|\\.)' {}
{letter_}({letter_}|{digit})* { identifiers++; 
printf("line %d: %s\n", lines, yytext); 
tokens_ptr->push_back(Token(Token::Type::identifier, std::string(yytext)));
} 
\n { lines += 1;  }
[\t\r ]+ { /* does nothing when seeing white spaces except new line */ }
. { /* a final rule that does nothing when seeing any character but new line */ }
<<EOF>> { printf("There are %d occurrences of valid identifiers\n", identifiers); yyterminate(); }

%%
std::vector<Token> GetTokens(const std::string& file_path) {
  tokens_ptr = new std::vector<Token>();
  if (!(yyin = fopen(file_path.c_str(), "r"))) {
    perror(file_path.c_str());
    return *tokens_ptr;
  }
  yylex();
  return *tokens_ptr;
}