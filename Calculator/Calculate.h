#pragma once
#include <sstream>

double calculate();
extern std::stringstream ss;

union Token
{
	//Ĭ��public
	explicit Token(const double value) :value(value) { }
	explicit Token(const char brace) :brace(brace) { }
	char brace{ '_' };//ʼ�ճ�ʼ��������������ֻ����һ��Ĭ�ϳ�ʼ��
	double value;
};