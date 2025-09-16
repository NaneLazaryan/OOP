#pragma once
#include <string>
#include <vector>

enum class TokenType
{
	ADD, REMOVE, EDIT, SET,
	SLIDE, TITLE, BULLET, SHAPE,
	TYPE, AT,
	STRING, IDENT, NUMBER,
	LPAREN, RPAREN, COMMA,
	END, UNKNOWN
};

struct Token
{
	TokenType name;
	std::string value;
};

class Tokenizer
{
public:
	Tokenizer(std::string str) : input(str) {}

	TokenType lookupKeyword(const std::string& word);
	std::vector<Token> tokenize();

private:
	std::string input;

	bool isEnd(int i) const;
	//char peek(int i);
	//char get(int i);
};
