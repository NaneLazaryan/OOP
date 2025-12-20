#pragma once

#include <string>
#include "factory/ICmdCreator.h"

namespace cli::cmd
{
	struct ArgumentInfo
	{
		std::string name;            
		bool required;              
		std::string defaultValue;   

		ArgumentInfo(const std::string& name, bool req = false, const std::string& defVal = "")
			: name(name), required(req), defaultValue(defVal) {}
	};

	class IMetaCommand
	{
	public:
		//virtual std::string getDescription() = 0;
		virtual factory::CommandCreatorPtr getCreator() = 0;
	};

	class MetaCommand : public IMetaCommand
	{
	public:
		MetaCommand(const std::string& name, factory::CommandCreatorPtr creator)
			: cmdName(name), creator(creator) {}

		factory::CommandCreatorPtr getCreator()
		{
			return creator;
		}

		const std::vector<ArgumentInfo>& getArguments() const
		{
			return arguments;
		}

	private:
		std::string cmdName;
		factory::CommandCreatorPtr creator;
		std::vector<ArgumentInfo> arguments;
	};
}