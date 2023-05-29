#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdint.h>
#include <assert.h>
#include <memory>
#include <iostream>

namespace memory::circularbuffer{

    class CircularBuffer{

        private:

            char * data;
            
            uint64_t buffer_length;
            uint64_t expected_length;
            uint64_t buf_idx; 

        public:

            CircularBuffer(){

                this->data = nullptr;
                this->buffer_length = 0; 
                this->expected_length = 0;

            }
            
            ~CircularBuffer(){

                std::free(data);

            }

            CircularBuffer(uint64_t buffer_length, uint64_t expected_length){
                
                CircularBuffer temp; 
                
                temp.set_buffer_length(buffer_length);
                temp.set_expected_length(expected_length);

                (*this) = std::move(temp);

            }

            CircularBuffer& operator = (CircularBuffer&& obj){

                this->data = obj.data;
                this->expected_length = obj.expected_length;
                this->buf_idx = obj.buf_idx;

                obj.data = nullptr;
                
                return *this;
                
            }

            void set_buffer_length(uint64_t buffer_length){

                this->buffer_length = buffer_length;
                
                free(this->data);
                this->data = (char *) malloc(this->buffer_length * sizeof(char));
            
            }


            void set_expected_length(uint64_t expected_length){
                
                assert(this->expected_length < this->buffer_length);

                this->expected_length = expected_length;

            }

            void * get(){
                                
                if (this->buf_idx + this->expected_length <= this->buffer_length){
                    
                    this->buf_idx += this->expected_length;

                    return this->data + this->buf_idx - this->expected_length;

                }

                this->buf_idx = this->expected_length;

                return this->data;

            }

    };

}

#endif