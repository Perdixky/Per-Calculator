#include <iostream>
#include <Windows.h>

#include "Variable.h"
#include "Calculate.h"

std::stringstream ss;//建立字符串流，必须是全局变量，否则无法在其它文件访问

std::stringstream& operator>>(std::stringstream& s, std::string& string)
//运算符重载，针对string类型，拒绝输入数字，字母，下划线以外的内容，方便在运算符处停止输入
{
	char temp;
	ss >> temp;         //去掉前面的空白符
	if (temp == '^')
	{
		string += temp;
		return s;
	}
	for (; isalnum(temp) || temp == '_' || isdigit(temp); s.get(temp))       //用get函数获取空白符来达到空白符终止的效果
		string += temp;
	s.unget();         //退回最后一个字符，否则会丢失第一个不符合条件的字符
	return s;
}

void extern_version()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	std::cout << R"(
    /**
    * @file 计算器.exe/.cpp
    * @brief 一个可以实现科学计算的计算器
    * @author Perdixky
    * @date 2023/11/28
    * @version 5.3.0
    *
    * @{
	* v6.0.0  2023/11/28  改进了负数检测法，使用了Git和Github，项目地址https://github.com/Perdixky/Calculator-v5
	* v5.2.2  2023/11/12  修复了不能使用负数的bug，提高了程序鲁棒性
    * v5.2.0  2023/11/6   修复了许多bug，支持了基本初等函数运算
    * v5.1.0  2023/11/6   使用了字符串流代替标准输入流，使用流状态来判断缓冲区是否为空，使用了输入运算符重载
    * v5.0.0  2023/11/3   算法完全重写，支持了小括号，中括号，大括号，计算器迎来全新时代
    * v4.0.0  2023/10/30  使用多文件链接，使用了运算符重载，进行大量重写，修复了对";"进行检测的漏洞
    * v3.2.0  2023/10/20  修复了许多bug，此版本较为稳定
    * v3.1.1  2023/10/18  修复了一些bug，添加了赋值变量的功能
    * v3.1.0  2023/10/18  使用ReSharper进行了近乎极致的优化，替换掉了旧式语法，使用了c++最新语言特性
    * v3.0.0  2023/10/18  增加了关键词及控制指令
    * v2.0.2  2023/10/18  优化了宏定义方式，使用constexpr代替宏定义
    * v2.0.1  2023/10/17  添加了使用示例
    * v2.0.0  2023/10/17  添加了自定义变量的功能
    * v1.2.0  2023/10/11  修复了不能使用小数的bug
    * v1.1.3  2023/10/11  修复了不能正确检测符号的bug，修复了未清除缓存区的bug
    * v1.1.2  2023/10/10  添加了颜色显示效果
    * v1.1.1  2023/10/10  添加了版本声明，将提示词改为中文
    * v1.1.0  2023/10/10  修复了不能使用负数的bug，优化了代码可读性
    * }
    */
    )" << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("pause");
	system("cls");
}

void show_keyword()   //显示关键词
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	std::cout << R"(
let        声明变量
show       显示已经定义的变量
delete     删除已经定义的变量
keyword    显示关键字
clear      清屏
quit       退出程序
function   显示支持的函数及用法
        )";
	std::cout << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void show_function()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	std::cout << R"(
 函数名                   语法                示例
正弦函数               sin(表达式)          sin(pi/2)
余弦函数               cos(表达式)            cos(0)
正切函数               tan(表达式)          tan(pi/4)
对数函数               log(底数)(真数)      log(2)(4)
自然对数函数           ln(真数)               ln(e)
幂函数（指数函数）     底数^指数               e^2
        )";
	std::cout << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void start_up()   //起始信息输出
{
	std::cout << "请输入计算式、变量定义或指令，用\";\"结尾 \n";
	std::cout << "提示：\n" << "加 +\t减 -\t乘 *\t除 /\t括号 () [] {}\n";
	std::cout << "使用let来声明变量，\"=\"赋值变量，变量名必须仅含有字母、数字和下划线，并以字母开头\n" << "圆周率π输入pi   自然对数的底数e输入e\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	std::cout << "请使用英文输入法\n" << "输入\"keyword\"查看关键字\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void creat_stringstream()
{
	std::string expression;
	std::getline(std::cin, expression, '\n');   //如果用普通的 >> 输入运算符会在空格处停止获取
	//先用字符串承载流，方便检查最后是否以";"结尾
	if (expression.at(expression.size() - 1) != ';') throw std::runtime_error("表达式没有以\";\"结尾！");
	ss.clear();   //重点！！在输出完成后流状态变成std::ios_base::iostate::eofbit，必须归位流状态后才可继续使用
	ss.str(expression);
}

void keywords_preprocess()
{
	while (true)
	{
		creat_stringstream();
		if (isalpha(ss.peek()))
		{
			std::string temp;
			ss >> temp;
			if (temp == "let") Variable::define();
			else if (temp == "show") Variable::show_var();
			else if (temp == "clear") {
				system("cls"); start_up();
			}
			else if (temp == "delete") Variable::delete_var();
			else if (temp == "keyword") show_keyword();
			else if (temp == "quit") exit(0);
			else if (temp == "function") show_function();
			else
			{
				if(Variable::is_defined(temp))
				{
					char sign;
					ss >> sign;
					if (sign != '=')
					{
						ss.unget();
						return;
					}
					Variable::assign(temp);
				}
				else
				{
					ss.str(ss.str());   //这TM是怎么返回输入在temp中的流的？？？？？
					return;
				}
			}
		}
		else return;
	}
}

int main()
{
	extern_version();
	start_up();
	beginning:
	try {
		while (true)
		{
			keywords_preprocess();
			const double result = calculate();
			std::cout << '=' << result << "\n\n";
		}
	}
	catch (std::runtime_error& e)   //对所有可能的错误统一处理
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		std::cerr << "\n警告：\n";
		std::cerr << "严重错误：" << e.what() << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		std::cin.clear();   //恢复输入流状态标识符
		std::cout << "重新输入？[y/n]\n\n";
		while (true)
		{
			std::string x;
			std::cin >> x;
			if (x == "y") {
				std::cout << "\n重启成功！请重新输入：\n\n";
				std::cin.ignore(1);
				// >> 标准输入会将\n留在缓冲区中，进而在creat_stringstream()中的getline()函数会直接结束读取，故需忽略一个字符
				goto beginning;
			}
			if (x == "n")
				return 0;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cerr << "\n警告：\n";
			std::cerr << "这不是正确的指令，请重新输入\n\n";
			std::cin.clear();
			std::cin.ignore(50, '\n');
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}
