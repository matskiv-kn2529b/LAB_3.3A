#include "LongLong.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
constexpr std::int64_t TWO32 = 4294967296LL;
constexpr std::int64_t MIN32 = -2147483648LL;
constexpr std::int64_t MAX32 = 2147483647LL;
static_assert(std::numeric_limits<long>::digits >= 31, "long needs at least 32 bits");

// Читаємо цілий токен: "123abc", "1.5", "+" і переповнення відхиляються.
bool parseInteger(const std::string& token, std::int64_t& result) {
    std::size_t pos = 0;
    bool negative = false;
    if (!token.empty() && (token[0] == '-' || token[0] == '+')) {
        negative = token[0] == '-';
        ++pos;
    }
    if (pos == token.size()) return false;
    const std::uint64_t positiveMax =
        static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max());
    const std::uint64_t limit = negative ? positiveMax + 1 : positiveMax;
    std::uint64_t magnitude = 0;
    for (; pos < token.size(); ++pos) {
        const char ch = token[pos];
        if (ch < '0' || ch > '9') return false;
        const unsigned digit = static_cast<unsigned>(ch - '0');
        if (magnitude > (limit - digit) / 10) return false;
        magnitude = magnitude * 10 + digit;
    }
    if (negative && magnitude == positiveMax + 1)
        result = std::numeric_limits<std::int64_t>::min();
    else {
        result = static_cast<std::int64_t>(magnitude);
        if (negative) result = -result;
    }
    return true;
}
}

LongLong::LongLong() : high(0), low(0) {}
LongLong::LongLong(long h, long l) : high(0), low(0) { Init(h, l); }
LongLong::LongLong(const LongLong& r) : high(r.high), low(r.low) {}
LongLong::~LongLong() {}

long LongLong::getHigh() const { return high; }
long LongLong::getLow() const { return low; }

void LongLong::setHigh(long h) {
    if (h < MIN32 || h > MAX32)
        throw std::out_of_range("Старша частина має бути знаковим 32-бітним числом.");
    high = h;
}
void LongLong::setLow(long l) {
    // На Windows long є 32-бітним: для бітів 0xFFFFFFFF передаємо -1.
    // На платформах з 64-бітним long приймаємо також 0..4294967295.
    const std::int64_t value = static_cast<std::int64_t>(l);
    if (value < MIN32 || value > TWO32 - 1)
        throw std::out_of_range("Молодша частина виходить за межі 32 бітів.");
    const std::uint32_t bits = static_cast<std::uint32_t>(l);
    const std::int64_t signedValue = bits > static_cast<std::uint32_t>(MAX32)
        ? static_cast<std::int64_t>(bits) - TWO32 : bits;
    low = static_cast<long>(signedValue);
}
void LongLong::Init(long h, long l) {
    // Перевіряємо обидва аргументи до зміни поточного об'єкта.
    LongLong checked;
    checked.setHigh(h);
    checked.setLow(l);
    high = checked.high;
    low = checked.low;
}
void LongLong::Read() {
    if (!(std::cin >> *this))
        throw std::invalid_argument("Очікується ціле число в межах int64.");
}
void LongLong::Display() const { std::cout << *this << '\n'; }

std::int64_t LongLong::toInt64() const {
    return static_cast<std::int64_t>(high) * TWO32
        + static_cast<std::uint32_t>(low);
}
LongLong LongLong::fromInt64(std::int64_t value) {
    std::int64_t h = value / TWO32;
    std::int64_t l = value % TWO32;
    if (l < 0) { l += TWO32; --h; }
    if (l > MAX32) l -= TWO32;
    return LongLong(static_cast<long>(h), static_cast<long>(l));
}
LongLong LongLong::subtract(const LongLong& b) const {
    const std::int64_t x = toInt64(), y = b.toInt64();
    const auto min = std::numeric_limits<std::int64_t>::min();
    const auto max = std::numeric_limits<std::int64_t>::max();
    if ((y > 0 && x < min + y) || (y < 0 && x > max + y))
        throw std::overflow_error("Переповнення при відніманні.");
    return fromInt64(x - y);
}
LongLong LongLong::divide(const LongLong& b) const {
    const auto x = toInt64(), y = b.toInt64();
    if (y == 0) throw std::domain_error("Ділення на нуль.");
    if (x == std::numeric_limits<std::int64_t>::min() && y == -1)
        throw std::overflow_error("Переповнення при діленні.");
    return fromInt64(x / y);
}
bool LongLong::notGreater(const LongLong& b) const { return toInt64() <= b.toInt64(); }
bool LongLong::equal(const LongLong& b) const { return toInt64() == b.toInt64(); }
bool LongLong::notEqual(const LongLong& b) const { return !equal(b); }
void LongLong::increment() {
    const auto value = toInt64();
    if (value == std::numeric_limits<std::int64_t>::max())
        throw std::overflow_error("Переповнення при збільшенні.");
    const LongLong next = fromInt64(value + 1);
    high = next.high; low = next.low;
}
void LongLong::decrement() {
    const auto value = toInt64();
    if (value == std::numeric_limits<std::int64_t>::min())
        throw std::overflow_error("Переповнення при зменшенні.");
    const LongLong next = fromInt64(value - 1);
    high = next.high; low = next.low;
}
std::string LongLong::toString() const {
    std::ostringstream out;
    out << toInt64() << " (high = " << high
        << ", low bits = " << static_cast<std::uint32_t>(low) << ')';
    return out.str();
}
std::ostream& operator<<(std::ostream& out, const LongLong& value) {
    return out << value.toString();
}
std::istream& operator>>(std::istream& in, LongLong& value) {
    std::string token;
    if (!(in >> token)) return in;
    std::int64_t parsed = 0;
    if (!parseInteger(token, parsed)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    const LongLong checked = LongLong::fromInt64(parsed);
    value.high = checked.high;
    value.low = checked.low;
    return in;
}
