#include <cmath>
#include <iostream>
namespace saldaev
{
	struct Point_t
	{
		double x, y;
		Point_t operator-(const Point_t &other);
		Point_t operator+(const Point_t &other);
		Point_t operator*(const double &coef);
		Point_t operator/(const double &coef);
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

	struct Rectangle : Shape
	{
		Rectangle(Point_t p, double w, double h);
		double getArea() const override;
		Rectangle_t getFrameRect() const override;
		void move(Point_t target) override;
		void move(double dx, double dy) override;
		void scale(double coef) override;

	private:
		Point_t pos;
		double width, height;
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

	void isotropicScaleFromPoint(Shape **shps, size_t k, Point_t pt, double coef);
}

int main()
{
	using saldaev::Point_t;
	using saldaev::Polygon;
	using saldaev::Rectangle;
	using saldaev::Shape;
	using saldaev::Square;

	Shape *shps[3] = {};
	shps[0] = new Rectangle({10, 10}, 5, 3);
	shps[1] = new Square({-1, -1}, 10);
	Point_t vss[6] = {{0, 0}, {-2, 3}, {-1, 5}, {0, 4}, {1, 5}, {2, 3}};
	shps[2] = new Polygon(vss, 6);

	double area = 0;
	double sum_area = 0;
	saldaev::Rectangle_t frame = {0, 0, {0, 0}};
	double mix, max, miy, may;
	double x = 0, y = 0, c = 0;
	while (std::cin >> x >> y >> c)
	{
		std::cout << "before:\n";
		for (size_t i = 0; i < 3; ++i)
		{
			area = shps[i]->getArea();
			frame = shps[i]->getFrameRect();
			std::cout << " * own area - " << area << ", frame: {" << frame.pos.x << ", ";
			std::cout << frame.pos.y << "}, w - " << frame.width << ", h - " << frame.height << '\n';
			sum_area += area;
			mix = std::min(mix, frame.pos.x - frame.width / 2);
			max = std::max(max, frame.pos.x + frame.width / 2);
			miy = std::min(miy, frame.pos.y - frame.height / 2);
			may = std::max(may, frame.pos.y + frame.height / 2);
		}
		std::cout << "sum area - " << sum_area << ", frame: {" << (mix + max) / 2 << ", ";
		std::cout << (miy + may) / 2 << "}, w - " << max - mix << ", h - " << may - miy << '\n';

		isotropicScaleFromPoint(shps, 3, {x, y}, c);

		sum_area = 0;
		std::cout << "after:\n";
		for (size_t i = 0; i < 3; ++i)
		{
			area = shps[i]->getArea();
			frame = shps[i]->getFrameRect();
			std::cout << " * own area - " << area << ", frame: {" << frame.pos.x << ", ";
			std::cout << frame.pos.y << "}, w - " << frame.width << ", h - " << frame.height << '\n';
			sum_area += area;
			mix = std::min(mix, frame.pos.x - frame.width / 2);
			max = std::max(max, frame.pos.x + frame.width / 2);
			miy = std::min(miy, frame.pos.y - frame.height / 2);
			may = std::max(may, frame.pos.y + frame.height / 2);
		}
		std::cout << "sum area - " << sum_area << ", frame: {" << (mix + max) / 2 << ", ";
		std::cout << (miy + may) / 2 << "}, w - " << max - mix << ", h - " << may - miy << '\n';
	}

	for (size_t i = 0; i < 3; ++i)
	{
		delete shps[i];
	}

	if (std::cin.eof())
	{
		return 0;
	}
	std::cerr << "bad input";
	return 1;
}

saldaev::Point_t saldaev::Point_t::operator-(const Point_t &other)
{
	return {x - other.x, y - other.y};
}

saldaev::Point_t saldaev::Point_t::operator+(const Point_t &other)
{
	return {x + other.x, y + other.y};
}

saldaev::Point_t saldaev::Point_t::operator*(const double &coef)
{
	return {x * coef, y * coef};
}

saldaev::Point_t saldaev::Point_t::operator/(const double &coef)
{
	return {x / coef, y / coef};
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

saldaev::Rectangle::Rectangle(Point_t p, double w, double h) : Shape(), pos(p), width(w), height(h)
{
}

double saldaev::Rectangle::getArea() const
{
	return width * height;
}

saldaev::Rectangle_t saldaev::Rectangle::getFrameRect() const
{
	return {width, height, pos};
}

void saldaev::Rectangle::move(Point_t target)
{
	pos = target;
}

void saldaev::Rectangle::move(double dx, double dy)
{
	pos.x += dx;
	pos.y += dy;
}

void saldaev::Rectangle::scale(double coef)
{
	width *= coef;
	height *= coef;
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
		vertexes(new Point_t[other.k]), pos(other.pos)
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
		vertexes(other.vertexes), pos(other.pos)
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
	pos += dp;
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

saldaev::Point_t saldaev::calculateCenter(Point_t *vs, size_t kk)
{
	if (kk < 3)
	{
		throw std::logic_error("Invalid polygon vertices");
	}
	Point_t centre = {0, 0};
	double t_area = 0;
	double sum_area = 0;
	Point_t ab = {0, 0};
	Point_t ac = {0, 0};
	for (size_t i = 1; i < kk - 1; ++i)
	{
		ab = vs[i] - vs[0];
		ac = vs[i + 1] - vs[0];
		t_area = (ab.x * ac.y - ab.y * ac.x) / 2.0;
		centre += ((vs[0] + vs[i] + vs[i + 1]) / 3.0) * t_area;
		sum_area += t_area;
	}

	if (sum_area == 0.0)
	{
		throw std::logic_error("Invalid polygon vertices");
	}
	return centre / sum_area;
}

void saldaev::isotropicScaleFromPoint(Shape **shps, size_t k, Point_t pt, double coef)
{
	for (size_t i = 0; i < k; ++i)
	{
		Point_t p1 = shps[i]->getFrameRect().pos;
		shps[i]->move(pt);
		Point_t p2 = shps[i]->getFrameRect().pos;
		shps[i]->move(pt + ((p1 - p2) * coef));
		shps[i]->scale(coef);
	}
}
