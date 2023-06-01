#ifndef __BOOLVECTOR_H_
#define __BOOLVECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include "CircularBuffer.h"

namespace algorithm::memory{

    typedef void (*memcpy_lambda)(void * dst, void * src);

    inline void defaultize(char &data, uint8_t trailing_length){
        
        data &= 0xFF >> trailing_length; 

    }

    inline void memcpy(void * dst, void * src, size_t n){
        if (n >= 0x4){
            if (n >= 0x6){
                if (n >= 0x7){
                    std::memcpy(dst, src, n);
                    return;
                }
                ((char *) dst)[0] = ((char *) src)[0];
                ((char *) dst)[1] = ((char *) src)[1];
                ((char *) dst)[2] = ((char *) src)[2];
                ((char *) dst)[3] = ((char *) src)[3];
                ((char *) dst)[4] = ((char *) src)[4];
                ((char *) dst)[5] = ((char *) src)[5];
                return;
            }
            if (n >= 0x5){
                ((char *) dst)[0] = ((char *) src)[0];
                ((char *) dst)[1] = ((char *) src)[1];
                ((char *) dst)[2] = ((char *) src)[2];
                ((char *) dst)[3] = ((char *) src)[3];
                ((char *) dst)[4] = ((char *) src)[4];
                return;
            }
            ((char *) dst)[0] = ((char *) src)[0];
            ((char *) dst)[1] = ((char *) src)[1];
            ((char *) dst)[2] = ((char *) src)[2];
            ((char *) dst)[3] = ((char *) src)[3];
            return;
        }
        if (n >= 0x2){
            if (n >= 0x3){
                ((char *) dst)[0] = ((char *) src)[0];
                ((char *) dst)[1] = ((char *) src)[1];
                ((char *) dst)[2] = ((char *) src)[2];
                return;
            }

            ((char *) dst)[0] = ((char *) src)[0];
            ((char *) dst)[1] = ((char *) src)[1];
            return;
        }
        if (n >= 0x1){
            ((char *) dst)[0] = ((char *) src)[0];
            return;
        }
        return;
    }

    inline void small_memcpy(void * dst, void * src, size_t n){

        if (n >= 0x3){
            if (n >= 0x4){
                std::memcpy(dst, src, n);
                return;
            }
            ((char *) dst)[0] = ((char *) src)[0];
            ((char *) dst)[1] = ((char *) src)[1];
            ((char *) dst)[2] = ((char *) src)[2];
            return;
        }
        if (n >= 0x2){

            ((char *) dst)[0] = ((char *) src)[0];
            ((char *) dst)[1] = ((char *) src)[1];
            return;
        }
        ((char *) dst)[0] = ((char *) src)[0];
    }

    inline void memcpy_3(void * dst, void * src){

        ((uint16_t *) dst)[0] = ((uint16_t *) src)[0];
        ((uint8_t *) dst)[2] = ((uint8_t *) src)[2];

    }

    inline void memcpy_2(void * dst, void * src){

        ((uint16_t *) dst)[0] = ((uint16_t *) src)[0];

    }

    inline void memcpy_1(void * dst, void * src){

        ((uint8_t *) dst)[0] = ((uint8_t *) src)[0];

    }

    inline void memcpy_4(void * dst, void * src){
        ((uint32_t *) dst)[0] = ((uint32_t *) src)[0];
    }

    inline memcpy_lambda get_optimized_memcpy(size_t n){

        switch (n){
            case 1:
                return memcpy_1;
            case 2:
                return memcpy_2;
            case 3: 
                return memcpy_3;
            case 4:
                return memcpy_4;
            default:
                break;
        }

        assert(false);

    }

    inline void memcpy_w_trailing(void * dst, void * src, size_t n, size_t trailing_spaces){
        algorithm::memory::memcpy(dst, src, n);
        algorithm::memory::defaultize(*((char *) dst + n - 1), trailing_spaces);
    }

    inline void small_memcpy_w_trailing(void * dst, void * src, size_t n, size_t trailing_spaces){
        algorithm::memory::small_memcpy(dst, src, n);
        algorithm::memory::defaultize(*((char *) dst + n - 1), trailing_spaces);
    }

}

namespace datastructure{

    inline static const int CIRCULAR_BUFFER_LENGTH = 2000;
    inline static const int EXPECTED_BUFFER_LENGTH = 50;
    inline static memory::circularbuffer::CircularBuffer very_undefined(CIRCULAR_BUFFER_LENGTH, EXPECTED_BUFFER_LENGTH);

    template <class T>
    class UnalignedViewableBoolVector{

        public:

            size_t length(){

                return static_cast<T*>(this)->length(); 

            }

            size_t byte_length(){

                return static_cast<T*>(this)->byte_length();

            }

            bool get(size_t idx){

                return static_cast<T*>(this)->get(idx);

            }

            UnalignedViewableBoolVector<T> * to_unaligned_viewable(){

                return this;
            
            }
    
    };

    template <class T>
    class ViewableBoolVector: public UnalignedViewableBoolVector<T>{

        public:

            void * get_data(){

                return static_cast<T*>(this)->get_data();

            }

            void dump(void * dst){

                static_cast<T*>(this)->dump(dst);

            }

            ViewableBoolVector<T> * to_viewable(){

                return this;

            }

    };
    
    template <class T>
    class UnalignedOperatableBoolVector: public UnalignedViewableBoolVector<T>{

        public:

            void set_val(size_t idx, bool val){

                static_cast<T*>(this)->set_val(idx, val);
                
            }

            UnalignedOperatableBoolVector * to_unaligned_operatable(){

                return this;

            }

    };

    template <class T>
    class OperatableBoolVector : public UnalignedOperatableBoolVector<OperatableBoolVector<T>>, public ViewableBoolVector<T>{

        public:

            size_t length(){

                return static_cast<T*>(this)->length(); 

            }

            size_t byte_length(){

                return static_cast<T*>(this)->byte_length();

            }

            bool get(size_t idx){

                return static_cast<T*>(this)->get(idx);

            }

            template <class T1>
            void combine(ViewableBoolVector<T1>& data){

                static_cast<T*>(this)->combine(data);

            }

            template <class T1>
            void combine_w_provided_memcpy(ViewableBoolVector<T1>& data, algorithm::memory::memcpy_lambda& lambda){

                static_cast<T*>(this)->combine_w_provided_memcpy(data, lambda);

            }

            void add(bool val){

                static_cast<T*>(this)->add(val);

            }

            void clear(){

                static_cast<T*>(this)->clear();

            } 

            bool pop(){

                return static_cast<T*>(this)->pop();

            }

            void resize(size_t n){

                static_cast<T*>(this)->resize(n);

            }

            void reserve(size_t n){

                static_cast<T*>(this)->reserve(n);

            }

            UnalignedViewableBoolVector<T> * to_unaligned_viewable(){
                
                return ViewableBoolVector<T>::to_unaligned_viewable();

            }
    
    };

    template <class T>
    class UnalignedOffsetBoolVectorView: public UnalignedViewableBoolVector<UnalignedOffsetBoolVectorView<T>>{ // UnstableAddr

        private:

            UnalignedViewableBoolVector<T> * data;
            size_t offset;
            size_t sz;
        
        public:

            UnalignedOffsetBoolVectorView() = default;

            UnalignedOffsetBoolVectorView(UnalignedViewableBoolVector<T> * data, size_t offset, size_t sz){
                                
                this->data = data;
                this->offset = offset;
                this->sz = sz;

            }

            size_t length(){

                return this->sz;

            }

            size_t byte_length(){

                if (this->length() % 8 == 0){

                    return this->length() >> 3;

                }

                return (this->length() >> 3) + 1;

            }

            bool get(size_t idx){

                return this->data->get(idx + this->offset); 

            }
    
    };

    template <class T>
    class UnalignedOffsetBoolVectorOperator: public UnalignedOperatableBoolVector<UnalignedOffsetBoolVectorOperator<T>>{ // UnstableAddr

        private:

            UnalignedOperatableBoolVector<T> * data; 
            size_t offset;
            size_t sz;

        public:

            UnalignedOffsetBoolVectorOperator() = default;

            UnalignedOffsetBoolVectorOperator(UnalignedOperatableBoolVector<T> * data, size_t offset, size_t sz){
                
                this->data = data;
                this->offset = offset;
                this->sz = sz;

            }

            void set_val(size_t idx, bool val){

                this->data->set_val(idx + this->offset, val);

            }
            
            size_t length(){

                return this->sz;

            }

            size_t byte_length(){

                if (this->length() % 8 == 0){

                    return this->length() >> 3;

                }

                return (this->length() >> 3) + 1;

            }

            bool get(size_t idx){

                return this->data->get(idx + this->offset); 

            }

    };
    
    class UnalignedOffsetBoolVectorViewGenerator{

        public:

            template<class T>
            auto get(UnalignedViewableBoolVector<T>& data, size_t offset, size_t sz){ // -> UnalignedViewableBoolVector<>& UnstableAddr

                return UnalignedOffsetBoolVectorView<T>(&data, offset, sz); 

            }

    };

    class BoolVectorView : public ViewableBoolVector<BoolVectorView>{ // UnstableAddr

        public:
            
            char * data; 
            size_t size; 

        public:

            BoolVectorView(){
                
                this->data = nullptr;
                this->size = 0;

            }

            BoolVectorView(char * data, size_t size){

                this->data = data; 
                this->size = size;
                
            }
             
            size_t length(){

                return this->size;

            }

            size_t byte_length(){

                if (this->size % 8 == 0){

                    return this->size >> 3;

                }

                return (this->size >> 3)  + 1;

            }

            bool get(size_t idx){

                assert(idx < size);

                size_t offset = idx >> 3; 
                size_t offset_ = idx % 8;

                if (this->data[offset] & (1ULL << offset_)){

                    return true;

                }

                return false; 

            }

            void * get_data(){

                return this->data;

            }

            void dump(void * dst){
                
                if (this->size % 8 == 0){
                    
                    algorithm::memory::small_memcpy(dst, this->data, this->size >> 3);

                    return;

                }

                size_t trailing_spaces = 8 - this->size % 8;
                
                algorithm::memory::small_memcpy_w_trailing(dst, this->data, (this->size >> 3) + 1, trailing_spaces);
                
            }


    };

    class SmallBoolVectorView : public ViewableBoolVector<SmallBoolVectorView>{ // UnstableAddr

        public:
            
            char * data; 
            uint16_t size; 

        public:

            SmallBoolVectorView(){
                
                this->data = nullptr;
                this->size = 0;

            }

            SmallBoolVectorView(char * data, size_t size){

                this->data = data; 
                this->size = size;
                
            }
             
            size_t length(){

                return this->size;

            }

            size_t byte_length(){

                if (this->size % 8 == 0){

                    return this->size >> 3;

                }

                return (this->size >> 3)  + 1;

            }

            bool get(size_t idx){

                assert(idx < size);

                size_t offset = idx >> 3; 
                size_t offset_ = idx % 8;

                if (this->data[offset] & (1ULL << offset_)){

                    return true;

                }

                return false; 

            }

            void * get_data(){

                return this->data;

            }

            void dump(void * dst){
                
                if (this->size % 8 == 0){
                    
                    algorithm::memory::small_memcpy(dst, this->data, this->size >> 3);

                    return;

                }

                size_t trailing_spaces = 8 - this->size % 8;
                
                algorithm::memory::small_memcpy_w_trailing(dst, this->data, (this->size >> 3) + 1, trailing_spaces);
                
            }


    };

    class BoolVector: public OperatableBoolVector<BoolVector> {

        public:

            char * data; 

            size_t size;
            size_t buffer_length;
            
            inline static const uint8_t DEFAULT_CAPACITY = 5;

        public:

            BoolVector(){
                
                this->size = 0; 
                this->buffer_length = DEFAULT_CAPACITY;

                this->data = (char *) malloc(DEFAULT_CAPACITY);   
             
            }

            BoolVector(const BoolVector& obj){

                this->size = obj.size;
                this->buffer_length = obj.buffer_length;

                this->data = (char *) malloc(obj.buffer_length);
                memcpy(this->data, obj.data, obj.buffer_length * sizeof(char));

            }

            BoolVector(BoolVector&& obj){
                
                this->size = obj.size;
                this->buffer_length = obj.buffer_length;
                this->data = obj.data; 
                
                obj.data = nullptr;
                obj.size = 0;
                obj.buffer_length = 0;

            }

            template <class T>
            BoolVector(ViewableBoolVector<T> &obj){

                this->size = obj.length();
                this->buffer_length = obj.byte_length();

                this->data = (char *) malloc(this->buffer_length);

                obj.dump(this->data); 

            }

            ~BoolVector(){

                free(this->data);

            }

            BoolVector& operator = (const BoolVector& obj){
                
                free(this->data);

                this->size = obj.size;
                this->buffer_length = obj.buffer_length;

                this->data = (char *) malloc(this->buffer_length);
                memcpy(this->data, obj.data, this->buffer_length * sizeof(char));

                return (*this);

            };

            BoolVector& operator = (BoolVector&& obj){ 

                free(this->data);

                this->size = obj.size;
                this->buffer_length = obj.buffer_length;
                this->data = obj.data;

                obj.data = nullptr;

                return (*this);

            }

            bool get(size_t idx){

                assert(idx < this->size);

                size_t offset = idx >> 3; 
                uint8_t offset_ = idx % 8;

                if (this->data[offset] & (1ULL << offset_)){
                    return true; 
                }

                return false; 

            }

            void * get_data(){
                
                return (void *) this->data;

            }

            size_t length(){
                
                return this->size;

            }

            size_t byte_length(){
                
                if (this->size % 8 == 0){
                    
                    return this->size >> 3; 

                }

                return (this->size >> 3) + 1;

            }

            void set_val(size_t idx, bool val){

                assert(idx < this->size);

                size_t offset = idx >> 3; 
                size_t offset_ = idx % 8;
                
                if (val){

                    this->data[offset] |= 1ULL << offset_; 
                    
                    return;

                } 

                this->data[offset] &= (0xFF ^ (1ULL << offset_));

            }

            void add(bool val){
                
                if (this->size == this->get_capacity()){

                    this->reserve(this->get_capacity() * 2);

                }

                this->set_val(this->size++, val);

            }

            template <class T>
            void combine_w_provided_memcpy(ViewableBoolVector<T>& obj, algorithm::memory::memcpy_lambda &lambda){

                size_t bit_length = (this->byte_length() + obj.byte_length()) << 3;
                this->reserve(bit_length);
                
                if (this->length() % 8 == 0){

                    lambda(this->data + this->byte_length(), obj.get_data());
                    this->size += obj.length();

                    return; 

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++ i){

                    this->add(obj.get(i));

                }

            }

            template <class T>
            void combine(ViewableBoolVector<T>& obj){
                
                size_t bit_length = (this->byte_length() + obj.byte_length()) << 3;
                this->reserve(bit_length);
                
                if (this->length() % 8 == 0){

                    obj.dump(this->data + this->byte_length());
                    this->size += obj.length();

                    return; 

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++ i){

                    this->add(obj.get(i));

                }

            }

            void dump(void * dst){
                
                if (this->size % 8 == 0){
                    
                    algorithm::memory::small_memcpy(dst, this->data, this->size >> 3);

                    return;

                }

                size_t trailing_spaces = 8 - this->size % 8;
                
                algorithm::memory::small_memcpy_w_trailing(dst, this->data, (this->size >> 3) + 1, trailing_spaces);
                
            }

            void reserve(size_t size){

                if (this->get_capacity() >= size){

                    return;

                }   

                size_t new_buffer_length = size >> 3; 

                if (size % 8 != 0) ++new_buffer_length;

                char * new_data = (char *) malloc(new_buffer_length);
                memcpy(new_data, this->data, this->buffer_length);

                free(this->data);
                this->data = new_data;
                this->buffer_length = new_buffer_length;            

            }

            void resize(size_t size){
                
                if (size <= this->size){
                    
                    this->size = size;

                    return;

                }

                size_t start_idx = size; 
                this->size = size; 

                this->reserve(this->size);
                this->fill_blank(start_idx, this->size);

            }

            void clear(){

                this->size = 0;

            }

            bool pop(){
                
                assert(this->size);

                bool rs = this->get(this->size - 1);
                --this->size;

                return rs; 
            
            }

        private:

            size_t get_capacity(){
                
                return this->buffer_length << 3; 
                
            }

            void fill_blank(size_t start_idx, size_t end_idx_not_inclusive){

                while (start_idx % 8 != 0 && start_idx != end_idx_not_inclusive){

                    this->set_val(start_idx++, false);

                }

                if (start_idx == end_idx_not_inclusive) return; 

                size_t start_byte_idx = start_idx >> 3; 
                size_t delta = (end_idx_not_inclusive - start_idx) >> 3; 

                if ((end_idx_not_inclusive - start_idx) % 8 != 0) ++delta;

                memset(this->data + start_byte_idx, 0, delta * sizeof(char)); 

            }

    };

    class SmallBoolVector : public OperatableBoolVector<SmallBoolVector> {

        public:

            inline static const size_t CAPACITY = 160;
            inline static const size_t BUFFER_LENGTH = 20;

            char data[BUFFER_LENGTH]; 

            size_t size;

        public:

            template <class T>
            SmallBoolVector(ViewableBoolVector<T> &view){
                
                assert (view.length() <= CAPACITY);

                this->size = view.length();
                view.dump(data);

            }

            SmallBoolVector(){

                this->size = 0;

            }

            size_t length(){

                return this->size;
                
            }

            void * get_data(){

                return (void *) this->data;

            }
            
            size_t byte_length(){

                if (this->size % 8 == 0){
                    
                    return this->size >> 3; 

                }

                return (this->size >> 3) + 1;

            }

            bool get(size_t idx){

                assert(idx < this->length());

                size_t offset = idx >> 3;
                uint8_t offset_ = idx % 8;

                if (this->data[offset] & (1ULL << offset_)){

                    return true;

                }

                return false;

            }

            void dump(void * dst){
                
                if (this->size % 8 == 0){
                    
                    algorithm::memory::small_memcpy(dst, this->data, this->size >> 3);

                    return;

                }

                size_t trailing_spaces = 8 - this->size % 8;
                
                algorithm::memory::small_memcpy_w_trailing(dst, this->data, (this->size >> 3) + 1, trailing_spaces);
                
            }

            template <class T>
            void combine_w_provided_memcpy(ViewableBoolVector<T>& obj, algorithm::memory::memcpy_lambda &lambda){

                assert(this->size + obj.length() <= CAPACITY);

                if (this->size % 8 == 0){

                    lambda(this->data + this->byte_length(), obj.get_data());
                    this->size += obj.length();

                    return;

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++i){
                    
                    this->add(obj.get(i));

                }

            }

            void combine(ViewableBoolVector &obj){
                
                assert(this->size + obj.length() <= CAPACITY);

                if (this->size % 8 == 0){

                    obj.dump(this->data + this->byte_length());
                    this->size += obj.length();

                    return;

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++i){
                    
                    this->add(obj.get(i));

                }

            }

            void set_val(size_t idx, bool val){

                assert(idx < this->size);

                size_t offset = idx >> 3;
                uint8_t offset_ = idx % 8;

                if (val){
                    
                    this->data[offset] |= 1ULL << offset_; 

                    return;

                }

                this->data[offset] &= 0xFF ^ (1ULL << offset_);
                
            }

            void add(bool val){

                assert(this->size != CAPACITY);

                this->set_val(this->size++, val);
                
            }

            void clear(){

                this->size = 0; 

            }

            bool pop(){

                assert(this->size);

                bool rs = this->get(this->size - 1);
                --this->size;

                return rs; 
            
            }

            void resize(size_t size){
                
                if (size <= this->size){

                    this->size = size; 

                    return;

                }
                
                assert(size <= this->CAPACITY);

                size_t start_idx = size; 
                this->size = size; 

                this->fill_blank(start_idx, this->size);

            }

            void reserve(size_t size){

                assert(size <= this->CAPACITY);
                
            }


        private:

            void fill_blank(size_t start_idx, size_t end_idx_not_inclusive){

                while (start_idx % 8 != 0 && start_idx != end_idx_not_inclusive){

                    this->set_val(start_idx++, false);

                }

                if (start_idx == end_idx_not_inclusive) return; 

                size_t start_byte_idx = start_idx >> 3; 
                size_t delta = (end_idx_not_inclusive - start_idx) >> 3; 

                if ((end_idx_not_inclusive - start_idx) % 8 != 0) ++delta;

                memset(this->data + start_byte_idx, 0, delta * sizeof(char)); 

            }

    };

    class OptimizedBoolVector : public OperatableBoolVector<OptimizedBoolVector> {
        
        private: 

            SmallBoolVector small_bvec;
            std::unique_ptr<BoolVector> norm_bvec; 

        public:

            OptimizedBoolVector(){

                this->norm_bvec = nullptr;

            }

            OptimizedBoolVector(const OptimizedBoolVector& obj){

                this->small_bvec = obj.small_bvec;

                if (obj.norm_bvec){
                    
                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(*obj.norm_bvec));

                } else {

                    this->norm_bvec = nullptr;

                }

            }

            template <class T>
            OptimizedBoolVector(ViewableBoolVector<T>& obj){

                if (obj.length() <= SmallBoolVector::CAPACITY){

                    this->small_bvec = SmallBoolVector(obj);
                    this->norm_bvec = nullptr;

                }   else{

                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(obj));

                }

            }

            OptimizedBoolVector(OptimizedBoolVector&& obj){

                this->small_bvec = obj.small_bvec;
                this->norm_bvec = std::move(obj.norm_bvec);
            
            }

            OptimizedBoolVector& operator = (OptimizedBoolVector&& obj){
                
                if (obj.norm_bvec){
                    
                    this->norm_bvec = std::move(obj.norm_bvec);

                }

                this->small_bvec = obj.small_bvec;

                return *this;
                
            }

            bool get(size_t idx){

                if (this->norm_bvec){

                    return (*this->norm_bvec).get(idx);
                    
                }

                return this->small_bvec.get(idx);

            }

            void * get_data(){

                if (this->norm_bvec){

                    return (*this->norm_bvec).get_data();

                }

                return this->small_bvec.get_data();
                
            }

            size_t length(){

                if (this->norm_bvec){

                    return (*this->norm_bvec).length();

                }

                return this->small_bvec.length();

            }

            size_t byte_length(){

                if (this->norm_bvec){

                    return (*this->norm_bvec).byte_length();

                }

                return this->small_bvec.byte_length();

            }

            void set_val(size_t idx, bool val){

                if (this->norm_bvec){

                    (*this->norm_bvec).set_val(idx, val);

                    return;

                }

                this->small_bvec.set_val(idx, val);

            }

            void add(bool val){

                if (this->norm_bvec){

                    (*this->norm_bvec).add(val);

                    return;

                }

                if (this->small_bvec.length() == SmallBoolVector::CAPACITY){

                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(this->small_bvec));
                    (*this->norm_bvec).add(val);

                    return;

                }

                this->small_bvec.add(val);

            }

            template <class T>
            void combine(ViewableBoolVector<T> &obj){
                
                if (this->norm_bvec){

                    (*this->norm_bvec).combine(obj);

                    return;

                }

                if (this->small_bvec.length() + obj.length() > SmallBoolVector::CAPACITY){
                    
                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(this->small_bvec));
                    (*this->norm_bvec).combine(obj);

                    return;

                }

                this->small_bvec.combine(obj);

            }

            template <class T>
            void combine_w_provided_memcpy(ViewableBoolVector<T> &obj, algorithm::memory::memcpy_lambda &lambda){
                
                if (this->norm_bvec){

                    (*this->norm_bvec).combine_w_provided_memcpy(obj, lambda);

                    return;

                }

                if (this->small_bvec.length() + obj.length() > SmallBoolVector::CAPACITY){
                    
                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(this->small_bvec));
                    (*this->norm_bvec).combine_w_provided_memcpy(obj, lambda);

                    return;

                }

                this->small_bvec.combine_w_provided_memcpy(obj, lambda);

            }

            void dump(void * dst){

                if (this->norm_bvec){

                    (*this->norm_bvec).dump(dst);

                    return;

                }

                this->small_bvec.dump(dst);

            }

            void reserve(size_t size){

                if (this->norm_bvec){

                    (*this->norm_bvec).reserve(size);

                    return;

                }

                if (size > SmallBoolVector::CAPACITY){

                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(this->small_bvec));
                    (*this->norm_bvec).reserve(size);

                    return;

                }

            }

            void resize(size_t size){

                if (this->norm_bvec){

                    (*this->norm_bvec).resize(size);

                    return;

                }

                if (size > SmallBoolVector::CAPACITY){
                    
                    this->norm_bvec = std::unique_ptr<BoolVector>(new BoolVector(this->small_bvec));
                    (*this->norm_bvec).resize(size);
                    
                    return;

                }

                this->small_bvec.resize(size);

            }

            void clear(){

                if (this->norm_bvec){

                    (*this->norm_bvec).clear();

                    return;

                }

                this->small_bvec.clear();

            }

            bool pop(){
                
                if (this->norm_bvec){

                    return (*this->norm_bvec).pop();

                }

                return this->small_bvec.pop();

            }   
            
    };

    class PreAllocatedBoolVector: public OperatableBoolVector<PreAllocatedBoolVector>{
        
        public:

            char * data; 

            size_t size;
            size_t buffer_length;
            size_t capacity;

        public:

            PreAllocatedBoolVector(char * buffer, size_t buffer_length){

                this->data = buffer;
                this->buffer_length = buffer_length;
                this->capacity = buffer_length * 8;

                this->size = 0;

            }

            size_t length(){

                return this->size;
                
            }

            void reserve(size_t length){

                assert(length <= this->buffer_length);
                
            }

            size_t byte_length(){

                if (this->size % 8 == 0){
                    
                    return this->size >> 3; 

                }

                return (this->size >> 3) + 1;

            }

            bool get(size_t idx){

                assert(idx < this->length());

                size_t offset = idx >> 3;
                uint8_t offset_ = idx % 8;

                if (this->data[offset] & (1ULL << offset_)){

                    return true;

                }

                return false;

            }

            void * get_data(){

                return (void *) this->data;

            }

            void dump(void * dst){
                
                if (this->size % 8 == 0){
                    
                    algorithm::memory::small_memcpy(dst, this->data, this->size >> 3);

                    return;

                }

                size_t trailing_spaces = 8 - this->size % 8;
                
                algorithm::memory::small_memcpy_w_trailing(dst, this->data, (this->size >> 3) + 1, trailing_spaces);
                
            }

            template <class T>
            void combine_w_provided_memcpy(ViewableBoolVector<T>& obj, algorithm::memory::memcpy_lambda& lambda){
                                
                assert(this->size + obj.length() <= this->capacity);

                if (this->size % 8 == 0){

                    lambda(this->data + this->byte_length(), obj.get_data());
                    this->size += obj.length();

                    return;

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++i){
                    
                    this->add(obj.get(i));

                }

            }

            template <class T>
            void combine(ViewableBoolVector<T>& obj){
                                
                assert(this->size + obj.length() <= this->capacity);

                if (this->size % 8 == 0){

                    obj.dump(this->data + this->byte_length());
                    this->size += obj.length();

                    return;

                }

                size_t i = 0; 

                for (i = 0; i < obj.length(); ++i){
                    
                    this->add(obj.get(i));

                }

            }

            void set_val(size_t idx, bool val){

                assert(idx < this->size);

                size_t offset = idx >> 3;
                uint8_t offset_ = idx % 8;

                if (val){
                    
                    this->data[offset] |= 1ULL << offset_; 

                    return;

                }

                this->data[offset] &= 0xFF ^ (1ULL << offset_);
                
            }

            void add(bool val){

                assert(this->size != this->capacity);

                this->set_val(this->size++, val);
                
            }

            void clear(){

                this->size = 0; 

            }

            bool pop(){

                assert(this->size);

                bool rs = this->get(this->size - 1);
                --this->size;

                return rs; 
            
            }

            void resize(size_t size){
                
                if (size <= this->size){

                    this->size = size; 

                    return;

                }
                
                assert(size <= this->capacity);

                size_t start_idx = size; 
                this->size = size; 

                this->fill_blank(start_idx, this->size);

            }

        private:

            void fill_blank(size_t start_idx, size_t end_idx_not_inclusive){

                while (start_idx % 8 != 0 && start_idx != end_idx_not_inclusive){

                    this->set_val(start_idx++, false);

                }

                if (start_idx == end_idx_not_inclusive) return; 

                size_t start_byte_idx = start_idx >> 3; 
                size_t delta = (end_idx_not_inclusive - start_idx) >> 3; 

                if ((end_idx_not_inclusive - start_idx) % 8 != 0) ++delta;

                memset(this->data + start_byte_idx, 0, delta * sizeof(char)); 

            }
    };

    template <class T>
    class StableUnalignedViewableBoolVectorContainer: public UnalignedViewableBoolVector<StableUnalignedViewableBoolVectorContainer<T>>{

        private:

            T data;
        
        public:
            
            StableUnalignedViewableBoolVectorContainer() = default;

            StableUnalignedViewableBoolVectorContainer(T obj){
                
                this->data = obj;
               
            }

            size_t length(){

                return this->data.length(); 

            }

            size_t byte_length(){

                return this->data.byte_length();

            }

            bool get(size_t idx){

                return this->data.get(idx);

            }

    };

    template <class T>
    class StableUnalignedOperatableBoolVectorContainer: public UnalignedOperatableBoolVector<StableUnalignedOperatableBoolVectorContainer<T>>{

        private:

            T data;
        
        public:
          
            StableUnalignedOperatableBoolVectorContainer() = default;

            StableUnalignedOperatableBoolVectorContainer(T obj){
                
                this->data = obj;
               
            }

            size_t length(){

                return this->data.length(); 

            }

            size_t byte_length(){

                return this->data.byte_length();

            }

            bool get(size_t idx){

                return this->data.get(idx);

            }

            void set_val(size_t idx, bool val){

                this->data.set_val(idx, val);

            }

    };
}

namespace utility::boolvector{
    
    template <class T>
    class BoolVectorViewTrimmable{
        
        public:

            template <class T1>
            auto trim(datastructure::UnalignedViewableBoolVector<T1>& data){

                return static_cast<T*>(this)->trim(data);

            }

            template <class T1>
            std::shared_ptr<BoolVectorViewTrimmable<T>> to_boolvector_view_trimmable_sp(std::shared_ptr<T1> data){

                return std::dynamic_pointer_cast<BoolVectorViewTrimmable<T>>(data); 

            }


    };
    
    template <class T>
    class BoolVectorOperatorTrimmable{

        public:

            template <class T1>
            auto trim(datastructure::UnalignedOperatableBoolVector<T1>& data){

                return static_cast<T*>(this)->trim(data);

            }

    };

    template <class T>
    class StableBoolVectorViewTrimmable{
        
        public:

            template <class T1>
            auto trim(T1 data){

                return static_cast<T*>(this)->trim(data);

            }

            template <class T1>
            std::shared_ptr<StableBoolVectorViewTrimmable<T>> to_stable_boolvector_view_trimmable_sp(std::shared_ptr<T1> data){

                return std::dynamic_pointer_cast<StableBoolVectorViewTrimmable<T>>(data); 

            }


    };

    template <class T>
    class StableBoolVectorOperatorTrimmable{

        public:


            template <class T1>
            auto trim(T1 data){

                return static_cast<T*>(this)->trim(data);

            }

            template <class T1>
            std::shared_ptr<StableBoolVectorOperatorTrimmable<T>> to_stable_boolvector_operator_trimmable_sp(std::shared_ptr<T1> data){

                return std::dynamic_pointer_cast<StableBoolVectorOperatorTrimmable<T>>(data); 

            }
    };

    template <class ID>
    class RightZeroViewTrimmer: public BoolVectorViewTrimmable<RightZeroViewTrimmer<ID>>{ // -> return unstable addr obj

        public:

            template <class T>
            datastructure::UnalignedOffsetBoolVectorView<T> trim(datastructure::UnalignedViewableBoolVector<T>& data){
                
                size_t artificial_length = data.length();

                while (artificial_length != 0 && !data.get(artificial_length - 1)){

                    --artificial_length;

                } 

                return datastructure::UnalignedOffsetBoolVectorView<T>(data.to_unaligned_viewable(), 0, artificial_length);

            }


    };

    template <class ID>
    class RightZeroOperatorTrimmer: public BoolVectorOperatorTrimmable<RightZeroOperatorTrimmer<ID>>{ // -> return unstable addr obj

        public:
            
            template <class T>
            datastructure::UnalignedOffsetBoolVectorOperator<T> trim(datastructure::UnalignedOperatableBoolVector<T>& data){

                size_t artificial_length = data.length();

                while (artificial_length != 0 && !data.get(artificial_length - 1)){

                    --artificial_length;

                } 

                return datastructure::UnalignedOffsetBoolVectorOperator<T>(&data, 0, artificial_length);

            }
    };
    
    template <class ID>
    class StableRightZeroViewTrimmer: public StableBoolVectorViewTrimmable<StableRightZeroViewTrimmer<ID>> {

        public:

            template <class T>
            auto trim(T data){
                
                size_t artificial_length = data.length();

                while (artificial_length != 0 && !data.get(artificial_length - 1)){

                    --artificial_length;

                } 

                return datastructure::StableUnalignedViewableBoolVectorContainer<T>(data);

            }

    };

    template <class ID>
    class StableRightZeroOperatorTrimmer: public StableBoolVectorOperatorTrimmable<StableRightZeroOperatorTrimmer<ID>> {

        public:

            template <class T>
            auto trim(T data){
                
                size_t artificial_length = data.length();

                while (artificial_length != 0 && !data.get(artificial_length - 1)){

                    --artificial_length;

                } 

                return datastructure::StableUnalignedOperatableBoolVectorContainer<T>(data);

            }

    };

    class GenericID{};

    class StandardGenerator{

        public:

            auto get_right_zero_view_trimmer(){

                return std::shared_ptr<RightZeroViewTrimmer<GenericID>>(new RightZeroViewTrimmer<GenericID>());

            }

            auto get_right_zero_operator_trimmer(){

                return std::shared_ptr<RightZeroOperatorTrimmer<GenericID>>(new RightZeroOperatorTrimmer<GenericID>());
                
            }

            auto get_right_zero_view_stable_trimmer(){

                return std::shared_ptr<StableRightZeroViewTrimmer<GenericID>>(new StableRightZeroViewTrimmer<GenericID>());

            }

            auto get_right_zero_operator_stable_trimmer(){

                return std::shared_ptr<StableRightZeroOperatorTrimmer<GenericID>>(new StableRightZeroOperatorTrimmer<GenericID>());

            }

    };

    class IDGenerator{

        public:

            template <class ID>
            auto get_right_zero_view_trimmer(ID){

                return std::shared_ptr<RightZeroViewTrimmer<ID>>(new RightZeroViewTrimmer<ID>());

            }

            template <class ID>
            auto get_right_zero_operator_trimmer(ID){

                return std::shared_ptr<RightZeroOperatorTrimmer<ID>>(new RightZeroOperatorTrimmer<ID>());
                
            }

            template <class ID>
            auto get_right_zero_view_stable_trimmer(ID){

                return std::shared_ptr<StableRightZeroViewTrimmer<ID>>(new StableRightZeroViewTrimmer<ID>());

            }

            template <class ID>
            auto get_right_zero_operator_stable_trimmer(ID){

                return std::shared_ptr<StableRightZeroOperatorTrimmer<ID>>(new StableRightZeroOperatorTrimmer<ID>());

            }

    };

};

#endif