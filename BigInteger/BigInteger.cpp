//
// Created by sikalovnikita on 12.04.18.
//

#include "BigInteger.h"
#include <cstdlib>


BigInteger::BigInteger(const char *s) {
  if(s[0] == '-') {
    this->sign = -1;
    s += 1;
  } else {
    this->sign = 1;
  }
  size_t len_s = strlen(s);
  this->length = (len_s + BASE - 1) / BASE;
  size_t len = this->length;
  this->capacity = 2 * len;
  this->values = new int[this->capacity];
  int index = 0;
  if (len_s % BASE != 0) {
    char* temp = new char[len_s % BASE + 1];
    for(int i = 0; i < (int)len_s % BASE; ++i) {
      temp[i] = s[i];
    }
    temp[len_s % BASE] = '\0';
    this->values[index] = atoi(temp);
    delete []temp;
    index++;
  }
  for (int i = 0; i < (int)len_s; i += BASE, ++index) {
    char *temp = new char[BASE];
    std::memcpy(temp, s + i, BASE);
    this->values[index] = atoi(temp);
    delete []temp;
  }
}

BigInteger::BigInteger(const BigInteger &a) {
  this->length = a.length;
  this->capacity = a.length * 2;
  this->values = new int[a.length * 2];
  *this = a;
}

BigInteger::BigInteger(size_t len) {
  this->capacity = len;
  this->length = 0;
  this->values = new int[len];
  this->sign = 1;
}

BigInteger::BigInteger(int value) {
  this->capacity = 1;
  this->values = new int[1];
  this->values[0] = value;
  this->length = 1;
  this->sign = value < 0 ? -1 : 1;
  this->normal_type();
}

BigInteger::BigInteger(unsigned int value) {
  this->capacity = 1;
  this->values = new int[1];
  this->length = 0;
  char* s = new char[100];
  itoa(value, s, 10);
  *this = s;
  delete []s;
}

BigInteger::BigInteger(long long int value) {
  this->capacity = 1;
  this->values = new int[1];
  this->length = 0;
  char* s = new char[100];
  _i64toa(value, s, 10);
  *this = s;
  delete []s;
}


BigInteger::~BigInteger() {
  delete []this->values;
}

std::ostream& operator<<(std::ostream &os, const BigInteger& a) {
  if(a.sign == -1) {
    os << '-';
  }
  os << a.values[0];
  for (int i = 1; i < a.length; ++i) {
    int temp = MAX_VALUE / 10;
    while(a.values[i] < temp && temp > 1) {
      os << '0';
      temp /= 10;
    }
    os << a.values[i];
  }
  return os;
}

std::istream &operator>>(std::istream &is, BigInteger &a) {
  char *temp = new char[10005];
  is >> temp;
  a = temp;
  delete []temp;
  return is;
}

BigInteger& BigInteger::set_capacity(size_t new_size) {
  size_t old_size = this->length;
  int *temp = new int[old_size];
  size_t byte_size = old_size * sizeof(int);
  std::memcpy(temp, this->values, byte_size);
  delete []this->values;
  this->values = new int[new_size];
  std::memcpy(this->values, temp, byte_size);
  this->capacity = new_size;
  return (*this);
}

BigInteger& BigInteger::operator=(const char *s) {
  BigInteger temp(s);
  *this = temp;
  return (*this);
}

BigInteger& BigInteger::operator=(const BigInteger &that) {
  delete []this->values;
  this->values = new int[that.capacity];
  this->capacity = that.length;
  std::memcpy(this->values, that.values, sizeof(int) * that.length);
  this->length = that.length;
  this->sign = that.sign;
  return (*this);
}

bool BigInteger::operator<(const BigInteger& that) {
  if(this->length > that.length) {
    return false;
  }
  if(this->length < that.length) {
    return true;
  }
  int i = 0;
  while(i < this->length && this->values[i] == that.values[i]) {
    i++;
  }
  return !(i == (int)this->length || that.values[i] <= this->values[i]);
}

bool BigInteger::operator==(const BigInteger &that) const{
  if(this->length != that.length)
    return false;
  int i = 0;
  while(i < this->length && this->values[i] == that.values[i]) {
    i++;
  }
  return i == this->length;
}

bool BigInteger::operator>(const BigInteger &that) {
  return !(*this < that || *this == that);
}

bool BigInteger::operator>=(const BigInteger &that) {
  return (*this > that || *this == that);
}

BigInteger BigInteger::minus(const BigInteger& that) {
  BigInteger a = *this < that ? that : *this;
  BigInteger b = *this < that ? *this : that;
  BigInteger result(2 * a.length);
  size_t ind_a = a.length - 1;
  size_t ind_b = b.length - 1;
  size_t current = a.length - 1;
  for (size_t i = 0; i < b.length; ++i, --ind_a, --ind_b, --current) {
    result.values[current] = a.values[ind_a] - b.values[ind_b];
    result.length++;
  }
  for (size_t i = b.length; i < a.length; ++i, --ind_a, --ind_b, --current) {
    result.values[current] = a.values[ind_a];
    result.length++;
  }
  result.normal_type();
  return result;
}

BigInteger BigInteger::plus(const BigInteger& that) {
  size_t max_len = std::max(this->length, that.length);
  size_t min_len = std::min(this->length, that.length);
  BigInteger result(2 * max_len);
  size_t ind_this = this->length - 1;
  size_t ind_that = that.length - 1;
  size_t current = max_len - 1;
  for (size_t i = 0; i < min_len; ++i, --ind_this, --ind_that, --current) {
    result.values[current] = this->values[ind_this] + that.values[ind_that];
    result.length++;
  }
  for (size_t i = min_len; i < max_len; ++i, --ind_this, --ind_that, --current) {
    result.values[current] = (this->length > that.length ? this->values[ind_this] : that.values[ind_that]);
    result.length++;
  }
  result.normal_type();
  return result;
}

BigInteger BigInteger::operator+(const BigInteger &that) {
  if(this->sign * that.sign == 1) {
    BigInteger result = this->plus(that);
    result.sign = this->sign;
    return result;
  }
  BigInteger result = this->minus(that);
  if(*this > that) {
    result.sign = (this->sign) == -1 ? -1 : 1;
  } else {
    result.sign = (that.sign) == -1 ? -1 : 1;
  }
  return result;
}

BigInteger BigInteger::operator-(const BigInteger &that) {
  BigInteger temp_that = that;
  temp_that.sign *= -1;
  return *this + temp_that;
}

BigInteger BigInteger::operator* (const BigInteger& mul2) const{
  BigInteger mul1 = *this;
  BigInteger a = mul1.length > mul2.length ? mul1 : mul2;
  BigInteger b = mul1.length > mul2.length ? mul2 : mul1;
  BigInteger result = 0;
  result.sign = mul1.sign * mul2.sign;
  if (b.values[0] != 0) {
    BigInteger temp = a;
    result.set_capacity(a.capacity+ b.capacity + 100);
    a.set_capacity(a.capacity + b.capacity + 100);
    temp.set_capacity(a.capacity + b.capacity + 100);
    for (int i = (int)b.length - 1; i >= 0; --i) {
      for (int j = (int)temp.length - 1; j >= 0; --j) {
        temp.values[j] *= b.values[i];
      }
      temp.normal_type();
      result = result + temp;
      result.normal_type();
      a.length++;
      a.values[a.length - 1] = 0;
      temp = a;
    }
  }
  return result;
}

BigInteger BigInteger::operator/(const BigInteger& that) {
  BigInteger result = 0;
  if(that == 0) {
    MyExcept ex;
    throw ex;
  }
  result.sign = this->sign * that.sign;
  if (*this < that) {
    return result;
  } else {
    result.set_capacity(this->length);
    int current = 0;
    BigInteger temp = this->values[current];
    temp.set_capacity(2 * this->length);
    while (temp < that) {
      current++;
      temp.values[current] = this->values[current];
      temp.length++;
    }
    while (current < this->length && (temp > that || temp == that)) {
      int j = 1;
      while (temp >= that * j) {
        j++;
      }
      j--;
      BigInteger j_temp = j;
      j_temp.set_capacity(2 * this->length);
      for (int k = 0; k < this->length - current - 1; ++k) {
        j_temp.values[j_temp.length] = 0;
        j_temp.length++;
      }
      result = result + j_temp;
      temp = temp - that * j;
      while (current < (int) this->length - 1 && this->values[current + 1] == 0) {
        result.values[result.length] = 0;
        result.length++;
        current++;
      }
      while (current < this->length - 1 && temp < that) {
        current++;
        temp.values[temp.length] = this->values[current];
        temp.length++;
        temp.normal_type();
      }
    }
    return result;
  }
}

BigInteger BigInteger::operator%(const BigInteger &that) {
  BigInteger temp = *this / that;
  return *this - temp * that;
}


BigInteger& BigInteger::operator+=(const BigInteger &that) {
  *this = *this + that;
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger &that) {
  *this = *this - that;
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger &that) {
  *this = *this * that;
  return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger &that) {
  *this = *this / that;
  return *this;
}

BigInteger& BigInteger::operator++() {
  *this = *this + 1;
  return *this;
}

BigInteger BigInteger::operator++(int) {
  BigInteger old = *this;
  ++(*this);
  return old;
}


BigInteger BigInteger::nod(const BigInteger& that) {
  BigInteger a = *this < that ? *this : that;
  BigInteger b = *this < that ? that : *this;
  while(!(a == 0)) {
    BigInteger temp = a;
    a = b % a;
    b = temp;
  }
  return b;
}

void BigInteger::normal_type() {
  for (size_t i = this->length - 1; i > 0; --i) {
    if(this->values[i] >= MAX_VALUE) {
      this->values[i - 1] += this->values[i] / MAX_VALUE;
      this->values[i] %= MAX_VALUE;
    }
    if(this->values[i] < 0) {
      int temp = -this->values[i] / MAX_VALUE;
      temp += this->values[i] % MAX_VALUE == 0 ? 0 : 1;
      this->values[i] += temp * MAX_VALUE ;
      this->values[i - 1] -= temp;
    }
  }
  int j = 0;
  while(j < (int)this->length - 1 && this->values[j] == 0) {
    j++;
  }
  if(j != 0) {
    std::memcpy(this->values, this->values + j, sizeof(int) * this->length);
    this->length -= j;
  }
  if(this->values[0] >= MAX_VALUE) {
    int temp = MAX_VALUE;
    int add_size = 0;
    while(this->values[0] >= temp) {
      add_size++;
      temp *= MAX_VALUE;
    }
    std::memcpy(this->values + add_size, this->values, this->length * sizeof(int));
    this->length += add_size;
    for (int i = add_size - 1; i >= 0; --i) {
      this->values[i] = 0;
      int x = this->values[i + 1] % MAX_VALUE;
      this->values[i] += this->values[i + 1] / MAX_VALUE;
      this->values[i + 1] = x;
    }
  }
}

BigInteger sqrt(const BigInteger& a) {
  BigInteger first = 1;
  BigInteger last = a;
  BigInteger mid = (first + last) / 2;
  while(!(first == mid)) {
    if(mid * mid > a) {
      last = mid;
    } else {
      first = mid;
    }
    mid = (first + last) / 2;
  }
  return first;
}



