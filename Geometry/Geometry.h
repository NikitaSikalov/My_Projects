//
// Created by sikalovnikita on 12.04.18.
//

#ifndef MY_PROJECTS_GEOMETRY_H
#define MY_PROJECTS_GEOMETRY_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstring>

using namespace std;

class Vector;
class Point;
class Segment;

class Figure {
public:
  virtual Figure& shift(const Vector&) = 0;
  virtual bool contains_point(const Point&) const = 0;
  virtual bool cross_segment(const Segment&) const = 0;
};

class Vector{
private:
  double x;
  double y;
public:
  Vector(const Point &p1, const Point &p2);
  Vector(const double &x = 0, const double &y = 0): x(x), y(y) {};
  Vector(const Vector &a): x(a.x), y(a.y) {};

  friend double abs(const Vector&);
  Vector& operator=(const Vector&);

  friend istream& operator>>(istream&, Vector&);
  friend ostream& operator<<(ostream&, const Vector&);
  friend Vector operator*(const double&, const Vector&);
  friend Vector operator*(const Vector &a, const double &k)  {return k * a; };
  double operator*(const Vector &a) const {return (this->x * a.x + this->y * a.y); };
  Vector operator+(const Vector&);
  Vector operator-(const Vector&) const ;
  double operator&&(const Vector&) const ;
  double angle(const Vector&) const;

  double get_x() const { return this->x; };
  double get_y() const { return this->y; };
};

class Point : public Figure {
private:
  double x;
  double y;
public:
  Point(const double &a = 0, const double &b = 0): x(a), y(b) {};
  double get_x() const {return this->x; };
  double get_y() const {return this->y; };
  Point& operator=(const Point&);
  friend istream& operator>>(istream&, Point&);
  Figure& shift(const Vector&);
  bool contains_point(const Point &p) const {return (this->x == p.x && this->y == p.y); };
  bool cross_segment(const Segment&) const ;
  Vector operator-(const Point &p) const ;
  friend ostream& operator<<(ostream &os, const Point &p);

};

class Line: public Figure {
private:
  double A, B, C;
public:
  Line(double a, double b, double c) : A(a), B(b), C(c) {};
  Line(const Point&, const Point&);
  Line(const Point&, const Vector&);
  Figure& shift(const Vector&) { return *this; };
  bool contains_point(const Point &p) const { return (A * p.get_x() + B * p.get_y() + C) == 0; };
  bool cross_segment(const Segment&) const;
  Point cross_line(const Line&) const;
  double distance(const Line&) const;
  double distance_point(const Point&) const;
  Vector getDirectionVector() const { return (-B, A); };
  Vector getNormalVector() const { return  (A, B) ; };
};

class Segment: public  Figure {
private:
  Point start;
  Point end;
public:
  Segment(const Point &p1, const Point &p2): start(p1), end(p2) {};
  Point get_start() const { return this->start; };
  Point get_end() const { return  this->end; };
  Figure& shift(const Vector&);
  bool contains_point(const Point &p) const { return p.cross_segment(*this);};
  bool cross_segment(const Segment&) const ;
  double len_segment() const { return abs(this->end - this->start); };
  double distance_point(const Point&) const;
  double distance_segment(const Segment&) const;
};

class Ray: public  Figure {
private:
  Point A;
  Vector V;
public:
  Ray(const Point&, const Point&);
  bool contains_point(const Point&) const;
  bool cross_segment(const Segment&) const;
  Figure& shift(const Vector &v);
  double distance_point(const Point&) const;
};

class Polygon: public Figure {
private:
  Point* tops;
  int size;
  size_t capacity;
public:
  Polygon();
  Polygon(const Point*, int n);
  friend istream& operator>>(istream &is, Polygon &pol);
  friend ostream& operator<<(ostream &os, const Polygon &pol);
  Figure& shift(const Vector&) { return *this; };
  bool contains_point(const Point&) const;
  bool edge_point(const Point&) const;
  bool cross_segment(const Segment&) const { return true; };
  bool is_convex();

  ~Polygon() { delete []this->tops; };
};

class MyExcept: logic_error {
public:
  MyExcept() : logic_error("Error") {};
};

#endif //MY_PROJECTS_GEOMETRY_H
