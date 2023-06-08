
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

                    return std::static_pointer_cast<Allocatable<T>>(data); 

                }

        };

        template <class T>
        class Reallocatable: public Allocatable<T>{

            public:

                void * realloc(void * ptr, size_t n){
                                        
                    return static_cast<T*>(this)->realloc(ptr, n); 

                }

                template <class T1>
                std::shared_ptr<Reallocatable<T>> to_reallocatable_sp(std::shared_ptr<T1> data){

                    return std::static_pointer_cast<Reallocatable<T>>(data); 

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

                    return std::static_pointer_cast<AlignedAllocatable<T>>(data); 

                }

        };

        template <class T>
        class AllocatorSingleton{

            private:

                static inline Allocatable<T> * instance = nullptr;

            public:

                static Allocatable<T> * get(){
                    
                    if (AllocatorSingleton<T>::instance == nullptr){
                        
                        AllocatorSingleton<T>::instance = new T();
                        
                    }

                    return AllocatorSingleton<T>::instance;

                }

                static void destruct(){
                    
                    delete AllocatorSingleton<T>::instance; 

                    AllocatorSingleton<T>::instance = nullptr;
                    
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
        class StackAllocatedOperatableVector: public OperatableVector<T>, public StackAllocatedVectorReadable<StackAllocatedOperatableVector<T>>{

            public:

                using OperatableVector<T>::get;
                using OperatableVector<T>::length;
                using OperatableVector<T>::get_data;
                using OperatableVector<T>::sizeof_slot;
                using OperatableVector<T>::to_vector_readable;
                using OperatableVector<T>::to_operatable_vector;

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
        class StackAllocatedReallocatableOperatableVector: public ReallocatableOperatableVector<T>, public StackAllocatedVectorReadable<StackAllocatedReallocatableOperatableVector<T>>{
                
            public:
                
                using ReallocatableOperatableVector<T>::get;
                using ReallocatableOperatableVector<T>::length;
                using ReallocatableOperatableVector<T>::get_data;
                using ReallocatableOperatableVector<T>::sizeof_slot;
                using ReallocatableOperatableVector<T>::to_vector_readable;

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

                    return std::static_pointer_cast<TempStorageGeneratable<T>>(data); 

                }

                auto get_containee_type(){

                    return T(); 

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

                    return std::static_pointer_cast<DanglingOperatableVectorGeneratable<T>>(data);

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

                    return std::static_pointer_cast<VectorReadableGeneratable<T>>(data); 

                }

        };

        template <class T>
        class ReallocatableVectorGeneratable{

            public:

                auto get(){

                    return static_cast<T*>(this)->get();

                }

                template <class T1>
                std::shared_ptr<ReallocatableVectorGeneratable<T>> to_reallocatable_vector_generatable_sp(std::shared_ptr<T1> data){

                    return std::static_pointer_cast<ReallocatableVectorGeneratable<T>>(data); 

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

                    return std::static_pointer_cast<StackVectorReadableGeneratable<T>>(data); 

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

                    return std::static_pointer_cast<AddableVectorGeneratable<T>>(data); 

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
                
                template <class T1>
                bool next(VectorReadable<T1>& data, size_t& rs, size_t read_sz){

                    return static_cast<T*>(this)->next(data, rs, read_sz);

                }

        };

        template<class T>
        class DynamicBitIteratorGeneratable{

            public:

                auto get(){ // -> DynamicBitIterable<>&

                    return static_cast<T*>(this)->get(); 

                }

                auto get(size_t offset){ // -> DynamicBitIterable<>&

                    return static_cast<T*>(this)->get(offset);

                }

                template <class T1>
                std::shared_ptr<DynamicBitIteratorGeneratable<T>> to_dynamic_bit_iterator_generatable_sp(std::shared_ptr<T1> data){

                    return std::static_pointer_cast<DynamicBitIteratorGeneratable<T>>(data); 

                } 


        };
        
        template<class T>
        class DynamicBitIterWritable{

            public:

                template <class T1>
                bool write(OperatableVector<T1>& data, size_t write_val, size_t write_sz){

                    return static_cast<T*>(this)->write(data, write_val, write_sz);

                }

        };

        template<class T>
        class DynamicBitIterWriterGeneratable{

            public:

                auto get(size_t offset){ // -> DynamicBitIterWritable&

                    return static_cast<T*>(this)->get(offset);

                } 
                
                template <class T2>
                std::shared_ptr<DynamicBitIterWriterGeneratable<T>> to_dynamic_bit_iter_writer_generatable_sp(std::shared_ptr<T2> data){

                    return std::static_pointer_cast<DynamicBitIterWriterGeneratable<T>>(data);

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

            void free(void * ptr){

            }

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

    class StdAllocator: public Allocatable<StdAllocator>{

        public:

            void * malloc(size_t n) const noexcept{

                return std::malloc(n);
            
            }

            void free(void * ptr) const noexcept{
                      
                std::free(ptr);

            }

    };

    class StdReallocator: public Reallocatable<StdReallocator>{
        
        public:

            void * malloc(size_t n) {
                
                return std::malloc(n);
            
            }

            void free(void * ptr){
                  
                std::free(ptr);

            }

            void * realloc(void * ptr, size_t n){
                
                return std::realloc(ptr, n);

            }

    };

    class CircularAllocator: public Allocatable<CircularAllocator>{

        private:

            void * buf;
            size_t n;
            size_t offset;
        
        public:

            CircularAllocator(size_t n){

                this->buf = std::malloc(n);
                this->n = n;

            }
            CircularAllocator(const CircularAllocator&) = delete;
            CircularAllocator(CircularAllocator&& obj) = delete;
            CircularAllocator& operator = (const CircularAllocator&) = delete;
            CircularAllocator& operator = (CircularAllocator&&) = delete;
           
            ~CircularAllocator(){

                std::free(this->buf);

            }

            void * malloc(size_t buf_sz){
                
                assert(buf_sz <= this->n);

                if (this->offset + buf_sz <= this->n){

                    this->offset += buf_sz;
                    return (char *) this->buf + (this->offset - buf_sz); 
                
                }

                this->offset = 0;
                return this->malloc(buf_sz);

            }
    
            void free(void*){};

    };

    class CircularReallocator: public Reallocatable<CircularReallocator>, public CircularAllocator{

        public:

            CircularReallocator(size_t base_length): CircularAllocator(base_length){}

            void * malloc(size_t n){
                
                size_t total_buf_n = n + sizeof(size_t); 
                void * buf = CircularAllocator::malloc(total_buf_n);
                std::memcpy(buf, &n, sizeof(size_t));

                return (char *) buf + sizeof(size_t);

            }

            void * realloc(void * ptr, size_t n){
                
                if (ptr == nullptr){
                    
                    return this->malloc(n);

                }

                size_t prev_n = 0;
                void * src = (char *) ptr - sizeof(size_t); 

                std::memcpy(&prev_n, src, sizeof(size_t));
                
                if (n <= prev_n){

                    return ptr;

                } 
                                
                void * new_buf = this->malloc(n);
                std::memcpy(new_buf, ptr, prev_n);

                return new_buf;


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
        auto casted = static_cast<ThreadSafeStackAllocator *>(obj);
        void * plm_space = casted->malloc(sizeof(T), alignof(T));
        T * rs = new (plm_space) T(std::forward<Args>(args)...);
                
        return stack_shared_ptr<T>(rs);
        
    }

    template <class T, class T1>
    inline static stack_shared_ptr<T> stack_make_shared_from_shared_ptr(std::shared_ptr<T> data, T1 derived){

        using namespace memory::linear;

        auto obj = AllocatorSingleton<ThreadSafeStackAllocator>::get();
        auto casted = static_cast<ThreadSafeStackAllocator *>(obj);
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


};

namespace memory::sizet_linear{

    inline static const size_t DEFAULT_VAL = 0;
    inline static const size_t BIT_WIDTH = sizeof(size_t) * 8; 

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

            StandardVectorOperator(){

                this->data = nullptr;
                this->sz = 0;

            }

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
    
    template <unsigned CAP>
    class StandardStackAllocatedVector: public StackAllocatedReallocatableOperatableVector<StandardStackAllocatedVector<CAP>>{

        private:

            size_t data[CAP];
            uint8_t sz;
        
        public:

            constexpr StandardStackAllocatedVector(){

                this->sz = 0;

            }

            constexpr StandardStackAllocatedVector(const StandardStackAllocatedVector& obj){

                *this = obj;

            }

            constexpr StandardStackAllocatedVector(StandardStackAllocatedVector&& obj){

                *this = obj;

            }

            constexpr StandardStackAllocatedVector& operator = (StandardStackAllocatedVector&& obj){
                
                return *this = obj;

            }

            constexpr StandardStackAllocatedVector& operator = (const StandardStackAllocatedVector& obj){
                
                this->sz = obj.sz;
                
                for (size_t i = 0; i < obj.sz; ++i){

                    this->data[i] = obj.data[i];
                
                }

                return *this;

            }

            constexpr size_t get(size_t idx){

                return this->data[idx];

            }

            constexpr void set(size_t idx, size_t val){

                this->data[idx] = val;

            }

            constexpr size_t length(){

                return this->sz;

            }

            constexpr void * get_data(){

                return this->data; 

            }

            constexpr void resize(size_t sz){

                assert(sz <= CAP);

                this->sz = sz;

            }

            constexpr void resize_no_copy(size_t sz){

                assert(sz <= CAP);

                this->sz = sz;

            }

    };

    template <class ID>
    class StackAllocatedAddableVectorGenerator: public AddableVectorGeneratable<StackAllocatedAddableVectorGenerator<ID>>{

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
    
    template <class T>
    class StandardMemControlledVectorOperator: public StandardVectorOperator{

        public:

            StandardMemControlledVectorOperator(): StandardVectorOperator(nullptr, 0){}
            
            StandardMemControlledVectorOperator(size_t * buf, size_t n): StandardVectorOperator(buf, n){}

            StandardMemControlledVectorOperator(size_t sz): StandardVectorOperator((size_t *) memory::linear::AllocatorSingleton<T>().get()->malloc(sz * sizeof(size_t)), sz){}
     
            StandardMemControlledVectorOperator(StandardMemControlledVectorOperator&& mv): StandardVectorOperator(std::forward<StandardVectorOperator>(mv)){};
            
            StandardMemControlledVectorOperator(StandardMemControlledVectorOperator& obj): StandardVectorOperator((size_t *) memory::linear::AllocatorSingleton<T>().get()->malloc(obj.length() * sizeof(size_t)), obj.length()){

                std::memcpy(this->get_data(), obj.get_data(), obj.length() * sizeof(size_t));

            }


            StandardMemControlledVectorOperator& operator =(StandardMemControlledVectorOperator&& obj){
                
                memory::linear::AllocatorSingleton<T>().get()->free(this->get_data());
                StandardVectorOperator::operator=(std::move(obj));

                return *this;

            }

            StandardMemControlledVectorOperator& operator =(StandardMemControlledVectorOperator& obj){
                                
                StandardMemControlledVectorOperator cp_obj(obj);

                return *this = std::move(cp_obj);

            }

            ~StandardMemControlledVectorOperator(){

                memory::linear::AllocatorSingleton<T>().get()->free(this->get_data());

            }


    };

    template <class T>
    class StandardReallocatableMemControlledVectorOperator: public StandardMemControlledVectorOperator<T>, public ReallocatableOperatableVector<StandardReallocatableMemControlledVectorOperator<T>>{

        public:

            StandardReallocatableMemControlledVectorOperator() = default;

            StandardReallocatableMemControlledVectorOperator(size_t sz): StandardMemControlledVectorOperator<T>(sz){}

            void resize(size_t sz){                 
                
                StandardMemControlledVectorOperator<T> new_data(sz);
                size_t cp_length = std::min(sz, this->length()); 
                
                for (size_t i = 0; i < cp_length; ++i){

                    new_data.set(i, this->get(i));

                }
   
                StandardMemControlledVectorOperator<T>::operator=(std::move(new_data));

            }

            void resize_no_copy(size_t sz){

                StandardMemControlledVectorOperator<T> new_data(sz);
                StandardMemControlledVectorOperator<T>::operator=(std::move(new_data));

            }

            using StandardMemControlledVectorOperator<T>::get;
            using StandardMemControlledVectorOperator<T>::get_data;
            using StandardMemControlledVectorOperator<T>::length;
            using StandardMemControlledVectorOperator<T>::sizeof_slot;
            using StandardMemControlledVectorOperator<T>::set;
            using StandardMemControlledVectorOperator<T>::to_operatable_vector;
            using StandardMemControlledVectorOperator<T>::to_vector_readable;

    };

    template <class T>
    class RelaxReallocatableMemControlledVectorOperator: public StandardMemControlledVectorOperator<T>, public ReallocatableOperatableVector<RelaxReallocatableMemControlledVectorOperator<T>>{

        public:

            RelaxReallocatableMemControlledVectorOperator() = default;

            RelaxReallocatableMemControlledVectorOperator(size_t sz): StandardMemControlledVectorOperator<T>(sz){}

            void resize(size_t sz){

                if (this->length() >= sz){
                    
                    StandardVectorOperator::operator=(StandardVectorOperator((size_t *) this->get_data(), sz));
                    
                    return;

                }

                memory::linear::Reallocatable<T> * casted = static_cast<memory::linear::Reallocatable<T>*>(memory::linear::AllocatorSingleton<T>::get());
                void * ptr = casted->realloc(this->get_data(), sz * sizeof(size_t));
                StandardVectorOperator::operator=(StandardVectorOperator((size_t *) ptr, sz));

            }

            void resize_no_copy(size_t sz){
                
                if (this->length() >= sz){
                    
                    StandardVectorOperator::operator=(StandardVectorOperator((size_t *) this->get_data(), sz));

                    return;

                }

                size_t * new_buf = (size_t *) memory::linear::AllocatorSingleton<T>::get()->malloc(sz * sizeof(size_t));
                StandardMemControlledVectorOperator<T>::operator=(StandardMemControlledVectorOperator<T>(new_buf, sz));

            }

            using StandardMemControlledVectorOperator<T>::get;
            using StandardMemControlledVectorOperator<T>::get_data;
            using StandardMemControlledVectorOperator<T>::length;
            using StandardMemControlledVectorOperator<T>::sizeof_slot;
            using StandardMemControlledVectorOperator<T>::set;
            using StandardMemControlledVectorOperator<T>::to_operatable_vector;
            using StandardMemControlledVectorOperator<T>::to_vector_readable;

    };

    template <class ID>
    class StandardDanglingOperatableVectorGenerator: public DanglingOperatableVectorGeneratable<StandardDanglingOperatableVectorGenerator<ID>>{

        public:

            StandardVectorOperator from_ptr(void * data, size_t sz){

                return StandardVectorOperator((size_t *) data, sz); 

            }

    };

    template <class ID>
    class StandardDanglingDefaultInitOperatableVectorGenerator: public DanglingOperatableVectorGeneratable<StandardDanglingDefaultInitOperatableVectorGenerator<ID>>{

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

                    this->small_view = *obj.small_view.to_vector_readable();
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

                    this->small_view = *obj.small_view.to_vector_readable();
                    StandardVectorView::operator=(StandardVectorView((size_t *) this->small_view.get_data(), this->small_view.length())); 
                    this->is_stack_allocated = true;

                } else{

                    StandardVectorView::operator=(std::move(obj));
                    this->is_stack_allocated = false;
                    
                }

                return *this;

            }

    };

    template <class ID>
    class StandardVectorReadableGenerator: public VectorReadableGeneratable<StandardVectorReadableGenerator<ID>>{

        public:

            template <class T>
            auto from_vec_readable(VectorReadable<T>& data){

                return StandardVectorView((size_t *) data.get_data(), data.length()); 

            } 

            auto from_ptr(void * data, size_t sz){

                return StandardVectorView((size_t *) data, sz); 

            }

    };

    template <class ID>
    class StandardStackVectorReadableGenerator: public StackVectorReadableGeneratable<StandardStackVectorReadableGenerator<ID>>{

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

    template <class T>
    class CustomAllocatorReallocatableVector: public StandardVectorOperator, public ReallocatableOperatableVector<CustomAllocatorReallocatableVector<T>>{ //relax resize

        private:

            memory::linear::Reallocatable<T> * allocator;

        public:

            CustomAllocatorReallocatableVector(memory::linear::Reallocatable<T> * allocator): StandardVectorOperator(nullptr, 0){
                
                this->allocator = allocator;

            }
            
            ~CustomAllocatorReallocatableVector(){

                this->allocator->free(this->get_data());

            }

            using StandardVectorOperator::get;
            using StandardVectorOperator::get_data;
            using StandardVectorOperator::length;
            using StandardVectorOperator::set;
            using StandardVectorOperator::sizeof_slot;
            using StandardVectorOperator::to_vector_readable;
            using StandardVectorOperator::to_operatable_vector;

            void resize(size_t n){

                if (n <= StandardVectorOperator::length()){
                    
                    StandardVectorOperator::operator=(StandardVectorOperator((size_t *) StandardVectorOperator::get_data(), n));
                    
                    return;

                }

                size_t * buf = (size_t *) this->allocator->realloc(StandardVectorOperator::get_data(), n * sizeof(size_t));                
                StandardVectorOperator::operator=(StandardVectorOperator(buf, n));
                
            }

            void resize_no_copy(size_t n){
                
                if (n <= StandardVectorOperator::length()){
                
                    StandardVectorOperator::operator=(StandardVectorOperator((size_t *) StandardVectorOperator::get_data(), n));
                    
                    return;
                
                }
                
                this->allocator->free(StandardVectorOperator::get_data());
                size_t * buf = (size_t *) this->allocator->malloc(n * sizeof(size_t));
                StandardVectorOperator::operator=(StandardVectorOperator(buf, n));

            }

    };

    template <class ID>
    class VectorReadableSplitter: public VectorReadableSplittable<VectorReadableSplitter<ID>>{

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
        
        template <class ID>
        class StandardBoolVectorViewCaster: public BoolVectorViewCastable<StandardBoolVectorViewCaster<ID>>{

            public:

                template <class T>
                StandardBoolVectorView cast(VectorReadable<T>& data){

                    return StandardBoolVectorView(data);  

                }

        };

        template <class ID>
        class StandardBoolVectorOpCaster: public BoolVectorOpCastable<StandardBoolVectorOpCaster<ID>>{

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

    template <class ID>
    class StandardExponentialLinearTempStorageGenerator: public TempStorageGeneratable<StandardExponentialLinearTempStorageGenerator<ID>>{
        
        private:

            const size_t DEFAULT_BUF = 1024; 
            
            std::shared_ptr<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator<ID>, StandardDanglingDefaultInitOperatableVectorGenerator<ID>>> allocator;

        public:

            StandardExponentialLinearTempStorageGenerator(){
                    
                auto readable_gen = std::make_shared<StandardVectorReadableGenerator<ID>>();
                auto operatable_gen = std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator<ID>>();
                
                auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                auto casted_operatable_gen = operatable_gen->to_dangling_operatable_vector_generatable_sp(operatable_gen); 

                this->allocator = std::make_shared<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator<ID>, StandardDanglingDefaultInitOperatableVectorGenerator<ID>>>(DEFAULT_BUF, casted_readable_gen, casted_operatable_gen);

            }

            StandardExponentialLinearTempStorageGenerator(size_t base_length){

                auto readable_gen = std::make_shared<StandardVectorReadableGenerator<ID>>();
                auto operatable_gen = std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator<ID>>();
                
                auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                auto casted_operatable_gen = operatable_gen->to_dangling_operatable_vector_generatable_sp(operatable_gen); 

                this->allocator = std::make_shared<ExponentialLinearBlockTempStorageGenerator<StandardVectorReadableGenerator<ID>, StandardDanglingDefaultInitOperatableVectorGenerator<ID>>>(base_length, casted_readable_gen, casted_operatable_gen);

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
    class TempStorageAllocator: public memory::linear::Reallocatable<TempStorageAllocator<T>>{

        private:

            std::shared_ptr<TempStorageGeneratable<T>> temp_storage;

        public:

            TempStorageAllocator(std::shared_ptr<TempStorageGeneratable<T>> temp_storage){

                this->temp_storage = temp_storage;

            }

            void * malloc(size_t n){
                
                size_t total_buf_n = n + sizeof(size_t); 
                void * buf = this->temp_storage->get(total_buf_n);
                std::memcpy(buf, &n, sizeof(size_t));

                return (char *) buf + sizeof(size_t);

            }

            void * realloc(void * ptr, size_t n){
                
                if (ptr == nullptr){
                    
                    return this->malloc(n);

                }

                size_t prev_n = 0;
                void * src = (char *) ptr - sizeof(size_t); 

                std::memcpy(&prev_n, src, sizeof(size_t));
                
                if (n <= prev_n){

                    return ptr;

                } 
                                
                void * new_buf = this->malloc(n);
                std::memcpy(new_buf, ptr, prev_n);

                return new_buf;


            }

            void free(void *){}
            
    };

    template <class T, class ID>
    class ReallocatableVectorGenerator: public ReallocatableVectorGeneratable<ReallocatableVectorGenerator<T, ID>>{

        private:

            std::shared_ptr<memory::linear::Reallocatable<T>> allocator;

        public:

            ReallocatableVectorGenerator() = default;
            
            ReallocatableVectorGenerator(std::shared_ptr<memory::linear::Reallocatable<T>> allocator){

                this->allocator = allocator;

            } 

            auto get(){

                return CustomAllocatorReallocatableVector(&*this->allocator);

            }

    };

    class BitIteratorBase{

        protected:

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

            template <class T>
            size_t read(VectorReadable<T>& data, size_t bob_slot, size_t bob_offset, size_t eor_slot, size_t eor_offset){
                
                const size_t MIN_BIT_IDX = 0;
                const size_t MAX_BIT_IDX = BIT_WIDTH - 1;

                if (bob_slot == eor_slot){

                    return this->extract(data.get(bob_slot), bob_offset, eor_offset);

                } 

                size_t delta = BIT_WIDTH - bob_offset; 
                size_t eor_data = this->extract(data.get(eor_slot), MIN_BIT_IDX, eor_offset);
                size_t bob_data = this->extract(data.get(bob_slot), bob_offset, MAX_BIT_IDX);

                return (eor_data << delta) | bob_data;  

            }

    };

    class DynamicBitIterator: public DynamicBitIterable<DynamicBitIterator>, protected BitIteratorBase{

        private:

            size_t bob_slot;
            size_t bob_offset;
        
        public:

            DynamicBitIterator(){

                this->bob_slot = 0;
                this->bob_offset = 0;

            }

            DynamicBitIterator(size_t offset){

                this->bob_slot = offset / BIT_WIDTH;
                this->bob_offset = offset % BIT_WIDTH;

            }

            template <class T>
            bool next(VectorReadable<T>& data, size_t& rs, size_t read_length){ 
                
                if (this->bob_slot >= data.length()){

                    return false;

                }

                size_t eor_slot = this->bob_slot;
                size_t eor_offset = this->bob_offset + read_length - 1;

                if (eor_offset >= BIT_WIDTH){

                    ++eor_slot;
                    eor_offset -= BIT_WIDTH;

                    if (eor_slot == data.length()){

                        eor_slot -= 1;
                        eor_offset = BIT_WIDTH - 1;

                    }

                } 

                rs = this->read(data, this->bob_slot, this->bob_offset, eor_slot, eor_offset);
                
                this->bob_slot = eor_slot;
                this->bob_offset = eor_offset + 1;

                if (this->bob_offset >= BIT_WIDTH){

                    ++this->bob_slot;
                    this->bob_offset -= BIT_WIDTH;

                }
                
                return true;

            }

    };

    class ReverseDynamicBitIterator: public DynamicBitIterable<ReverseDynamicBitIterator>, protected BitIteratorBase{

        private:

            intmax_t bob_slot;
            intmax_t bob_offset;
        
        public:

            ReverseDynamicBitIterator(){

                this->bob_slot = -1;
                this->bob_offset = 0;

            }

            ReverseDynamicBitIterator(size_t idx){

                this->bob_slot = idx / BIT_WIDTH;
                this->bob_offset = idx % BIT_WIDTH;

            }

            template <class T>
            bool next(VectorReadable<T>& data, size_t& rs, size_t read_length){ 

                if (this->bob_slot < 0){

                    return false; 

                }

                intmax_t eor_slot = bob_slot;
                intmax_t eor_offset = this->bob_offset - read_length + 1;

                if (eor_offset < 0){

                    --eor_slot;
                    eor_offset += BIT_WIDTH;

                    if (eor_slot == -1){

                        eor_slot = 0;
                        eor_offset = 0;

                    }

                } 

                rs = this->read(data, eor_slot, eor_offset, this->bob_slot, this->bob_offset);
                
                this->bob_slot = eor_slot;
                this->bob_offset = eor_offset - 1;

                if (bob_offset < 0){

                    --this->bob_slot;
                    this->bob_offset += BIT_WIDTH;

                }

                return true;

            }

    };

    template <class ID>
    class StandardDynamicBitIteratorGenerator: public DynamicBitIteratorGeneratable<StandardDynamicBitIteratorGenerator<ID>>{

        public:

            DynamicBitIterator get(){

                return DynamicBitIterator();

            }

            DynamicBitIterator get(size_t offset){

                return DynamicBitIterator(offset);

            }

    };

    template <class ID>
    class ReverseDynamicBitIteratorGenerator: public DynamicBitIteratorGeneratable<ReverseDynamicBitIteratorGenerator<ID>>{

        public:

            ReverseDynamicBitIterator get(){

                return ReverseDynamicBitIterator();

            }

            ReverseDynamicBitIterator get(size_t offset){

                return ReverseDynamicBitIterator(offset);

            }

    };

    class DynamicBitIterOrWriter: public DynamicBitIterWritable<DynamicBitIterOrWriter>{

        private:

            size_t bob_slot;
            size_t bob_offset;
        
        public:

            DynamicBitIterOrWriter(){

                this->bob_slot = 0;
                this->bob_offset = 0;

            }

            DynamicBitIterOrWriter(size_t offset){

                this->bob_slot = offset / BIT_WIDTH;
                this->bob_offset = offset % BIT_WIDTH;

            }

            template <class T>
            bool write(OperatableVector<T>& data, size_t val, size_t write_length){
                
                if (this->bob_slot >= data.length()){
                    
                    return false;

                }
                
                size_t eor_slot = bob_slot;
                size_t eor_offset = this->bob_offset + write_length - 1;

                if (eor_offset >= BIT_WIDTH){

                    ++eor_slot;
                    eor_offset -= BIT_WIDTH;

                    if (eor_slot == data.length()){

                        --eor_slot;
                        eor_offset = BIT_WIDTH - 1;
                        write_length = BIT_WIDTH - this->bob_offset;

                    }

                }

                this->write(data, val, this->bob_slot, this->bob_offset, eor_slot, eor_offset, write_length);
                
                this->bob_slot = eor_slot;
                this->bob_offset = eor_offset + 1;

                if (this->bob_offset >= BIT_WIDTH){

                    ++this->bob_slot;
                    this->bob_offset -= BIT_WIDTH;

                }

                return true; 

            }
        
        private:

            size_t get_extractor(size_t length){

                if (length == BIT_WIDTH){

                    return std::numeric_limits<size_t>::max();

                }

                return ((size_t) 1 << length) - 1;

            } 

            template <class T>
            void write(OperatableVector<T>& data, size_t val, size_t bob_slot, size_t bob_offset, size_t eor_slot, size_t eor_offset, size_t length){

                val = val & this->get_extractor(length); 

                if (bob_slot == eor_slot){
                    
                    data.set(bob_slot, data.get(bob_slot) | (val << bob_offset));

                    return;

                } 

                size_t lo_length = BIT_WIDTH - bob_offset;
                size_t lo_val = this->get_extractor(lo_length) & val;
                size_t lo_adjusted_val = data.get(bob_slot) | (lo_val << bob_offset);
                size_t hi_adjusted_val = data.get(eor_slot) | (val >> lo_length);

                data.set(bob_slot, lo_adjusted_val);
                data.set(eor_slot, hi_adjusted_val);

            }

    };
    
    class DynamicBitIterReplaceWriterBase{

        protected:

            size_t get_extractor(size_t length){

                if (length == BIT_WIDTH){

                    return std::numeric_limits<size_t>::max();

                }

                return ((size_t) 1 << length) - 1;

            } 

            size_t get_erasor(size_t length){

                return this->get_extractor(BIT_WIDTH - length) << length;

            }

            size_t get_erasor(size_t idx, size_t length){

                return std::numeric_limits<size_t>::max() ^ (this->get_extractor(length) << idx);


            }

            template <class T>
            void write(OperatableVector<T>& data, size_t val, size_t bob_slot, size_t bob_offset, size_t eor_slot, size_t eor_offset, size_t length){
                                
                val = val & this->get_extractor(length); 

                if (bob_slot == eor_slot){
                    
                    size_t erasor = this->get_erasor(bob_offset, length);
                    data.set(bob_slot, (data.get(bob_slot) & erasor) | (val << bob_offset));

                    return;

                } 

                size_t lo_length = BIT_WIDTH - bob_offset;
                size_t hi_length = length - lo_length;
                size_t lo_erasor = this->get_extractor(bob_offset);
                size_t hi_erasor = this->get_erasor(hi_length); 

                size_t lo_val = this->get_extractor(lo_length) & val;
                size_t lo_adjusted_val = (data.get(bob_slot) & lo_erasor) | (lo_val << bob_offset);
                size_t hi_adjusted_val = (data.get(eor_slot) & hi_erasor) | (val >> lo_length);

                data.set(bob_slot, lo_adjusted_val);
                data.set(eor_slot, hi_adjusted_val);

            }
    };

    class DynamicBitIterReplaceWriter: public DynamicBitIterWritable<DynamicBitIterReplaceWriter>,
                                       private DynamicBitIterReplaceWriterBase{

        
        private:

            size_t bob_offset;
            size_t bob_slot; 

        public:

            DynamicBitIterReplaceWriter(){

                this->bob_offset = 0;
                this->bob_slot = 0;
                
            }

            DynamicBitIterReplaceWriter(size_t idx){

                this->bob_offset = idx % BIT_WIDTH;
                this->bob_slot = idx / BIT_WIDTH;

            }

            template <class T>
            bool write(OperatableVector<T>& data, size_t val, size_t write_length){
                                
                if (this->bob_slot >= data.length()){
                    
                    return false;

                }
                
                size_t eor_slot = this->bob_slot;
                size_t eor_offset = this->bob_offset + write_length - 1; 

                if (eor_offset >= BIT_WIDTH){

                    ++eor_slot;
                    eor_offset -= BIT_WIDTH;

                    if (eor_slot == data.length()){

                        --eor_slot;
                        eor_offset = BIT_WIDTH - 1; 
                        
                        write_length = BIT_WIDTH - this->bob_offset;

                    }

                }

                DynamicBitIterReplaceWriterBase::write(data, val, this->bob_slot, this->bob_offset, eor_slot, eor_offset, write_length);
                
                this->bob_slot = eor_slot;
                this->bob_offset = eor_offset + 1;

                if (this->bob_offset >= BIT_WIDTH){

                    ++this->bob_slot;
                    this->bob_offset -= BIT_WIDTH;

                }

                return true; 

            }

    };

    class ReverseDynamicBitIterReplaceWriter: public DynamicBitIterWritable<ReverseDynamicBitIterReplaceWriter>, 
                                              private DynamicBitIterReplaceWriterBase{
        
        private:

            intmax_t bob_slot;
            intmax_t bob_offset;

        public:

            ReverseDynamicBitIterReplaceWriter(){

                this->bob_slot = -1;
                this->bob_offset = -1;

            }

            ReverseDynamicBitIterReplaceWriter(size_t idx){

                this->bob_slot = idx / BIT_WIDTH;
                this->bob_offset = idx % BIT_WIDTH;

            }

            template <class T>
            bool write(OperatableVector<T>& data, size_t val, size_t write_length){

                if (this->bob_slot < 0){

                    return false; 

                }

                intmax_t eor_slot = this->bob_slot;
                intmax_t eor_offset = this->bob_offset - write_length + 1;

                if (eor_offset < 0){

                    --eor_slot;
                    eor_offset += BIT_WIDTH;

                    if (eor_slot == -1){

                        eor_slot = 0;
                        eor_offset = 0;
                        write_length = this->bob_offset + 1; 

                    }

                }

                DynamicBitIterReplaceWriterBase::write(data, val, eor_slot, eor_offset, this->bob_slot, this->bob_offset, write_length); 

                this->bob_slot = eor_slot;
                this->bob_offset = eor_offset - 1;

                if (this->bob_offset < 0){

                    --this->bob_slot;
                    this->bob_offset += BIT_WIDTH;

                } 

                return true;

            }

    };

    template <class ID>
    class DynamicBitIterEmptyWriterGenerator: public DynamicBitIterWriterGeneratable<DynamicBitIterEmptyWriterGenerator<ID>>{

        public:

            DynamicBitIterOrWriter get(size_t offset){ 

                return DynamicBitIterOrWriter(offset);

            }

    };

    template <class ID>
    class DynamicBitIterReplaceWriterGenerator: public DynamicBitIterWriterGeneratable<DynamicBitIterReplaceWriterGenerator<ID>>{

        public:

            DynamicBitIterReplaceWriter get(size_t offset){ 

                return DynamicBitIterReplaceWriter(offset);

            }

    };

    template <class ID>
    class ReverseDynamicBitIterReplaceWriterGenerator: public DynamicBitIterWriterGeneratable<ReverseDynamicBitIterReplaceWriterGenerator<ID>>{

        public:

            ReverseDynamicBitIterReplaceWriter get(size_t offset){ 

                return ReverseDynamicBitIterReplaceWriter(offset);

            }

    };

    
    using namespace caster::plm_boolvector;
    using namespace caster;

    class GenericID{};

    class StandardGenerator{

        public:

            auto get_temp_storage(size_t n){

                return std::make_shared<StandardExponentialLinearTempStorageGenerator<GenericID>>(n);

            }

            auto get_dangling_op_vec_default_init_gen(){

                return std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator<GenericID>>();

            }

            auto get_stack_allocated_addable_vector_gen(){

                return std::make_shared<StackAllocatedAddableVectorGenerator<GenericID>>();

            }

            auto get_dangling_op_vec_gen(){

                return std::make_shared<StandardDanglingOperatableVectorGenerator<GenericID>>();

            }

            auto get_dangling_read_vec_gen(){

                return std::make_shared<StandardVectorReadableGenerator<GenericID>>();

            } 

            auto get_dangling_stack_read_vec_gen(){

                return std::make_shared<StandardStackVectorReadableGenerator<GenericID>>();

            } 
            
            auto get_vector_splitter(){

                return std::make_shared<VectorReadableSplitter<GenericID>>();

            }

            auto get_boolvector_view_caster(){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorViewCaster<GenericID>>();

            }

            auto get_boolvector_op_caster(){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorOpCaster<GenericID>>();

            }

            auto get_dynamic_bit_or_writer_gen(){
                
                return std::make_shared<DynamicBitIterEmptyWriterGenerator<GenericID>>();

            }

            auto get_dynamic_bit_replace_writer_gen(){
                
                return std::make_shared<DynamicBitIterReplaceWriterGenerator<GenericID>>();

            }

            auto get_reverse_dynamic_bit_replace_write_gen(){

                return std::make_shared<ReverseDynamicBitIterReplaceWriterGenerator<GenericID>>();

            }

            auto get_dynamic_bit_iter_gen(){

                return std::make_shared<StandardDynamicBitIteratorGenerator<GenericID>>();

            }

            auto get_reverse_dynamic_bit_iter_gen(){

                return std::make_shared<ReverseDynamicBitIteratorGenerator<GenericID>>();

            }

            template <class T>
            auto get_reallocatable_vector_generator(std::shared_ptr<memory::linear::Reallocatable<T>> allocator){

                return std::make_shared<ReallocatableVectorGenerator<T, GenericID>>(allocator);

            }

            template <class T>
            auto get_temp_gen_realloc_vector_generator(std::shared_ptr<TempStorageGeneratable<T>> temp_storage){ // warning: vector and generator obj coexists 

                auto wrapper = std::make_shared<TempStorageAllocator<T>>(temp_storage);
                auto casted = wrapper->to_reallocatable_sp(wrapper);
                
                return this->get_reallocatable_vector_generator(casted);

            }

    };  

    class IDGenerator{

        public:

            template <class ID>
            auto get_temp_storage(size_t n, ID){

                return std::make_shared<StandardExponentialLinearTempStorageGenerator<ID>>(n);

            }

            template <class ID>
            auto get_dangling_op_vec_default_init_gen(ID){

                return std::make_shared<StandardDanglingDefaultInitOperatableVectorGenerator<ID>>();

            }

            template <class ID>
            auto get_stack_allocated_addable_vector_gen(ID){

                return std::make_shared<StackAllocatedAddableVectorGenerator<ID>>();

            }

            template <class ID>
            auto get_dangling_op_vec_gen(ID){

                return std::make_shared<StandardDanglingOperatableVectorGenerator<ID>>();

            }

            template <class ID>
            auto get_dangling_read_vec_gen(ID){

                return std::make_shared<StandardVectorReadableGenerator<ID>>();

            } 

            template <class ID>
            auto get_dangling_stack_read_vec_gen(ID){

                return std::make_shared<StandardStackVectorReadableGenerator<ID>>();

            } 

            template <class ID>
            auto get_vector_splitter(ID){

                return std::make_shared<VectorReadableSplitter<ID>>();

            }

            template <class ID>
            auto get_boolvector_view_caster(ID){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorViewCaster<ID>>();

            }

            template <class ID>
            auto get_boolvector_op_caster(ID){

                return std::make_shared<caster::plm_boolvector::StandardBoolVectorOpCaster<ID>>();

            }

            template <class ID>
            auto get_dynamic_bit_or_writer_gen(ID){
                
                return std::make_shared<DynamicBitIterEmptyWriterGenerator<ID>>();

            }

            template <class ID>
            auto get_dynamic_bit_replace_writer_gen(ID){
                
                return std::make_shared<DynamicBitIterReplaceWriterGenerator<ID>>();

            }

            template <class ID>
            auto get_reverse_dynamic_bit_replace_write_gen(ID){

                return std::make_shared<ReverseDynamicBitIterReplaceWriterGenerator<ID>>();

            }

            template <class ID>
            auto get_dynamic_bit_iter_gen(ID){

                return std::make_shared<StandardDynamicBitIteratorGenerator<ID>>();

            }

            template <class ID>
            auto get_reverse_dynamic_bit_iter_gen(ID){

                return std::make_shared<ReverseDynamicBitIteratorGenerator<ID>>();

            }

            template <class T, class ID>
            auto get_reallocatable_vector_generator(std::shared_ptr<memory::linear::Reallocatable<T>> allocator, ID){

                return std::make_shared<ReallocatableVectorGenerator<T, ID>>(allocator);

            }

            template <class T, class ID>
            auto get_temp_gen_realloc_vector_generator(std::shared_ptr<TempStorageGeneratable<T>> temp_storage, ID id_){ // warning: vector and generator obj coexists 

                auto wrapper = std::make_shared<TempStorageAllocator<T>>(temp_storage);
                auto casted = wrapper->to_reallocatable_sp(wrapper);
                
                return this->get_reallocatable_vector_generator(casted, id_);

            }

    };  


}


#endif                   