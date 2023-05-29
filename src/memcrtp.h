
#ifndef __LINEAR_MEM_H__
#define __LINEAR_MEM_H__

#include "boolvectorcrtp.h"
#include <vector>
#include <memory>
#include <iostream>
#include <atomic>

namespace memory{
    
    namespace linear{

        template<class T>
        class Allocatable{

            public:

                void * malloc(size_t n){

                    return static_cast<T*>(this)->malloc(n);

                }

                void free(void * ptr){

                    static_cast<T*>(this)->free(ptr);

                }

                template <class T1>
                std::shared_ptr<Allocatable<T>> to_allocatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Allocatable<T>>(data); 

                }

        };

        template <class T>
        class AlignedAllocatable: public Allocatable<T>{

            public:

                void * malloc(size_t n, size_t align_n){

                    return static_cast<T*>(this)->malloc(n, align_n);

                }

                template <class T1>
                std::shared_ptr<AlignedAllocatable<T>> to_aligned_allocatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<AlignedAllocatable<T>>(data); 

                }

        };

        template <class T>
        class AllocatorSingleton{

            private:

                static inline std::shared_ptr<Allocatable<T>> instance = nullptr;

            public:

                static std::shared_ptr<Allocatable<T>> get(){
                    
                    if (AllocatorSingleton<T>::instance == nullptr){
                        
                        AllocatorSingleton<T>::instance = std::make_shared<T>();
                        
                    }

                    return AllocatorSingleton<T>::instance;

                }

        };

        template<class T>
        class AlignmentAdjustable{

            public:

                size_t get_true_size(size_t n, size_t align_n){

                    return static_cast<T*>(this)->get_true_size(n, align_n);

                } 

                void * align(void * buf, size_t align_n){

                    return static_cast<T*>(this)->align(buf, align_n); 

                }

        };

        template<class T>
        class TempStorageGeneratable{

            public:

                void enter_scope(){

                    static_cast<T*>(this)->enter_scope();

                }
                
                void exit_scope(bool shrinked = true){

                    static_cast<T*>(this)->exit_scope(shrinked);

                }

                void * exit_scope(void * ptr, size_t n, bool shrinked = true){

                    return static_cast<T*>(this)->exit_scope(ptr, n, shrinked); 

                }

                void * get(size_t n){

                    return static_cast<T*>(this)->get(n);

                }               
              
                void shrink(){

                    static_cast<T*>(this)->shrink();

                }

        };

        template <class T>
        class AlignedTempStorageGeneratable: public TempStorageGeneratable<T>{

            public:

                void * exit_scope_align(void * ptr, size_t n, size_t align_n, bool shrinked = true){

                    return static_cast<T*>(this)->exit_scope_align(ptr, n, align_n, shrinked); 

                }

                void * get_align(size_t buf_n, size_t align_n){

                    return static_cast<T*>(this)->get_align(buf_n, align_n);

                } 

        };

        template <class T>
        class MainScopeAllocatableTempStorageGeneratable: public TempStorageGeneratable<T>, public Allocatable<T>{};

    };

    namespace sizet_linear{
        
        template <class T>
        class VectorReadable{

            public:

                size_t get(size_t idx){

                    return static_cast<T*>(this)->get(idx); 

                }

                size_t length(){

                    return static_cast<T*>(this)->length();

                } 

                void * get_data(){

                    return static_cast<T*>(this)->get_data();

                }

                size_t sizeof_slot(){

                    return sizeof(size_t);

                }

                VectorReadable<T> * to_vector_readable(){

                    return this;

                }

        };

        template <class T>
        class StackAllocatedVectorReadable: public VectorReadable<T>{};

        template <class T>
        class OperatableVector: public VectorReadable<T>{

            public:

                void set(size_t idx, size_t val){

                    static_cast<T*>(this)->set(idx, val);

                }

                OperatableVector<T> * to_operatable_vector(){

                    return this;

                }


        };

        template <class T>
        class StackAllocatedOperatableVector: public OperatableVector<StackAllocatedOperatableVector<T>>, public StackAllocatedVectorReadable<T>{

            public:

                size_t get(size_t idx){

                    return static_cast<T*>(this)->get(idx); 

                }

                size_t length(){

                    return static_cast<T*>(this)->length();

                } 

                void * get_data(){

                    return static_cast<T*>(this)->get_data();

                }

                size_t sizeof_slot(){

                    return StackAllocatedVectorReadable<T>::sizeof_slot();

                }

                VectorReadable<T> * to_vec_readable(){

                    return this;
                    
                }

        };

        template <class T>
        class AddableVector: public OperatableVector<T>{

            public:

                void add(size_t data){

                    static_cast<T*>(this)->add(data);

                }

                AddableVector * to_addable_vector(){
                    
                    return this;

                }

        };

        template <class T>
        class ReallocatableOperatableVector: public OperatableVector<T>{

            public:

                void resize(size_t n){

                    static_cast<T*>(this)->resize(n);

                }

                void resize_no_copy(size_t n){

                    static_cast<T*>(this)->resize_no_copy(n);

                }

                ReallocatableOperatableVector<T> * to_reallocatable_operatable_vector(){

                    return this;

                }

        };

        template <class T>
        class TempStorageGeneratable: public linear::TempStorageGeneratable<T>{

            public:

                using linear::TempStorageGeneratable<T>::exit_scope;

                template <class T1>
                auto exit_scope(VectorReadable<T1>& data){ // -> VectorReadable&

                    return static_cast<T*>(this)->exit_scope(data); 

                }

                template <class T1>
                auto exit_scope_no_shrink(VectorReadable<T1>& data){ // -> VectorReadable&

                    return static_cast<T*>(this)->exit_scope_no_shrink(data); 

                }

                auto vec_get(size_t n){ // -> OperatableVector&

                    return static_cast<T*>(this)->vec_get(n);

                } 

                auto vec_get_default(size_t n){ // -> OperatableVector&

                    return static_cast<T*>(this)->vec_get_default(n);

                }
                
                template <class T1>
                std::shared_ptr<TempStorageGeneratable<T>> to_temp_storage_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<TempStorageGeneratable<T>>(data); 

                }

        };

        template<class T>
        class DanglingOperatableVectorGeneratable{

            public:

                auto from_ptr(void * ptr, size_t n){ // ->OperatableVector&

                    return static_cast<T*>(this)->from_ptr(ptr, n);

                }

                template <class T1>
                std::shared_ptr<DanglingOperatableVectorGeneratable<T>> to_dangling_operatable_vector_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<DanglingOperatableVectorGeneratable<T>>(data);

                }

        };
        
        template<class T>
        class VectorReadableGeneratable{

            public:

                template <class T1>
                auto from_vec_readable(VectorReadable<T1>& data){ // -> VectorReadable<>&

                    return static_cast<T*>(this)->from_vec_readable(data);

                } 

                auto from_ptr(void * ptr, size_t n){ // -> VectorReadable<>&

                    return static_cast<T*>(this)->from_ptr(ptr, n);

                }

                template <class T1>
                std::shared_ptr<VectorReadableGeneratable<T>> to_vector_readable_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<VectorReadableGeneratable<T>>(data); 

                }

        };

        template <class T>
        class StackVectorReadableGeneratable: public VectorReadableGeneratable<T>{

            public:

                using VectorReadableGeneratable<T>::from_vec_readable; 

                template <class T1>
                auto from_vec_readable(StackAllocatedVectorReadable<T1>& data){

                    return static_cast<T*>(this)->from_vec_readable(data); 

                }

                template <class T1>
                std::shared_ptr<StackVectorReadableGeneratable<T>> to_stack_vector_readable_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<StackVectorReadableGeneratable<T>>(data); 

                }

        };

        template <class T>
        class AddableVectorGeneratable{

            public:

                auto get(){ //AddableVector&

                    return static_cast<T*>(this)->get(); 

                }

                template<class T1>
                std::shared_ptr<AddableVectorGeneratable<T>> to_addable_vector_generatable(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<AddableVectorGeneratable<T>>(data); 

                }

        };

        template<class T>
        class VectorReadableSplittable{

            public:

                template <class T1>
                auto split(VectorReadable<T1>& data, size_t n){ // -> std::pair<VectorReadable&, VectorReadable&> 

                    return static_cast<T*>(this)->split(data, n);

                }

        };

        template<class T>
        class DynamicBitIterable{
            
            public:

                bool next(size_t& data, size_t read_sz){

                    return static_cast<T*>(this)->next(data, read_sz);

                }

        };

        template<class T>
        class DynamicBitIteratorGeneratable{

            public:

                template <class T1>
                auto get(VectorReadable<T1>& data){ // -> DynamicBitIterable<>&

                    return static_cast<T*>(this)->get(data); 

                }

                template <class T1>
                auto get(VectorReadable<T1>& data, size_t offset){ // -> DynamicBitIterable<>&

                    return static_cast<T*>(this)->get(data, offset);

                }

                template <class T1>
                std::shared_ptr<DynamicBitIteratorGeneratable<T>> to_dynamic_bit_iterator_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<DynamicBitIteratorGeneratable<T>>(data); 

                } 


        };
        
        template<class T>
        class BitIterable{

            public:

                bool next(size_t& data){

                    return static_cast<T*>(this)->next(data);

                }

        };

        template<class T>
        class BitIteratorGeneratable{

            public:

                template <class T2>
                auto get(VectorReadable<T2>& data, size_t n){ // -> BitIterable&

                    return static_cast<T*>(this)->get(data, n);

                } 

                template <class T2>
                std::shared_ptr<BitIteratorGeneratable<T>> to_bit_iterator_generatable_sp(std::shared_ptr<T2> data){

                    return std::dynamic_pointer_cast<BitIteratorGeneratable<T>>(data);

                }

        };
        
        template<class T>
        class DynamicBitIterWritable{

            public:

                bool write(size_t data, size_t write_sz){

                    return static_cast<T*>(this)->write(data, write_sz);

                }

        };

        template<class T>
        class BitIterWritable{

            public:

                bool write(size_t data){

                    return static_cast<T*>(this)->write(data); 

                }

        };

        template<class T>
        class DynamicBitIterWriterGeneratable{

            public:

                template<class T2>
                auto get(OperatableVector<T2>& data, size_t offset){ // -> DynamicBitIterWritable&

                    return static_cast<T*>(this)->get(data, offset);

                } 
                
                template <class T2>
                std::shared_ptr<DynamicBitIterWriterGeneratable<T>> to_dynamic_bit_iter_writer_generatable_sp(std::shared_ptr<T2> data){

                    return std::dynamic_pointer_cast<DynamicBitIterWriterGeneratable<T>>(data);

                }


        };

        template<class T>
        class BitIterWriterGeneratable{

            public:

                template<class T2>
                auto get(OperatableVector<T2>& data , size_t sz){ // -> BitIterWritable&

                    return static_cast<T*>(this)->get(data, sz);

                } 
                
        };

        namespace caster{
            
            template<class T>
            class BoolVectorViewCastable{

                public:

                    template<class T1>
                    auto cast(VectorReadable<T1>& data){ // -> datastructure::UnalignedViewableBoolVector&

                        return static_cast<T*>(this)->cast(data);

                    } 

            };

            template<class T>
            class BoolVectorOpCastable{

                public:

                    template<class T1>
                    auto cast(OperatableVector<T1>& data){ // -> datastructure::UnalignedOperatableBoolVector

                        return static_cast<T*>(this)->cast(data);

                    }

            };

        };

    };

}

namespace memory::linear{

    class StandardAlignmentAdjuster: public AlignmentAdjustable<StandardAlignmentAdjuster>{

        public:

            size_t get_true_size(size_t n, size_t alignment){

                return n + alignment;

            }

            void * align(void * buf, size_t alignment){

                size_t mod = (uintptr_t) buf % alignment;
                size_t dif = alignment - mod;

                return (char *) buf + dif;

            }

    };

    class ExponentialLinearStorageBlockGenerator: public TempStorageGeneratable<ExponentialLinearStorageBlockGenerator>{

        protected:

            std::vector<void *> buf; 
            std::vector<std::pair<size_t, size_t>> enter_points; 
            
            size_t slot; 
            size_t offset;
            size_t base_length;

        public:

            ExponentialLinearStorageBlockGenerator() = default;

            ExponentialLinearStorageBlockGenerator(size_t base_length){

                this->base_length = base_length;
                this->slot = 0;
                this->offset = 0;

                this->enter_points = {};
                this->buf = {};

            }

            ExponentialLinearStorageBlockGenerator(ExponentialLinearStorageBlockGenerator&) = delete;
      
            ExponentialLinearStorageBlockGenerator(ExponentialLinearStorageBlockGenerator&& obj){

                *this = std::move(obj);
                
            }

            ~ExponentialLinearStorageBlockGenerator(){
                
                assert(this->enter_points.size() == 0);

            }

            ExponentialLinearStorageBlockGenerator& operator = (ExponentialLinearStorageBlockGenerator&) = delete;

            ExponentialLinearStorageBlockGenerator& operator = (ExponentialLinearStorageBlockGenerator&& obj){

                this->buf = obj.buf;
                this->enter_points = obj.enter_points;
                this->slot = obj.slot;
                this->offset = obj.offset;
                this->base_length = obj.base_length;

                obj.buf = {};
                obj.enter_points = {};
                obj.base_length = 0;
                obj.slot = 0;
                obj.offset = 0;

                return *this;

            } 

            void enter_scope(){

                this->enter_points.push_back(std::pair<size_t, size_t>(this->slot, this->offset));

            }

            void exit_scope(bool shrinked = true){

                this->slot = this->enter_points.back().first;
                this->offset = this->enter_points.back().second;

                this->enter_points.pop_back();

                if (shrinked){

                    this->shrink_buf(this->slot + 1);

                }

            }

            void * exit_scope(void * data, size_t sz, bool shrinked = true){

                this->exit_scope(false);
                void * cp_data = this->get(sz);

                if (cp_data != data){

                    std::memcpy(cp_data, data, sz);

                }

                if (shrinked){

                    this->shrink_buf(this->slot + 1);

                }

                return cp_data;

            }

            void * get(size_t sz){
                                
                std::pair<size_t, size_t> point = this->reserve(this->slot, this->offset, sz); 
                void * rs = (char *) this->buf[point.first] + point.second;

                this->slot = point.first;
                this->offset = point.second + sz;
                
                return rs;

            }   

            void shrink(){

                this->shrink_buf(this->slot + 1);

            }

        private:

            void shrink_buf(size_t n){

                while (this->buf.size() > n){

                    std::free(this->buf.back());
                    this->buf.pop_back();

                }

            }

            void allocate_buf(size_t n){

                while (this->buf.size() < n){

                    this->buf.push_back(std::malloc(this->get_slot_length(this->buf.size()) * sizeof(char)));

                }

            }

            std::pair<size_t, size_t> reserve(size_t slot, size_t offset, size_t sz){

                while ((offset + sz) > this->get_slot_length(slot)){

                    ++slot;
                    offset = 0;

                }

                this->allocate_buf(slot + 1);

                return std::pair<size_t, size_t>(slot, offset); 

            }
           
            size_t get_slot_length(size_t slot){

                return this->base_length << slot;

            }

    };

    class AlignedExponentialLinearStorageBlockGenerator: public AlignedTempStorageGeneratable<AlignedExponentialLinearStorageBlockGenerator>{

        private:

            StandardAlignmentAdjuster alignment_adjuster; 
            ExponentialLinearStorageBlockGenerator gen; 

        public:

            AlignedExponentialLinearStorageBlockGenerator() = default;
            AlignedExponentialLinearStorageBlockGenerator(size_t base_length){
               
                this->gen = ExponentialLinearStorageBlockGenerator(base_length);

            }

            AlignedExponentialLinearStorageBlockGenerator(AlignedExponentialLinearStorageBlockGenerator&) = delete;
            AlignedExponentialLinearStorageBlockGenerator(AlignedExponentialLinearStorageBlockGenerator&&) = default;
            ~AlignedExponentialLinearStorageBlockGenerator() = default;
            AlignedExponentialLinearStorageBlockGenerator& operator = (AlignedExponentialLinearStorageBlockGenerator&) = delete;
            AlignedExponentialLinearStorageBlockGenerator& operator = (AlignedExponentialLinearStorageBlockGenerator&&) = default;

            void enter_scope(){

                this->gen.enter_scope();

            }

            void exit_scope(bool shrinked = true){

                this->gen.exit_scope(shrinked);

            }

            void * exit_scope(void * data, size_t sz, bool shrinked = true){

                return this->gen.exit_scope(data, sz, shrinked);

            }

            void * get(size_t sz){

                return this->gen.get(sz);

            }

            void shrink(){

                this->gen.shrink();

            }

            void * get_align(size_t sz, size_t alignment){

                size_t true_size = this->alignment_adjuster.get_true_size(sz, alignment);
                void * rs = this->gen.get(true_size); 

                return this->alignment_adjuster.align(rs, alignment);

            }

            void * exit_scope_align(void * data, size_t sz, size_t alm, bool shrinked = true){
                
                this->gen.exit_scope(false);
                void * cp_data = this->get_align(sz, alm);

                if (cp_data != data){

                    std::memcpy(cp_data, data, sz);

                }

                if (shrinked){

                    this->gen.shrink();

                }

                return cp_data;

            }

    };

    class StackAllocator: public Allocatable<StackAllocator>{

        private:

            ExponentialLinearStorageBlockGenerator allocator;
            const size_t base_length = 8192; 

        public:

            StackAllocator(){
                
                this->allocator = ExponentialLinearStorageBlockGenerator(base_length);
                this->allocator.enter_scope();

            }

            ~StackAllocator(){

                this->allocator.exit_scope();

            }

            StackAllocator(StackAllocator&) = delete;
            
            StackAllocator(StackAllocator&& obj){

                *this = std::move(obj);

            }

            StackAllocator& operator = (StackAllocator&) = delete;

            StackAllocator& operator = (StackAllocator&& obj){

                this->allocator = std::move(obj.allocator);

                return *this;

            }

            void * malloc(size_t n){

                return this->allocator.get(n); 

            }

            void free(void *){}

    };

    class AlignedStackAllocator: public AlignedAllocatable<AlignedStackAllocator>{

        private:

            AlignedExponentialLinearStorageBlockGenerator allocator;
            const size_t base_length = 8192;
        
        public:

            AlignedStackAllocator(){

                this->allocator = AlignedExponentialLinearStorageBlockGenerator(base_length);
                this->allocator.enter_scope();

            }

            AlignedStackAllocator(AlignedStackAllocator&) = delete;
            AlignedStackAllocator(AlignedStackAllocator&&) = default;

            ~AlignedStackAllocator(){

                this->allocator.exit_scope();

            }

            AlignedStackAllocator& operator = (AlignedStackAllocator&) = delete;
            AlignedStackAllocator& operator = (AlignedStackAllocator&&) = default;

            void * malloc(size_t n){

                return allocator.get(n); 

            }

            void * malloc(size_t n, size_t align_n){

                return allocator.get_align(n, align_n);

            }

            void free(void *){}


    };

}

namespace memory::pointer{
    
    class ThreadSafeStackAllocator: public memory::linear::AlignedAllocatable<ThreadSafeStackAllocator>{

        private:

            memory::linear::AlignedStackAllocator allocator;
            
            #if defined(DG_MULTI_THREAD)
            std::mutex mtx;
            #endif

        public:

            ThreadSafeStackAllocator() = default;
            ~ThreadSafeStackAllocator() = default;
            ThreadSafeStackAllocator(ThreadSafeStackAllocator&) = delete;
            ThreadSafeStackAllocator(ThreadSafeStackAllocator&&) = default;
            ThreadSafeStackAllocator& operator = (ThreadSafeStackAllocator&) = delete;
            ThreadSafeStackAllocator& operator = (ThreadSafeStackAllocator&&) = default;

            void * malloc(size_t n){

                #if defined(DG_MULTI_THREAD)
                std::lock_guard<std::mutex> grd(this->mtx);
                #endif

                return this->allocator.malloc(n);

            }

            void * malloc(size_t n, size_t align_n){
                
                #if defined(DG_MULTI_THREAD)
                std::lock_guard<std::mutex> grd(this->mtx);
                #endif
                
                return this->allocator.malloc(n, align_n);

            }

            void free(void *){}

    };

    class StdAllocator: public memory::linear::AlignedAllocatable<StdAllocator>{

        public:

            void * malloc(size_t n){

               return std::malloc(n);

            }

            void * malloc(size_t n, size_t align_n){
                
                return std::malloc(n);

            }

            void free(void * ptr){

                std::free(ptr);

            }
            
    };

    template <class T, class T1>
    class custom_allocator_shared_ptr{

        private:

            template <class U, class U1>
            friend class custom_allocator_shared_ptr;

            T * containee;
            std::atomic<size_t> * counter;
            
        public:

            custom_allocator_shared_ptr(T * containee, std::atomic<size_t> * counter){ //containee & counter have to be generated by T1 malloced and placement new

                this->containee = containee;
                this->counter = counter;
                
            }

            custom_allocator_shared_ptr(){

                this->containee = nullptr;
                void * plm_space = memory::linear::AllocatorSingleton<T1>::get()->malloc(sizeof(std::atomic<size_t>));
                this->counter = new (plm_space) std::atomic<size_t>(1); 

            } 

            custom_allocator_shared_ptr(std::nullptr_t){

                this->containee = nullptr;
                void * plm_space = memory::linear::AllocatorSingleton<T1>::get()->malloc(sizeof(std::atomic<size_t>));
                this->counter = new (plm_space) std::atomic<size_t>(1); 

            }

            custom_allocator_shared_ptr(T * containee){ //containee has to be generated by T1 malloced and placement new

                this->containee = containee;
                void * plm_space = memory::linear::AllocatorSingleton<T1>::get()->malloc(sizeof(std::atomic<size_t>));
                this->counter = new (plm_space) std::atomic<size_t>(1);

            }

            custom_allocator_shared_ptr(const custom_allocator_shared_ptr<T, T1>& obj){
                
                this->containee = obj.containee;
                this->counter = obj.counter;
                ++(*this->counter);

            }

            custom_allocator_shared_ptr(custom_allocator_shared_ptr<T, T1>&& obj){
                
                this->containee = obj.containee;
                this->counter = obj.counter;
                ++(*this->counter);

            }

            template <class T2>
            custom_allocator_shared_ptr(const custom_allocator_shared_ptr<T2, T1>& obj){

                this->containee = static_cast<T*>(obj.containee);
                this->counter = obj.counter;
                ++(*this->counter);

            }

            template <class T2>
            custom_allocator_shared_ptr(custom_allocator_shared_ptr<T2, T1>&& obj){

                this->containee = static_cast<T*>(obj.containee);
                this->counter = obj.counter;
                ++(*this->counter);

            }

            custom_allocator_shared_ptr& operator = (const custom_allocator_shared_ptr<T, T1>& obj){

                this->~custom_allocator_shared_ptr();

                this->containee = static_cast<T*>(obj.containee);
                this->counter = obj.counter;
                ++(*this->counter);

                return *this;

            }

            custom_allocator_shared_ptr& operator = (custom_allocator_shared_ptr<T, T1>&& obj){

                this->~custom_allocator_shared_ptr();
                
                this->containee = static_cast<T*>(obj.containee);
                this->counter = obj.counter;
                ++(*this->counter);
                return *this;

            } 

            custom_allocator_shared_ptr& operator = (std::nullptr_t){
                
                this->~custom_allocator_shared_ptr();
                
                this->containee = nullptr;
                void * plm_space = memory::linear::AllocatorSingleton<T1>::get()->malloc(sizeof(std::atomic<size_t>));
                this->counter = new (plm_space) std::atomic<size_t>(1); 
                
                return *this;

            } 

            ~custom_allocator_shared_ptr(){

                if (--(*this->counter) == 0){
                    
                    if (this->containee != nullptr){
                        
                        this->containee->~T();
                        memory::linear::AllocatorSingleton<T1>::get()->free(this->containee);

                    }

                    this->counter->~atomic<size_t>();
                    memory::linear::AllocatorSingleton<T1>::get()->free(this->counter);

                }


            }

            T& operator *(){

                return *this->containee;

            }

            T* operator ->(){

                return this->containee;

            }

            std::atomic<size_t> * get_counter(){

                return this->counter;

            }

            bool operator == (std::nullptr_t){

                return this->containee == nullptr; 

            }

    };

    template <class T>
    class stack_shared_ptr: public custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>{

        private:
            
            template <class U>
            friend class stack_shared_ptr;

        public:

            stack_shared_ptr() = default;

            stack_shared_ptr(std::nullptr_t): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(nullptr){}

            stack_shared_ptr(custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>& ptr): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(ptr){}

            stack_shared_ptr(custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>&& ptr): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(std::move(ptr)){}

            stack_shared_ptr(T * containee): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(containee){}

            stack_shared_ptr(const stack_shared_ptr<T>& obj): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(obj){}

            stack_shared_ptr(stack_shared_ptr<T>&& obj): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(std::move(obj)){}

            template <class T1>
            stack_shared_ptr(const stack_shared_ptr<T1>& obj): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(obj){}

            template <class T1>
            stack_shared_ptr(stack_shared_ptr<T1>&& obj): custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>(std::move(obj)){}

            stack_shared_ptr<T>& operator = (const stack_shared_ptr<T>& obj){

                custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>::operator=(obj);

                return *this;

            }

            stack_shared_ptr<T>& operator = (stack_shared_ptr<T>&& obj){
                
                custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>::operator=(std::move(obj));
                
                return *this;

            }

            stack_shared_ptr<T>& operator = (std::nullptr_t){
                
                custom_allocator_shared_ptr<T, ThreadSafeStackAllocator>::operator=(nullptr);
                
                return *this;

            }

    };

    template <class T, class T1, class T2>
    static custom_allocator_shared_ptr<T, T1> custom_allocator_shared_ptr_static_cast(T2&& obj){

        T * casted = static_cast<T*>(obj.operator->());
        std::atomic<size_t> * counter = obj.get_counter();
        ++(*counter);

        return custom_allocator_shared_ptr<T, T1>(casted, counter);

    }

    template <class T, class T1>
    inline static stack_shared_ptr<T> stack_shared_ptr_static_cast(T1&& data){

        auto casted = custom_allocator_shared_ptr_static_cast<T, ThreadSafeStackAllocator>(std::forward<T1>(data));
    
        return stack_shared_ptr<T>(casted);

    }

    template <class T, class ...Args>
    inline static stack_shared_ptr<T> stack_make_shared(Args&& ...args){

        using namespace memory::linear;

        auto obj = AllocatorSingleton<ThreadSafeStackAllocator>::get();
        auto casted = std::static_pointer_cast<ThreadSafeStackAllocator>(obj);
        void * plm_space = casted->malloc(sizeof(T), alignof(T));
        T * rs = new (plm_space) T(std::forward<Args>(args)...);
                
        return stack_shared_ptr<T>(rs);
        
    }

    template <class T, class T1>
    inline static stack_shared_ptr<T> stack_make_shared_from_shared_ptr(std::shared_ptr<T> data, T1 derived){

        using namespace memory::linear;

        auto obj = AllocatorSingleton<ThreadSafeStackAllocator>::get();
        auto casted = std::static_pointer_cast<ThreadSafeStackAllocator>(obj);
        void * plm_space = casted->malloc(sizeof(T1), alignof(T1));
        T1 * rs = new (plm_space) T1(std::move(derived));
        
        return stack_shared_ptr<T>(rs);
        
    }

}

namespace dgstd{

    template <class T>
    using stack_shared_ptr = memory::pointer::stack_shared_ptr<T>; 

    template <class T, class ...Args>
    inline static stack_shared_ptr<T> stack_make_shared(Args&& ...args){

        return memory::pointer::stack_make_shared<T, Args...>(std::forward<Args>(args)...);

    }
    
    template <class T, class T1>
    inline static stack_shared_ptr<T> stack_shared_ptr_static_cast(T1&& ptr){

        return memory::pointer::stack_shared_ptr_static_cast<T>(std::forward<T1>(ptr)); 

    }

    template <class T, class T1>
    inline static stack_shared_ptr<T> stack_make_shared_from_shared_ptr(std::shared_ptr<T> data, T1 derived){

        return memory::pointer::stack_make_shared_from_shared_ptr(data, derived);
        
    }

};

namespace memory::sizet_linear{

    inline static const uint8_t DEFAULT_VAL = 0;
    inline static const uint8_t BIT_WIDTH = sizeof(size_t) * 8; 

    class StandardVectorView: public VectorReadable<StandardVectorView>{

        protected:

            size_t * data; 
            size_t sz; 

        public:
            
            StandardVectorView(){

                this->data = nullptr;
                this->sz = 0;

            }

            StandardVectorView(size_t * data, size_t sz){

                this->data = data;
                this->sz = sz;

            }

            StandardVectorView(StandardVectorView& obj){

                *this = obj;

            }

            StandardVectorView(StandardVectorView&& obj){

               *this = std::move(obj);

            }

            StandardVectorView& operator =(StandardVectorView& obj){
                
                this->data = obj.data;
                this->sz = obj.sz;

                return *this;

            }

            StandardVectorView& operator =(StandardVectorView&& obj){
                
                this->data = obj.data;
                this->sz = obj.sz;

                obj.data = nullptr;
                obj.sz = 0;

                return *this;

            }

            void * get_data(){

                return this->data;
                
            }

            size_t get(size_t idx){
                
                return data[idx]; 

            }

            size_t length(){

                return this->sz;

            }


    };

    template<class T>
    class StandardOffsetVectorView: public VectorReadable<StandardOffsetVectorView<T>>{

        private:    

            VectorReadable<T> * data;
            
            size_t offset;
            size_t sz;
        
        public:

            StandardOffsetVectorView() = default;

            StandardOffsetVectorView(VectorReadable<T> * data, size_t offset, size_t sz){

                this->data = data;
                this->offset = offset;
                this->sz = sz;

            }

            size_t get(size_t idx){

                return this->data->get(idx + this->offset); 

            }

            void * get_data(){

                return (size_t *) this->data.get_data() + this->offset;

            }

            size_t length(){

                return this->sz; 

            }


    };

    class StandardVectorOperator: public OperatableVector<StandardVectorOperator>{

        protected:

            size_t * data;
            size_t sz;

        public:

            StandardVectorOperator() = default;

            StandardVectorOperator(size_t * data, size_t sz){

                this->data = data;
                this->sz = sz;

            }

            StandardVectorOperator(StandardVectorOperator& obj){

                *this = obj;

            }

            StandardVectorOperator(StandardVectorOperator&& obj){
                
                *this = std::move(obj);

            }

            StandardVectorOperator& operator =(StandardVectorOperator& obj){

                this->data = obj.data;
                this->sz = obj.sz;

                return *this;

            }

            StandardVectorOperator& operator =(StandardVectorOperator&& obj){

                this->data = obj.data;
                this->sz = obj.sz;
                obj.data = nullptr;
                obj.sz = 0;

                return *this;

            }

            void * get_data(){

                return this->data;
                
            }

            size_t get(size_t idx){

                return this->data[idx]; 

            }

            size_t length(){

                return this->sz;

            }
            
            void set(size_t idx, size_t val){

                this->data[idx] = val;

            }


    };

    class SmallAddableVector: public StackAllocatedOperatableVector<SmallAddableVector>{
        
        public:
            
            inline static const uint8_t CAPACITY = 64;

        private:
            
            size_t data[CAPACITY];
            uint8_t sz; 

        public:

            SmallAddableVector(){

                this->sz = 0;

            }

            template <class T>
            SmallAddableVector(VectorReadable<T>& obj){

                *this = obj;

            }

            template <class T>
            SmallAddableVector& operator = (VectorReadable<T>& obj){

                assert(obj.length() <= CAPACITY);

                memcpy(this->data, obj.get_data(), obj.length() * sizeof(size_t));
                this->sz = obj.length();

                return *this;

            }

            SmallAddableVector(SmallAddableVector& obj){

                *this = (VectorReadable<SmallAddableVector>&) obj;

            }

            SmallAddableVector(SmallAddableVector&& obj){

                *this = std::move(obj);

            }

            SmallAddableVector& operator =(SmallAddableVector& obj){

                return *this = (VectorReadable<SmallAddableVector>&) obj;
                
            }

            SmallAddableVector& operator = (SmallAddableVector&& obj){
                
                memcpy(this->data, obj.get_data(), obj.length() * sizeof(size_t));
                this->sz = obj.length();

                return *this;

            }

            size_t length(){

                return this->sz;

            }

            size_t get(size_t idx){

                return this->data[idx]; 

            }

            void * get_data(){

                return this->data;

            }

            void set(size_t idx, size_t val){

                this->data[idx] = val;

            }

            void add(size_t val){

                assert(this->sz != CAPACITY);

                this->data[this->sz++] = val;

            }

    };

    class StackAllocatedAddableVectorGenerator: public AddableVectorGeneratable<StackAllocatedAddableVectorGenerator>{

        public:

            SmallAddableVector get(){

                return SmallAddableVector(); 

            }
    };

    class StandardDefaultInitVectorOperator: public StandardVectorOperator{

        public:

            StandardDefaultInitVectorOperator(): StandardVectorOperator(nullptr, 0){}
            StandardDefaultInitVectorOperator(size_t * data, size_t sz): StandardVectorOperator(data, sz){

                std::memset(data, DEFAULT_VAL, sz * sizeof(size_t));

            };

    };

    class StandardMemControlledVectorOperator: public StandardVectorOperator{

        public:

            StandardMemControlledVectorOperator(): StandardVectorOperator(nullptr, 0){}
            
            StandardMemControlledVectorOperator(size_t sz): StandardVectorOperator((size_t *) std::malloc(sz * sizeof(size_t)), sz){}
                        
            StandardMemControlledVectorOperator(StandardMemControlledVectorOperator&& mv): StandardVectorOperator(std::forward<StandardVectorOperator>(mv)){};
            
            StandardMemControlledVectorOperator(StandardMemControlledVectorOperator& obj): StandardVectorOperator((size_t *) std::malloc(obj.length() * sizeof(size_t)), obj.length()){

                std::memcpy(this->data, obj.data, obj.length() * sizeof(size_t));

            }

            StandardMemControlledVectorOperator& operator =(StandardMemControlledVectorOperator&& obj){
                
                std::free(this->data);
                StandardVectorOperator::operator=(std::move(obj));

                return *this;

            }

            StandardMemControlledVectorOperator& operator =(StandardMemControlledVectorOperator& obj){
                
                StandardMemControlledVectorOperator cp_obj(obj);

                return *this = std::move(cp_obj);

            }

            ~StandardMemControlledVectorOperator(){

                std::free(this->data);

            }


    };

    class StandardMemControlledDefaultInitVectorOperator: public StandardMemControlledVectorOperator{

        private:

            inline static const uint8_t DEFAULT_VAL = 0;

        public:

            StandardMemControlledDefaultInitVectorOperator() = default;

            StandardMemControlledDefaultInitVectorOperator(size_t sz): StandardMemControlledVectorOperator(sz){

                std::memset(this->data, DEFAULT_VAL, sz * sizeof(size_t));

            }

            StandardMemControlledDefaultInitVectorOperator(StandardMemControlledDefaultInitVectorOperator& obj): StandardMemControlledVectorOperator(obj){}

            StandardMemControlledDefaultInitVectorOperator(StandardMemControlledDefaultInitVectorOperator&& obj): StandardMemControlledVectorOperator(std::move(obj)){}

            StandardMemControlledDefaultInitVectorOperator& operator = (StandardMemControlledDefaultInitVectorOperator& obj){

                StandardMemControlledVectorOperator::operator=(obj);

                return *this;

            }   
            
            StandardMemControlledDefaultInitVectorOperator& operator = (StandardMemControlledDefaultInitVectorOperator&& obj){

                StandardMemControlledVectorOperator::operator=(std::move(obj));

                return *this;

            }   

    };

    class StandardReallocatableMemControlledVectorOperator: public StandardMemControlledVectorOperator, public ReallocatableOperatableVector<StandardReallocatableMemControlledVectorOperator>{

        public:

            StandardReallocatableMemControlledVectorOperator() = default;

            StandardReallocatableMemControlledVectorOperator(size_t sz): StandardMemControlledVectorOperator(sz){}

            void resize(size_t sz){
                                
                StandardMemControlledVectorOperator new_data(sz);
                size_t cp_length = std::min(sz, this->length()); 
                
                for (size_t i = 0; i < cp_length; ++i){

                    new_data.set(i, this->get(i));

                }
                
                StandardMemControlledVectorOperator::operator=(std::move(new_data));

            }

            void resize_no_copy(size_t sz){

                StandardMemControlledVectorOperator new_data(sz);
                StandardMemControlledVectorOperator::operator=(std::move(new_data));

            }

            using StandardMemControlledVectorOperator::get;
            using StandardMemControlledVectorOperator::get_data;
            using StandardMemControlledVectorOperator::length;
            using StandardMemControlledVectorOperator::sizeof_slot;
            using StandardMemControlledVectorOperator::set;
            using StandardMemControlledVectorOperator::to_operatable_vector;
            using StandardMemControlledVectorOperator::to_vector_readable;

    };

    class StandardDanglingOperatableVectorGenerator: public DanglingOperatableVectorGeneratable<StandardDanglingOperatableVectorGenerator>{

        public:

            StandardVectorOperator from_ptr(void * data, size_t sz){

                return StandardVectorOperator((size_t *) data, sz); 

            }

    };

    class StandardDanglingDefaultInitOperatableVectorGenerator: public DanglingOperatableVectorGeneratable<StandardDanglingDefaultInitOperatableVectorGenerator>{

        public:

            StandardDefaultInitVectorOperator from_ptr(void * data, size_t sz){
                                
                return StandardDefaultInitVectorOperator((size_t *) data, sz); 

            }


    };

    class SmallStackVectorView: public StandardVectorView{

        private:

            SmallAddableVector small_view;
            bool is_stack_allocated; 

        public:

            SmallStackVectorView(): StandardVectorView(){

                this->is_stack_allocated = false;

            }

            SmallStackVectorView(size_t * data, size_t sz): StandardVectorView(data, sz){

                this->is_stack_allocated = false;

            }

            template <class T>
            SmallStackVectorView(StackAllocatedVectorReadable<T>& data): StandardVectorView(){

                assert(data.length() <= SmallAddableVector::CAPACITY);
                
                this->small_view = SmallAddableVector(data);                
                StandardVectorView::operator=(StandardVectorView((size_t *) this->small_view.get_data(), this->small_view.length()));
                this->is_stack_allocated = true;

            }

            template <class T>
            SmallStackVectorView(VectorReadable<T>& data){
                                
                StandardVectorView::operator=(StandardVectorView((size_t *) data.get_data(), data.length())); 
                this->is_stack_allocated = false;

            }

            SmallStackVectorView(SmallStackVectorView& obj){

                *this = obj;

            }

            SmallStackVectorView(SmallStackVectorView&& obj){

                *this = std::move(obj);

            }

            SmallStackVectorView& operator = (SmallStackVectorView& obj){
                
                if (obj.is_stack_allocated){

                    this->small_view = *obj.small_view.to_vec_readable();
                    StandardVectorView::operator=(StandardVectorView((size_t *) this->small_view.get_data(), this->small_view.length())); 
                    this->is_stack_allocated = true;

                } else{
                                        
                    StandardVectorView::operator=(obj); 
                    this->is_stack_allocated = false;

                }

                return *this;

            }

            SmallStackVectorView& operator = (SmallStackVectorView&& obj){
                
                if (obj.is_stack_allocated){

                    this->small_view = *obj.small_view.to_vec_readable();
                    StandardVectorView::operator=(StandardVectorView((size_t *) this->small_view.get_data(), this->small_view.length())); 
                    this->is_stack_allocated = true;

                } else{

                    StandardVectorView::operator=(std::move(obj));
                    this->is_stack_allocated = false;
                    
                }

                return *this;

            }

    };

    class StandardVectorReadableGenerator: public VectorReadableGeneratable<StandardVectorReadableGenerator>{

        public:

            template <class T>
            auto from_vec_readable(VectorReadable<T>& data){

                return StandardVectorView((size_t *) data.get_data(), data.length()); 

            } 

            auto from_ptr(void * data, size_t sz){

                return StandardVectorView((size_t *) data, sz); 

            }

    };

    class StandardStackVectorReadableGenerator: public StackVectorReadableGeneratable<StandardStackVectorReadableGenerator>{

        public:

            template <class T>
            auto from_vec_readable(VectorReadable<T>& data){
                                
                return SmallStackVectorView(data);

            }

            template <class T>
            auto from_vec_readable(StackAllocatedVectorReadable<T>& data){

                return SmallStackVectorView(data);

            }

            auto from_ptr(void * data, size_t sz){

                return SmallStackVectorView((size_t *) data, sz);

            }

    };

    class VectorReadableSplitter: public VectorReadableSplittable<VectorReadableSplitter>{

        public:

            struct SplittedPair{

                StandardVectorView first;
                StandardVectorView second;

            };

            SplittedPair make_pair(StandardVectorView first, StandardVectorView second){

                SplittedPair rs;

                rs.first = first;
                rs.second = second;

                return rs;

            }

            template <class T>
            SplittedPair split(VectorReadable<T>& data, size_t split_point){

                size_t first_length = 0;
                size_t second_length = 0;

                this->get_length(data, split_point, first_length, second_length);

                auto rs = make_pair(StandardVectorView((size_t *) data.get_data(), first_length),
                                    StandardVectorView((size_t *) data.get_data() + first_length, second_length));
                
                return rs;

            }

        private:

            template <class T>
            void get_length(VectorReadable<T>& data, size_t split_point, size_t& first_length, size_t& second_length){
                
                first_length = std::min(split_point, data.length());
                second_length = data.length() - first_length;

            }

    };

    class StandardBoolVectorView: public datastructure::UnalignedViewableBoolVector<StandardBoolVectorView>{

        private:

            StandardVectorView data;
            
        public:

            StandardBoolVectorView() = default;

            template <class T>
            StandardBoolVectorView(VectorReadable<T>& obj){

                this->data = StandardVectorView((size_t *) obj.get_data(), obj.length());

            }

            size_t length(){
                
                return this->data.length() * this->get_slot_bit_width();  

            }

            size_t byte_length(){

                uint64_t bit_length = this->length();

                if (bit_length % 8 == 0){
                    
                    return bit_length >> 3; 

                }

                return (bit_length >> 3) + 1;

            }

            bool get(size_t idx){

                return this->data.get(this->get_slot(idx)) & (1ULL << this->get_offset(idx));

            }

        protected:

            size_t get_slot(size_t idx){

                return idx / this->get_slot_bit_width();
            }

            size_t get_offset(size_t idx){

                return idx % this->get_slot_bit_width();

            }

            size_t get_slot_bit_width(){

                return this->data.sizeof_slot() * 8;

            }

            size_t get_slot_size(){

                return this->data.length();

            }

    };

    class StandardBoolVectorOperator: public datastructure::UnalignedOperatableBoolVector<StandardBoolVectorOperator>{

        private:

            StandardVectorOperator data; 

        public:

            StandardBoolVectorOperator() = default;

            template <class T>
            StandardBoolVectorOperator(OperatableVector<T>& obj){
                
                this->data = StandardVectorOperator((size_t *) obj.get_data(), obj.length());

            }

            void set_val(uint64_t idx, bool val){
                
                size_t slot = this->get_slot(idx);
                size_t offset = this->get_offset(idx); 
                size_t updated = this->data.get(slot);

                if (val){

                    updated |= (1ULL << offset);                 
                    this->data.set(slot, updated);

                    return;

                }

                updated &= (std::numeric_limits<size_t>::max() ^ 1ULL << offset);
                this->data.set(slot, updated);

            }

            uint64_t length(){

                return this->data.length() * this->get_slot_bit_width();  

            }

            uint64_t byte_length(){

                uint64_t bit_length = this->length();

                if (bit_length % 8 == 0){
                    
                    return bit_length >> 3; 

                }

                return (bit_length >> 3) + 1;

            }

            bool get(uint64_t idx){

                return this->data.get(this->get_slot(idx)) & (1ULL << this->get_offset(idx));

            }

        protected:

            size_t get_slot(size_t idx){

                return idx / this->get_slot_bit_width();
            }

            size_t get_offset(size_t idx){

                return idx % this->get_slot_bit_width();

            }

            size_t get_slot_bit_width(){

                return this->data.sizeof_slot() * 8;

            }

            size_t get_slot_size(){

                return this->data.length();

            }

    }; 

    namespace caster::plm_boolvector{
        
        class StandardBoolVectorViewCaster: public BoolVectorViewCastable<StandardBoolVectorViewCaster>{

            public:

                template <class T>
                StandardBoolVectorView cast(VectorReadable<T>& data){

                    return StandardBoolVectorView(data);  

                }

        };

        class StandardBoolVectorOpCaster: public BoolVectorOpCastable<StandardBoolVectorOpCaster>{

            public:

                template <class T>
                StandardBoolVectorOperator cast(OperatableVector<T>& data){

                    return StandardBoolVectorOperator(data);

                }

        };

    }

    template <class T, class T1>
    class ExponentialLinearBlockTempStorageGenerator: public TempStorageGeneratable<ExponentialLinearBlockTempStorageGenerator<T,T1>>{

        protected:

            std::shared_ptr<VectorReadableGeneratable<T>> vec_generator;
            std::shared_ptr<DanglingOperatableVectorGeneratable<T1>> op_vector_generator;
            linear::ExponentialLinearStorageBlockGenerator allocator;

        public:

            ExponentialLinearBlockTempStorageGenerator(size_t base_length,
                                                       std::shared_ptr<VectorReadableGeneratable<T>> vec_generator,
                                                       std::shared_ptr<DanglingOperatableVectorGeneratable<T1>> op_vector_generator){
                
                this->allocator = linear::ExponentialLinearStorageBlockGenerator(base_length);
                this->vec_generator = vec_generator;
                this->op_vector_generator = op_vector_generator;

            };


            void enter_scope(){

                this->allocator.enter_scope();

            }
            
            void exit_scope(bool shrinked = true){

                this->allocator.exit_scope(shrinked);

            }

            void * exit_scope(void * ptr, size_t n, bool shrinked = true){

                return this->allocator.exit_scope(ptr, n, shrinked); 

            }

            void * get(size_t n){
                                
                return this->allocator.get(n);

            }               
            
            void shrink(){

                this->allocator.shrink();

            }

            template <class T2>
            auto exit_scope(VectorReadable<T2>& data){

                void * data_buf = this->allocator.exit_scope(data.get_data(), data.length() * sizeof(size_t));

                return this->vec_generator->from_ptr(data_buf, data.length()); 

            }

            template <class T2>
            auto exit_scope_no_shrink(VectorReadable<T2>& data){ // -> VectorReadable&

                void * data_buf = this->allocator.exit_scope(data.get_data(), data.length() * sizeof(size_t), false);

                return this->vec_generator->from_ptr(data_buf, data.length()); 

            }

            auto vec_get(size_t sz){

                void * data_buf = this->allocator.get(sz * sizeof(size_t));

                return this->op_vector_generator->from_ptr(data_buf, sz); 

            }

            auto vec_get_default(size_t sz){

                auto rs = this->vec_get(sz);

                for (size_t i = 0; i < rs.length(); ++i){

                    rs.set(i, 0);

                }

                return rs;

            }

    };

    class StandardExponentialLinearTempStorageGenerator: public TempStorageGeneratable<StandardExponentialLinearTempStorageGenerator>{
        
        private:

            const size_t DEFAULT_BUF = 1024; 
            
            std::shared_ptr<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator, StandardDanglingDefaultInitOperatableVectorGenerator>> allocator;

        public:

            StandardExponentialLinearTempStorageGenerator(){
                    
                auto readable_gen = std::make_shared<StandardVectorReadableGenerator>();
                auto operatable_gen = std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator>();
                
                auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                auto casted_operatable_gen = operatable_gen->to_dangling_operatable_vector_generatable_sp(operatable_gen); 

                this->allocator = std::make_shared<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator, StandardDanglingDefaultInitOperatableVectorGenerator>>(DEFAULT_BUF, casted_readable_gen, casted_operatable_gen);

            }

            StandardExponentialLinearTempStorageGenerator(size_t base_length){

                auto readable_gen = std::make_shared<StandardVectorReadableGenerator>();
                auto operatable_gen = std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator>();
                
                auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                auto casted_operatable_gen = operatable_gen->to_dangling_operatable_vector_generatable_sp(operatable_gen); 

                this->allocator = std::make_shared<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator, StandardDanglingDefaultInitOperatableVectorGenerator>>(base_length, casted_readable_gen, casted_operatable_gen);

            }
            
            template <class T1>
            auto exit_scope(VectorReadable<T1>& data){ // -> VectorReadable&

                return this->allocator->exit_scope(data); 

            }

            template <class T1>
            auto exit_scope_no_shrink(VectorReadable<T1>& data){ // -> VectorReadable&

                return this->allocator->exit_scope_no_shrink(data);

            }

            auto vec_get(size_t n){ // -> OperatableVector&

                return this->allocator->vec_get(n);

            } 

            auto vec_get_default(size_t n){ // -> OperatableVector&

                return this->allocator->vec_get_default(n);

            }

            void enter_scope(){

                this->allocator->enter_scope();

            }
            
            void exit_scope(bool shrinked = true){

                this->allocator->exit_scope(shrinked);

            }

            void * exit_scope(void * ptr, size_t n, bool shrinked = true){

                return this->allocator->exit_scope(ptr, n, shrinked);

            }

            void * get(size_t n){
                                
                return this->allocator->get(n);

            }               
            
            void shrink(){

                this->allocator->shrink();

            }
             
    };

    template <class T>
    class DynamicBitIterator: public DynamicBitIterable<DynamicBitIterator<T>>{

        private:

            VectorReadable<T> * data;
            size_t idx;
        
        public:

            DynamicBitIterator(){

                this->data = nullptr;
                this->idx = 0;

            }

            DynamicBitIterator(VectorReadable<T> * data){

                this->data = data;
                this->idx = 0;

            }

            DynamicBitIterator(VectorReadable<T> * data, size_t offset){

                this->data = data;
                this->idx = offset;

            }

            bool next(size_t& rs, size_t read_length){ 

                assert(read_length <= BIT_WIDTH);
                assert(this->data != nullptr);

                size_t total_length = this->data->length() * BIT_WIDTH; 
                
                if (this->idx >= total_length){

                    return false;

                }

                size_t real_read_length = std::min(read_length, total_length - this->idx);
                rs = this->read((size_t *) this->data->get_data(), this->idx, real_read_length);
                this->idx += real_read_length;
                
                return true;

            }

        private:

            size_t get_extractor(size_t bit_length){
                
                if (bit_length == BIT_WIDTH){

                    return std::numeric_limits<size_t>::max();

                }

                return ((size_t) 1 << bit_length) - 1;

            } 

            size_t extract(size_t data, size_t bob_idx, size_t eor_idx){

                size_t extractor = this->get_extractor(eor_idx - bob_idx + 1);

                return (data >> bob_idx) & extractor;

            }

            size_t read(size_t * data, size_t bit_idx, size_t read_length){
                                
                const uint8_t MIN_BIT_IDX = 0;
                const uint8_t MAX_BIT_IDX = BIT_WIDTH - 1;

                size_t eor = bit_idx + read_length - 1;  
                size_t bob_slot = bit_idx / BIT_WIDTH;
                size_t eor_slot = eor / BIT_WIDTH;
                size_t bob_offset = bit_idx % BIT_WIDTH;
                size_t eor_offset = eor % BIT_WIDTH;

                if (bob_slot == eor_slot){

                    return this->extract(data[bob_slot], bob_offset, eor_offset);

                } 

                size_t delta = BIT_WIDTH - bob_offset; 
                size_t eor_data = this->extract(data[eor_slot], MIN_BIT_IDX, eor_offset);
                size_t bob_data = this->extract(data[bob_slot], bob_offset, MAX_BIT_IDX);

                return (eor_data << delta) | bob_data;  

            }
    };

    template <class T>
    class StandardBitIterator: public DynamicBitIterator<T>, public BitIterable<StandardBitIterator<T>>{

        private:

            size_t read_length;
        
        public:

            StandardBitIterator(VectorReadable<T> * data, size_t read_length): DynamicBitIterator<T>(data){

                this->read_length = read_length;

            }

            bool next(size_t& rs){ 

                return DynamicBitIterator<T>::next(rs, this->read_length);

            }
  
    };

    class StandardDynamicBitIteratorGenerator: public DynamicBitIteratorGeneratable<StandardDynamicBitIteratorGenerator>{

        public:

            template <class T>
            DynamicBitIterator<T> get(VectorReadable<T>& data){

                return DynamicBitIterator<T>(&data);

            }

            template <class T>
            DynamicBitIterator<T> get(VectorReadable<T>& data, size_t offset){

                return DynamicBitIterator<T>(&data, offset);

            }

    };

    class StandardBitIteratorGenerator: public BitIteratorGeneratable<StandardBitIteratorGenerator>{

        public:

            template <class T>
            StandardBitIterator<T> get(VectorReadable<T>& data, size_t read_length){

                return StandardBitIterator<T>(&data, read_length);

            }

    };

    template <class T>
    class DynamicBitIterOrWriter: public DynamicBitIterWritable<DynamicBitIterOrWriter<T>>{

        private:

            OperatableVector<T> * data;
            size_t idx;
        
        public:

            DynamicBitIterOrWriter(){

                this->data = nullptr;
                this->idx = 0;

            }

            DynamicBitIterOrWriter(OperatableVector<T> * data, size_t offset){

                this->data = data;
                this->idx = offset;

            }

            bool write(size_t val, size_t write_length){
                
                assert(write_length <= BIT_WIDTH);
                assert(this->data != nullptr);

                size_t total_length = this->data->length() * BIT_WIDTH;
                
                if (this->idx >= total_length){
                    
                    return false;

                }
                
                write_length = std::min(total_length - this->idx, write_length);
                this->write(val, this->idx, write_length);
                this->idx += write_length;

                return true; 

            }
        
        private:

            size_t get_extractor(size_t length){

                if (length == BIT_WIDTH){

                    return std::numeric_limits<size_t>::max();

                }

                return ((size_t) 1 << length) - 1;

            } 

            void write(size_t val, size_t bob_idx, size_t length){

                val = val & this->get_extractor(length); 

                size_t eor_idx = bob_idx + length - 1;
                size_t bob_slot = bob_idx / BIT_WIDTH;
                size_t bob_offset = bob_idx % BIT_WIDTH;
                size_t eor_slot = eor_idx / BIT_WIDTH;
                size_t eor_offset = eor_idx % BIT_WIDTH;

                if (bob_slot == eor_slot){
                    
                    this->data->set(bob_slot, this->data->get(bob_slot) | (val << bob_offset));

                    return;

                } 

                size_t lo_length = BIT_WIDTH - bob_offset;
                size_t lo_val = this->get_extractor(lo_length) & val;
                size_t lo_adjusted_val = this->data->get(bob_slot) | (lo_val << bob_offset);
                size_t hi_adjusted_val = this->data->get(eor_slot) | (val >> lo_length);

                this->data->set(bob_slot, lo_adjusted_val);
                this->data->set(eor_slot, hi_adjusted_val);

            }

    };

    template <class T>
    class DynamicBitIterReplaceWriter: public DynamicBitIterWritable<DynamicBitIterReplaceWriter<T>>{

        private:

            OperatableVector<T> * data;
            size_t idx;
        
        public:

            DynamicBitIterReplaceWriter(){

                this->data = nullptr;
                this->idx = 0;

            }

            DynamicBitIterReplaceWriter(OperatableVector<T> * data, size_t offset){

                this->data = data;
                this->idx = offset;

            }

            bool write(size_t val, size_t write_length){
                
                assert(write_length <= BIT_WIDTH);
                assert(this->data != nullptr);

                size_t total_length = this->data->length() * BIT_WIDTH;
                
                if (this->idx >= total_length){
                    
                    return false;

                }
                
                write_length = std::min(total_length - this->idx, write_length);
                this->write(val, this->idx, write_length);
                this->idx += write_length;

                return true; 

            }
        
        private:

            size_t get_extractor(size_t length){

                if (length == BIT_WIDTH){

                    return std::numeric_limits<size_t>::max();

                }

                return ((size_t) 1 << length) - 1;

            } 

            size_t get_eraser(size_t length){

                return this->get_extractor(BIT_WIDTH - length) << length;

            }

            void write(size_t val, size_t bob_idx, size_t length){

                val = val & this->get_extractor(length); 

                size_t eor_idx = bob_idx + length - 1;
                size_t bob_slot = bob_idx / BIT_WIDTH;
                size_t bob_offset = bob_idx % BIT_WIDTH;
                size_t eor_slot = eor_idx / BIT_WIDTH;
                size_t eor_offset = eor_idx % BIT_WIDTH;

                if (bob_slot == eor_slot){
                    
                    size_t erasor = this->get_extractor(bob_offset);
                    this->data->set(bob_slot, (this->data->get(bob_slot) & erasor) | (val << bob_offset));

                    return;

                } 

                size_t lo_length = BIT_WIDTH - bob_offset;
                size_t hi_length = length - lo_length;
                size_t lo_erasor = this->get_extractor(bob_offset);
                size_t hi_erasor = this->get_eraser(hi_length); 

                size_t lo_val = this->get_extractor(lo_length) & val;
                size_t lo_adjusted_val = (this->data->get(bob_slot) & lo_erasor) | (lo_val << bob_offset);
                size_t hi_adjusted_val = (this->data->get(eor_slot) & hi_erasor) | (val >> lo_length);

                this->data->set(bob_slot, lo_adjusted_val);
                this->data->set(eor_slot, hi_adjusted_val);

            }

    };

    template <class T>
    class BitIterOrWriter: public BitIterWritable<BitIterOrWriter<T>>, public DynamicBitIterOrWriter<T>{

        private:

            size_t write_length;
        
        public:

            BitIterOrWriter(OperatableVector<T> * data, size_t offset, size_t write_length): DynamicBitIterOrWriter<T>(data, offset){

                this->write_length = write_length;

            }

            bool write(size_t val){

                return DynamicBitIterOrWriter<T>::write(val, this->write_length);

            }

    };

    template <class T>
    class BitIterReplaceWriter: public BitIterWritable<BitIterReplaceWriter<T>>, public DynamicBitIterReplaceWriter<T>{

        private:

            size_t write_length;
        
        public:

            BitIterReplaceWriter(OperatableVector<T> * data, size_t offset, size_t write_length): DynamicBitIterReplaceWriter<T>(data, offset){

                this->write_length = write_length;

            }

            bool write(size_t val){

                return DynamicBitIterReplaceWriter<T>::write(val, this->write_length);

            }

    };

    class DynamicBitIterEmptyWriterGenerator: public DynamicBitIterWriterGeneratable<DynamicBitIterEmptyWriterGenerator>{

        public:

            template<class T>
            DynamicBitIterOrWriter<T> get(OperatableVector<T>& data, size_t offset){ 

                return DynamicBitIterOrWriter<T>(&data, offset);

            }

    };

    class DynamicBitIterReplaceWriterGenerator: public DynamicBitIterWriterGeneratable<DynamicBitIterReplaceWriterGenerator>{

        public:

            template<class T>
            DynamicBitIterReplaceWriter<T> get(OperatableVector<T>& data, size_t offset){ 

                return DynamicBitIterReplaceWriter<T>(&data, offset);

            }

    };

    class BitIterEmptyWriterGenerator: public BitIterWriterGeneratable<BitIterEmptyWriterGenerator>{

        public:

            template<class T>
            BitIterOrWriter<T> get(OperatableVector<T>& data, size_t write_length, size_t offset, void * plm_space){

                return BitIterOrWriter<T>(&data, offset, write_length);

            }

    };
    
    using namespace caster::plm_boolvector;
    using namespace caster;

    class StandardGenerator{

        public:

            auto get_temp_storage(size_t n){

                return std::make_shared<StandardExponentialLinearTempStorageGenerator>(n);

            }

            auto get_dangling_op_vec_default_init_gen(){

                return std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator>();

            }

            auto get_stack_allocated_addable_vector_gen(){

                return std::make_shared<StackAllocatedAddableVectorGenerator>();

            }

            auto get_dangling_op_vec_gen(){

                return std::make_shared<StandardDanglingOperatableVectorGenerator>();

            }

            auto get_dangling_read_vec_gen(){

                return std::make_shared<StandardVectorReadableGenerator>();

            } 

            auto get_dangling_stack_read_vec_gen(){

                return std::make_shared<StandardStackVectorReadableGenerator>();

            } 
            
            auto get_vector_splitter(){

                return std::make_shared<VectorReadableSplitter>();

            }

            auto get_boolvector_view_caster(){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorViewCaster>();

            }

            auto get_boolvector_op_caster(){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorOpCaster>();

            }

            auto get_dynamic_bit_or_writer_gen(){
                
                return std::make_shared<DynamicBitIterEmptyWriterGenerator>();

            }

            auto get_dynamic_bit_replace_writer_gen(){
                
                return std::make_shared<DynamicBitIterReplaceWriterGenerator>();

            }

            auto get_bit_empty_writer_gen(){

                return std::make_shared<BitIterEmptyWriterGenerator>();

            }

            auto get_dynamic_bit_iter_gen(){

                return std::make_shared<StandardDynamicBitIteratorGenerator>();

            }

            auto get_bit_iter_gen(){

                return std::make_shared<StandardBitIteratorGenerator>();

            }
            
    };  

}


#endif                   