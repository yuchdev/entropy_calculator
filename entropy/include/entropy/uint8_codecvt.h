#pragma once
#include <cinttypes>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <locale>

using byte_type = std::uint8_t;

namespace std
{
    // Specialization of std::char_traits
    template <> struct char_traits< std::uint8_t >
    {
        using char_type = std::uint8_t;
        using int_type = int;
        using off_type = std::streamoff;
        using pos_type = std::streampos;
        using state_type = std::mbstate_t;

        static void assign(char_type& value1, const char_type& value2)
        {
            value1 = value2;
        }

        static char_type* assign(char_type* ptr, std::size_t count, char_type value)
        {
            return static_cast<char_type*>(std::memset(ptr, value, count));
        }

        static constexpr bool eq(const char_type& value1, const char_type& value2) noexcept
        {
            return value1 == value2;
        }

        static constexpr bool lt(const char_type value1, const char_type value2) noexcept
        {
            return value1 < value2;
        }

        static std::size_t length(const char_type* ptr)
        {
            std::size_t i = 0;
            while (!eq(ptr[i], char_type()))
            {
                ++i;
            }
            return i;
        }

        static int compare(const char_type* ptr1, const char_type* ptr2, std::size_t count)
        {
            return std::memcmp(ptr1, ptr2, count);
        }

        static const char_type* find(const char_type* ptr, std::size_t count, const char_type& value)
        {
            return static_cast<const char_type*>(std::memchr(ptr, value, count));
        }

        static char_type* move(char_type* dest, const char_type* src, std::size_t count)
        {
            return static_cast<char_type*>(std::memmove(dest, src, count));
        }

        static char_type* copy(char_type* dest, const char_type* src, std::size_t count)
        {
            return static_cast<char_type*>(std::memcpy(dest, src, count));
        }

        static constexpr char_type to_char_type(const int_type& value) noexcept
        {
            return static_cast<char_type>(value);
        }

        static constexpr int_type to_int_type(const char_type& value) noexcept
        {
            return static_cast<int_type>(value);
        }

        static constexpr bool eq_int_type(const int_type& value1, const int_type& value2) noexcept
        {
            return value1 == value2;
        }

        static constexpr int_type eof() noexcept
        {
            return static_cast<int_type>(std::char_traits<char>::eof());
        }

        static constexpr int_type not_eof(const int_type& value) noexcept
        {
            return (value == eof()) ? 0 : value;
        }
    };

    // Specialization of std::codecvt
    template<> class codecvt< std::uint8_t, char, std::mbstate_t > : public locale::facet
    {
    public:
        using internal_type = std::uint8_t;
        using external_type = char;
        using state_type = std::mbstate_t;

        static std::locale::id id;

        codecvt(std::size_t refs = 0)
            : locale::facet(refs)
        {}

        std::codecvt_base::result out(state_type& state, const internal_type* from, const internal_type* from_end, const internal_type*& from_next, external_type* to, external_type* to_end, external_type*& to_next) const
        {
            return do_out(state, from, from_end, from_next, to, to_end, to_next);
        }

        std::codecvt_base::result in(state_type& state, const external_type* from, const external_type* from_end, const external_type*& from_next, internal_type* to, internal_type* to_end, internal_type*& to_next) const
        {
            return do_in(state, from, from_end, from_next, to, to_end, to_next);
        }

        std::codecvt_base::result unshift(state_type& state, external_type* to, external_type* to_end, external_type*& to_next) const
        {
            return do_unshift(state, to, to_end, to_next);
        }

        int length(state_type& state, const external_type* from, const external_type* from_end, std::size_t max) const
        {
            return do_length(state, from, from_end, max);
        }

        int max_length() const noexcept
        {
            return do_max_length();
        }

        int encoding() const noexcept
        {
            return do_encoding();
        }

        bool always_noconv() const noexcept
        {
            return do_always_noconv();
        }

    protected:
        virtual ~codecvt() {}
        virtual std::codecvt_base::result do_out(state_type& state, const internal_type* from, const internal_type* from_end, const internal_type*& from_next, external_type* to, external_type* to_end, external_type*& to_next) const;
        virtual std::codecvt_base::result do_in(state_type& state, const external_type* from, const external_type* from_end, const external_type*& from_next, internal_type* to, internal_type* to_end, internal_type*& to_next) const;
        virtual std::codecvt_base::result do_unshift(state_type& state, external_type* to, external_type* to_end, external_type*& to_next) const;
        virtual int do_length(state_type& state, const external_type* from, const external_type* from_end, std::size_t max) const;
        virtual int do_max_length() const noexcept;
        virtual int do_encoding() const noexcept;
        virtual bool do_always_noconv() const noexcept;
    }; // class codecvt

    locale::id codecvt< std::uint8_t, char, std::mbstate_t >::id;

    codecvt_base::result codecvt< std::uint8_t, char, std::mbstate_t >::do_out(state_type& state, const internal_type* from, const internal_type* from_end, const internal_type*& from_next, external_type* to, external_type* to_end, external_type*& to_next) const
    {
        (void) state; (void) from_end; (void) to_end; // Unused parameters
        from_next = from;
        to_next = to;
       return codecvt_base::noconv;
    }

    codecvt_base::result codecvt< std::uint8_t, char, std::mbstate_t >::do_in(state_type& state, const external_type* from, const external_type* from_end, const external_type*& from_next, internal_type* to, internal_type* to_end, internal_type*& to_next) const
    {
        (void) state; (void) from_end; (void) to_end; // Unused parameters
        from_next = from;
        to_next = to;
        return std::codecvt_base::noconv;
    }

    codecvt_base::result codecvt< std::uint8_t, char, std::mbstate_t >::do_unshift(state_type& state, external_type* to, external_type* to_end, external_type*& to_next) const
    {
        (void) state; (void) to_end; // Unused perameters
        to_next = to;
        return std::codecvt_base::noconv;
    }

    int codecvt< std::uint8_t, char, std::mbstate_t >::do_length(state_type& state, const external_type* from, const external_type* from_end, std::size_t max) const
    {
        (void) state; // Unused parameter
        return static_cast<int>(std::min< std::size_t >(max, static_cast<std::size_t>(from_end - from)));
    }

    int codecvt< std::uint8_t, char, std::mbstate_t >::do_max_length() const noexcept
    {
        return 1;
    }

    int codecvt< std::uint8_t, char, std::mbstate_t >::do_encoding() const noexcept
    {
        return 1;
    }

    bool codecvt< std::uint8_t, char, std::mbstate_t >::do_always_noconv() const noexcept
    {
        return true;
    }
} // namespace std

