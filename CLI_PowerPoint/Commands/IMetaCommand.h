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
		virtual ~IMetaCommand() = default;
		virtual std::string getName() const = 0;
		virtual std::string getDescription() const = 0;
		virtual const std::vector<ArgumentInfo>& getArguments() const = 0;
		virtual factory::CommandCreatorPtr getCreator() = 0;
	};

	class MetaCommand : public IMetaCommand
	{
	public:
		MetaCommand(const std::string& name,const std::string& description, factory::CommandCreatorPtr creator)
			: m_name(name), m_description(description), m_creator(creator) {}

		// Getters
		std::string getName() const { return m_name; }
		std::string getDescription() const { return m_description; }
		const std::vector<ArgumentInfo>& getArguments() const { return m_arguments; }
		factory::CommandCreatorPtr getCreator() { return m_creator; }

		void addArgument(const ArgumentInfo& arg)
		{
			m_arguments.push_back(arg);
		}

	private:
		std::string m_name;
		std::string m_description;
		factory::CommandCreatorPtr m_creator;
		std::vector<ArgumentInfo> m_arguments;
	};
}