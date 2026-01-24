#include "Integer.h"
Integer& Integer::operator++() {
    ++m_val;
    return *this;
}
Integer Integer::operator++(int) {
    Integer temp = *this;
    ++(*this);
    return temp;
}
Integer& Integer::operator--() {
    --m_val;
    return *this;
}
Integer Integer::operator--(int) {
    Integer temp = *this;
    --(*this);
    return temp;
}
Integer Integer::operator+(const Integer& that) const { return Integer(m_val + that.m_val); }
Integer Integer::operator-(const Integer& that) const { return Integer(m_val - that.m_val); }
Integer Integer::operator*(const Integer& that) const { return Integer(m_val * that.m_val); }
Integer Integer::operator/(const Integer& that) const {

    if (that.m_val == 0) {
        std::cerr << "Error: Division by zero!" << std::endl;
        return Integer(0);
    }
    return Integer(m_val / that.m_val);
}
Integer Integer::operator%(const Integer& that) const {

    if (that.m_val == 0) {
        std::cerr << "Error: Modulus by zero!" << std::endl;
        return Integer(0);
    }
    return Integer(m_val % that.m_val);
}
Integer& Integer::operator+=(const Integer& that) {
    m_val += that.m_val;
    return *this;
}
Integer& Integer::operator-=(const Integer& that) {
    m_val -= that.m_val;
    return *this;
}
Integer& Integer::operator*=(const Integer& that) {
    m_val *= that.m_val;
    return *this;
}
Integer& Integer::operator/=(const Integer& that) {
    if (that.m_val != 0) {
        m_val /= that.m_val;
    } else {
        std::cerr << "Error: Division by zero in /=!" << std::endl;
    }
    return *this;
}
Integer& Integer::operator%=(const Integer& that) {
    if (that.m_val != 0) {
        m_val %= that.m_val;
    } else {
        std::cerr << "Error: Modulus by zero in %=!" << std::endl;
    }
    return *this;
}
Integer& Integer::twice() {
    m_val *= 2;
    return *this;
}
std::ostream& operator<<(std::ostream& out, const Integer& i) {
    out << i.m_val;
    return out;
}
bool operator==(const Integer& left, const Integer& right) { return left.m_val == right.m_val; }
bool operator!=(const Integer& left, const Integer& right) { return left.m_val != right.m_val; }
bool operator<(const Integer& left, const Integer& right) { return left.m_val < right.m_val; }
bool operator>(const Integer& left, const Integer& right) { return left.m_val > right.m_val; }
bool operator<=(const Integer& left, const Integer& right) { return !(left > right); }
bool operator>=(const Integer& left, const Integer& right) { return !(left < right); }