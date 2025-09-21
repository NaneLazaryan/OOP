#pragma once
#include <string>
#include <vector>
#include <istream>

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
	Tokenizer(std::string str) : input(str), pos(0) {}

	TokenType lookupKeyword(const std::string& word);
	Token tokenize();

private:
	std::string input;
	size_t pos;

	bool isEnd(int i) const;
};
