#pragma once

#include <array>

#define PATTERN(str) pattern<str>::value()

template<size_t len>
struct string_literal
{
    size_t length = len - 1;
    std::array<char, len - 1> value{ };

    consteval string_literal(const char(&str)[len])
    {
        for (size_t i{ }; i < len - 1; i++)
            value[i] = str[i];
    }
};

static consteval bool is_digit(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

static consteval int convert_digit(char c)
{
    return (c > '9') ? (c & ~0x20) - 'A' + 10 : (c - '0');
}

static consteval int make_digits(char a, char b)
{
    return 16 * convert_digit(a) + convert_digit(b);
}

template<string_literal str>
struct pattern
{
    struct length_t
    {
        static consteval auto get()
        {
            size_t ret{ };
            bool was_digit{ };

            for (size_t i = 0; i < str.length; i++) {
                if (is_digit(str.value[i])) {
                    if (!was_digit)
                        ret++;
                    was_digit = true;
                } else if (str.value[i] == '?') {
                    ret++;
                    was_digit = false;
                } else if (str.value[i] == ' ')
                    was_digit = false;

            }
            return ret;
        }
    };

    static consteval auto value()
    {
        constexpr auto len = length_t::get();
        std::array<int, len> ret{ };

        for (size_t i = 0, j = 0; i < str.length; i++) {
            if (str.value[i] == ' ')
                continue;

            if (is_digit(str.value[i])) {
                i++;
                if (j < len) {
                    if (is_digit(str.value[i]))
                        ret[j++] = make_digits(str.value[i - 1], str.value[i]);
                }
            } else if (str.value[i] == '?')
                ret[j++] = -1;
        }
        return ret;
    }
};