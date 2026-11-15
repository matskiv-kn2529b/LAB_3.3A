//////////////////////////////////////////////////////////////////////////////
// PrivateLongLong.cpp
// файл реалізації – реалізація методів закритого нащадка
#include "PrivateLongLong.h"
using namespace std;

PrivateLongLong::PrivateLongLong() : LongLong()
{ }

PrivateLongLong::PrivateLongLong(long h, long l) : LongLong(h, l)
{ }

PrivateLongLong::PrivateLongLong(const LongLong& r) : LongLong(r)
{ }

PrivateLongLong::PrivateLongLong(const PrivateLongLong& r) : LongLong(r)
{ }

PrivateLongLong& PrivateLongLong::operator = (const PrivateLongLong& r)
{
    LongLong::operator = (r);
    return *this;
}

PrivateLongLong::operator string() const
{
    return toString();                          // тут (всередині класу) база доступна
}

PrivateLongLong& PrivateLongLong::operator ++ ()
{
    increment();
    return *this;
}

PrivateLongLong& PrivateLongLong::operator -- ()
{
    decrement();
    return *this;
}

PrivateLongLong PrivateLongLong::operator ++ (int)
{
    PrivateLongLong t(*this);
    increment();
    return t;
}

PrivateLongLong PrivateLongLong::operator -- (int)
{
    PrivateLongLong t(*this);
    decrement();
    return t;
}

// Дружні функції нащадка мають такий самий доступ до бази, що й його методи
PrivateLongLong operator - (const PrivateLongLong& a, const PrivateLongLong& b)
{
    return PrivateLongLong(a.subtract(b));
}

PrivateLongLong operator / (const PrivateLongLong& a, const PrivateLongLong& b)
{
    return PrivateLongLong(a.divide(b));
}

bool operator <= (const PrivateLongLong& a, const PrivateLongLong& b)
{
    return a.notGreater(b);
}

bool operator == (const PrivateLongLong& a, const PrivateLongLong& b)
{
    return a.equal(b);
}

bool operator != (const PrivateLongLong& a, const PrivateLongLong& b)
{
    return a.notEqual(b);
}

// Ввід-вивід нащадка спираються на операції бази: явно «піднімаємося» до
// LongLong (це дозволено лише всередині нащадка та його друзів)
ostream& operator << (ostream& out, const PrivateLongLong& r)
{
    return out << static_cast<const LongLong&>(r);
}

istream& operator >> (istream& in, PrivateLongLong& r)
{
    return in >> static_cast<LongLong&>(r);
}