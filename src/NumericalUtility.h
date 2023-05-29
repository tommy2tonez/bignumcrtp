><?~#ifndef __NUMERICAL_UTILITY_H__
#define __NUMERICAL_UTILITY_H__

#include <stdint.h>
#include "BoolVector.h"

namespace utility::numerical{

    template <typename T>
    uint64_t log2able_ceil(T int_pod){

    }
    
    namespace serializer{

        namespace bitstream{

            namespace fixed_length{

                datastructure::OptimizedBoolVector serialize_uint(uint64_t data, uint64_t bvec_length){

                }

                uint64_t deserialize_uint(datastructure::ViewableBoolVector& data, uint64_t bvec_length){

                }
            }

        }

    }

}

#endif 