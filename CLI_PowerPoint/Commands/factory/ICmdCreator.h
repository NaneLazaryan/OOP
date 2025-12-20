#pragma once

#include <memory>
#include <variant>
#include <unordered_map>
#include <string>

// Forward declaration
namespace cli::cmd {
	class ICommand;
	using CommandPtr = std::unique_ptr<ICommand>;
}

namespace cli::cmd::factory
{
	using ArgValue = std::variant<
		std::monostate,	 // no value
		std::string,
		int,
		float,
		std::pair<double, double>  // for coordinates
	>;

	using ArgMap = std::unordered_map<std::string, ArgValue>;

	class ICmdCreator
	{
	public:
		virtual ~ICmdCreator() = default;
		virtual CommandPtr createCommand(const ArgMap& args) = 0;
	};

	using CommandCreatorPtr = std::shared_ptr<ICmdCreator>;
}