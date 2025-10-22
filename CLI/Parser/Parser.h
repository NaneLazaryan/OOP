#pragma once
#include <string>
#include "Tokenizer.h"
#include <memory>
#include <vector>
#include "Command.h"


enum class State {
	START,        // beginning
	ACTION,       // saw a verb (add/remove/edit/set)
	TARGET,       // saw an object (slide/title/shape/bullet)
	ARGUMENTS,    // collecting args like numbers or strings
	DONE,         // command successfully parsed
	ERROR         // invalid sequence
};

class Parser
{
public:
	Parser(std::istream& stream);
	
	std::unique_ptr<Command> parse();
	void processToken(Token token, std::unique_ptr<Command>& cmd);
	//void parseArgs(const ArgumentList& arguments);
	//bool tryParseCoordinate(const std::string& input, std::pair<double, double>& coord);
	/*bool isCoordinateStart(const Token& token);
	bool tryParseCoordinateFromTokens(std::pair<double, double>& coord);*/

private:
	Tokenizer tokenizer;
	Token currentToken;
	Token previousToken;
	State currentState = State::START;

	Token nextToken();

	State transitionTable[20][20];
};
