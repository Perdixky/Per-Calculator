#pragma once
#include <sstream>

double calculate();
extern std::stringstream ss;

union Token
{
	//默认public
	explicit Token(const double value) :value(value) { }
	explicit Token(const char brace) :brace(brace) { }
	char brace{ '_' };//始终初始化，但联合类型只能有一个默认初始化
	double value;
};