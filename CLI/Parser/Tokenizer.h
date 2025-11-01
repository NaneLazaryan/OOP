#pragma once
#include <string>
#include <vector>
#include <istream>

namespace cli
{
	enum class TokenType
	{
		KEYWORD,     // CREATE, ADD, SET, SLIDE, TEXT, IMAGE, TITLE
		NUMBER,
		STRING,
		SYMBOL,      // {}, (), ;
		END_OF_LINE,
		UNKNOWN
	};

	enum class Keyword
	{
		ADD, REMOVE, EDIT, SET, AT,
		SLIDE, TITLE, BULLET, SHAPE,
		CIRCLE, TYPE, POS, TEXT,
		UNKNOWN
	};

	struct Token
	{
		TokenType name;
		std::string value;
		Keyword keyword;
	};


	class Tokenizer
	{
	public:
		Tokenizer(std::istream& input) : stream(input), buff_pos(0), buff_size(0) {}

		Keyword lookupKeyword(const std::string& word);
		Token tokenize();
	private:
		std::istream& stream;
		static constexpr size_t BUFF_SIZE = 1024;
		char buffer[BUFF_SIZE];
		size_t buff_pos;
		size_t buff_size;

		bool fillBuffer();
		char peek(); // current character
		char get();  // next character
		bool isEnd() const;
	};
}
