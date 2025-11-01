#pragma once
#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

namespace cli
{
	enum ArgType
	{
		STRING,		// string value
		NUMBER,		// float value
		INT,		// int value
		COORDINATE	// pair value
	};

	class Argument
	{
	public:
		using ArgValue = std::variant<
			std::monostate,	 // no value
			std::string,
			int,
			float,
			std::pair<double, double>  // for coordinates
		>;

		// Constructors
		Argument(ArgType type)
			: type(type), value(std::monostate{}), flag(false) {}

		Argument(ArgType type, const ArgValue& value)
			: type(type), value(value), flag(true) {}

		// Copy and move constructors 
		Argument(const Argument& other) = default;
		Argument(Argument&& other) noexcept = default;
		Argument& operator=(const Argument& other) = default;
		Argument& operator=(Argument&& other) noexcept = default;
		~Argument() = default;

		// Getters
		ArgType getType() const;
		bool hasValue() const;

		template <typename T>
		const T& getValue() const
		{
			if (!hasValue()) {
				throw std::runtime_error("Argument has no value");
			}
			if (!std::holds_alternative<T>(value)) {
				throw std::runtime_error("Argument type mismatch");
			}
			return std::get<T>(value);
		}

		// Default values (font size, line thickness)
		/*template <typename T>
		T getValueOrDefault(const T& defaultValue) const
		{
			if (!hasValue() || !std::holds_alternative<T>(value)) {
				return defaultValue;
			}
			return std::get<T>(value);
		}*/

		std::string getStrValue() const;
		int getIntValue() const;
		float getFloatValue() const;
		std::pair<double, double> getPairValue() const;

		/*std::string getStrValueOrDefault(const std::string& defaultValue = "") const;
		int getIntValueOrDefault(int defaultValue = 0) const;
		float getFloatValueOrDefault(float defaultValue = 0.0f) const;
		std::pair<double, double> getPairValueOrDefault(const std::pair<double, double>& defaultValue = { 0.0, 0.0 }) const;*/

		// Setter
		void setValue(const ArgValue& val);

		// Type checking
		bool isString() const;
		bool isInt() const;
		bool isFloat() const;
		bool isCoordinate() const;

	private:
		ArgType type;
		ArgValue value;
		bool flag;
	};

	using ArgumentPtr = std::unique_ptr<Argument>;
	using ArgumentMap = std::unordered_map<std::string, ArgumentPtr>;
}
