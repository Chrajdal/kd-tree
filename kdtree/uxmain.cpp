#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <vector>
#include <chrono>
#include <cstdio>
#include <random>
#include <deque>
#include <cmath>
#include <ctime>
#include <list>
using namespace std;
using namespace std::chrono;

constexpr double GMINY = -1000.0;
constexpr double GMAXY = 1000.0;
constexpr double GMINX = -1000.0;
constexpr double GMAXX = 1000.0;

constexpr double alignment_weight = 5.0f;
constexpr double separation_weight = 5.0f;
constexpr double cohesion_weight = 5.0f;

//-----------------------------------------------------------------------------
class Graphics
{
public:
	static constexpr int ScreenWidth  = 1000;
	static constexpr int ScreenHeight = 1000;
};

constexpr double PI = std::acos(-1);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
inline ostream & nl(ostream & os) { return os << "\n"; }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
class _Vec2
{
public:
	_Vec2(const T & x = {}, const T & y = {})
		:
		x(x),
		y(y)
	{}
	_Vec2(const _Vec2& vect)
		:
		_Vec2(vect.x, vect.y)
	{}
	template <typename T2>
	explicit operator _Vec2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T LenSq() const
	{
		return *this * *this;
	}
	T Len() const
	{
		return sqrt(LenSq());
	}
	_Vec2 & Normalize()
	{
		const T length = std::sqrt(x * x + y * y);
		x /= length;
		y /= length;
		return *this;
	}
	_Vec2 GetNormalized() const
	{
		_Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}
	_Vec2 operator-() const
	{
		return _Vec2(-x, -y);
	}
	_Vec2& operator=(const _Vec2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	_Vec2& operator+=(const _Vec2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	_Vec2& operator-=(const _Vec2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T operator*(const _Vec2 &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	_Vec2 operator+(const _Vec2 &rhs) const
	{
		return _Vec2(*this) += rhs;
	}
	_Vec2 operator-(const _Vec2 &rhs) const
	{
		return _Vec2(*this) -= rhs;
	}
	_Vec2 & operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	_Vec2 operator*(const T &rhs) const
	{
		return _Vec2(*this) *= rhs;
	}
	_Vec2&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	_Vec2	operator/(const T &rhs) const
	{
		return _Vec2(*this) /= rhs;
	}
	bool	operator==(const _Vec2 &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=(const _Vec2 &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef _Vec2<double> Vec2;
typedef _Vec2<double> Ved2;
typedef _Vec2<int> Vei2;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
struct Tpoint
{
	Tpoint(const T & _x = 0, const T & _y = 0)
		: m_x(_x), m_y(_y)
	{}
	Tpoint(const Tpoint<T> & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}
	Tpoint(const Tpoint<T> && src)
		: m_x(move(src.m_x)), m_y(move(src.m_y))
	{}
	Tpoint<T> & operator = (const Tpoint<T> & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}
	Tpoint<T> & operator = (const Tpoint<T> && src)
	{
		if (this == &src)
			return *this;
		m_x = move(src.m_x);
		m_y = move(src.m_y);
		return *this;
	}
	inline bool operator == (const Tpoint<T> & rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	inline bool operator != (const Tpoint<T> & rhs) const
	{
		return !(*this == rhs);
	}

	friend ostream & operator << (ostream & os, const Tpoint<T> & src)
	{
		return os << src.m_x << ", " << src.m_y;
	}

	T m_x;
	T m_y;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
struct Trect
{
	Trect(const Tpoint<T> & upleft = {}, const Tpoint<T> & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}
	Trect(const Trect<T> & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}
	Trect(const Trect<T> && src)
		: m_upleft(move(src.m_upleft)), m_downright(move(src.m_downright))
	{}
	Trect<T> & operator = (const Trect<T> & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}
	Trect<T> & operator = (const Trect<T> && src)
	{
		if (this == &src)
			return *this;
		m_upleft = move(src.m_upleft);
		m_downright = move(src.m_downright);
		return *this;
	}

	friend ostream & operator << (ostream & os, const Trect<T> & src)
	{
		return os << src.m_upleft << " / " << src.m_downright;
	}

	Tpoint<T> m_upleft;
	Tpoint<T> m_downright;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int random_int(int min, int max);
double random_between(const double & min, const double & max);

long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b);
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b);
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b);
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b);

template<typename T>
Tpoint<T> get_closest_p(const Trect<T> & rect, const Tpoint<T> & p);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class node_f
{
public:
	node_f(const Tpoint<double> & p = {},
		const Trect<double> upleft_rect = {},
		const Trect<double> m_upright_rect = {},
		const Trect<double> m_downleft_rect = {},
		const Trect<double> m_downright_rect = {});
	node_f(const node_f & src);
	node_f & operator=(const node_f & src);
	~node_f(void);
	void insert(const Tpoint<double> & p, const node_f * parent);
	void insert(const node_f & p, const node_f * parent);
	Tpoint<double> find_closest_point(const Tpoint<double> & p, Tpoint<double> & closest, double & best_dist) const;
	vector<node_f> find_n_closest_points(const Tpoint<double> & p, int n, vector<node_f> & found) const;

	friend ostream & operator << (ostream & os, const node_f & src)
	{
		os << src.m_p << " --- " << src.m_velocity.x << ", " << src.m_velocity.y << nl;
		if(src.m_ul != NULL) os << *src.m_ul;
		if(src.m_ur != NULL) os << *src.m_ur;
		if(src.m_dl != NULL) os << *src.m_dl;
		if(src.m_dr != NULL) os << *src.m_dr;
		return os;
	}
public:
	Tpoint<double> m_p;
	Trect<double> m_ul_r, m_ur_r, m_dl_r, m_dr_r;
	_Vec2<double> m_velocity;
	node_f * m_ul, *m_ur, *m_dl, *m_dr;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class quad_tree_f
{
public:
	quad_tree_f(void);
	quad_tree_f(const quad_tree_f & src);
	quad_tree_f & operator = (const quad_tree_f & src);
	~quad_tree_f(void);

	void insert(double x, double y);
	void insert(const Tpoint<double> & p);
	void insert(const node_f & n);
	void clear(void);
	Tpoint<double> find_closest_point(const Tpoint<double> & p) const;
	vector<node_f> find_n_closest_points(const Tpoint<double> & p, int n);

	friend ostream & operator << (ostream & os, const quad_tree_f & src)
	{
		if(src.m_root != NULL)
			os << *src.m_root;
		return os;
	}
public:
	node_f * m_root;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}
double random_between(const double & min, const double & max)
{
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}

template<typename T>
Tpoint<T> get_closest_p(const Trect<T> & rect, const Tpoint<T> & p)
{
	/* 1 | 2 | 3
	  ---+---+---
	   4 | 5 | 6
	  ---+---+---
	   7 | 8 | 9 */

	if (p.m_x < rect.m_upleft.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 1
			return Tpoint<T>(rect.m_upleft);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 7
			return Tpoint<T>(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 4
			return Tpoint<T>(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x > rect.m_downright.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 3
			return Tpoint<T>(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 9
			return Tpoint<T>(rect.m_downright);
		}
		else
		{	// no.: 6
			return Tpoint<T>(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 2
			return Tpoint<T>(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 8
			return Tpoint<T>(p.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 5
			return p;
		}
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
node_f::node_f(const Tpoint<double> & p,
	const Trect<double> upleft_rect,
	const Trect<double> upright_rect,
	const Trect<double> downleft_rect,
	const Trect<double> downright_rect)
	: m_p(p),
	m_ul_r(upleft_rect),
	m_ur_r(upright_rect),
	m_dl_r(downleft_rect),
	m_dr_r(downright_rect),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{}

node_f::node_f(const node_f & src)
	: m_p(src.m_p),
	m_ul_r(src.m_ul_r),
	m_ur_r(src.m_ur_r),
	m_dl_r(src.m_dl_r),
	m_dr_r(src.m_dr_r),
	m_velocity(src.m_velocity),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{
	if (src.m_ul != NULL) m_ul = new node_f(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node_f(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node_f(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node_f(*src.m_dr);
}

node_f & node_f::operator = (const node_f & src)
{
	if (this == &src)
		return *this;

	m_p = src.m_p;
	m_ul_r = src.m_ul_r;
	m_ur_r = src.m_ur_r;
	m_dl_r = src.m_dl_r;
	m_dr_r = src.m_dr_r;
	m_velocity = src.m_velocity;

	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;

	if (src.m_ul != NULL) m_ul = new node_f(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node_f(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node_f(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node_f(*src.m_dr);

	return *this;
}

node_f::~node_f(void)
{
	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;
}

void node_f::insert(const Tpoint<double> & p, const node_f * parent)
{
	if (m_p.m_x == p.m_x && m_p.m_y == p.m_y)
		return;

	if (p.m_y < m_p.m_y) // UP
	{
		if (p.m_x < m_p.m_x) // UP-LEFT
		{
			if (m_ul == NULL)
			{
				m_ul = new node_f(p);
				double MIN_X = parent->m_ul_r.m_upleft.m_x;
				double MID_X = p.m_x;
				double MAX_X = parent->m_ul_r.m_downright.m_x;
				double MIN_Y = parent->m_ul_r.m_upleft.m_y;
				double MID_Y = p.m_y;
				double MAX_Y = parent->m_ul_r.m_downright.m_y;

				m_ul->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_ul->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_ul->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_ul->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_ul->insert(p, m_ul);
			}
		}
		else // (p.m_x >= m_p.m_x) // UP-RIGHT
		{
			if (m_ur == NULL)
			{
				m_ur = new node_f(p);
				double MIN_X = parent->m_ur_r.m_upleft.m_x;
				double MID_X = p.m_x;
				double MAX_X = parent->m_ur_r.m_downright.m_x;
				double MIN_Y = parent->m_ur_r.m_upleft.m_y;
				double MID_Y = p.m_y;
				double MAX_Y = parent->m_ur_r.m_downright.m_y;

				m_ur->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_ur->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_ur->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_ur->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_ur->insert(p, m_ur);
			}
		}
	}
	else // DOWN
	{
		if (p.m_x < m_p.m_x) // DOWN-LEFT
		{
			if (m_dl == NULL)
			{
				m_dl = new node_f(p);
				double MIN_X = parent->m_dl_r.m_upleft.m_x;
				double MID_X = p.m_x;
				double MAX_X = parent->m_dl_r.m_downright.m_x;
				double MIN_Y = parent->m_dl_r.m_upleft.m_y;
				double MID_Y = p.m_y;
				double MAX_Y = parent->m_dl_r.m_downright.m_y;

				m_dl->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_dl->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_dl->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_dl->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_dl->insert(p, m_dl);
			}
		}
		else // DOWN-RIGHT
		{
			if (m_dr == NULL)
			{
				m_dr = new node_f(p);
				double MIN_X = parent->m_dr_r.m_upleft.m_x;
				double MID_X = p.m_x;
				double MAX_X = parent->m_dr_r.m_downright.m_x;
				double MIN_Y = parent->m_dr_r.m_upleft.m_y;
				double MID_Y = p.m_y;
				double MAX_Y = parent->m_dr_r.m_downright.m_y;

				m_dr->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_dr->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_dr->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_dr->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_dr->insert(p, m_dr);
			}
		}
	}
}

void node_f::insert(const node_f & n, const node_f * parent)
{
	if (m_p.m_x == n.m_p.m_x && m_p.m_y == n.m_p.m_y)
		return;

	if (n.m_p.m_y < m_p.m_y) // UP
	{
		if (n.m_p.m_x < m_p.m_x) // UP-LEFT
		{
			if (m_ul == NULL)
			{
				m_ul = new node_f(n.m_p);
				m_ul->m_velocity = n.m_velocity;

				double MIN_X = parent->m_ul_r.m_upleft.m_x;
				double MID_X = n.m_p.m_x;
				double MAX_X = parent->m_ul_r.m_downright.m_x;
				double MIN_Y = parent->m_ul_r.m_upleft.m_y;
				double MID_Y = n.m_p.m_y;
				double MAX_Y = parent->m_ul_r.m_downright.m_y;

				m_ul->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_ul->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_ul->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_ul->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_ul->insert(n, m_ul);
			}
		}
		else // (p.m_x >= m_p.m_x) // UP-RIGHT
		{
			if (m_ur == NULL)
			{
				m_ur = new node_f(n.m_p);
				m_ur->m_velocity = n.m_velocity;

				double MIN_X = parent->m_ur_r.m_upleft.m_x;
				double MID_X = n.m_p.m_x;
				double MAX_X = parent->m_ur_r.m_downright.m_x;
				double MIN_Y = parent->m_ur_r.m_upleft.m_y;
				double MID_Y = n.m_p.m_y;
				double MAX_Y = parent->m_ur_r.m_downright.m_y;

				m_ur->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_ur->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_ur->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_ur->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_ur->insert(n, m_ur);
			}
		}
	}
	else // DOWN
	{
		if (n.m_p.m_x < m_p.m_x) // DOWN-LEFT
		{
			if (m_dl == NULL)
			{
				m_dl = new node_f(n.m_p);
				m_dl->m_velocity = n.m_velocity;

				double MIN_X = parent->m_dl_r.m_upleft.m_x;
				double MID_X = n.m_p.m_x;
				double MAX_X = parent->m_dl_r.m_downright.m_x;
				double MIN_Y = parent->m_dl_r.m_upleft.m_y;
				double MID_Y = n.m_p.m_y;
				double MAX_Y = parent->m_dl_r.m_downright.m_y;

				m_dl->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_dl->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_dl->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_dl->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_dl->insert(n, m_dl);
			}
		}
		else // DOWN-RIGHT
		{
			if (m_dr == NULL)
			{
				m_dr = new node_f(n.m_p);
				m_dr->m_velocity = n.m_velocity;

				double MIN_X = parent->m_dr_r.m_upleft.m_x;
				double MID_X = n.m_p.m_x;
				double MAX_X = parent->m_dr_r.m_downright.m_x;
				double MIN_Y = parent->m_dr_r.m_upleft.m_y;
				double MID_Y = n.m_p.m_y;
				double MAX_Y = parent->m_dr_r.m_downright.m_y;

				m_dr->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
				m_dr->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
				m_dr->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
				m_dr->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
			}
			else
			{
				m_dr->insert(n, m_dr);
			}
		}
	}
}

Tpoint<double> node_f::find_closest_point(const Tpoint<double> & p, Tpoint<double> & closest, double & best_dist) const
{
	double dist = sq_distance(p, m_p);
	if (dist < best_dist)
	{
		best_dist = dist;
		closest = m_p;
	}

	if (best_dist > sq_distance(get_closest_p(m_ul_r, p), p) && m_ul != NULL)
		m_ul->find_closest_point(p, closest, best_dist);
	if (best_dist > sq_distance(get_closest_p(m_ur_r, p), p) && m_ur != NULL)
		m_ur->find_closest_point(p, closest, best_dist);
	if (best_dist > sq_distance(get_closest_p(m_dl_r, p), p) && m_dl != NULL)
		m_dl->find_closest_point(p, closest, best_dist);
	if (best_dist > sq_distance(get_closest_p(m_dr_r, p), p) && m_dr != NULL)
		m_dr->find_closest_point(p, closest, best_dist);

	return closest;
}

vector<node_f> node_f::find_n_closest_points(const Tpoint<double> & p, int n, vector<node_f> & found) const
{
	if (found.empty())
		found.push_back(m_p);
	else
	{
		auto it = lower_bound(found.begin(), found.end(), m_p,
				[p](const node_f & a, const node_f & b)
			{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
		
		found.insert(it, m_p);

		if ((int)found.size() > n)
			found.pop_back();
	}
	
	/*
	//
	// if number of points hasn't reached n-points yet, we can push to the right position
	//
	if ((int)found.size() < n)
	{
		if (found.empty())
			found.push_back(m_p);
		else
		{
			auto it = lower_bound(found.begin(), found.end(), m_p,
				[p](const node_f & a, const node_f & b)
			{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
			found.insert(it, m_p);
		}
	}
	//
	// number of point exceeded n so we have to binary search through found
	// to insert better solution then kick last one
	//
	else
	{
		// not binary search - but working brute force:
		double curr_dist = sq_distance(p, m_p);
		for (auto it = found.begin(); it != found.end(); ++it)
		{
			if (curr_dist <= sq_distance(p, it->m_p))
			{
				found.insert(it, m_p);
				if ((int)found.size() >= n)
					found.pop_back();
				break;
			}
		}
	}
	*/

	//
	// continue searching through appropriate branch
	//
	if (m_ul != NULL)
		if (sq_distance(get_closest_p(m_ul_r, p), p) <= sq_distance(p, found.back().m_p))
			m_ul->find_n_closest_points(p, n, found);
	if (m_ur != NULL)
		if (sq_distance(get_closest_p(m_ur_r, p), p) <= sq_distance(p, found.back().m_p))
			m_ur->find_n_closest_points(p, n, found);
	if (m_dl != NULL)
		if (sq_distance(get_closest_p(m_dl_r, p), p) <= sq_distance(p, found.back().m_p))
			m_dl->find_n_closest_points(p, n, found);
	if (m_dr != NULL)
		if (sq_distance(get_closest_p(m_dr_r, p), p) <= sq_distance(p, found.back().m_p))
			m_dr->find_n_closest_points(p, n, found);
	return found;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
quad_tree_f::quad_tree_f(void)
	: m_root(NULL)
{}

quad_tree_f::quad_tree_f(const quad_tree_f & src)
{
	if (src.m_root == NULL)
		m_root = NULL;
	else
		m_root = new node_f(*src.m_root);
}

quad_tree_f & quad_tree_f::operator = (const quad_tree_f & src)
{
	if (this == &src)
		return *this;
	clear();
	if (src.m_root != NULL)
		m_root = new node_f(*src.m_root);
	return *this;
}

quad_tree_f::~quad_tree_f(void)
{
	delete m_root;
	m_root = NULL;
}

inline void quad_tree_f::insert(double x, double y)
{
	insert(Tpoint<double>(x, y));
}

void quad_tree_f::insert(const Tpoint<double> & p)
{
	if (p.m_x < GMINX || p.m_x >= GMAXX || p.m_y < GMINY || p.m_y >= GMAXY)
		return;

	if (m_root == NULL)
	{
		m_root = new node_f(p);
		double MIN_X = GMINX;
		double MID_X = p.m_x;
		double MAX_X = GMAXX;
		double MIN_Y = GMINY;
		double MID_Y = p.m_y;
		double MAX_Y = GMAXY;

		m_root->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
		m_root->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
		m_root->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
		m_root->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
	}
	else
	{
		m_root->insert(p, m_root);
	}
}

void quad_tree_f::insert(const node_f & n)
{
	if (n.m_p.m_x < GMINX || n.m_p.m_x >= GMAXX || n.m_p.m_y < GMINY || n.m_p.m_y >= GMAXY)
		return;

	if (m_root == NULL)
	{
		m_root = new node_f(n.m_p);
		m_root->m_velocity = n.m_velocity;

		double MIN_X = GMINX;
		double MID_X = n.m_p.m_x;
		double MAX_X = GMAXX;
		double MIN_Y = GMINY;
		double MID_Y = n.m_p.m_y;
		double MAX_Y = GMAXY;

		m_root->m_ul_r = Trect<double>(Tpoint<double>(MIN_X, MIN_Y), Tpoint<double>(MID_X, MID_Y));
		m_root->m_ur_r = Trect<double>(Tpoint<double>(MID_X, MIN_Y), Tpoint<double>(MAX_X, MID_Y));
		m_root->m_dl_r = Trect<double>(Tpoint<double>(MIN_X, MID_Y), Tpoint<double>(MID_X, MAX_Y));
		m_root->m_dr_r = Trect<double>(Tpoint<double>(MID_X, MID_Y), Tpoint<double>(MAX_X, MAX_Y));
	}
	else
	{
		m_root->insert(n, m_root);
	}
}

void quad_tree_f::clear(void)
{
	delete m_root;
	m_root = NULL;
}

Tpoint<double> quad_tree_f::find_closest_point(const Tpoint<double> & p) const
{
	if (m_root == NULL)
		return Tpoint<double>();
	else
	{
		double best_dist = sq_distance(p, m_root->m_p);
		Tpoint<double> closest(m_root->m_p);
		return m_root->find_closest_point(p, closest, best_dist);
	}
}

vector<node_f> quad_tree_f::find_n_closest_points(const Tpoint<double> & p, int n)
{
	if (m_root == NULL)
	{
		cout << "m_root == NULL" << nl;
		return vector<node_f>();
	}
	else
	{
		vector<node_f> res;
		return m_root->find_n_closest_points(p, n, res);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
_Vec2<double> update_alignment(const vector<node_f> & closest_points, node_f & n)
{
	_Vec2<double> v (0.0f, 0.0f);
	if(closest_points.empty())
		return v;

	for (const auto & i : closest_points)
		v += i.m_velocity;
	v.x /= closest_points.size();
	v.y /= closest_points.size();
	v = v - n.m_velocity;

	return v;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
_Vec2<double> update_separation(const vector<node_f> & closest_points, node_f & n)
{
	_Vec2<double> v (0, 0);
	if(closest_points.empty())
		return v;

	for (const auto & i : closest_points)
	{
		if (i.m_p != n.m_p)
		{
			v.x += (i.m_p.m_x - n.m_p.m_x);
			v.y += (i.m_p.m_y - n.m_p.m_y);
		}
	}

	v.x *= -1;
	v.y *= -1;

	return v;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
_Vec2<double> update_cohesion(const vector<node_f> & closest_points, node_f & n)
{
	_Vec2<double> v (0, 0);
	if(closest_points.empty())
		return v;

	for (const auto & i : closest_points)
	{
		if (i.m_p != n.m_p)
		{
			v.x += i.m_p.m_x;
			v.y += i.m_p.m_y;
		}
	}

	v.x /= (closest_points.size() - 1);
	v.y /= (closest_points.size() - 1);

	return v;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void test_update_node(quad_tree_f & t, node_f & n, quad_tree_f & nt)
{
	vector<node_f> closest_points = t.find_n_closest_points(n.m_p, 10);

	_Vec2<double> alignment (0, 0);
	_Vec2<double> separation (0, 0);
	_Vec2<double> cohesion (0, 0);

	//alignment = update_alignment(closest_points, n);
	//separation = update_separation(closest_points, n);
	//cohesion = update_cohesion(closest_points, n);

	n.m_velocity.x += alignment.x * alignment_weight + separation.x * separation_weight + cohesion.x * cohesion_weight;
	n.m_velocity.y += alignment.y * alignment_weight + separation.y * separation_weight + cohesion.y * cohesion_weight;

	double length = sqrt(n.m_velocity.x * n.m_velocity.x + n.m_velocity.y * n.m_velocity.y);
	if(length >= 5.0f)
	{
		n.m_velocity.x /= length;
		n.m_velocity.y /= length;
	}

	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;

	if (n.m_p.m_x >= double(Graphics::ScreenWidth))
		n.m_p.m_x = double((int)(n.m_p.m_x) % Graphics::ScreenWidth);
	if (n.m_p.m_x < 0.0f)
		n.m_p.m_x += (double)Graphics::ScreenWidth;
	if (n.m_p.m_y >= double(Graphics::ScreenHeight))
		n.m_p.m_y = double((int)(n.m_p.m_y) % Graphics::ScreenHeight);
	if (n.m_p.m_y < 0.0f)
		n.m_p.m_y += (double)Graphics::ScreenHeight;

	if (n.m_ul != NULL)	test_update_node(t, *n.m_ul, nt);
	if (n.m_ur != NULL)	test_update_node(t, *n.m_ur, nt);
	if (n.m_dl != NULL)	test_update_node(t, *n.m_dl, nt);
	if (n.m_dr != NULL)	test_update_node(t, *n.m_dr, nt);

	nt.insert(n);
}

void test_update_tree(quad_tree_f & t)
{
	quad_tree_f nt;
	if(t.m_root != NULL)
		test_update_node(t, *t.m_root, nt);
	t = nt;
}
//-----------------------------------------------------------------------------
void local_test(void)
{
	quad_tree_f t, nt;
	int tree_size = 100;
	for (int i = 0; i < tree_size; ++i)
	{
		node_f x(Tpoint<double>(random_between(GMINX, GMAXX),random_between(GMINY, GMAXY)));
		x.m_velocity = _Vec2<double>(random_between(-1.0, 1.0), random_between(-1.0, 1.0));
		t.insert(x);
	}
	
	for(int i = 0; i < 1; ++i)
	{
		test_update_tree(t);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int main(void)
{
	srand(unsigned(time(0)));
	std::random_device rd;
    std::mt19937 g(rd());

    std::vector<node_f> v;
    std::deque<node_f> d;
    std::list<node_f> l;
    int n = 150;
    int count = 1000000;

    double time_vec = 0.0;
    double time_deq = 0.0;
    double time_lis = 0.0;

    //-----------------------------------------------------------------------------------
    for(int i = 0; i < count; ++i)
    {
    	Tpoint<double> p { random_between(-100.0, 100.0), random_between(-100.0, 100.0)};
    	Tpoint<double> m_p { random_between(-100.0, 100.0), random_between(-100.0, 100.0)};

    	CTimer timer_vec;
    	if (v.empty())
			v.push_back(m_p);
		else
		{
			auto it = lower_bound(v.begin(), v.end(), m_p,
					[p](const node_f & a, const node_f & b)
					{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
		
			v.insert(it, m_p);

			if ((int)v.size() > n)
				v.pop_back();
		}
		time_vec += timer_vec.elapsed();
		//++++++++++++++++++++++++++++++++++++
		CTimer timer_deq;
		if (d.empty())
			d.push_back(m_p);
		else
		{
			auto it = lower_bound(d.begin(), d.end(), m_p,
					[p](const node_f & a, const node_f & b)
					{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
		
			d.insert(it, m_p);

			if ((int)d.size() > n)
				d.pop_back();
		}
		time_deq += timer_deq.elapsed();
		//++++++++++++++++++++++++++++++++++++
		CTimer timer_lis;
		if (l.empty())
			l.push_back(m_p);
		else
		{
			auto it = lower_bound(l.begin(), l.end(), m_p,
					[p](const node_f & a, const node_f & b)
					{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
		
			l.insert(it, m_p);

			if ((int)l.size() > n)
				l.pop_back();
		}
		time_lis += timer_lis.elapsed();
    }
    //-----------------------------------------------------------------------------------

    // kontrola ze je vse spravne:
    auto itv = v.begin();
    auto itd = d.begin();
    auto itl = l.begin();
    for(; itv != v.end() && itd != d.end() && itl != l.end();)
    {
    	assert(itv->m_p == itd->m_p);
    	assert(itv->m_p == itl->m_p);
    	assert(itd->m_p == itl->m_p);
    	++itv; ++itd; ++itl;
    }

    cout << "vec :" << time_vec << " milliseconds" << nl;
    cout << "deq :" << time_deq << " milliseconds" << nl;
    cout << "lis :" << time_lis << " milliseconds" << nl;

	//system("PAUSE");
	return 0;
}
//-----------------------------------------------------------------------------
