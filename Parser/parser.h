#pragma once
#include <string>
#include "tokenizer.h"
#include <memory>
#include <vector>

enum class State {
	START,        // beginning
	ACTION,       // saw a verb (add/remove/edit/set)
	TARGET,       // saw an object (slide/title/shape/bullet)
	ARGUMENTS,    // collecting args like numbers or strings
	DONE,         // command successfully parsed
	ERROR         // invalid sequence
};

enum class CommandType {
	Add,
	Remove,
	Edit,
	Set,
	Unknown
};

enum class ObjectType {
	Slide,
	Title,
	Shape,
	Bullet,
	Unknown
};

struct Key
{
	State state;
	std::string token;
};

struct Command
{
	CommandType action;
	ObjectType target;
	std::vector<std::string> args;
};
class Parser
{
public:
	Parser(std::istream& stream);
	
	//Command parse(std::istream& stream);
	Command parse();
	void processToken(Token token, Command& cmd);

private:
	Tokenizer tokenizer;
	Token currentToken;
	State currentState = State::START;

	Token nextToken();

	State transitionTable[20][20];
};
