#include "Parser.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace cli;
using namespace cli::cmd;
using namespace cli::cmd::factory;

Parser::Parser(std::istream& stream, CommandRegister& cmdRegister)
	: tokenizer(stream), 
	cmdRegister(cmdRegister),
	currentState(State::START)
{
	initializeTransitionTable();
}

void Parser::initializeTransitionTable()
{
	for (size_t i = 0; i < MAX_STATES; i++)
		for (size_t j = 0; j < MAX_TOKEN_TYPES; j++)
			transitionTable[i][j] = State::ERROR;

	// Valid state transitions
	transitionTable[(int)State::START][(int)TokenType::KEYWORD] = State::ACTION;

	transitionTable[(int)State::ACTION][(int)TokenType::KEYWORD] = State::TARGET;
	transitionTable[(int)State::ACTION][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::ACTION][(int)TokenType::END_OF_LINE] = State::DONE;

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

	// Standalone commands (LOAD, SAVE)
	transitionTable[(int)State::ACTION][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::ACTION][(int)TokenType::END_OF_LINE] = State::DONE;
}

bool Parser::isArgumentFlag(const Token& token) const
{
	return token.name == TokenType::KEYWORD && !token.value.empty() && token.value[0] == '-';
}

std::string Parser::toLower(const std::string& str) const
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), 
		[](unsigned char c) { return std::tolower(c); });
	return result;
}

cmd::factory::ArgValue Parser::convertTokenToArgValue(const std::string& value, TokenType type) const
{
	switch (type) {
	case TokenType::NUMBER:
		try {
			int intVal = std::stoi(value);
			if (value.find('.') != std::string::npos) {
				return static_cast<float>(std::stof(value));
			}
			return intVal;
		}
		catch (...) {
			return std::string(value);
		}
	case TokenType::STRING:
		//return std::string(value);
	case TokenType::KEYWORD:
		//return std::string(value);
	default:
		return std::string(value);
	}
}

cmd::CommandPtr Parser::parse()
{
	// Reset state for new parsing
	currentState = State::START;
	cmd.clear();
	object.clear();
	flags.clear();
	args.clear();
	currentFlag.clear();
	errorMsg.clear();

	while (currentState != State::DONE && currentState != State::ERROR) {
		Token tok = tokenizer.tokenize();

		int stateIdx = static_cast<int>(currentState);
		int tokenIdx = static_cast<int>(tok.name);
		State next = transitionTable[stateIdx][tokenIdx];

		if (next == State::ERROR) {
			errorMsg = "Unexpected token '" + tok.value + "' in state " + std::to_string(stateIdx);
			throw std::invalid_argument(errorMsg);
		}

		// if we're in ARGUMENTS and see a flag, expect its value next
		if (currentState == State::ARGUMENTS && isArgumentFlag(tok)) {
			currentFlag = tok.value;
			flags[currentFlag] = "";
			currentState = State::EXPECTING_VALUE;
			continue; // Process next token (flag's value)
		}

		currentState = next;

		switch (currentState) {
		case State::ACTION:
			if (tok.name == TokenType::KEYWORD) {
				cmd = toLower(tok.value);
			}
			break;

		case State::TARGET:
			if (tok.name == TokenType::KEYWORD) {
				object = toLower(tok.value);
			}
			break;

		case State::ARGUMENTS:
			args.push_back(tok.value);
			break;

		case State::EXPECTING_VALUE:
			if (!currentFlag.empty()) {
				flags[currentFlag] = tok.value;
				currentFlag.clear();
				currentState = State::ARGUMENTS; // Return to arguments state
			}
			break;

		case State::DONE:
			break;
		default:
			break;
		}

		if (tok.name == TokenType::END_OF_LINE) {
			break;
		}
	}

	if (currentState == State::ERROR) {
		if (errorMsg.empty()) {
			errorMsg = "Parsing failed - invalid command sequence";
		}
		throw std::invalid_argument(errorMsg);
	}

	if (cmd.empty()) {
		errorMsg = "No command found";
		throw std::invalid_argument(errorMsg);
	}

	// Construct command name (add shape)
	std::string cmdName = cmd;
	if (!object.empty()) {
		cmdName = cmd + " " + object;
	}

	// Get CommandCreator from register
	CommandCreatorPtr cmdCreator = cmdRegister.find(cmdName);
	if (!cmdCreator) {
		errorMsg = "Unknown command: " + cmdName;
		throw std::invalid_argument(errorMsg);
	}

	ArgMap parsedArgs;

	// Add all flags to CommandCreator
	for (const auto& flagPair : flags) {
		TokenType flagType = TokenType::STRING; 
		if (!flagPair.second.empty()) {
			if (std::all_of(flagPair.second.begin(), flagPair.second.end(), ::isdigit)) {  // if all are digits
				flagType = TokenType::NUMBER;
			}
		}
		parsedArgs[flagPair.first] = convertTokenToArgValue(flagPair.second, flagType);

		//ArgValue argValue = convertTokenToArgValue(flagPair.second, flagType);
		//cmdCreator->addArgument(flagPair.first, argValue);
	}

	// Add all arguments to CommandCreator
	for (size_t i = 0; i < args.size(); ++i) {
		TokenType argType = TokenType::STRING; 
		if (!args[i].empty()) {
			if (std::all_of(args[i].begin(), args[i].end(), ::isdigit)) {
				argType = TokenType::NUMBER;
			}
		}
		parsedArgs["$" + std::to_string(i)] = convertTokenToArgValue(args[i], argType);

		//ArgValue argValue = convertTokenToArgValue(args[i], argType);
		//// Use index as key for positional arguments  ??????????????????????????????????????
		//cmdCreator->addArgument("arg" + std::to_string(i), argValue);
	}

	return cmdCreator->createCommand(parsedArgs);
}

