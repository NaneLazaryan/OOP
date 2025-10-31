#include "Parser.h"
#include <iostream>
#include "IncludsCommands.h"
#include "Argument.h"
#include "CommandFactory.h"

Parser::Parser(std::istream& stream) 
	: tokenizer(stream), 
	currentToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN }), 
	previousToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN }),
	currentFlagName(""),
	expectedValueType(ArgType::STRING)
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

	// Should see the value token
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::KEYWORD] = State::ARGUMENTS;
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

CommandPtr Parser::parse()
{
	CommandPtr cmd;
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

void Parser::processToken(Token token, CommandPtr& cmd) {
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
	case State::EXPECTING_VALUE:
		processExpectingValueState(token, cmd);
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

void Parser::processActionState(const Token& token, CommandPtr& cmd)
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

void Parser::processArgumentsState(const Token& token, CommandPtr& cmd)
{
	if (!cmd) return;

	// Check for -at, -type, ....
	if (isArgumentFlag(token)) {
		currentFlagName = token.value;
		expectedValueType = determineExpectedType(token.keyword);

		currentState = State::EXPECTING_VALUE;
		return;
	}

	ArgumentPtr arg;
	std::string argName;

	switch (token.name)
	{
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

void Parser::processExpectingValueState(const Token& token, CommandPtr& cmd)
{
	if (!cmd) return;

	ArgumentPtr arg = createArgument(token, expectedValueType);

	if (arg) {
		std::string flagName = currentFlagName;
		if (!flagName.empty() && flagName[0] == '-') {
			flagName = flagName.substr(1);
		}

		cmd->addArgument(flagName, std::move(arg));
	}

	currentFlagName.clear();
}

bool Parser::isArgumentFlag(const Token& token) const
{
	return token.name == TokenType::KEYWORD && (token.keyword == Keyword::AT || token.keyword == Keyword::TYPE || token.keyword == Keyword::POS || token.keyword == Keyword::SHAPE);
}

ArgType Parser::determineExpectedType(Keyword flag) const
{
	switch (flag)
	{
	case Keyword::AT:
		return ArgType::INT;
	case Keyword::TYPE:
		return ArgType::STRING;
	case Keyword::POS:
		return ArgType::COORDINATE;
	default:
		return ArgType::STRING;
	}
}

ArgumentPtr Parser::createArgument(const Token& token, ArgType expectedType)
{
	try {
		switch (expectedType)
		{
		case ArgType::STRING:
			if (token.name == TokenType::STRING || token.name == TokenType::KEYWORD) {
				return std::make_unique<Argument>(ArgType::STRING, token.value);
			}
			break;
		case ArgType::INT:
			if (token.name == TokenType::NUMBER) {
				return std::make_unique<Argument>(ArgType::INT, std::stoi(token.value));
			}
			break;
		case ArgType::NUMBER:
			if (token.name == TokenType::NUMBER) {
				return std::make_unique<Argument>(ArgType::NUMBER, std::stof(token.value));
			}
			break;
		case ArgType::COORDINATE:
			if (token.name == TokenType::NUMBER) {
				float x = std::stof(token.value);
				Token comma = nextToken();
				if (comma.name == TokenType::SYMBOL && comma.value == ",") {
					Token yToken = nextToken();
					if (yToken.name == TokenType::NUMBER) {
						float y = std::stof(yToken.value);
						return std::make_unique<Argument>(ArgType::COORDINATE, std::make_pair(static_cast<double>(x), static_cast<double>(y)));
					}
				}

				throw std::invalid_argument("Invalid coordinate format. Expected: x,y");
			}
			break;
		}
	}
	catch (const std::exception& ex) {
		throw std::invalid_argument(std::string("Failed to parse argument value: ") + ex.what());
	}
}