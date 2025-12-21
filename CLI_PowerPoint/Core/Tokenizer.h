#pragma once
#include <string>
#include <vector>
#include <istream>

namespace cli
{
	enum class TokenType
	{
		IDENTIFIER,  // Any word (commands, flags, arguments)
		NUMBER,
		STRING,
		SYMBOL,      // {}, (), ;
		END_OF_LINE,
		UNKNOWN
	};

	struct Token
	{
		TokenType type;
		std::string value;
	};


	class Tokenizer
	{
	public:
		Tokenizer(std::istream& input) : stream(input), buff_pos(0), buff_size(0) {}

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
