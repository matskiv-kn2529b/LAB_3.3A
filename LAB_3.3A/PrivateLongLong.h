//////////////////////////////////////////////////////////////////////////////
// PrivateLongLong.h
// заголовний файл – ЗАКРИТИЙ нащадок класу LongLong
#pragma once
#include "LongLong.h"
using std::string;
using std::ostream;
using std::istream;

#ifdef LAB_PACKED
#pragma pack(push, 1)
#endif

// Закрите успадкування = успадкування РЕАЛІЗАЦІЇ: PrivateLongLong НЕ «є»
// LongLong. Увесь відкритий інтерфейс бази стає закритим, перетворення
// PrivateLongLong -> LongLong ззовні заборонене, тому:
//   * користуватись методами бази можна лише всередині класу;
//   * потрібне з бази відкриваємо директивою доступу (using);
//   * операції вводу-виводу базового класу ззовні не працюють, тому їх
//     потрібно визначити для нащадка (вони використовують операції бази).
class PrivateLongLong : private LongLong
{
public:
    // ---- директиви доступу: відкриваємо потрібні методи доступу та службові методи ----
    using LongLong::getHigh;
    using LongLong::getLow;
    using LongLong::setHigh;
    using LongLong::setLow;
    using LongLong::Init;
    using LongLong::Read;
    using LongLong::Display;
    using LongLong::toString;

    // ---- конструктори ----
    PrivateLongLong();
    PrivateLongLong(long, long);
    explicit PrivateLongLong(const LongLong&);      // з об'єкта базового класу
    PrivateLongLong(const PrivateLongLong&);

    // ---- перевантажені операції ----
    PrivateLongLong& operator = (const PrivateLongLong&);
    operator string() const;

    PrivateLongLong& operator ++ ();
    PrivateLongLong& operator -- ();
    PrivateLongLong  operator ++ (int);
    PrivateLongLong  operator -- (int);

    friend PrivateLongLong operator - (const PrivateLongLong&, const PrivateLongLong&);
    friend PrivateLongLong operator / (const PrivateLongLong&, const PrivateLongLong&);
    friend bool operator <= (const PrivateLongLong&, const PrivateLongLong&);
    friend bool operator == (const PrivateLongLong&, const PrivateLongLong&);
    friend bool operator != (const PrivateLongLong&, const PrivateLongLong&);

    // ---- ввід-вивід ЗАДАЄТЬСЯ для нащадка (на відміну від відкритого) ----
    friend ostream& operator << (ostream&, const PrivateLongLong&);
    friend istream& operator >> (istream&, PrivateLongLong&);
};
#ifdef LAB_PACKED
#pragma pack(pop)
#endif
