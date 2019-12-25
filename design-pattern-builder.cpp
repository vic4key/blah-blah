#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Person
{
public:
	Person() : m_id(0), m_name(""), m_age(0) {}
	virtual ~Person() {}

	Person& id(int v)
	{
		m_id = v;
		return *this;
	}

	Person& name(const string& v)
	{
		m_name = v;
		return *this;
	}

	Person& age(int v)
	{
		m_age = v;
		return *this;
	}

	string str()
	{
		stringstream ss;
		ss << *this;
		return ss.str();
	}

	friend ostream& operator<<(ostream& os, const Person& person)
	{
		os << person.m_id << "," << person.m_name << "," << person.m_age;
		return os;
	}

private:
	int m_id;
	string m_name;
	int m_age;
};

int main(int argc, char const *argv[])
{
	auto s = Person().id(1).name("vic").age(29).str();

	cout << s << endl;

	return 0;
}