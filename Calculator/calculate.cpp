#include <stack>
#include <cmath>

#include "Calculate.h"
#include "Variable.h"


std::stack<double> numbers; //����ջ���洢����
std::stack<char> operators;  //�����ջ���洢������ͷ��̺�����

std::stringstream& operator>>(std::stringstream& s, std::string& string);//�������������


inline bool is_operator(const char temp)     //ʹ�������������Ч��
{
	return temp == '+' || temp == '-' || temp == '*' || temp == '/';     //��(��Ϊ0�����ȼ���ͣ���֤ѹ��
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
	else numbers.push(Variable::get_value(name));//�������ñ���
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
	default:         //���������ȼ����
		return 0;
	}
}

inline bool if_odd(const int number)
{
	return static_cast<bool>(number & 1);  //�ð�λ�������ж���ż
}

void operate()              //��ͨ���㣬�����������ֺ�һ�������������
{
	if (operators.empty()) throw std::runtime_error("���ʽ�ṹ����");
	switch (operators.top())
	{
	case '+':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
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
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
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
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		double right{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		right = numbers.top() * right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case '/':
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		if (static_cast<int>(numbers.top()) == 0)
			throw std::runtime_error("��������Ϊ0��");
		double right{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		right = numbers.top() / right;
		numbers.pop();
		numbers.push(right);
		break;
	}
	case 's': //sin
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		double right{ numbers.top() };
		numbers.pop();
		right = sin(right);
		numbers.push(right);
		break;
	}
	case 'c': //cos
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		double right{ numbers.top() };
		numbers.pop();
		right = cos(right);
		numbers.push(right);
		break;
	}
	case 'l': //log
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		const double natural{ numbers.top()};  //ȡ����
		if (natural <= 0)
			throw std::runtime_error("��������Ϊ��������");
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		const double base{ numbers.top() };     //ȡ����
		if (static_cast<int>(base) == 1 || base <= 0) throw std::runtime_error("��������Ϊ��������1��");
		numbers.pop();
		numbers.push(log(natural) / log(base));
		break;
	}
	case 't': //tan
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		double right{ numbers.top() };
		numbers.pop();
		right = tan(right);
		numbers.push(right);
		break;
	}
	case '^': //�ݺ���
	{
		operators.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		const double exponential{ numbers.top() };
		numbers.pop();
		if (numbers.empty()) throw std::runtime_error("���ʽ�ṹ����");
		const double base{ numbers.top() };
		numbers.pop();
		if (static_cast<int>(base) == 0 && static_cast<int>(exponential) == 0) throw std::runtime_error("������ָ������ͬʱΪ0��");
		if (base < 0 && !is_integral(exponential)) throw std::runtime_error("����Ϊ����ʱ��ָ������Ϊ������");
		const double power = pow(base, exponential);
		numbers.push(power);
		break;
	}
	default:
		throw std::runtime_error("���Ų�������");          //�����û�������ŵ�����·������ţ�����
	}
}

void operate(char brace)     //�������أ�������ʾ�������
{
	char left;
	switch (brace)           //�ҵ���Ӧ��ֹ����
	{
	case ')': left = '('; break;
	case ']': left = '['; break;
	case '}': left = '{'; break;
	default: left = ';';
	}
	while (!operators.empty() && operators.top() != left) operate();    //���㵽��Ӧ����
	//ע��Ҫ����֤ջ�Ƿ�Ϊ����ȡջ��Ԫ��
	if (operators.empty() && left != ';') throw std::runtime_error("���Ų�������");  //������������ŵ������û�������ţ�����
	if (left != ';') operators.pop(); //����������
}

double calculate()
{
	do
	{
		char temp;
		ss >> temp;
		if (ss.tellg() == 1 && temp == '-')  //���ָ����ĵ�һ����������ʽ��ǰ����ָ���
		{
			ss.unget();
			double num;
			ss >> num;
			numbers.push(num);
		}
		else if (isdigit(temp))    //��������֣�ѹ������ջ
		{
			ss.unget();
			double num;
			ss >> num;
			numbers.push(num);
		}
		else if (temp == '(' || temp == '[' || temp == '{')
		{
			operators.push(temp);     //����������ţ�ѹ�������ջ
			if (ss.peek() == '-')     //���ָ����ĵڶ�����������ź���ָ��ţ�����������������ֱ���ڱ���ѭ������ǰѹ�븺��
			{
				double num;
				ss >> num;
				numbers.push(num);
			}
		}
		else if (is_operator(temp))     //������������ѹ�������ջ
		{
			if (!operators.empty() && get_priority(operators.top()) > get_priority(temp)) operate();
			//�����ߵ���������ȼ�С��ǰ��ģ��ȵ����������ֺ�һ�������Ԥ���ѹ������ջ���ٰѺ��������ѹ�������ջ
			else operators.push(temp);
		}
		else if (temp == ')' || temp == '}' || temp == ']' || temp == ';') operate(temp);   //����������Ż�ֺŵ���ʾ������������㵽��Ӧ����
		else
		{
			std::string name;
			ss.unget();
			ss >> name;
			string_process(name);
		}
	} while (!(operators.empty() && ss.eof()));    //�����״̬��ȷ���������Ƿ�Ϊ�ո�Ϊ��Ч
	return numbers.top();
}
