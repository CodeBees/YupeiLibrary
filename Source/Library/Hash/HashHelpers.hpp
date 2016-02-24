#pragma once

#include <cstdint>
#include <cstddef>

namespace Yupei
{
    namespace Internal
    {
        class HashHelpers
        {
            //http://planetmath.org/goodhashtableprimes
            static constexpr std::uint32_t primes[] = {
                3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
                1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
                17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
                187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
                1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369, 12582917u, 25165843u,
                50331653u, 100663319u, 201326611u, 402653189u, 805306457u, 1610612741u
            };

            static bool IsPrime(std::size_t candidate) noexcept;

        public:        
            static std::size_t GetPrime(std::size_t min) noexcept;
            static std::size_t ExpandPrime(std::size_t oldSize) noexcept;
        };
    }
    
}