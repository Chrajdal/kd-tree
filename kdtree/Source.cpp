#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <sstream>
#include <ctime>
#include <numeric>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;

struct Tpoint
{
	Tpoint(int _x = 0, int _y = 0)
		: m_x(_x), m_y(_y) {}

	Tpoint(const Tpoint & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}

	Tpoint & operator = (const Tpoint & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}

	friend ostream & operator << (ostream & os, const Tpoint & src)
	{
		return os << src.m_x << ", " << src.m_y;
	}

	int m_x;
	int m_y;
};

struct Trect
{
	Trect(const Tpoint & upleft = {}, const Tpoint & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}

	Trect(const Trect & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}

	Trect & operator = (const Trect & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}

	friend ostream & operator << (ostream & os, const Trect & src)
	{
		return os << "(" << src.m_upleft << ") -> (" << src.m_downright << ")";
	}

	Tpoint m_upleft;
	Tpoint m_downright;
};

double distance(const Tpoint & a, const Tpoint & b)
{
	return sqrt((double)(a.m_x - b.m_x) * (double)(a.m_x - b.m_x)
	          + (double)(a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}

class odd_node;
class even_node
{
public:
	even_node(const Tpoint & p = {})
		: m_p(p), m_top(NULL), m_bot(NULL)
	{}
	~even_node(void);
	void insert(const Tpoint & p);
	Tpoint find_closest_point(const Tpoint & p) const;

	Tpoint m_p;
	odd_node * m_top;
	odd_node * m_bot;
};

ostream & operator << (ostream & os, const even_node & src);

class odd_node
{
public:
	odd_node(const Tpoint & p = {})
		: m_p(p), m_left(NULL), m_right(NULL)
	{}
	~odd_node(void)
	{
		delete m_left; delete m_right;
	}

	void insert(const Tpoint & p)
	{
		if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
			return;
		if (p.m_x < m_p.m_y)
		{
			if (m_left == NULL)
				m_left = new even_node(p);
			else
				m_left->insert(p);
		}
		else
		{
			if (m_right == NULL)
				m_right = new even_node(p);
			else
				m_right->insert(p);
		}
	}

	Tpoint find_closest_point(const Tpoint & p) const
	{
		return Tpoint();
	}

	friend ostream & operator << (ostream & os, const odd_node & src)
	{
		os << src.m_p << endl;
		if (src.m_left != NULL)
			os << *src.m_left;
		if (src.m_right != NULL)
			os << *src.m_right;
		return os;
	}

	Tpoint m_p;
	even_node * m_left;
	even_node * m_right;
};

even_node::~even_node(void)
{
	delete m_top; delete m_bot;
}

void even_node::insert(const Tpoint & p)
{
	if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
		return;
	if (p.m_y < m_p.m_y)
	{
		if (m_bot == NULL)
			m_bot = new odd_node(p);
		else
			m_bot->insert(p);
	}
	else
	{
		if (m_top == NULL)
			m_top = new odd_node(p);
		else
			m_top->insert(p);
	}
}

Tpoint even_node::find_closest_point(const Tpoint & p) const
{
	if (m_bot != NULL)
		m_bot->find_closest_point(p);
	if (m_top != NULL)
		m_top->find_closest_point(p);
	return Tpoint();
}

ostream & operator << (ostream & os, const even_node & src)
{
	os << src.m_p << endl;
	if (src.m_bot != NULL)
		os << *src.m_bot;
	if (src.m_top != NULL)
		os << *src.m_top;
	return os;
}

class kdtree
{
public:
	kdtree(void)
		: m_root(NULL)
	{}
	~kdtree(void)
	{
		delete m_root;
	}

	void insert(const Tpoint & p)
	{
		if (m_root == NULL)
			m_root = new even_node(p);
		else
			m_root->insert(p);
	}

	Tpoint find_closest_point(const Tpoint & p) const
	{
		if (m_root == NULL)
			return Tpoint();
		else
			return m_root->find_closest_point(p);
	}

	friend ostream & operator << (ostream & os, const kdtree & src)
	{
		if (src.m_root != NULL)
			os << *src.m_root;
		return os;
	}
private:
	even_node * m_root;
};

class CTimer
{
public:
	CTimer(void)
	{
		start = chrono::system_clock::now();
	}
	double elapsed(void) const
	{
		return (double)(chrono::duration_cast<chrono::milliseconds>
			(chrono::system_clock::now() - start)).count();
	}
private:
	chrono::time_point<chrono::system_clock> start;
};

int main(void)
{
	srand(unsigned(time(0)));
	kdtree tree;

	CTimer t;
	for (int i = 0; i < 100000; ++i)
		tree.insert(Tpoint(rand() % 1000 - 500, rand() % 1000 - 500));
	//cout << tree << endl;
	cout << "Took me: "<< t.elapsed() << " millisecond." << endl;

	system("PAUSE");
	return 0;
}