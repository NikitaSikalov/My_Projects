//
// Created by sikalovnikita on 12.04.18.
//

#include "Geometry.h"

Vector::Vector(const Point &p1, const Point &p2) {
  this->x = p2.get_x() - p1.get_x();
  this->y = p2.get_y() - p1.get_y();
}

Vector& Vector::operator=(const Vector &a) {
  this->x = a.x;
  this->y = a.y;
  return (*this);
}

Vector Vector::operator+(const Vector &a){
  Vector result(this->x + a.x, this->y + a.y);
  return result;
}

Vector Vector::operator-(const Vector &a) const {
  Vector result(this->x - a.x, this->y - a.y);
  return result;
}

double Vector::operator&&(const Vector &a) const {
  double result = this->x * a.y - a.x * this->y;
  return result;
}

double abs(const Vector &a) {
  double result = fabs(a.x * a.x + a.y * a.y);
  return sqrt(result);
}

Vector operator*(const double &k, const Vector &a) {
  Vector result(a.x * k, a.y * k);
  return result;
}

double Vector::angle(const Vector &v2) const {
  Vector v1 = *this;
  double res;
  res = acos((v1 * v2) / (abs(v1) * abs(v2)));
  res *= (v1 && v2) < 0 ? -1 : 1;
  return res;
}

istream& operator>>(istream &is, Vector &a){
  Point p1, p2;
  is >> p1 >> p2;
  Vector ans(p1, p2);
  a = ans;
  return is;
}

ostream& operator<<(ostream &os, const Vector &a) {
  os << a.x << ' ' << a.y;
  return os;
}

istream& operator>>(istream &is, Point &p){
  is >> p.x >> p.y;
  return is;
}

Point& Point::operator=(const Point &p) {
  this->x = p.x;
  this->y = p.y;
  return (*this);
}

Figure& Point::shift(const Vector &a) {
  this->x += a.get_x();
  this->y += a.get_y();
  return (*this);
}

bool Point::cross_segment(const Segment &s) const {
  Vector v1(s.get_start(), *this);
  Vector v2(*this, s.get_end());
  return (v1 && v2) == 0 && (v1 * v2) >= 0;
}

Vector Point::operator-(const Point &p) const {
  Vector v(this->x - p.x, this->y - p.y);
  return v;
}

ostream& operator<<(ostream &os, const Point &p) {
  os << p.x << ' ' << p.y;
  return os;
}


Figure& Segment::shift(const Vector &v) {
  this->start.shift(v);
  this->end.shift(v);
  return *this;
}

bool Segment::cross_segment(const Segment &s) const {
  Vector v1(this->start, this->end);
  Vector v2(s.start, s.end);
  Point p1 = this->start, p2 = this->end;
  Point p3 = s.start, p4 = s.end;
  bool flag1 = ((p3 - p1) && (p2 - p1)) * ((p4 - p1) && (p2 - p1)) <= 0;
  bool flag2 = ((p1 - p3) && (p4 - p3)) * ((p2 - p3) && (p4 - p3)) <= 0;
  double x1 = this->start.get_x(), x2 = this->end.get_x();
  double x3 = s.start.get_x(), x4 = s.end.get_x();
  double y1 = this->start.get_y(), y2 = this->end.get_y();
  double y3 = s.start.get_y(), y4 = s.end.get_y();
  double a1 = min(x1, x2), a2 = max(x1, x2), b1 = min(y1, y2), b2 = max(y1, y2);
  double a3 = min(x3, x4), a4 = max(x3, x4), b3 = min(y3, y4), b4 = max(y3, y4);
  return (a2 >= a3) && (a1 <= a4) && (b2 >= b3) && (b1 <= b4) && flag1 && flag2;
}

double Segment::distance_point(const Point &p) const {
  Segment s1(this->start, p), s2(this->end, p);
  Line l(this->start, this->end);
  Vector v1(this->start, this->end), v2(this->end, this->start);
  Vector u1 = p - this->start, u2 = p - this->end;
  //cout << v1 << ' ' << u1 << ' ' << '\n';
  if(this->contains_point(p)) {
    return 0;
  }
  if(v1 * u1 >= 0 && v2 * u2 >= 0) {
    return l.distance_point(p);
  } else {
    if((v1 * u1) >= 0) {
      return s2.len_segment();
    } else {
      return s1.len_segment();
    }
  }
}

double Segment::distance_segment(const Segment &s2) const {
  Segment s1 = *this;
  if(s1.cross_segment(s2)){
    return 0;
  }
  double dis1 = s1.distance_point(s2.start), dis2 = s1.distance_point(s2.end);
  double dis3 = s2.distance_point(s1.start), dis4 = s2.distance_point(s1.end);
  double temp = min(dis1, dis2);
  double temp2 = min(temp, dis3);
  double ans = min(temp2, dis4);
  return ans;
}



Line::Line(const Point &p1, const Point &p2) {
  double x1 = p1.get_x(), x2 = p2.get_x();
  double y1 = p1.get_y(), y2 = p2.get_y();
  this->A = y2 - y1;
  this->B = x1 - x2;
  this->C = x2 * y1 - x1 * y2;
}

Line::Line(const Point &p, const Vector &v) {
  double A0 = v.get_y(), B0 = -(v.get_x());
  double C0 = -(A0 * p.get_x() + B0 * p.get_y());
  this->A = A0;
  this->B = B0;
  this->C = C0;
}

bool Line::cross_segment(const Segment &s) const {
  Vector v = s.get_end() - s.get_start();
  return (this->getDirectionVector() && v) != 0;
}

Point Line::cross_line(const Line &l) const {
  if((l.getDirectionVector() && this->getDirectionVector()) == 0) {
    MyExcept ex;
    throw ex;
  } else {
    double A1 = this->A, B1 = this->B, C1 = this->C;
    double A2 = l.A, B2 = l.B, C2 = l.C;
    double x = (B2 * C1 - B1 * C2) / (A2 * B1 - B2 * A1);
    double y = (C2 * A1 - A2 * C1) / (A2 * B1 - A1 * B2);
    Point p(x, y);
    return p;
  }
}

double Line::distance(const Line &l) const {
  if((l.getDirectionVector() && this->getDirectionVector()) != 0) {
    MyExcept ex;
    throw ex;
  } else {
    Line n(this->B, -(this->A), 0);
    Segment s(n.cross_line(*this), n.cross_line(l));
    return s.len_segment();
  }
}

double Line::distance_point(const Point &p) const {
  if(this->contains_point(p)) {
    return 0;
  } else {
    Line ln(p, this->getNormalVector());
    Point p2 = this->cross_line(ln);
    Segment s(p, p2);
    return s.len_segment();
  }
}



Ray::Ray(const Point &p1, const Point &p2){
  this->A = p1;
  this->V = p2 - p1;
}

bool Ray::contains_point(const Point &p) const {
  Vector u = p - this->A;
  bool f1 = (this->V && u) == 0;
  bool f2 = this->V * u >= 0;
  return (f1 && f2);
}

bool Ray::cross_segment(const Segment &s) const {
  Vector u = s.get_end() - s.get_start();
  return (this->V && u) != 0;
}

Figure &Ray::shift(const Vector &v) {
  this->A.shift(v);
  return *this;
}

double Ray::distance_point(const Point &p) const {
  Vector u = p - this->A;
  if(u * this->V < 0) {
    Segment s(p, this->A);
    return s.len_segment();
  } else {
    Line l(this->A, this->V);
    return l.distance_point(p);
  }
}


Polygon::Polygon() {
  this->tops = new Point[0];
  this->size = 0;
  this->capacity = 0;
}

Polygon::Polygon(const Point *points, int n) {
  this->tops = new Point[n];
  memcpy(this->tops, points, n * sizeof(Point));
  this->size = n;
  this->capacity = (size_t)n;
}

istream& operator>>(istream &is, Polygon &pol) {
  int n;
  is >> n;
  delete []pol.tops;
  pol.tops = new Point[n];
  pol.capacity = (size_t)n;
  pol.size = n;
  for (int i = 0; i < n; ++i) {
    is >> pol.tops[i];
  }
  return is;
}

ostream& operator<<(ostream &os, const Polygon &pol) {
  os << pol.size << '\n';
  for (int i = 0; i < pol.size; ++i) {
    os << pol.tops[i] << '\n';
  }
  return os;
}

bool Polygon::edge_point(const Point &p) const {
  for (int i = 0; i < this->size - 1; ++i) {
    Segment s(this->tops[i], this->tops[i + 1]);
    if(s.contains_point(p)){
      return true;
    }
  }
  return false;
}

bool Polygon::contains_point(const Point &p) const {
  if(this->edge_point(p)){
    return true;
  } else {
    Vector v0(p, this->tops[0]);
    Vector temp = v0;
    double ans = 0;
    for (int i = 1; i < this->size; ++i) {
      Vector vi(p, this->tops[i]);
      ans += temp.angle(vi);
      temp = vi;
    }
    ans += temp.angle(v0);
    return ans == 2 * M_PI;
  }
}

bool Polygon::is_convex() {
  int n = this->size;
  int i = 0;
  int sign = 1;
  bool flag = true;
  while(i < this->size && flag){
    int ind1 = i, ind2 = (i + 1) % n, ind3 = (i + 2) % n;
    Vector v1(this->tops[ind1], this->tops[ind2]);
    Vector v2(this->tops[ind2], this->tops[ind3]);
    sign = (v1 && v2) < 0 ? -1 : 1;
    if((v1 && v2) != 0){
      flag = false;
    }
    i++;
  }
  for (int j = 1; j < this->size; ++j) {
    int ind1 = j, ind2 = (j + 1) % n, ind3 = (j + 2) % n;
    Vector v1(this->tops[j], this->tops[ind2]);
    Vector v2(this->tops[ind2], this->tops[ind3]);
    if((v1 && v2) * sign < 0){
      return false;
    }
  }
  return true;
}
