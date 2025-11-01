#include "Argument.h"

using namespace cli;

ArgType Argument::getType() const 
{
    return type;
}

bool Argument::hasValue() const 
{
    return flag;
}

std::string Argument::getStrValue() const
{
    return getValue<std::string>();
}

int Argument::getIntValue() const 
{
    return getValue<int>();
}

float Argument::getFloatValue() const 
{
    return getValue<float>();
}

std::pair<double, double> Argument::getPairValue() const 
{
    return getValue<std::pair<double, double>>();
}

//std::string Argument::getStrValueOrDefault(const std::string& defaultValue) const 
//{
//    return getValueOrDefault<std::string>(defaultValue);
//}
//
//int Argument::getIntValueOrDefault(int defaultValue) const 
//{
//    return getValueOrDefault<int>(defaultValue);
//}
//
//float Argument::getFloatValueOrDefault(float defaultValue) const 
//{
//    return getValueOrDefault<float>(defaultValue);
//}
//
//std::pair<double, double> Argument::getPairValueOrDefault(const std::pair<double, double>& defaultValue) const 
//{
//    return getValueOrDefault<std::pair<double, double>>(defaultValue);
//}

// Setter
void Argument::setValue(const ArgValue& val) 
{
    value = val;
    flag = !std::holds_alternative<std::monostate>(val);
}

// Type checking
bool Argument::isString() const 
{ 
    return hasValue() && std::holds_alternative<std::string>(value); 
}

bool Argument::isInt() const 
{ 
    return hasValue() && std::holds_alternative<int>(value); 
}

bool Argument::isFloat() const 
{ 
    return hasValue() && std::holds_alternative<float>(value); 
}

bool Argument::isCoordinate() const 
{ 
    return hasValue() && std::holds_alternative<std::pair<double, double>>(value); 
}