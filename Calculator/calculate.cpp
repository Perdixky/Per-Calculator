#include <stack>
#include <cmath>

#include "Calculate.h"
#include "Variable.h"


std::stack<double> numbers; //数字栈，存储数字
std::stack<char> operators;  //运算符栈，存储运算符和方程和括号

std::stringstream& operator>>(std::stringstream& s, std::string& string);//运算符重载声明


inline bool is_operator(const char temp)     //使用内联函数提高效率
{
	return temp == '+' || temp == '-' || temp == '*' || temp == '/';     //将(设为0，优先级最低，保证压入
}

inline bool is_integral(const double x)
{
	return x - static_cast<int>(x) <= 1e-06;
}

void string_process(const std::string& name)
{
	if (name == "sin") operators.push('s');
	else if (name == "cos") operators.push('c');
	else if (name == "log") operators.push('l');
	else if (name == "ln")
	{
		operators.push('l');
		numbers.push( 2.718281828 );
	}
	else if (name == "^") operators.push('^');
	else if (name == "tan") operators.push('t');
	else if (name == "e") numbers.push( 2.718281828 );
	else if (name == "pi") numbers.push(3.1415926535);
	else numbers.push(Variable::get_value(name));//其中内置报错
}

int get_priority(const char temp)
{
	switch (temp)
	{
	case '+':case '-':
		return 1;
	case '*':case '/':
		return 2;
	case 's':case 'c':case 'l':case '^':case 't':
		return 3;
	default:         //左括号优先级最低
		return 0;
	}
}

inline bool if_odd(const int number)
{
	return static_cast<bool>(number & 1);  //用按位与运算判断奇偶
}

void operate()              //普通运算，弹出两个数字和一个运算符并运算
{
	if (operators.empty()) throw std::runtime_error("表达式结构错误！");
	switch (operators.top())
	{
	case '+':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top()};
		numbers.pop();
		right = numbers.top() + right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case '-':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top() };
		numbers.pop();
		right = numbers.top() - right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case '*':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		right = numbers.top() * right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case '/':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		if (static_cast<int>(numbers.top()) == 0)
			throw std::runtime_error("除数不能为0！");
		double right{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		right = numbers.top() / right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case 's': //sin
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top() };
		numbers.pop();
		right = sin(right);
		numbers.push(right);
		break;
	}
	case 'c': //cos
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top() };
		numbers.pop();
		right = cos(right);
		numbers.push(right);
		break;
	}
	case 'l': //log
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		const double natural{ numbers.top()};  //取真数
		if (natural <= 0)
			throw std::runtime_error("真数不能为非整数！");
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		const double base{ numbers.top() };     //取底数
		if (static_cast<int>(base) == 1 || base <= 0) throw std::runtime_error("底数不能为非正数或1！");
		numbers.pop();
		numbers.push(log(natural) / log(base));
		break;
	}
	case 't': //tan
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		double right{ numbers.top() };
		numbers.pop();
		right = tan(right);
		numbers.push(right);
		break;
	}
	case '^': //幂函数
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		const double exponential{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("表达式结构错误！");
		const double base{ numbers.top() };
		numbers.pop();
		if (static_cast<int>(base) == 0 && static_cast<int>(exponential) == 0) throw std::runtime_error("底数和指数不能同时为0！");
		if (base < 0 && !is_integral(exponential)) throw std::runtime_error("底数为负数时，指数必须为整数！");
		const double power = pow(base, exponential);
		numbers.push(power);
		break;
	}
	default:
		throw std::runtime_error("括号不完整！");          //如果在没有右括号的情况下发现括号，报错
	}
}

void operate(char brace)     //函数重载，加入提示运算符号
{
	char left;
	switch (brace)           //找到对应终止符号
	{
	case ')': left = '('; break;
	case ']': left = '['; break;
	case '}': left = '{'; break;
	default: left = ';';
	}
	while (!operators.empty() && operators.top() != left) operate();    //运算到相应符号
	//注意要先验证栈是否为空再取栈顶元素
	if (operators.empty() && left != ';') throw std::runtime_error("括号不完整！");  //如果在有右括号的情况下没有左括号，报错
	if (left != ';') operators.pop(); //弹出左括号
}

double calculate()
{
	do
	{
		char temp;
		ss >> temp;
		if (ss.tellg() == 1 && temp == '-')  //出现负数的第一种情况：表达式最前面出现负号
		{
			ss.unget();
			double num;
			ss >> num;
			numbers.push(num);
		}
		else if (isdigit(temp))    //如果是数字，压入数字栈
		{
			ss.unget();
			double num;
			ss >> num;
			numbers.push(num);
		}
		else if (temp == '(' || temp == '[' || temp == '{')
		{
			operators.push(temp);     //如果是左括号，压入运算符栈
			if (ss.peek() == '-')     //出现负数的第二种情况：括号后出现负号，如果出现这种情况，直接在本次循环中提前压入负数
			{
				double num;
				ss >> num;
				numbers.push(num);
			}
		}
		else if (is_operator(temp))     //如果是运算符，压入运算符栈
		{
			if (!operators.empty() && get_priority(operators.top()) > get_priority(temp)) operate();
			//如果后边的运算符优先级小于前面的，先弹出两个数字和一个运算符预算后压入数字栈，再把后面运算符压入运算符栈
			else operators.push(temp);
		}
		else if (temp == ')' || temp == '}' || temp == ']' || temp == ';') operate(temp);   //如果是右括号或分号等提示运算符，则运算到相应符号
		else
		{
			std::string name;
			ss.unget();
			ss >> name;
			string_process(name);
		}
	} while (!(operators.empty() && ss.eof()));    //检查流状态来确定缓冲区是否为空更为有效
	return numbers.top();
}
