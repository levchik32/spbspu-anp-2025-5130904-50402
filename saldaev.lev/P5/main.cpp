#include <cmath>
#include <iostream>
namespace saldaev
{
  const size_t ShapeAmount = 3;

  struct point_t
  {
    double x, y;
  };

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  struct Shape
  {
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t target) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    void scale(double coef);
    void nsfwscale(double coef) noexcept;

  private:
    virtual void doScale(double coef) noexcept = 0;
  };

  struct Rectangle final: Shape
  {
    Rectangle(point_t p, double w, double h);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t pos_;
    double width_, height_;
    void doScale(double coef) noexcept override;
  };

  struct Square final: Shape
  {
    Square(point_t p, double s);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t pos_;
    double side_;
    void doScale(double coef) noexcept override;
  };

  struct Polygon final: Shape
  {
    Polygon(const Polygon &other);
    Polygon(Polygon &&other) noexcept;
    Polygon(point_t *vs, size_t kk);
    ~Polygon() noexcept;
    Polygon &operator=(const Polygon &other);
    Polygon &operator=(Polygon &&other) noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    size_t k_;
    point_t pos_;
    point_t *vertexes_;
    void doScale(double coef) noexcept override;
  };

  point_t operator+(const point_t &a, const point_t &b) noexcept;
  point_t operator-(const point_t &a, const point_t &b) noexcept;
  point_t operator*(const point_t &p, double coef) noexcept;
  point_t operator/(const point_t &p, double coef) noexcept;

  point_t &operator+=(point_t &a, const point_t &b) noexcept;
  point_t &operator-=(point_t &a, const point_t &b) noexcept;
  point_t &operator*=(point_t &p, double coef) noexcept;
  point_t &operator/=(point_t &p, double coef) noexcept;

  point_t calculateCenter(const point_t *vs, const size_t kk);
  void isotropicScaleFromPoint(Shape **shps, const size_t k, const point_t pt, const double coef);
  void tellStatistics(const Shape *const *shps, const size_t k) noexcept;
}

int main()
{
  using saldaev::point_t;
  using saldaev::Polygon;
  using saldaev::Rectangle;
  using saldaev::Shape;
  using saldaev::Square;

  Shape *shps[saldaev::ShapeAmount] = {nullptr, nullptr, nullptr};
  try {
    shps[0] = new Rectangle({10, 10}, 5, 3);
    shps[1] = new Square({-1, -1}, 10);
    point_t vss[6] = {{0, 0}, {-2, 3}, {-1, 5}, {0, 4}, {1, 5}, {2, 3}};
    shps[2] = new Polygon(vss, 6);
  } catch (const std::bad_alloc &e) {
    for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
      delete shps[i];
    }
    return 1;
  }

  std::cout << "before:\n";
  saldaev::tellStatistics(shps, saldaev::ShapeAmount);

  double x = 0, y = 0, c = 0;
  std::cin >> x >> y >> c;
  if (std::cin.fail() || c <= 0) {
    std::cerr << "bad input" << '\n';
    for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
      delete shps[i];
    }
    return 1;
  }

  isotropicScaleFromPoint(shps, saldaev::ShapeAmount, {x, y}, c);

  std::cout << "\nafter:\n";
  saldaev::tellStatistics(shps, saldaev::ShapeAmount);

  for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
    delete shps[i];
  }

  return 0;
}

saldaev::point_t saldaev::operator-(const point_t &a, const point_t &b) noexcept
{
  return {a.x - b.x, a.y - b.y};
}

saldaev::point_t saldaev::operator+(const point_t &a, const point_t &b) noexcept
{
  return {a.x + b.x, a.y + b.y};
}

saldaev::point_t saldaev::operator*(const point_t &p, double coef) noexcept
{
  return {p.x * coef, p.y * coef};
}

saldaev::point_t saldaev::operator/(const point_t &p, double coef) noexcept
{
  return {p.x / coef, p.y / coef};
}

saldaev::point_t &saldaev::operator+=(point_t &a, const point_t &b) noexcept
{
  a.x += b.x;
  a.y += b.y;
  return a;
}

saldaev::point_t &saldaev::operator-=(point_t &a, const point_t &b) noexcept
{
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

saldaev::point_t &saldaev::operator*=(point_t &p, double coef) noexcept
{
  p.x *= coef;
  p.y *= coef;
  return p;
}

saldaev::point_t &saldaev::operator/=(point_t &p, double coef) noexcept
{
  p.x /= coef;
  p.y /= coef;
  return p;
}

void saldaev::Shape::scale(double coef)
{
  if (coef <= 0) {
    throw std::logic_error("coef <= 0");
  }
  doScale(coef);
}

void saldaev::Shape::nsfwscale(double coef) noexcept
{
  doScale(coef);
}

saldaev::Rectangle::Rectangle(point_t p, double w, double h):
  pos_(p),
  width_(w),
  height_(h)
{}

double saldaev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

saldaev::rectangle_t saldaev::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, pos_};
}

void saldaev::Rectangle::move(point_t target) noexcept
{
  pos_ = target;
}

void saldaev::Rectangle::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
}

void saldaev::Rectangle::doScale(double coef) noexcept
{
  width_ *= coef;
  height_ *= coef;
}

saldaev::Square::Square(point_t p, double s):
  pos_(p),
  side_(s)
{}

double saldaev::Square::getArea() const noexcept
{
  return side_ * side_;
}

saldaev::rectangle_t saldaev::Square::getFrameRect() const noexcept
{
  return {side_, side_, pos_};
}

void saldaev::Square::move(point_t target) noexcept
{
  pos_ = target;
}

void saldaev::Square::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
}

void saldaev::Square::doScale(double coef) noexcept
{
  side_ *= coef;
}

saldaev::Polygon::Polygon(point_t *vs, size_t kk):
  k_(kk),
  pos_(calculateCenter(vs, kk)),
  vertexes_(new point_t[kk])
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] = vs[i];
  }
}

saldaev::Polygon::~Polygon()
{
  delete[] vertexes_;
}

saldaev::Polygon::Polygon(const Polygon &other):
  k_(other.k_),
  pos_(other.pos_),
  vertexes_(new point_t[other.k_])
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] = other.vertexes_[i];
  }
}

saldaev::Polygon &saldaev::Polygon::operator=(const Polygon &other)
{
  if (&other == this) {
    return *this;
  }

  point_t *newVertexes = new point_t[other.k_];
  for (size_t i = 0; i < k_; ++i) {
    newVertexes[i] = other.vertexes_[i];
  }

  delete[] vertexes_;
  vertexes_ = newVertexes;
  k_ = other.k_;
  pos_ = other.pos_;
  return *this;
}

saldaev::Polygon::Polygon(Polygon &&other) noexcept:
  k_(other.k_),
  pos_(other.pos_),
  vertexes_(other.vertexes_)
{
  other.vertexes_ = nullptr;
}

saldaev::Polygon &saldaev::Polygon::operator=(Polygon &&other) noexcept
{
  if (&other == this) {
    return *this;
  }

  k_ = other.k_;
  pos_ = other.pos_;

  delete[] vertexes_;
  vertexes_ = other.vertexes_;
  other.vertexes_ = nullptr;

  return *this;
}

double saldaev::Polygon::getArea() const noexcept
{
  double area = 0;
  size_t j = 0;
  for (size_t i = 0; i < k_; ++i) {
    j = (i + 1) % k_;
    area += vertexes_[i].x * vertexes_[j].y - vertexes_[j].x * vertexes_[i].y;
  }
  return std::abs(area) * 0.5;
}

saldaev::rectangle_t saldaev::Polygon::getFrameRect() const noexcept
{
  double maxx = vertexes_[0].x;
  double minx = maxx;
  double maxy = vertexes_[0].y;
  double miny = maxy;

  for (size_t i = 1; i < k_; ++i) {
    maxx = std::max(maxx, vertexes_[i].x);
    minx = std::min(minx, vertexes_[i].x);
    maxy = std::max(maxy, vertexes_[i].y);
    miny = std::min(miny, vertexes_[i].y);
  }

  double dx = maxx - minx;
  double dy = maxy - miny;
  return {dx, dy, {minx + dx / 2, miny + dy / 2}};
}

void saldaev::Polygon::move(point_t target) noexcept
{
  point_t dp = target - pos_;
  move(dp.x, dp.y);
}

void saldaev::Polygon::move(double dx, double dy) noexcept
{
  pos_ += {dx, dy};
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] += {dx, dy};
  }
}

void saldaev::Polygon::doScale(double coef) noexcept
{
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] += (vertexes_[i] - pos_) * (coef - 1);
  }
}

saldaev::point_t saldaev::calculateCenter(const point_t *vs, const size_t kk)
{
  if (kk < 3) {
    throw std::logic_error("Invalid polygon vertices");
  }
  point_t centre = {0, 0};
  double t_area = 0;
  double sum_area = 0;
  point_t ab = {0, 0};
  point_t ac = {0, 0};
  for (size_t i = 1; i < kk - 1; ++i) {
    ab = vs[i] - vs[0];
    ac = vs[i + 1] - vs[0];
    t_area = (ab.x * ac.y - ab.y * ac.x) / 2.0;
    centre += ((vs[0] + vs[i] + vs[i + 1]) / 3.0) * t_area;
    sum_area += t_area;
  }

  if (sum_area == 0.0) {
    throw std::logic_error("Invalid polygon vertices");
  }
  return centre / sum_area;
}

void saldaev::isotropicScaleFromPoint(Shape **shps, size_t k, point_t pt, double coef)
{
  for (size_t i = 0; i < k; ++i) {
    point_t p1 = shps[i]->getFrameRect().pos;
    shps[i]->move(pt);
    point_t p2 = shps[i]->getFrameRect().pos;
    shps[i]->move(pt + ((p1 - p2) * coef));
    shps[i]->nsfwscale(coef);
  }
}

void saldaev::tellStatistics(const Shape *const *shps, size_t k) noexcept
{
  double area = 0;
  double sum_area = 0;
  saldaev::rectangle_t frame = {0, 0, {0, 0}};
  double mix = shps[0]->getFrameRect().pos.x - shps[0]->getFrameRect().width / 2;
  double max = mix;
  double miy = shps[0]->getFrameRect().pos.y - shps[0]->getFrameRect().height / 2;
  double may = miy;

  for (size_t i = 0; i < k; ++i) {
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
  std::cout << (miy + may) / 2 << "}, w - " << max - mix << ", h - " << may - miy << "\n";
}
