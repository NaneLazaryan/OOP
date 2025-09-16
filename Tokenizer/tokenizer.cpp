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
    auto it = keywords.find(word);

    if (it != keywords.end()) return it->second;
    return TokenType::UNKNOWN;
}

bool Tokenizer::isEnd(int i) const
{
	return i >= input.size();
}

//char Tokenizer::peek(int i)
//{
//	if (!isEnd(i)) return input[i];
//	return '\0';
//}
//
//char Tokenizer::get(int i)
//{
//	if (!isEnd(i)) return input[i++];
//	return '\0';
//}

std::vector<Token> Tokenizer::tokenize()
{
	std::vector<Token> tokens;

	for (size_t i = 0; i < input.size();) {
		if (std::isspace(input[i])) { i++; continue; }

		// Identifiers / Keywords / CLI flags
		if (std::isalpha(input[i]) || input[i] == '-') {
			std::string word;

			if (input[i] == '-') {
				word.push_back(input[i++]);
			}

			while (!isEnd(i) && std::isalpha(input[i]))
				word.push_back(input[i++]);


			TokenType type = lookupKeyword(word);
			tokens.push_back({ type, word });
			continue;

		}
		// Numbers
		else if (std::isdigit(input[i])) {
			std::string number;
			while (!isEnd(i) && std::isdigit(input[i]))
				number.push_back(input[i++]);

			tokens.push_back({ TokenType::NUMBER, number });
			continue;
		}
		// Strings
		else if (input[i] == '"') {
			i++;  // skip opening quote
			std::string str;

			while (!isEnd(i) && input[i] != '"')
				str.push_back(input[i++]);

			if (!isEnd(i) && input[i] == '"') i++; // consume closing quote
			tokens.push_back({ TokenType::STRING, str });
			continue;
		}

		// Symbols
		if (input[i] == '(') {
			tokens.push_back({ TokenType::LPAREN,"(" });
			i++;
			continue;
		}

		if (input[i] == ')') {
			tokens.push_back({ TokenType::RPAREN, ")" });
			i++;
			continue;
		}

		if (input[i] == ',') {
			tokens.push_back({ TokenType::COMMA, "," });
			i++;
			continue;
		}

		// Unknown
		tokens.push_back({ TokenType::UNKNOWN, std::string(1, input[i]) });

	}

	tokens.push_back({ TokenType::END, "" });
	return tokens;
}
