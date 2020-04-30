/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2010-2018
Author(s): Jonathan Poelen
*/

#ifdef IN_IDE_PARSER
# define REDEMPTION_UNIT_TEST_CPP
#endif

#include "./redemption_unit_tests.hpp"
#include "cxx/cxx.hpp"

#include <boost/test/results_collector.hpp>
// #include <boost/test/results_reporter.hpp>
#include <boost/test/framework.hpp>

#include <algorithm>
#include <ostream>
#include <iomanip>

namespace redemption_unit_test__
{
    unsigned long current_count_error()
    {
        using boost::unit_test::results_collector;
        using boost::unit_test::framework::current_test_case;
        using boost::unit_test::test_case;
        using boost::unit_test::test_results;
        return results_collector.results(current_test_case().p_id).p_assertions_failed.get();
    }

    // std::ostream& current_stream()
    // {
    //     return boost::unit_test::results_reporter::get_stream();
    // }

    // based on element_compare from boost/test/tools/collection_comparison_op.hpp
    boost::test_tools::assertion_result bytes_EQ(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = pos != a.size() || a.size() != b.size();
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;

            ar.message()
              << "[" << Put2Mem{pos, a, b, " == ", min_len, pattern} << "]\n"
                "Mismatch at position " << pos;

            if (a.size() != b.size())
            {
                ar.message()
                    << "\nCollections size mismatch: "
                    << a.size() << " != " << b.size()
                ;
            }
        }

        return ar;
    }

    boost::test_tools::assertion_result bytes_NE(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = pos == a.size() && a.size() == b.size();
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;
            ar.message() << "[" << Put2Mem{pos, a, b, " != ", min_len, pattern} << "]";
        }

        return ar;
    }

    boost::test_tools::assertion_result bytes_LT(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = (pos == a.size())
            ? a.size() < b.size()
            : (pos != b.size() && a[pos] < b[pos]);
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;
            ar.message()
              << "[" << Put2Mem{pos, a, b, " >= ", min_len, pattern} << "]\n"
                "Mismatch at position " << pos;
        }

        return ar;
    }

    boost::test_tools::assertion_result bytes_LE(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = (pos == a.size())
            ? a.size() <= b.size()
            : (pos != b.size() && a[pos] <= b[pos]);
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;
            ar.message()
              << "[" << Put2Mem{pos, a, b, " > ", min_len, pattern} << "]\n"
                "Mismatch at position " << pos;
        }

        return ar;
    }

    boost::test_tools::assertion_result bytes_GT(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = (pos == a.size())
            ? a.size() > b.size()
            : (pos != b.size() && a[pos] > b[pos]);
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;
            ar.message()
              << "[" << Put2Mem{pos, a, b, " <= ", min_len, pattern} << "]\n"
                "Mismatch at position " << pos;
        }

        return ar;
    }

    boost::test_tools::assertion_result bytes_GE(bytes_view a, bytes_view b, char pattern, std::size_t min_len)
    {
        boost::test_tools::assertion_result ar(true);

        size_t pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        const bool r = (pos == a.size())
            ? a.size() >= b.size()
            : (pos != b.size() && a[pos] >= b[pos]);
        if (REDEMPTION_UNLIKELY(r))
        {
            ar = false;
            ar.message()
              << "[" << Put2Mem{pos, a, b, " < ", min_len, pattern} << "]\n"
                "Mismatch at position " << pos;
        }

        return ar;
    }

    std::ostream & operator<<(std::ostream & out, Put2Mem const & x)
    {
        ut::put_view(x.pos, out, {x.lhs, ut::PatternView(x.pattern), x.min_len});
        out << x.revert;
        ut::put_view(x.pos, out, {x.rhs, ut::PatternView(x.pattern), x.min_len});
        return out;
    }

    namespace
    {
#ifdef __clang__
        constexpr std::size_t start_type_name = 43;
        constexpr std::size_t end_type_name = 1;

        constexpr std::size_t prefix_value_name = 60;
        constexpr char end_value_name = '>';
#elif defined(__GNUC__)
        constexpr std::size_t start_type_name = 48;
        constexpr std::size_t end_type_name = 34;

        [[maybe_unused]] constexpr std::size_t prefix_value_name = 97;
        [[maybe_unused]] constexpr char end_value_name = ';';
#endif
    }

    std::string_view Enum::get_type_name(std::string_view s) noexcept
    {
        return {s.data() + start_type_name, s.size() - start_type_name - end_type_name};
    }

    std::string_view Enum::get_value_name(
        long long x, std::string_view name,
        std::string_view s0, std::string_view s1, std::string_view s2,
        std::string_view s3, std::string_view s4, std::string_view s5,
        std::string_view s6, std::string_view s7, std::string_view s8,
        std::string_view s9) noexcept
    {
        std::string_view s;
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 9)
        switch (x)
        {
            case 0: s = s0; break;
            case 1: s = s1; break;
            case 2: s = s2; break;
            case 3: s = s3; break;
            case 4: s = s4; break;
            case 5: s = s5; break;
            case 6: s = s6; break;
            case 7: s = s7; break;
            case 8: s = s8; break;
            case 9: s = s9; break;
            default:
                return {};
        }

        s.remove_prefix(prefix_value_name);

#ifdef __clang__
        if ('0' <= s[0] && s[0] <= '9')
#else
        if ('(' == s[name.size()])
#endif
        {
            return {};
        }

        s.remove_prefix(name.size());
        auto pos = s.find(end_value_name, name.size() + 2);
        if (pos != std::string_view::npos) {
            s.remove_suffix(s.size() - pos);
        }
        else {
            s = {};
        }
#else
        (void)x;
        (void)name;
        (void)s0; (void)s1; (void)s2; (void)s3; (void)s4;
        (void)s5; (void)s6; (void)s7; (void)s8; (void)s9;
#endif

        return s;
    }
} // namespace redemption_unit_test__


void RED_TEST_PRINT_TYPE_STRUCT_NAME<redemption_unit_test__::int_variation>::operator()(
    std::ostream& out, redemption_unit_test__::int_variation const & x) const
{
    if (x.left == x.right) {
        out << x.left;
    }
    else {
        out << x.value << "+-" << x.variant << (x.is_percent ? "%" : "")
            << " [" << x.left << ", " << x.right << "]";
    }
}

std::ostream& std::operator<<(std::ostream& out, ::redemption_unit_test__::Enum const& e)
{
    if (e.value_name.empty()) {
        out << e.name << "{";
        if (e.is_signed) {
            out << e.x;
        }
        else {
            out << static_cast<unsigned long long>(e.x);
        }
        out << "}";
    }
    else {
        out << e.value_name;
    }
    return out;
}

std::ostream& std::operator<<(std::ostream& out, ::redemption_unit_test__::BytesView const& v)
{
    ut::put_view(v.bytes.size(), out, {v.bytes});
    return out;
}

namespace
{
    constexpr uint8_t utf8_byte_size_table[] {
        // 0xxx x[xxx]
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        // 10xx x[xxx]  invalid value
        0, 0, 0, 0,
        0, 0, 0, 0,
        // 110x x[xxx]  2 bytes
        2, 2, 2, 2,
        // 1110 x[xxx]  3 bytes
        0, 0,
        // 1111 0[xxx]  4 bytes
        0,
        // 1111 1[xxx]  invalid value
        0,
    };

    using N0 = std::integral_constant<std::size_t, 0>;
    using N1 = std::integral_constant<std::size_t, 1>;
    using N2 = std::integral_constant<std::size_t, 2>;
    using N3 = std::integral_constant<std::size_t, 3>;
    using N4 = std::integral_constant<std::size_t, 4>;

    template<class Size, class F>
    void utf8_char_process(byte_ptr v, Size size, F&& f)
    {
        switch (utf8_byte_size_table[v[0] >> 3]) {
            case 0: return f(N0{});
            case 1: return f(N1{});
            case 2: return size >= 2 && (v[1] >> 6) == 0b10 ? f(N2{}) : f(N0{});
            // case 3:
            //     if (size < 3 || (v[2] >> 6) != 0b10) {
            //         return f(N0{});
            //     }
            //     switch (v[0] & 0b1111) {
            //         case 0: return (v[1] >> 5) == 0b101 ? f(N3{}) : f(N0{});
            //         case 0b1101: return (v[1] >> 5) == 0b100 ? f(N3{}) : f(N0{});
            //         default: return (v[1] >> 6) == 0b10 ? f(N3{}) : f(N0{});
            //     }
            // case 4:
            //     if (size < 4 || (v[2] >> 6) != 0b10 || (v[3] >> 6) != 0b10) {
            //         return f(N0{});
            //     }
            //     switch (v[0] & 0b111) {
            //         case 0b000: return (v[1] >> 6) == 0b10 && (v[1] >> 4) != 0b1000 ? f(N4{}) : f(N0{});
            //         case 0b001:
            //         case 0b010:
            //         case 0b011: return (v[1] >> 6) == 0b10 ? f(N4{}) : f(N0{});
            //         case 0b100: return (v[1] >> 4) == 0b1000 ? f(N4{}) : f(N0{});
            //         default: return f(N0{});
            //     }
        }

        REDEMPTION_UNREACHABLE();
    }

    char const * hex_table = "0123456789abcdef";

    static bool is_printable_ascii(uint8_t c)
    {
        return 0x20 <= c && c < 127;
    }

    static bool is_escapable_ascii(uint8_t c)
    {
        return c == '\b'
            || c == '\t'
            || c == '\r'
            || c == '\n';
    }

    enum class UnpritableMode
    {
        cstr,
        cstr_with_raw_newline,
        hex,
    };

    static bool put_unprintable_char(
        std::ostream& out, uint8_t c, UnpritableMode mode = UnpritableMode::cstr)
    {
        switch (mode)
        {
            case UnpritableMode::cstr:
            case UnpritableMode::cstr_with_raw_newline:
                switch (c) {
                    case '"': out << "\\\""; return false;
                    case '\b': out << "\\b"; return false;
                    case '\t': out << "\\t"; return false;
                    case '\r': out << "\\r"; return false;
                    case '\n':
                        out << ((mode == UnpritableMode::cstr) ? "\\n" : "\n");
                        return false;
                }
                break;

            case UnpritableMode::hex:
                break;
        }

        out << "\\x" << hex_table[c >> 4] << hex_table[c & 0xf];
        return true;
    }

    static bool is_hex(uint8_t c)
    {
        return
            ('0' <= c && c <= '9')
        || ('a' <= c && c <= 'f')
        || ('A' <= c && c <= 'F')
        ;
    }

    struct PutCharCtx
    {
        bool previous_is_printable = true;

        void put(std::ostream& out, uint8_t c, UnpritableMode mode = UnpritableMode::cstr)
        {
            this->put_cond(out, c, mode, is_printable_ascii(c));
        }

        void put_cond(std::ostream& out, uint8_t c, UnpritableMode mode, bool is_printable)
        {
            if (is_printable) {
                if (!previous_is_printable) {
                    // split hexadecimal rendering "\xaaa" -> "\xaa""a"
                    if (is_hex(c)) {
                        out << "\"\"";
                    }
                    previous_is_printable = true;
                }

                if (c == '\\') {
                    out << "\\\\";
                }
                else {
                    out << char(c);
                }
            }
            else {
                previous_is_printable = !put_unprintable_char(out, c, mode);
            }
        }
    };

    static std::ostream& put_dump_bytes(
        size_t pos, std::ostream& out, bytes_view x, size_t /*min_len*/)
    {
        if (x.size() == 0){
            return out << "\"\"\n";
        }

        constexpr size_t line_size = 16;
        char const* empty_line
            = "                                                                ";

        auto print_hex = [&](bytes_view av){
            for (uint8_t c : av) {
                out << "\\x" << hex_table[c >> 4] << hex_table[c & 0xf];
            }
        };

        auto print_data = [&](bytes_view av){
            for (uint8_t c : av) {
                out << (is_printable_ascii(c) ? char(c) : '.');
            }
        };

        auto print_line = [&](bytes_view av){
            out << '"';
            print_hex(av.first(line_size));
            out << "\" // ";
            print_data(av.first(line_size));
            av = av.from_offset(line_size);
            out << "\n";
            return av;
        };

        auto lbytes = x.first(pos);
        auto rbytes = x.from_offset(pos);

        while (lbytes.size() >= line_size) {
            lbytes = print_line(lbytes);
        }

        if (lbytes.empty()) {
            out << "\x1b[35m";
        }
        else {
            auto partial_right_size = std::min(line_size - lbytes.size(), rbytes.size());
            out << "\n\"";
            print_hex(lbytes);
            out << "\x1b[35m";
            print_hex(rbytes.first(partial_right_size));
            out << "\"" << &empty_line[(lbytes.size() + partial_right_size) * 4] << " // ";
            print_data(lbytes);
            print_data(rbytes.first(partial_right_size));
            out << "\n";
            rbytes = rbytes.from_offset(partial_right_size);
        }

        while (rbytes.size() >= line_size) {
            rbytes = print_line(rbytes);
        }

        if (!rbytes.empty()) {
            out << '"';
            print_hex(rbytes);
            out << "\"" << &empty_line[rbytes.size() * 4] << " // ";
            print_data(rbytes);
            out << "\n";
        }

        return out << "\x1b[0m";
    }

    static void put_utf8_bytes(
        size_t pos, std::ostream& out, bytes_view v,
        size_t /*min_len*/, UnpritableMode mode = UnpritableMode::cstr)
    {
        PutCharCtx putc_ctx;
        auto print = [&](bytes_view x, bool is_markable){
            auto* p = x.as_u8p();
            auto* end = p + x.size();

            while (p < end) {
                utf8_char_process(p, end-p, [&](std::size_t n){
                    if (n <= 1) {
                        putc_ctx.put(out, *p, mode);
                        ++p;
                    }
                    else {
                        out.write(char_ptr_cast(p), n);
                        putc_ctx.previous_is_printable = true;
                        p += n;
                    }
                });
            }

            if (is_markable) {
                out << "\x1b[35m";
            }
        };

        print(v.first(pos), pos != v.size());
        if (pos != v.size()) {
            out << "\x1b[35m";
            print(v.from_offset(pos), false);
            out << "\x1b[0m";
        }
    }

    static void put_utf8_bytes2(size_t pos, std::ostream& out, bytes_view v, size_t min_len)
    {
        put_utf8_bytes(pos, out, v, min_len, UnpritableMode::cstr_with_raw_newline);
    }

    static void put_ascii_bytes(
        size_t pos, std::ostream& out, bytes_view v,
        size_t min_len, UnpritableMode mode = UnpritableMode::cstr)
    {
        PutCharCtx putc_ctx;

        auto put_bytes = [&](auto print){
            print(v.first(pos));
            if (pos != v.size()) {
                out << "\x1b[35m";
                print(v.from_offset(pos));
                out << "\x1b[0m";
            }
        };

        if (min_len)
        {
            std::vector<bool> is_ascii_mask(v.size());

            auto pmask = is_ascii_mask.begin();
            auto first = v.begin();
            auto last = v.end();

            while (first != last) {
                auto pos = std::find_if(first, last, [](uint8_t c){
                    return not is_printable_ascii(c);
                });

                if (pos == first) {
                    ++first;
                    ++pmask;
                    continue;
                }

                auto end = pmask + (pos - first);
                if (size_t(pos - first) > min_len) {
                    std::fill(pmask, end, true);
                }
                pmask = end;
                first = pos;
            }

            pmask = is_ascii_mask.begin();

            put_bytes([&](bytes_view x){
                for (uint8_t c : x) {
                    putc_ctx.put_cond(out, c, mode, *pmask);
                    ++pmask;
                }
            });
        }
        else {
            put_bytes([&](bytes_view x){
                for (uint8_t c : x) {
                    putc_ctx.put(out, c, mode);
                }
            });
        }
    }

    static void put_ascii_bytes2(size_t pos, std::ostream& out, bytes_view v, size_t min_len)
    {
        put_ascii_bytes(pos, out, v, min_len, UnpritableMode::cstr_with_raw_newline);
    }

    static void put_hex_bytes(size_t pos, std::ostream& out, bytes_view v, size_t /*min_len*/)
    {
        auto print = [&](bytes_view x){
            for (uint8_t c : x) {
                out << "\\x" << hex_table[c >> 4] << hex_table[c & 0xf];
            }
        };

        print(v.first(pos));
        if (pos != v.size()) {
            out << "\x1b[35m";
            print(v.from_offset(pos));
            out << "\x1b[0m";
        }
    }

    static void put_auto_bytes(size_t pos, std::ostream& out, bytes_view v, size_t min_len)
    {
        auto n = std::min(int(v.size()), 36);
        auto* p = v.as_u8p();
        auto* end = p + n;
        int count_invalid = 0;
        int count_alpha = 0;
        int count_escaped = 0;
        while (p < end) {
            utf8_char_process(p, end-p, [&](std::size_t n){
                if (n == 0) {
                    ++count_invalid;
                    ++p;
                }
                else {
                    if (n == 1) {
                        if (is_printable_ascii(*p)) {
                            ++count_alpha;
                        }
                        else {
                            if (is_escapable_ascii(*p)) {
                                ++count_escaped;
                            }
                            ++count_invalid;
                        }
                    }
                    p += n;
                }
            });
        }

        if (count_invalid > n / 6) {
            put_ascii_bytes(pos, out, v, min_len,
                (count_alpha / 4 > count_escaped)
                    ? UnpritableMode::cstr
                    : UnpritableMode::hex);
        }
        else {
            put_utf8_bytes(pos, out, v, min_len,
                (v.size() < 255)
                    ? UnpritableMode::cstr
                    : UnpritableMode::cstr_with_raw_newline);
        }
    }
}

namespace ut
{
    bool compare_bytes(size_t& pos, bytes_view b, bytes_view a) noexcept
    {
        pos = std::mismatch(a.begin(), a.end(), b.begin(), b.end()).first - a.begin();
        return pos == a.size() && a.size() == b.size();
    }

    void put_view(size_t pos, std::ostream& out, flagged_bytes_view x)
    {
        char const* sep = (x.pattern == PatternView('d')) ? "" : "\"";
        out << sep;
        switch (x.pattern) {
            #define CASE(c, print) case PatternView(c): \
                print(pos, out, x, x.min_len);          \
                break
            CASE('c', put_ascii_bytes);
            CASE('C', put_ascii_bytes2);
            CASE('s', put_utf8_bytes);
            CASE('S', put_utf8_bytes2);
            CASE('b', put_hex_bytes);
            CASE('d', put_dump_bytes);
            default:
            CASE('a', put_auto_bytes);
            #undef CASE
        }
        out << sep;
    }
}
