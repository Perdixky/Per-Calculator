#include <iostream>
#include <Windows.h>

#include "Variable.h"
#include "Calculate.h"

std::stringstream ss;//�����ַ�������������ȫ�ֱ����������޷��������ļ�����

std::stringstream& operator>>(std::stringstream& s, std::string& string)
//��������أ����string���ͣ��ܾ��������֣���ĸ���»�����������ݣ��������������ֹͣ����
{
	char temp;
	ss >> temp;         //ȥ��ǰ��Ŀհ׷�
	if (temp == '^')
	{
		string += temp;
		return s;
	}
	for (; isalnum(temp) || temp == '_' || isdigit(temp); s.get(temp))       //��get������ȡ�հ׷����ﵽ�հ׷���ֹ��Ч��
		string += temp;
	s.unget();         //�˻����һ���ַ�������ᶪʧ��һ���������������ַ�
	return s;
}

void extern_version()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	std::cout << R"(
    /**
    * @file ������.exe/.cpp
    * @brief һ������ʵ�ֿ�ѧ����ļ�����
    * @author Perdixky
    * @date 2023/11/28
    * @version 5.3.0
    *
    * @{
	* v6.0.0  2023/11/28  �Ľ��˸�����ⷨ��ʹ����Git��Github����Ŀ��ַhttps://github.com/Perdixky/Calculator-v5
	* v5.2.2  2023/11/12  �޸��˲���ʹ�ø�����bug������˳���³����
    * v5.2.0  2023/11/6   �޸������bug��֧���˻������Ⱥ�������
    * v5.1.0  2023/11/6   ʹ�����ַ����������׼��������ʹ����״̬���жϻ������Ƿ�Ϊ�գ�ʹ�����������������
    * v5.0.0  2023/11/3   �㷨��ȫ��д��֧����С���ţ������ţ������ţ�������ӭ��ȫ��ʱ��
    * v4.0.0  2023/10/30  ʹ�ö��ļ����ӣ�ʹ������������أ����д�����д���޸��˶�";"���м���©��
    * v3.2.0  2023/10/20  �޸������bug���˰汾��Ϊ�ȶ�
    * v3.1.1  2023/10/18  �޸���һЩbug������˸�ֵ�����Ĺ���
    * v3.1.0  2023/10/18  ʹ��ReSharper�����˽������µ��Ż����滻���˾�ʽ�﷨��ʹ����c++������������
    * v3.0.0  2023/10/18  �����˹ؼ��ʼ�����ָ��
    * v2.0.2  2023/10/18  �Ż��˺궨�巽ʽ��ʹ��constexpr����궨��
    * v2.0.1  2023/10/17  �����ʹ��ʾ��
    * v2.0.0  2023/10/17  ������Զ�������Ĺ���
    * v1.2.0  2023/10/11  �޸��˲���ʹ��С����bug
    * v1.1.3  2023/10/11  �޸��˲�����ȷ�����ŵ�bug���޸���δ�����������bug
    * v1.1.2  2023/10/10  �������ɫ��ʾЧ��
    * v1.1.1  2023/10/10  ����˰汾����������ʾ�ʸ�Ϊ����
    * v1.1.0  2023/10/10  �޸��˲���ʹ�ø�����bug���Ż��˴���ɶ���
    * }
    */
    )" << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("pause");
	system("cls");
}

void show_keyword()   //��ʾ�ؼ���
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	std::cout << R"(
let        ��������
show       ��ʾ�Ѿ�����ı���
delete     ɾ���Ѿ�����ı���
keyword    ��ʾ�ؼ���
clear      ����
quit       �˳�����
function   ��ʾ֧�ֵĺ������÷�
        )";
	std::cout << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void show_function()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	std::cout << R"(
 ������                   �﷨                ʾ��
���Һ���               sin(���ʽ)          sin(pi/2)
���Һ���               cos(���ʽ)            cos(0)
���к���               tan(���ʽ)          tan(pi/4)
��������               log(����)(����)      log(2)(4)
��Ȼ��������           ln(����)               ln(e)
�ݺ�����ָ��������     ����^ָ��               e^2
        )";
	std::cout << '\n';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void start_up()   //��ʼ��Ϣ���
{
	std::cout << "���������ʽ�����������ָ���\";\"��β \n";
	std::cout << "��ʾ��\n" << "�� +\t�� -\t�� *\t�� /\t���� () [] {}\n";
	std::cout << "ʹ��let������������\"=\"��ֵ�����������������������ĸ�����ֺ��»��ߣ�������ĸ��ͷ\n" << "Բ���ʦ�����pi   ��Ȼ�����ĵ���e����e\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	std::cout << "��ʹ��Ӣ�����뷨\n" << "����\"keyword\"�鿴�ؼ���\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void creat_stringstream()
{
	std::string expression;
	std::getline(std::cin, expression, '\n');   //�������ͨ�� >> ������������ڿո�ֹͣ��ȡ
	//�����ַ���������������������Ƿ���";"��β
	if (expression.at(expression.size() - 1) != ';') throw std::runtime_error("���ʽû����\";\"��β��");
	ss.clear();   //�ص㣡���������ɺ���״̬���std::ios_base::iostate::eofbit�������λ��״̬��ſɼ���ʹ��
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
					ss.str(ss.str());   //��TM����ô����������temp�е����ģ���������
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
	catch (std::runtime_error& e)   //�����п��ܵĴ���ͳһ����
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		std::cerr << "\n���棺\n";
		std::cerr << "���ش���" << e.what() << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		std::cin.clear();   //�ָ�������״̬��ʶ��
		std::cout << "�������룿[y/n]\n\n";
		while (true)
		{
			std::string x;
			std::cin >> x;
			if (x == "y") {
				std::cout << "\n�����ɹ������������룺\n\n";
				std::cin.ignore(1);
				// >> ��׼����Ὣ\n���ڻ������У�������creat_stringstream()�е�getline()������ֱ�ӽ�����ȡ���������һ���ַ�
				goto beginning;
			}
			if (x == "n")
				return 0;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cerr << "\n���棺\n";
			std::cerr << "�ⲻ����ȷ��ָ�����������\n\n";
			std::cin.clear();
			std::cin.ignore(50, '\n');
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}
