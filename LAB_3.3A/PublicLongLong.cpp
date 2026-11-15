//////////////////////////////////////////////////////////////////////////////
// PublicLongLong.cpp
// файл реалізації – реалізація методів відкритого нащадка
#include "PublicLongLong.h"
using namespace std;

// ---------------------------------------------------------------------------
//                            конструктори
// ---------------------------------------------------------------------------

// Кожен конструктор нащадка спершу викликає конструктор бази
PublicLongLong::PublicLongLong() : LongLong()
{ }

PublicLongLong::PublicLongLong(long h, long l) : LongLong(h, l)
{ }

PublicLongLong::PublicLongLong(const LongLong& r) : LongLong(r)
{ }

PublicLongLong::PublicLongLong(const PublicLongLong& r) : LongLong(r)
{ }

// ---------------------------------------------------------------------------
//                       перевантажені операції
// ---------------------------------------------------------------------------

PublicLongLong& PublicLongLong::operator = (const PublicLongLong& r)
{
    LongLong::operator = (r);                   // присвоєння базової частини
    return *this;
}

PublicLongLong::operator string() const
{
    return toString();                          // метод базового класу
}

PublicLongLong& PublicLongLong::operator ++ ()
{
    increment();                                // метод базового класу
    return *this;
}

PublicLongLong& PublicLongLong::operator -- ()
{
    decrement();
    return *this;
}

PublicLongLong PublicLongLong::operator ++ (int)
{
    PublicLongLong t(*this);                    // запам'ятовуємо старе значення
    increment();
    return t;
}

PublicLongLong PublicLongLong::operator -- (int)
{
    PublicLongLong t(*this);
    decrement();
    return t;
}

PublicLongLong operator - (const PublicLongLong& a, const PublicLongLong& b)
{
    return PublicLongLong(a.subtract(b));       // subtract() повертає LongLong
}

PublicLongLong operator / (const PublicLongLong& a, const PublicLongLong& b)
{
    return PublicLongLong(a.divide(b));
}

bool operator <= (const PublicLongLong& a, const PublicLongLong& b)
{
    return a.notGreater(b);
}

bool operator == (const PublicLongLong& a, const PublicLongLong& b)
{
    return a.equal(b);
}

bool operator != (const PublicLongLong& a, const PublicLongLong& b)
{
    return a.notEqual(b);
}