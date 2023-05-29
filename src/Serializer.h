#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include "stdint.h"

namespace serializer{

    class Serializable{
        
        public:
        
            virtual void serialize(void *&, size_t&) = 0;
            virtual void deserialize(void *) = 0;

    };

};


#endif