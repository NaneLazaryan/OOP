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
private:
	Tokenizer tokenizer;
	Token currentToken;
	Token previousToken;
	State currentState = State::START;

	Token nextToken();

	State transitionTable[20][20];
private:
	void initializeTransitionTable();
	void validateToken(const Token& token);

	void processToken(Token token, std::unique_ptr<Command>& cmd);

	// State-specific processors
	void processStartState(const Token& token);
	void processActionState(const Token& token, std::unique_ptr<Command>& cmd);
	void processArgumentsState(const Token& token, std::unique_ptr<Command>& cmd);
};