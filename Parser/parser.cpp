#include "parser.h"

Parser::Parser(std::istream& stream) :tokenizer(stream), currentToken({ TokenType::UNKNOWN,"", Keyword::UNKNOWN })
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

Command Parser::parse()
{
	Command cmd;
	while (currentState != State::DONE && currentState != State::ERROR) {
		currentToken = nextToken();
		processToken(currentToken, cmd);
	}

	return cmd;
}

void Parser::processToken(Token token, Command& cmd) {
	State next = transitionTable[(int)currentState][(int)token.name];

	if (next == State::ERROR) {
		currentState = State::ERROR;
		return;
	}

	switch (currentState)
	{
	case State::START:
		if (token.name == TokenType::KEYWORD) {
			switch (token.keyword)
			{
			case Keyword::ADD: cmd.action = CommandType::Add; break;
			case Keyword::REMOVE: cmd.action = CommandType::Remove; break;
			case Keyword::EDIT:   cmd.action = CommandType::Edit; break;
			case Keyword::SET:    cmd.action = CommandType::Set; break;
			default: cmd.action = CommandType::Unknown; break;
			}
		}
		break;
	case State::ACTION:
		if (token.name == TokenType::KEYWORD) {
			switch (token.keyword)
			{
			case Keyword::SLIDE: cmd.target = ObjectType::Slide; break;
			case Keyword::TITLE: cmd.target = ObjectType::Title; break;
			case Keyword::SHAPE: cmd.target = ObjectType::Shape; break;
			case Keyword::BULLET: cmd.target = ObjectType::Bullet; break;
			default: cmd.target = ObjectType::Unknown; break;
			}
		}
		break;
	case State::TARGET:
	case State::ARGUMENTS:
		if (token.name == TokenType::NUMBER || token.name == TokenType::STRING || token.name == TokenType::KEYWORD || token.name == TokenType::SYMBOL) {
			cmd.args.push_back(token.value);
		}
		break;
	default:
		break;
	}

	currentState = next; // move DFA forward
}
