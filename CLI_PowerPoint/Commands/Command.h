#pragma once

//#include "Argument.h"
#include <memory>
#include "../Models/Editor.h"

namespace cli::cmd
{
	// Interface 
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void execute(editing::Editor& editor) = 0;		
	};

	using CommandPtr = std::unique_ptr<ICommand>;


	//class BaseCommand : public ICommand
	//{
	//public:
	//	BaseCommand() = default;
	//	virtual ~BaseCommand() = default;

	//	void addArgument(const std::string& key, ArgumentPtr arg) override
	//	{
	//		arguments[key] = std::move(arg);
	//	}

	//	const ArgumentMap& getArguments() const
	//	{
	//		return arguments;
	//	}

	//	bool hasArgument(const std::string& key) const
	//	{
	//		return arguments.find(key) != arguments.end();
	//	}

	//	const Argument* getArgument(const std::string& key) const
	//	{
	//		auto it = arguments.find(key);
	//		if (it != arguments.end())
	//			return it->second.get();
	//		return nullptr;
	//	}

	//protected:
	//	ArgumentMap arguments;
	//};
}

