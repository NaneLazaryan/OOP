#include "Tokenizer.h"
#include <unordered_map>
#include <algorithm>

using namespace cli;

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

Token Tokenizer::tokenize()
{
	// Skip whitespace
	while (!isEnd() && std::isspace(static_cast<unsigned char>(peek()))) {
		get();
	}

	if (isEnd()) {
		return { TokenType::END_OF_LINE, "" };
	}

	char c = peek();

	// Identifiers (includes commands and flags starting with -)
	// Also handles file extensions like .json, .svg
	if (std::isalpha(static_cast<unsigned char>(c)) || c == '-' || c == '.') {
		std::string word;

		// Handle flags starting with - or file extensions starting with .
		if (c == '-' || c == '.') {
			word.push_back(get());
		}

		
		while (!isEnd() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '-' || peek() == '.')) {
			word.push_back(get());
		}

		return { TokenType::IDENTIFIER, word };
	}
	// Numbers 
	else if (std::isdigit(static_cast<unsigned char>(c))) {
		std::string number;

		// Read digits and decimal point
		while (!isEnd() && (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.')) {
			number.push_back(get());
		}

		return { TokenType::NUMBER, number };
	}
	// Quoted strings
	else if (c == '"') {
		get(); // skip opening quote
		std::string str;

		while (!isEnd() && peek() != '"') {
			if (peek() == '\\' && !isEnd()) {
				get(); // skip escape character
				if (!isEnd()) {
					str.push_back(get()); // add escaped character
				}
			}
			else {
				str.push_back(get());
			}
		}

		if (!isEnd() && peek() == '"') {
			get(); // consume closing quote
		}

		return { TokenType::STRING, str };
	}
	// Symbols
	else if (c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == ';') {
		std::string symbol(1, get());
		return { TokenType::SYMBOL, symbol };
	}

	// Unknown character
	get();
	return { TokenType::UNKNOWN, std::string(1, c) };
}