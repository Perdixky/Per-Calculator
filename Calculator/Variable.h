#pragma once
#include <string>
#include <vector>

class Variable
{
public:
	Variable(const std::string& name, const double value): name_(name), value_(value){ }
	static double get_value(const std::string& name);
	static void define();
	static void assign(const std::string& temp);
	static bool is_defined(const std::string& name);
	static void show_var();
	static void delete_var();
private:
	std::string name_{ "not_a_name" };
	double value_{ 0 };
};inline std::vector<Variable> variables;
