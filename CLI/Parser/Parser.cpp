#include "Parser.h"
#include <iostream>
#include "IncludsCommands.h"
#include "Argument.h"

Parser::Parser(std::istream& stream) :tokenizer(stream), currentToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN }), previousToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN })
{
	for (size_t i = 0; i < 20; i++)
		for (size_t j = 0; j < 20; j++)
			transitionTable[i][j] = State::ERROR;

	transitionTable[(int)State::START][(int)TokenType::KEYWORD] = State::ACTION;
	transitionTable[(int)State::ACTION][(int)TokenType::KEYWORD] = State::TARGET;
	
	transitionTable[(int)State::TARGET][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::KEYWORD] = State::ARGUMENTS;
	transitionTable[(int)State::TARGET][(int)TokenType::END_OF_LINE] = State::DONE;
	
	// Collecting args
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
	if (token.name == TokenType::UNKNOWN) {
		throw std::invalid_argument("Invalid Command: " + token.value);
	}

	if (token.name == TokenType::KEYWORD && token.keyword == Keyword::UNKNOWN) {
		throw std::invalid_argument("Unexpected Keyword: " + token.value);
	}

	State next = transitionTable[(int)currentState][(int)token.name];

	if (next == State::ERROR) {
		currentState = State::ERROR;
		throw std::invalid_argument("Invalid Command!");
	}

	// Process tokens based on current state
	switch (currentState)
	{
	case State::START:
		if (token.name == TokenType::KEYWORD) {
			switch (token.keyword)
			{
			case Keyword::ADD: 
			case Keyword::REMOVE:
			case Keyword::EDIT: 
			case Keyword::SET: 
				break;
			default: 
				throw std::invalid_argument("Invalid action keyword: " + token.value);
			}
		}
		break;
	case State::ACTION:
		if (token.name == TokenType::KEYWORD) {
			if (previousToken.keyword == Keyword::ADD) {
				switch (token.keyword)
				{
				case Keyword::SLIDE: 
					cmd = std::make_unique<C_AddSlide>(); 
					break;
				case Keyword::SHAPE: 
					cmd = std::make_unique<C_AddShape>(); 
					break;
				default: 
					throw std::invalid_argument("Invalid target for ADD command: " + token.value);
				}
			}
			else if (previousToken.keyword == Keyword::REMOVE) {
				switch (token.keyword)
				{
				case Keyword::SLIDE: 
					cmd = std::make_unique<C_RemoveSlide>(); 
					break;
				case Keyword::SHAPE: 
					cmd = std::make_unique<C_RemoveShape>(); 
					break;
				default: 
					throw std::invalid_argument("Invalid target for REMOVE command: " + token.value);
				}
			}
			else if (previousToken.keyword == Keyword::SET) {
				switch (token.keyword)
				{
				case Keyword::TITLE: 
					cmd = std::make_unique<C_SetTitle>(); 
					break;
				default: 
					throw std::invalid_argument("Invalid target for SET command: " + token.value);
				}
			}
			else {
				throw std::invalid_argument("Invalid target keyword: " + token.value);
			}
		}
		break;
	case State::TARGET:
	case State::ARGUMENTS:
		// Collect arguments and add to the command
		if (cmd) {
			ArgumentPtr arg;
			std::string argName;
			
			switch (token.name) {
			case TokenType::KEYWORD:
				if (token.keyword == Keyword::AT)
					argName = "at";
				else if (token.keyword == Keyword::TYPE)
					argName = "type";
				else if (token.keyword == Keyword::POS)
					argName = "pos";

				arg = std::make_unique<Argument>(ArgType::STRING, token.value);
				break;
			case TokenType::STRING:
				argName = "text";
				arg = std::make_unique<Argument>(ArgType::STRING, token.value);

				// Check if coordinate
				/*std::pair<double, double> coord;
				if (tryParseCoordinate(token.value, coord)) {
					arg = std::make_unique<Argument>("coordinate", ArgType::COORDINATE, coord);
				} else {
					arg = std::make_unique<Argument>(argName, ArgType::STRING, std::string(token.value));
				}*/
				break;
			case TokenType::NUMBER:
				arg = std::make_unique<Argument>(ArgType::NUMBER, std::stof(token.value));
				// Check if this might be the start of a coordinate
				//if (isCoordinateStart(token)) {
				//	// Try to parse coordinate from tokens
				//	std::pair<double, double> coordFromTokens;
				//	if (tryParseCoordinateFromTokens(coordFromTokens)) {
				//		arg = std::make_unique<Argument>("coordinate", ArgType::COORDINATE, coordFromTokens);
				//	} else {
				//		arg = std::make_unique<Argument>(ArgType::NUMBER, std::stof(token.value));
				//	}
				//} else {
				//	arg = std::make_unique<Argument>(ArgType::NUMBER, std::stof(token.value));
				//}
				break;
			case TokenType::SYMBOL:
				arg = std::make_unique<Argument>(ArgType::STRING, std::string(token.value));
				break;
			default:
				break;
			}
			if (arg && !argName.empty()) {
				cmd->addArgument(argName, std::move(arg));
			}
		}
		break;
	default:
		break;
	}

	/*if (cmd && !cmd->getArguments().empty())
		parseArgs(cmd->getArguments());*/

	currentState = next; // move DFA forward
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
