/*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*   Product name: redemption, a FLOSS RDP proxy
*   Copyright (C) Wallix 2010-2016
*   Author(s): Jonathan Poelen
*/

#pragma once

#include <type_traits>
#include <string_view>

#include <cstddef>
#include <cstring>
#include <chrono>

#include "configs/attributes/spec.hpp"


struct string_type_name
{
    char const * first;
    char const * last;

    std::string_view to_sv() const
    {
        return std::string_view{first, std::size_t(last - first)};
    }
};

namespace detail
{
    template<class T, bool = std::is_integral<T>::value || std::is_enum<T>::value>
    struct type_name_impl;

    template<class T>
    struct type_name_impl<T, true>
    {
        static string_type_name impl()
        {
            char const * s = __PRETTY_FUNCTION__;
#ifdef __clang__
            std::size_t const sz = strlen(s);
            return {s + 47, s + (47 + (sz-68)/2)};
#elif defined(__GNUG__)
            return {s + 74, s + strlen(s) - 1};
#endif
        }
    };

    template<std::size_t n>
    string_type_name zstring_to_string_type(char const (&s)[n])
    {
        return {s, s+n-1};
    }

#define TYPE_NAME_I(...)                                 \
    template<>                                           \
    struct type_name_impl<__VA_ARGS__, false>            \
    {                                                    \
        static string_type_name impl()                   \
        {                                                \
            return zstring_to_string_type(#__VA_ARGS__); \
        }                                                \
    }

    TYPE_NAME_I(std::chrono::hours);
    TYPE_NAME_I(std::chrono::minutes);
    TYPE_NAME_I(std::chrono::seconds);
    TYPE_NAME_I(std::chrono::milliseconds);
    TYPE_NAME_I(std::chrono::duration<unsigned, std::ratio<1, 10>>);
    TYPE_NAME_I(std::chrono::duration<unsigned, std::ratio<1, 100>>);

#undef TYPE_NAME_I
}

template<class T>
std::string_view type_name(T const * = nullptr)
{ return detail::type_name_impl<T>::impl().to_sv(); }

#define CONFIG_DEFINE_TYPE_NAME(type, name)               \
    namespace detail {                                    \
        template<bool B> struct type_name_impl<type, B> { \
            static string_type_name impl() {              \
                char const * s = name;                    \
                return {s, s + (sizeof(name) - 1)};       \
            }                                             \
        };                                                \
    }

#define CONFIG_DEFINE_TYPE_NAME2(type) \
    CONFIG_DEFINE_TYPE_NAME(type, #type)

#define CONFIG_DEFINE_TYPE(type) \
    class type {};               \
    CONFIG_DEFINE_TYPE_NAME(type, #type)

CONFIG_DEFINE_TYPE_NAME2(std::string)
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::u8,  "uint8_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::u16, "uint16_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::u32, "uint32_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::u64, "uint64_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::s8,  "int8_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::s16, "int16_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::s32, "int32_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::s64, "int64_t")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::int_, "int")
CONFIG_DEFINE_TYPE_NAME(cfg_attributes::types::unsigned_, "unsigned")
