#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <locale>

using namespace std;

/**
 * @brief Пользовательский итератор ввода для чтения целых чисел из потока
 *
 * Данный итератор позволяет читать целочисленные значения из входного потока,
 * используя алгоритмы STL. Обрабатывает условия конца потока и предоставляет
 * стандартную функциональность итератора.
 */
class ptin_iterator {
    std::istream* in_stream;
    int value;
    bool end_marker;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    /**
     * @brief Конструктор по умолчанию - создаёт итератор конца потока
     */
    ptin_iterator() : in_stream(nullptr), end_marker(true) {}

    /**
     * @brief Конструктор для активного итератора потока
     * @param s Ссылка на входной поток для чтения
     *
     * Инициализирует итератор и читает первое значение из потока.
     */
    ptin_iterator(std::istream& s) : in_stream(&s), end_marker(false) {
        ++(*this);
    }

    /**
     * @brief Оператор разыменования
     * @return Константная ссылка на текущее значение
     */
    reference operator*() const { return value; }

    /**
     * @brief Оператор доступа к членам
     * @return Указатель на текущее значение
     */
    pointer operator->() const { return &value; }

    /**
     * @brief Префиксный оператор инкремента - читает следующее целое число из потока
     * @return Ссылка на данный итератор после инкремента
     */
    ptin_iterator& operator++() {
        if (in_stream && !(*in_stream >> value)) {
            end_marker = true;
        }
        return *this;
    }

    /**
     * @brief Постфиксный оператор инкремента
     * @return Состояние итератора до инкремента
     */
    ptin_iterator operator++(int) {
        ptin_iterator temp = *this;
        ++(*this);
        return temp;
    }

    /**
     * @brief Оператор сравнения на равенство
     * @param other Итератор для сравнения
     * @return true, если итераторы равны, false в противном случае
     *
     * Два итератора считаются равными, если оба являются маркерами конца,
     * либо если у них одинаковое состояние маркера конца и указатель на поток.
     */
    bool operator==(const ptin_iterator& other) const {
        return (end_marker && other.end_marker) ||
            (end_marker == other.end_marker && in_stream == other.in_stream);
    }

    /**
     * @brief Оператор сравнения на неравенство
     * @param other Итератор для сравнения
     * @return true, если итераторы не равны, false в противном случае
     */
    bool operator!=(const ptin_iterator& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Подсчитывает количество нулей в потоке ввода
 * @param input Ссылка на входной поток
 * @param source_name Название источника для вывода (по умолчанию "потоке")
 * @return Количество найденных нулей
 */
long countZerosInStream(std::istream& input, const std::string& source_name = "потоке") {
    ptin_iterator it_begin(input);
    ptin_iterator it_end;

    long count = std::count(it_begin, it_end, 0);
    std::cout << "Количество нулей в " << source_name << ": " << count << std::endl;

    return count;
}

/**
 * @brief Точка входа в программу
 *
 * Читает целые числа из стандартного ввода и подсчитывает количество нулей,
 * используя алгоритм STL с пользовательским итератором.
 *
 * @return 0 при успешном выполнении
 */
int main() {
    setlocale(LC_ALL, "Russian");
    string filename;
    cout << "Введите имя файла (например, numbers.txt): ";
    cin >> filename;
    ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        std::cerr << "Убедитесь, что файл существует в директории с программой." << std::endl;
        return 1;
    }

    cout << "Чтение чисел из файла " << filename << ":" << std::endl;
    countZerosInStream(file, "файле " + filename);
    file.close();
    
    return 0;
}