//
// Created by sikalovnikita on 12.04.18.
//

#ifndef MY_PROJECTS_RATIONAL_H
#define MY_PROJECTS_RATIONAL_H

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <stdlib.h>

class Rational {
private:
  int p;
  int q;
  void reduce();
  float get_value() const ;
public:
  Rational();
  Rational(int p, int q);
  Rational(int value);
  Rational(const Rational&);

  friend int nod(const Rational&);
  friend std::ostream& operator<<(std::ostream&, const Rational&);
  friend std::istream& operator>>(std::istream&, Rational&);
  int getNumerator() const ;
  int getDenominator() const ;

  friend Rational operator/(const Rational&, const Rational&);
  friend Rational operator+(const Rational&, const Rational&);
  friend Rational operator*(const Rational&, const Rational&);
  friend Rational operator-(const Rational&, const Rational&);

  Rational& operator+=(const Rational&);
  Rational& operator-=(const Rational&);
  Rational& operator*=(const Rational&);
  Rational& operator/=(const Rational&);
  Rational& operator++();
  Rational operator++(int);
  Rational operator--();
  Rational operator--(int);
  Rational operator-()const;
  Rational operator+();


  friend bool operator<(const Rational&, const Rational&);
  friend bool operator<=(const Rational&, const Rational&);
  friend bool operator==(const Rational&, const Rational&);
  friend bool operator>=(const Rational&, const Rational&);
  friend bool operator>(const Rational&, const Rational&);
  friend bool operator!=(const Rational&, const Rational&);
};

class RationalDivisionByZero : logic_error {
public:
  RationalDivisionByZero(): logic_error("Error") {};
};

Rational::Rational() {
  this->p = 1;
  this->q = 1;
}

Rational::Rational(int p, int q) {
  this->p = p;
  this->q = q;
  this->reduce();
}

Rational::Rational(int value){
  this->p= value;
  this->q = 1;
}

void Rational::reduce() {
  int n = nod(*this);
  this->p /= n;
  this->q /= n;
  if(this->q < 0){
    this->p *= -1;
    this->q *= -1;
  }
}

int nod(const Rational &x){
  int a = x.p < x.q ? x.p : x.q;
  int b = x.p < x.q ? x.q : x.p;
  while(a != 0){
    int temp = a;
    a = b % a;
    b = temp;
  }
  return b;
}

std::ostream& operator<<(std::ostream &os, const Rational &x) {
  os << x.p;
  if(x.p != 0 && x.q != 1){
    os << '/' << x.q;
  }
  return os;
}

std::istream &operator>>(std::istream &is, Rational &x) {
  char s[100];
  is >> s;
  char *c = strchr(s, '/');
  x.p = atoi(s);
  if(c != NULL) {
    x.q = atoi(c + 1);
  }
  x.reduce();
  return is;
}

int Rational::getNumerator() const {
  return this->p;
}

int Rational::getDenominator() const {
  return this->q;
}

float Rational::get_value() const {
  return (float)(this->p) / this->q;
}

Rational operator/(const Rational &a, const Rational &b){
  int nominator = a.p * b.q;
  int denominator = a.q * b.p;
  if(a == 1 && b == 0) {
    RationalDivisionByZero ex;
    throw ex;
  } else {
    if(b == 0){
      RationalDivisionByZero ex;
      throw ex;
    }
  }
  Rational result(nominator, denominator);
  return result;
}

Rational operator+(const Rational& a, const Rational& b){
  int nominator = a.p * b.q + b.p * a.q;
  int denominator = b.q * a.q;
  Rational result(nominator, denominator);
  return result;
}

Rational operator*(const Rational& a, const Rational& b){
  int nominator = a.p * b.p;
  int denominator = b.q * a.q;
  Rational result(nominator, denominator);
  return result;
}

Rational operator-(const Rational& a, const Rational& b) {
  int nominator = a.p * b.q - b.p * a.q;
  int denominator = b.q * a.q;
  Rational result(nominator, denominator);
  return result;
}

Rational& Rational::operator+=(const Rational& that) {
  *this = *this + that;
  return *this;
}

Rational& Rational::operator-=(const Rational& that) {
  *this = *this - that;
  return *this;
}

Rational& Rational::operator*=(const Rational& that) {
  *this = *this * that;
  return *this;
}

Rational& Rational::operator/=(const Rational& that) {
  *this = *this / that;
  return *this;
}

Rational& Rational::operator++(){
  *this += 1;
  return *this;
}

Rational Rational::operator++(int) {
  Rational temp = *this;
  *this += 1;
  return temp;
}

Rational Rational::operator--(){
  *this -= 1;
  return *this;
}

Rational Rational::operator--(int) {
  Rational temp = *this;
  *this -= 1;
  return temp;
}

Rational Rational::operator-() const{
  Rational temp = *this;
  temp *= -1;
  return temp;
}

Rational Rational::operator+() {
  Rational temp = *this;
  return temp;
}

bool operator<(const Rational &a, const Rational &b){
  return a.get_value() < b.get_value();
}

bool operator<=(const Rational &a, const Rational &b){
  return a.get_value() <= b.get_value();
}

bool operator==(const Rational &a, const Rational &b){
  return a.get_value() == b.get_value();
}

bool operator>=(const Rational &a, const Rational &b){
  return a.get_value() >= b.get_value();
}

bool operator>(const Rational &a, const Rational &b){
  return a.get_value() > b.get_value();
}

bool operator!=(const Rational &a, const Rational &b) {
  return a.get_value() != b.get_value();
}

Rational::Rational(const Rational &that) {
  this->p = that.p;
  this->q = that.q;
}

#endif //MY_PROJECTS_RATIONAL_H
