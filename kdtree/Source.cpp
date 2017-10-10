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

//#define GMIN numeric_limits<int>::min()
//#define GMAX numeric_limits<int>::max()

//#define GMIN -100
//#define GMAX 100

#define GMIN 0
#define GMAX 1000

/*asdklfalskdfj asdlfk jaskldf*/

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

int sq_distance(const Tpoint & a, const Tpoint & b)
{
	return (a.m_x - b.m_x) * (a.m_x - b.m_x)
		+ (a.m_y - b.m_y) * (a.m_y - b.m_y);
}

Tpoint get_closest_p(const Trect & rect, const Tpoint & p)
{
  /* 1 | 2 | 3
	---+---+---
	 4 | 5 | 6
	---+---+---
	 7 | 8 | 9 */

	if (p.m_x > rect.m_upleft.m_x)
	{
		if (p.m_y > rect.m_upleft.m_y)
		{
			// no.: 1
			cout << "no.: 1" << endl;
			return Tpoint(rect.m_upleft);
		}
		else if (p.m_y < rect.m_downright.m_y)
		{
			// no.: 7
			cout << "no.: 7" << endl;
			return Tpoint(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{
			// no.: 4
			cout << "no.: 4" << endl;
			return Tpoint(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x < rect.m_downright.m_x)
	{
		if (p.m_y > rect.m_upleft.m_y)
		{
			// no.: 3
			cout << "no.: 3" << endl;
			return Tpoint(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y < rect.m_downright.m_y)
		{
			// no.: 9
			cout << "no.: 9" << endl;
			return Tpoint(rect.m_downright);
		}
		else
		{
			// no.: 6
			cout << "no.: 6" << endl;
			return Tpoint(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y > rect.m_upleft.m_y)
		{
			// no.: 2
			cout << "no.: 2" << endl;
			return Tpoint(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y < rect.m_downright.m_y)
		{
			// no.: 8
			cout << "no.: 8" << endl;
			return Tpoint(p.m_x, rect.m_downright.m_y);
		}
		else
		{
			// no.: 5
			cout << "no.: 5" << endl;
			return p;
		}
		cout << "ERROR" << endl;
	}
}

class odd_node;
class even_node
{
public:
	even_node(const Tpoint & p = {})
		: m_p(p), m_top(NULL), m_bot(NULL)
	{}
	~even_node(void);
	void insert(const Tpoint & p, const Trect & lrect, const Trect & rrect);
	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const;
	friend ostream & operator << (ostream & os, const even_node & src);

	Tpoint m_p;
	Trect m_trect;
	Trect m_brect;
	odd_node * m_top;
	odd_node * m_bot;
};


class odd_node
{
public:
	odd_node(const Tpoint & p = {})
		: m_p(p), m_left(NULL), m_right(NULL)
	{}
	~odd_node(void)
	{
		delete m_left;
		delete m_right;
	}

	void insert(const Tpoint & p, const Trect & lrect, const Trect & rrect);

	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const
	{
		int dist = sq_distance(p, m_p);
		if (dist < best_dist)
		{
			best_dist = dist;
			closest = m_p;
		}

		if (best_dist > sq_distance(get_closest_p(m_lrect, p), p) && m_left != NULL)
			m_left->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_rrect, p), p) && m_right != NULL)
			m_right->find_closest_point(p, closest, best_dist);
		return closest;
	}

	friend ostream & operator << (ostream & os, const odd_node & src)
	{
		os << src.m_p << " * " << src.m_lrect << " --- " << src.m_rrect << endl;
		if (src.m_left != NULL)
			os << *src.m_left;
		if (src.m_right != NULL)
			os << *src.m_right;
		return os;
	}

	Tpoint m_p;
	Trect m_lrect;
	Trect m_rrect;
	even_node * m_left;
	even_node * m_right;
};

even_node::~even_node(void)
{
	delete m_top;
	delete m_bot;
}

void odd_node::insert(const Tpoint & p, const Trect & lrect, const Trect & rrect)
{
	if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
		return;

	if (p.m_x < m_p.m_x)
	{
		if (m_left == NULL)
		{
			m_left = new even_node(p);

			Tpoint ta(lrect.m_upleft);
			Tpoint tb(lrect.m_downright.m_x, p.m_y);
			Tpoint ba(lrect.m_upleft.m_x, p.m_y);
			Tpoint bb(lrect.m_downright);

			Trect trect(ta, tb);
			Trect brect(ba, bb);
			m_left->m_trect = trect;
			m_left->m_brect = brect;

			cout << "odd left inserting: " << p << ", " << trect << ", " << brect << endl;
		}
		else
		{
			m_left->insert(p, m_left->m_trect, m_left->m_brect);
		}
	}
	else
	{
		if (m_right == NULL)
		{
			m_right = new even_node(p);

			Tpoint ta(rrect.m_upleft);
			Tpoint tb(rrect.m_downright.m_x, p.m_y);
			Tpoint ba(rrect.m_upleft.m_x, p.m_y);
			Tpoint bb(rrect.m_downright);

			Trect trect(ta, tb);
			Trect brect(ba, bb);
			m_right->m_trect = trect;
			m_right->m_brect = brect;

			cout << "odd right inserting: " << p << ", " << trect << ", " << brect << endl;
		}
		else
		{
			m_right->insert(p, m_right->m_trect, m_right->m_brect);
		}
	}
}

void even_node::insert(const Tpoint & p, const Trect & trect, const Trect & brect)
{
	if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
		return;
	if (p.m_y < m_p.m_y)
	{
		if (m_bot == NULL)
		{
			m_bot = new odd_node(p);

			Tpoint la(brect.m_upleft);
			Tpoint lb(p.m_x, brect.m_downright.m_y);
			Tpoint ra(p.m_x, brect.m_upleft.m_y);
			Tpoint rb(brect.m_downright);

			Trect lrect(la, lb);
			Trect rrect(ra, rb);
			m_bot->m_lrect = lrect;
			m_bot->m_rrect = rrect;

			cout << "even bot inserting: " << p << ", " << lrect << ", " << rrect << endl;
		}
		else
		{
			m_bot->insert(p, m_bot->m_lrect, m_bot->m_rrect);
		}
	}
	else
	{
		if (m_top == NULL)
		{
			m_top = new odd_node(p);

			Tpoint la(trect.m_upleft);
			Tpoint lb(p.m_x, trect.m_downright.m_y);
			Tpoint ra(p.m_x, trect.m_upleft.m_y);
			Tpoint rb(trect.m_downright);

			Trect lrect(la, lb);
			Trect rrect(ra, rb);
			m_top->m_lrect = lrect;
			m_top->m_rrect = rrect;

			cout << "even top inserting: " << p << ", " << lrect << ", " << rrect << endl;
		}
		else
		{
			m_top->insert(p, m_top->m_lrect, m_top->m_rrect);
		}
	}
}

Tpoint even_node::find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const
{
	int dist = sq_distance(p, m_p);
	if (dist < best_dist)
	{
		best_dist = dist;
		closest = m_p;
	}

	if (best_dist > sq_distance(get_closest_p(m_brect, p), p) && m_bot != NULL)
		m_bot->find_closest_point(p, closest, best_dist);
	if (best_dist > sq_distance(get_closest_p(m_trect, p), p) && m_top != NULL)
		m_top->find_closest_point(p, closest, best_dist);
	return closest;
}

ostream & operator << (ostream & os, const even_node & src)
{
	os << src.m_p << " * " << src.m_trect << " --- " << src.m_brect << endl;
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

	void insert(int x, int y)
	{
		insert(Tpoint(x, y));
	}

	void insert(const Tpoint & p)
	{

		if (m_root == NULL)
		{
			Tpoint la(GMIN, GMIN);
			Tpoint lb(p.m_x, GMAX);
			Tpoint ra(p.m_x, GMIN);
			Tpoint rb(GMAX, GMAX);
			Trect lrect(la, lb);
			Trect rrect(ra, rb);
			m_root = new odd_node(p);

			m_root->m_lrect = lrect;
			m_root->m_rrect = rrect;
		}
		else
			m_root->insert(p, m_root->m_lrect, m_root->m_rrect);
	}

	void erase(const Tpoint & p)
	{
		if (m_root == NULL)
			return;
		else
		{
			return;
		}
	}

	Tpoint find_closest_point(const Tpoint & p) const
	{
		if (m_root == NULL)
			return Tpoint();
		else
		{
			Tpoint closest = m_root->m_p;
			int best_dist = sq_distance(p, m_root->m_p);
			m_root->find_closest_point(p, closest, best_dist);
			return closest;
		}
	}

	friend ostream & operator << (ostream & os, const kdtree & src)
	{
		if (src.m_root != NULL)
			os << *src.m_root;
		return os;
	}
private:
	odd_node * m_root;
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

bool test_search(void)
{
	kdtree tree;
	vector<Tpoint> points;
	int range = 10;
	for (int i = 0; i < 10; ++i)
	{
		Tpoint x(rand() % (2 * range) - range, rand() % (2 * range) - range);
		points.push_back(x);
		tree.insert(x);
	}
	Tpoint p(Tpoint(rand() % (2 * range) - range, rand() % (2 * range) - range));

	/*
	points.push_back(Tpoint(0,0));
	tree.insert(     Tpoint(0,0));

	points.push_back(Tpoint(100,0));
	tree.insert(     Tpoint(100,0));

	points.push_back(Tpoint(0,100));
	tree.insert(     Tpoint(0,100));

	points.push_back(Tpoint(0,150));
	tree.insert(     Tpoint(0,150));

	points.push_back(Tpoint(110,10));
	tree.insert(     Tpoint(110,10));

	points.push_back(Tpoint(0,-100));
	tree.insert(     Tpoint(0,-100));

	points.push_back(Tpoint(0,50));
	tree.insert(     Tpoint(0,50));

	points.push_back(Tpoint(-50,50));
	tree.insert(     Tpoint(-50,50));

	points.push_back(Tpoint(50,0));
	tree.insert(     Tpoint(50,0));

	points.push_back(Tpoint(40,30));
	tree.insert(     Tpoint(40,30));

	points.push_back(Tpoint(40,29));
	tree.insert(     Tpoint(40,29));

	points.push_back(Tpoint(30,40));
	tree.insert(     Tpoint(30,40));

	points.push_back(Tpoint(29,40));
	tree.insert(     Tpoint(29,40));
	Tpoint p (25,25);
	*/

	CTimer t1;
	Tpoint closest = tree.find_closest_point(p);
	double time1 = t1.elapsed();

	double dist = numeric_limits<double>::max();
	Tpoint closest_correct;

	CTimer t2;
	for (auto & i : points)
	{
		double d = distance(i, p);
		if (d < dist)
		{
			closest_correct = i;
			dist = d;
		}
	}
	double time2 = t2.elapsed();

	cout << tree << endl;
	cerr << endl;
	cerr << p << endl;
	cerr << closest << endl;
	cerr << closest_correct << endl;

	//cout << "Closest to: " << p << " is: " << closest << ", distance = " << sq_distance(p,closest) << endl;
	//cout << "Took me: " << time1 << " milliseconds." << endl;
	//cout << "Closest to: " << p << " is: " << closest_correct << ", distance = " << sq_distance(p,closest_correct) << endl;
	//cout << "Took me: " << time2 << " milliseconds." << endl;

	if (closest.m_x == closest_correct.m_x && closest.m_y == closest_correct.m_y)
		return true;
	else
		return false;
}

int main(void)
{
	srand(unsigned(time(0)));

	test_search();

	system("PAUSE");
	return 0;
}