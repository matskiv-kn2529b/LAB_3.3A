#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include "PublicLongLong.h"
#include "PrivateLongLong.h"

// Підстановка: відкритий нащадок придатний там, де очікується база.
// Функція і приймає, і повертає об'єкт базового класу.
LongLong decreased(LongLong value) { value.decrement(); return value; }
void showBase(const LongLong& value) { std::cout << value << '\n'; }

void printSizes() {
#ifdef LAB_PACKED
    std::cout << "Режим: #pragma pack(1)\n";
#else
    std::cout << "Режим: стандартне пакування\n";
#endif
    std::cout << "sizeof(LongLong) = " << sizeof(LongLong) << '\n'
              << "sizeof(PublicLongLong) = " << sizeof(PublicLongLong) << '\n'
              << "sizeof(PrivateLongLong) = " << sizeof(PrivateLongLong) << '\n'
              << "alignof(LongLong) = " << alignof(LongLong) << '\n';
}

template<class T> void demonstrateOperators(const char* title) {
    std::cout << "\n" << title << '\n';
    T a(1, 705032704), b(0, 3), copy(a), assigned;
    assigned = a;
    std::cout << "Копія: " << copy << "; присвоєння: " << assigned << '\n';
    std::cout << "a - b = " << a - b << "; a / b = " << a / b << '\n';
    std::cout << std::boolalpha << "b <= a: " << (b <= a)
              << "; a == copy: " << (a == copy)
              << "; a != b: " << (a != b) << '\n';
    T edge(0, -1); // low bits = 0xFFFFFFFF; усе число = 2^32 - 1.
    std::cout << "edge: " << edge << '\n';
    std::cout << "edge++ (старе): " << edge++ << '\n';
    std::cout << "після edge++: " << edge << '\n';
    std::cout << "++edge: " << ++edge << '\n';
    std::cout << "edge-- (старе): " << edge-- << '\n';
    std::cout << "--edge: " << --edge << '\n';
    const std::string text = a;
    std::cout << "operator string(): " << text << '\n';
    T fromBase(LongLong(0, 77));
    std::cout << "З базового об'єкта: " << fromBase << '\n';
    fromBase.Init(0, 10);
    fromBase.setHigh(0);
    fromBase.setLow(12);
    std::cout << "Гетери: " << fromBase.getHigh() << ", " << fromBase.getLow() << '\n';
    fromBase.Display();
    std::cout << fromBase.toString() << '\n';

    T defaults[2];
    T initialized[2] = { T(0, 8), T(a) };
    std::unique_ptr<T[]> dynamic(new T[2]);
    defaults[0] = initialized[0];
    dynamic[1] = initialized[1];
    std::cout << "Масиви: " << defaults[0] << "; " << dynamic[1] << '\n';
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc > 1 && std::string(argv[1]) == "--sizes") {
        printSizes(); return 0;
    }
    try {
        std::cout << "Лабораторна робота 3.3А. Варіант 21\n";
        LongLong zero, a(1, 705032704), b(0, 3), copy(a);
        zero.Init(0, 7);
        zero.setHigh(0); zero.setLow(8);
        std::cout << "Гетери бази: " << zero.getHigh() << ", " << zero.getLow() << '\n';
        zero.Display();
        std::cout << "toString: " << zero.toString() << '\n';
        std::cout << "subtract: " << a.subtract(b) << "; divide: " << a.divide(b) << '\n';
        std::cout << std::boolalpha << "notGreater: " << b.notGreater(a)
                  << "; equal: " << a.equal(copy) << "; notEqual: " << a.notEqual(b) << '\n';
        zero.increment(); zero.decrement();
        LongLong baseArray[2] = { a, b };
        std::unique_ptr<LongLong[]> baseDynamic(new LongLong[2]);
        baseDynamic[0] = baseArray[0];
        std::cout << "Масив бази: " << baseDynamic[0] << '\n';

        demonstrateOperators<PublicLongLong>("Відкрите успадкування");
        demonstrateOperators<PrivateLongLong>("Закрите успадкування");
        PublicLongLong pub(0, 100);
        showBase(pub);
        std::cout << "decreased(pub) = " << decreased(pub) << '\n';
        LongLong& ref = pub;
        ref.setLow(101);
        LongLong* pointer = &pub;
        std::cout << "Через базове посилання/покажчик: " << *pointer << '\n';
        // PrivateLongLong priv; showBase(priv); // Помилка: база недоступна ззовні.
        // using у private-нащадку відкриває окремі методи, а не перетворення до бази.

        // Демонстрація правильного та неправильного потокового введення.
        LongLong streamValue;
        std::istringstream good("5000000000");
        if (!(good >> streamValue)) throw std::runtime_error("Помилка демонстрації введення.");
        std::cout << "Введення з потоку: " << streamValue << '\n';
        std::istringstream bad("24abc");
        bad >> streamValue;
        std::cout << "24abc відхилено: " << bad.fail()
                  << "; попереднє значення: " << streamValue << '\n';

        printSizes();
        // --demo зручний для автоматичного запуску без клавіатури.
        if (argc > 1 && std::string(argv[1]) == "--demo") return 0;
        std::cout << "\nВведіть LongLong через Read(): ";
        LongLong input;
        input.Read(); input.Display();
        std::cout << "Введіть PublicLongLong через оператор бази: ";
        PublicLongLong inputPublic;
        if (!(std::cin >> inputPublic))
            throw std::invalid_argument("Некоректне ціле число.");
        std::cout << inputPublic << '\n';
        std::cout << "Введіть PrivateLongLong через власний оператор: ";
        PrivateLongLong inputPrivate;
        if (!(std::cin >> inputPrivate))
            throw std::invalid_argument("Некоректне ціле число.");
        std::cout << inputPrivate << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Помилка: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
