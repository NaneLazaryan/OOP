#pragma once
#include "Argument.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void execute() = 0;
	
	virtual void addArgument(const std::string& key, ArgumentPtr arg) 
	{ 
		arguments[key] = std::move(arg);
	}
	
	const ArgumentMap& getArguments() const 
	{ 
		return arguments; 
	}	

	bool hasArgument(const std::string& key) const {
		return arguments.find(key) != arguments.end();
	}

	const Argument* getArgument(const std::string& key) const
	{
		auto it = arguments.find(key);
		if (it != arguments.end())
			return it->second.get();
		return nullptr;
	}

protected:
	ArgumentMap arguments;
};
