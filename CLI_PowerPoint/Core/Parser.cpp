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
	transitionTable[(int)State::START][(int)TokenType::IDENTIFIER] = State::COMMAND;

	transitionTable[(int)State::COMMAND][(int)TokenType::IDENTIFIER] = State::COMMAND;
	transitionTable[(int)State::COMMAND][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::COMMAND][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::COMMAND][(int)TokenType::END_OF_LINE] = State::DONE;

	// Collecting arguments
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::IDENTIFIER] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::SYMBOL] = State::ARGUMENTS;
	transitionTable[(int)State::ARGUMENTS][(int)TokenType::END_OF_LINE] = State::DONE;

	// Expecting value after a flag
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::NUMBER] = State::ARGUMENTS;
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::STRING] = State::ARGUMENTS;
	transitionTable[(int)State::EXPECTING_VALUE][(int)TokenType::IDENTIFIER] = State::ARGUMENTS;
}

bool Parser::isArgumentFlag(const Token& token) const
{
	return token.type == TokenType::IDENTIFIER &&
		!token.value.empty() &&
		token.value[0] == '-';
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
			// Check if it's a float
			if (value.find('.') != std::string::npos) {
				return static_cast<float>(std::stof(value));
			}
			// Otherwise treat as int
			return std::stoi(value);
		}
		catch (...) {
			return std::string(value);
		}
	case TokenType::STRING:
	case TokenType::IDENTIFIER:
	default:
		return std::string(value);
	}
}

std::string Parser::buildCommandName() const
{
	if (commandTokens.empty()) {
		return "";
	}

	std::string cmdName = toLower(commandTokens[0]);
	for (size_t i = 1; i < commandTokens.size(); ++i) {
		cmdName += " " + toLower(commandTokens[i]);
	}
	return cmdName;
}

cmd::factory::CommandCreatorPtr Parser::findCommandCreator()
{
	// Try to find command with all tokens first
	for (size_t numTokens = commandTokens.size(); numTokens > 0; --numTokens) {
		std::string cmdName = toLower(commandTokens[0]);
		for (size_t i = 1; i < numTokens; ++i) {
			cmdName += " " + toLower(commandTokens[i]);
		}

		CommandCreatorPtr creator = cmdRegister.find(cmdName);
		if (creator) {
			// Move remaining tokens to arguments
			for (size_t i = numTokens; i < commandTokens.size(); ++i) {
				args.insert(args.begin(), commandTokens[i]);
			}
			commandTokens.resize(numTokens);
			return creator;
		}
	}

	return nullptr;
}

cmd::CommandPtr Parser::parse()
{
	// Reset state for new parsing
	currentState = State::START;
	commandTokens.clear();
	flags.clear();
	args.clear();
	currentFlag.clear();
	errorMsg.clear();

	while (currentState != State::DONE && currentState != State::ERROR) {
		Token tok = tokenizer.tokenize();

		// Check for end of line
		if (tok.type == TokenType::END_OF_LINE) {
			currentState = State::DONE;
			break;
		}

		// Handle flag detection
		if ((currentState == State::COMMAND || currentState == State::ARGUMENTS) && isArgumentFlag(tok)) {
			currentFlag = tok.value;
			flags[currentFlag] = "";
			currentState = State::EXPECTING_VALUE;
			continue;
		}

		// If we're expecting a value, handle it directly
		if (currentState == State::EXPECTING_VALUE) {
			if (!currentFlag.empty()) {
				flags[currentFlag] = tok.value;
				currentFlag.clear();
				currentState = State::ARGUMENTS;
			}
			continue;
		}

		// Check transition table for other states
		int stateIdx = static_cast<int>(currentState);
		int tokenIdx = static_cast<int>(tok.type);
		State next = transitionTable[stateIdx][tokenIdx];

		if (next == State::ERROR) {
			errorMsg = "Unexpected token '" + tok.value + "' in state " + std::to_string(stateIdx);
			throw std::invalid_argument(errorMsg);
		}		

		currentState = next;

		switch (currentState) {
		case State::COMMAND:
			if (tok.type == TokenType::IDENTIFIER) {
				commandTokens.push_back(tok.value);
			}
			break;
		case State::ARGUMENTS:
			if (tok.type != TokenType::END_OF_LINE) {
				args.push_back(tok.value);
			}
			break;
		case State::DONE:
			break;
		default:
			break;
		}

		if (tok.type == TokenType::END_OF_LINE) {
			break;
		}
	}

	if (currentState == State::ERROR) {
		if (errorMsg.empty()) {
			errorMsg = "Parsing failed - invalid command sequence";
		}
		throw std::invalid_argument(errorMsg);
	}

	if (commandTokens.empty()) {
		errorMsg = "No command found";
		throw std::invalid_argument(errorMsg);
	}

	// Get CommandCreator from register
	CommandCreatorPtr cmdCreator = findCommandCreator();
	if (!cmdCreator) {
		errorMsg = "Unknown command: " + buildCommandName();
		throw std::invalid_argument(errorMsg);
	}

	ArgMap parsedArgs;

	// Add all flags
	for (const auto& flagPair : flags) {
		TokenType flagType = TokenType::STRING;
		if (!flagPair.second.empty()) {
			if (std::all_of(flagPair.second.begin(), flagPair.second.end(),
				[](unsigned char c) { return std::isdigit(c) || c == '.' || c == '-'; })) {
				flagType = TokenType::NUMBER;
			}
		}
		parsedArgs[flagPair.first] = convertTokenToArgValue(flagPair.second, flagType);
	}

	// Add positional arguments
	for (size_t i = 0; i < args.size(); ++i) {
		TokenType argType = TokenType::STRING;
		if (!args[i].empty()) {
			if (std::all_of(args[i].begin(), args[i].end(),
				[](unsigned char c) { return std::isdigit(c) || c == '.' || c == '-'; })) {
				argType = TokenType::NUMBER;
			}
		}
		parsedArgs["$" + std::to_string(i)] = convertTokenToArgValue(args[i], argType);
	}


	return cmdCreator->createCommand(parsedArgs);
}

