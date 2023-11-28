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
	throw std::runtime_error("���ֲ��ܴ�����ַ���" + name);
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
	if (is_defined(name)) throw std::runtime_error("�����ظ����壡");
	char sign;
	ss >> sign;
	if (sign != '=') throw std::runtime_error("�����������");
	variables.push_back(Variable{ name, calculate() });
	std::cout << '\n';
}

void Variable::show_var()//��ʾ�Ѿ�����ı���
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	if (variables.empty())//����Ƿ�Ϊ��
		std::cout << "δ�����κα�����\n";
	for (const Variable& v : variables)
		std::cout << v.name_ << '\t' << v.value_ << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void Variable::delete_var()//�ͷ�vector�ռ�
{
	std::vector <Variable>().swap(variables);
}
