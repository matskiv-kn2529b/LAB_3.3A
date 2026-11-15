//////////////////////////////////////////////////////////////////////////////
// PublicLongLong.h
// заголовний файл – ВІДКРИТИЙ нащадок класу LongLong
#pragma once
#include "LongLong.h"
using std::string;
using std::ostream;
using std::istream;

#ifdef LAB_PACKED
#pragma pack(push, 1)
#endif

// Відкрите успадкування = успадкування ІНТЕРФЕЙСУ: PublicLongLong «є» LongLong,
// тому його можна підставляти замість базового класу, а відкриті методи
// бази доступні; зовнішні оператори введення-виведення працюють через
// доступне перетворення до базового класу.
// Саме тому функції вводу-виводу для цього класу НЕ визначаються (вимога
// завдання) – використовуються операції << та >> базового класу.
class PublicLongLong : public LongLong
{
public:
    // ---- конструктори (визначаємо власні конструктори) ----
    PublicLongLong();                               // за умовчанням
    PublicLongLong(long, long);                     // ініціалізації
    explicit PublicLongLong(const LongLong&);       // з об'єкта базового класу
    PublicLongLong(const PublicLongLong&);          // копіювання

    // ---- перевантажені операції (використовують методи базового класу) ----
    PublicLongLong& operator = (const PublicLongLong&);
    operator string() const;                        // приведення типу до рядка

    PublicLongLong& operator ++ ();                 // ++ та -- змінюють число на 1;
    PublicLongLong& operator -- ();                 // префіксні повертають нове значення,
    PublicLongLong  operator ++ (int);              // постфіксні – старе
    PublicLongLong  operator -- (int);

    friend PublicLongLong operator - (const PublicLongLong&, const PublicLongLong&);
    friend PublicLongLong operator / (const PublicLongLong&, const PublicLongLong&);
    friend bool operator <= (const PublicLongLong&, const PublicLongLong&);   // не більше
    friend bool operator == (const PublicLongLong&, const PublicLongLong&);
    friend bool operator != (const PublicLongLong&, const PublicLongLong&);
};
#ifdef LAB_PACKED
#pragma pack(pop)
#endif
