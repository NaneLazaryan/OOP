#include "Parser.h"
#include <iostream>
#include "IncludsCommands.h"
#include "Argument.h"
#include "CommandFactory.h"

Parser::Parser(std::istream& stream) :tokenizer(stream), currentToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN }), previousToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN })
{
	initializeTransitionTable();
}

void Parser::initializeTransitionTable()
{
	for (size_t i = 0; i < 20; i++)
		for (size_t j = 0; j < 20; j++)
			transitionTable[i][j] = State::ERROR;

	// Valid state transitions
	transitionTable[(int)State::START][(int)TokenType::KEYWORD] = State::ACTION;
	transitionTable[(int)State::ACTION][(int)TokenType::KEYWORD] = State::TARGET;

	transitionTable[(int)State::TARGET][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::KEYWORD] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::END_OF_LINE] = State::DONE;

	// Collecting arguments
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::KEYWORD] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::SYMBOL] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::END_OF_LINE] = State::DONE;
}

Token Parser::nextToken()
{
	return tokenizer.tokenize();
}

void Parser::validateToken(const Token& token)
{
	if (token.name == TokenType::UNKNOWN) {
		throw std::invalid_argument("Invalid token: " + token.value);
	}

	if (token.name == TokenType::KEYWORD && token.keyword == Keyword::UNKNOWN) {
		throw std::invalid_argument("Unknown keyword: " + token.value);
	}
}

std::unique_ptr<Command> Parser::parse()
{
	std::unique_ptr<Command> cmd;
	while (currentState != State::DONE && currentState != State::ERROR) {
		previousToken = currentToken;  // Store previous token
		currentToken = nextToken();
		
		processToken(currentToken, cmd);
	}

	if (currentState == State::ERROR) {
		throw std::invalid_argument("Parsing failed - invalid command sequence");
	}

	if (!cmd) {
		throw std::invalid_argument("No valid command was parsed");
	}

	return cmd;
}

void Parser::processToken(Token token, std::unique_ptr<Command>& cmd) {
	validateToken(token);

	State next = transitionTable[(int)currentState][(int)token.name];

	if (next == State::ERROR) {
		currentState = State::ERROR;
		throw std::invalid_argument("Invalid Command!");
	}

	// Process tokens based on current state
	switch (currentState)
	{
	case State::START:
		processStartState(token);
		break;
	case State::ACTION:
		processActionState(token,cmd);
		break;
	case State::TARGET:
	case State::ARGUMENTS:
		processArgumentsState(token, cmd);
		break;
	default:
		break;
	}

	currentState = next; // move DFA forward
}

void Parser::processStartState(const Token& token)
{
	if (token.name == TokenType::KEYWORD) {
		switch (token.keyword)
		{
		case Keyword::ADD:
		case Keyword::REMOVE:
		case Keyword::EDIT:
		case Keyword::SET:
			break; // Valid action keywords
		default:
			throw std::invalid_argument("Invalid action keyword: " + token.value);
		}
	}
}

void Parser::processActionState(const Token& token, std::unique_ptr<Command>& cmd)
{
	if (token.name == TokenType::KEYWORD) {
		try {
			cmd = CommandFactory::createCommand(previousToken.keyword, token.keyword);
		}
		catch (const std::invalid_argument&) {
			throw std::invalid_argument("Invalid command combination: " + previousToken.value + " " + token.value);
		}
	}
}

void Parser::processArgumentsState(const Token& token, std::unique_ptr<Command>& cmd)
{
	if (!cmd) return;

	ArgumentPtr arg;
	std::string argName;

	switch (token.name)
	{
	case TokenType::KEYWORD:
		if (token.keyword == Keyword::AT)
			argName = "at";
		else if (token.keyword == Keyword::TYPE)
			argName = "type";
		else if (token.keyword == Keyword::POS)
			argName = "pos";

		if (!argName.empty()) {
			arg = std::make_unique<Argument>(ArgType::STRING, token.value);
		}
		break;
	case TokenType::STRING:
		argName = "text";
		arg = std::make_unique<Argument>(ArgType::STRING, token.value);
		break;
	case TokenType::NUMBER:
		argName = "number";
		arg = std::make_unique<Argument>(ArgType::NUMBER, std::stof(token.value));
		break;
	case TokenType::SYMBOL:
		argName = "symbol";
		arg = std::make_unique<Argument>(ArgType::STRING, std::string(token.value));
		break;
	default:
		break;
	}

	if (arg && !argName.empty()) {
		cmd->addArgument(argName, std::move(arg));
	}
}

//bool Parser::isCoordinateStart(const Token& token)
//{
//	// A coordinate starts with a number
//	return token.name == TokenType::NUMBER;
//}
//
//bool Parser::tryParseCoordinateFromTokens(std::pair<double, double>& coord)
//{
//	// This method will be called when we're in COORDINATE state
//	// We need to look ahead to get the comma and second number
//	Token commaToken = nextToken();
//	if (commaToken.name != TokenType::SYMBOL || commaToken.value != ",") {
//		return false;
//	}
//	
//	Token yToken = nextToken();
//	if (yToken.name != TokenType::NUMBER) {
//		return false;
//	}
//	
//	try {
//		coord.first = std::stof(currentToken.value);  // x from current token
//		coord.second = std::stof(yToken.value);       // y from next token
//		return true;
//	}
//	catch (const std::exception&) {
//		return false;
//	}
//}