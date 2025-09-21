#include "tokenizer.h"
#include <unordered_map>
#include <algorithm>

std::unordered_map<std::string, TokenType> keywords{
	{"add", TokenType::ADD},
	{"remove", TokenType::REMOVE},
	{"slide", TokenType::SLIDE},
	{"set", TokenType::SET},
	{"title", TokenType::TITLE},
	{"edit", TokenType::EDIT},
	{"shape", TokenType::SHAPE},
	{"title", TokenType::TITLE},
	{"bullet", TokenType::BULLET}
};

TokenType Tokenizer::lookupKeyword(const std::string& word)
{
	std::string lower = word;
	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
		return static_cast<char>(std::tolower(c));
		});
	auto it = keywords.find(lower);

	if (it != keywords.end()) return it->second;
	return TokenType::UNKNOWN;
}

bool Tokenizer::isEnd(int i) const
{
	return i >= input.size();
}

Token Tokenizer::tokenize()
{
	while (pos < input.size() && std::isspace(input[pos])) ++pos;

	if (pos >= input.size()) return { TokenType::END, "" };

	char c = input[pos];

	// Identifiers / Keywords / CLI flags
	if (std::isalpha(c) || c == '-') {
		std::string word;

		if (c == '-') {
			word.push_back(input[pos++]);
		}

		while (!isEnd(pos) && std::isalpha(input[pos]))
			word.push_back(input[pos++]);


		TokenType type = lookupKeyword(word);
		return { type, word };
	}
	// Numbers
	else if (std::isdigit(c)) {
		std::string number;
		while (!isEnd(pos) && std::isdigit(input[pos]))
			number.push_back(input[pos++]);

		return { TokenType::NUMBER, number };
	}
	// Strings
	else if (c == '"') {
		pos++;  // skip opening quote
		std::string str;

		while (!isEnd(pos) && input[pos] != '"')
			str.push_back(input[pos++]);

		if (!isEnd(pos) && input[pos] == '"') pos++; // consume closing quote
		return { TokenType::STRING, str };
	}

	// Symbols
	if (c == '(') {
		++pos;
		return { TokenType::RPAREN, ")" };
	}
	if (c == ')') {
		++pos;
		return { TokenType::RPAREN, ")" };
	}
	if (c == ',') {
		++pos;
		return Token{ TokenType::COMMA, "," };
	}

	// Unknown
	++pos;
	return Token{ TokenType::UNKNOWN, std::string(1, c) };

}
