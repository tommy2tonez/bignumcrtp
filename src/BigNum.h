
#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include "stdint.h"
#include "assert.h"
#include <vector>
#include "boolvectorcrtp.h"
#include <memory>
#include "memcrtp.h"
#include <mutex> 
#include <string>
#include "log2.h"
#include <typeinfo>
#include "math.h"
#include <queue>

namespace bignum::vector{

    inline static const uint8_t BIT_LENGTH_PER_SLOT = 62;
    inline static const size_t MAX_VAL_PER_SLOT = ((size_t) 1 << BIT_LENGTH_PER_SLOT) - 1;
    inline static const uint8_t ACTUAL_BIT_LENGTH_PER_SLOT = sizeof(size_t) * 8;

    typedef size_t (*pairwise_ops)(size_t, size_t); 
    inline static uint8_t NULL_VALUE = 0;  

    namespace caster{

        template <class T>
        class BoolVectorViewCastable{

            public:

                template <class T1>
                auto cast(memory::sizet_linear::VectorReadable<T1>& data){ // -> datastructure::UnalignedViewableBoolVector<>& 

                    return static_cast<T*>(this)->cast(data); 

                }

                template <class T1>
                std::shared_ptr<BoolVectorViewCastable<T>> to_boolvector_view_castable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<BoolVectorViewCastable<T>>(data); 

                }

        };
        
        template <class T>
        class BoolVectorOpCastable{

            public:

                template <class T1>
                auto cast(memory::sizet_linear::OperatableVector<T1>& data){ //-> datastructure::UnalignedOperatableBoolVector<>&

                    return static_cast<T*>(this)->cast(data);

                }

                template <class T1>
                std::shared_ptr<BoolVectorOpCastable<T>> to_boolvector_op_castable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<BoolVectorOpCastable<T>>(data);

                }


        };

        template <class T>
        class SizeTLinearCastable{

            public:

                auto cast(size_t * data, size_t sz){ //-> VectorReadable<>&

                    return static_cast<T*>(this)->cast(data, sz);

                }

                template <class T1>
                std::shared_ptr<SizeTLinearCastable<T>> to_sizet_linear_castable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<SizeTLinearCastable<T>>(data); 

                }
        };

    }

    namespace comparer{

        template <class T>
        class Comparable{

            public:

                template <class T1, class T2>
                int8_t compare(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                    return static_cast<T*>(this)->compare(lhs, rhs);

                }
                
                template <class T1>
                std::shared_ptr<Comparable<T>> to_comparable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Comparable<T>>(data); 

                }

        };
         

    };

    namespace operation_utility{
        
        template <class T>
        class Judgable{

            public:

                template<class T1, class T2>
                bool fit(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                    return static_cast<T*>(this)->fit(lhs, rhs); 

                } 

                template <class T1>
                std::shared_ptr<Judgable<T>> to_judgable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Judgable<T>>(data);

                }

        };
        
        template <class T>
        class ShiftJudgable{

            public:

                template <class T1>
                bool fit(memory::sizet_linear::VectorReadable<T1>& lhs, size_t rhs){

                    return static_cast<T*>(this)->fit(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<ShiftJudgable<T>> to_shift_judgable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ShiftJudgable<T>>(data); 

                } 

        };

        template <class T>
        class VectorViewTrimmable{
            
            public:

                template <class T1>
                auto trim(memory::sizet_linear::VectorReadable<T1>& data){ //->memory::sizet_linear::VectorReadable<>&{

                    return static_cast<T*>(this)->trim(data);

                }

                template <class T1>
                std::shared_ptr<VectorViewTrimmable<T>> to_vector_view_trimmable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<VectorViewTrimmable<T>>(data); 

                }

        };

        template <class T>        
        class VectorViewSplittable{

            public:

                template <class T1>
                auto split(memory::sizet_linear::VectorReadable<T1>& data, size_t split_sz){ //->std::pair<memory::sizet_linear::VectorReadable<>&, memory::sizet_linear::VectorReadable<>&>
                                        
                    return static_cast<T*>(this)->split(data, split_sz);

                }

                template <class T1>
                std::shared_ptr<VectorViewSplittable<T>> to_view_splittable_sp(std::shared_ptr<T1> data){
                                        
                    return std::dynamic_pointer_cast<VectorViewSplittable<T>>(data); 

                } 

        };

        template <class T>
        class Estimatable{

            public:

                template <class T1, class T2>
                size_t get(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){
                    
                    return static_cast<T*>(this)->get(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<Estimatable<T>> to_estimatable_sp(std::shared_ptr<T1> data){
                    
                    return std::dynamic_pointer_cast<Estimatable<T>>(data); 

                }

        };

        template <class T>
        class BitLengthRetrievable{

            public:

                template <class T1>
                size_t get_bit_length(memory::sizet_linear::VectorReadable<T1>& lhs){

                    return static_cast<T*>(this)->get_bit_length(lhs); 

                }

                template <class T1>
                std::shared_ptr<BitLengthRetrievable<T>> to_bit_length_retrievable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<BitLengthRetrievable<T>>(data); 

                }

        };

        template <class T>
        class ShiftEstimatable{

            public:

                template <class T1>
                size_t get(memory::sizet_linear::VectorReadable<T1>& lhs, size_t rhs){

                    return static_cast<T*>(this)->get(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<ShiftEstimatable<T>> to_shift_estimatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ShiftEstimatable<T>>(data); 

                }

        };

        template <class T>
        class TrailingZeroCountable{

            public:
                
                template <class T1>
                size_t ctz(memory::sizet_linear::VectorReadable<T1>& data){

                    return static_cast<T*>(this)->ctz(data);

                } 

        };

        template <class T>
        class ZeroDetectable{

            public:

                template <class T1>
                bool is_zero(memory::sizet_linear::VectorReadable<T1>& data){

                    return static_cast<T*>(this)->is_zero(data); 

                }

                template <class T1>
                std::shared_ptr<ZeroDetectable<T>> to_zero_detectable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ZeroDetectable<T>>(data); 

                }


        };

        template <class T>
        class ZeroGeneratable{

            public:

                auto get(){ //-> VectorReadable<T> *
                     
                     return static_cast<T*>(this)->get(); 

                } 
                
        };

        template <class T>
        class BitIterable{

            public:

                bool next(size_t& data){

                    return static_cast<T*>(this)->next(data); 

                }


        };

        template <class T>
        class BitIteratorGeneratable{

            public:

                template <class T1>
                auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length){ // -> BitIterable<>&
                    
                    return static_cast<T*>(this)->get(data, read_length);

                }

                template <class T1>
                auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t offset){ // -> BitIterable<>&

                    return static_cast<T*>(this)->get(data, read_length, offset);
                }
                
                template <class T1>
                auto get_custom_width(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t custom_width){ // -> BitIterable<>&

                    return static_cast<T*>(this)->get_custom_width(data, read_length, custom_width);

                }

                template <class T1>
                std::shared_ptr<BitIteratorGeneratable<T>> to_bit_iterator_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<BitIteratorGeneratable<T>>(data);

                } 

        };
        
        template <class T>
        class BitIterWritable{

            public:

                bool write(size_t data){

                    return static_cast<T*>(this)->write(data); 

                }

        };

        template <class T>
        class BitIterWriterGeneratable{

            public:

                template <class T1>
                auto get(memory::sizet_linear::OperatableVector<T1>& data, size_t write_length, size_t offset) { // -> BitIterWritable<>&

                    return static_cast<T*>(this)->get(data, write_length, offset); 

                }

                template <class T1>
                std::shared_ptr<BitIterWriterGeneratable<T>> to_bit_iter_writer_generatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<BitIterWriterGeneratable<T>>(data);

                }

        };

        template <class T>
        class Discretizable{

            public:

                template <class T1>
                size_t get_instantiation_cost(memory::sizet_linear::VectorReadable<T1>& data, size_t n){

                    return static_cast<T*>(this)->get_instantiation_cost(data, n); 

                } 

                template <class T1>
                auto discretize(memory::sizet_linear::VectorReadable<T1>& data, size_t n, void * plm_space) {//-> OperatableVector<>&

                    return static_cast<T*>(this)->discretize(data, n, plm_space); 

                } 

                template <class T1>
                std::shared_ptr<Discretizable<T>> to_discretizable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Discretizable<T>>(data); 

                }

        };
        
    };

    namespace mutable_operation{
        
        template <class T>
        class Operatable{

            public:

                template <class T1, class T2>
                void ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data);

                }


        };

        template <class T>
        class ShiftOperatable{

            public:

                template <class T1>
                void ops(memory::sizet_linear::OperatableVector<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<ShiftOperatable<T>> to_shift_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ShiftOperatable<T>>(data); 

                }

        };

        template <class T>
        class PairWiseOperatable{

            public:

                template <class T1, class T2>
                void custom_ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, pairwise_ops pw_ops){
                    
                    static_cast<T*>(this)->custom_ops(lhs, rhs, pw_ops);

                }

                template <class T1>
                std::shared_ptr<PairWiseOperatable<T>> to_pairwise_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<PairWiseOperatable<T>>(data);

                }

        };

    };

    namespace operation{
        
        template <class T>
        class Operatable{

            public:

                template <class T1, class T2>
                auto ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){ //-> memory::sizet_linear::VectorReadable<>&

                    return static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                }

        };

        template <class T>
        class ShiftOperatable{

            public:

                template <class T1>
                auto ops(memory::sizet_linear::VectorReadable<T1>& lhs, size_t rhs){ // -> memory::sizet_linear::VectorReadable<>&

                    return static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<ShiftOperatable<T>> to_shift_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ShiftOperatable<T>>(data); 

                }

        };

        template <class T>
        class PairWiseOperatable{

            public:

                template <class T1, class T2>
                auto custom_ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, pairwise_ops pw_ops) {// -> memory::sizet_linear::VectorReadable<>&

                    return static_cast<T*>(this)->custom_ops(lhs, rhs, pw_ops); 

                }

                template <class T1>
                std::shared_ptr<PairWiseOperatable<T>> to_pairwise_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<PairWiseOperatable<T>>(data); 

                }
        };

    }

    namespace immutable_operation{

        template <class T>
        class Operatable{

            public:

                template <class T1, class T2, class T3>
                void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){

                    static_cast<T*>(this)->ops(lhs, rhs, rs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                }


        };

        template <class T>
        class ShiftOperatable{

            public:

                template <class T1, class T2>
                void ops(memory::sizet_linear::VectorReadable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T2>& rs){

                    static_cast<T*>(this)->ops(lhs, rhs, rs);

                }

                template <class T1>
                std::shared_ptr<ShiftOperatable<T>> to_shift_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<ShiftOperatable<T>>(data); 

                }

        };

        template <class T>
        class PairWiseOperatable{

            public:

                template <class T1, class T2, class T3>
                void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs, pairwise_ops pw_ops){

                    static_cast<T*>(this)->ops(lhs, rhs, rs, pw_ops);

                }

                template <class T1>
                std::shared_ptr<PairWiseOperatable<T>> to_pairwise_operatable_sp(std::shared_ptr<T1> data){
                    
                    return std::dynamic_pointer_cast<PairWiseOperatable<T>>(data); 

                } 

        };


    }

    namespace mutable_extensible_operation{
        
        template<class T>
        class Operatable{

            public:

                template <class T1, class T2>
                void ops(memory::sizet_linear::ReallocatableOperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                } 

        };

        template<class T>
        class ShiftOperatable{

            public:

                template <class T1>
                void ops(memory::sizet_linear::ReallocatableOperatableVector<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);

                }

        };
        
        template <class T>
        class PairWiseOperatable{

            public:

                template <class T1, class T2>
                void custom_ops(memory::sizet_linear::ReallocatableOperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, pairwise_ops pw_ops){

                    static_cast<T*>(this)->custom_ops(lhs, rhs, pw_ops);

                }

        };

    };

};

namespace bignum::integer::usgn{

    template <class T>
    class BigNumable: public memory::sizet_linear::VectorReadable<T>{

        public:

            BigNumable<T> * to_bignumable(){

                return this;

            }

    };

    template <class T>
    class MutableBigNumable: public BigNumable<MutableBigNumable<T>>, public memory::sizet_linear::ReallocatableOperatableVector<T>{

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

                return static_cast<T*>(this)->sizeof_slot();

            }

            memory::sizet_linear::VectorReadable<T> * to_vector_readable(){

                return memory::sizet_linear::ReallocatableOperatableVector<T>::to_vector_readable();

            }

    };

    template <class T>
    class Comparable{

        public:

            template <class T1, class T2>
            int8_t compare(BigNumable<T1>& lhs, BigNumable<T2>& rhs){

                return static_cast<T*>(this)->compare(lhs, rhs);

            } 

            template <class T1>
            int8_t compare(BigNumable<T1>& lhs, size_t rhs){

                return static_cast<T*>(this)->compare(lhs, rhs);

            }

            template <class T1>
            std::shared_ptr<Comparable<T>> to_comparable_sp(std::shared_ptr<T1> data){

                return std::dynamic_pointer_cast<Comparable<T>>(data); 

            } 

            T get_containee_type(){

                return T();

            }

    };

    template <class T>
    class Stringifiable{

        public:

            template <class T1>
            std::string stringify(BigNumable<T1>& data){

                return static_cast<T*>(this)->stringify(data); 

            }

            template <class T1>
            std::shared_ptr<Stringifiable<T>> to_stringifiable_sp(std::shared_ptr<T1> data){

                return std::dynamic_pointer_cast<Stringifiable<T>>(data); 

            }

            T get_containee_type(){

                return T();

            }

    };

    namespace parser{

        class Estimatable{

            public:

                virtual size_t get(std::string_view data) = 0;
        
        };

        template <class T>
        class StringParsable{

            public: 

                auto parse(std::string_view data){//->BigNumable<T>&

                    return static_cast<T*>(this)->parse(data); 

                } 

                template <class T1>
                std::shared_ptr<StringParsable<T>> to_string_parsable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<StringParsable<T>>(data);

                }

                T get_containee_type(){

                    return T();

                }

        };

        class StringParserClassifiable{

            public:

                static inline const uint8_t HEX_CONST = 0;
                static inline const uint8_t BIN_CONST = 1;
                static inline const uint8_t DEC_CONST = 2;

                virtual uint8_t classify(std::string_view) = 0;
                virtual std::string_view remove_header(std::string_view) = 0;

        };

    }

    namespace operation{

        template<class T>
        class Operatable{

            public:
                
                template <class T1, class T2>
                auto ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){  

                    return static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                auto ops(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                }

        };

        template<class T>
        class UsgnOperatable{

            public:
                
                template <class T1, class T2>
                auto minus(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->minus(lhs, rhs);

                }   
                
                template <class T1>
                auto minus(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->minus(lhs, rhs); 

                }

                template <class T1, class T2>
                auto plus(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->plus(lhs, rhs);

                }

                template <class T1>
                auto plus(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->plus(lhs, rhs); 

                }

                template <class T1, class T2>
                auto rshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->rshift(lhs, rhs);

                }

                template <class T1>
                auto rshift(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->rshift(lhs, rhs); 

                }

                template <class T1, class T2>
                auto lshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->lshift(lhs, rhs);

                }

                template <class T1>
                auto lshift(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->lshift(lhs, rhs); 

                }

                template <class T1, class T2>
                auto multiply(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->multiply(lhs, rhs);

                }

                template <class T1>
                auto multiply(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->multiply(lhs, rhs); 

                }

                template <class T1, class T2>
                auto divide(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->divide(lhs, rhs);

                }

                template <class T1>
                auto divide(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->divide(lhs, rhs); 

                }

                template <class T1, class T2>
                auto and_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->and_ops(lhs, rhs);

                }

                template <class T1>
                auto and_ops(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->and_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto or_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->or_ops(lhs, rhs);

                }

                template <class T1>
                auto or_ops(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->or_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto xor_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->xor_ops(lhs, rhs);

                }

                template <class T1>
                auto xor_ops(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->xor_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto mod(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ 

                    return static_cast<T*>(this)->mod(lhs, rhs);

                }

                template <class T1>
                auto mod(BigNumable<T1>& lhs, size_t rhs){ 

                    return static_cast<T*>(this)->mod(lhs, rhs); 

                }

                template <class T1>
                std::shared_ptr<UsgnOperatable<T>> to_usgn_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<UsgnOperatable<T>>(data); 

                }

                T get_containee_type(){

                    return T();

                }

        };

    }

    namespace immutable_operation{

        template<class T>
        class Operatable{

            public:
                
                template <class T1, class T2, class T3>
                void ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){  

                    static_cast<T*>(this)->ops(lhs, rhs, rs);

                }

                template <class T1, class T2>
                void ops(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T2>& rs){

                    static_cast<T*>(this)->ops(lhs, rhs, rs);

                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                }

        };

        template<class T>
        class UsgnOperatable{

            public:
                
                template <class T1, class T2, class T3>
                void minus(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->minus(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void minus(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->minus(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void plus(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->plus(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void plus(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->plus(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void rshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->rshift(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void rshift(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->rshift(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void lshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->lshift(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void lshift(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->lshift(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void multiply(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->multiply(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void multiply(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->multiply(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void divide(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->divide(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void divide(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->divide(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void and_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->and_ops(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void and_ops(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->and_ops(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void or_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->or_ops(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void or_ops(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->or_ops(lhs, rhs, rs); 

                }

                template <class T1, class T2, class T3>
                void xor_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->xor_ops(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void xor_ops(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ 

                    return static_cast<T*>(this)->xor_ops(lhs, rhs, rs); 

                }      

                template <class T1, class T2, class T3>
                void mod(BigNumable<T1>& lhs, BigNumable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){

                    return static_cast<T*>(this)->mod(lhs, rhs, rs);

                }   
                
                template <class T1, class T3>
                void mod(BigNumable<T1>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){

                    return static_cast<T*>(this)->mod(lhs, rhs, rs); 

                }  

                template <class T1>
                std::shared_ptr<UsgnOperatable<T>> to_usgn_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<UsgnOperatable<T>>(data); 

                }

                T get_containee_type(){

                    return T();

                }

        };

    };

    namespace mutable_operation{
        
        template <class T>
        class Operatable{

            public:

                template <class T1, class T2>
                void ops(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                void ops(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->ops(lhs, rhs);
                }

                template <class T1>
                std::shared_ptr<Operatable<T>> to_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<Operatable<T>>(data); 

                }

        };

        template <class T>
        class UsgnOperatable{

            public:

                template <class T1, class T2>
                void assign(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){
                                        
                    static_cast<T*>(this)->assign(lhs, rhs);

                }

                template <class T1>
                void assign(MutableBigNumable<T1>& lhs, size_t rhs){
                                        
                    static_cast<T*>(this)->assign(lhs, rhs);
                }

                template <class T1, class T2>
                void minus(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->minus(lhs, rhs);

                }

                template <class T1>
                void minus(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->minus(lhs, rhs);
                }

                template <class T1, class T2>
                void plus(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->plus(lhs, rhs);

                }

                template <class T1>
                void plus(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->plus(lhs, rhs);
                }

                template <class T1, class T2>
                void rshift(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->rshift(lhs, rhs);

                }

                template <class T1>
                void rshift(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->rshift(lhs, rhs);
                }

                template <class T1, class T2>
                void lshift(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->lshift(lhs, rhs);

                }

                template <class T1>
                void lshift(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->lshift(lhs, rhs);
                }

                template <class T1, class T2>
                void multiply(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->multiply(lhs, rhs);

                }

                template <class T1>
                void multiply(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->multiply(lhs, rhs);
                }

                template <class T1, class T2>
                void divide(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->divide(lhs, rhs);

                }

                template <class T1>
                void divide(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->divide(lhs, rhs);
                }

                template <class T1, class T2>
                void and_ops(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->and_ops(lhs, rhs);

                }

                template <class T1>
                void and_ops(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->and_ops(lhs, rhs);
                }

                template <class T1, class T2>
                void or_ops(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->or_ops(lhs, rhs);

                }

                template <class T1>
                void or_ops(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->or_ops(lhs, rhs);
                }

                template <class T1, class T2>
                void xor_ops(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->xor_ops(lhs, rhs);

                }

                template <class T1>
                void xor_ops(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->xor_ops(lhs, rhs);
                }

                template <class T1, class T2>
                void mod(MutableBigNumable<T1>& lhs, BigNumable<T2>& rhs){

                    static_cast<T*>(this)->mod(lhs, rhs);

                }

                template <class T1>
                void mod(MutableBigNumable<T1>& lhs, size_t rhs){

                    static_cast<T*>(this)->mod(lhs, rhs);
                }

                template <class T1>
                std::shared_ptr<UsgnOperatable<T>> to_usgn_operatable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<UsgnOperatable<T>>(data); 

                }

                T get_containee_type(){
                                        
                    return T();

                }

        };

    };
    
    namespace operation_utility{

        template <class T>
        class OOBCheckable{

            public:
                
                template <class T1>
                bool is_oob(BigNumable<T1>& data){

                    return static_cast<T*>(this)->is_oob(data); 

                }

                template <class T1>
                bool is_in_bound(BigNumable<T1>& data){

                    return static_cast<T*>(this)->is_in_bound(data); 

                }

                bool is_oob(size_t data){

                    return static_cast<T*>(this)->is_oob(data); 

                }

                bool is_in_bound(size_t data){

                    return static_cast<T*>(this)->is_in_bound(data);

                }

                template <class T1>
                std::shared_ptr<OOBCheckable<T>> to_oob_checkable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<OOBCheckable<T>>(data); 

                }

        };

    }

    namespace backward_caster{

        template <class T>        
        class SizeTLinearVectorCastable{

            public:

                template <class T1>
                auto cast(T1 data){ // VectorReadable& -> BigNumable<>&

                    return static_cast<T*>(this)->cast(data);

                }

                template <class T1>
                std::shared_ptr<SizeTLinearVectorCastable<T>> to_sizet_linear_vector_castable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<SizeTLinearVectorCastable<T>>(data); 

                }

        };

        template <class T>
        class UINTCastable{

            public:

                auto cast(size_t data){ 

                    return static_cast<T*>(this)->cast(data); 

                }

                template <class T1>
                std::shared_ptr<UINTCastable<T>> to_uint_castable_sp(std::shared_ptr<T1> data){
                    
                    return std::dynamic_pointer_cast<UINTCastable<T>>(data); 

                }


        };

    }

    namespace caster{

        template<class T>
        class UINTCastable{

            public:

                template<class T1>
                size_t to_size_t(BigNumable<T1>& data){

                    return static_cast<T*>(this)->to_size_t(data); 

                }

                template <class T1>
                std::shared_ptr<UINTCastable<T>> to_uint_castable_sp(std::shared_ptr<T1> data){
                    
                    return std::dynamic_pointer_cast<UINTCastable<T>>(data); 

                }

        };
        
    }

}

namespace bignum::vector{

    template <class ID>
    class RPadIndexConverter{

        public:
            
            RPadIndexConverter() {};

            template <class T>
            RPadIndexConverter(RPadIndexConverter<T>&&){};

            size_t get_actual_idx(size_t idx){
                
                return this->get_slot(idx) * ACTUAL_BIT_LENGTH_PER_SLOT + this->get_offset(idx);
            
            }
        
        protected:

            size_t get_slot(size_t idx){

                return idx / BIT_LENGTH_PER_SLOT;

            }

            size_t get_offset(size_t idx){

                return idx % BIT_LENGTH_PER_SLOT;

            }

    };

    class BigNumBoolVectorView: public datastructure::UnalignedViewableBoolVector<BigNumBoolVectorView>, 
                                private RPadIndexConverter<BigNumBoolVectorView>{

        private:

            memory::sizet_linear::StandardBoolVectorView data;

        public:

            BigNumBoolVectorView() {};

            template <class T>
            BigNumBoolVectorView(memory::sizet_linear::VectorReadable<T>& obj){

                this->data = memory::sizet_linear::StandardBoolVectorView(obj); 

            }

            BigNumBoolVectorView(BigNumBoolVectorView& obj){
                
                this->data = obj.data;

            }

            BigNumBoolVectorView(BigNumBoolVectorView&& obj){

                this->data = std::move(obj.data);

            }

            BigNumBoolVectorView& operator = (BigNumBoolVectorView& obj){

                this->data = obj.data;

                return *this;

            }

            BigNumBoolVectorView& operator = (BigNumBoolVectorView&& obj){

                this->data = std::move(obj.data);

                return *this;

            }

            size_t length(){

                return this->data.length() / ACTUAL_BIT_LENGTH_PER_SLOT * BIT_LENGTH_PER_SLOT; 

            }

            bool get(size_t idx){
                
                return this->data.get(this->get_actual_idx(idx));

            }

            size_t byte_length(){

                size_t bit_length = this->length();

                if (bit_length % 8 == 0){
                    
                    return bit_length >> 3; 

                }

                return (bit_length >> 3) + 1;

            }


    };

    class BigNumBoolVectorOp: public datastructure::UnalignedOperatableBoolVector<BigNumBoolVectorOp>,
                              private RPadIndexConverter<BigNumBoolVectorOp>{
        
        private:

            memory::sizet_linear::StandardBoolVectorOperator data;

        public:

            BigNumBoolVectorOp() {};
            
            template <class T>
            BigNumBoolVectorOp(memory::sizet_linear::OperatableVector<T>& obj){
                
                this->data = memory::sizet_linear::StandardBoolVectorOperator(obj); 

            }

            BigNumBoolVectorOp(BigNumBoolVectorOp& obj){

                this->data = obj.data;

            }

            BigNumBoolVectorOp(BigNumBoolVectorOp&& obj){

                this->data = std::move(obj.data);

            }

            BigNumBoolVectorOp& operator = (BigNumBoolVectorOp& obj){

                this->data = obj.data;

                return *this;

            }

            BigNumBoolVectorOp& operator = (BigNumBoolVectorOp&& obj){

                this->data = std::move(obj.data);

                return *this;

            }

            size_t length(){

                return this->data.length() / ACTUAL_BIT_LENGTH_PER_SLOT * BIT_LENGTH_PER_SLOT; 

            }

            bool get(size_t idx){
                
                return this->data.get(this->get_actual_idx(idx));

            }

            size_t byte_length(){

                size_t bit_length = this->length();

                if (bit_length % 8 == 0){
                    
                    return bit_length >> 3; 

                }

                return (bit_length >> 3) + 1;

            }

            void set_val(uint64_t idx, bool val){

                this->data.set_val(this->get_actual_idx(idx), val);

            }

    };

    namespace caster{
        
        class GenericID{};

        template <class ID>
        class FullBoolVectorViewCaster: public BoolVectorViewCastable<FullBoolVectorViewCaster<ID>>{

            public:

                FullBoolVectorViewCaster(){}

                template <class T>
                BigNumBoolVectorView cast(memory::sizet_linear::VectorReadable<T>& data){

                    return BigNumBoolVectorView(data); 

                }

        };

        template <class T, class ID>
        class TrueBoolVectorViewCaster: public BoolVectorViewCastable<TrueBoolVectorViewCaster<T, ID>>,
                                        private FullBoolVectorViewCaster<ID>{
            
            private:

                std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer;

            public:

                using BoolVectorViewCastable<TrueBoolVectorViewCaster<T, ID>>::to_boolvector_view_castable_sp;

                TrueBoolVectorViewCaster() {};

                TrueBoolVectorViewCaster(std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer){

                    this->trimmer = trimmer;

                }
                
                template <class T1>
                auto cast(memory::sizet_linear::VectorReadable<T1>& data){

                    auto rs = FullBoolVectorViewCaster<ID>::cast(data);
                    
                    return this->trimmer->trim(rs);;

                }

        };

        template <class ID>
        class FullBoolVectorOperatorCaster: public BoolVectorOpCastable<FullBoolVectorOperatorCaster<ID>>{

            public: 

                FullBoolVectorOperatorCaster(){}

                template <class T>
                auto cast(memory::sizet_linear::OperatableVector<T>& data){

                    return BigNumBoolVectorOp(data);

                }

        };

        template <class T, class ID>
        class TrueBoolVectorOperatorCaster: public BoolVectorOpCastable<TrueBoolVectorOperatorCaster<T, ID>>,
                                            private FullBoolVectorOperatorCaster<ID>{

            private:

                std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer;

            public:

                TrueBoolVectorOperatorCaster(){}

                TrueBoolVectorOperatorCaster(std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer){

                    this->trimmer = trimmer;

                }

                template <class T1>
                auto cast(memory::sizet_linear::OperatableVector<T1>& data){

                    auto casted = FullBoolVectorOperatorCaster<ID>::cast(data);

                    return this->trimmer->trim(casted);

                }

        };

        template <class T1, class T2, class T3, class ID>
        class SizeTLinearNoPaddingCaster: public SizeTLinearCastable<SizeTLinearNoPaddingCaster<T1, T2, T3, ID>>{
            
            private:
                
                std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T1>> readable_gen;
                std::shared_ptr<memory::sizet_linear::BitIteratorGeneratable<T2>> iter_gen;
                std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T3>> addable_vec_gen;

            public:

                SizeTLinearNoPaddingCaster(){};

                SizeTLinearNoPaddingCaster(std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T1>> readable_gen,
                                           std::shared_ptr<memory::sizet_linear::BitIteratorGeneratable<T2>> iter_gen,
                                           std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T3>> addable_vec_gen){

                    this->readable_gen = readable_gen;
                    this->iter_gen = iter_gen;
                    this->addable_vec_gen = addable_vec_gen;

                }

                auto cast(size_t * data, size_t sz){
                    
                    assert(sz != 0);

                    size_t cur = 0;
                    size_t slot_sz = this->get_size(data, sz);
                    
                    auto rs = this->addable_vec_gen->get();
                    auto casted = this->readable_gen->from_ptr(data, sz);
                    auto iter = this->iter_gen->get(casted, BIT_LENGTH_PER_SLOT);

                    while (iter.next(cur) && (rs.length() < slot_sz)){

                        rs.add(cur);

                    }

                    return rs;

                }
            

            private:

                size_t get_size(size_t * data, size_t sz){
                    
                    while ((sz != 0) && (data[sz - 1] == 0)){

                        --sz;
                    
                    } 

                    if (sz == 0){

                        return 1;

                    }

                    size_t bit_length = (sz - 1) * ACTUAL_BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(data[sz - 1]) + 1;

                    if (bit_length % BIT_LENGTH_PER_SLOT == 0){

                        return bit_length / BIT_LENGTH_PER_SLOT;

                    }

                    return (bit_length / BIT_LENGTH_PER_SLOT) + 1;

                }


        };

        class StandardBoolVectorCasterGenerator{

            private:

                template <class T>
                auto init_true_view_caster(std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer){
                    
                    return std::make_shared<TrueBoolVectorViewCaster<T, GenericID>>(trimmer);

                }

                template <class T>
                auto init_true_op_caster(std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer){

                    return std::make_shared<TrueBoolVectorOperatorCaster<T, GenericID>>(trimmer);

                }
                
            public:

                auto get_full_view_caster(){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    return std::make_shared<FullBoolVectorViewCaster<GenericID>>(); 

                }

                auto get_true_view_caster(){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    auto r_trimmer = utility::boolvector::StandardGenerator().get_right_zero_view_stable_trimmer();
                    auto casted_trimmer = r_trimmer->to_stable_boolvector_view_trimmable_sp(r_trimmer);

                    return this->init_true_view_caster(casted_trimmer);

                }

                auto get_full_opv_caster(){ // -> shared_ptr<BoolVectorOpCastable<>>

                    return std::make_shared<FullBoolVectorOperatorCaster<GenericID>>();

                }

                auto get_true_opv_caster(){ // -> shared_ptr<BoolVectorOpCastable<>>

                    auto r_trimmer = utility::boolvector::StandardGenerator().get_right_zero_operator_stable_trimmer();
                    auto casted = r_trimmer->to_stable_boolvector_operator_trimmable_sp(r_trimmer);

                    return this->init_true_op_caster(casted);

                }


        };

        class StandardCasterGenerator{

            private:

                template <class T, class T1, class T3>
                auto init_sizet_linear_no_padding_caster(std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T>> readable_gen,
                                                         std::shared_ptr<memory::sizet_linear::BitIteratorGeneratable<T1>> iter_gen,
                                                         std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T3>> addable_vec_gen){
                    
                    return std::make_shared<SizeTLinearNoPaddingCaster<T, T1, T3, GenericID>>(readable_gen, iter_gen, addable_vec_gen);

                };

            public:
                
                template <class T>
                auto get_sizet_linear_no_padding_caster(std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T>> addable_vec_gen){
                    
                    auto readable_gen = memory::sizet_linear::StandardGenerator().get_dangling_read_vec_gen();
                    auto iter_gen = memory::sizet_linear::StandardGenerator().get_bit_iter_gen();

                    auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                    auto casted_iter_gen = iter_gen->to_bit_iterator_generatable_sp(iter_gen);

                    return this->init_sizet_linear_no_padding_caster(casted_readable_gen, casted_iter_gen, addable_vec_gen);

                }

        };

        class IDBoolVectorCasterGenerator{

            private:

                template <class T, class ID>
                auto init_true_view_caster(std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer, ID){
                    
                    return std::make_shared<TrueBoolVectorViewCaster<T, ID>>(trimmer);

                }

                template <class T, class ID>
                auto init_true_op_caster(std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer, ID){

                    return std::make_shared<TrueBoolVectorOperatorCaster<T, ID>>(trimmer);

                }
                
            public:

                template <class ID>
                auto get_full_view_caster(ID){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    return std::make_shared<FullBoolVectorViewCaster<ID>>(); 

                }

                template <class ID>
                auto get_true_view_caster(ID id_){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    auto r_trimmer = utility::boolvector::IDGenerator().get_right_zero_view_stable_trimmer(id_);
                    auto casted_trimmer = r_trimmer->to_stable_boolvector_view_trimmable_sp(r_trimmer);

                    return this->init_true_view_caster(casted_trimmer, id_);

                }

                template <class ID>
                auto get_full_opv_caster(ID id_){ // -> shared_ptr<BoolVectorOpCastable<>>

                    return std::make_shared<FullBoolVectorOperatorCaster<ID>>();

                }

                template <class ID>
                auto get_true_opv_caster(ID id_){ // -> shared_ptr<BoolVectorOpCastable<>>

                    auto r_trimmer = utility::boolvector::IDGenerator().get_right_zero_operator_stable_trimmer(id_);
                    auto casted = r_trimmer->to_stable_boolvector_operator_trimmable_sp(r_trimmer);

                    return this->init_true_op_caster(casted, id_);

                }


        };

        class IDCasterGenerator{

            private:

                template <class T, class T1, class T3, class ID>
                auto init_sizet_linear_no_padding_caster(std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T>> readable_gen,
                                                         std::shared_ptr<memory::sizet_linear::BitIteratorGeneratable<T1>> iter_gen,
                                                         std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T3>> addable_vec_gen,
                                                         ID){
                    
                    return std::make_shared<SizeTLinearNoPaddingCaster<T, T1, T3, ID>>(readable_gen, iter_gen, addable_vec_gen);

                };

            public:
                
                template <class T, class ID>
                auto get_sizet_linear_no_padding_caster(std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T>> addable_vec_gen,
                                                        ID id_){
                    
                    auto readable_gen = memory::sizet_linear::IDGenerator().get_dangling_read_vec_gen(id_);
                    auto iter_gen = memory::sizet_linear::IDGenerator().get_bit_iter_gen(id_);

                    auto casted_readable_gen = readable_gen->to_vector_readable_generatable_sp(readable_gen);
                    auto casted_iter_gen = iter_gen->to_bit_iterator_generatable_sp(iter_gen);

                    return this->init_sizet_linear_no_padding_caster(casted_readable_gen, casted_iter_gen, addable_vec_gen, id_);

                }

        };


    }

}

namespace bignum::vector::comparer{

    class GenericID{};

    template <class ID>
    class BackwardComparer: public Comparable<BackwardComparer<ID>>{

        public:

            template <class T1, class T2>
            int8_t compare(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){
                
                if (lhs.length() > rhs.length()){

                    return 1;

                }

                if (lhs.length() < rhs.length()){

                    return -1;

                }

                for (intmax_t i = (intmax_t) lhs.length() - 1; i >= 0; --i){
                                                            
                    if (lhs.get(i) > rhs.get(i)){

                        return 1;

                    }

                    if (lhs.get(i) < rhs.get(i)){

                        return -1;

                    }

                }
                
                return 0;

            }

    };

    class StandardComparerGenerator{

        public:

            auto get_backward_comparer(){

                return std::make_shared<BackwardComparer<GenericID>>();

            }

            template <class T>
            auto get_backward_comparer(T){

                return std::make_shared<BackwardComparer<T>>();

            }

    };

}

namespace bignum::vector::operation_utility{

    class Generic{};

    template <class ID>
    class VectorLengthRightTrimmer{

        protected:
            
            VectorLengthRightTrimmer() {};

            template <class T>
            size_t get_size(memory::sizet_linear::VectorReadable<T>& data){

                size_t true_length = data.length();

                while (true_length != 0 && data.get(true_length - 1) == 0){

                    --true_length;

                }

                return true_length;

            }


    };

    template <class ID>
    class VectorViewTrimmer: public VectorLengthRightTrimmer<VectorViewTrimmer<ID>>, public VectorViewTrimmable<VectorViewTrimmer<ID>>{

        public:

            VectorViewTrimmer() {};

            template <class T>
            auto trim(memory::sizet_linear::VectorReadable<T>& data){

                return memory::sizet_linear::StandardVectorView((size_t *) data.get_data(), this->get_size(data));

            } 


    };

    template <class ID>
    class LegalBitLengthRetriever: public BitLengthRetrievable<LegalBitLengthRetriever<ID>>{

        public:

            LegalBitLengthRetriever() {};

            template <class T1>
            size_t get_bit_length(memory::sizet_linear::VectorReadable<T1>& data){

                return (data.length() - 1) * BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(data.get(data.length() - 1)) + 1; 

            }

    };
    
    template <class ID>
    class BitLengthRetriever: public BitLengthRetrievable<BitLengthRetriever<ID>>, 
                              private VectorLengthRightTrimmer<BitLengthRetriever<ID>>{

        public:

            BitLengthRetriever() {};

            template <class T>
            size_t get_bit_length(memory::sizet_linear::VectorReadable<T>& data){

                size_t data_sz = this->get_size(data);

                if (data_sz == 0){

                    return 0;

                } 

                return (data_sz - 1) * BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(data.get(data_sz - 1)) + 1;

            }


    };

    template <class ID>
    class LegalTrueBoolVectorViewCaster: public caster::BoolVectorViewCastable<LegalTrueBoolVectorViewCaster<ID>>,
                                         private LegalBitLengthRetriever<LegalTrueBoolVectorViewCaster<ID>>,
                                         private caster::FullBoolVectorViewCaster<ID>{

        public:

            LegalTrueBoolVectorViewCaster() {};

            template <class T>
            auto cast(memory::sizet_linear::VectorReadable<T>& data){

                auto casted = caster::FullBoolVectorViewCaster<ID>::cast(data);

                return datastructure::UnalignedOffsetBoolVectorViewGenerator().get(casted, 0, LegalBitLengthRetriever<LegalTrueBoolVectorViewCaster<ID>>::get_bit_length(data)); 

            }


            
    };

    template <class ID>
    class FastTrailingZeroCounter: public TrailingZeroCountable<FastTrailingZeroCounter<ID>>, 
                                   private VectorLengthRightTrimmer<FastTrailingZeroCounter<ID>>{

        public:

            FastTrailingZeroCounter() {};

            template <class T>
            size_t ctz(memory::sizet_linear::VectorReadable<T>& data){

                size_t sz = this->get_size(data);

                if (sz == 0){

                    return 0;

                }

                size_t trailing_zero_sz = 0;

                while (data.get(trailing_zero_sz) == 0){

                    ++trailing_zero_sz;

                }

                size_t r_most_sig_val = data.get(trailing_zero_sz) & (-data.get(trailing_zero_sz)); 

                return trailing_zero_sz * BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(r_most_sig_val);

            }


    };

    template <class ID>
    class ZeroDetector: public ZeroDetectable<ZeroDetector<ID>>, 
                        private VectorLengthRightTrimmer<ZeroDetector<ID>>{
        
        public:

            ZeroDetector() {};

            template <class T>
            bool is_zero(memory::sizet_linear::VectorReadable<T>& data){
                
                return this->get_size(data) == 0;

            }


    };

    template <class ID>
    class ZeroGenerator: public ZeroGeneratable<ZeroGenerator<ID>>{
        
        private:

            memory::sizet_linear::StandardVectorView data;  
            size_t NULL_VALUE = 0;

        public:

            ZeroGenerator(){

                this->data = memory::sizet_linear::StandardVectorView(&NULL_VALUE, 1);

            }

            auto get(){

                return this->data;

            }

    };

    template <class ID>
    class DefaultGuardedVectorRightViewTrimmer: public VectorViewTrimmable<DefaultGuardedVectorRightViewTrimmer<ID>>,
                                                private ZeroGenerator<DefaultGuardedVectorRightViewTrimmer<ID>>,
                                                private VectorViewTrimmer<DefaultGuardedVectorRightViewTrimmer<ID>>{

        public:

            DefaultGuardedVectorRightViewTrimmer() {};

            template <class T>
            auto trim(memory::sizet_linear::VectorReadable<T>& data){

                auto rs = VectorViewTrimmer<DefaultGuardedVectorRightViewTrimmer<ID>>::trim(data); 

                if (rs.length() == 0){
                                        
                    return ZeroGenerator<DefaultGuardedVectorRightViewTrimmer<ID>>::get();

                }

                return rs;

            }


    };

    template <class ID>
    class StandardSplitter: public VectorViewSplittable<StandardSplitter<ID>>,
                            private DefaultGuardedVectorRightViewTrimmer<StandardSplitter<ID>>,
                            private memory::sizet_linear::VectorReadableSplitter<ID>{

        public:

            StandardSplitter() {};
            
            template <class T>
            auto split(memory::sizet_linear::VectorReadable<T>& data, size_t split_point){
                
                auto rs = memory::sizet_linear::VectorReadableSplitter<ID>::split(data, split_point);

                rs.first = DefaultGuardedVectorRightViewTrimmer<StandardSplitter<ID>>::trim(rs.first);
                rs.second = DefaultGuardedVectorRightViewTrimmer<StandardSplitter<ID>>::trim(rs.second); 
                
                return rs;

            }


    };

    class IteratorHelper{
        
        protected:

            size_t actual_bit_width;
            size_t virtual_bit_width;
            size_t virtual_read_size;

            IteratorHelper(size_t actual_bit_width, size_t virtual_bit_width, size_t virtual_read_size){

                assert(actual_bit_width <= ACTUAL_BIT_LENGTH_PER_SLOT);
                assert(virtual_bit_width <= actual_bit_width);
                assert(virtual_read_size <= virtual_bit_width);
                assert(virtual_read_size > 0);

                this->actual_bit_width = actual_bit_width;
                this->virtual_bit_width = virtual_bit_width;
                this->virtual_read_size = virtual_read_size;

            }

            void get_metadata(size_t actual_bob_idx, size_t& actual_read_length, int8_t& illegal_interval_start_idx, 
                              int8_t& illegal_interval_end_idx){
                
                size_t actual_eor_idx = actual_bob_idx + this->virtual_read_size - 1;
                size_t actual_bob_slot = actual_bob_idx / this->actual_bit_width;
                size_t actual_bob_offset = actual_bob_idx % this->actual_bit_width; 
                size_t actual_eor_slot = actual_eor_idx / this->actual_bit_width;
                size_t actual_eor_offset = actual_eor_idx % this->actual_bit_width;
                
                if ((actual_bob_slot == actual_eor_slot) && (actual_eor_offset < this->virtual_bit_width)){

                    actual_read_length = this->virtual_read_size;
                    illegal_interval_start_idx = -1;
                    illegal_interval_end_idx = -1; 

                    return;

                }

                illegal_interval_end_idx = this->actual_bit_width - actual_bob_offset - 1;

                if (actual_bob_offset >= this->virtual_bit_width){

                    actual_read_length = this->virtual_read_size + (this->actual_bit_width - actual_bob_offset);
                    illegal_interval_start_idx = 0;

                    return;

                }

                actual_read_length = this->virtual_read_size + (this->actual_bit_width - this->virtual_bit_width);
                illegal_interval_start_idx = this->virtual_bit_width - actual_bob_offset;

            }

            size_t get_extractor(size_t length){
                
                assert(length <= sizeof(size_t) * 8);

                if (length == sizeof(size_t) * 8){

                    return std::numeric_limits<size_t>::max(); 

                }

                return ((size_t) 1 << length) - 1; 

            }

            size_t crop(size_t data, int8_t start_idx, int8_t length){

                if (length == -1){

                    length = sizeof(size_t) * 8; 

                }

                return (data >> start_idx) & this->get_extractor(length);

            }

    };

    template <class T>
    class BitIterator: public BitIterable<BitIterator<T>>, protected IteratorHelper{

        private:

            size_t actual_offset;
            T iter; // memory::sizet_linear::DynamicBitIterable<T>&

        public:

            BitIterator(size_t actual_bit_width, size_t virtual_bit_width, size_t virtual_read_size, size_t actual_offset,
                        T iter): IteratorHelper(actual_bit_width, virtual_bit_width, virtual_read_size){

                this->actual_offset = actual_offset;
                this->iter = iter;

            }

            bool next(size_t& rs){ 
                                
                size_t actual_read_length = 0;
                int8_t istart_idx = 0;
                int8_t iend_idx = 0;

                this->get_metadata(this->actual_offset, actual_read_length, istart_idx, iend_idx);

                if (!this->iter.next(rs, actual_read_length)){
                    
                    return false;

                }
                
                this->actual_offset += actual_read_length;

                if (istart_idx == -1){

                    return true;

                }

                size_t first_half = this->crop(rs, 0, istart_idx); 
                size_t second_half = this->crop(rs, iend_idx + 1, -1);

                rs = (second_half << istart_idx) | first_half;

                return true;

            }

    }; 
    
    template <class T, class ID>
    class BitIteratorGenerator: public BitIteratorGeneratable<BitIteratorGenerator<T, ID>>, 
                                private RPadIndexConverter<BitIteratorGenerator<T, ID>>{

        private:

            std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen;

        public:

            BitIteratorGenerator() {};

            BitIteratorGenerator(std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen) {

                this->iter_gen = iter_gen;

            };

            template <class T1>
            auto get_custom_width(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t custom_width){

                auto iter = this->iter_gen->get(data);

                return this->get(read_length, custom_width, 0, iter);

            }

            template <class T1>
            auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length){
                
                auto iter = this->iter_gen->get(data); 

                return this->get(read_length, BIT_LENGTH_PER_SLOT, 0, iter);

            }

            template <class T1>
            auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t offset){
                
                offset = this->get_actual_idx(offset);
                auto iter = this->iter_gen->get(data, offset); 

                return this->get(read_length, BIT_LENGTH_PER_SLOT, offset, iter);

            }


        private:

            template <class T2>
            auto get(size_t read_length, size_t custom_width, size_t offset, T2 iter){

                return BitIterator<T2>(ACTUAL_BIT_LENGTH_PER_SLOT, custom_width, read_length, offset, iter); 

            }

    };
    
    template <class T>
    class BitIterWriter: public BitIterWritable<BitIterWriter<T>>, protected IteratorHelper{

        private:

            size_t actual_idx;            
            T or_writer_iter; // memory::sizet_linear::DynamicBitIterWritable<T>&

        public:

            BitIterWriter(size_t actual_bit_width, size_t virtual_bit_width, size_t virtual_write_size, size_t actual_idx,
                          T or_writer_iter): IteratorHelper(actual_bit_width, virtual_bit_width, virtual_write_size){
                
                this->actual_idx = actual_idx;
                this->or_writer_iter = or_writer_iter;

            }

            bool write(size_t data){
                                
                size_t actual_write_length = 0;
                int8_t istart_idx = 0;
                int8_t iend_idx = 0;

                this->get_metadata(this->actual_idx, actual_write_length, istart_idx, iend_idx);

                if (istart_idx == -1){

                    if (this->or_writer_iter.write(data, actual_write_length)){

                        this->actual_idx += actual_write_length;

                        return true;

                    }

                    return false;

                }

                size_t lo_crop = this->crop(data, 0, istart_idx);
                size_t hi_crop = this->crop(data, istart_idx , -1);
                size_t combined = (hi_crop << (iend_idx + 1)) | lo_crop;
               
                if (this->or_writer_iter.write(combined, actual_write_length)){
                    
                    this->actual_idx += actual_write_length; 

                    return true;

                }

                return false;

            }

    };

    template <class T, class ID>
    class BitIterWriterGenerator: public BitIterWriterGeneratable<BitIterWriterGenerator<T, ID>>
                                , private RPadIndexConverter<BitIterWriterGenerator<T, ID>>{

        private:

            std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_gen;

        public:

            BitIterWriterGenerator() {};


            BitIterWriterGenerator(std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_gen){

                this->iter_gen = iter_gen;

            }
            
            template <class T1>
            auto get(memory::sizet_linear::OperatableVector<T1>& data, size_t write_length, size_t offset){
                
                offset = RPadIndexConverter<BitIterWriterGenerator<T, ID>>::get_actual_idx(offset);
                auto iter_writer = this->iter_gen->get(data, offset);

                return this->get(write_length, offset, iter_writer);

            }


        private:

            template <class T1>
            BitIterWriter<T1> get(size_t write_length, size_t offset, T1 iter_writer){
                
                return BitIterWriter<T1>(ACTUAL_BIT_LENGTH_PER_SLOT, BIT_LENGTH_PER_SLOT, write_length, offset, iter_writer); 

            }

    };

    class ReverseBitIterator: public BitIterable<ReverseBitIterator>{ //reverse the read-order, not the bit-order

        private:

            std::vector<size_t> data;
        
        public:

            ReverseBitIterator() {};

            template <class T>
            ReverseBitIterator(BitIterable<T>& iter){
                
                size_t cur_read = 0;
                
                while (iter.next(cur_read)){

                    data.push_back(cur_read);

                }

            }

            bool next(size_t& rs){

                if (this->data.size() == 0){

                    return false;

                }

                rs = this->data.back();
                this->data.pop_back();

                return true;

            }
        

    };

    template <class T, class ID>
    class ReverseBitIteratorGenerator: public BitIteratorGeneratable<ReverseBitIteratorGenerator<T, ID>>{

        private:

            std::shared_ptr<BitIteratorGeneratable<T>> iter_gen; 

        public:

            ReverseBitIteratorGenerator() {};

            ReverseBitIteratorGenerator(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen){

                this->iter_gen = iter_gen;

            }

            template <class T1>
            auto get_custom_width(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t custom_width){

                auto iter = this->iter_gen->get(data, read_length, custom_width);

                return ReverseBitIterator(iter);

            }

            template <class T1>
            auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length){
                
                auto iter = this->iter_gen->get(data, read_length); 

                return ReverseBitIterator(iter);

            }

            template <class T1>
            auto get(memory::sizet_linear::VectorReadable<T1>& data, size_t read_length, size_t offset){
                
                auto iter = this->iter_gen->get(data, read_length, offset); 

                return ReverseBitIterator(iter);

            }


    };

    template <class T, class T1, class ID>
    class Discretizer: public Discretizable<Discretizer<T, T1, ID>>,
                       private BitLengthRetriever<Discretizer<T, T1, ID>>{

        private:

            std::shared_ptr<BitIteratorGeneratable<T>> iter_gen;
            std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen;


        public:

            Discretizer() {};

            Discretizer(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen,
                        std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen){
                
                this->iter_gen = iter_gen;
                this->opv_gen = opv_gen;

            }

            template <class T2>
            size_t get_instantiation_cost(memory::sizet_linear::VectorReadable<T2>& data, size_t bit_width){

                assert(bit_width <= ACTUAL_BIT_LENGTH_PER_SLOT);
     
                return this->get_sz(BitLengthRetriever<Discretizer<T, T1, ID>>::get_bit_length(data), bit_width) * sizeof(size_t);

            }
            
            template <class T2>
            auto discretize(memory::sizet_linear::VectorReadable<T2>& data, size_t bit_width, void * plm_space){

                assert(bit_width <= ACTUAL_BIT_LENGTH_PER_SLOT);
                
                size_t discretized_sz = this->get_sz(BitLengthRetriever<Discretizer<T, T1, ID>>::get_bit_length(data), bit_width);
                size_t cur_read = 0;
                size_t cur_length = 0; 

                auto rs = this->opv_gen->from_ptr(plm_space, discretized_sz);
                auto iter = this->iter_gen->get(data, bit_width);

                while (cur_length < discretized_sz){
                    
                    iter.next(cur_read);
                    rs.set(cur_length, cur_read);
                    ++cur_length;
                                        
                }
                
                return rs;

            }   


        private:

            size_t get_sz(size_t bit_length, size_t bit_width){

                if (bit_length % bit_width != 0){

                    return bit_length / bit_width + 1;

                }

                return bit_length / bit_width;

            }

    };

    template <class ID>
    class PlusEstimator: public Estimatable<PlusEstimator<ID>>{

        public:

            PlusEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return std::max(lhs.length(), rhs.length()) + 1;

            }


    };

    template <class ID>
    class PlusEstimatorv2: public Estimatable<PlusEstimatorv2<ID>>{

        public:

            PlusEstimatorv2() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){
                
                constexpr const size_t MAX_THRESHOLD = MAX_VAL_PER_SLOT >> 1;
                
                if (lhs.length() < rhs.length()){

                    return this->get(rhs, lhs);

                }

                if (lhs.get(lhs.length() - 1) < MAX_THRESHOLD){

                    return lhs.length();

                }

                return lhs.length() + 1;

            }


    };

    template <class ID>
    class PairWiseEstimator: public Estimatable<PairWiseEstimator<ID>>{

        public:

            PairWiseEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return std::max(lhs.length(), rhs.length());

            }



    };

    template <class ID>
    class MinusEstimator: public Estimatable<MinusEstimator<ID>>{

        public:

            MinusEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length(); 

            }


    };

    template <class ID>
    class RShiftEstimator: public ShiftEstimatable<RShiftEstimator<ID>>{

        public:

            RShiftEstimator() {};

            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                return lhs.length();

            }


    };
    
    template <class ID>
    class TrueRShiftEstimator: public ShiftEstimatable<TrueRShiftEstimator<ID>>, 
                               private LegalBitLengthRetriever<TrueRShiftEstimator<ID>>, 
                               private RPadIndexConverter<TrueRShiftEstimator<ID>>{

        public:

            TrueRShiftEstimator() {};

            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                size_t pre_length = this->get_bit_length(lhs);

                if (pre_length <= rhs){

                    return 1;

                }

                return RPadIndexConverter<TrueRShiftEstimator<ID>>::get_slot(pre_length - rhs - 1) + 1;

            }


    };

    template <class ID>
    class TrueLShiftEstimator: public ShiftEstimatable<TrueLShiftEstimator<ID>>, 
                               private LegalBitLengthRetriever<TrueLShiftEstimator<ID>>, 
                               private RPadIndexConverter<TrueLShiftEstimator<ID>>{

        public:

            TrueLShiftEstimator() {};
            
            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                size_t pre_length = this->get_bit_length(lhs);

                if ((pre_length == 1) && (lhs.get(0) == 0)){
                    
                    return 1;

                }

                return RPadIndexConverter<TrueLShiftEstimator<ID>>::get_slot(pre_length + rhs - 1) + 1;
                
            }


    };

    template <class ID>
    class LShiftEstimator: public ShiftEstimatable<LShiftEstimator<ID>>{

        public:

            LShiftEstimator() {};

            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){
                
                size_t sz = rhs / BIT_LENGTH_PER_SLOT + 1; 

                return lhs.length() + sz; 

            }


    };

    template <class ID>
    class MultiplyEstimator: public Estimatable<MultiplyEstimator<ID>>{

        public:

            MultiplyEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length() + rhs.length();

            }


    };

    template <class ID>
    class RelaxMultiplyEstimator: public Estimatable<RelaxMultiplyEstimator<ID>>{

        public:

            RelaxMultiplyEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length() + rhs.length() + 1;

            }


    };

    template <class ID>
    class DivideEstimator: public Estimatable<DivideEstimator<ID>>{

        public:

            DivideEstimator() {};

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length();

            }

    };

    template <class ID>
    class LightningPlusJudge: public Judgable<LightningPlusJudge<ID>>{
        
        private:

            size_t threshold; 

        public:

            LightningPlusJudge(){

                this->threshold = MAX_VAL_PER_SLOT >> 1;

            }

            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){
                                
                size_t lhs_last_idx = lhs.length() - 1;
                size_t rhs_last_idx = rhs.length() - 1; 

                if (lhs_last_idx == rhs_last_idx){ 
                    
                    return (rhs.get(lhs_last_idx) < lhs.get(lhs_last_idx)) && (lhs.get(lhs_last_idx) <= this->threshold); 

                }

                if ((lhs_last_idx > rhs_last_idx) && (lhs.get(lhs_last_idx) <= this->threshold)){

                    return true;

                }

                return false;

            }


    };

    template <class ID>
    class QuickMinusJudge: public Judgable<QuickMinusJudge<ID>>{
        
        public:

            QuickMinusJudge() {};

            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){
                
                uint8_t MIN_THRESHOLD = 1;
                size_t lhs_last_idx = lhs.length() - 1;
                size_t rhs_last_idx = rhs.length() - 1;

                if (lhs_last_idx == rhs_last_idx){

                    if ((lhs.get(lhs_last_idx) >> 1) > rhs.get(lhs_last_idx)){

                        return true;

                    }

                    return false;

                }

                if (lhs_last_idx > rhs_last_idx){

                    if (lhs.get(lhs_last_idx) > MIN_THRESHOLD){

                        return true;

                    }

                    return false;

                }

                return false;

            }


    };

    template <class ID>
    class RightShiftJudge: public ShiftJudgable<RightShiftJudge<ID>>, 
                           private RPadIndexConverter<RightShiftJudge<ID>>,
                           private LegalBitLengthRetriever<RightShiftJudge<ID>>{
        
        public:

            RightShiftJudge() {};
            
            template<class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                size_t pre_length = LegalBitLengthRetriever<RightShiftJudge<ID>>::get_bit_length(lhs);
                size_t post_length = 0;

                if (pre_length > rhs){

                    post_length = pre_length - rhs; 

                }

                if (post_length != 0 && pre_length != 0){

                    return this->get_slot(post_length - 1) == this->get_slot(pre_length - 1);

                } 

                return false;
                
            }

    };

    template <class ID>
    class LeftShiftJudge: public ShiftJudgable<LeftShiftJudge<ID>>,
                          private RPadIndexConverter<LeftShiftJudge<ID>>,
                          private LegalBitLengthRetriever<LeftShiftJudge<ID>>{
        
        public:

            LeftShiftJudge() {};

            template <class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){
                
                size_t pre_length = LegalBitLengthRetriever<LeftShiftJudge<ID>>::get_bit_length(lhs);

                if (pre_length == 0){

                    return true;

                }

                size_t post_length = pre_length + rhs;

                return this->get_slot(pre_length - 1) == this->get_slot(post_length - 1); 

            }


    };

    template <class ID>
    class ImmutableJudge: public ShiftJudgable<ImmutableJudge<ID>>{

        public:

            ImmutableJudge() {};

            template <class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                return false;

            }

    };

    template <class ID>
    class CustomPairWiseJudge: public Judgable<CustomPairWiseJudge<ID>>{
        
        public:

            CustomPairWiseJudge() {};
            
            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length() >= rhs.length();

            }

    };

    class StandardOperationUtilityGenerator{

        private:

            template <class T>
            auto init_reverse_bit_iter_gen(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen){

                return std::make_shared<ReverseBitIteratorGenerator<T, Generic>>(iter_gen);

            }

            template <class T>
            auto init_bit_writer_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_writer){
                
                return std::make_shared<BitIterWriterGenerator<T, Generic>>(iter_writer); 

            }

            template <class T>
            auto init_bit_iter_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen){

                return std::make_shared<BitIteratorGenerator<T, Generic>>(iter_gen);

            }

            template <class T, class T1>
            auto init_discretizer(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen,
                                  std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen){
                
                return std::make_shared<Discretizer<T, T1, Generic>>(iter_gen, opv_gen);

            }

        public:


            auto get_bit_iter_orwriter_gen(){ //-> shared_ptr<BitIterWriterGeneratable<>>

                auto dynamic_bit_writer = memory::sizet_linear::StandardGenerator().get_dynamic_bit_or_writer_gen();

                return this->init_bit_writer_gen(dynamic_bit_writer->to_dynamic_bit_iter_writer_generatable_sp(dynamic_bit_writer));

            }

            auto get_bit_iter_replace_writer_gen(){ //-> shared_ptr<BitIterWriterGeneratable<>>

                auto dynamic_bit_writer = memory::sizet_linear::StandardGenerator().get_dynamic_bit_replace_writer_gen();

                return this->init_bit_writer_gen(dynamic_bit_writer->to_dynamic_bit_iter_writer_generatable_sp(dynamic_bit_writer));

            }

            auto get_bit_iter_gen(){ // -> shared_ptr<BitIteratorGeneratable<>>

                auto dynamic_bit_iter = memory::sizet_linear::StandardGenerator().get_dynamic_bit_iter_gen(); 

                return this->init_bit_iter_gen(dynamic_bit_iter->to_dynamic_bit_iterator_generatable_sp(dynamic_bit_iter));

            }

            auto get_reverse_bit_iter_gen(){ // ->shared_ptr<BitIteratorGeneratable<>> reverse read-order, not bit-order

                auto std_iter = this->get_bit_iter_gen(); 
                auto casted = std_iter->to_bit_iterator_generatable_sp(std_iter);

                return this->init_reverse_bit_iter_gen(casted);

            } 

            auto get_discretizer(){ // -> shared_ptr<Discretizable<>>

                auto bit_iter = this->get_bit_iter_gen();
                auto opv_gen = memory::sizet_linear::StandardGenerator().get_dangling_op_vec_default_init_gen(); 
                
                auto casted_bit_iter = bit_iter->to_bit_iterator_generatable_sp(bit_iter);
                auto casted_opv_gen = opv_gen->to_dangling_operatable_vector_generatable_sp(opv_gen); 

                return this->init_discretizer(casted_bit_iter, casted_opv_gen);

            }

            auto get_view_trimmer(){ // -> shared_ptr<VectorViewTrimmable<>>

                return std::make_shared<VectorViewTrimmer<Generic>>();

            }

            auto get_default_guarded_vector_right_view_trimmer(){ // -> shared_ptr<VectorViewTrimmable<>>

                return std::make_shared<DefaultGuardedVectorRightViewTrimmer<Generic>>();

            }

            auto get_bit_length_retriever(){ // -> shared_ptr<BitLengthRetrievable<>> 

                return std::make_shared<BitLengthRetriever<Generic>>();

            }

            auto get_legal_bit_length_retriever(){ // -> shared_ptr<BitLengthRetrievable<>>

                return std::make_shared<LegalBitLengthRetriever<Generic>>();

            }

            auto get_legal_true_boolvector_view_caster(){ // -> shared_ptr<caster::BoolVectorViewCastable<>>

                return std::make_shared<LegalTrueBoolVectorViewCaster<Generic>>();
                
            } 

            auto get_trailing_zero_counter(){ // -> shared_ptr<TrailingZeroCountable<>>

                return std::make_shared<FastTrailingZeroCounter<Generic>>();

            }

            auto get_zero_detector(){ // -> shared_ptr<ZeroDetectable<>>

                return std::make_shared<ZeroDetector<Generic>>();

            }

            auto get_zero_generator(){ // -> shared_ptr<ZeroGeneratable<>>

                return std::make_shared<ZeroGenerator<Generic>>();

            }

            auto get_zero_default_splitter(){ // -> shared_ptr<VectorViewSplittable<>>

                return std::make_shared<StandardSplitter<Generic>>(); 

            }

            auto get_plus_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PlusEstimator<Generic>>();

            }

            auto get_plus_estimator_v2(){

                return std::make_shared<PlusEstimatorv2<Generic>>();

            }

            auto get_pairwise_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PairWiseEstimator<Generic>>();

            }

            auto get_minus_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MinusEstimator<Generic>>(); 

            }

            auto get_rshift_estimator(){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<RShiftEstimator<Generic>>();

            }

            auto get_true_rshift_estimator(){

                return std::make_shared<TrueRShiftEstimator<Generic>>();

            }

            auto get_lshift_estimator(){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<LShiftEstimator<Generic>>();

            }   

            auto get_true_lshift_estimator(){

                return std::make_shared<TrueLShiftEstimator<Generic>>();

            }

            auto get_multiply_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MultiplyEstimator<Generic>>(); 

            }

            auto get_relax_mul_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<RelaxMultiplyEstimator<Generic>>(); 

            }

            auto get_divide_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<DivideEstimator<Generic>>();

            }

            auto get_lightning_plus_judge(){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<LightningPlusJudge<Generic>>();

            }

            auto get_minus_judge(){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<QuickMinusJudge<Generic>>();

            }

            auto get_custom_pairwise_judge(){ // -> shared_ptr<Jugdable<>> 

                return std::make_shared<CustomPairWiseJudge<Generic>>();

            }

            auto get_rshift_judge(){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<RightShiftJudge<Generic>>();

            }

            auto get_lshift_judge(){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<LeftShiftJudge<Generic>>();

            }

            auto get_immutable_judge(){ // -> shared_ptr<Judgable<>>
                 
                 return std::make_shared<ImmutableJudge<Generic>>();

            }

    };

    class IDOperationUtilityGenerator{

        private:

            template <class T, class ID>
            auto init_reverse_bit_iter_gen(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen, ID){

                return std::make_shared<ReverseBitIteratorGenerator<T, ID>>(iter_gen);

            }

            template <class T, class ID>
            auto init_bit_writer_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_writer, ID){
                
                return std::make_shared<BitIterWriterGenerator<T, ID>>(iter_writer); 

            }

            template <class T, class ID>
            auto init_bit_iter_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen, ID){

                return std::make_shared<BitIteratorGenerator<T, ID>>(iter_gen);

            }

            template <class T, class T1, class ID>
            auto init_discretizer(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen,
                                  std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen, ID){
                
                return std::make_shared<Discretizer<T, T1, ID>>(iter_gen, opv_gen);

            }

        public:


            template <class ID>
            auto get_bit_iter_orwriter_gen(ID id_){ //-> shared_ptr<BitIterWriterGeneratable<>>

                auto dynamic_bit_writer = memory::sizet_linear::IDGenerator().get_dynamic_bit_or_writer_gen(id_);

                return this->init_bit_writer_gen(dynamic_bit_writer->to_dynamic_bit_iter_writer_generatable_sp(dynamic_bit_writer), id_);

            }

            template <class ID>
            auto get_bit_iter_replace_writer_gen(ID id_){ //-> shared_ptr<BitIterWriterGeneratable<>>

                auto dynamic_bit_writer = memory::sizet_linear::IDGenerator().get_dynamic_bit_replace_writer_gen(id_);

                return this->init_bit_writer_gen(dynamic_bit_writer->to_dynamic_bit_iter_writer_generatable_sp(dynamic_bit_writer), id_);

            }

            template <class ID>
            auto get_bit_iter_gen(ID id_){ // -> shared_ptr<BitIteratorGeneratable<>>

                auto dynamic_bit_iter = memory::sizet_linear::IDGenerator().get_dynamic_bit_iter_gen(id_); 

                return this->init_bit_iter_gen(dynamic_bit_iter->to_dynamic_bit_iterator_generatable_sp(dynamic_bit_iter), id_);

            }

            template <class ID>
            auto get_reverse_bit_iter_gen(ID id_){ // ->shared_ptr<BitIteratorGeneratable<>> reverse read-order, not bit-order

                auto std_iter = this->get_bit_iter_gen(id_); 
                auto casted = std_iter->to_bit_iterator_generatable_sp(std_iter);

                return this->init_reverse_bit_iter_gen(casted, id_);

            } 

            template <class ID>
            auto get_discretizer(ID id_){ // -> shared_ptr<Discretizable<>>

                auto bit_iter = this->get_bit_iter_gen(id_);
                auto opv_gen = memory::sizet_linear::StandardGenerator().get_dangling_op_vec_default_init_gen(); 
                
                auto casted_bit_iter = bit_iter->to_bit_iterator_generatable_sp(bit_iter);
                auto casted_opv_gen = opv_gen->to_dangling_operatable_vector_generatable_sp(opv_gen); 

                return this->init_discretizer(casted_bit_iter, casted_opv_gen, id_);

            }

            template <class ID>
            auto get_view_trimmer(ID){ // -> shared_ptr<VectorViewTrimmable<>>

                return std::make_shared<VectorViewTrimmer<ID>>();

            }

            template <class ID>
            auto get_default_guarded_vector_right_view_trimmer(ID){ // -> shared_ptr<VectorViewTrimmable<>>

                return std::make_shared<DefaultGuardedVectorRightViewTrimmer<ID>>();

            }

            template <class ID>
            auto get_bit_length_retriever(ID){ // -> shared_ptr<BitLengthRetrievable<>> 

                return std::make_shared<BitLengthRetriever<ID>>();

            }

            template <class ID>
            auto get_legal_bit_length_retriever(ID){ // -> shared_ptr<BitLengthRetrievable<>>

                return std::make_shared<LegalBitLengthRetriever<ID>>();

            }

            template <class ID>
            auto get_legal_true_boolvector_view_caster(ID){ // -> shared_ptr<caster::BoolVectorViewCastable<>>

                return std::make_shared<LegalTrueBoolVectorViewCaster<ID>>();
                
            } 

            template <class ID>
            auto get_trailing_zero_counter(ID){ // -> shared_ptr<TrailingZeroCountable<>>

                return std::make_shared<FastTrailingZeroCounter<ID>>();

            }

            template <class ID>
            auto get_zero_detector(ID){ // -> shared_ptr<ZeroDetectable<>>

                return std::make_shared<ZeroDetector<ID>>();

            }

            template <class ID>
            auto get_zero_generator(ID){ // -> shared_ptr<ZeroGeneratable<>>

                return std::make_shared<ZeroGenerator<ID>>();

            }

            template <class ID>
            auto get_zero_default_splitter(ID){ // -> shared_ptr<VectorViewSplittable<>>

                return std::make_shared<StandardSplitter<ID>>(); 

            }

            template <class ID>
            auto get_plus_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PlusEstimator<ID>>();

            }

            template <class ID>
            auto get_plus_estimator_v2(ID){

                return std::make_shared<PlusEstimatorv2<ID>>();

            }

            template <class ID>
            auto get_pairwise_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PairWiseEstimator<ID>>();

            }

            template <class ID>
            auto get_minus_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MinusEstimator<ID>>(); 

            }

            template <class ID>
            auto get_rshift_estimator(ID){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<RShiftEstimator<ID>>();

            }

            template <class ID>
            auto get_true_rshift_estimator(ID){

                return std::make_shared<TrueRShiftEstimator<ID>>();

            }

            template <class ID>
            auto get_lshift_estimator(ID){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<LShiftEstimator<ID>>();

            }   

            template <class ID>
            auto get_true_lshift_estimator(ID){

                return std::make_shared<TrueLShiftEstimator<ID>>();

            }

            template <class ID>
            auto get_multiply_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MultiplyEstimator<ID>>(); 

            }

            template <class ID>
            auto get_relax_mul_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<RelaxMultiplyEstimator<ID>>(); 

            }

            template <class ID>
            auto get_divide_estimator(ID){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<DivideEstimator<ID>>();

            }

            template <class ID>
            auto get_lightning_plus_judge(ID){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<LightningPlusJudge<ID>>();

            }

            template <class ID>
            auto get_minus_judge(ID){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<QuickMinusJudge<ID>>();

            }

            template <class ID>
            auto get_custom_pairwise_judge(ID){ // -> shared_ptr<Jugdable<>> 

                return std::make_shared<CustomPairWiseJudge<ID>>();

            }

            template <class ID>
            auto get_rshift_judge(ID){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<RightShiftJudge<ID>>();

            }

            template <class ID>
            auto get_lshift_judge(){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<LeftShiftJudge<ID>>();

            }

            template <class ID>
            auto get_immutable_judge(ID){ // -> shared_ptr<Judgable<>>
                 
                 return std::make_shared<ImmutableJudge<ID>>();

            }

    };

}  

namespace bignum::vector::mutable_operation{
    
    template <class T, unsigned>
    class DoubleID{};

    template <class ID>
    class AssignOperator: public Operatable<AssignOperator<ID>>{

        public:
            
            AssignOperator() {};

            template <class T, class T1>
            void ops(memory::sizet_linear::OperatableVector<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                for (size_t i = 0; i < rhs.length(); ++i){

                    lhs.set(i, rhs.get(i));

                }

                for (size_t i = rhs.length(); i < lhs.length(); ++i){

                    lhs.set(i, NULL_VALUE);

                }

            }

    };

    template <class ID>
    class PlusOperator: public Operatable<PlusOperator<ID>>{

        public:

            PlusOperator() {};
        
            template <class T1, class T2>
            void ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                size_t total = 0;
                size_t i = 0;

                for (i = 0; i < rhs.length(); ++i){

                    total += lhs.get(i) + rhs.get(i);
                    lhs.set(i, total & MAX_VAL_PER_SLOT);
                    total >>= BIT_LENGTH_PER_SLOT; 

                }

                while (total){
                    
                    total += lhs.get(i);
                    lhs.set(i, total & MAX_VAL_PER_SLOT);
                    total >>= BIT_LENGTH_PER_SLOT;

                    ++i;

                }


            }

    };

    template <class ID>
    class MinusOperator: public Operatable<MinusOperator<ID>>{

        public:

            MinusOperator() {};

            template <class T1, class T2>
            void ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                assert(BIT_LENGTH_PER_SLOT + 1 < ACTUAL_BIT_LENGTH_PER_SLOT);
                
                constexpr const size_t BORROW_VAL = MAX_VAL_PER_SLOT + 1; 

                intmax_t rs = 0;
                size_t i = 0;
                size_t borrow = 0;

                for (i = 0; i < rhs.length(); ++i){

                    rs = (intmax_t) lhs.get(i) - (rhs.get(i) + borrow);
                    borrow = 0;

                    while (rs < 0){

                        rs += BORROW_VAL;
                        borrow += 1;

                    }

                    lhs.set(i, rs);

                }

                while (borrow){

                    rs = (intmax_t) lhs.get(i) - borrow; 

                    if (rs < 0){

                        rs += BORROW_VAL;
                        borrow = 1;

                    } else{

                        borrow = 0;

                    }

                    lhs.set(i, rs);
                    ++i;

                }

            }

    };

    template <class T, class T1, class T2, class ID>
    class RShiftOperator: public ShiftOperatable<RShiftOperator<T, T1, T2, ID>>,
                          private T, private T1, private T2{

        public:

            RShiftOperator() {};

            RShiftOperator(std::shared_ptr<operation_utility::BitIteratorGeneratable<T>> read_iter_gen,
                           std::shared_ptr<operation_utility::BitIterWriterGeneratable<T1>> write_iter_gen,
                           std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever): T(static_cast<T&>(*read_iter_gen)), 
                                                                                                               T1(static_cast<T1&>(*write_iter_gen)), 
                                                                                                               T2(static_cast<T2&>(*bit_length_retriever)){}


            template <class T3>
            void ops(memory::sizet_linear::OperatableVector<T3>& lhs, size_t rhs){
                
                size_t pre_length = T2::get_bit_length(lhs);
                size_t post_length = (pre_length > rhs) ? pre_length - rhs : 0; 
                size_t read_length = BIT_LENGTH_PER_SLOT;
                size_t data = 0;
                
                auto write_iter = T1::get(lhs, read_length, 0);

                if (post_length != 0){
                    
                    auto read_iter = T::get(lhs, read_length, rhs);

                    for (size_t i = 0; i < post_length; i += read_length){
                        
                        assert(read_iter.next(data));
                        assert(write_iter.write(data));

                    }        

                }

                while (write_iter.write(0));

            }


    };

    template <class ID>
    class PairWiseOperator: public PairWiseOperatable<PairWiseOperator<ID>>{

        public:

            PairWiseOperator() {};

            template <class T, class T1>
            void custom_ops(memory::sizet_linear::OperatableVector<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs, pairwise_ops lambda){

                for (size_t i = 0; i < rhs.length(); ++i){

                    lhs.set(i, lambda(lhs.get(i), rhs.get(i)));

                }
                
                for (size_t i = rhs.length(); i < lhs.length(); ++i){

                    lhs.set(i, lambda(lhs.get(i), NULL_VALUE));

                }

            };

    };

    class IDArithmeticOperatorGenerator{

        private:

            template <class ID>
            auto init_plus_ops(ID){

                return std::make_shared<PlusOperator<ID>>(); 

            }

            template <class ID>
            auto init_minus_ops(ID){

                return std::make_shared<MinusOperator<ID>>(); 

            }

            template <class T, class T1, class T2, class ID>
            auto init_rshift_ops(std::shared_ptr<operation_utility::BitIteratorGeneratable<T>> read_iter_gen,
                                 std::shared_ptr<operation_utility::BitIterWriterGeneratable<T1>> write_iter_gen,
                                 std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever,
                                 ID){
                
                return std::make_shared<RShiftOperator<T, T1, T2, ID>>(read_iter_gen, write_iter_gen, bit_length_retriever); 

            }

            template <class ID>
            auto init_assigner(ID){

                return std::make_shared<AssignOperator<ID>>();

            }

            template <class ID>
            auto init_custom_pairwise(ID){

                return std::make_shared<PairWiseOperator<ID>>();

            }

        public:
            
            template <class ID>
            auto get_assigner(ID id){ // -> shared_ptr<Operatable<>>

                return this->init_assigner(id);

            }

            template <class ID>
            auto get_plus(ID id){ // -> shared_ptr<Operatable<>>
                
                return this->init_plus_ops(id);

            } 

            template <class ID>
            auto get_minus(ID id){ // -> shared_ptr<Operatable<>>

                return this->init_minus_ops(id);

            }

            template <class ID>
            auto get_rshift(ID id){ // -> shared_ptr<ShiftOperatable<>>

                auto read_iter_gen = operation_utility::IDOperationUtilityGenerator().get_bit_iter_gen(DoubleID<ID, 0>());
                auto write_iter_gen = operation_utility::IDOperationUtilityGenerator().get_bit_iter_replace_writer_gen(DoubleID<ID, 1>());
                auto bit_length_retriever = operation_utility::IDOperationUtilityGenerator().get_bit_length_retriever(DoubleID<ID, 2>());

                auto casted_read_iter_gen = read_iter_gen->to_bit_iterator_generatable_sp(read_iter_gen);
                auto casted_write_iter_gen = write_iter_gen->to_bit_iter_writer_generatable_sp(write_iter_gen);
                auto casted_bit_length_retriever = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_rshift_ops(casted_read_iter_gen, casted_write_iter_gen, casted_bit_length_retriever);

            }

            template <class ID>
            auto get_custom_pairwise(ID id){ // -> shared_ptr<PairWiseOperatable<>>
               
                return this->init_custom_pairwise(id);
            
            }

    };

}

namespace bignum::vector::immutable_operation{
    
    template <class T, unsigned>
    class DoubleID{};

    template <class T, class ID>
    class PlusOperator: public Operatable<PlusOperator<T, ID>>,
                        private T{

        public:

            PlusOperator() {};

            PlusOperator(std::shared_ptr<operation_utility::Estimatable<T>> estimator): T(static_cast<T&>(*estimator)){}
            
            template <class T1, class T2, class T3>
            void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){

                if (lhs.length() < rhs.length()){

                    this->ops(rhs, lhs, rs);

                    return;

                }

                size_t est_n = T::get(lhs, rhs);
                size_t i = 0;
                size_t carry = 0;

                rs.resize_no_copy(est_n);

                while (i < rhs.length()){

                    carry += rhs.get(i) + lhs.get(i);
                    rs.set(i, carry & MAX_VAL_PER_SLOT);
                    carry >>= BIT_LENGTH_PER_SLOT;
                    ++i;

                }

                while (carry && (i < lhs.length())){

                    carry += lhs.get(i);
                    rs.set(i, carry & MAX_VAL_PER_SLOT);
                    carry >>= BIT_LENGTH_PER_SLOT;
                    ++i;

                }

                if (carry){

                    rs.set(i, carry);
                    ++i;

                } else {

                    while (i < lhs.length()){

                        rs.set(i, lhs.get(i));
                        ++i;

                    }

                }

                if (i != est_n){
                    
                    rs.resize(i);

                }

            }


    };

    template <class T, class ID>
    class MinusOperator: public Operatable<MinusOperator<T, ID>>,
                         private T{

        public:

            MinusOperator() {};

            MinusOperator(std::shared_ptr<operation_utility::Estimatable<T>> estimator): T(static_cast<T&>(*estimator)){}

            template <class T1, class T2, class T3>
            void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){
                                
                size_t est_n = T::get(lhs, rhs);
                size_t i = 0;
                intmax_t minus_rs = 0;
                size_t borrow = 0;

                rs.resize_no_copy(est_n); 
                
                while (i < rhs.length()){

                    minus_rs = (intmax_t) lhs.get(i) - rhs.get(i) - borrow;
                    borrow = 0;

                    while (minus_rs < 0){

                        minus_rs += MAX_VAL_PER_SLOT + 1;
                        borrow += 1;

                    }

                    rs.set(i, minus_rs);
                    ++i;

                }

                while (borrow){
                    
                    minus_rs = (intmax_t) lhs.get(i) - borrow;
                    
                    if (minus_rs < 0){

                        minus_rs += MAX_VAL_PER_SLOT + 1;
                        borrow = 1;

                    } else{

                        borrow = 0;

                    }

                    rs.set(i, minus_rs);
                    ++i;

                }

                while (i < lhs.length()){

                    rs.set(i, lhs.get(i));
                    ++i;

                }

                while ((i > 1) && (rs.get(i - 1) == 0)){

                    --i;
                
                }

                if (i != est_n){

                    rs.resize(i);

                }
            }

    };

    template <class T, class T2, class ID>
    class TwoLimbMultiplication: public Operatable<TwoLimbMultiplication<T, T2, ID>>,
                                 private T2{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            
            const uint8_t BIT_PER_LIMB = BIT_LENGTH_PER_SLOT >> 1;
            const uint8_t LIMB_PER_SLOT = 2;

            template <class U, class U1, class U2>
            friend class TwoLimbMultiplication;

        public:

            TwoLimbMultiplication() {};

            TwoLimbMultiplication(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                  std::shared_ptr<operation_utility::Estimatable<T2>> estimator): T2(static_cast<T2&>(*estimator)){
                
                assert(BIT_LENGTH_PER_SLOT % 2 == 0);
                
                this->allocator = allocator;

            }
            
            template <class T4, class T5, class T6>
            void ops(memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T6>& op){ // -> VectorReadable<>&

                if (lhs.length() > rhs.length()){

                    this->ops(rhs, lhs, op);
                    return;

                }
                

                size_t n = T2::get(lhs, rhs);
                size_t flatten_lhs = lhs.length() * this->LIMB_PER_SLOT;
                size_t flatten_rhs = rhs.length() * this->LIMB_PER_SLOT;
                size_t flatten_n = n * this->LIMB_PER_SLOT;
                size_t i = 0;

                this->allocator->enter_scope();
                auto rs = this->allocator->vec_get(flatten_n);

                if (this->prop_default(lhs, rhs, rs)){

                    i = flatten_rhs + 1;

                } else{

                    i = flatten_rhs;

                }

                while (i < flatten_n){

                    rs.set(i, 0);
                    ++i;

                }

                for (i = 1; i < flatten_lhs; ++i){

                    this->prop(i, lhs, rhs, rs);

                }

                while ((flatten_n > 1) && (rs.get(flatten_n - 1) == 0)){

                    --flatten_n;

                } 

                size_t op_n = (flatten_n % this->LIMB_PER_SLOT == 0) ? flatten_n / this->LIMB_PER_SLOT : (flatten_n / this->LIMB_PER_SLOT) + 1;
                size_t idx = 0;
                op.resize_no_copy(op_n);

                for (i = 0; i < op_n; ++i){
                    
                    op.set(i, (rs.get(idx + 1) << this->BIT_PER_LIMB) | rs.get(idx));
                    idx += 2;

                }

                this->allocator->exit_scope();

            }
        
        private:    

            template <class T4, class T5, class T6>
            void prop(size_t idx, memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs, memory::sizet_linear::OperatableVector<T6>& rs){
                
                const size_t MAX_VAL_PER_LIMB = ((size_t) 1 << this->BIT_PER_LIMB) - 1;
                size_t carry = 0;
                size_t multiplier = (idx % 2 == 0) ? lhs.get(idx >> 1) & MAX_VAL_PER_LIMB: lhs.get(idx >> 1) >> BIT_PER_LIMB;

                for (size_t j = 0; j < rhs.length(); ++j){
                        
                    carry += multiplier * (rhs.get(j) & MAX_VAL_PER_LIMB) + rs.get(idx);
                    rs.set(idx, carry & MAX_VAL_PER_LIMB);
                    carry >>= this->BIT_PER_LIMB;

                    ++idx;

                    carry += multiplier * (rhs.get(j) >> BIT_PER_LIMB) + rs.get(idx);
                    rs.set(idx, carry & MAX_VAL_PER_LIMB);
                    carry >>= this->BIT_PER_LIMB;

                    ++idx;

                }

                if (carry != 0){

                    rs.set(idx, carry + rs.get(idx));

                }

            }
    
            template <class T4, class T5, class T6>
            bool prop_default(memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs, memory::sizet_linear::OperatableVector<T6>& rs){
                
                const size_t MAX_VAL_PER_LIMB = ((size_t) 1 << this->BIT_PER_LIMB) - 1;
                size_t carry = 0;
                size_t idx = 0;
                size_t multiplier = lhs.get(idx) & MAX_VAL_PER_LIMB;

                for (size_t j = 0; j < rhs.length(); ++j){
                        
                    carry += multiplier * (rhs.get(j) & MAX_VAL_PER_LIMB);
                    rs.set(idx, carry & MAX_VAL_PER_LIMB);
                    carry >>= this->BIT_PER_LIMB;

                    ++idx;

                    carry += multiplier * (rhs.get(j) >> BIT_PER_LIMB);
                    rs.set(idx, carry & MAX_VAL_PER_LIMB);
                    carry >>= this->BIT_PER_LIMB;

                    ++idx;

                }

                if (carry != 0){

                    rs.set(idx, carry);

                    return true;

                }

                return false;

            }

    };

    template <class T, class ID>
    class UINT128Multiplication: public Operatable<UINT128Multiplication<T, ID>>,
                                 private T{

        public:

            UINT128Multiplication() {};

            UINT128Multiplication(std::shared_ptr<operation_utility::Estimatable<T>> estimator): T(static_cast<T&>(*estimator)){}

            template <class T1, class T2, class T3>
            void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& op){ 
                
                if (lhs.length() > rhs.length()){

                    this->ops(rhs, lhs, op);

                    return;

                }

                __uint128_t carry = 0;
                __uint128_t lhs_0 = lhs.get(0);
                size_t i = 0;
                size_t j = 0;
                size_t idx = 0; 
                size_t n = T::get(lhs, rhs);
                 
                op.resize_no_copy(n);
    
                for (i = 0; i < rhs.length(); ++i){

                    carry += lhs_0 * rhs.get(i);
                    op.set(i, carry & MAX_VAL_PER_SLOT);
                    carry >>= BIT_LENGTH_PER_SLOT; 

                }

                if (carry != 0){

                    op.set(i, carry);
                    carry = 0;
                    ++i;

                }

                while (i < op.length()){
                    
                    op.set(i, 0);
                    ++i;

                }

                for (i = 1; i < lhs.length(); ++i){
                    
                    idx = i;

                    for (j = 0; j < rhs.length(); ++j){

                        carry += (__uint128_t) lhs.get(i) * rhs.get(j) + op.get(idx);
                        op.set(idx, carry & MAX_VAL_PER_SLOT);
                        carry >>= BIT_LENGTH_PER_SLOT;

                        ++idx; 

                    }

                    if (carry){

                        op.set(idx, carry);
                        carry = 0;
                        ++idx;

                    }

                }

                while ((n > 1) && (op.get(n - 1) == 0)){

                    --n;
                
                }

                op.resize(n);
                
            }

    };

    template <class T, class T1, class T2, class ID>
    class LShiftOperator: public ShiftOperatable<LShiftOperator<T, T1, T2, ID>>,
                          private T, private T1, private T2{

        private:
            
            size_t write_delta; 

        public:

            LShiftOperator() {};

            LShiftOperator(std::shared_ptr<operation_utility::ShiftEstimatable<T>> true_estimator,
                           std::shared_ptr<operation_utility::BitIteratorGeneratable<T1>> read_iter_gen,
                           std::shared_ptr<operation_utility::BitIterWriterGeneratable<T2>> or_writer_iter_gen,
                           size_t write_delta): T(static_cast<T&>(*true_estimator)), T1(static_cast<T1&>(*read_iter_gen)), T2(static_cast<T2&>(*or_writer_iter_gen)) {
                
                this->write_delta = write_delta;

            }

            template <class T3, class T4>
            void ops(memory::sizet_linear::VectorReadable<T3>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T4>& rs){ // -> VectorReadable<>&

                size_t n = T::get(lhs, rhs);
                size_t cur_read = 0;

                rs.resize_no_copy(n);

                for (size_t i = 0; i < rs.length(); ++i){

                    rs.set(i, 0);

                }

                auto write_iter = T2::get(rs, this->write_delta, rhs);
                auto read_iter = T1::get(lhs, this->write_delta);

                while (read_iter.next(cur_read)){
                    
                    write_iter.write(cur_read);

                } 

            }
        

    };

    template <class T, class T1, class T2, class T3, class ID>
    class RShiftOperator: public ShiftOperatable<RShiftOperator<T, T1, T2, T3, ID>>,
                          private T, private T1, private T2, private T3{

        private:

            size_t write_delta; 

        public:

            RShiftOperator() {};

            RShiftOperator(std::shared_ptr<operation_utility::ShiftEstimatable<T>> true_estimator,
                           std::shared_ptr<operation_utility::BitIteratorGeneratable<T1>> read_iter_gen,
                           std::shared_ptr<operation_utility::BitIterWriterGeneratable<T2>> replace_writer_iter_gen,
                           std::shared_ptr<operation_utility::BitLengthRetrievable<T3>> bit_length_retriever,
                           size_t write_delta): T(static_cast<T&>(*true_estimator)), 
                                                T1(static_cast<T1&>(*read_iter_gen)),
                                                T2(static_cast<T2&>(*replace_writer_iter_gen)),
                                                T3(static_cast<T3&>(*bit_length_retriever)){
                
                this->write_delta = write_delta;

            }

            template <class T4, class T5>
            void ops(memory::sizet_linear::VectorReadable<T4>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T5>& rs){

                size_t n = T::get(lhs, rhs);
                size_t pre_length = T3::get_bit_length(lhs);
                size_t post_length = (pre_length > rhs) ? pre_length - rhs : 0;
                size_t cur_read = 0;

                rs.resize_no_copy(n); 
                auto write_iter = T2::get(rs, this->write_delta, 0);
                
                if (post_length != 0){

                    auto read_iter = T1::get(lhs, this->write_delta, rhs);

                    while (read_iter.next(cur_read)){
                        
                        write_iter.write(cur_read);

                    }

                }
                
                while (write_iter.write(0));

            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class ID>
    class RecursiveKaratsubaMultiplicationOperator: public Operatable<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6, T7, ID>>,
                                                    private T, private T1, private T2, private T3, private T4, private T5, private T7{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T6>> temp_gen_allocator;
            const uint8_t NAIVE_MUL_THRESHOLD = 50;            

        public:

            using Operatable<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6, T7, ID>>::to_operatable_sp;

            RecursiveKaratsubaMultiplicationOperator() {};

            RecursiveKaratsubaMultiplicationOperator(std::shared_ptr<operation_utility::VectorViewSplittable<T>> splitter,
                                                     std::shared_ptr<Operatable<T1>> plus_ops,
                                                     std::shared_ptr<Operatable<T2>> minus_ops,
                                                     std::shared_ptr<ShiftOperatable<T3>> lshifter,
                                                     std::shared_ptr<Operatable<T4>> multiplier,
                                                     std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T5>> temp_gen, //relax resize temp_gen
                                                     std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T6>> temp_gen_allocator,
                                                     std::shared_ptr<operation_utility::Estimatable<T7>> estimator): T(static_cast<T&>(*splitter)), // relax estimator
                                                                                                                     T1(static_cast<T1&>(*plus_ops)),
                                                                                                                     T2(static_cast<T2&>(*minus_ops)),
                                                                                                                     T3(static_cast<T3&>(*lshifter)),
                                                                                                                     T4(static_cast<T4&>(*multiplier)),
                                                                                                                     T5(static_cast<T5&>(*temp_gen)),
                                                                                                                     T7(static_cast<T7&>(*estimator)){

                this->temp_gen_allocator = temp_gen_allocator;

            }

            template <class T8, class T9, class T10>
            void ops(memory::sizet_linear::VectorReadable<T8>& lhs, memory::sizet_linear::VectorReadable<T9>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T10>& rs){

                if (lhs.length() < rhs.length()){

                    this->ops(rhs, lhs, rs);

                    return;

                }

                if (rhs.length() <= NAIVE_MUL_THRESHOLD){
                    
                    T4::ops(lhs, rhs, rs);

                    return; 

                }
                
                this->temp_gen_allocator->enter_scope();

                size_t split_point = lhs.length() >> 1;                 
                size_t lower_bit_length = split_point * BIT_LENGTH_PER_SLOT;

                auto lhs_splitted = T::split(lhs, split_point);
                auto rhs_splitted = T::split(rhs, split_point);

                auto p1 = T5::get();
                auto p2 = T5::get();
                auto p3 = T5::get();
                auto p4 = T5::get();
                auto z1 = T5::get();
                auto z2 = T5::get();
                auto z3 = T5::get();
                auto z4 = T5::get();
                auto s1 = T5::get();
                auto s2 = T5::get();

                T1::ops(lhs_splitted.first, lhs_splitted.second, p1);
                T1::ops(rhs_splitted.first, rhs_splitted.second, p2);

                z1.resize_no_copy(T7::get(lhs_splitted.first, rhs_splitted.first));
                z2.resize_no_copy(T7::get(lhs_splitted.second, rhs_splitted.second));
                z3.resize_no_copy(T7::get(*p1.to_vector_readable(), *p2.to_vector_readable()));

                this->ops(lhs_splitted.first, rhs_splitted.first, z1);
                this->ops(lhs_splitted.second, rhs_splitted.second, z2);
                this->ops(*p1.to_vector_readable(), *p2.to_vector_readable(), z3);    

                T1::ops(*z1.to_vector_readable(), *z2.to_vector_readable(), p3);
                T2::ops(*z3.to_vector_readable(), *p3.to_vector_readable(), z4);

                T3::ops(*z2.to_vector_readable(), lower_bit_length << 1, s1);
                T3::ops(*z4.to_vector_readable(), lower_bit_length, s2); 

                T1::ops(*z1.to_vector_readable(), *s1.to_vector_readable(), p4); 
                T1::ops(*p4.to_vector_readable(), *s2.to_vector_readable(), rs); 

                this->temp_gen_allocator->exit_scope(); 

            }

    };
    
    template <class T, class ID>
    class CustomPairWiseOperator: public PairWiseOperatable<CustomPairWiseOperator<T, ID>>,
                                  private T{

        public:

            CustomPairWiseOperator() {};

            CustomPairWiseOperator(std::shared_ptr<operation_utility::Estimatable<T>> estimator): T(static_cast<T&>(*estimator)){}

            template <class T1, class T2, class T3>
            void ops(memory::sizet_linear::VectorReadable<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs, pairwise_ops pw_ops){
                
                if (lhs.length() < rhs.length()){

                    this->ops(rhs, lhs, rs, pw_ops);

                    return;

                }

                size_t n = T::get(lhs, rhs);
                size_t i = 0;
                rs.resize_no_copy(n); 

                for (i = 0; i < rhs.length(); ++i){

                    rs.set(i, pw_ops(lhs.get(i), rhs.get(i)));

                }

                for (i = rhs.length(); i < lhs.length(); ++i){
                    
                    rs.set(i, pw_ops(lhs.get(i), 0));

                }
                
                while ((i > 1) && (rs.get(i - 1) == 0)){

                    --i;
                
                }

                if (i != n){
                
                    rs.resize(i);
                
                }

            }
            
    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class ID>
    class RecursiveDivider: public Operatable<RecursiveDivider<T, T1, T2, T3, T4, T5, T6, T7, T8, ID>>,
                            private T, private T1, private T2, private T3, private T4, private T5, private T6, private T7{
    
        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T8>> temp_gen_allocator; 

        public:

            using Operatable<RecursiveDivider<T, T1, T2, T3, T4, T5, T6, T7, T8, ID>>::to_operatable_sp;

            RecursiveDivider() {};

            RecursiveDivider(std::shared_ptr<Operatable<T>> minus_ops,
                            std::shared_ptr<operation_utility::BitLengthRetrievable<T1>> bit_length_retriever,
                            std::shared_ptr<operation_utility::Estimatable<T2>> estimator,
                            std::shared_ptr<operation_utility::ZeroDetectable<T3>> zero_detector,
                            std::shared_ptr<comparer::Comparable<T4>> comparer,
                            std::shared_ptr<ShiftOperatable<T5>> lshifter,
                            std::shared_ptr<caster::BoolVectorOpCastable<T6>> full_op_caster,
                            std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T7>> temp_gen,
                            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T8>> temp_gen_allocator): T(static_cast<T&>(*minus_ops)), 
                                                                                                                   T1(static_cast<T1&>(*bit_length_retriever)),
                                                                                                                   T2(static_cast<T2&>(*estimator)), 
                                                                                                                   T3(static_cast<T3&>(*zero_detector)),
                                                                                                                   T4(static_cast<T4&>(*comparer)), 
                                                                                                                   T5(static_cast<T5&>(*lshifter)),
                                                                                                                   T6(static_cast<T6&>(*full_op_caster)), 
                                                                                                                   T7(static_cast<T7&>(*temp_gen)){
                
                this->temp_gen_allocator = temp_gen_allocator;

            }

            template <class T9, class T10, class T11>
            auto ops(memory::sizet_linear::VectorReadable<T9>& lhs, memory::sizet_linear::VectorReadable<T10>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ // -> VectorReadable&

                assert(!(T3::is_zero(rhs)));

                size_t est_n = T2::get(lhs, rhs);
                size_t n = est_n;
                size_t rhs_length = T1::get_bit_length(rhs); 

                rs.resize(est_n);
                
                for (size_t i = 0 ; i < n; ++i){
                    
                    rs.set(i, 0);

                }

                auto casted_rs = T6::cast(rs); 
                this->divide(lhs, rhs, rhs_length, casted_rs);

                while ((n > 1) && (rs.get(n - 1) == 0)){

                    --n;
                
                }

                if (n != est_n){

                    rs.resize(n);

                }

            }

        private:

            template <class T9, class T10, class T11>
            void divide(memory::sizet_linear::VectorReadable<T9>& lhs, memory::sizet_linear::VectorReadable<T10>& rhs, size_t rhs_length, datastructure::UnalignedOperatableBoolVector<T11>& rs){

                size_t bit_dif = 0; 
                size_t lhs_length = T1::get_bit_length(lhs);

                if (lhs_length < rhs_length){

                    return;

                }
                
                if (lhs_length == rhs_length){

                    if (T4::compare(lhs, rhs) >= 0){

                        this->bitwise_plus(rs, 0);

                    }

                    return;

                }

                this->temp_gen_allocator->enter_scope();
                
                bit_dif = lhs_length - rhs_length - 1;
                this->bitwise_plus(rs, bit_dif);

                auto shifted = T7::get();
                auto remainder = T7::get();

                T5::ops(rhs, bit_dif, shifted);
                T::ops(lhs, *shifted.to_vector_readable(), remainder);

                auto exited_remainder = this->temp_gen_allocator->exit_scope(*remainder.to_vector_readable());
                this->divide(exited_remainder, rhs, rhs_length, rs);

            }

            template <class T9>
            void bitwise_plus(datastructure::UnalignedOperatableBoolVector<T9>& rs, size_t idx){

                while (rs.get(idx)){

                    rs.set_val(idx, false);
                    ++idx;

                }

                rs.set_val(idx, true);

            }
    };

    class IDArithmeticGenerator{

        private:

            template <class T, class ID>
            std::shared_ptr<PlusOperator<T, ID>> init_plus_ops(std::shared_ptr<operation_utility::Estimatable<T>> estimator, 
                                                               ID){

                return std::make_shared<PlusOperator<T, ID>>(estimator); 

            }

            template <class T, class ID>
            std::shared_ptr<MinusOperator<T, ID>> init_minus_ops(std::shared_ptr<operation_utility::Estimatable<T>> estimator, 
                                                                 ID){

                return std::make_shared<MinusOperator<T, ID>>(estimator);

            }

            template <class T, class T1, class ID>
            std::shared_ptr<TwoLimbMultiplication<T, T1, ID>> init_two_limb_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> temp_storage,
                                                                                std::shared_ptr<operation_utility::Estimatable<T1>> estimator, 
                                                                                ID){
                
                return std::make_shared<TwoLimbMultiplication<T, T1, ID>>(temp_storage, estimator);

            }

            template <class T, class ID>
            std::shared_ptr<UINT128Multiplication<T, ID>> init_uint128_mul(std::shared_ptr<operation_utility::Estimatable<T>> estimator, 
                                                                           ID){
                
                return std::make_shared<UINT128Multiplication<T, ID>>(estimator);

            }

            template <class T, class T1, class T2, class ID>
            std::shared_ptr<LShiftOperator<T, T1, T2, ID>> init_lshift(std::shared_ptr<operation_utility::ShiftEstimatable<T>> true_estimator,
                                                                       std::shared_ptr<operation_utility::BitIteratorGeneratable<T1>> read_iter_gen,
                                                                       std::shared_ptr<operation_utility::BitIterWriterGeneratable<T2>> or_write_iter_gen,
                                                                       size_t write_delta,
                                                                       ID){
                
                return std::make_shared<LShiftOperator<T, T1, T2, ID>>(true_estimator, read_iter_gen, or_write_iter_gen, write_delta);

            }

            template <class T, class T1, class T2, class T3, class ID>
            std::shared_ptr<RShiftOperator<T, T1, T2, T3, ID>> init_r_shift(std::shared_ptr<operation_utility::ShiftEstimatable<T>> true_estimator,
                                                                        std::shared_ptr<operation_utility::BitIteratorGeneratable<T1>> read_iter_gen,
                                                                        std::shared_ptr<operation_utility::BitIterWriterGeneratable<T2>> replace_writer_iter_gen,
                                                                        std::shared_ptr<operation_utility::BitLengthRetrievable<T3>> bit_length_retriever,
                                                                        size_t write_delta,
                                                                        ID){
                
                return std::make_shared<RShiftOperator<T, T1, T2, T3, ID>>(true_estimator, read_iter_gen, replace_writer_iter_gen, bit_length_retriever, write_delta);

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class ID>
            std::shared_ptr<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6, T7, ID>> init_karatsuba(std::shared_ptr<operation_utility::VectorViewSplittable<T>> splitter,
                                                                                                                        std::shared_ptr<Operatable<T1>> plus_ops,
                                                                                                                        std::shared_ptr<Operatable<T2>> minus_ops,
                                                                                                                        std::shared_ptr<ShiftOperatable<T3>> lshifter,
                                                                                                                        std::shared_ptr<Operatable<T4>> multiplier,
                                                                                                                        std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T5>> temp_gen,
                                                                                                                        std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T6>> temp_gen_allocator,
                                                                                                                        std::shared_ptr<operation_utility::Estimatable<T7>> estimator,
                                                                                                                        ID){
                
                return std::make_shared<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6, T7, ID>>(splitter, plus_ops, minus_ops, lshifter, multiplier, temp_gen, temp_gen_allocator, estimator);

            }

            template <class T, class ID>
            std::shared_ptr<CustomPairWiseOperator<T, ID>> init_custom_pairwise(std::shared_ptr<operation_utility::Estimatable<T>> estimator,
                                                                                ID){

                return std::make_shared<CustomPairWiseOperator<T, ID>>(estimator); 

            }
            
            template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class ID>
            std::shared_ptr<RecursiveDivider<T1, T2, T3, T4, T5, T6, T7, T8, T9, ID>> init_recursive_div(std::shared_ptr<Operatable<T1>> minus_ops,
                                                                                                    std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever,
                                                                                                    std::shared_ptr<operation_utility::Estimatable<T3>> estimator,
                                                                                                    std::shared_ptr<operation_utility::ZeroDetectable<T4>> zero_detector,
                                                                                                    std::shared_ptr<comparer::Comparable<T5>> comparer,
                                                                                                    std::shared_ptr<ShiftOperatable<T6>> lshifter,
                                                                                                    std::shared_ptr<caster::BoolVectorOpCastable<T7>> full_op_caster,
                                                                                                    std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T8>> temp_gen,
                                                                                                    std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T9>> temp_gen_allocator,
                                                                                                    ID){
                
                return std::make_shared<RecursiveDivider<T1, T2, T3, T4, T5, T6, T7, T8, T9, ID>>(minus_ops, bit_length_retriever, estimator, zero_detector, comparer, lshifter, full_op_caster, temp_gen, temp_gen_allocator);

            }

        public:

            template <class ID>
            auto get_plus(ID id_){
                
                auto estimator = operation_utility::IDOperationUtilityGenerator().get_plus_estimator_v2(id_);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_plus_ops(casted_estimator, id_);

            }

            template <class ID>
            auto get_minus(ID id_){

                auto estimator = operation_utility::IDOperationUtilityGenerator().get_minus_estimator(id_);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_minus_ops(casted_estimator, id_);

            }

            template <class ID>
            auto get_lshift(ID id_){

                auto estimator = operation_utility::IDOperationUtilityGenerator().get_true_lshift_estimator(DoubleID<ID, 0>());
                auto read_iter = operation_utility::IDOperationUtilityGenerator().get_bit_iter_gen(DoubleID<ID, 1>());
                auto write_iter = operation_utility::IDOperationUtilityGenerator().get_bit_iter_orwriter_gen(DoubleID<ID, 2>());
                
                auto casted_estimator = estimator->to_shift_estimatable_sp(estimator);
                auto casted_read_iter = read_iter->to_bit_iterator_generatable_sp(read_iter);
                auto casted_write_iter = write_iter->to_bit_iter_writer_generatable_sp(write_iter);

                return this->init_lshift(casted_estimator, casted_read_iter, casted_write_iter, BIT_LENGTH_PER_SLOT, id_);

            }

            template <class ID>
            auto get_rshift(ID id_){

                auto estimator = operation_utility::IDOperationUtilityGenerator().get_true_rshift_estimator(DoubleID<ID, 0>());
                auto read_iter = operation_utility::IDOperationUtilityGenerator().get_bit_iter_gen(DoubleID<ID, 1>());
                auto write_iter = operation_utility::IDOperationUtilityGenerator().get_bit_iter_replace_writer_gen(DoubleID<ID, 2>());
                auto bit_length_retriever = operation_utility::IDOperationUtilityGenerator().get_legal_bit_length_retriever(DoubleID<ID, 3>()); 

                auto casted_estimator = estimator->to_shift_estimatable_sp(estimator);
                auto casted_read_iter = read_iter->to_bit_iterator_generatable_sp(read_iter);
                auto casted_write_iter = write_iter->to_bit_iter_writer_generatable_sp(write_iter);
                auto casted_length_retriever = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_r_shift(casted_estimator, casted_read_iter, casted_write_iter, casted_length_retriever, BIT_LENGTH_PER_SLOT, id_);

            }

            template <class ID>
            auto get_mul_uint128(ID id_){

                auto estimator = operation_utility::IDOperationUtilityGenerator().get_multiply_estimator(id_);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_uint128_mul(casted_estimator, id_);

            }

            template <class T, class ID>
            auto get_mul_std(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){

                auto splitter = operation_utility::IDOperationUtilityGenerator().get_zero_default_splitter(DoubleID<ID, 0>());
                auto plus_ops = this->get_plus(DoubleID<ID, 1>());
                auto minus_ops = this->get_minus(DoubleID<ID, 2>());
                auto lshifter = this->get_lshift(DoubleID<ID, 3>());
                auto mul = this->get_mul_uint128(DoubleID<ID, 4>());
                auto temp_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 5>());
                auto estimator = operation_utility::IDOperationUtilityGenerator().get_relax_mul_estimator(DoubleID<ID, 6>());

                auto casted_splitter = splitter->to_view_splittable_sp(splitter);
                auto casted_plus_ops = plus_ops->to_operatable_sp(plus_ops);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops);
                auto casted_lshifter = lshifter->to_shift_operatable_sp(lshifter);
                auto casted_mul = mul->to_operatable_sp(mul);
                auto casted_temp_gen = temp_gen->to_reallocatable_vector_generatable_sp(temp_gen);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_karatsuba(casted_splitter, casted_plus_ops, casted_minus_ops, casted_lshifter, casted_mul, casted_temp_gen, allocator, casted_estimator, id_);

            }

            template <class ID>
            auto get_pairwise_ops(ID id_){

                auto estimator = operation_utility::IDOperationUtilityGenerator().get_pairwise_estimator(id_);
                auto casted_est = estimator->to_estimatable_sp(estimator);

                return this->init_custom_pairwise(casted_est, id_); 

            }

            template <class T, class ID>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){
                
                auto minus_ops = this->get_minus(DoubleID<ID, 0>());
                auto bit_length_retriever = operation_utility::IDOperationUtilityGenerator().get_legal_bit_length_retriever(DoubleID<ID, 1>());
                auto estimator = operation_utility::IDOperationUtilityGenerator().get_divide_estimator(DoubleID<ID, 2>());
                auto zero_detector = operation_utility::IDOperationUtilityGenerator().get_zero_detector(DoubleID<ID, 3>());
                auto comparer = comparer::StandardComparerGenerator().get_backward_comparer(DoubleID<ID, 4>());
                auto lshifter = this->get_lshift(DoubleID<ID, 5>());
                auto full_op_caster = caster::IDBoolVectorCasterGenerator().get_full_opv_caster(DoubleID<ID, 6>());
                auto temp_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 7>());

                auto casted_minus = minus_ops->to_operatable_sp(minus_ops);
                auto casted_blr = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                auto casted_est = estimator->to_estimatable_sp(estimator);
                auto casted_zd = zero_detector->to_zero_detectable_sp(zero_detector);
                auto casted_cmp = comparer->to_comparable_sp(comparer);
                auto casted_lshift = lshifter->to_shift_operatable_sp(lshifter);
                auto casted_foc = full_op_caster->to_boolvector_op_castable_sp(full_op_caster);
                auto casted_temp_gen = temp_gen->to_reallocatable_vector_generatable_sp(temp_gen);

                return this->init_recursive_div(casted_minus, casted_blr, casted_est, casted_zd, casted_cmp, casted_lshift, casted_foc, casted_temp_gen, allocator, id_);

            }

    };

}  

namespace bignum::vector::mutable_extensible_operation{
    
    template <class ID>
    class AssignOperator: public Operatable<AssignOperator<ID>>{

        public:

            template <class T1, class T2>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                if (lhs.length() != rhs.length()){
                    
                    lhs.resize_no_copy(rhs.length());

                }
                                
                for (size_t i = 0; i < rhs.length(); ++i){
                    
                    lhs.set(i, rhs.get(i));

                }
                
            }


    };

    template <class T, class T1, class T2, class ID>
    class ImmutableOperator: public Operatable<ImmutableOperator<T, T1, T2, ID>>,
                             private AssignOperator<ID>, private T1, private T2{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;

        public:
            
            ImmutableOperator() {};

            ImmutableOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                              std::shared_ptr<immutable_operation::Operatable<T1>> immutable_operator,
                              std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T2>> vec_gen): T1(static_cast<T1&>(*immutable_operator)), T2(static_cast<T2&>(*vec_gen)){
                
                this->allocator = allocator;

            }

            template<class T3, class T4>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T3>& lhs, memory::sizet_linear::VectorReadable<T4>& rhs){
                
                this->allocator->enter_scope();

                auto rs = T2::get();
                T1::ops(lhs, rhs, rs);
                AssignOperator<ID>::ops(lhs, *rs.to_vector_readable());

                this->allocator->exit_scope();

            }

            using Operatable<ImmutableOperator<T, T1, T2, ID>>::to_operatable_sp;

    };

    template <class T, class T1, class T2, class T3, class T4, class ID>
    class Operator: public Operatable<Operator<T, T1, T2, T3, T4, ID>>,
                    private AssignOperator<ID>, private T1, private T2, 
                    private T3, private T4{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;

        public:

            Operator() {};

            Operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                     std::shared_ptr<immutable_operation::Operatable<T1>> im_operator,
                     std::shared_ptr<mutable_operation::Operatable<T2>> m_operator,
                     std::shared_ptr<operation_utility::Judgable<T3>> judge,
                     std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T4>> vec_gen): T1(static_cast<T1&>(*im_operator)), T2(static_cast<T2&>(*m_operator)), 
                                                                                                         T3(static_cast<T3&>(*judge)), T4(static_cast<T4&>(*vec_gen)){
                
                this->allocator = allocator;

            }

            template <class T5, class T6>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T5>& lhs, memory::sizet_linear::VectorReadable<T6>& rhs){

                if (T3::fit(lhs, rhs)){

                    T2::ops(lhs, rhs);
                    
                    return;

                } 

                this->allocator->enter_scope();

                auto rs = T4::get();
                T1::ops(lhs, rhs, rs);
                AssignOperator<ID>::ops(lhs, *rs.to_vector_readable());

                this->allocator->exit_scope();

            }

            using Operatable<Operator<T, T1, T2, T3, T4, ID>>::to_operatable_sp;

    };

    template <class T, class T1, class T2, class T3, class T4, class ID>
    class ShiftOperator: public ShiftOperatable<ShiftOperator<T, T1, T2, T3, T4, ID>>,
                         private AssignOperator<ID>, private T1, private T2,
                         private T3, private T4{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
        
        public:

            ShiftOperator() {};

            ShiftOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                          std::shared_ptr<operation::ShiftOperatable<T1>> im_operator,
                          std::shared_ptr<mutable_operation::ShiftOperatable<T2>> m_operator,
                          std::shared_ptr<operation_utility::ShiftJudgable<T3>> judge,
                          std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T4>> vec_gen): T1(static_cast<T1&>(*im_operator)), T2(static_cast<T2&>(*m_operator)),
                                                                                                              T3(static_cast<T3&>(*judge)), T4(static_cast<T4&>(*vec_gen)){
                
                this->allocator = allocator;

            }

            template <class T5>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T5>& lhs, size_t rhs){

                if (T3::fit(lhs, rhs)){

                    T2::ops(lhs, rhs);
                    return;

                } 

                this->allocator->enter_scope();

                auto rs = T4::get();
                T1::ops(lhs, rhs, rs);
                AssignOperator<ID>::ops(lhs, *rs.to_vector_readable());

                this->allocator->exit_scope();

            }

            using ShiftOperatable<ShiftOperator<T, T1, T2, T3, T4, ID>>::to_shift_operatable_sp;

    };

    template <class ID, unsigned>
    class DoubleID{};

    class IDArithmeticOperatableGenerator{

        private:

            template <class T, class T1, class T2, class T3, class T4, class ID>
            auto init_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                               std::shared_ptr<immutable_operation::Operatable<T1>> im_operator,
                               std::shared_ptr<mutable_operation::Operatable<T2>> m_operator,
                               std::shared_ptr<operation_utility::Judgable<T3>> judge,
                               std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T4>> vec_gen,
                               ID){
                
                return std::make_shared<Operator<T, T1, T2, T3, T4, ID>>(allocator, im_operator, m_operator, judge, vec_gen); 

            }  

            template <class T, class T1, class T2, class T3, class T4, class ID>
            auto init_shift_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                     std::shared_ptr<operation::ShiftOperatable<T1>> im_operator,
                                     std::shared_ptr<mutable_operation::ShiftOperatable<T2>> m_operator,
                                     std::shared_ptr<operation_utility::ShiftJudgable<T3>> judge,
                                     std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T4>> vec_gen,
                                     ID){
                
                return std::make_shared<ShiftOperator<T, T1, T2, T3, T4, ID>>(allocator, im_operator, m_operator, judge, vec_gen);

            }

        public:

            template <class ID>
            auto get_assign(ID){ // -> shared_ptr<Operatable>

                return std::make_shared<AssignOperator<ID>>();

            }

            template <class T, class ID>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable>
                 
                // assert(allocator != nullptr);

                auto immutable_plus_ops = immutable_operation::IDArithmeticGenerator().get_plus(DoubleID<ID, 0>());
                auto mutable_plus_ops = mutable_operation::IDArithmeticOperatorGenerator().get_plus(DoubleID<ID, 1>());
                auto judge = operation_utility::IDOperationUtilityGenerator().get_lightning_plus_judge(DoubleID<ID, 2>());
                auto vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 3>()); 

                auto casted_immutable_plus_ops = immutable_plus_ops->to_operatable_sp(immutable_plus_ops);
                auto casted_mutable_plus_ops = mutable_plus_ops->to_operatable_sp(mutable_plus_ops);
                auto casted_judge = judge->to_judgable_sp(judge);
                auto casted_vec_gen = vec_gen->to_reallocatable_vector_generatable_sp(vec_gen);

                return this->init_operator(allocator, casted_immutable_plus_ops, casted_mutable_plus_ops, casted_judge, casted_vec_gen, id_);

            }

            template <class T, class ID>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto immutable_minus_ops = immutable_operation::IDArithmeticGenerator().get_minus(DoubleID<ID, 0>());
                auto mutable_minus_ops = mutable_operation::IDArithmeticOperatorGenerator().get_minus(DoubleID<ID, 1>());
                auto judge = operation_utility::IDOperationUtilityGenerator().get_minus_judge(DoubleID<ID, 2>());
                auto vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 3>()); 

                auto casted_immutable_minus_ops = immutable_minus_ops->to_operatable_sp(immutable_minus_ops);
                auto casted_mutable_minus_ops = mutable_minus_ops->to_operatable_sp(mutable_minus_ops);
                auto casted_judge = judge->to_judgable_sp(judge); 
                auto casted_vec_gen = vec_gen->to_reallocatable_vector_generatable_sp(vec_gen);

                return this->init_operator(allocator, casted_immutable_minus_ops, casted_mutable_minus_ops, casted_judge, casted_vec_gen, id_);

            }


    };

}

namespace bignum::integer::usgn{ 

    class StandardBigNumView: public BigNumable<StandardBigNumView>, public memory::sizet_linear::SmallStackVectorView{

        public:

            StandardBigNumView() {};

            template <class T>
            StandardBigNumView(T&& obj): memory::sizet_linear::SmallStackVectorView(std::forward<T>(obj)){}

            using memory::sizet_linear::SmallStackVectorView::get_data;
            using memory::sizet_linear::SmallStackVectorView::length;
            using memory::sizet_linear::SmallStackVectorView::get;
            using memory::sizet_linear::SmallStackVectorView::sizeof_slot;
            using memory::sizet_linear::SmallStackVectorView::to_vector_readable;

    };

}

namespace bignum::integer::usgn::backward_caster{
    
    template <class T, class ID>
    class SizeTLinearVectorViewCaster: public SizeTLinearVectorCastable<SizeTLinearVectorViewCaster<T, ID>>,
                                       private T{

        public:

            SizeTLinearVectorViewCaster(){}

            SizeTLinearVectorViewCaster(std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen): T(static_cast<T&>(*gen)){}

            template <class T1> 
            auto cast(T1&& data){ // (VectorReadable&) -> BigNumable&
                
                return this->generate(std::forward<T1>(data));

            }

        private:

            auto generate(StandardBigNumView& data){

                return StandardBigNumView(data);

            }
            
            auto generate(StandardBigNumView&& data){

                return StandardBigNumView(std::move(data));
                
            }

            template <class T1>
            auto generate(T1&& data){
                
                auto casted = T::from_vec_readable(std::forward<T1>(data));

                return StandardBigNumView(casted);

            }

    };

    template <class T, class T1, class ID>
    class UINTCaster: public UINTCastable<UINTCaster<T, T1, ID>>,
                      private T, private T1{

        private:

            size_t MAX_VAL = std::numeric_limits<size_t>::max(); 

        public:
            
            using UINTCastable<UINTCaster<T, T1, ID>>::to_uint_castable_sp;

            UINTCaster() {};

            UINTCaster(std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster,
                       std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster): T(static_cast<T&>(*vector_caster)),
                                                                                      T1(static_cast<T1&>(*bignum_caster)){
            
            }

            auto cast(size_t data){ // -> BigNumable&

                auto vec_casted = T::cast(&data, 1);
                
                return T1::cast(vec_casted);

            }


    };

    static const size_t PRECOMPUTED_THRESHOLD = 1024;

    template <class T>
    class PrecomputedUINTCaster{

        private:

            T * data; // BigNumable&[]
            size_t n;
        
        public:

            template <class T1>
            PrecomputedUINTCaster(std::shared_ptr<UINTCastable<T1>> caster, size_t n){
                
                this->data = this->prep(caster, n);
                this->n = n;

            }

            ~PrecomputedUINTCaster(){
                
                std::free(this->data);

            }

            T& cast(size_t num){

                assert(num < n);

                return this->data[num];

            }

        private:

            template <class T1>
            T * prep(std::shared_ptr<UINTCastable<T1>> caster, size_t n){
                
                T * ptr = (T*) std::malloc(sizeof(T) * n);

                for (size_t i = 0; i < n; ++i){
                    
                    ptr[i] = caster->cast(i);

                }

                return ptr;

            }

    };
        
    template <class T>
    class PrecomputedUINTCasterSingleton{

        private:

            static inline std::shared_ptr<PrecomputedUINTCaster<T>> caster = nullptr;

        public:

            auto get(){

                return this->caster;

            }

            template <class T1>
            void initiate(std::shared_ptr<UINTCastable<T1>> caster){ // caster must return StandardBigNumWrapper<memory::sizet_linear::SmallAddableVector>

                assert(this->caster == nullptr);

                this->caster = std::make_shared<PrecomputedUINTCaster<T>>(caster, PRECOMPUTED_THRESHOLD);

            }

    };

    template <class T, class T1, class ID>
    class StandardUINTCaster: public UINTCastable<StandardUINTCaster<T, T1, ID>>,
                              private T{

        private:

            std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster;
            size_t precomputed_size;
            
        public:

            using UINTCastable<StandardUINTCaster<T, T1, ID>>::to_uint_castable_sp;

            StandardUINTCaster() {};

            StandardUINTCaster(std::shared_ptr<UINTCastable<T>> runtime_caster,
                               std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster,
                               size_t precomputed_size): T(static_cast<T&>(*runtime_caster)){
                
                this->precomputed_caster = precomputed_caster;
                this->precomputed_size = precomputed_size;

            } 

            auto cast(size_t num){

                if (num < this->precomputed_size){

                    return this->precomputed_caster->cast(num);

                }

                return T::cast(num);

            }

    };

    class GenericID{};

    class StandardGenerator{ // - Type Guaranteed
 
        private:
            
            template <class T>
            auto init_sizet_linear_vector_view_caster(std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen){

                return std::make_shared<SizeTLinearVectorViewCaster<T, GenericID>>(gen);

            }   

            
            template <class T, class T1>
            auto init_slow_uint_caster(std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster,
                                       std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster){
                
                return std::make_shared<UINTCaster<T, T1, GenericID>>(vector_caster, bignum_caster); 

            } 

            template <class T, class T1>
            auto init_std_uint_caster(std::shared_ptr<UINTCastable<T>> runtime_caster,
                                      std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster,
                                      size_t precomputed_size){
                
                return std::make_shared<StandardUINTCaster<T, T1, GenericID>>(runtime_caster, precomputed_caster, precomputed_size); 

            } 


            template <class T>
            auto get_precomputed_uint_caster(std::shared_ptr<UINTCastable<T>> uint_caster){

                auto op = uint_caster->cast(0);                

                if (PrecomputedUINTCasterSingleton<decltype(op)>().get() == nullptr){
                    
                    PrecomputedUINTCasterSingleton<decltype(op)>().initiate(uint_caster);

                }

                return PrecomputedUINTCasterSingleton<decltype(op)>().get();

            }

        public:

            auto get_sizet_linear_caster(){ // -> shared_ptr<SizeTLinearVectorCastable<>>

                auto vec_cast = memory::sizet_linear::StandardGenerator().get_dangling_stack_read_vec_gen();
                auto casted_vec_cast = vec_cast->to_stack_vector_readable_generatable_sp(vec_cast);

                return this->init_sizet_linear_vector_view_caster(casted_vec_cast);

            };

            auto get_slow_uint_caster(){ // -> shared_ptr<UINTCastable<>>

                auto addable_vec_gen = memory::sizet_linear::StandardGenerator().get_stack_allocated_addable_vector_gen();
                auto casted = addable_vec_gen->to_addable_vector_generatable(addable_vec_gen); 

                auto bignum_caster = this->get_sizet_linear_caster();
                auto vector_caster = vector::caster::StandardCasterGenerator().get_sizet_linear_no_padding_caster(casted); 

                auto casted_bignum_caster = bignum_caster->to_sizet_linear_vector_castable_sp(bignum_caster);
                auto casted_vector_caster = vector_caster->to_sizet_linear_castable_sp(vector_caster);

                return this->init_slow_uint_caster(casted_vector_caster, casted_bignum_caster);
                
            }

            auto get_uint_caster(){ // -> shared_ptr<UINTCastable<>>
                
                auto slow_caster = this->get_slow_uint_caster();
                auto casted_slow_caster = slow_caster->to_uint_castable_sp(slow_caster);
                auto precomputed_caster = this->get_precomputed_uint_caster(casted_slow_caster); 

                return this->init_std_uint_caster(casted_slow_caster, precomputed_caster, PRECOMPUTED_THRESHOLD);

            }
            
    };

    class IDGenerator{ // - Type Guaranteed
 
        private:
            
            template <class T, class ID>
            auto init_sizet_linear_vector_view_caster(std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen,
                                                      ID){

                return std::make_shared<SizeTLinearVectorViewCaster<T, ID>>(gen);

            }   

            
            template <class T, class T1, class ID>
            auto init_slow_uint_caster(std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster,
                                       std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster,
                                       ID){
                
                return std::make_shared<UINTCaster<T, T1, ID>>(vector_caster, bignum_caster); 

            } 

            template <class T, class T1, class ID>
            auto init_std_uint_caster(std::shared_ptr<UINTCastable<T>> runtime_caster,
                                      std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster,
                                      size_t precomputed_size,
                                      ID){
                
                return std::make_shared<StandardUINTCaster<T, T1, ID>>(runtime_caster, precomputed_caster, precomputed_size); 

            } 


            template <class T>
            auto get_precomputed_uint_caster(std::shared_ptr<UINTCastable<T>> uint_caster){

                auto op = uint_caster->cast(0);                

                if (PrecomputedUINTCasterSingleton<decltype(op)>().get() == nullptr){
                    
                    PrecomputedUINTCasterSingleton<decltype(op)>().initiate(uint_caster);

                }

                return PrecomputedUINTCasterSingleton<decltype(op)>().get();

            }

        public:

            template <class ID>
            auto get_sizet_linear_caster(ID id_){ // -> shared_ptr<SizeTLinearVectorCastable<>>

                auto vec_cast = memory::sizet_linear::IDGenerator().get_dangling_stack_read_vec_gen(id_);
                auto casted_vec_cast = vec_cast->to_stack_vector_readable_generatable_sp(vec_cast);

                return this->init_sizet_linear_vector_view_caster(casted_vec_cast, id_);

            };

            template <class ID>
            auto get_slow_uint_caster(ID id_){ // -> shared_ptr<UINTCastable<>>

                auto addable_vec_gen = memory::sizet_linear::IDGenerator().get_stack_allocated_addable_vector_gen(id_);
                auto casted = addable_vec_gen->to_addable_vector_generatable(addable_vec_gen); 

                auto bignum_caster = this->get_sizet_linear_caster(id_);
                auto vector_caster = vector::caster::IDCasterGenerator().get_sizet_linear_no_padding_caster(casted, id_); 

                auto casted_bignum_caster = bignum_caster->to_sizet_linear_vector_castable_sp(bignum_caster);
                auto casted_vector_caster = vector_caster->to_sizet_linear_castable_sp(vector_caster);

                return this->init_slow_uint_caster(casted_vector_caster, casted_bignum_caster, id_);
                
            }

            template <class ID>
            auto get_uint_caster(ID id_){ // -> shared_ptr<UINTCastable<>>
                
                auto slow_caster = this->get_slow_uint_caster(id_);
                auto casted_slow_caster = slow_caster->to_uint_castable_sp(slow_caster);
                auto precomputed_caster = this->get_precomputed_uint_caster(casted_slow_caster); 

                return this->init_std_uint_caster(casted_slow_caster, precomputed_caster, PRECOMPUTED_THRESHOLD, id_);

            }
            
    };

};

namespace bignum::integer::usgn::caster{

    template <class ID>
    class RawUINTCaster{

        public:

            template <class T>
            size_t raw_to_size_t(BigNumable<T>& val){

                size_t rs = 0;

                for (intmax_t i = (intmax_t) val.length() - 1; i >= 0; --i){

                    rs = (rs << bignum::vector::BIT_LENGTH_PER_SLOT) | val.get(i);

                }

                return rs;

            }

    };

    template <class T, class ID>
    class OOBMaxGuardUINTCaster: private RawUINTCaster<ID>, public UINTCastable<OOBMaxGuardUINTCaster<T, ID>>{
        
        private:

            std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            
            const size_t BIT_MAX = sizeof(size_t) * 8;
            const size_t VAL_MAX = std::numeric_limits<size_t>::max();

        public:

            OOBMaxGuardUINTCaster(){}

            OOBMaxGuardUINTCaster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                this->bit_length_retriever = bit_length_retriever;

            }

            template <class T1>
            size_t to_size_t(BigNumable<T1>& val){

                if (this->bit_length_retriever->get_bit_length(val) > BIT_MAX){

                    return VAL_MAX;

                }

                return RawUINTCaster<ID>::raw_to_size_t(val);

            }

    };

    template <class T, class ID>
    class OOBErrorUINTCaster: private RawUINTCaster<ID>, public UINTCastable<OOBErrorUINTCaster<T, ID>>{

        private:
            
            std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            const size_t BIT_MAX = sizeof(size_t) * 8;

        public:
            
            OOBErrorUINTCaster(){}

            OOBErrorUINTCaster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                this->bit_length_retriever = bit_length_retriever;

            }

            template <class T1>
            size_t to_size_t(BigNumable<T1>& val){

                assert(this->bit_length_retriever->get_bit_length(val) <= BIT_MAX);

                return RawUINTCaster<ID>::raw_to_size_t(val);

            }

    };

    class GenericID{};

    class StandardGenerator{

        private:
            
            template <class T>
            auto init_oob_max_guard_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                return std::make_shared<OOBMaxGuardUINTCaster<T, GenericID>>(bit_length_retriever); 

            }

            template <class T>
            auto init_oob_error_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                return std::make_shared<OOBErrorUINTCaster<T, GenericID>>(bit_length_retriever); 

            }

        public:

            auto get_uint_oob_guard_caster(){

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_oob_max_guard_uint_caster(casted); 

            }

            auto get_uint_oob_error_caster(){

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_oob_error_uint_caster(casted);

            }

    };

    class IDGenerator{

        private:
            
            template <class T, class ID>
            auto init_oob_max_guard_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                                                ID){

                return std::make_shared<OOBMaxGuardUINTCaster<T, ID>>(bit_length_retriever); 

            }

            template <class T, class ID>
            auto init_oob_error_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                                            ID){

                return std::make_shared<OOBErrorUINTCaster<T, ID>>(bit_length_retriever); 

            }

        public:

            template <class ID>
            auto get_uint_oob_guard_caster(ID id_){

                auto bit_length_retriever = bignum::vector::operation_utility::IDOperationUtilityGenerator().get_bit_length_retriever(id_);
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_oob_max_guard_uint_caster(casted, id_); 

            }

            template <class ID>
            auto get_uint_oob_error_caster(ID id_){

                auto bit_length_retriever = bignum::vector::operation_utility::IDOperationUtilityGenerator().get_bit_length_retriever(id_);
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_oob_error_uint_caster(casted, id_);

            }

    };


};

namespace bignum::integer::usgn::operation_utility{
    
    template <class T, class ID>
    class BitChecker: public OOBCheckable<BitChecker<T, ID>>{

        private:

            std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            size_t max_bits;

        public:

            BitChecker() {};

            BitChecker(std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                       size_t max_bits){

                this->bit_length_retriever = bit_length_retriever;
                this->max_bits = max_bits;

            }


            template <class T1>
            bool is_oob(BigNumable<T1>& num){

                return this->bit_length_retriever->get_bit_length(num) > this->max_bits;

            }

            bool is_oob(size_t num){

                return algorithm::log2::log2_64(num) + 1 > this->max_bits; 

            }

            template <class T1>
            bool is_in_bound(BigNumable<T1>& num){

                return !this->is_oob(num);

            }

            bool is_in_bound(size_t num){

                return !this->is_oob(num);

            }

              
    }; 

    class GenericID{};

    class StandardGenerator{

        private:

            template <class T>
            auto init_bit_checker(std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                                  size_t max_bit_length){

                return std::make_shared<BitChecker<T, GenericID>>(bit_length_retriever, max_bit_length);

            }

        public:

            auto get_bitwise_modulo_check(){

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                size_t max_bit_length = sizeof(size_t) * 8 - 1;

                return this->init_bit_checker(casted, max_bit_length);

            }

            auto get_discretized_modulo_check(){
                
                const uint8_t MIN_READ = 5;

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                size_t max_bit_length = 0;

                if (vector::BIT_LENGTH_PER_SLOT > MIN_READ){

                    max_bit_length = vector::BIT_LENGTH_PER_SLOT - MIN_READ; 

                }

                return this->init_bit_checker(casted, max_bit_length);

            }

    };

    class IDGenerator{

        private:

            template <class T, class ID>
            auto init_bit_checker(std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                                  size_t max_bit_length,
                                  ID){

                return std::make_shared<BitChecker<T, ID>>(bit_length_retriever, max_bit_length);

            }

        public:

            template <class ID>
            auto get_bitwise_modulo_check(ID id_){

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                size_t max_bit_length = sizeof(size_t) * 8 - 1;

                return this->init_bit_checker(casted, max_bit_length, id_);

            }

            template <class ID>
            auto get_discretized_modulo_check(ID id_){
                
                const uint8_t MIN_READ = 5;

                auto bit_length_retriever = bignum::vector::operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto casted = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                size_t max_bit_length = 0;

                if (vector::BIT_LENGTH_PER_SLOT > MIN_READ){

                    max_bit_length = vector::BIT_LENGTH_PER_SLOT - MIN_READ; 

                }

                return this->init_bit_checker(casted, max_bit_length, id_);

            }

    };


};

namespace bignum::integer::usgn::immutable_operation{

    inline size_t bitwise_or(size_t lhs, size_t rhs){

        return lhs | rhs;

    }

    inline size_t bitwise_xor(size_t lhs, size_t rhs){
        
        return lhs ^ rhs;

    }

    inline size_t bitwise_and(size_t lhs, size_t rhs){

        return lhs & rhs;

    }

    template <class T, class T1, class ID>
    class Operator: public Operatable<Operator<T, T1, ID>>,
                    private T, private T1{

        public:

            using Operatable<Operator<T, T1, ID>>::to_operatable_sp;

            Operator() {};

            Operator(std::shared_ptr<vector::immutable_operation::Operatable<T>> im_operator,
                     std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster): T(static_cast<T&>(*im_operator)), 
                                                                                        T1(static_cast<T1&>(*bignum_caster)){}


            template <class T2, class T3, class T4>
            void ops(BigNumable<T2>& lhs, BigNumable<T3>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T4>& rs){ // -> BigNumable&
                
                T::ops(*lhs.to_vector_readable(), *rhs.to_vector_readable(), rs);
                        
            }

            template <class T2, class T3>
            void ops(BigNumable<T2>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ // ->BigNumable&
                
                auto casted = T1::cast(rhs);

                T::ops(*lhs.to_vector_readable(), *casted.to_vector_readable(), rs); 

            }


    };
    
    template <class T, class T1, class ID>
    class ShiftOperator: public Operatable<ShiftOperator<T, T1, ID>>,
                         private T, private T1{
        
        public:

            using Operatable<ShiftOperator<T, T1, ID>>::to_operatable_sp;

            ShiftOperator() {};

            ShiftOperator(std::shared_ptr<vector::immutable_operation::ShiftOperatable<T>> shift_operator,
                          std::shared_ptr<caster::UINTCastable<T1>> uint_oob_guard_caster): T(static_cast<T&>(*shift_operator)),
                                                                                            T1(static_cast<T1&>(*uint_oob_guard_caster)){}


            template <class T2, class T3>
            void ops(BigNumable<T2>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){ // -> BigNumable&
                
                T::ops(*lhs.to_vector_readable(), rhs, rs);
                
            }

            template <class T2, class T3, class T4>
            void ops(BigNumable<T2>& lhs, BigNumable<T3>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T4>& rs){ // BigNumable&
                
                size_t casted = T1::to_size_t(rhs);

                T::ops(*lhs.to_vector_readable(), casted, rs);

            }

    };
    
    template <class T, class T1, class ID>
    class PairWiseOperator: public Operatable<PairWiseOperator<T, T1, ID>>,
                            private T, private T1{

        private:

            vector::pairwise_ops pw_ops;

        public:
            
            using Operatable<PairWiseOperator<T, T1, ID>>::to_operatable_sp;

            PairWiseOperator() {};

            PairWiseOperator(std::shared_ptr<vector::immutable_operation::PairWiseOperatable<T>> custom_pairwise_operator,
                             std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                             vector::pairwise_ops pw_ops): T(static_cast<T&>(*custom_pairwise_operator)),
                                                           T1(static_cast<T1&>(*bignum_caster)){
                
                this->pw_ops = pw_ops;

            }

            template <class T2, class T3, class T4>
            void ops(BigNumable<T2>& lhs, BigNumable<T3>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T4>& rs){ 
                
                T::ops(*lhs.to_vector_readable(), *rhs.to_vector_readable(), rs, this->pw_ops);

            }

            template <class T2, class T3>
            void ops(BigNumable<T2>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T3>& rs){

                auto casted = T1::cast(rhs);

                T::ops(*lhs.to_vector_readable(), *casted.to_vector_readable(), rs);

            }


    };
    
    template <class T, class T1, class T2, class T3, class ID>
    class BitwiseSizeTModuloOperator: public Operatable<BitwiseSizeTModuloOperator<T, T1, T2, T3, ID>>,
                                      private T, private T1, private T2, private T3{

        
        public:

            using Operatable<BitwiseSizeTModuloOperator<T, T1, T2, T3, ID>>::to_operatable_sp;

            BitwiseSizeTModuloOperator() {};

            BitwiseSizeTModuloOperator(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> trueview_caster,
                                       std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                                       std::shared_ptr<caster::UINTCastable<T2>> uint_caster,
                                       std::shared_ptr<vector::mutable_extensible_operation::Operatable<T3>> assign_ops): T(static_cast<T&>(*trueview_caster)),
                                                                                                                          T1(static_cast<T1&>(*bignum_caster)),
                                                                                                                          T2(static_cast<T2&>(*uint_caster)),
                                                                                                                          T3(static_cast<T3&>(*assign_ops)){

            }

            template <class T4, class T5>
            void ops(BigNumable<T4>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T5>& op){ 

                assert(rhs != 0);
                assert(algorithm::log2::log2_64(rhs) + 1 < sizeof(size_t) * 8);

                auto casted = T::cast(lhs);
                size_t rs = 0;

                for (intmax_t i = (intmax_t) casted.length() - 1; i >= 0; --i){

                    rs <<= 1;

                    if (casted.get(i)){

                        rs += 1;

                    }

                    rs %= rhs;

                } 

                auto casted_rs = T1::cast(rs);
                T3::ops(*op.to_reallocatable_operatable_vector(), *casted_rs.to_vector_readable());

            }
            
            template <class T4, class T5, class T6>
            void ops(BigNumable<T4>& lhs, BigNumable<T5>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T6>& rs){ 

                size_t casted_rs = T2::to_size_t(rhs);

                this->ops(lhs, casted_rs, rs); 

            }

    };

    template <class T, class T1, class T2, class T3, class ID>
    class DiscretizedSizeTModuloOperator: public Operatable<DiscretizedSizeTModuloOperator<T, T1, T2, T3, ID>>,
                                          private T, private T1, private T2, private T3{


        public:

            using Operatable<DiscretizedSizeTModuloOperator<T, T1, T2, T3, ID>>::to_operatable_sp;

            DiscretizedSizeTModuloOperator() {};

            DiscretizedSizeTModuloOperator(std::shared_ptr<vector::operation_utility::BitIteratorGeneratable<T>> reverse_read_iter_gen,
                                           std::shared_ptr<backward_caster::UINTCastable<T1>> result_caster,
                                           std::shared_ptr<caster::UINTCastable<T2>> uint_caster,
                                           std::shared_ptr<vector::mutable_extensible_operation::Operatable<T3>> assign_ops): T(static_cast<T&>(*reverse_read_iter_gen)),
                                                                                                                              T1(static_cast<T1&>(*result_caster)),
                                                                                                                              T2(static_cast<T2&>(*uint_caster)),
                                                                                                                              T3(static_cast<T3&>(*assign_ops)){
            
            }

            template <class T4, class T5>
            void ops(BigNumable<T4>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T5>& op){
                
                assert(rhs != 0);
                uint8_t mod_bit_length = algorithm::log2::log2_64(rhs) + 1;
                assert(mod_bit_length < vector::BIT_LENGTH_PER_SLOT);

                uint8_t read_length = vector::BIT_LENGTH_PER_SLOT - mod_bit_length;
                size_t cur_read = 0;
                size_t rs = 0;

                auto read_iter = T::get(lhs, read_length);

                while (read_iter.next(cur_read)){

                    rs <<= read_length;
                    rs |= cur_read;
                    rs %= rhs; 

                }

                auto casted_rs = T1::cast(rs);
                
                T3::ops(*op.to_reallocatable_operatable_vector(), *casted_rs.to_vector_readable());

            }

            template <class T4, class T5, class T6>
            void ops(BigNumable<T4>& lhs, BigNumable<T5>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T6>& rs){

                size_t casted = T2::to_size_t(rhs); 
                
                this->ops(lhs, casted, rs);

            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class ID>
    class BruteForceModuloOperator: public Operatable<BruteForceModuloOperator<T, T1, T2, T3, T4, T5, T6, ID>>,
                                    private T, private T1, private T2, private T3, private T4, private T6{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T5>> temp_gen;

        public:

            using Operatable<BruteForceModuloOperator<T, T1, T2, T3, T4, T5, T6, ID>>::to_operatable_sp;

            BruteForceModuloOperator() {};

            BruteForceModuloOperator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> assign_ops,
                                     std::shared_ptr<backward_caster::UINTCastable<T1>> bvec_caster,
                                     std::shared_ptr<vector::immutable_operation::Operatable<T2>> mul_ops,
                                     std::shared_ptr<vector::immutable_operation::Operatable<T3>> divide_ops,
                                     std::shared_ptr<vector::immutable_operation::Operatable<T4>> minus_ops,
                                     std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T5>> temp_gen,
                                     std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T6>> temp_vec_gen): T(static_cast<T&>(*assign_ops)),
                                                                                                                              T1(static_cast<T1&>(*bvec_caster)),
                                                                                                                              T2(static_cast<T2&>(*mul_ops)),
                                                                                                                              T3(static_cast<T3&>(*divide_ops)),
                                                                                                                              T4(static_cast<T4&>(*minus_ops)),
                                                                                                                              T6(static_cast<T6&>(*temp_vec_gen)){
                
                this->temp_gen = temp_gen;

            }

            template <class T7, class T8, class T9>
            void ops(BigNumable<T7>& lhs, BigNumable<T8>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T9>& rs){
                
                this->temp_gen->enter_scope();

                auto div_rs = T6::get();
                auto mul_rs = T6::get();
                auto mod_rs = T6::get();

                T3::ops(*lhs.to_vector_readable(), *rhs.to_vector_readable(), div_rs);
                T2::ops(*div_rs.to_vector_readable(), *rhs.to_vector_readable(), mul_rs);
                T4::ops(*lhs.to_vector_readable(), *mul_rs.to_vector_readable(), mod_rs);
                T::ops(*rs.to_reallocatable_operatable_vector(), *mod_rs.to_vector_readable());

                this->temp_gen->exit_scope();

            }

            template <class T7, class T8>
            void ops(BigNumable<T7>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T8>& rs){

                auto casted = T1::cast(rhs);

                return this->ops(lhs, casted, rs);

            }


    }; 

    template <class T, class T1, class T2, class T3, class T4, class ID>
    class ModuloOperator: public Operatable<ModuloOperator<T, T1, T2, T3, T4, ID>>,
                          private T, private T1, private T2, private T3, private T4{

        public:

            using Operatable<ModuloOperator<T, T1, T2, T3, T4, ID>>::to_operatable_sp;

            ModuloOperator() {};

            ModuloOperator(std::shared_ptr<Operatable<T>> bitwise_modulo_ops, 
                           std::shared_ptr<Operatable<T1>> standard_modulo_ops, 
                           std::shared_ptr<operation_utility::OOBCheckable<T2>> bitwise_modulo_checker,
                           std::shared_ptr<Operatable<T3>> discretized_modulo_ops,
                           std::shared_ptr<operation_utility::OOBCheckable<T4>> discretized_modulo_checker): T(static_cast<T&>(*bitwise_modulo_ops)),
                                                                                                             T1(static_cast<T1&>(*standard_modulo_ops)),
                                                                                                             T2(static_cast<T2&>(*bitwise_modulo_checker)),
                                                                                                             T3(static_cast<T3&>(*discretized_modulo_ops)),
                                                                                                             T4(static_cast<T4&>(*discretized_modulo_checker)){

            }

            template <class T5, class T6, class T7>
            void ops(BigNumable<T5>& lhs, BigNumable<T6>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T7>& rs){ // -> BigNumable&

                if (T4::is_in_bound(rhs)){

                    T3::ops(lhs, rhs, rs);
                    return;

                }

                if (T2::is_in_bound(rhs)){

                    T::ops(lhs, rhs, rs);
                    return;

                }

                T1::ops(lhs, rhs, rs);

            }

            template <class T5, class T6>
            void ops(BigNumable<T5>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T6>& rs){ // -> BigNumable&
                
                if (T4::is_in_bound(rhs)){

                    T3::ops(lhs, rhs, rs);
                    return;

                }

                if (T2::is_in_bound(rhs)){

                    T::ops(lhs, rhs, rs);
                    return;

                }

                T1::ops(lhs, rhs, rs);

            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class ID>
    class ArithmeticOperator: public UsgnOperatable<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, ID>>,
                              private T, private T1, private T2, private T3, private T4, private T5, private T6, 
                              private T7, private T8, private T9{

        public:

            ArithmeticOperator() {};

            ArithmeticOperator(std::shared_ptr<Operatable<T>> plus_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                               std::shared_ptr<Operatable<T2>> mul_ops, std::shared_ptr<Operatable<T3>> divide_ops,
                               std::shared_ptr<Operatable<T4>> or_operator, std::shared_ptr<Operatable<T5>> and_operator,
                               std::shared_ptr<Operatable<T6>> xor_operator, std::shared_ptr<Operatable<T7>> lshift_ops,
                               std::shared_ptr<Operatable<T8>> rshift_ops, std::shared_ptr<Operatable<T9>> mod_ops): T(static_cast<T&>(*plus_ops)),
                                                                                                                     T1(static_cast<T1&>(*minus_ops)),
                                                                                                                     T2(static_cast<T2&>(*mul_ops)),
                                                                                                                     T3(static_cast<T3&>(*divide_ops)),
                                                                                                                     T4(static_cast<T4&>(*or_operator)),
                                                                                                                     T5(static_cast<T5&>(*and_operator)),
                                                                                                                     T6(static_cast<T6&>(*xor_operator)),
                                                                                                                     T7(static_cast<T7&>(*lshift_ops)),
                                                                                                                     T8(static_cast<T8&>(*rshift_ops)),
                                                                                                                     T9(static_cast<T9&>(*mod_ops)) {
            
            }

            template <class T10, class T11, class T12>
            void minus(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T1::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void minus(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T1::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void plus(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void plus(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void rshift(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T8::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void rshift(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T8::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void lshift(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T7::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void lshift(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T7::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void multiply(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T2::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void multiply(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T2::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void divide(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T3::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void divide(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T3::ops(lhs, rhs, rs); 

            }

            template <class T10, class T11, class T12>
            void and_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T5::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void and_ops(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T5::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void or_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T4::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void or_ops(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T4::ops(lhs, rhs, rs);

            }

            template <class T10, class T11, class T12>
            void xor_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){ 

                T6::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void xor_ops(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){ 

                T6::ops(lhs, rhs, rs); 

            }      

            template <class T10, class T11, class T12>
            void mod(BigNumable<T10>& lhs, BigNumable<T11>& rhs, memory::sizet_linear::ReallocatableOperatableVector<T12>& rs){

                T9::ops(lhs, rhs, rs);

            }   
            
            template <class T10, class T11>
            void mod(BigNumable<T10>& lhs, size_t rhs, memory::sizet_linear::ReallocatableOperatableVector<T11>& rs){

                T9::ops(lhs, rhs, rs);

            }  

    }; 

    template <class T, unsigned>
    class DoubleID{};

    class IDArithmeticOperatorGenerator{

        private:

            template <class T, class T1, class T2, class T3, class ID>
            auto init_discretized_modulo_operator(std::shared_ptr<vector::operation_utility::BitIteratorGeneratable<T>> reverse_read_iter_gen,
                                                  std::shared_ptr<backward_caster::UINTCastable<T1>> result_caster,
                                                  std::shared_ptr<caster::UINTCastable<T2>> uint_caster,
                                                  std::shared_ptr<vector::mutable_extensible_operation::Operatable<T3>> assign_ops,
                                                  ID){
                
                return std::make_shared<DiscretizedSizeTModuloOperator<T, T1, T2, T3, ID>>(reverse_read_iter_gen, result_caster, uint_caster, assign_ops);

            }

            template <class T, class T1, class ID>
            auto init_operator(std::shared_ptr<vector::immutable_operation::Operatable<T>> im_operator,
                               std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                               ID){
                
                return std::make_shared<Operator<T, T1, ID>>(im_operator, bignum_caster);

            } 

            template <class T, class T1, class ID>
            auto init_shift_operator(std::shared_ptr<vector::immutable_operation::ShiftOperatable<T>> shift_operator,
                                     std::shared_ptr<caster::UINTCastable<T1>> uint_oob_guard_caster,
                                     ID){
                
                return std::make_shared<ShiftOperator<T, T1, ID>>(shift_operator, uint_oob_guard_caster); 

            }

            template <class T, class T1, class ID>
            auto init_pairwise_ops(std::shared_ptr<vector::immutable_operation::PairWiseOperatable<T>> custom_pairwise_operator,
                                   std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                                   vector::pairwise_ops pw_ops,
                                   ID){
                
                return std::make_shared<PairWiseOperator<T, T1, ID>>(custom_pairwise_operator, bignum_caster, pw_ops); 

            }

            template <class T, class T1, class T2, class T3, class ID>
            auto init_bitwise_sizet_modulo_operator(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> trueview_caster,
                                                    std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                                                    std::shared_ptr<caster::UINTCastable<T2>> oob_error_uint_caster,
                                                    std::shared_ptr<vector::mutable_extensible_operation::Operatable<T3>> assign_ops,
                                                    ID){
                
                return std::make_shared<BitwiseSizeTModuloOperator<T, T1, T2, T3, ID>>(trueview_caster, bignum_caster, oob_error_uint_caster, assign_ops); 

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class ID>
            auto init_bruteforce_modulo_operator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> assign_ops,
                                                 std::shared_ptr<backward_caster::UINTCastable<T1>> bvec_caster,
                                                 std::shared_ptr<vector::immutable_operation::Operatable<T2>> mul_ops,
                                                 std::shared_ptr<vector::immutable_operation::Operatable<T3>> divide_ops,
                                                 std::shared_ptr<vector::immutable_operation::Operatable<T4>> minus_ops,
                                                 std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T5>> temp_gen,
                                                 std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T6>> temp_vec_gen,
                                                 ID){
                
                return std::make_shared<BruteForceModuloOperator<T, T1, T2, T3, T4, T5, T6, ID>>(assign_ops, bvec_caster, mul_ops, divide_ops, minus_ops, temp_gen, temp_vec_gen);

            }

            template <class T, class T1, class T2, class T3, class T4, class ID>
            auto init_standard_modulo_operator(std::shared_ptr<Operatable<T>> bitwise_modulo_ops, 
                                               std::shared_ptr<Operatable<T1>> bruteforce_modulo_ops, 
                                               std::shared_ptr<operation_utility::OOBCheckable<T2>> bitwise_modulo_checker,
                                               std::shared_ptr<Operatable<T3>> discretized_modulo_ops,
                                               std::shared_ptr<operation_utility::OOBCheckable<T4>> discretized_modulo_checker,
                                               ID){
                
                return std::make_shared<ModuloOperator<T, T1, T2, T3, T4, ID>>(bitwise_modulo_ops, bruteforce_modulo_ops, bitwise_modulo_checker, discretized_modulo_ops, discretized_modulo_checker); 

            } 

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class ID>
            auto init_arithmetic(std::shared_ptr<Operatable<T>> plus_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                                 std::shared_ptr<Operatable<T2>> mul_ops, std::shared_ptr<Operatable<T3>> divide_ops,
                                 std::shared_ptr<Operatable<T4>> or_operator, std::shared_ptr<Operatable<T5>> and_operator,
                                 std::shared_ptr<Operatable<T6>> xor_operator, std::shared_ptr<Operatable<T7>> lshift_ops,
                                 std::shared_ptr<Operatable<T8>> rshift_ops, std::shared_ptr<Operatable<T9>> mod_ops,
                                 ID){
                
                return std::make_shared<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, ID>>(plus_ops, minus_ops, mul_ops, divide_ops, or_operator, and_operator, xor_operator, lshift_ops, rshift_ops, mod_ops);
            }

        public:

            template <class ID>
            auto get_plus(ID id_){

                // assert(allocator != nullptr);

                auto ops = vector::immutable_operation::IDArithmeticGenerator().get_plus(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_caster, id_); 

            }
            
            template <class ID>
            auto get_minus(ID id_){

                // assert(allocator != nullptr);

                auto ops = vector::immutable_operation::IDArithmeticGenerator().get_minus(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_caster, id_); 

            }

            template <class T, class ID>
            auto get_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                         ID id_){

                // assert(allocator != nullptr);

                auto ops = vector::immutable_operation::IDArithmeticGenerator().get_mul_std(allocator, DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_caster, id_); 


            }

            template <class T, class ID>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                            ID id_){

                // assert(allocator != nullptr);

                auto ops = vector::immutable_operation::IDArithmeticGenerator().get_divide(allocator, DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_caster, id_); 

            }

            template <class T, class ID>
            auto get_bruteforce_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                    ID id_){

                // assert(allocator != nullptr);

                auto assign_ops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 0>());
                auto bvec_caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());
                auto mul_ops = vector::immutable_operation::IDArithmeticGenerator().get_mul_std(allocator, DoubleID<ID, 2>());
                auto divide_ops = vector::immutable_operation::IDArithmeticGenerator().get_divide(allocator, DoubleID<ID, 3>());
                auto minus_ops = vector::immutable_operation::IDArithmeticGenerator().get_minus(DoubleID<ID, 4>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 5>());

                auto casted_assign_ops = assign_ops->to_operatable_sp(assign_ops);
                auto casted_bvec_caster = bvec_caster->to_uint_castable_sp(bvec_caster);
                auto casted_mul_ops = mul_ops->to_operatable_sp(mul_ops);
                auto casted_divide_ops = divide_ops->to_operatable_sp(divide_ops);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops); 
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_bruteforce_modulo_operator(casted_assign_ops, casted_bvec_caster, casted_mul_ops, casted_divide_ops, casted_minus_ops, allocator, casted_temp_vec_gen, id_);

            }

            template <class ID>
            auto get_bitwise_mod(ID id_){
                
                auto trueview_caster = vector::caster::IDBoolVectorCasterGenerator().get_true_view_caster(DoubleID<ID, 0>());
                auto bignum_caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());
                auto oob_error_uint_caster = caster::IDGenerator().get_uint_oob_error_caster(DoubleID<ID, 2>());
                auto assign_ops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 3>());

                auto casted_trueview_caster = trueview_caster->to_boolvector_view_castable_sp(trueview_caster);
                auto casted_bignum_caster = bignum_caster->to_uint_castable_sp(bignum_caster);
                auto casted_oob_error_uint_caster = oob_error_uint_caster->to_uint_castable_sp(oob_error_uint_caster);
                auto casted_assign_ops = assign_ops->to_operatable_sp(assign_ops);

                return this->init_bitwise_sizet_modulo_operator(casted_trueview_caster, casted_bignum_caster, casted_oob_error_uint_caster, casted_assign_ops, id_);

            }
            
            template <class ID>
            auto get_discretized_mod(ID id_){

                auto bit_iter = vector::operation_utility::IDOperationUtilityGenerator().get_reverse_bit_iter_gen(DoubleID<ID, 0>());
                auto bignum_caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());
                auto oob_error_uint_caster = caster::IDGenerator().get_uint_oob_error_caster(DoubleID<ID, 2>()); 
                auto assign_ops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 3>());

                auto casted_bit_iter = bit_iter->to_bit_iterator_generatable_sp(bit_iter);
                auto casted_bignum_caster = bignum_caster->to_uint_castable_sp(bignum_caster);
                auto casted_oob_error_uint_caster = oob_error_uint_caster->to_uint_castable_sp(oob_error_uint_caster);
                auto casted_assign_ops = assign_ops->to_operatable_sp(assign_ops);

                return this->init_discretized_modulo_operator(casted_bit_iter, casted_bignum_caster, casted_oob_error_uint_caster, casted_assign_ops, id_);

            } 

            template <class T, class ID>
            auto get_standard_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                  ID id_){

                auto bitwise_modulo_checker = operation_utility::IDGenerator().get_bitwise_modulo_check(DoubleID<ID, 0>());
                auto bitwise_mod = this->get_bitwise_mod(DoubleID<ID, 1>());
                auto bruteforce_mod = this->get_bruteforce_mod(allocator, DoubleID<ID, 2>());
                auto discretized_mod = this->get_discretized_mod(DoubleID<ID, 3>());
                auto discretized_check = operation_utility::IDGenerator().get_discretized_modulo_check(DoubleID<ID, 4>()); 

                auto casted_bitwise_modulo_checker = bitwise_modulo_checker->to_oob_checkable_sp(bitwise_modulo_checker);
                auto casted_bitwise_mod = bitwise_mod->to_operatable_sp(bitwise_mod);
                auto casted_bruteforce_mod = bruteforce_mod->to_operatable_sp(bruteforce_mod);
                auto casted_discretized_mod = discretized_mod->to_operatable_sp(discretized_mod);
                auto casted_discretized_check = discretized_check->to_oob_checkable_sp(discretized_check); 

                return this->init_standard_modulo_operator(casted_bitwise_mod, casted_bruteforce_mod, casted_bitwise_modulo_checker, casted_discretized_mod, casted_discretized_check, id_);

            }

            template <class ID>
            auto get_lshift(ID id_){

                auto shift_ops = vector::immutable_operation::IDArithmeticGenerator().get_lshift(DoubleID<ID, 0>());
                auto caster = caster::IDGenerator().get_uint_oob_error_caster(DoubleID<ID, 1>());
                
                auto casted_shift_ops = shift_ops->to_shift_operatable_sp(shift_ops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_shift_operator(casted_shift_ops, casted_caster, id_); 

            }

            template <class ID>
            auto get_rshift(ID id_){

                auto shift_ops = vector::immutable_operation::IDArithmeticGenerator().get_rshift(DoubleID<ID, 0>());
                auto caster = caster::IDGenerator().get_uint_oob_guard_caster(DoubleID<ID, 1>());
                
                auto casted_shift_ops = shift_ops->to_shift_operatable_sp(shift_ops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_shift_operator(casted_shift_ops, casted_caster, id_); 

            }

            template <class ID>
            auto get_or(ID id_){
                
                auto pairwise_operator = vector::immutable_operation::IDArithmeticGenerator().get_pairwise_ops(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_ops(casted_pairwise_operator, casted_caster, bitwise_or, id_);

            }

            template <class ID>
            auto get_xor(ID id_){

                auto pairwise_operator = vector::immutable_operation::IDArithmeticGenerator().get_pairwise_ops(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_ops(casted_pairwise_operator, casted_caster, bitwise_xor, id_);

            }

            template <class ID>
            auto get_and(ID id_){

                auto pairwise_operator = vector::immutable_operation::IDArithmeticGenerator().get_pairwise_ops(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_ops(casted_pairwise_operator, casted_caster, bitwise_and, id_);

            }

            template <class T, class ID>
            auto get_arithmetic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                ID id_){
                
                auto plus_ops = this->get_plus(DoubleID<ID, 0>()); 
                auto minus_ops = this->get_minus(DoubleID<ID, 1>()); 
                auto mul_ops = this->get_mul(allocator, DoubleID<ID, 2>()); 
                auto div_ops = this->get_divide(allocator, DoubleID<ID, 3>()); 
                auto or_ops = this->get_or(DoubleID<ID, 4>()); 
                auto and_ops = this->get_and(DoubleID<ID, 5>()); 
                auto xor_ops = this->get_xor(DoubleID<ID, 6>()); 
                auto rshift_ops = this->get_rshift(DoubleID<ID, 7>()); 
                auto lshift_ops = this->get_lshift(DoubleID<ID, 8>()); 
                auto mod_ops = this->get_standard_mod(allocator, DoubleID<ID, 9>()); 

                auto casted_plus_ops = plus_ops->to_operatable_sp(plus_ops); 
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops); 
                auto casted_mul_ops = mul_ops->to_operatable_sp(mul_ops); 
                auto casted_div_ops = div_ops->to_operatable_sp(div_ops); 
                auto casted_or_ops = or_ops->to_operatable_sp(or_ops); 
                auto casted_and_ops = and_ops->to_operatable_sp(and_ops); 
                auto casted_xor_ops = xor_ops->to_operatable_sp(xor_ops); 
                auto casted_rshift_ops = rshift_ops->to_operatable_sp(rshift_ops); 
                auto casted_lshift_ops = lshift_ops->to_operatable_sp(lshift_ops); 
                auto casted_mod_ops = mod_ops->to_operatable_sp(mod_ops); 

                return this->init_arithmetic(casted_plus_ops, casted_minus_ops, casted_mul_ops, casted_div_ops,
                                             casted_or_ops, casted_and_ops, casted_xor_ops, casted_lshift_ops,
                                             casted_rshift_ops, casted_mod_ops, id_);

            }
    };

};

namespace bignum::integer::usgn::mutable_operation{

    template <class T, class T1, class ID>
    class Operator: public Operatable<Operator<T, T1, ID>>,
                    private T,
                    private T1{

        public:

            Operator() {};

            Operator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> op, 
                     std::shared_ptr<backward_caster::UINTCastable<T1>> caster): T(static_cast<T&>(*op)), T1(static_cast<T1&>(*caster)){}

            template <class T2, class T3>
            void ops(MutableBigNumable<T2>& lhs, BigNumable<T3>& rhs){
                
                T::ops(*lhs.to_reallocatable_operatable_vector(), *rhs.to_vector_readable());

            }

            template <class T2>
            void ops(MutableBigNumable<T2>& lhs, size_t rhs){
                
                auto casted = T1::cast(rhs);
                T::ops(*lhs.to_reallocatable_operatable_vector(), *casted.to_vector_readable());

            }

            using Operatable<Operator<T, T1, ID>>::to_operatable_sp;

    };

    template <class T, class T1, class T2, class T3, class ID>
    class ImmutableWrapper: public Operatable<ImmutableWrapper<T, T1, T2, T3, ID>>,
                            private T, private T1, private T3{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T2>> allocator;

        public:

            ImmutableWrapper() {};

            ImmutableWrapper(std::shared_ptr<usgn::immutable_operation::Operatable<T>> op,
                             std::shared_ptr<vector::mutable_extensible_operation::Operatable<T1>> assign_op,
                             std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T2>> allocator,
                             std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T3>> vec_gen): T(static_cast<T&>(*op)),
                                                                                                                 T1(static_cast<T1&>(*assign_op)),
                                                                                                                 T3(static_cast<T3&>(*vec_gen)){
                
                this->allocator = allocator;

            }

            template <class T4, class T5>
            void ops(MutableBigNumable<T4>& lhs, BigNumable<T5>& rhs){
                
                this->allocator->enter_scope();

                auto rs = T3::get(); 
                T::op->ops(lhs, rhs, rs);
                T1::ops(*lhs.to_reallocatable_operatable_vector(), *rs.to_vector_readable());

                this->allocator->exit_scope();

            }

            template <class T4>
            void ops(MutableBigNumable<T4>& lhs, size_t rhs){
                
                this->allocator->enter_scope();

                auto rs = T3::vec_gen->get();
                T::op->ops(lhs, rhs, rs);
                T1::ops(*lhs.to_reallocatable_operatable_vector(), *rs.to_vector_readable());
                
                this->allocator->exit_scope();

            }

        using Operatable<ImmutableWrapper<T, T1, T2, T3, ID>>::to_operatable_sp;
    
    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class ID>
    class ArithmeticOperator: public UsgnOperatable<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, ID>>,
                              private T, private T1, private T2, private T3, private T4, private T5, private T6,
                              private T7, private T8, private T9, private T10{

        public:

            ArithmeticOperator() = default;

            ArithmeticOperator(std::shared_ptr<Operatable<T>> assign_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                               std::shared_ptr<Operatable<T2>> plus_ops, std::shared_ptr<Operatable<T3>> rshift_ops,
                               std::shared_ptr<Operatable<T4>> lshift_ops, std::shared_ptr<Operatable<T5>> mul_ops,
                               std::shared_ptr<Operatable<T6>> div_ops, std::shared_ptr<Operatable<T7>> and_operator,
                               std::shared_ptr<Operatable<T8>> or_operator, std::shared_ptr<Operatable<T9>> xor_operator,
                               std::shared_ptr<Operatable<T10>> mod_ops): T(static_cast<T&>(*assign_ops)), 
                                                                          T1(static_cast<T1&>(*minus_ops)), 
                                                                          T2(static_cast<T2&>(*plus_ops)), 
                                                                          T3(static_cast<T3&>(*rshift_ops)), 
                                                                          T4(static_cast<T4&>(*lshift_ops)), 
                                                                          T5(static_cast<T5&>(*mul_ops)), 
                                                                          T6(static_cast<T6&>(*div_ops)), 
                                                                          T7(static_cast<T7&>(*and_operator)), 
                                                                          T8(static_cast<T8&>(*or_operator)),
                                                                          T9(static_cast<T9&>(*xor_operator)), 
                                                                          T10(static_cast<T10&>(*mod_ops)){

            }

            template <class T12, class T13>
            void assign(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){
                
                T::ops(lhs, rhs);

            }

            template <class T12>
            void assign(MutableBigNumable<T12>& lhs, size_t rhs){
                
                T::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void minus(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T1::ops(lhs, rhs);

            }

            template <class T12>
            void minus(MutableBigNumable<T12>& lhs, size_t rhs){

                T1::ops(lhs, rhs);
                
            }

            template <class T12, class T13>
            void plus(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T2::ops(lhs, rhs);

            }

            template <class T12>
            void plus(MutableBigNumable<T12>& lhs, size_t rhs){

                T2::ops(lhs, rhs);
                
            }

            template <class T12, class T13>
            void rshift(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T3::ops(lhs, rhs);

            }

            template <class T12>
            void rshift(MutableBigNumable<T12>& lhs, size_t rhs){

                T3::ops(lhs, rhs);

            }            

            template <class T12, class T13>
            void lshift(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T4::ops(lhs, rhs);

            }

            template <class T12>
            void lshift(MutableBigNumable<T12>& lhs, size_t rhs){

                T4::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void multiply(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T5::ops(lhs, rhs);

            }

            template <class T12>
            void multiply(MutableBigNumable<T12>& lhs, size_t rhs){

                T5::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void divide(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T6::ops(lhs, rhs);

            }

            template <class T12>
            void divide(MutableBigNumable<T12>& lhs, size_t rhs){

                T6::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void and_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T7::ops(lhs, rhs);

            }

            template <class T12>
            void and_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                T7::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void or_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T8::ops(lhs, rhs);

            }

            template <class T12>
            void or_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                T8::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void xor_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T9::ops(lhs, rhs);

            }

            template <class T12>
            void xor_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                T9::ops(lhs, rhs);

            }

            template <class T12, class T13>
            void mod(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                T10::ops(lhs, rhs);

            }

            template <class T12>
            void mod(MutableBigNumable<T12>& lhs, size_t rhs){

                T10::ops(lhs, rhs);

            }

    };

    template <class T, unsigned>
    class DoubleID{};

    class IDArithmeticOperatorGenerator{

        private:

            template <class T, class T1, class ID>
            std::shared_ptr<Operator<T, T1, ID>> init_operator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> op, 
                                                               std::shared_ptr<backward_caster::UINTCastable<T1>> caster,
                                                               ID){
                
                return std::make_shared<Operator<T, T1, ID>>(op, caster); 

            } 

            template <class T, class T1, class T2, class T3, class ID>
            std::shared_ptr<ImmutableWrapper<T, T1, T2, T3, ID>> init_immutable_wrapper(std::shared_ptr<usgn::immutable_operation::Operatable<T>> op,
                                                                                        std::shared_ptr<vector::mutable_extensible_operation::Operatable<T1>> assign_op,
                                                                                        std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T2>> allocator,
                                                                                        std::shared_ptr<memory::sizet_linear::ReallocatableVectorGeneratable<T3>> temp_vec_gen,
                                                                                        ID){
                
                return std::make_shared<ImmutableWrapper<T, T1, T2, T3, ID>>(op, assign_op, allocator, temp_vec_gen);

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class ID>
            auto init_arithmetic(std::shared_ptr<Operatable<T>> assign_ops, 
                                 std::shared_ptr<Operatable<T1>> minus_ops,
                                 std::shared_ptr<Operatable<T2>> plus_ops, 
                                 std::shared_ptr<Operatable<T3>> rshift_ops,
                                 std::shared_ptr<Operatable<T4>> lshift_ops, 
                                 std::shared_ptr<Operatable<T5>> mul_ops,
                                 std::shared_ptr<Operatable<T6>> div_ops, 
                                 std::shared_ptr<Operatable<T7>> and_ops,
                                 std::shared_ptr<Operatable<T8>> or_ops, 
                                 std::shared_ptr<Operatable<T9>> xor_ops,
                                 std::shared_ptr<Operatable<T10>> mod_ops, 
                                 ID){
                
                return std::make_shared<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, ID>>(assign_ops, minus_ops, plus_ops, rshift_ops,
                                                                                                            lshift_ops, mul_ops, div_ops, and_ops,
                                                                                                            or_ops, xor_ops, mod_ops);

                
            }  

        public:

            template <class ID>
            auto get_assign(ID id_){ // -> shared_ptr<Operatable<>>
                
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster, id_);
 
            }

            template <class T, class ID>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_plus(allocator, DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster, id_);

            }

            template <class T, class ID>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_minus(allocator, DoubleID<ID, 0>());
                auto caster = backward_caster::IDGenerator().get_uint_caster(DoubleID<ID, 1>());

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster, id_);

            }

            template <class T, class ID>
            auto get_rshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_rshift(DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
            
            }

            template <class T, class ID>
            auto get_lshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_lshift(DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            

            }

            template <class T, class ID>
            auto get_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_mul(allocator, DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
                 
            }

            template <class T, class ID>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_divide(allocator, DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
                
            }

            template <class T, class ID>
            auto get_and(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_and(DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
        
              
            }

            template <class T, class ID>
            auto get_or(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);
                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_or(DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
                
            }

            template <class T, class ID>
            auto get_xor(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_xor(DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            
               
            }

            template <class T, class ID>
            auto get_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = immutable_operation::IDArithmeticOperatorGenerator().get_standard_mod(allocator, DoubleID<ID, 0>());
                auto xmops = vector::mutable_extensible_operation::IDArithmeticOperatableGenerator().get_assign(DoubleID<ID, 1>());
                auto temp_vec_gen = memory::sizet_linear::IDGenerator().get_temp_gen_realloc_vector_generator(allocator, DoubleID<ID, 2>()); 

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_temp_vec_gen = temp_vec_gen->to_reallocatable_vector_generatable_sp(temp_vec_gen);

                return this->init_immutable_wrapper(casted_imops, casted_xmops, allocator, casted_temp_vec_gen, id_);            


            }

            template <class T, class ID>
            auto get_arithmetic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator, ID id_){ // -> shared_ptr<UsgnOperatable<>>
                
                auto assign_ops = this->get_assign(DoubleID<ID, 0>());
                auto plus_ops = this->get_plus(allocator, DoubleID<ID, 1>());
                auto minus_ops = this->get_minus(allocator, DoubleID<ID, 2>());
                auto rshift_ops = this->get_rshift(allocator, DoubleID<ID, 3>());
                auto lshift_ops = this->get_lshift(allocator, DoubleID<ID, 4>());
                auto mul_ops = this->get_mul(allocator, DoubleID<ID, 5>());
                auto div_ops = this->get_divide(allocator, DoubleID<ID, 6>());
                auto and_ops = this->get_and(allocator, DoubleID<ID, 7>());
                auto or_ops = this->get_or(allocator, DoubleID<ID, 8>());
                auto xor_ops = this->get_xor(allocator, DoubleID<ID, 9>());
                auto mod_ops = this->get_mod(allocator, DoubleID<ID, 10>());
                
                auto casted_assign_ops = assign_ops->to_operatable_sp(assign_ops);
                auto casted_plus_ops = plus_ops->to_operatable_sp(plus_ops);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops);
                auto casted_rshift_ops = rshift_ops->to_operatable_sp(rshift_ops);
                auto casted_lshift_ops = lshift_ops->to_operatable_sp(lshift_ops);
                auto casted_mul_ops = mul_ops->to_operatable_sp(mul_ops);
                auto casted_div_ops = div_ops->to_operatable_sp(div_ops);
                auto casted_and_ops = and_ops->to_operatable_sp(and_ops);
                auto casted_or_ops = or_ops->to_operatable_sp(or_ops);
                auto casted_xor_ops = xor_ops->to_operatable_sp(xor_ops);
                auto casted_mod_ops = mod_ops->to_operatable_sp(mod_ops);

                return this->init_arithmetic(casted_assign_ops, casted_minus_ops, casted_plus_ops, casted_rshift_ops,
                                             casted_lshift_ops, casted_mul_ops, casted_div_ops, casted_and_ops,
                                             casted_or_ops, casted_xor_ops, casted_mod_ops, id_);

            }

    };

};

namespace bignum::integer::usgn::parser{

    class GenericEstimator: public virtual Estimatable{
        
        private:

            size_t bit_width; 

        public:

            GenericEstimator(size_t bit_width){

                this->bit_width = bit_width;

            }

            size_t get(std::string_view data){
                
                assert(data.length() != 0);

                size_t total_length = data.length() * bit_width;

                if (total_length % vector::BIT_LENGTH_PER_SLOT == 0){

                    return total_length / vector::BIT_LENGTH_PER_SLOT;

                }

                return total_length / vector::BIT_LENGTH_PER_SLOT + 1;

            }

    };

    class BinEstimator: public GenericEstimator{

        public:

            BinEstimator(): GenericEstimator(1){};

    };
    
    class HexEstimator: public GenericEstimator{

        public:

            HexEstimator(): GenericEstimator(4){};

    };
    
    template <class T, class T1, class T2>
    class BinaryStringParser: public StringParsable<BinaryStringParser<T, T1, T2>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster;
            std::shared_ptr<Estimatable> accurate_estimator;

        public:

            BinaryStringParser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                               std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen,
                               std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster,
                               std::shared_ptr<Estimatable> accurate_estimator){
                
                this->allocator = allocator;
                this->bit_writer_gen = bit_writer_gen;
                this->bignum_caster = bignum_caster;
                this->accurate_estimator = accurate_estimator;

            }

            auto parse(std::string_view data){
                                
                const uint8_t WRITE_LENGTH = 1;
                const uint8_t OFFSET = 0;
            
                auto vec = this->allocator->vec_get_default(this->accurate_estimator->get(data));
                auto iter_writer = this->bit_writer_gen->get(vec, WRITE_LENGTH, OFFSET);
            
                for (intmax_t i = (intmax_t) data.size() - 1; i >= 0; --i){

                    if (data[i] == '1'){

                        assert(iter_writer.write(1));
                        
                    } else{
                        
                        assert(data[i] == '0');
                        assert(iter_writer.write(0));

                    }

                }

                return this->bignum_caster->cast(vec);

            }   

            
    };

    template <class T, class T1, class T2>
    class HexStringParser: public StringParsable<HexStringParser<T, T1, T2>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster;
            std::shared_ptr<Estimatable> accurate_estimator;

        public:

            HexStringParser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                            std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen,
                            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster,
                            std::shared_ptr<Estimatable> accurate_estimator){
                
                this->allocator = allocator;
                this->bit_writer_gen = bit_writer_gen;
                this->bignum_caster = bignum_caster;
                this->accurate_estimator = accurate_estimator;

            }


            auto parse(std::string_view data){
                
                const uint8_t WRITE_LENGTH = 4;
                const uint8_t OFFSET = 0;

                auto vec = this->allocator->vec_get_default(this->accurate_estimator->get(data));
                auto iter_writer = this->bit_writer_gen->get(vec, WRITE_LENGTH, OFFSET);

                for (intmax_t i = (intmax_t) data.size() - 1; i >= 0; --i){

                    assert(iter_writer.write(this->to_hex(data[i])));

                }

                return this->bignum_caster->cast(vec); 

            }
        
        private:

            size_t to_hex(char data){

                if ((data >= '0') && (data <= '9')){

                    return data - '0';

                }

                if ((data >= 'A') && (data <= 'F')){

                    return data + 10 - 'A'; 

                }

                if ((data >= 'a') && (data <= 'f')){
                    
                    return data + 10 - 'a';

                }
                
                assert(false);

                return 0;

            }

    };

    class StringParserClassifier: public virtual StringParserClassifiable{

        public:

            uint8_t classify(std::string_view data){

                assert(data.length() > 2);

                if ((data[0] == '0') && (data[1] == 'b')){

                    return BIN_CONST; 

                }

                if ((data[0] == '0') && (data[1] == 'x')){

                    return HEX_CONST;

                }

                return DEC_CONST;

            }

            std::string_view remove_header(std::string_view data){

                assert(data.length() > 2);

                return std::string_view(data.data() + 2, data.length() - 2); 

            }
            
    };
    
    template <class T, class T1, class T2>
    class StandardStringParser: public StringParsable<StandardStringParser<T, T1, T2>>{

        private:

            std::shared_ptr<StringParserClassifiable> classifier;
            std::shared_ptr<StringParsable<T>> bin_parser;
            std::shared_ptr<StringParsable<T1>> hex_parser;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> caster;
        
        public:
            
            StandardStringParser() {};

            StandardStringParser(std::shared_ptr<StringParserClassifiable> classifier,
                                 std::shared_ptr<StringParsable<T>> bin_parser,
                                 std::shared_ptr<StringParsable<T1>> hex_parser,
                                 std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> caster){
                
                this->classifier = classifier;
                this->bin_parser = bin_parser;
                this->hex_parser = hex_parser;
                this->caster = caster;

            }

            auto parse(std::string_view data){

                uint8_t id = this->classifier->classify(data);
                
                if (id == StringParserClassifier::BIN_CONST){
                    
                    auto parsed_rs = this->bin_parser->parse(this->classifier->remove_header(data));

                    return this->caster->cast(parsed_rs);

                }

                auto parsed_rs = this->hex_parser->parse(this->classifier->remove_header(data));

                return this->caster->cast(parsed_rs);

            }

    };

    class StandardGenerator{

        private:

            template <class T, class T1, class T2>
            std::shared_ptr<BinaryStringParser<T, T1, T2>> init_bin_parser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                                           std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen,
                                                                           std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster,
                                                                           std::shared_ptr<Estimatable> accurate_estimator){
                
                return std::make_shared<BinaryStringParser<T, T1, T2>>(allocator, bit_writer_gen, bignum_caster, accurate_estimator);

            }

            template <class T, class T1, class T2>
            std::shared_ptr<HexStringParser<T, T1, T2>> init_hex_parser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                                        std::shared_ptr<vector::operation_utility::BitIterWriterGeneratable<T1>> bit_writer_gen,
                                                                        std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> bignum_caster,
                                                                        std::shared_ptr<Estimatable> accurate_estimator){
                
                return std::make_shared<HexStringParser<T, T1, T2>>(allocator, bit_writer_gen, bignum_caster, accurate_estimator);

            }

            template <class T, class T1, class T2>
            std::shared_ptr<StandardStringParser<T, T1, T2>> init_std_string_parser(std::shared_ptr<StringParserClassifiable> classifier,
                                                                                    std::shared_ptr<StringParsable<T>> bin_parser,
                                                                                    std::shared_ptr<StringParsable<T1>> hex_parser,
                                                                                    std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T2>> caster){
                
                return std::make_shared<StandardStringParser<T, T1, T2>>(classifier, bin_parser, hex_parser, caster); 

            }
            
        public:

            template <class T>
            auto get_bin_parser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);
                
                auto iter_writer_gen = vector::operation_utility::StandardOperationUtilityGenerator().get_bit_iter_orwriter_gen();
                auto bignum_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto estimator = std::make_shared<BinEstimator>();

                auto casted_iter_write_gen = iter_writer_gen->to_bit_iter_writer_generatable_sp(iter_writer_gen);
                auto casted_bignum_caster = bignum_caster->to_sizet_linear_vector_castable_sp(bignum_caster);

                return this->init_bin_parser(allocator, casted_iter_write_gen, casted_bignum_caster, estimator);

            }

            template <class T>
            auto get_hex_parser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);
                
                auto iter_writer_gen = vector::operation_utility::StandardOperationUtilityGenerator().get_bit_iter_orwriter_gen();
                auto bignum_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto estimator = std::make_shared<HexEstimator>();

                auto casted_iter_write_gen = iter_writer_gen->to_bit_iter_writer_generatable_sp(iter_writer_gen);
                auto casted_bignum_caster = bignum_caster->to_sizet_linear_vector_castable_sp(bignum_caster);

                return this->init_hex_parser(allocator, casted_iter_write_gen, casted_bignum_caster, estimator);

            }

            template <class T>
            auto get_standard_parser(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){
                
                auto classifier = std::make_shared<StringParserClassifier>();
                auto bin_parser = this->get_bin_parser(allocator);
                auto hex_parser = this->get_hex_parser(allocator); 
                auto bignum_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();

                auto casted_bin_parser = bin_parser->to_string_parsable_sp(bin_parser);
                auto casted_hex_parser = hex_parser->to_string_parsable_sp(hex_parser);
                auto casted_bignum_caster = bignum_caster->to_sizet_linear_vector_castable_sp(bignum_caster);

                return this->init_std_string_parser(classifier, casted_bin_parser, casted_hex_parser, casted_bignum_caster);

            }

    };

} 

namespace bignum::integer::usgn{
    
    template <class T, class T1>
    class StandardComparer: public Comparable<StandardComparer<T, T1>>{

        private:

            std::shared_ptr<bignum::vector::comparer::Comparable<T>> vec_comparer;
            std::shared_ptr<backward_caster::UINTCastable<T1>> caster;

        public:

            StandardComparer() {};

            StandardComparer(std::shared_ptr<bignum::vector::comparer::Comparable<T>> vec_comparer,
                             std::shared_ptr<backward_caster::UINTCastable<T1>> caster){

                this->vec_comparer = vec_comparer; 
                this->caster = caster;

            } 

            template <class T2>
            int8_t compare(BigNumable<T2>& lhs, size_t rhs){

                auto casted = this->caster->cast(rhs);

                return this->compare(lhs, casted);

            }

            template <class T3, class T4>
            int8_t compare(BigNumable<T3>& lhs, BigNumable<T4>& rhs){

                return this->vec_comparer->compare(lhs, rhs);

            }

    };
    
    template <class T, class T1>
    class StandardStringifier: public Stringifiable<StandardStringifier<T, T1>>{

        private:

            std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> true_view_caster;
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T1>> allocator;

        public:

            StandardStringifier() {};

            StandardStringifier(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> true_view_caster,
                                std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T1>> allocator){

                this->true_view_caster = true_view_caster;
                this->allocator = allocator;

            }

            template <class T2>
            std::string stringify(BigNumable<T2>& data){
                
                this->allocator->enter_scope();

                auto casted = this->true_view_caster->cast(data);
                size_t max_digits = std::log10(2) * casted.length() + 1;
                size_t cur_sz = 1;
                uint8_t * ord_data = (uint8_t *) this->allocator->get(max_digits * sizeof(uint8_t)); 

                memset((void *) ord_data, 0, max_digits * sizeof(uint8_t));

                for (intmax_t i = (intmax_t) casted.length() - 1; i >= 0; --i){
                    
                    this->dup(ord_data, cur_sz);

                    if (casted.get(i)){

                        this->inc(ord_data);

                    }

                    cur_sz = this->resolve(ord_data, cur_sz);

                }

                std::string rs;
                this->reverse_to_char(ord_data, cur_sz, rs);

                this->allocator->exit_scope();

                return rs;

            }

        
        private:

            void dup(uint8_t * data, size_t data_sz){

                for (size_t i = 0; i < data_sz; ++i){

                    data[i] *= 2;

                }

            }

            void inc(uint8_t * data){

                ++data[0];

            }

            size_t resolve(uint8_t * data, size_t data_sz){

                assert(data_sz != 0);

                for (size_t i = 0; i < data_sz - 1; ++i){

                    size_t div = data[i] / 10;
                    size_t mod = data[i] % 10;

                    data[i] = mod;
                    data[i + 1] += div;

                }

                while (data[data_sz - 1] >= 10){
                    
                    size_t div = data[data_sz - 1] / 10;
                    size_t mod = data[data_sz - 1] % 10;

                    data[data_sz - 1] = mod;
                    data[data_sz] += div;

                    ++data_sz;

                }

                return data_sz;
                
            } 

            void reverse_to_char(uint8_t * data, size_t sz, std::string& op){
                
                assert(sz != 0);

                while ((sz != 0) && (data[sz - 1] == 0)){
                    --sz;
                }

                if (sz == 0){

                    op = '0';
                    
                    return;

                }

                op.reserve(sz);

                for (intmax_t i = (intmax_t) sz - 1; i >= 0; --i){

                    op.push_back(data[i] + 48);

                }

            }

    };

    class UtilityGenerator{

        private:

            template <class T, class T1>
            std::shared_ptr<StandardComparer<T, T1>> init_std_comparer(std::shared_ptr<bignum::vector::comparer::Comparable<T>> vec_comparer,
                                                                       std::shared_ptr<backward_caster::UINTCastable<T1>> caster){

                return std::make_shared<StandardComparer<T, T1>>(vec_comparer, caster);

            }

            template <class T, class T1>
            std::shared_ptr<StandardStringifier<T, T1>> init_std_stringifier(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> true_view_caster,
                                                                             std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T1>> allocator){

                return std::make_shared<StandardStringifier<T, T1>>(true_view_caster, allocator);

            }

        public:

            auto get_comparer(){ // -> shared_ptr<Comparable<>>
                
                auto vec_comparer = bignum::vector::comparer::StandardComparerGenerator().get_backward_comparer();
                auto uint_caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_vec_comparer = vec_comparer->to_comparable_sp(vec_comparer);
                auto casted_uint_caster = uint_caster->to_uint_castable_sp(uint_caster);

                return this->init_std_comparer(casted_vec_comparer, casted_uint_caster);

            }

            template <class T>
            auto get_stringifier(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Stringifiable<>>

                // assert(allocator != nullptr);

                auto true_view_caster = bignum::vector::caster::StandardBoolVectorCasterGenerator().get_true_view_caster();
                auto casted_true_view_caster = true_view_caster->to_boolvector_view_castable_sp(true_view_caster);

                return this->init_std_stringifier(casted_true_view_caster, allocator); 

            }


    };
    
    template <class T>
    class Retriever{

        private:
            
            inline static const uint8_t CAPACITY = 8;
            inline static T data[CAPACITY]; 
            inline static std::vector<std::shared_ptr<T>> data_;
            inline static bool initiated = false;

        public:

            void init_data(std::vector<std::shared_ptr<T>> data){

                assert(data.size() <= this->CAPACITY);

                if (!this->initiated){

                    for (size_t i = 0; i < CAPACITY; ++i){

                        this->data[i].~T();
                        
                    }

                    this->initiated = true;

                }

                this->data_ = data;

                for (size_t i = 0; i < data.size(); ++i){

                    memcpy(&this->data[i], &*data[i], sizeof(T));

                }


            }

            static inline std::shared_ptr<T> get_ptr(size_t idx){

                return Retriever::data_[idx]; 

            }

            static inline T * get(size_t idx){

                return &Retriever::data[idx];

            }

            static inline T * head(){

                return Retriever::data;

            }

    };
 
    template <class T, class T1, class T2, class T3, class T4, class T5>
    class ThreadSafeRetriever{

        private:

            inline static const uint8_t CAPACITY = 20;
            inline static size_t thread_ids[CAPACITY];
            inline static size_t n; 
            
            static void cb_func(size_t thread_id){

                //std::lock_guard<std::mutex> grd(ThreadSafeRetriever::mtx);

                ThreadSafeRetriever::thread_ids[ThreadSafeRetriever::n++] = thread_id;

            }

            template <class T6>
            struct Token{
                
                public:

                    T6 resource;
                    intmax_t thread_id; 

                public:

                    Token(T6 resource, size_t thread_id){
                        
                        this->resource = resource;
                        this->thread_id = thread_id;

                    }
                    
                    Token(const Token&) = delete;
                    
                    Token(Token&& data){
                        
                        this->resource = data.resource;
                        this->thread_id = data.thread_id;
                        data.thread_id = -1;            

                    }

                    Token& operator = (Token&& data){
                        
                        this->resource = data.resource;
                        this->thread_id = data.thread_id;
                        data.thread_id = -1;            

                        return *this;

                    }

                    Token& operator = (const Token& data) = delete;

                    ~Token(){
                        
                        if (this->thread_id == -1){

                            return;

                        }

                        ThreadSafeRetriever::cb_func(this->thread_id);

                    }

                    T6 operator ->(){

                        return this->resource; 

                    }

            };

            template <class T6, class T7>
            struct ScopeToken: public Token<T6>{

                public:

                    memory::sizet_linear::TempStorageGeneratable<T7> * allocator;
                    bool exit_allowed;

                public:

                    ScopeToken(T6 resource, size_t thread_id, memory::sizet_linear::TempStorageGeneratable<T7> * allocator): Token<T6>(resource, thread_id){

                        this->allocator = allocator;
                        this->exit_allowed = true;

                        this->allocator->enter_scope();

                    }
                    ScopeToken(const ScopeToken&) = delete;
                    ScopeToken(ScopeToken&& obj){
                        
                        this->allocator = obj.allocator;
                        this->exit_allowed = obj.exit_allowed;
                        obj.exit_allowed = false;

                    }

                    ScopeToken& operator = (const ScopeToken&) = delete;
                    ScopeToken& operator = (ScopeToken&& obj){

                        this->allocator = obj.allocator;
                        this->exit_allowed = obj.exit_allowed;
                        obj.exit_allowed = false;

                    }

                    ~ScopeToken(){

                        if (this->exit_allowed){
                            this->allocator->exit_scope();
                        }
                        
                    }

            };

            template <class T6>
            Token<T6> make_token(T6 resource, size_t thread_id){

                return Token<T6>(resource, thread_id);

            }

            template <class T6, class T7>
            ScopeToken<T6, T7> make_scope_token(T6 resource, size_t thread_id, memory::sizet_linear::TempStorageGeneratable<T7> * allocator){

                return ScopeToken<T6, T7>(resource, thread_id, allocator); 

            }

        public:

            void set_threads(size_t num_threads){

                //std::lock_guard<std::mutex> grd(this->mtx);
                ThreadSafeRetriever::n = 0;
                
                for (size_t i = 0; i < num_threads; ++i){

                    ThreadSafeRetriever::thread_ids[ThreadSafeRetriever::n++] = i;

                }

            }

            inline constexpr auto get_comparer() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(ThreadSafeRetriever::n != 0);

                size_t thread_id = ThreadSafeRetriever::thread_ids[--ThreadSafeRetriever::n]; 

                return this->make_token(Retriever<T>::get(thread_id), thread_id);

            }

            inline constexpr auto get_stringifier() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(ThreadSafeRetriever::n != 0);

                size_t thread_id = ThreadSafeRetriever::thread_ids[--ThreadSafeRetriever::n]; 

                return this->make_scope_token(Retriever<T1>::get(thread_id), thread_id, Retriever<T5>::get(thread_id));

            }

            inline constexpr auto get_mutable_operator() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(ThreadSafeRetriever::n != 0);

                size_t thread_id = ThreadSafeRetriever::thread_ids[--ThreadSafeRetriever::n]; 

                return this->make_token(Retriever<T2>::get(thread_id), thread_id);

            }

            inline constexpr auto get_immutable_operator() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(ThreadSafeRetriever::n != 0);

                size_t thread_id = ThreadSafeRetriever::thread_ids[--ThreadSafeRetriever::n]; 

                return this->make_token(Retriever<T3>::get(thread_id), thread_id);
           
            }

            inline constexpr auto get_parser() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(ThreadSafeRetriever::n != 0);

                size_t thread_id = ThreadSafeRetriever::thread_ids[--ThreadSafeRetriever::n]; 

                return this->make_scope_token(Retriever<T4>::get(thread_id), thread_id, Retriever<T5>::get(thread_id));

            }

    };
    
    template <class T, class T1, class T2, class T3, class T4, class T5>
    class SingleThreadRetriever{

        public:

            inline constexpr auto get_comparer() noexcept{

                return Retriever<T>::head();

            }

            inline constexpr auto get_stringifier() noexcept{

                return Retriever<T1>::head();

            }

            inline constexpr auto get_mutable_operator() noexcept{

                return Retriever<T2>::head();

            }

            inline constexpr auto get_immutable_operator() noexcept{

                return Retriever<T3>::head();
           
            }

            inline constexpr auto get_parser() noexcept{

                return Retriever<T4>::head();

            }

    };

    class GenericID{};

    class ResourceTypeManager{

        public:

            auto get_tstorage_type(){
                
                auto allocator = memory::sizet_linear::StandardGenerator().get_temp_storage(1024);

                return allocator->get_containee_type(); 

            }

            auto get_parser_type(){

                auto parser = parser::StandardGenerator().get_standard_parser<decltype(this->get_tstorage_type())>(nullptr); 

                return parser->get_containee_type();

            }

            auto get_comparer_type(){

                auto comparer = UtilityGenerator().get_comparer();
                
                return comparer->get_containee_type();

            }
            
            auto get_stringifier_type(){

                auto stringifier = UtilityGenerator().get_stringifier<decltype(this->get_tstorage_type())>(nullptr);

                return stringifier->get_containee_type();

            }

            auto get_immutable_ops_type(){

                auto immutable_ops = immutable_operation::IDArithmeticOperatorGenerator().get_arithmetic<decltype(this->get_tstorage_type()), GenericID>(nullptr, GenericID());

                return immutable_ops->get_containee_type();

            }

            auto get_mutable_ops_type(){

                auto mutable_ops = mutable_operation::IDArithmeticOperatorGenerator().get_arithmetic<decltype(this->get_tstorage_type()), GenericID>(nullptr, GenericID());

                return mutable_ops->get_containee_type();

            }

    };

    class ThreadSafeRetrieverWrapper: private ResourceTypeManager{

        public:

            inline constexpr auto get_retriever() noexcept{

                return ThreadSafeRetriever<decltype(this->get_comparer_type()), decltype(this->get_stringifier_type()), 
                        decltype(this->get_mutable_ops_type()), decltype(this->get_immutable_ops_type()), decltype(this->get_parser_type()),
                        decltype(this->get_tstorage_type())>();


            }

    };

    class SingleThreadRetrieverWrapper: private ResourceTypeManager{

        public:

            inline constexpr auto get_retriever() noexcept{

                return SingleThreadRetriever<decltype(this->get_comparer_type()), decltype(this->get_stringifier_type()), 
                        decltype(this->get_mutable_ops_type()), decltype(this->get_immutable_ops_type()), decltype(this->get_parser_type()),
                        decltype(this->get_tstorage_type())>();


            }

    };

    class ResourceInitiator: public ResourceTypeManager{

        private:

            void init_temp_storage_retriever(size_t n) {

                std::vector<std::shared_ptr<decltype(this->get_tstorage_type())>> allocator;

                for (size_t i = 0; i < n; ++i){

                    auto temp_storage = std::make_shared<decltype(this->get_tstorage_type())>();

                    allocator.push_back(temp_storage);

                }

                Retriever<decltype(this->get_tstorage_type())>().init_data(allocator);

            }

            void init_parser_retriever(size_t n){ 
                
                std::vector<std::shared_ptr<decltype(this->get_parser_type())>> data; 

                for (size_t i = 0; i < n; ++i){

                    auto allocator = Retriever<decltype(this->get_tstorage_type())>().get_ptr(i);
                    auto casted_allocator = allocator->to_temp_storage_generatable_sp(allocator);

                    data.push_back(parser::StandardGenerator().get_standard_parser(casted_allocator));

                }

                Retriever<decltype(this->get_parser_type())>().init_data(data);

            } 

            void init_comparer_retriever(size_t n){

                std::vector<std::shared_ptr<decltype(this->get_comparer_type())>> data;

                for (size_t i = 0; i < n; ++i){

                    data.push_back(UtilityGenerator().get_comparer());

                }

                Retriever<decltype(this->get_comparer_type())>().init_data(data);

            }

            void init_stringifier_retriever(size_t n){

                std::vector<std::shared_ptr<decltype(this->get_stringifier_type())>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = Retriever<decltype(this->get_tstorage_type())>().get_ptr(i);
                    auto casted_allocator = allocator->to_temp_storage_generatable_sp(allocator);

                    data.push_back(UtilityGenerator().get_stringifier(casted_allocator));

                }

                Retriever<decltype(this->get_stringifier_type())>().init_data(data);

            }

            void init_mutable_ops_retriever(size_t n){

                std::vector<std::shared_ptr<decltype(this->get_mutable_ops_type())>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = Retriever<decltype(this->get_tstorage_type())>().get_ptr(i);
                    auto casted_allocator = allocator->to_temp_storage_generatable_sp(allocator);

                    data.push_back(mutable_operation::IDArithmeticOperatorGenerator().get_arithmetic(casted_allocator, GenericID()));

                }

                Retriever<decltype(this->get_mutable_ops_type())>().init_data(data);


            }

            void init_immutable_ops_retriever(size_t n){

                std::vector<std::shared_ptr<decltype(this->get_immutable_ops_type())>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = Retriever<decltype(this->get_tstorage_type())>().get_ptr(i);
                    auto casted_allocator = allocator->to_temp_storage_generatable_sp(allocator);

                    data.push_back(immutable_operation::IDArithmeticOperatorGenerator().get_arithmetic(casted_allocator, GenericID()));

                }

                Retriever<decltype(this->get_immutable_ops_type())>().init_data(data);

            }

        public:

            ResourceInitiator(size_t num_threads){

                this->init_temp_storage_retriever(num_threads);
                this->init_parser_retriever(num_threads);
                this->init_comparer_retriever(num_threads);
                this->init_stringifier_retriever(num_threads);
                this->init_mutable_ops_retriever(num_threads);
                this->init_immutable_ops_retriever(num_threads);

                ThreadSafeRetrieverWrapper().get_retriever().set_threads(num_threads);

            }

    };

    class ResourceDestructor: public ResourceTypeManager{

        public:

            ResourceDestructor(){

                Retriever<decltype(this->get_tstorage_type())>().init_data({});
                Retriever<decltype(this->get_parser_type())>().init_data({});
                Retriever<decltype(this->get_comparer_type())>().init_data({});
                Retriever<decltype(this->get_mutable_ops_type())>().init_data({});
                Retriever<decltype(this->get_immutable_ops_type())>().init_data({});
                ThreadSafeRetrieverWrapper().get_retriever().set_threads(0);

            }

    };
    
    class BigNumStdReallocator: public memory::linear::Reallocatable<BigNumStdReallocator>, public memory::linear::StdReallocator{
        
        public:

            using memory::linear::StdReallocator::malloc;
            using memory::linear::StdReallocator::free;
            using memory::linear::StdReallocator::realloc;

    };

    class BigNumCircularReallocator: public memory::linear::Reallocatable<BigNumCircularReallocator>, public memory::linear::CircularReallocator{

        private:

            static const uint16_t CIRCULAR_BUF_LENGTH = std::numeric_limits<uint16_t>::max();
        
        public:

            BigNumCircularReallocator(): memory::linear::CircularReallocator(CIRCULAR_BUF_LENGTH){};

            using memory::linear::CircularReallocator::malloc;
            using memory::linear::CircularReallocator::free;
            using memory::linear::CircularReallocator::realloc;

    };

    template <class ReallocatableVector, class RetrieverWrapper>
    class StdMutableBigNum: public MutableBigNumable<StdMutableBigNum<ReallocatableVector, RetrieverWrapper>>, public ReallocatableVector, 
                            private RetrieverWrapper{
        
        private:

            StdMutableBigNum(std::nullptr_t): ReallocatableVector(){}

        public:

            using Self = StdMutableBigNum<ReallocatableVector, RetrieverWrapper>;
            using ReallocatableVector::get;
            using ReallocatableVector::length;
            using ReallocatableVector::resize;
            using ReallocatableVector::resize_no_copy;
            using ReallocatableVector::get_data;
            using ReallocatableVector::sizeof_slot;
            using ReallocatableVector::set;
            using ReallocatableVector::to_vector_readable;
            using ReallocatableVector::to_operatable_vector;
            using ReallocatableVector::to_reallocatable_operatable_vector;

        public:

            StdMutableBigNum(){

                *this = 0;

            }

            StdMutableBigNum(std::string obj){

                auto tup = this->get_retriever().get_parser();
                auto rs = tup->parse(obj);

                *this = rs;

            }

            StdMutableBigNum(size_t val){

                *this = val;

            }

            template <class T>
            StdMutableBigNum(BigNumable<T>& obj){
                                
                *this = obj;

            }

            StdMutableBigNum(Self& obj){
                                
                *this = obj;

            }

            StdMutableBigNum(StdMutableBigNum&& obj): ReallocatableVector(std::move(obj)){}

            template <class T>
            bool operator > (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == 1;

            }

            bool operator > (size_t rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == 1;

            }
            
            bool operator > (StdMutableBigNum&& rhs){
                
                return this->get_retriever().get_comparer()->compare(*this, rhs) == 1;

            }

            template <class T>
            bool operator < (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == -1;

            }

            bool operator < (size_t rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == -1;

            }

            bool operator < (StdMutableBigNum&& rhs){
                
                return this->get_retriever().get_comparer()->compare(*this, rhs) == -1;

            }

            template <class T>
            bool operator == (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == 0;

            }

            bool operator == (size_t rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) == 0;

            }

            bool operator == (StdMutableBigNum&& rhs){
                
                return this->get_retriever().get_comparer()->compare(*this, rhs) == 0;

            }

            template <class T>
            bool operator != (BigNumable<T>& rhs){

                return !(*this == rhs);

            }

            bool operator != (size_t rhs){

                return !(*this == rhs);

            }

            bool operator != (StdMutableBigNum&& rhs){

                return !(*this == rhs);

            }

            template <class T>
            bool operator >= (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) >= 0;

            }

            bool operator >= (size_t rhs){
                
                return this->get_retriever().get_comparer()->compare(*this, rhs) >= 0;

            }

            bool operator >= (StdMutableBigNum&& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) >= 0;

            }

            template <class T>
            bool operator <= (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) <= 0;

            }

            bool operator <= (size_t rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) <= 0;

            }
            
            bool operator <= (StdMutableBigNum&& rhs){

                return this->get_retriever().get_comparer()->compare(*this, rhs) <= 0;
                
            }

            std::string to_string(){

                return this->get_retriever().get_stringifier()->stringify(*this);

            }

            StdMutableBigNum& operator = (StdMutableBigNum&& obj){

                ReallocatableVector::operator=(std::move(obj));

                return *this;

            }

            template <class T>
            StdMutableBigNum& operator = (BigNumable<T>& rhs){
                                
                if (this->length() != rhs.length()){

                    this->resize_no_copy(rhs.length());

                }

                for (size_t i = 0; i < this->length(); ++i){

                    this->set(i, rhs.get(i));

                }

                return *this;

            }
            
            StdMutableBigNum& operator = (size_t rhs){
                
                this->get_retriever().get_mutable_operator()->assign(*this, rhs);
                
                return *this;

            }
            
            StdMutableBigNum& operator = (Self& rhs){
                
                return *this = *rhs.to_bignumable();

            }

            template <class T>
            StdMutableBigNum operator + (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->plus(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator + (size_t rhs){
                            
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->plus(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;
            
            }

            StdMutableBigNum operator + (StdMutableBigNum&& rhs){

                rhs += *this;

                return StdMutableBigNum(std::move(rhs));
                
            }

            template <class T>
            StdMutableBigNum operator - (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->minus(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator - (size_t rhs){
                
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->minus(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator - (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->minus(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }
            
            template <class T>
            StdMutableBigNum operator * (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->multiply(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator * (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->multiply(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator * (StdMutableBigNum&& rhs){
                
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->multiply(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }
        
            template <class T>
            StdMutableBigNum operator / (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->divide(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator / (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->divide(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator / (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->divide(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator % (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->mod(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator % (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->mod(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator % (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->mod(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator & (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->and_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;


            }

            StdMutableBigNum operator & (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->and_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator & (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->and_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator | (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->or_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator | (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->or_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator | (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->or_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator ^ (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->xor_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator ^ (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->xor_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator ^ (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->xor_ops(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator >> (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->rshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator >> (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->rshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator >> (StdMutableBigNum&& rhs){
                
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->rshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum operator << (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->lshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator << (size_t rhs){
                
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->lshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            StdMutableBigNum operator << (StdMutableBigNum&& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = StdMutableBigNum(nullptr);

                tup->lshift(*this, rhs, *rs.to_reallocatable_operatable_vector());

                return rs;

            }

            template <class T>
            StdMutableBigNum& operator += (BigNumable<T>& rhs){
                
                this->get_retriever().get_mutable_operator()->plus(*this, rhs);

                return *this;
            
            }

            StdMutableBigNum& operator += (size_t rhs){
                
                this->get_retriever().get_mutable_operator()->plus(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator += (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->plus(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator -= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->minus(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator -= (size_t rhs){

                this->get_retriever().get_mutable_operator()->minus(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator -= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->minus(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator *= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->multiply(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator *= (size_t rhs){

                this->get_retriever().get_mutable_operator()->multiply(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator *= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->multiply(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator /= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->divide(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator /= (size_t rhs){

                this->get_retriever().get_mutable_operator()->divide(*this, rhs);

                return *this;

            }
           
            StdMutableBigNum& operator /= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->divide(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator %= (BigNumable<T>& rhs){
                
                this->get_retriever().get_mutable_operator()->mod(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator %= (size_t rhs){

                this->get_retriever().get_mutable_operator()->mod(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator %= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->mod(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator &= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->and_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator &= (size_t rhs){

                this->get_retriever().get_mutable_operator()->and_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator &= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->and_ops(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator |= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->or_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator |= (size_t rhs){

                this->get_retriever().get_mutable_operator()->or_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator |= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->or_ops(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator ^= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->xor_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator ^= (size_t rhs){

                this->get_retriever().get_mutable_operator()->xor_ops(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator ^= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->xor_ops(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator >>= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->rshift(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator >>= (size_t rhs){

                this->get_retriever().get_mutable_operator()->rshift(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator >>= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->rshift(*this, rhs);

                return *this;
            
            }

            template <class T>
            StdMutableBigNum& operator <<= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator()->lshift(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator <<= (size_t rhs){

                this->get_retriever().get_mutable_operator()->lshift(*this, rhs);

                return *this;

            }

            StdMutableBigNum& operator <<= (StdMutableBigNum&& rhs){
                
                this->get_retriever().get_mutable_operator()->lshift(*this, rhs);

                return *this;
            
            }

    };

}   

namespace dgstd{

    using BigUINT = bignum::integer::usgn::StdMutableBigNum<memory::sizet_linear::RelaxReallocatableMemControlledVectorOperator<bignum::integer::usgn::BigNumStdReallocator>,
                                                            bignum::integer::usgn::SingleThreadRetrieverWrapper>;

    using CircularBigUINT = bignum::integer::usgn::StdMutableBigNum<memory::sizet_linear::RelaxReallocatableMemControlledVectorOperator<bignum::integer::usgn::BigNumCircularReallocator>,
                                                                    bignum::integer::usgn::SingleThreadRetrieverWrapper>;
    
    template <unsigned CAP>
    using StackBigUINT = bignum::integer::usgn::StdMutableBigNum<memory::sizet_linear::StandardStackAllocatedVector<CAP>,
                                                                 bignum::integer::usgn::SingleThreadRetrieverWrapper>;

    static void big_uint_resource_init(size_t n = 1){

        assert(n != 0);

        bignum::integer::usgn::ResourceInitiator _init_bigint_resource(n);

    }
    
    static void big_uint_resource_destruct(){

        bignum::integer::usgn::ResourceDestructor();
        memory::linear::AllocatorSingleton<bignum::integer::usgn::BigNumStdReallocator>::destruct();
        memory::linear::AllocatorSingleton<bignum::integer::usgn::BigNumCircularReallocator>::destruct();

    }

};

#endif