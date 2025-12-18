namespace saldaev
{
	struct Point_t
	{
		double x, y;
	};

	struct Rectangle_t
	{
		double width, height;
		Point_t pos;
	};

	struct Shape
	{
		virtual double getArea() = 0;
		virtual Rectangle_t getFrameRect() = 0;
		virtual void move(Point_t target) = 0;
		virtual void move(double dx, double dy) = 0;
		virtual void scale(double coefficient) = 0;
		virtual ~Shape() = default;
	};
	
}

int main()
{
}