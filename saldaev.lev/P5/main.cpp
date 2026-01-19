#include <cmath>
#include <iostream>
namespace saldaev
{
  const size_t ShapeAmount = 3;

  struct point_t
  {
    double x_, y_;
    point_t operator-(const point_t other) const;
    point_t operator+(const point_t other) const;
    point_t operator*(const double coef) const;
    point_t operator/(const double coef) const;
    point_t &operator+=(const point_t other);
    point_t &operator-=(const point_t other);
    point_t &operator*=(const double other);
  };

  struct rectangle_t
  {
    double width_, height_;
    point_t pos_;
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
    virtual void doScale(double coef) = 0;
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
    void doScale(double coef) override;
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
    void doScale(double coef) override;
  };

  struct Polygon final: Shape
  {
    Polygon(point_t *vs, size_t kk);
    ~Polygon();
    Polygon(const Polygon &other);
    Polygon &operator=(const Polygon &other);
    Polygon(Polygon &&other) noexcept;
    Polygon &operator=(Polygon &&other) noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t target) noexcept override;
    void move(double dx, double dy) noexcept override;

  private:
    point_t *vertexes_;
    size_t k_;
    point_t pos_;
    void doScale(double coef) override;
  };

  point_t calculateCenter(const point_t *vs, const size_t kk);

  void isotropicScaleFromPoint(Shape **shps, const size_t k, const point_t pt, const double coef);

  void tellStatistics(const Shape **shps, const size_t k);
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
  if (std::cin.fail() || c < 0) {
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

saldaev::point_t saldaev::point_t::operator-(const point_t other) const
{
  return {x_ - other.x_, y_ - other.y_};
}

saldaev::point_t saldaev::point_t::operator+(const point_t other) const
{
  return {x_ + other.x_, y_ + other.y_};
}

saldaev::point_t saldaev::point_t::operator*(const double coef) const
{
  return {x_ * coef, y_ * coef};
}

saldaev::point_t saldaev::point_t::operator/(const double coef) const
{
  return {x_ / coef, y_ / coef};
}

saldaev::point_t &saldaev::point_t::operator+=(const point_t other)
{
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

saldaev::point_t &saldaev::point_t::operator-=(const point_t other)
{
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

saldaev::point_t &saldaev::point_t::operator*=(const double other)
{
  x_ *= other;
  y_ *= other;
  return *this;
}

void saldaev::Shape::scale(double coef)
{
  if (coef <= 0)
  {
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

double saldaev::Rectangle::getArea() const
{
  return width_ * height_;
}

saldaev::rectangle_t saldaev::Rectangle::getFrameRect() const
{
  return {width_, height_, pos_};
}

void saldaev::Rectangle::move(point_t target)
{
  pos_ = target;
}

void saldaev::Rectangle::move(double dx, double dy)
{
  pos_.x_ += dx;
  pos_.y_ += dy;
}

void saldaev::Rectangle::doScale(double coef)
{
  width_ *= coef;
  height_ *= coef;
}

saldaev::Square::Square(point_t p, double s):
  Shape(),
  pos_(p),
  side_(s)
{}

double saldaev::Square::getArea() const
{
  return side_ * side_;
}

saldaev::rectangle_t saldaev::Square::getFrameRect() const
{
  return {side_, side_, pos_};
}

void saldaev::Square::move(point_t target)
{
  pos_ = target;
}

void saldaev::Square::move(double dx, double dy)
{
  pos_.x_ += dx;
  pos_.y_ += dy;
}

void saldaev::Square::doScale(double coef)
{
  side_ *= coef;
}

saldaev::Polygon::Polygon(point_t *vs, size_t kk):
  k_(kk),
  pos_(calculateCenter(vs, kk)),
  vertexes_(new point_t[k_])
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
  vertexes_(new point_t[other.k_]),
  k_(other.k_),
  pos_(other.pos_)
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
  k_ = other.k_;
  pos_ = other.pos_;

  point_t* newVertexes = new point_t[other.k_];
  delete[] vertexes_;
  vertexes_ = newVertexes;
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] = other.vertexes_[i];
  }

  return *this;
}

saldaev::Polygon::Polygon(Polygon &&other) noexcept:
  vertexes_(other.vertexes_),
  k_(other.k_),
  pos_(other.pos_)
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

double saldaev::Polygon::getArea() const
{
  double area = 0;
  size_t j = 0;
  for (size_t i = 0; i < k_; ++i) {
    j = (i + 1) % k_;
    area += vertexes_[i].x_ * vertexes_[j].y_ - vertexes_[j].x_ * vertexes_[i].y_;
  }
  return std::abs(area) * 0.5;
}

saldaev::rectangle_t saldaev::Polygon::getFrameRect() const
{
  double maxx = vertexes_[0].x_;
  double minx = maxx;
  double maxy = vertexes_[0].y_;
  double miny = maxy;

  for (size_t i = 1; i < k_; ++i) {
    maxx = std::max(maxx, vertexes_[i].x_);
    minx = std::min(minx, vertexes_[i].x_);
    maxy = std::max(maxy, vertexes_[i].y_);
    miny = std::min(miny, vertexes_[i].y_);
  }

  double dx = maxx - minx;
  double dy = maxy - miny;
  return {dx, dy, {minx + dx / 2, miny + dy / 2}};
}

void saldaev::Polygon::move(point_t target)
{
  point_t dp = target - pos_;
  this->move(dp.x_, dp.y_);
}

void saldaev::Polygon::move(double dx, double dy)
{
  pos_ += {dx, dy};
  for (size_t i = 0; i < k_; ++i) {
    vertexes_[i] += {dx, dy};
  }
}

void saldaev::Polygon::doScale(double coef)
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
    t_area = (ab.x_ * ac.y_ - ab.y_ * ac.x_) / 2.0;
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
    point_t p1 = shps[i]->getFrameRect().pos_;
    shps[i]->move(pt);
    point_t p2 = shps[i]->getFrameRect().pos_;
    shps[i]->move(pt + ((p1 - p2) * coef));
    shps[i]->scale(coef);
  }
}

void saldaev::tellStatistics(Shape **shps, size_t k)
{
  double area = 0;
  double sum_area = 0;
  saldaev::rectangle_t frame = {0, 0, {0, 0}};
  double mix = shps[0]->getFrameRect().pos_.x_ - shps[0]->getFrameRect().width_ / 2;
  double max = mix;
  double miy = shps[0]->getFrameRect().pos_.y_ - shps[0]->getFrameRect().height_ / 2;
  double may = miy;

  for (size_t i = 0; i < k; ++i) {
    area = shps[i]->getArea();
    frame = shps[i]->getFrameRect();
    std::cout << " * own area - " << area << ", frame: {" << frame.pos_.x_ << ", ";
    std::cout << frame.pos_.y_ << "}, w - " << frame.width_ << ", h - " << frame.height_ << '\n';
    sum_area += area;
    mix = std::min(mix, frame.pos_.x_ - frame.width_ / 2);
    max = std::max(max, frame.pos_.x_ + frame.width_ / 2);
    miy = std::min(miy, frame.pos_.y_ - frame.height_ / 2);
    may = std::max(may, frame.pos_.y_ + frame.height_ / 2);
  }
  std::cout << "sum area - " << sum_area << ", frame: {" << (mix + max) / 2 << ", ";
  std::cout << (miy + may) / 2 << "}, w - " << max - mix << ", h - " << may - miy << "\n";
}
