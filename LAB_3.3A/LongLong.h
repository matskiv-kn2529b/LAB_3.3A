#pragma once
#include <cstdint>
#include <iosfwd>
#include <string>

#ifdef LAB_PACKED
#pragma pack(push, 1)
#endif
// high — знакова старша частина; low — знакове long, що зберігає 32 біти.
// Значення = high * 2^32 + uint32_t(low).
class LongLong {
    long high;
    long low;
    std::int64_t toInt64() const;
    static LongLong fromInt64(std::int64_t);
public:
    LongLong();
    LongLong(long, long);
    LongLong(const LongLong&);
    ~LongLong();
    // Оператор присвоєння бази не оголошуємо: використовується неявний.
    long getHigh() const;
    long getLow() const;
    void setHigh(long);
    void setLow(long);
    void Init(long, long);
    void Read();
    void Display() const;
    LongLong subtract(const LongLong&) const;
    LongLong divide(const LongLong&) const;
    bool notGreater(const LongLong&) const;
    bool equal(const LongLong&) const;
    bool notEqual(const LongLong&) const;
    void increment();
    void decrement();
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream&, const LongLong&);
    friend std::istream& operator>>(std::istream&, LongLong&);
};
#ifdef LAB_PACKED
#pragma pack(pop)
#endif
