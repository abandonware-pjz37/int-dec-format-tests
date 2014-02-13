#ifndef CPPX_HPP_
#define CPPX_HPP_

#include <algorithm>
#include <limits>

namespace cppx {
    using std::numeric_limits;
    using std::reverse;

    typedef numeric_limits<long>    Long_info;
    int const long_digits   = Long_info::max_digits10;
    int const long_bufsize  = long_digits + 2;

    inline void unsigned_to_decimal( unsigned long number, char* buffer )
    {
        if( number == 0 )
        {
            *buffer++ = '0';
        }
        else
        {
            char* p_first = buffer;
            while( number != 0 )
            {
                *buffer++ = '0' + number % 10;
                number /= 10;
            }
            reverse( p_first, buffer );
        }
        *buffer = '\0';
    }

    inline auto decimal_from_unsigned( unsigned long number, char* buffer )
        -> char const*
    {
        unsigned_to_decimal( number, buffer );
        return buffer;
    }

    inline void to_decimal( long number, char* buffer )
    {
        if( number < 0 )
        {
            buffer[0] = '-';
            unsigned_to_decimal( -number, buffer + 1 );
        }
        else
        {
            unsigned_to_decimal( number, buffer );
        }
    }

    inline auto decimal_from( long number, char* buffer )
        -> char const*
    {
        to_decimal( number, buffer );
        return buffer;
    }
}  // namespace cppx

#endif // CPPX_HPP_
