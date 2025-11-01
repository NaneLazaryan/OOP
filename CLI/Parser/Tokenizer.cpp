#include "Tokenizer.h"
#include <unordered_map>
#include <algorithm>

using namespace cli;

std::unordered_map<std::string, Keyword> keywords{
	{"add", Keyword::ADD},
	{"remove", Keyword::REMOVE},
	{"slide", Keyword::SLIDE},
	{"set", Keyword::SET},
	{"title", Keyword::TITLE},
	{"edit", Keyword::EDIT},
	{"shape", Keyword::SHAPE},
	{"bullet", Keyword::BULLET},
	{"-at", Keyword::AT},
	{"-type", Keyword::TYPE},
	{"circle", Keyword::CIRCLE},
	{"-pos", Keyword::POS},
	{"-text", Keyword::TEXT},
	{"load", Keyword::LOAD},
	{"save", Keyword::SAVE}
};

bool Tokenizer::fillBuffer()
{
	if (buff_pos >= buff_size) {
		stream.read(buffer, BUFF_SIZE);  // reads new chunk of data
		buff_size = stream.gcount();
		buff_pos = 0;

	}
	return buff_size > 0;
}

bool Tokenizer::isEnd() const
{
	return buff_pos >= buff_size && stream.eof();
}

char Tokenizer::peek()
{
	if (buff_pos >= buff_size && !fillBuffer()) {
		return '\0';
	}

	return buffer[buff_pos];
}

char Tokenizer::get()
{
	if (buff_pos >= buff_size && !fillBuffer()) {
		return '\0';
	}

	return buffer[buff_pos++];
}

Keyword Tokenizer::lookupKeyword(const std::string& word)
{
	std::string lower = word;
	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
		return static_cast<char>(std::tolower(c));
		});
	auto it = keywords.find(lower);

	if (it != keywords.end()) return it->second;
	return Keyword::UNKNOWN;
}

Token Tokenizer::tokenize()
{
	// Skip whitespace
	while (!isEnd() && std::isspace(static_cast<unsigned char>(peek()))) {
		get();
	}

	if (isEnd()) {
		return { TokenType::END_OF_LINE, "", Keyword::UNKNOWN };
	}

	char c = peek();

	// Identifiers / Keywords / CLI flags
	if (std::isalpha(static_cast<unsigned char>(c)) || c == '-') {
		std::string word;

		if (c == '-') {
			word.push_back(get());
		}

		while (!isEnd() && std::isalnum(static_cast<unsigned char>(peek())))
			word.push_back(get());

		Keyword kyw = lookupKeyword(word);
		return { TokenType::KEYWORD, word, kyw };
	}
	// Numbers
	else if (std::isdigit(static_cast<unsigned char>(c))) {
		std::string number;
		while (!isEnd() && std::isdigit(static_cast<unsigned char>(peek())))
			number.push_back(get());

		return { TokenType::NUMBER, number, Keyword::UNKNOWN };
	}
	// Strings
	else if (c == '"') {
		get();  // skip opening quote
		std::string str;

		while (!isEnd() && peek() != '"')
			str.push_back(get());

		if (!isEnd() && peek() == '"') get(); // consume closing quote
		return { TokenType::STRING, str, Keyword::UNKNOWN };
	}

	// Symbols
	if (c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == ';') {
		std::string symbol(1, get());
		return { TokenType::SYMBOL, symbol, Keyword::UNKNOWN };
	}

	// Unknown
	get();
	return { TokenType::UNKNOWN, std::string(1, c), Keyword::UNKNOWN };
}