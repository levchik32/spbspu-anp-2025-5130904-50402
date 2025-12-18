#include <cmath>
#include <iostream>
namespace saldaev
{
	struct Point_t
	{
		double x, y;
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
}

int main()
{
}

//
saldaev::Point_t & saldaev::Point_t::operator-=(const saldaev::Point_t &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

saldaev::Point_t & saldaev::Point_t::operator*=(const double &other)
{
	x *= other;
	y *= other;
	return *this;
}

//
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
