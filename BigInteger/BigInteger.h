//
// Created by sikalovnikita on 12.04.18.
//

#ifndef MY_PROJECTS_BIGINTEGER_H
#define MY_PROJECTS_BIGINTEGER_H

#include <cstdlib>
#include <cstring>
#include <ostream>
#include <iostream>
#include <stdexcept>

#define BASE 3
#define MAX_VALUE 1000

class BigInteger {
private:
  int *values;
  size_t capacity;
  size_t length;
  BigInteger& set_capacity(size_t new_size);
  void normal_type();
  int sign;
public:
  BigInteger(size_t len = 0);
  BigInteger(int);
  BigInteger(unsigned int);
  BigInteger(long long int);
  BigInteger(const BigInteger&);
  BigInteger(const char *s);
  ~BigInteger();

  friend std::ostream &operator<<(std::ostream& os, const BigInteger&);
  friend std::istream &operator>>(std::istream& is, BigInteger&);
  BigInteger& operator=(const char *);
  BigInteger& operator=(const BigInteger&);

  BigInteger operator+(const BigInteger&);
  BigInteger plus(const BigInteger&);
  BigInteger minus(const BigInteger&);
  BigInteger operator-(const BigInteger&);
  BigInteger operator*(const BigInteger&) const;
  BigInteger operator/(const BigInteger&);
  BigInteger operator%(const BigInteger&);
  BigInteger& operator+=(const BigInteger&);
  BigInteger& operator-=(const BigInteger&);
  BigInteger& operator*=(const BigInteger&);
  BigInteger& operator/=(const BigInteger&);
  BigInteger& operator++();
  BigInteger operator++(int);
  bool operator<(const BigInteger&);
  bool operator==(const BigInteger&) const;
  bool operator>(const BigInteger&);
  bool operator>=(const BigInteger&);

  friend BigInteger sqrt(const BigInteger&);
  BigInteger nod(const BigInteger&);
};

class MyExcept: std::logic_error {
public:
  MyExcept() : std::logic_error("Error") {};
};


#endif //MY_PROJECTS_BIGINTEGER_H
