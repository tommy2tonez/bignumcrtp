#ifndef __ALGORITHM_BOOLVECTOR_H__
#define __ALGORITHM_BOOLVECTOR_H__

#include <stdint.h>

namespace algorithm::boolvector{

    inline const uint8_t BIT_WIDTH = 8;
    inline const uint8_t BIT_OFFSET = 3;

    inline uint64_t offset(uint64_t bit_idx){

        return bit_idx >> BIT_OFFSET;

    }

    inline uint64_t slot(uint64_t bit_idx){

        return bit_idx % BIT_WIDTH;

    }

    inline uint64_t byte_length(uint64_t bit_idx){

        return (bit_idx >> BIT_OFFSET) + 1; 

    }
    
    inline uint8_t combine_two_bool(bool a, bool b){

        return ((uint8_t) a << 1) | (uint8_t) b;
    }

    inline uint8_t intify_one_bool(bool a){

        return (uint8_t) a;
        
    }
    
    inline uint64_t bit_length(uint64_t byte_size){

        return byte_size << BIT_OFFSET;

    }

};

#endif

