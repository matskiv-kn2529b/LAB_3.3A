#include "pch.h"
#include "CppUnitTest.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../LAB_3.3A/LongLong.cpp"
#include "../LAB_3.3A/PublicLongLong.cpp"
#include "../LAB_3.3A/PrivateLongLong.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab33A
{
    static_assert(std::is_convertible<PublicLongLong*, LongLong*>::value,
        "PublicLongLong must allow conversion to LongLong");
    static_assert(!std::is_convertible<PrivateLongLong*, LongLong*>::value,
        "PrivateLongLong must not allow external conversion to LongLong");
    static_assert(std::is_base_of<LongLong, PrivateLongLong>::value,
        "PrivateLongLong still has LongLong as its base");

    // Незалежне від toString відновлення числового значення.
    template<class T>
    static std::int64_t value(const T& x)
    {
        return static_cast<std::int64_t>(x.getHigh()) * 4294967296LL
            + static_cast<std::uint32_t>(x.getLow());
    }

    // Коректне розбиття, зокрема для від'ємних значень та INT64_MIN.
    template<class T>
    static T makeValue(std::int64_t x)
    {
        std::int64_t high = x / 4294967296LL;
        std::int64_t low = x % 4294967296LL;
        if (low < 0) { low += 4294967296LL; --high; }
        if (low > 2147483647LL) low -= 4294967296LL;
        return T(static_cast<long>(high), static_cast<long>(low));
    }

    template<class T>
    static void verifyValidInput()
    {
        for (const auto number : {
            std::int64_t(0), std::int64_t(-1), std::int64_t(5000000000LL),
            std::numeric_limits<std::int64_t>::min(),
            std::numeric_limits<std::int64_t>::max() })
        {
            T x;
            std::istringstream input(std::to_string(number));
            input >> x;
            Assert::IsFalse(input.fail());
            Assert::IsTrue(value(x) == number);
        }
        T x;
        std::istringstream input("  +123 -456");
        input >> x;
        Assert::IsTrue(value(x) == 123);
        input >> x;
        Assert::IsTrue(value(x) == -456);
    }

    template<class T>
    static void verifyInvalidInput()
    {
        for (const char* text : {
            "abc", "24abc", "1.5", "+", "--1", "",
            "9223372036854775808", "-9223372036854775809" })
        {
            T x = makeValue<T>(42);
            std::istringstream input(text);
            input >> x;
            Assert::IsTrue(input.fail());
            Assert::IsTrue(value(x) == 42); // Об'єкт після помилки не змінюється.
        }
    }

    template<class T>
    static void verifyCopyAndAssignment()
    {
        T zero;
        Assert::IsTrue(value(zero) == 0);
        T a(1, 705032704);
        T copy(a);
        T assigned;
        assigned = a;
        const T& same = assigned;
        assigned = same; // Самоприсвоєння.
        Assert::IsTrue(value(copy) == 5000000000LL);
        Assert::IsTrue(value(assigned) == 5000000000LL);
        copy.setLow(7);
        Assert::IsTrue(value(a) == 5000000000LL);
    }

    template<class T>
    static void verifyAccessAndInit()
    {
        T x;
        x.Init(1, 705032704);
        Assert::IsTrue(x.getHigh() == 1);
        Assert::IsTrue(x.getLow() == 705032704);
        x.setHigh(-1);
        x.setLow(-1);
        Assert::IsTrue(value(x) == -1);
        x.setHigh(0);
        Assert::IsTrue(value(x) == 4294967295LL);
    }

    // Відновлює буфери та стани cin/cout навіть у разі винятку.
    class ConsoleRedirect
    {
        std::streambuf* oldInput;
        std::streambuf* oldOutput;
        std::ios::iostate oldState;
    public:
        ConsoleRedirect(std::istream& input, std::ostream& output)
            : oldInput(std::cin.rdbuf()), oldOutput(std::cout.rdbuf()),
              oldState(std::cin.rdstate())
        {
            std::cin.clear();
            std::cin.rdbuf(input.rdbuf());
            std::cout.rdbuf(output.rdbuf());
        }
        ~ConsoleRedirect()
        {
            std::cin.rdbuf(oldInput);
            std::cin.clear(oldState);
            std::cout.rdbuf(oldOutput);
        }
        ConsoleRedirect(const ConsoleRedirect&) = delete;
        ConsoleRedirect& operator=(const ConsoleRedirect&) = delete;
    };

    template<class T>
    static void verifyReadDisplay()
    {
        T x;
        std::istringstream input("5000000000");
        std::ostringstream output;
        {
            ConsoleRedirect redirect(input, output);
            x.Read();
            x.Display();
        }
        Assert::IsTrue(value(x) == 5000000000LL);
        Assert::IsTrue(output.str() == x.toString() + "\n");
    }

    template<class T>
    static void verifyDerivedArithmetic()
    {
        const T a = makeValue<T>(5000000000LL);
        const T b = makeValue<T>(3);
        Assert::IsTrue(value(a - b) == 4999999997LL);
        Assert::IsTrue(value(a / b) == 1666666666LL);
        Assert::IsTrue(value(makeValue<T>(3) - makeValue<T>(10)) == -7);
        Assert::IsTrue(value(makeValue<T>(-7) / makeValue<T>(2)) == -3);
        Assert::IsTrue(value(makeValue<T>(7) / makeValue<T>(-2)) == -3);
        Assert::IsTrue(value(makeValue<T>(-7) / makeValue<T>(-2)) == 3);
        Assert::IsTrue(value(a) == 5000000000LL); // Операції без присвоєння.
        Assert::IsTrue(value(b) == 3);
    }

    template<class T>
    static void verifyDerivedComparisons()
    {
        const T a = makeValue<T>(-5), b = makeValue<T>(3), copy(a);
        Assert::IsTrue(a <= b);
        Assert::IsFalse(b <= a);
        Assert::IsTrue(a == copy);
        Assert::IsFalse(a != copy);
        Assert::IsTrue(a != b);
        Assert::IsTrue(a <= copy);
        Assert::IsTrue(makeValue<T>(2147483647LL) <= makeValue<T>(2147483648LL));
        Assert::IsTrue(makeValue<T>(4294967295LL) <= makeValue<T>(4294967296LL));
    }

    template<class T>
    static void verifyDerivedIncrementDecrement()
    {
        T x = makeValue<T>(4294967295LL);
        T old = x++;
        Assert::IsTrue(value(old) == 4294967295LL);
        Assert::IsTrue(value(x) == 4294967296LL);
        Assert::IsTrue(&(++x) == &x);
        Assert::IsTrue(value(x) == 4294967297LL);
        old = x--;
        Assert::IsTrue(value(old) == 4294967297LL);
        Assert::IsTrue(value(x) == 4294967296LL);
        Assert::IsTrue(&(--x) == &x);
        Assert::IsTrue(value(x) == 4294967295LL);
        T zero;
        --zero;
        Assert::IsTrue(value(zero) == -1);
        ++zero;
        Assert::IsTrue(value(zero) == 0);
    }

    template<class T>
    static void verifyDerivedErrors()
    {
        T a = makeValue<T>(10), zero;
        Assert::ExpectException<std::domain_error>([&] { (void)(a / zero); });
        const auto min = std::numeric_limits<std::int64_t>::min();
        const auto max = std::numeric_limits<std::int64_t>::max();
        Assert::ExpectException<std::overflow_error>([&] {
            (void)(makeValue<T>(min) / makeValue<T>(-1)); });
        Assert::ExpectException<std::overflow_error>([&] {
            (void)(makeValue<T>(min) - makeValue<T>(1)); });
        Assert::ExpectException<std::overflow_error>([&] {
            (void)(makeValue<T>(max) - makeValue<T>(-1)); });
        T high = makeValue<T>(max), low = makeValue<T>(min);
        Assert::ExpectException<std::overflow_error>([&] { ++high; });
        Assert::ExpectException<std::overflow_error>([&] { high++; });
        Assert::ExpectException<std::overflow_error>([&] { --low; });
        Assert::ExpectException<std::overflow_error>([&] { low--; });
        Assert::IsTrue(value(high) == max && value(low) == min);
    }

    template<class T>
    static void verifyStringAndOutput()
    {
        const T x = makeValue<T>(5000000000LL);
        const std::string text = x;
        Assert::IsTrue(text == x.toString());
        Assert::IsTrue(text.find("5000000000") == 0);
        std::ostringstream output;
        output << x;
        Assert::IsTrue(output.str() == text);
    }

    template<class T>
    static void verifyRandomOperations()
    {
        std::mt19937_64 generator(33021);
        std::uniform_int_distribution<std::int64_t> range(
            -1000000000000LL, 1000000000000LL);
        // Діапазон гарантує, що еталонне віднімання не переповнюється.
        for (int i = 0; i < 1000; ++i)
        {
            const auto a = range(generator), b = range(generator);
            const T x = makeValue<T>(a), y = makeValue<T>(b);
            Assert::IsTrue(value(x - y) == a - b);
            if (b != 0) Assert::IsTrue(value(x / y) == a / b);
            Assert::IsTrue((x <= y) == (a <= b));
            Assert::IsTrue((x == y) == (a == b));
            Assert::IsTrue((x != y) == (a != b));
        }
    }

    TEST_CLASS(BaseLongLongTests)
    {
    public:
        TEST_METHOD(ConstructorsCopyAndAssignment) { verifyCopyAndAssignment<LongLong>(); }
        TEST_METHOD(GettersSettersAndInit) { verifyAccessAndInit<LongLong>(); }
        TEST_METHOD(Int64Boundaries)
        {
            Assert::IsTrue(value(LongLong(-2147483647L - 1, 0)) ==
                std::numeric_limits<std::int64_t>::min());
            Assert::IsTrue(value(LongLong(2147483647L, -1)) ==
                std::numeric_limits<std::int64_t>::max());
        }
        TEST_METHOD(SubtractMethod)
        {
            LongLong a = makeValue<LongLong>(5000000000LL), b(0, 3);
            Assert::IsTrue(value(a.subtract(b)) == 4999999997LL);
            Assert::IsTrue(value(LongLong(0, 3).subtract(LongLong(0, 10))) == -7);
            Assert::IsTrue(value(a) == 5000000000LL);
        }
        TEST_METHOD(DivideMethod)
        {
            Assert::IsTrue(value(makeValue<LongLong>(-7).divide(LongLong(0, 2))) == -3);
            Assert::IsTrue(value(makeValue<LongLong>(5000000000LL).divide(LongLong(0, 3))) == 1666666666LL);
        }
        TEST_METHOD(ComparisonMethods)
        {
            const LongLong a(-1, -1), b(0, 3), copy(a);
            Assert::IsTrue(a.notGreater(b));
            Assert::IsFalse(b.notGreater(a));
            Assert::IsTrue(a.equal(copy));
            Assert::IsFalse(a.notEqual(copy));
            Assert::IsTrue(a.notEqual(b));
        }
        TEST_METHOD(IncrementDecrementMethods_CarryBorrowAndZero)
        {
            LongLong x(0, -1);
            x.increment();
            Assert::IsTrue(value(x) == 4294967296LL && x.getHigh() == 1);
            x.decrement();
            Assert::IsTrue(value(x) == 4294967295LL && x.getHigh() == 0);
            LongLong zero;
            zero.decrement(); Assert::IsTrue(value(zero) == -1);
            zero.increment(); Assert::IsTrue(value(zero) == 0);
        }
        TEST_METHOD(ArithmeticErrors_ThrowAndPreserveObjects)
        {
            const LongLong min = makeValue<LongLong>(std::numeric_limits<std::int64_t>::min());
            const LongLong max = makeValue<LongLong>(std::numeric_limits<std::int64_t>::max());
            Assert::ExpectException<std::domain_error>([] {
                (void)LongLong(0, 10).divide(LongLong()); });
            Assert::ExpectException<std::overflow_error>([&] { (void)min.divide(LongLong(-1, -1)); });
            Assert::ExpectException<std::overflow_error>([&] { (void)min.subtract(LongLong(0, 1)); });
            Assert::ExpectException<std::overflow_error>([&] { (void)max.subtract(LongLong(-1, -1)); });
            LongLong high(max), low(min);
            Assert::ExpectException<std::overflow_error>([&] { high.increment(); });
            Assert::ExpectException<std::overflow_error>([&] { low.decrement(); });
            Assert::IsTrue(value(high) == value(max) && value(low) == value(min));
        }
        TEST_METHOD(ToStringAndOutput)
        {
            LongLong x(1, 705032704);
            Assert::IsTrue(x.toString().find("5000000000") == 0);
            std::ostringstream output; output << x;
            Assert::IsTrue(output.str() == x.toString());
        }
        TEST_METHOD(Input_ValidNumbers) { verifyValidInput<LongLong>(); }
        TEST_METHOD(Input_InvalidNumbersPreserveValue) { verifyInvalidInput<LongLong>(); }
        TEST_METHOD(ReadAndDisplay) { verifyReadDisplay<LongLong>(); }
        TEST_METHOD(Read_InvalidInputThrowsAndPreservesValue)
        {
            LongLong x(0, 42);
            std::istringstream input("24abc");
            std::ostringstream output;
            {
                ConsoleRedirect redirect(input, output);
                Assert::ExpectException<std::invalid_argument>([&] { x.Read(); });
            }
            Assert::IsTrue(value(x) == 42);
        }
    };

    TEST_CLASS(PublicLongLongTests)
    {
    public:
        TEST_METHOD(ConstructorsCopyAndAssignment) { verifyCopyAndAssignment<PublicLongLong>(); }
        TEST_METHOD(ConstructorFromBase)
        {
            LongLong base(1, 705032704);
            PublicLongLong x(base);
            Assert::IsTrue(value(x) == value(base));
        }
        TEST_METHOD(InheritedGettersSettersInit) { verifyAccessAndInit<PublicLongLong>(); }
        TEST_METHOD(MinusAndDivideOperators) { verifyDerivedArithmetic<PublicLongLong>(); }
        TEST_METHOD(ComparisonOperators) { verifyDerivedComparisons<PublicLongLong>(); }
        TEST_METHOD(PrefixPostfixIncrementDecrement) { verifyDerivedIncrementDecrement<PublicLongLong>(); }
        TEST_METHOD(ArithmeticErrors) { verifyDerivedErrors<PublicLongLong>(); }
        TEST_METHOD(StringConversionAndBaseOutput) { verifyStringAndOutput<PublicLongLong>(); }
        TEST_METHOD(BaseInput_ValidNumbers) { verifyValidInput<PublicLongLong>(); }
        TEST_METHOD(BaseInput_InvalidNumbersPreserveValue) { verifyInvalidInput<PublicLongLong>(); }
        TEST_METHOD(InheritedReadAndDisplay) { verifyReadDisplay<PublicLongLong>(); }
        TEST_METHOD(PublicSubstitution_ReferencePointerAndValue)
        {
            PublicLongLong x(0, 10);
            LongLong& reference = x;
            reference.setLow(20);
            LongLong* pointer = &x;
            Assert::IsTrue(value(x) == 20 && pointer->getLow() == 20);
            LongLong sliced = x;
            sliced.setLow(30);
            Assert::IsTrue(value(sliced) == 30 && value(x) == 20);
        }
        TEST_METHOD(RandomArithmeticAndComparisons) { verifyRandomOperations<PublicLongLong>(); }
    };

    TEST_CLASS(PrivateLongLongTests)
    {
    public:
        TEST_METHOD(ConstructorsCopyAndAssignment) { verifyCopyAndAssignment<PrivateLongLong>(); }
        TEST_METHOD(ConstructorFromBase)
        {
            LongLong base(1, 705032704);
            PrivateLongLong x(base);
            Assert::IsTrue(value(x) == value(base));
        }
        TEST_METHOD(UsingDeclarations_GettersSettersInit) { verifyAccessAndInit<PrivateLongLong>(); }
        TEST_METHOD(MinusAndDivideOperators) { verifyDerivedArithmetic<PrivateLongLong>(); }
        TEST_METHOD(ComparisonOperators) { verifyDerivedComparisons<PrivateLongLong>(); }
        TEST_METHOD(PrefixPostfixIncrementDecrement) { verifyDerivedIncrementDecrement<PrivateLongLong>(); }
        TEST_METHOD(ArithmeticErrors) { verifyDerivedErrors<PrivateLongLong>(); }
        TEST_METHOD(StringConversionAndOwnOutput) { verifyStringAndOutput<PrivateLongLong>(); }
        TEST_METHOD(OwnInput_ValidNumbers) { verifyValidInput<PrivateLongLong>(); }
        TEST_METHOD(OwnInput_InvalidNumbersPreserveValue) { verifyInvalidInput<PrivateLongLong>(); }
        TEST_METHOD(UsingDeclarations_ReadAndDisplay) { verifyReadDisplay<PrivateLongLong>(); }
        TEST_METHOD(RandomArithmeticAndComparisons) { verifyRandomOperations<PrivateLongLong>(); }
    };
}
