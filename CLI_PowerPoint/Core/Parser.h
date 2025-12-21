#pragma once
#include <string>
#include "Tokenizer.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include "../Commands/Command.h"
#include "CommandRegister.h"
#include "../Commands/factory/ICmdCreator.h"

static constexpr size_t MAX_STATES = 20;
static constexpr size_t MAX_TOKEN_TYPES = 20;

namespace cli
{
	enum class State {
		START,           // beginning
		COMMAND,         // reading command name (may be multi-word)
		ARGUMENTS,       // collecting arguments
		EXPECTING_VALUE, // saw a flag, expecting its value next
		DONE,            // command successfully parsed
		ERROR            // invalid sequence
	};

	using ArgMap = std::unordered_map<std::string, cmd::factory::ArgValue>;
	using ArgValue = cmd::factory::ArgValue;

	class Parser
	{
	public:
		Parser(std::istream& stream, CommandRegister& cmdRegister);

		cmd::CommandPtr parse();
	private:
		Tokenizer tokenizer;
		State currentState = State::START;
		State transitionTable[MAX_STATES][MAX_TOKEN_TYPES];

		std::vector<std::string> commandTokens; // Tokens that form the command name
		std::unordered_map<std::string, std::string> flags;
		std::vector<std::string> args;
		std::string currentFlag;
		std::string errorMsg;

		CommandRegister& cmdRegister;

	private:
		void initializeTransitionTable();
		bool isArgumentFlag(const Token& token) const;
		cmd::factory::ArgValue convertTokenToArgValue(const std::string& value, TokenType type) const;
		std::string toLower(const std::string& str) const;
		std::string buildCommandName() const;
		cmd::factory::CommandCreatorPtr findCommandCreator();
	};
}
