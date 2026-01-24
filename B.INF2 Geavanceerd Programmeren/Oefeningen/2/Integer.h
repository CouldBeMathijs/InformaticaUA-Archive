#ifndef OEFENINGEN_INTEGER_H
#define OEFENINGEN_INTEGER_H
#include <iostream>

class Integer {
private:
    int m_val;

public:
    Integer() : m_val(0) {}
    Integer(const Integer& that) = default;
    Integer& operator=(const Integer& that) = default;
    explicit Integer(int val) : m_val(val) {}
    ~Integer() = default;
    [[nodiscard]] int getValue() const { return m_val; }
    Integer operator+() const { return Integer(std::abs(m_val)); }
    Integer operator-() const { return Integer(-m_val); }
    Integer& operator++();
    Integer operator++(int);
    Integer& operator--();
    Integer operator--(int);
    Integer operator+(const Integer& that) const;
    Integer operator-(const Integer& that) const;
    Integer operator*(const Integer& that) const;

    Integer operator/(const Integer& that) const;

    Integer operator%(const Integer& that) const;

    Integer& operator+=(const Integer& that);

    Integer& operator-=(const Integer& that);

    Integer& operator*=(const Integer& that);

    Integer& operator/=(const Integer& that);

    Integer& operator%=(const Integer& that);
    Integer& twice();

    friend bool operator==(const Integer& left, const Integer& right);
    friend bool operator!=(const Integer& left, const Integer& right);
    friend bool operator<(const Integer& left, const Integer& right);
    friend bool operator>(const Integer& left, const Integer& right);
    friend bool operator<=(const Integer& left, const Integer& right);
    friend bool operator>=(const Integer& left, const Integer& right);
    friend std::ostream& operator<<(std::ostream& out, const Integer& i);
};

std::ostream& operator<<(std::ostream& out, const Integer& i);

bool operator==(const Integer& left, const Integer& right);

bool operator!=(const Integer& left, const Integer& right);

bool operator<(const Integer& left, const Integer& right);

bool operator>(const Integer& left, const Integer& right);

bool operator<=(const Integer& left, const Integer& right);

bool operator>=(const Integer& left, const Integer& right);

#endif
