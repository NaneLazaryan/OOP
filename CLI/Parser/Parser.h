#pragma once
#include <string>
#include "Tokenizer.h"
#include <memory>
#include <vector>
#include "Command.h"


enum class State {
	START,				// beginning
	ACTION,				// saw a verb (add/remove/edit/set)
	TARGET,				// saw an object (slide/title/shape/bullet)
	ARGUMENTS,			// collecting args like numbers or strings
	EXPECTING_VALUE,	// saw a flag, expecting its value next
	DONE,				// command successfully parsed
	ERROR				// invalid sequence
};

class Parser
{
public:
	Parser(std::istream& stream);
	
	CommandPtr parse();
private:
	Tokenizer tokenizer;
	Token currentToken;
	Token previousToken;
	State currentState = State::START;
	State transitionTable[20][20];

	std::string currentFlagName;
	ArgType expectedValueType;

private:
	void initializeTransitionTable();
	void validateToken(const Token& token);

	Token nextToken();

	void processToken(Token token, CommandPtr& cmd);

	// State-specific processors
	void processStartState(const Token& token);
	void processActionState(const Token& token, CommandPtr& cmd);
	void processArgumentsState(const Token& token, CommandPtr& cmd);
	void processExpectingValueState(const Token& token, CommandPtr& cmd);

	// Helpers
	bool isArgumentFlag(const Token& token) const;
	ArgType determineExpectedType(Keyword flag) const;
	ArgumentPtr createArgument(const Token& token, ArgType expectedType);
};