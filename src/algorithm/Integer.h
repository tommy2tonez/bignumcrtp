#ifndef __ALGORITHM_INTEGER_H__
#define __ALGORITHM_INTEGER_H__

#include "BoolVector.h"
#include "log2.h"

namespace algorithm::integer{

    inline uint64_t get_bit_mask(uint64_t bit_length){

        if (bit_length == 64){

            return 0xFFFFFFFFFFFFFFFF;

        }

        return (1ULL << bit_length) - 1;

    }
    
    inline uint8_t byte_length(uint64_t x){

        uint64_t bit_idx = algorithm::log2::log2_64(x);

        return algorithm::boolvector::byte_length(bit_idx);

    }

    inline uint64_t fuse(uint64_t a, uint64_t b, uint64_t b_bit_length){

        return (a << b_bit_length) | b;

    }

    inline std::pair<uint64_t, uint64_t> defuse(uint64_t fused, uint64_t b_bit_length){

        uint64_t first = fused >> b_bit_length;
        uint64_t second = fused & get_bit_mask(b_bit_length);

        return std::pair<uint64_t, uint64_t>(first, second);

    }


}

#endif