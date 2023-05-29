
#ifndef __LINEAR_MEM_H__
#define __LINEAR_MEM_H__

#include "BoolVector.h"
#include <vector>
#include <memory>
#include <iostream>

namespace memory{
    
    namespace linear{

        class Allocatable{

            public:

                virtual void * malloc(size_t) = 0;
                virtual void free(void *) = 0;

        };

        class AlignmentAdjustable{

            public:

                virtual size_t get_true_size(size_t, size_t) = 0; 
                virtual void * align(void *, size_t) = 0;

        };

        class TempStorageGeneratable{

            public:

                virtual void enter_scope() = 0;
                
                virtual void exit_scope(bool shrinked = true) = 0;
                virtual void * exit_scope(void *, size_t, bool shrinked = true) = 0;

                virtual void * get(size_t) = 0;               
                virtual void shrink() = 0;

        };

        class AlignedTempStorageGeneratable: public virtual TempStorageGeneratable{

            public:

                virtual void * exit_scope_align(void *, size_t, size_t, bool shrinked = true) = 0;
                virtual void * get_align(size_t, size_t) = 0; 

        };

        class MainScopeAllocatableTempStorageGeneratable: public virtual TempStorageGeneratable, public virtual Allocatable{};

    };

    namespace sizet_linear{
        
        class VectorReadable{

            public:

                virtual size_t get(size_t) = 0;
                virtual size_t length() = 0; 
                virtual void * get_data() = 0;
                virtual size_t sizeof_slot() = 0;

        };

        class OperatableVector: public virtual VectorReadable{

            public:

                virtual void set(size_t, size_t) = 0;

        };

        class ReallocatableOperatableVector: public virtual OperatableVector{

            public:

                virtual void resize(size_t) = 0;

        };

        class TempStorageGeneratable: public virtual linear::TempStorageGeneratable{

            public:

                using linear::TempStorageGeneratable::exit_scope;

                virtual VectorReadable * exit_scope(VectorReadable&) = 0;
                virtual OperatableVector * exit_scope(OperatableVector&) = 0;

                virtual OperatableVector * vec_get(size_t) = 0; 
                virtual OperatableVector * vec_get_default(size_t) = 0;
                
        };

        class TempStorageInjectible{

            public:

                virtual void set_temp_storage(TempStorageGeneratable *) = 0;

        };
        
        class DanglingOperatableVectorGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual OperatableVector * from_ptr(void *, size_t) = 0;
                virtual OperatableVector * from_ptr(void *, size_t, void *) = 0;

        };

        class VectorReadableGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual VectorReadable * from_ptr(void *, size_t) = 0;
                virtual VectorReadable * from_ptr(void *, size_t, void *) = 0;

        };

        class OperatableVectorGeneratable: public virtual DanglingOperatableVectorGeneratable{

            public:

                virtual OperatableVector * get(size_t) = 0; 

        };

        class VectorReadableSplittable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual std::pair<VectorReadable *, VectorReadable *> split(VectorReadable&, size_t) = 0;
                virtual std::pair<VectorReadable *, VectorReadable *> split(VectorReadable&, size_t, void *) = 0;

        };

        class DynamicBitIterable{
            
            public:

                virtual bool next(size_t&, size_t) = 0;

        };

        class DynamicBitIteratorGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual DynamicBitIterable * get(VectorReadable&, void *) = 0;

        };

        class BitIterable{

            public:

                virtual bool next(size_t&) = 0;

        };

        class BitIteratorGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual BitIterable * get(VectorReadable&, size_t, void *) = 0; 

        };
        
        class DynamicBitIterWritable{

            public:

                virtual bool write(size_t, size_t) = 0;

        };

        class BitIterWritable{

            public:

                virtual bool write(size_t) = 0;

        };

        class DynamicBitIterWriterGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual DynamicBitIterWritable * get(OperatableVector&, size_t, void *) = 0; 
                
        };

        class BitIterWriterGeneratable{

            public:

                virtual size_t get_instantiation_cost() = 0;
                virtual BitIterWritable * get(OperatableVector&, size_t, void *) = 0; 
                
        };

        namespace caster{
            
            namespace boolvector{
                
                class BoolVectorCastable{

                    public:

                        virtual datastructure::UnalignedViewableBoolVector * cast(VectorReadable&) = 0;

                };

                class BoolVectorEndianCastable{

                    public:

                        virtual datastructure::UnalignedViewableBoolVector * le_cast(VectorReadable&) = 0;
                        virtual datastructure::UnalignedViewableBoolVector * be_cast(VectorReadable&) = 0;

                };

            }

            namespace plm_boolvector{

                class BoolVectorViewCastable{

                    public:

                        virtual size_t get_instantiation_cost() = 0;
                        virtual datastructure::UnalignedViewableBoolVector * cast(VectorReadable&, void *) = 0; 

                };

                class BoolVectorOpCastable{

                    public:

                        virtual size_t get_instantiation_cost() = 0;
                        virtual datastructure::UnalignedOperatableBoolVector * cast(OperatableVector&, void *) = 0;

                };

            }

        };

    };

}

namespace memory::linear{

    class StandardAlignmentAdjuster: public virtual AlignmentAdjustable{

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

    class ExponentialLinearStorageBlockGenerator: public virtual TempStorageGeneratable{

        protected:

            std::vector<void *> buf; 
            std::vector<std::pair<size_t, size_t>> enter_points; 
            
            size_t slot; 
            size_t offset;
            size_t base_length;

        public:

            ExponentialLinearStorageBlockGenerator(size_t base_length){

                this->base_length = base_length;
                this->slot = 0;
                this->offset = 0;

                this->enter_points = {};
                this->buf = {};

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

    class AlignedExponentialLinearStorageBlockGenerator: public ExponentialLinearStorageBlockGenerator, public virtual AlignedTempStorageGeneratable{

        private:

            std::unique_ptr<AlignmentAdjustable> alignment_adjuster; 

        public:

            AlignedExponentialLinearStorageBlockGenerator(size_t base_length): ExponentialLinearStorageBlockGenerator(base_length){
               
                this->alignment_adjuster = std::make_unique<StandardAlignmentAdjuster>();

            }

            void * get_align(size_t sz, size_t alignment){

                size_t true_size = this->alignment_adjuster->get_true_size(sz, alignment);
                void * rs = ExponentialLinearStorageBlockGenerator::get(true_size); 

                return this->alignment_adjuster->align(rs, alignment);

            }

            void * exit_scope_align(void * data, size_t sz, size_t alm, bool shrinked = true){
                
                ExponentialLinearStorageBlockGenerator::exit_scope(false);
                void * cp_data = this->get_align(sz, alm);

                if (cp_data != data){

                    std::memcpy(cp_data, data, sz);

                }

                if (shrinked){

                    ExponentialLinearStorageBlockGenerator::shrink();

                }

                return cp_data;

            }

    };
}

namespace memory::sizet_linear{

    inline static const uint8_t DEFAULT_VAL = 0;
    inline static const uint8_t BIT_WIDTH = sizeof(size_t) * 8; 

    namespace utility{

        static inline size_t get_max_sizet_val(){

            if (sizeof(size_t) == 4){

                return 0xFFFFFFFF; 

            }

            if (sizeof(size_t) == 8){

                return 0xFFFFFFFFFFFFFFFF;

            }

            assert(false);

        }

    }

    class TempStorageInjector: public virtual memory::sizet_linear::TempStorageInjectible{

        protected:

            memory::sizet_linear::TempStorageGeneratable * allocator;
        
        public:

            TempStorageInjector(){

                this->allocator = nullptr;
            
            }

            TempStorageInjector(memory::sizet_linear::TempStorageGeneratable * allocator){

                this->allocator = allocator;

            }

            void set_temp_storage(memory::sizet_linear::TempStorageGeneratable * allocator){

                this->allocator = allocator;

            }

    };

    class StandardVectorView: public virtual VectorReadable{

        protected:

            size_t * data; 
            size_t sz; 

        public:

            StandardVectorView(size_t * data, size_t sz){

                this->data = data;
                this->sz = sz;

            }

            StandardVectorView(StandardVectorView& obj){

                this->data = data;
                this->sz = sz;

            }

            StandardVectorView(StandardVectorView&& obj){

               *this = std::move(obj);

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

            size_t sizeof_slot(){

                return sizeof(size_t);

            }

    };

    class StandardOffsetVectorView: public virtual VectorReadable{

        private:    

            VectorReadable * data;
            
            size_t offset;
            size_t sz;
        
        public:

            StandardOffsetVectorView(VectorReadable * data, size_t offset, size_t sz){

                this->data = data;

                this->offset = offset;
                this->sz = sz;

            }

            size_t get(size_t idx){

                return this->data->get(idx + this->offset); 

            }

            void * get_data(){

                return this->data + this->offset;

            }

            size_t length(){

                return this->sz; 

            }

            size_t sizeof_slot(){

                return this->data->sizeof_slot();

            }

    };

    class StandardVectorOperator: public virtual OperatableVector, public StandardVectorView{

        public:

            StandardVectorOperator(size_t * data, size_t sz): StandardVectorView(data, sz){};
            StandardVectorOperator(StandardVectorOperator& cp): StandardVectorView(cp){};
            StandardVectorOperator(StandardVectorOperator&& obj): StandardVectorView(std::forward<StandardVectorView>(obj)){};

            StandardVectorOperator& operator =(StandardVectorOperator&& obj){

                StandardVectorView::operator=(std::move(obj));

                return *this;

            }

            void set(size_t idx, size_t val){

                this->data[idx] = val;

            }


    };

    class ReverseVectorView: public virtual OperatableVector{

        private:

            VectorReadable * data;
        
        public:

            ReverseVectorView(VectorReadable * data){

                this->data = data;

            }

            size_t get(size_t idx){
                
                return this->data->get(this->data->length() - 1 - idx);

            }

            size_t length(){

                return this->data->length();

            } 

            void * get_data(){

                return this->data->get_data();

            }

            size_t sizeof_slot(){

                return this->data->sizeof_slot();

            }
    };

    class ReverseVectorOperator: public ReverseVectorView, public virtual OperatableVector{

        private:

            OperatableVector * data;
        
        public:

            ReverseVectorOperator(OperatableVector * data): ReverseVectorView(data){

                this->data = data;

            }

            void set(size_t idx, size_t val){

                this->data->set(this->data->length() - 1 - idx, val);

            }
    

    };

    class StandardDefaultInitVectorOperator: public virtual OperatableVector, public StandardVectorOperator{

        public:

            StandardDefaultInitVectorOperator(size_t * data, size_t sz): StandardVectorOperator(data, sz){

                std::memset(this->data, sz * sizeof(size_t), DEFAULT_VAL);

            };

    };

    class StandardMemControlledVectorOperator: public virtual OperatableVector, public StandardVectorOperator{

        public:

            StandardMemControlledVectorOperator(size_t sz): StandardVectorOperator((size_t *) std::malloc(sz * sizeof(size_t)), sz){}
            StandardMemControlledVectorOperator(size_t * data, size_t sz): StandardVectorOperator(data, sz){};
            StandardMemControlledVectorOperator(StandardMemControlledVectorOperator&& mv): StandardVectorOperator(std::forward<StandardVectorOperator>(mv)){};

            StandardMemControlledVectorOperator& operator =(StandardMemControlledVectorOperator&& obj){

                std::free(this->data);
                StandardVectorOperator::operator=(std::move(obj));

                return *this;

            }

            ~StandardMemControlledVectorOperator(){

                std::free(this->data);

            }

    };

    class StandardMemControlledDefaultInitVectorOperator: public virtual OperatableVector, public StandardMemControlledVectorOperator{

        private:

            const uint8_t DEFAULT_VAL = 0;

        public:

            StandardMemControlledDefaultInitVectorOperator(size_t sz): StandardMemControlledVectorOperator(sz){

                std::memset(this->data, DEFAULT_VAL, sizeof(size_t) * sz);

            }

            StandardMemControlledDefaultInitVectorOperator(size_t * data, size_t sz): StandardMemControlledVectorOperator(data, sz){

                std::memset(this->data, DEFAULT_VAL, sizeof(size_t) * sz);

            }

    };

    class StandardReallocatableMemControlledVectorOperator: public virtual ReallocatableOperatableVector, public StandardMemControlledVectorOperator{

        public:

            StandardReallocatableMemControlledVectorOperator(size_t sz): StandardMemControlledVectorOperator(sz){};
            StandardReallocatableMemControlledVectorOperator(StandardReallocatableMemControlledVectorOperator&& data): StandardMemControlledVectorOperator(std::move(data)){};

            StandardReallocatableMemControlledVectorOperator& operator =(StandardReallocatableMemControlledVectorOperator&& obj){

                StandardMemControlledVectorOperator::operator=(std::move(obj));

                return *this;
                
            }

            void resize(size_t sz){

                size_t * new_data = (size_t *) std::malloc(sz * sizeof(size_t));
                
                memcpy(new_data, this->data, std::min(sz, this->sz));
                std::free(this->data);

                this->data = new_data;
                this->sz = sz;

            }


    };

    class StandardMemControlledOperatableVectorGenerator: public virtual OperatableVectorGeneratable{

        public:

            OperatableVector * get(size_t length){

                return new StandardMemControlledVectorOperator(length); 

            }

            size_t get_instantiation_cost(){

                return sizeof(StandardMemControlledVectorOperator); 

            }

            OperatableVector * from_ptr(void * data, size_t sz){

                return new StandardMemControlledVectorOperator((size_t *) data, sz); 

            }

            OperatableVector * from_ptr(void * data, size_t sz, void * plm_space){

                return new (plm_space) StandardMemControlledVectorOperator((size_t *) data, sz);

            };


    };

    class StandardDanglingOperatableVectorGenerator: public virtual DanglingOperatableVectorGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(StandardVectorOperator);

            }

            OperatableVector * from_ptr(void * data, size_t sz){

                return new StandardVectorOperator((size_t *) data, sz); 

            }

            OperatableVector * from_ptr(void * data, size_t sz, void * plm_space){

                return new (plm_space) StandardVectorOperator((size_t *) data, sz);

            }

    };

    class StandardDanglingDefaultInitOperatableVectorGenerator: public virtual DanglingOperatableVectorGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(StandardDefaultInitVectorOperator);

            }

            OperatableVector * from_ptr(void * data, size_t sz){

                return new StandardDefaultInitVectorOperator((size_t *) data, sz); 

            }

            OperatableVector * from_ptr(void * data, size_t sz, void * plm_space){

                return new (plm_space) StandardDefaultInitVectorOperator((size_t *) data, sz);
            }

    };

    class StandardVectorReadableGenerator: public virtual VectorReadableGeneratable{

        public:

            size_t get_instantiation_cost(){
                
                return sizeof(StandardVectorView);

            }

            VectorReadable * from_ptr(void * data, size_t sz){

                return new StandardVectorView((size_t *) data, sz); 

            }

            VectorReadable * from_ptr(void * data, size_t sz, void * plm_space){

                return new (plm_space) StandardVectorView((size_t *) data, sz);

            }
    };

    class StandardVectorReadableSplitter: public virtual VectorReadableSplittable{

        public:

            size_t get_instantiation_cost(){

                return 2 * sizeof(StandardOffsetVectorView); 

            }

            std::pair<VectorReadable *, VectorReadable *> split(VectorReadable& data, size_t split_point){

                size_t first_length = 0;
                size_t second_length = 0;

                this->get_length(data, split_point, first_length, second_length);

                return std::pair<VectorReadable *, VectorReadable *>(new StandardOffsetVectorView(&data, 0, first_length),
                                                                     new StandardOffsetVectorView(&data, first_length, second_length));

            }

            std::pair<VectorReadable *, VectorReadable *> split(VectorReadable& data, size_t split_point, void * plm_space){

                size_t first_length = 0;
                size_t second_length = 0;

                this->get_length(data, split_point, first_length, second_length);

                return std::pair<VectorReadable *, VectorReadable *>(new (plm_space) StandardOffsetVectorView(&data, 0, first_length),
                                                                     new ((char *) plm_space + sizeof(StandardOffsetVectorView)) StandardOffsetVectorView(&data, first_length, second_length));

            }

        private:

            void get_length(VectorReadable& data, size_t split_point, size_t& first_length, size_t& second_length){
                
                first_length = std::min(split_point, data.length());
                second_length = data.length() - first_length;

            }

    };

    class FastVectorReadableSplitter: public virtual VectorReadableSplittable{

        public:

            size_t get_instantiation_cost(){

                return 2 * sizeof(StandardVectorView); 

            }

            std::pair<VectorReadable *, VectorReadable *> split(VectorReadable& data, size_t split_point){

                size_t first_length = 0;
                size_t second_length = 0;

                this->get_length(data, split_point, first_length, second_length);

                return std::pair<VectorReadable *, VectorReadable *>(new StandardVectorView((size_t *) data.get_data(), first_length),
                                                                     new StandardVectorView((size_t *) data.get_data() + first_length, second_length));

            }

            std::pair<VectorReadable *, VectorReadable *> split(VectorReadable& data, size_t split_point, void * plm_space){

                size_t first_length = 0;
                size_t second_length = 0;

                this->get_length(data, split_point, first_length, second_length);

                return std::pair<VectorReadable *, VectorReadable *>(new (plm_space) StandardVectorView((size_t *) data.get_data(), first_length),
                                                                     new ((char *) plm_space + sizeof(StandardVectorView)) StandardVectorView((size_t *) data.get_data() + first_length, second_length));

            }

        private:

            void get_length(VectorReadable& data, size_t split_point, size_t& first_length, size_t& second_length){
                
                first_length = std::min(split_point, data.length());
                second_length = data.length() - first_length;

            }

    };

    class StandardBoolVectorView: public virtual datastructure::UnalignedViewableBoolVector{

        private:

            VectorReadable * data;
            
        public:

            StandardBoolVectorView(VectorReadable * data){

                this->data = data;

            }

            uint64_t length(){

                return this->data->length() * this->get_slot_bit_width();  

            }

            uint64_t byte_length(){

                uint64_t bit_length = this->length();

                if (bit_length % 8 == 0){
                    
                    return bit_length >> 3; 

                }

                return (bit_length >> 3) + 1;

            }

            bool get(uint64_t idx){

                return this->data->get(this->get_slot(idx)) & (1ULL << this->get_offset(idx));

            }

        protected:

            size_t get_slot(size_t idx){

                return idx / this->get_slot_bit_width();
            }

            size_t get_offset(size_t idx){

                return idx % this->get_slot_bit_width();

            }

            size_t get_slot_bit_width(){

                return this->data->sizeof_slot() * 8;

            }

            size_t get_slot_size(){

                return this->data->length();

            }

    };

    class StandardBoolVectorOperator: public virtual datastructure::UnalignedOperatableBoolVector, public virtual StandardBoolVectorView{

        private:

            OperatableVector * data; 

        public:

            StandardBoolVectorOperator(OperatableVector * data): StandardBoolVectorView(data){
                
                this->data = data; 

            }

            void set_val(uint64_t idx, bool val){
                
                size_t slot = this->get_slot(idx);
                size_t offset = this->get_offset(idx); 
                size_t updated = this->data->get(slot);

                if (val){

                    updated |= (1ULL << offset);                 
                    this->data->set(slot, updated);

                    return;

                }

                updated &= (utility::get_max_sizet_val() ^ 1ULL << offset);
                this->data->set(slot, updated);

            }

    };
    
    namespace caster::plm_boolvector{

        class StandardBoolVectorViewCaster: public virtual BoolVectorViewCastable{

            public:

                size_t get_instantiation_cost(){

                    return sizeof(StandardBoolVectorView);

                }

                datastructure::UnalignedViewableBoolVector * cast(VectorReadable& data, void * plm_space){

                    return new (plm_space) StandardBoolVectorView(&data);  

                }

        };

        class StandardBoolVectorOpCaster: public virtual BoolVectorOpCastable{

            public:

                size_t get_instantiation_cost(){

                    return sizeof(StandardBoolVectorOperator);

                }

                datastructure::UnalignedOperatableBoolVector * cast(OperatableVector& data, void * plm_space){

                    return new (plm_space) StandardBoolVectorOperator(&data);
                }

        };

    }

    class ExponentialLinearBlockTempStorageGenerator: public virtual TempStorageGeneratable, public linear::ExponentialLinearStorageBlockGenerator{

        protected:

            VectorReadableGeneratable * vec_generator;
            DanglingOperatableVectorGeneratable * op_vector_generator;

        public:

            ExponentialLinearBlockTempStorageGenerator(size_t base_length,
                                                       VectorReadableGeneratable * vec_generator,
                                                       DanglingOperatableVectorGeneratable * op_vector_generator) : linear::ExponentialLinearStorageBlockGenerator(base_length){
                
                this->vec_generator = vec_generator;
                this->op_vector_generator = op_vector_generator;

            };

            void exit_scope(){

                linear::ExponentialLinearStorageBlockGenerator::exit_scope();

            }

            VectorReadable * exit_scope(VectorReadable& data){

                void * data_buf = data.get_data();
                size_t data_length = data.length();

                data_buf = linear::ExponentialLinearStorageBlockGenerator::exit_scope(data_buf, data_length * sizeof(size_t));
                void * plm_buf = linear::ExponentialLinearStorageBlockGenerator::get(this->vec_generator->get_instantiation_cost());

                return this->vec_generator->from_ptr(data_buf, data_length, plm_buf); 

            }

            OperatableVector * exit_scope(OperatableVector& data){
                
                void * data_buf = data.get_data();
                size_t data_length = data.length();

                data_buf = linear::ExponentialLinearStorageBlockGenerator::exit_scope(data_buf, data_length * sizeof(size_t));
                void * plm_buf = linear::ExponentialLinearStorageBlockGenerator::get(this->op_vector_generator->get_instantiation_cost());

                return this->op_vector_generator->from_ptr(data_buf, data_length, plm_buf);

            }

            OperatableVector * vec_get(size_t sz){

                size_t data_size = sz * sizeof(size_t);
                size_t total_size = data_size + this->op_vector_generator->get_instantiation_cost();
                
                void * data_buf = linear::ExponentialLinearStorageBlockGenerator::get(total_size);
                void * plm_buf = (char *) data_buf + data_size;

                return this->op_vector_generator->from_ptr(data_buf, sz, plm_buf); 

            }

            OperatableVector * vec_get_default(size_t sz){

                auto rs = this->vec_get(sz);

                for (size_t i = 0; i < rs->length(); ++i){

                    rs->set(i, 0);

                }

                return rs;

            }

    };

    class StandardExponentialLinearTempStorageGenerator: public ExponentialLinearBlockTempStorageGenerator{

        public:

            StandardExponentialLinearTempStorageGenerator(size_t base_length): ExponentialLinearBlockTempStorageGenerator(base_length, new StandardVectorReadableGenerator(), 
                                                                               new StandardDanglingDefaultInitOperatableVectorGenerator()){};
            
            ~StandardExponentialLinearTempStorageGenerator(){

                delete this->vec_generator;
                delete this->op_vector_generator;

            }
             
    };

    class DynamicBitIterator: public virtual DynamicBitIterable{

        private:

            VectorReadable * data;
            size_t idx;
        
        public:

            DynamicBitIterator(VectorReadable * data){

                this->data = data;
                this->idx = 0;

            }

            bool next(size_t& rs, size_t read_length){ 

                assert(read_length <= BIT_WIDTH);
                
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

    class StandardBitIterator: private DynamicBitIterator, public virtual BitIterable{

        private:

            size_t read_length;
        
        public:

            StandardBitIterator(VectorReadable * data, size_t read_length): DynamicBitIterator(data){

                this->read_length = read_length;

            }

            bool next(size_t& rs){ 

                return DynamicBitIterator::next(rs, this->read_length);

            }
  
    };

    class StandardDynamicBitIteratorGenerator: public virtual DynamicBitIteratorGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(DynamicBitIterator);

            }

            DynamicBitIterable * get(VectorReadable& data, void * plm_space){

                return new (plm_space) DynamicBitIterator(&data);

            }

    };

    class StandardBitIteratorGenerator: public virtual BitIteratorGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(StandardBitIterator);

            }

            BitIterable * get(VectorReadable& data, size_t read_length, void * plm_space){

                return new (plm_space) StandardBitIterator(&data, read_length);

            }

    };

    class DynamicBitIterOrWriter: public virtual DynamicBitIterWritable{

        private:

            OperatableVector * data;
            size_t idx;
        
        public:

            DynamicBitIterOrWriter(OperatableVector * data, size_t offset){

                this->data = data;
                this->idx = offset;

            }

            bool write(size_t val, size_t write_length){

                assert(write_length <= BIT_WIDTH);

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

    class BitIterOrWriter: public virtual BitIterWritable, private DynamicBitIterOrWriter{

        private:

            size_t write_length;
        
        public:

            BitIterOrWriter(OperatableVector * data, size_t offset, size_t write_length): DynamicBitIterOrWriter(data, offset){

                this->write_length = write_length;

            }

            bool write(size_t val){

                return DynamicBitIterOrWriter::write(val, this->write_length);

            }

    };

    class DynamicBitIterEmptyWriterGenerator: public virtual DynamicBitIterWriterGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(DynamicBitIterOrWriter);

            }

            DynamicBitIterWritable * get(OperatableVector& data, size_t offset, void * plm_space){

                return new (plm_space) DynamicBitIterOrWriter(&data, offset);

            }

    };

    class BitIterEmptyWriterGenerator: public virtual BitIterWriterGeneratable{

        public:

            size_t get_instantiation_cost(){

                return sizeof(BitIterOrWriter);

            }

            BitIterWritable * get(OperatableVector& data, size_t write_length, size_t offset, void * plm_space){

                return new (plm_space) BitIterOrWriter(&data, offset, write_length);

            }

    };

}

#endif                   