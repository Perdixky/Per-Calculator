#include "Variable.h"
#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include "Calculate.h"

double Variable::get_value(const std::string& name)
{
	for (const Variable& v : variables)
	{
		if (v.name_ == name)
			return v.value_;
	}
	throw std::runtime_error("发现不能处理的字符：" + name);
}

bool Variable::is_defined(const std::string& name)
{
	for(const Variable& v : variables)
	{
		if (v.name_ == name) return true;
	}
	return false;
}

void Variable::assign(const std::string& temp)
{
	for (Variable& v : variables)
		if (temp == v.name_)
			v.value_ = calculate();
	std::cout << '\n';
}

void Variable::define()
{
	std::string name;
	ss >> name;
	if (is_defined(name)) throw std::runtime_error("变量重复定义！");
	char sign;
	ss >> sign;
	if (sign != '=') throw std::runtime_error("变量定义错误！");
	variables.push_back(Variable{ name, calculate() });
	std::cout << '\n';
}

void Variable::show_var()//显示已经定义的变量
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	if (variables.empty())//检测是否为空
		std::cout << "未定义任何变量！\n";
	for (const Variable& v : variables)
		std::cout << v.name_ << '\t' << v.value_ << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void Variable::delete_var()//释放vector空间
{
	std::vector <Variable>().swap(variables);
}
