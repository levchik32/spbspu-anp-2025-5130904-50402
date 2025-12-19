#include <cmath>
#include <iostream>
namespace saldaev
{
	struct Point_t
	{
		double x, y;
		Point_t operator-(const Point_t &other);
		Point_t operator*(const double &coef);
		Point_t &operator+=(const Point_t &other);
		Point_t &operator-=(const Point_t &other);
		Point_t &operator*=(const double &other);
	};

	struct Rectangle_t
	{
		double width, height;
		Point_t pos;
	};

	struct Shape
	{
		virtual double getArea() const = 0;
		virtual Rectangle_t getFrameRect() const = 0;
		virtual void move(Point_t target) = 0;
		virtual void move(double dx, double dy) = 0;
		virtual void scale(double coef) = 0;
		virtual ~Shape() = default;
	};

	struct Square : Shape
	{
		Square(Point_t p, double s);
		double getArea() const override;
		Rectangle_t getFrameRect() const override;
		void move(Point_t target) override;
		void move(double dx, double dy) override;
		void scale(double coef) override;

	private:
		Point_t pos;
		double side;
	};

	struct Polygon : Shape
	{
		Polygon(Point_t *vs, size_t kk);
		~Polygon();
		Polygon(const Polygon &other);
		Polygon &operator=(const Polygon &other);
		Polygon(Polygon &&other) noexcept;
		Polygon &operator=(Polygon &&other) noexcept;

		double getArea() const override;
		Rectangle_t getFrameRect() const override;
		void move(Point_t target) override;
		void move(double dx, double dy) override;
		void scale(double coef) override;

	private:
		Point_t *vertexes;
		size_t k;
		Point_t pos;
	};

	Point_t calculateCenter(Point_t *vs, size_t kk);
}

int main()
{
}

saldaev::Point_t saldaev::Point_t::operator-(const Point_t &other)
{
	return {x - other.x, y - other.y};
}

saldaev::Point_t saldaev::Point_t::operator*(const double &coef)
{
	return {x * coef, y * coef};
}

saldaev::Point_t &saldaev::Point_t::operator+=(const Point_t &other)
{
	x += other.x;
	y += other.y;
	return *this;
}

saldaev::Point_t &saldaev::Point_t::operator-=(const Point_t &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

saldaev::Point_t &saldaev::Point_t::operator*=(const double &other)
{
	x *= other;
	y *= other;
	return *this;
}

saldaev::Square::Square(Point_t p, double s) : Shape(), pos(p), side(s)
{
}

double saldaev::Square::getArea() const
{
	return side * side;
}

saldaev::Rectangle_t saldaev::Square::getFrameRect() const
{
	return {side, side, pos};
}

void saldaev::Square::move(Point_t target)
{
	pos = target;
}

void saldaev::Square::move(double dx, double dy)
{
	pos.x += dx;
	pos.y += dy;
}

void saldaev::Square::scale(double coef)
{
	side *= coef;
}

saldaev::Polygon::Polygon(Point_t *vs, size_t kk) : k(kk), pos(calculateCenter(vs, kk))
{
	vertexes = new Point_t[k];
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] = vs[i];
	}
}

saldaev::Polygon::~Polygon()
{
	delete[] vertexes;
}

saldaev::Polygon::Polygon(const Polygon &other) : k(other.k),
																									vertexes(new Point_t[other.k]),
																									pos(other.pos)
{
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] = other.vertexes[i];
	}
}

saldaev::Polygon &saldaev::Polygon::operator=(const Polygon &other)
{
	if (&other == this)
	{
		return *this;
	}

	k = other.k;
	pos = other.pos;

	delete[] vertexes;
	vertexes = new Point_t[other.k];
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] = other.vertexes[i];
	}

	return *this;
}

saldaev::Polygon::Polygon(Polygon &&other) noexcept : k(other.k),
																											vertexes(other.vertexes),
																											pos(other.pos)
{
	other.vertexes = nullptr;
}

saldaev::Polygon &saldaev::Polygon::operator=(Polygon &&other) noexcept
{
	if (&other == this)
	{
		return *this;
	}

	k = other.k;
	pos = other.pos;

	delete[] vertexes;
	vertexes = other.vertexes;
	other.vertexes = nullptr;

	return *this;
}

double saldaev::Polygon::getArea() const
{
	double area = 0;
	size_t j = 0;
	for (size_t i = 0; i < k; ++i)
	{
		j = (i + 1) % k;
		area += vertexes[i].x * vertexes[j].y - vertexes[j].x * vertexes[i].y;
	}
	return std::abs(area) * 0.5;
}

saldaev::Rectangle_t saldaev::Polygon::getFrameRect() const
{
	double maxx = vertexes[0].x;
	double minx = maxx;
	double maxy = vertexes[0].y;
	double miny = maxy;

	for (size_t i = 1; i < k; ++i)
	{
		maxx = std::max(maxx, vertexes[i].x);
		minx = std::min(minx, vertexes[i].x);
		maxy = std::max(maxy, vertexes[i].y);
		miny = std::min(miny, vertexes[i].y);
	}

	double dx = maxx - minx;
	double dy = maxy - miny;
	return {dx, dy, {minx + dx / 2, miny + dy / 2}};
}

void saldaev::Polygon::move(Point_t target)
{
	Point_t dp = target - pos;
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] += dp;
	}
}

void saldaev::Polygon::move(double dx, double dy)
{
	pos += {dx, dy};
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] += {dx, dy};
	}
}

void saldaev::Polygon::scale(double coef)
{
	for (size_t i = 0; i < k; ++i)
	{
		vertexes[i] += (vertexes[i] - pos) * (coef - 1);
	}
}
