
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

namespace bignum::vector{

    //inline static const uint8_t BIT_LENGTH_PER_SLOT = 1;
     
    #define QWORD_MACHINE
    #define BIT_LENGTH_PER_SLOT_62
    //#define BIT_LENGTH_PER_SLOT_30
    inline static const uint8_t BIT_LENGTH_PER_SLOT = 62;

    //inline static const uint8_t BIT_LENGTH_PER_SLOT = sizeof(size_t) * 8 - 2;
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
        
        template <class T>
        class GeneralUtilizable{

            public:

                size_t get_borrow(){

                    return static_cast<T*>(this)->get_borrow();

                }

                size_t get_max_per_slot(){

                    return static_cast<T*>(this)->get_max_per_slot();

                }

                size_t prune(size_t data){

                    return static_cast<T*>(this)->prune(data);

                }

                size_t get_overflow(size_t data){

                    return static_cast<T*>(this)->get_overflow(data);

                }

                template <class T1>
                std::shared_ptr<GeneralUtilizable<T>> to_general_utilizable_sp(std::shared_ptr<T1> data){

                    return std::dynamic_pointer_cast<GeneralUtilizable<T>>(data); 

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
    class BigNumable: public memory::sizet_linear::VectorReadable<T>{};

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
                auto ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>& 

                    return static_cast<T*>(this)->ops(lhs, rhs);

                }

                template <class T1>
                auto ops(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

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
                auto minus(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->minus(lhs, rhs);

                }   
                
                template <class T1>
                auto minus(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->minus(lhs, rhs); 

                }

                template <class T1, class T2>
                auto plus(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->plus(lhs, rhs);

                }

                template <class T1>
                auto plus(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->plus(lhs, rhs); 

                }

                template <class T1, class T2>
                auto rshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->rshift(lhs, rhs);

                }

                template <class T1>
                auto rshift(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->rshift(lhs, rhs); 

                }

                template <class T1, class T2>
                auto lshift(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->lshift(lhs, rhs);

                }

                template <class T1>
                auto lshift(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->lshift(lhs, rhs); 

                }

                template <class T1, class T2>
                auto multiply(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->multiply(lhs, rhs);

                }

                template <class T1>
                auto multiply(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->multiply(lhs, rhs); 

                }

                template <class T1, class T2>
                auto divide(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->divide(lhs, rhs);

                }

                template <class T1>
                auto divide(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->divide(lhs, rhs); 

                }

                template <class T1, class T2>
                auto and_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->and_ops(lhs, rhs);

                }

                template <class T1>
                auto and_ops(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->and_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto or_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->or_ops(lhs, rhs);

                }

                template <class T1>
                auto or_ops(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->or_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto xor_ops(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->xor_ops(lhs, rhs);

                }

                template <class T1>
                auto xor_ops(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->xor_ops(lhs, rhs); 

                }

                template <class T1, class T2>
                auto mod(BigNumable<T1>& lhs, BigNumable<T2>& rhs){ // -> BigNumable<>&

                    return static_cast<T*>(this)->mod(lhs, rhs);

                }

                template <class T1>
                auto mod(BigNumable<T1>& lhs, size_t rhs){ // -> BigNumable<>&

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

                auto cast(size_t data){ // -> BigNumable<>&

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

    class RPadIndexConverter{

        public:

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

    class BigNumBoolVectorView: public datastructure::UnalignedViewableBoolVector<BigNumBoolVectorView>, public RPadIndexConverter{

        private:

            memory::sizet_linear::StandardBoolVectorView data;

        public:

            BigNumBoolVectorView() = default;

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

    class BigNumBoolVectorOp: public datastructure::UnalignedOperatableBoolVector<BigNumBoolVectorOp>, public RPadIndexConverter{
        
        private:

            memory::sizet_linear::StandardBoolVectorOperator data;

        public:

            BigNumBoolVectorOp() = default;
            
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

        class FullBoolVectorViewCaster: public BoolVectorViewCastable<FullBoolVectorViewCaster>{

            public:

                template <class T>
                BigNumBoolVectorView cast(memory::sizet_linear::VectorReadable<T>& data){

                    return BigNumBoolVectorView(data); 

                }

        };

        template <class T>
        class TrueBoolVectorViewCaster: public BoolVectorViewCastable<TrueBoolVectorViewCaster<T>>{
            
            private:

                FullBoolVectorViewCaster fullview_caster;
                std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer;

            public:

                TrueBoolVectorViewCaster(std::shared_ptr<utility::boolvector::StableBoolVectorViewTrimmable<T>> trimmer){

                    this->trimmer = trimmer;

                }
                
                template <class T1>
                auto cast(memory::sizet_linear::VectorReadable<T1>& data){

                    auto rs = fullview_caster.cast(data);
                    
                    return this->trimmer->trim(rs);;

                }

        };

        class FullBoolVectorOperatorCaster: public BoolVectorOpCastable<FullBoolVectorOperatorCaster>{

            public:

                template <class T>
                auto cast(memory::sizet_linear::OperatableVector<T>& data){

                    return BigNumBoolVectorOp(data);

                }

        };

        template <class T>
        class TrueBoolVectorOperatorCaster: public BoolVectorOpCastable<TrueBoolVectorOperatorCaster<T>>{

            private:

                std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer;
                FullBoolVectorOperatorCaster full_op_caster; 

            public:

                TrueBoolVectorOperatorCaster(std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer){

                    this->trimmer = trimmer;

                }

                template <class T1>
                auto cast(memory::sizet_linear::OperatableVector<T1>& data){

                    auto casted = full_op_caster.cast(data);

                    return this->trimmer->trim(casted);

                }

        };

        template <class T1, class T2, class T3>
        class SizeTLinearNoPaddingCaster: public SizeTLinearCastable<SizeTLinearNoPaddingCaster<T1, T2, T3>>{
            
            private:
                
                std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T1>> readable_gen;
                std::shared_ptr<memory::sizet_linear::BitIteratorGeneratable<T2>> iter_gen;
                std::shared_ptr<memory::sizet_linear::AddableVectorGeneratable<T3>> addable_vec_gen;

            public:

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
                    
                    return std::make_shared<TrueBoolVectorViewCaster<T>>(trimmer);

                }

                template <class T>
                auto init_true_op_caster(std::shared_ptr<utility::boolvector::StableBoolVectorOperatorTrimmable<T>> trimmer){

                    return std::make_shared<TrueBoolVectorOperatorCaster<T>>(trimmer);

                }
                
            public:

                auto get_full_view_caster(){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    return std::make_shared<FullBoolVectorViewCaster>(); 

                }

                auto get_true_view_caster(){ // -> shared_ptr<BoolVectorViewCastable<>>
                    
                    auto r_trimmer = utility::boolvector::StandardGenerator().get_right_zero_view_stable_trimmer();
                    auto casted_trimmer = r_trimmer->to_stable_boolvector_view_trimmable_sp(r_trimmer);

                    return this->init_true_view_caster(casted_trimmer);

                }

                auto get_full_opv_caster(){ // -> shared_ptr<BoolVectorOpCastable<>>

                    return std::make_shared<FullBoolVectorOperatorCaster>();

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
                    
                    return std::make_shared<SizeTLinearNoPaddingCaster<T,T1, T3>>(readable_gen, iter_gen, addable_vec_gen);

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

    }

}

namespace bignum::vector::comparer{

    class BackwardComparer: public Comparable<BackwardComparer>{

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

                return std::make_shared<BackwardComparer>();

            }

    };

}

namespace bignum::vector::operation_utility{

    class VectorLengthRightTrimmer{

        protected:

            template <class T>
            size_t get_size(memory::sizet_linear::VectorReadable<T>& data){

                size_t true_length = data.length();

                while (true_length != 0 && data.get(true_length - 1) == 0){

                    --true_length;

                }

                return true_length;

            }

    };

    class VectorViewTrimmer: public VectorLengthRightTrimmer, public VectorViewTrimmable<VectorViewTrimmer>{

        public:

            template <class T>
            auto trim(memory::sizet_linear::VectorReadable<T>& data){

                return memory::sizet_linear::StandardVectorView((size_t *) data.get_data(), this->get_size(data));

            } 

    };

    class LegalBitLengthRetriever: public BitLengthRetrievable<LegalBitLengthRetriever>{

        public:

            template <class T>
            size_t get_bit_length(memory::sizet_linear::VectorReadable<T>& data){

                size_t data_length = data.length();

                if (data_length == 0){

                    return 0;

                }

                return (data_length - 1) * BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(data.get(data_length - 1)) + 1; 

            }

    };

    class BitLengthRetriever: public BitLengthRetrievable<LegalBitLengthRetriever>, public VectorLengthRightTrimmer{

        public:

            template <class T>
            size_t get_bit_length(memory::sizet_linear::VectorReadable<T>& data){

                size_t data_sz = this->get_size(data);

                if (data_sz == 0){

                    return 0;

                } 

                return (data_sz - 1) * BIT_LENGTH_PER_SLOT + algorithm::log2::log2_64(data.get(data_sz - 1)) + 1;

            }

    };

    class LegalTrueBoolVectorViewCaster: public caster::BoolVectorViewCastable<LegalTrueBoolVectorViewCaster>{

        private:

            caster::FullBoolVectorViewCaster full_view_caster;
            LegalBitLengthRetriever retriever; 

        public:

            template <class T>
            auto cast(memory::sizet_linear::VectorReadable<T>& data){

                auto casted = full_view_caster.cast(data);

                return datastructure::UnalignedOffsetBoolVectorViewGenerator().get(casted, 0, retriever.get_bit_length(data)); 

            }

            
    };

    class FastTrailingZeroCounter: public TrailingZeroCountable<FastTrailingZeroCounter>, public VectorLengthRightTrimmer{

        public:

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

    class ZeroDetector: public ZeroDetectable<ZeroDetector>, public VectorLengthRightTrimmer{
        
        public:

            template <class T>
            bool is_zero(memory::sizet_linear::VectorReadable<T>& data){
                
                return this->get_size(data) == 0;

            }

    };

    class ZeroGenerator: public ZeroGeneratable<ZeroGenerator>{
        
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

    class DefaultGuardedVectorRightViewTrimmer: public VectorViewTrimmable<DefaultGuardedVectorRightViewTrimmer>{

        private:

            ZeroGenerator zero_gen;
            VectorViewTrimmer trimmer;

        public:

            template <class T>
            auto trim(memory::sizet_linear::VectorReadable<T>& data){

                auto rs = trimmer.trim(data); 

                if (rs.length() == 0){
                                        
                    return this->zero_gen.get();

                }

                return rs;

            }


    };

    class StandardSplitter: public VectorViewSplittable<StandardSplitter>{

        private:

            DefaultGuardedVectorRightViewTrimmer trimmer;
            memory::sizet_linear::VectorReadableSplitter splitter;

        public:

            template<class T>
            auto split(memory::sizet_linear::VectorReadable<T>& data, size_t split_point){
                
                auto rs = this->splitter.split(data, split_point);

                rs.first = this->trimmer.trim(rs.first);
                rs.second = this->trimmer.trim(rs.second); 
                
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
    
    template <class T>
    class BitIteratorGenerator: public BitIteratorGeneratable<BitIteratorGenerator<T>>, private RPadIndexConverter{

        private:

            std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen;

        public:

            BitIteratorGenerator(std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen){

                this->iter_gen = iter_gen;

            }

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

    template <class T>
    class BitIterWriterGenerator: public BitIterWriterGeneratable<BitIterWriterGenerator<T>>, private RPadIndexConverter{

        private:

            std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_gen;

        public:

            BitIterWriterGenerator(std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_gen){

                this->iter_gen = iter_gen;

            }
            
            template <class T1>
            auto get(memory::sizet_linear::OperatableVector<T1>& data, size_t write_length, size_t offset){
                
                offset = RPadIndexConverter::get_actual_idx(offset);
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

            ReverseBitIterator() = default;

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

    template <class T>
    class ReverseBitIteratorGenerator: public BitIteratorGeneratable<ReverseBitIteratorGenerator<T>>{

        private:

            std::shared_ptr<BitIteratorGeneratable<T>> iter_gen; 

        public:

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

    template <class T, class T1>
    class Discretizer: public Discretizable<Discretizer<T, T1>>{

        private:

            std::shared_ptr<BitIteratorGeneratable<T>> iter_gen;
            std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen;
            BitLengthRetriever length_retriever;

        public:

            Discretizer(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen,
                        std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen){
                
                this->iter_gen = iter_gen;
                this->opv_gen = opv_gen;

            }

            template <class T2>
            size_t get_instantiation_cost(memory::sizet_linear::VectorReadable<T2>& data, size_t bit_width){

                assert(bit_width <= ACTUAL_BIT_LENGTH_PER_SLOT);
     
                return this->get_sz(length_retriever.get_bit_length(data), bit_width) * sizeof(size_t);

            }
            
            template <class T2>
            auto discretize(memory::sizet_linear::VectorReadable<T2>& data, size_t bit_width, void * plm_space){

                assert(bit_width <= ACTUAL_BIT_LENGTH_PER_SLOT);
                
                size_t discretized_sz = this->get_sz(length_retriever.get_bit_length(data), bit_width);
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

    class PlusEstimator: public Estimatable<PlusEstimator>{

        public:

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return std::max(lhs.length(), rhs.length()) + 1;

            }

    };

    class PairWiseEstimator: public Estimatable<PairWiseEstimator>{

        public:

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return std::max(lhs.length(), rhs.length());

            }

    };

    class MinusEstimator: public Estimatable<MinusEstimator>{

        public:

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length(); 

            }

    };

    class RShiftEstimator: public ShiftEstimatable<RShiftEstimator>{

        public:

            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                return lhs.length();

            }

    };
    
    class LShiftEstimator: public ShiftEstimatable<LShiftEstimator>{

        public:

            template <class T>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){
                
                size_t sz = rhs / BIT_LENGTH_PER_SLOT + 1; 

                return lhs.length() + sz; 

            }

    };

    class MultiplyEstimator: public Estimatable<MultiplyEstimator>{

        public:

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length() + rhs.length();

            }

    };

    class DivideEstimator: public Estimatable<DivideEstimator>{

        public:

            template <class T, class T1>
            size_t get(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length();

            }

    };

    class LightningPlusJudge: public Judgable<LightningPlusJudge>{
        
        private:

            size_t threshold; 

        public:

            LightningPlusJudge(size_t max_val_per_slot){

                this->threshold = max_val_per_slot >> 1;

            }

            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){
                                
                if (lhs.length() == rhs.length()){ 
                    
                    return (rhs.get(rhs.length() - 1) < lhs.get(lhs.length() - 1)) && (lhs.get(lhs.length() - 1) <= this->threshold); 

                }

                if ((lhs.length() > rhs.length()) && (lhs.get(lhs.length() - 1) <= this->threshold)){

                    return true;

                }

                return false;

            }

    };

    class QuickMinusJudge: public Judgable<QuickMinusJudge>{

        private:

            LegalTrueBoolVectorViewCaster true_view_caster;

            uint8_t WINDOW_SIZE = 5;
        
        public:

            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                auto casted_lhs = this->true_view_caster.cast(lhs);
                auto casted_rhs = this->true_view_caster.cast(rhs);

                return this->is_post_same_length(casted_lhs, casted_rhs); 

            }
        
        private:

            template <class T, class T1>
            bool is_post_same_length(datastructure::UnalignedViewableBoolVector<T> lhs, datastructure::UnalignedViewableBoolVector<T1> rhs){
                
                if (lhs.length() == 0){

                    return false;

                }

                size_t ptr = rhs.length();

                while ((ptr < (lhs.length() - 1)) && ((ptr - rhs.length()) < WINDOW_SIZE)){

                    if (lhs.get(ptr)){

                        return true;
                        
                    }

                    ++ptr;

                }

                return false;

            }


    };

    class RightShiftJudge: public ShiftJudgable<RightShiftJudge>, public RPadIndexConverter{

        private:

            LegalBitLengthRetriever length_retriever;
        
        public:

            template<class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                size_t pre_length = this->length_retriever.get_bit_length(lhs);
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

    class LeftShiftJudge: public ShiftJudgable<LeftShiftJudge>, public RPadIndexConverter{

        private:

            LegalBitLengthRetriever length_retriever;
        
        public:

            template <class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){
                
                size_t pre_length = this->length_retriever.get_bit_length(lhs);

                if (pre_length == 0){

                    return true;

                }

                size_t post_length = pre_length + rhs;

                return this->get_slot(pre_length - 1) == this->get_slot(post_length - 1); 

            }

    };

    class ImmutableJudge: public ShiftJudgable<ImmutableJudge>{

        public:

            template <class T>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, size_t rhs){

                return false;

            }

    };

    class CustomPairWiseJudge: public Judgable<CustomPairWiseJudge>{
        
        public:

            template <class T, class T1>
            bool fit(memory::sizet_linear::VectorReadable<T>& lhs, memory::sizet_linear::VectorReadable<T1>& rhs){

                return lhs.length() >= rhs.length();

            }


    };

    class GeneralUltility: public GeneralUtilizable<GeneralUltility>{

        public:

            size_t get_borrow(){

                return MAX_VAL_PER_SLOT + 1;

            }

            size_t get_max_per_slot(){

                return MAX_VAL_PER_SLOT;

            }

            size_t prune(size_t data){

                return data & MAX_VAL_PER_SLOT; 

            }

            size_t get_overflow(size_t data){

                return data >> BIT_LENGTH_PER_SLOT;

            }

    };

    class StandardOperationUtilityGenerator{

        private:

            template <class T>
            auto init_reverse_bit_iter_gen(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen){

                return std::make_shared<ReverseBitIteratorGenerator<T>>(iter_gen);

            }

            template <class T>
            auto init_bit_writer_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIterWriterGeneratable<T>> iter_writer){
                
                return std::make_shared<BitIterWriterGenerator<T>>(iter_writer); 

            }

            template <class T>
            auto init_bit_iter_gen(std::shared_ptr<memory::sizet_linear::DynamicBitIteratorGeneratable<T>> iter_gen){

                return std::make_shared<BitIteratorGenerator<T>>(iter_gen);

            }

            template <class T, class T1>
            auto init_discretizer(std::shared_ptr<BitIteratorGeneratable<T>> iter_gen,
                                  std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable<T1>> opv_gen){
                
                return std::make_shared<Discretizer<T, T1>>(iter_gen, opv_gen);

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

                return std::make_shared<VectorViewTrimmer>();

            }

            auto get_default_guarded_vector_right_view_trimmer(){ // -> shared_ptr<VectorViewTrimmable<>>

                return std::make_shared<DefaultGuardedVectorRightViewTrimmer>();

            }

            auto get_bit_length_retriever(){ // -> shared_ptr<BitLengthRetrievable<>> 

                return std::make_shared<BitLengthRetriever>();

            }

            auto get_legal_bit_length_retriever(){ // -> shared_ptr<BitLengthRetrievable<>>

                return std::make_shared<LegalBitLengthRetriever>();

            }

            auto get_legal_true_boolvector_view_caster(){ // -> shared_ptr<caster::BoolVectorViewCastable<>>

                return std::make_shared<LegalTrueBoolVectorViewCaster>();
                
            } 

            auto get_trailing_zero_counter(){ // -> shared_ptr<TrailingZeroCountable<>>

                return std::make_shared<FastTrailingZeroCounter>();

            }

            auto get_zero_detector(){ // -> shared_ptr<ZeroDetectable<>>

                return std::make_shared<ZeroDetector>();

            }

            auto get_zero_generator(){ // -> shared_ptr<ZeroGeneratable<>>

                return std::make_shared<ZeroGenerator>();

            }

            auto get_zero_default_splitter(){ // -> shared_ptr<VectorViewSplittable<>>

                return std::make_shared<StandardSplitter>(); 

            }

            auto get_plus_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PlusEstimator>();

            }

            auto get_pairwise_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<PairWiseEstimator>();

            }

            auto get_minus_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MinusEstimator>(); 

            }

            auto get_rshift_estimator(){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<RShiftEstimator>();

            }

            auto get_lshift_estimator(){ // -> shared_ptr<ShiftEstimatable<>>

                return std::make_shared<LShiftEstimator>();

            }   

            auto get_multiply_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<MultiplyEstimator>(); 

            }

            auto get_divide_estimator(){ // -> shared_ptr<Estimatable<>>

                return std::make_shared<DivideEstimator>();

            }

            auto get_general_utility(){ // -> shared_ptr<GeneralUtilizable<>>

                return std::make_shared<GeneralUltility>();

            }
            
            auto get_lightning_plus_judge(){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<LightningPlusJudge>(MAX_VAL_PER_SLOT);

            }

            auto get_minus_judge(){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<QuickMinusJudge>();

            }

            auto get_custom_pairwise_judge(){ // -> shared_ptr<Jugdable<>> 

                return std::make_shared<CustomPairWiseJudge>();

            }

            auto get_rshift_judge(){ // -> shared_ptr<Jugdable<>>
                
                return std::make_shared<RightShiftJudge>();

            }

            auto get_lshift_judge(){ // -> shared_ptr<Jugdable<>>

                return std::make_shared<LeftShiftJudge>();

            }

            auto get_immutable_judge(){ // -> shared_ptr<Judgable<>>
                 
                 return std::make_shared<ImmutableJudge>();

            }

    };

}  

namespace bignum::vector::mutable_operation{
    
    class AssignOperator: public Operatable<AssignOperator>{

        public:
            
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

    template <class T>
    class PlusOperator: public Operatable<PlusOperator<T>>{

        private:

            std::shared_ptr<vector::operation_utility::GeneralUtilizable<T>> utility;

        public:

            PlusOperator(std::shared_ptr<vector::operation_utility::GeneralUtilizable<T>> utility){

                this->utility = utility;

            }

            template <class T1, class T2>
            void ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                size_t total = 0;
                size_t i = 0;

                for (i = 0; i < rhs.length(); ++i){

                    total += lhs.get(i) + rhs.get(i);
                    lhs.set(i, this->utility->prune(total));
                    total = this->utility->get_overflow(total); 

                }

                while (total){
                    
                    total += lhs.get(i);
                    lhs.set(i, this->utility->prune(total));
                    total = this->utility->get_overflow(total);

                    ++i;

                }


            }

    };

    template <class T>
    class MinusOperator: public Operatable<MinusOperator<T>>{

        private:

            std::shared_ptr<operation_utility::GeneralUtilizable<T>>  utility;

        public:

            MinusOperator(std::shared_ptr<operation_utility::GeneralUtilizable<T>> utility){

                this->utility = utility;

            }

            template <class T1, class T2>
            void ops(memory::sizet_linear::OperatableVector<T1>& lhs, memory::sizet_linear::VectorReadable<T2>& rhs){

                assert(BIT_LENGTH_PER_SLOT + 1 < ACTUAL_BIT_LENGTH_PER_SLOT);

                intmax_t rs = 0;
                size_t i = 0;
                size_t borrow = 0;

                for (i = 0; i < rhs.length(); ++i){

                    rs = (intmax_t) lhs.get(i) - (rhs.get(i) + borrow);
                    borrow = 0;

                    while (rs < 0){

                        rs += this->utility->get_borrow();
                        borrow += 1;

                    }

                    lhs.set(i, rs);

                }

                while (borrow){

                    rs = (intmax_t) lhs.get(i) - borrow; 
                    borrow = 0;

                    while (rs < 0){

                        rs += this->utility->get_borrow();
                        borrow += 1;

                    }

                    lhs.set(i, rs);
                    ++i;

                }

            }

    };

    template <class T, class T1, class T2>
    class RShiftOperator: public ShiftOperatable<RShiftOperator<T, T1, T2>>{

        private:

            std::shared_ptr<operation_utility::BitIteratorGeneratable<T>> read_iter_gen;
            std::shared_ptr<operation_utility::BitIterWriterGeneratable<T1>> write_iter_gen;
            std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever;  

        public:

            RShiftOperator(std::shared_ptr<operation_utility::BitIteratorGeneratable<T>> read_iter_gen,
                           std::shared_ptr<operation_utility::BitIterWriterGeneratable<T1>> write_iter_gen,
                           std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever){
                
                this->read_iter_gen = read_iter_gen;
                this->write_iter_gen = write_iter_gen;
                this->bit_length_retriever = bit_length_retriever;

            }

            template <class T3>
            void ops(memory::sizet_linear::OperatableVector<T3>& lhs, size_t rhs){
                
                size_t pre_length = this->bit_length_retriever->get_bit_length(lhs);
                size_t post_length = (pre_length > rhs) ? pre_length - rhs : 0; 
                size_t read_length = BIT_LENGTH_PER_SLOT;
                size_t data = 0;
                
                auto write_iter = this->write_iter_gen->get(lhs, read_length, 0);

                if (post_length != 0){
                    
                    auto read_iter = this->read_iter_gen->get(lhs, read_length, rhs);

                    for (size_t i = 0; i < post_length; i += read_length){
                        
                        assert(read_iter.next(data));
                        assert(write_iter.write(data));

                    }        

                }

                while (write_iter.write(0));

            }

    };

    class PairWiseOperator: public PairWiseOperatable<PairWiseOperator>{

        public:

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

    class StandardArithmeticOperatorGenerator{

        private:
        
            template <class T>
            auto init_plus_ops(std::shared_ptr<operation_utility::GeneralUtilizable<T>> utility){

                return std::make_shared<PlusOperator<T>>(utility); 

            }

            template <class T>
            auto init_minus_ops(std::shared_ptr<operation_utility::GeneralUtilizable<T>> utility){

                return std::make_shared<MinusOperator<T>>(utility); 

            }

            template <class T, class T1, class T2>
            auto init_rshift_ops(std::shared_ptr<operation_utility::BitIteratorGeneratable<T>> read_iter_gen,
                                 std::shared_ptr<operation_utility::BitIterWriterGeneratable<T1>> write_iter_gen,
                                 std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever){
                
                return std::make_shared<RShiftOperator<T, T1, T2>>(read_iter_gen, write_iter_gen, bit_length_retriever); 

            }

        public:
            
            auto get_assigner(){ // -> shared_ptr<Operatable<>>

                return std::make_shared<AssignOperator>(); 

            }

            auto get_plus(){ // -> shared_ptr<Operatable<>>
                
                auto general_utility = operation_utility::StandardOperationUtilityGenerator().get_general_utility(); 
                auto casted_utility = general_utility->to_general_utilizable_sp(general_utility); 

                return this->init_plus_ops(casted_utility);

            } 

            auto get_minus(){ // -> shared_ptr<Operatable<>>

                auto general_utility = operation_utility::StandardOperationUtilityGenerator().get_general_utility(); 
                auto casted_utility = general_utility->to_general_utilizable_sp(general_utility);
                
                return this->init_minus_ops(casted_utility);

            }

            auto get_rshift(){ // -> shared_ptr<ShiftOperatable<>>
            
                auto read_iter_gen = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_gen();
                auto write_iter_gen = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_replace_writer_gen();
                auto bit_length_retriever = operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();

                auto casted_read_iter_gen = read_iter_gen->to_bit_iterator_generatable_sp(read_iter_gen);
                auto casted_write_iter_gen = write_iter_gen->to_bit_iter_writer_generatable_sp(write_iter_gen);
                auto casted_bit_length_retriever = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);

                return this->init_rshift_ops(casted_read_iter_gen, casted_write_iter_gen, casted_bit_length_retriever);

            }

            auto get_lshift(){

                return nullptr;
                
            }

            auto get_custom_pairwise(){ // -> shared_ptr<PairWiseOperatable<>>
               
                return std::make_shared<PairWiseOperator>();
            
            }

    };

}

namespace bignum::vector::operation{

    template <class T, class T1, class T2, class T3, class T4>
    class PairWiseOperator: public Operatable<PairWiseOperator<T, T1, T2, T3, T4>>{

        private:
            
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator; 
            std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator;
            std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator;
            std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer;
            std::shared_ptr<vector::mutable_operation::Operatable<T4>> mutable_operator;

        public:

            PairWiseOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                             std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator,
                             std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                             std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                             std::shared_ptr<vector::mutable_operation::Operatable<T4>> mutable_operator){
                
                this->allocator = allocator;
                this->estimator = estimator;
                this->assign_operator = assign_operator;
                this->view_trimmer = view_trimmer;
                this->mutable_operator = mutable_operator;

            }
            
            template <class T5, class T6>
            auto ops(memory::sizet_linear::VectorReadable<T5>& lhs, memory::sizet_linear::VectorReadable<T6>& rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();
                
                auto rs = this->allocator->vec_get(this->estimator->get(lhs, rhs)); 
                
                this->assign_operator->ops(rs, lhs);
                this->mutable_operator->ops(rs, rhs);

                auto trimmed_rs = this->view_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }

    };

    template <class T, class T1, class T2, class T3, class T4>
    class ShiftOperator: public ShiftOperatable<ShiftOperator<T, T1, T2, T3, T4>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<vector::operation_utility::ShiftEstimatable<T1>> estimator;
            std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator;
            std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer;
            std::shared_ptr<vector::mutable_operation::ShiftOperatable<T4>> mutable_operator;
        
        public:

            ShiftOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                          std::shared_ptr<vector::operation_utility::ShiftEstimatable<T1>> estimator,
                          std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                          std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                          std::shared_ptr<vector::mutable_operation::ShiftOperatable<T4>> mutable_operator){
                        
                this->allocator = allocator;
                this->estimator = estimator;
                this->assign_operator = assign_operator;
                this->view_trimmer = view_trimmer;
                this->mutable_operator = mutable_operator;

            }

            template <class T5>
            auto ops(memory::sizet_linear::VectorReadable<T5>& lhs, size_t rhs){ // -> VectorReadable<>&
                
                this->allocator->enter_scope();
                
                auto rs = this->allocator->vec_get(this->estimator->get(lhs, rhs)); 
                
                this->assign_operator->ops(rs, lhs);
                this->mutable_operator->ops(rs, rhs);

                auto trimmed_rs = this->view_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }

    };

    template <class T, class T1, class T2, class T3, class T4>
    class LShiftOperator: public ShiftOperatable<LShiftOperator<T, T1, T2, T3, T4>>{

        private:
            
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::ShiftEstimatable<T1>> estimator;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> view_trimmer;
            std::shared_ptr<operation_utility::BitIteratorGeneratable<T3>> read_iter_gen;
            std::shared_ptr<operation_utility::BitIterWriterGeneratable<T4>> write_iter_gen;
            size_t write_delta; 

        public:

            LShiftOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>>  allocator,
                           std::shared_ptr<operation_utility::ShiftEstimatable<T1>> estimator,
                           std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> view_trimmer,
                           std::shared_ptr<operation_utility::BitIteratorGeneratable<T3>> read_iter_gen,
                           std::shared_ptr<operation_utility::BitIterWriterGeneratable<T4>> write_iter_gen,
                           size_t write_delta){
                
                this->allocator = allocator;
                this->estimator = estimator;
                this->view_trimmer = view_trimmer;
                this->read_iter_gen = read_iter_gen;
                this->write_iter_gen = write_iter_gen;
                this->write_delta = write_delta;

            }

            template <class T5>
            auto ops(memory::sizet_linear::VectorReadable<T5>& lhs, size_t rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();
                
                auto rs = this->allocator->vec_get_default(this->estimator->get(lhs, rhs));
                auto write_iter = this->write_iter_gen->get(rs, this->write_delta, rhs);
                auto read_iter = this->read_iter_gen->get(lhs, this->write_delta);
                size_t cur_read = 0;

                while (read_iter.next(cur_read)){
                    
                    assert(write_iter.write(cur_read));

                } 

                auto trimmed_rs = this->view_trimmer->trim(rs); 
                
                return this->allocator->exit_scope(trimmed_rs);

            }

    };

    template <class T, class T1, class T2, class T3, class T4>
    class CustomPairWiseOperator: public PairWiseOperatable<CustomPairWiseOperator<T, T1, T2, T3, T4>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator;
            std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator;
            std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>>  view_trimmer;
            std::shared_ptr<vector::mutable_operation::PairWiseOperatable<T4>> pairwise_operator;

        public:

            CustomPairWiseOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                   std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator,
                                   std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                                   std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                                   std::shared_ptr<vector::mutable_operation::PairWiseOperatable<T4>> pairwise_operator){
                
                this->allocator = allocator;
                this->estimator = estimator;
                this->assign_operator = assign_operator;
                this->view_trimmer = view_trimmer;
                this->pairwise_operator = pairwise_operator;

            }

            template <class T5, class T6>
            auto custom_ops(memory::sizet_linear::VectorReadable<T5>& lhs, memory::sizet_linear::VectorReadable<T6>& rhs, pairwise_ops ops){ // -> VectorReadable&
                
                this->allocator->enter_scope();

                auto rs = this->allocator->vec_get(this->estimator->get(lhs, rhs));
                
                this->assign_operator->ops(rs, lhs);
                this->pairwise_operator->custom_ops(rs, rhs, ops);

                auto trimmed_view = this->view_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_view);

            }   

    };

    template <class T, class T1, class T2, class T3, class T4, class T5>
    class PropagatedBucketMultiplicationOperator: public Operatable<PropagatedBucketMultiplicationOperator<T, T1, T2, T3, T4, T5>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::Discretizable<T1>> discretizer;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> result_trimmer;
            std::shared_ptr<operation_utility::Estimatable<T3>> estimator;
            std::shared_ptr<operation_utility::BitIteratorGeneratable<T4>> read_iter_gen;
            std::shared_ptr<operation_utility::BitIterWriterGeneratable<T5>> write_iter_gen;
            size_t read_sz; 

        public:

            PropagatedBucketMultiplicationOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                   std::shared_ptr<operation_utility::Discretizable<T1>> discretizer,
                                                   std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> result_trimmer,
                                                   std::shared_ptr<operation_utility::Estimatable<T3>> estimator,
                                                   std::shared_ptr<operation_utility::BitIteratorGeneratable<T4>> read_iter_gen,
                                                   std::shared_ptr<operation_utility::BitIterWriterGeneratable<T5>> write_iter_gen,
                                                   size_t read_sz){
                
                assert(read_sz + read_sz + 1 <= ACTUAL_BIT_LENGTH_PER_SLOT);

                this->allocator = allocator;
                this->discretizer = discretizer;
                this->result_trimmer = result_trimmer;
                this->read_sz = read_sz;
                this->estimator = estimator;
                this->read_iter_gen = read_iter_gen;
                this->write_iter_gen = write_iter_gen;

            }
            
            template <class T6, class T7>
            auto ops(memory::sizet_linear::VectorReadable<T6>& lhs, memory::sizet_linear::VectorReadable<T7>& rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();

                auto rs = this->allocator->vec_get_default(this->estimator->get(lhs, rhs));
                
                void * plm_space = this->allocator->get(this->discretizer->get_instantiation_cost(lhs, this->read_sz));
                auto discretized_lhs = this->discretizer->discretize(lhs, this->read_sz, plm_space);

                plm_space = this->allocator->get(this->discretizer->get_instantiation_cost(rhs, this->read_sz));
                auto discretized_rhs = this->discretizer->discretize(rhs, this->read_sz, plm_space);
                
                const size_t MAX_VAL_PER_BUCKET = ((size_t) 1 << this->read_sz) - 1;
                size_t combined_length = discretized_lhs.length() + discretized_rhs.length();
                auto combined = this->allocator->vec_get_default(combined_length);  
                
                for (size_t i = 0; i < discretized_lhs.length(); ++i){

                    for (size_t j = 0; j < discretized_rhs.length(); ++j){
                        
                        size_t offset = i + j;
                        size_t mul = discretized_lhs.get(i) * discretized_rhs.get(j); 
                                                
                        combined.set(offset, combined.get(offset) + mul);

                        while (combined.get(offset) > MAX_VAL_PER_BUCKET){

                            size_t overflow = combined.get(offset) >> this->read_sz;
                            size_t pruned = combined.get(offset) & MAX_VAL_PER_BUCKET;

                            combined.set(offset, pruned);

                            ++offset;
                            combined.set(offset, combined.get(offset) + overflow);

                        }
                        
                    }

                }

                auto trimmed_combined = this->result_trimmer->trim(combined);
                auto read_iter = this->read_iter_gen->get_custom_width(trimmed_combined, this->read_sz, this->read_sz);
                auto write_iter = this->write_iter_gen->get(rs, this->read_sz, 0);
                size_t cur = 0;

                while (read_iter.next(cur)){
                                        
                    assert(write_iter.write(cur));

                }

                auto trimmed_rs = this->result_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }


    };

    template <class T, class T1, class T2>
    class NaiveMultiplication: public Operatable<NaiveMultiplication<T, T1, T2>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer;
            std::shared_ptr<operation_utility::Estimatable<T2>> estimator;

        public:

            NaiveMultiplication(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                this->allocator = allocator;
                this->result_trimmer = result_trimmer;
                this->estimator = estimator;                               
            }
            
            template <class T4, class T5>
            auto ops(memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();

                size_t n = this->estimator->get(lhs, rhs);
                auto rs = this->allocator->vec_get_default(n);
                size_t carry = 0;
                size_t z = 0;

                for (size_t i = 0; i < lhs.length(); ++i){

                    z = i;

                    for (size_t j = 0; j < rhs.length(); ++j){
                        
                        carry += lhs.get(i) * rhs.get(j) + rs.get(z); 
                        rs.set(z, carry & MAX_VAL_PER_SLOT);
                        carry >>= BIT_LENGTH_PER_SLOT;

                        ++z;

                    }

                    if (carry != 0){

                        rs.set(z, carry);
                        carry = 0;

                    }

                } 

                auto trimmed_rs = this->result_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }

    };

    template <class T, class T1, class T2>
    class SixtyTwoBitBucketMultiplicationOperator: public Operatable<SixtyTwoBitBucketMultiplicationOperator<T, T1, T2>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer;
            std::shared_ptr<operation_utility::Estimatable<T2>> estimator;
        public:

            SixtyTwoBitBucketMultiplicationOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                    std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                                    std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                this->allocator = allocator;
                this->result_trimmer = result_trimmer;
                this->estimator = estimator;
                               
            }
            
            template <class T4, class T5>
            auto ops(memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();

                size_t n = this->estimator->get(lhs, rhs);
                size_t flatten_n = n * this->LIMB_PER_SLOT;
                auto rs = this->allocator->vec_get_default(flatten_n);
                const size_t MAX_VAL_PER_LIMB = ((size_t) 1 << this->BIT_PER_LIMB) - 1;
                size_t carry = 0;
                size_t idx = 0; 
                size_t multiplier = 0; 

                for (size_t i = 0; i < lhs.length(); ++i){

                    idx = i * this->LIMB_PER_SLOT; 
                    multiplier = lhs.get(i) & MAX_VAL_PER_LIMB;

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

                        rs.set(idx, carry);
                        carry = 0;

                    }

                    idx = i * this->LIMB_PER_SLOT + 1; 
                    multiplier = lhs.get(i) >> BIT_PER_LIMB;

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

                        rs.set(idx, carry);
                        carry = 0;

                    }
                    
                }

                for (size_t i = 0; i < n; ++i){

                    idx = i * this->LIMB_PER_SLOT;
                    rs.set(i, rs.get(idx) | (rs.get(idx + 1) << this->BIT_PER_LIMB));

                } 

                auto return_rs = memory::sizet_linear::StandardVectorView((size_t *) rs.get_data(), n);
                auto trimmed_rs = this->result_trimmer->trim(return_rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }

        private:

            const uint8_t BIT_PER_LIMB = 31;
            const uint8_t LIMB_PER_SLOT = 2;
            const size_t EXTRACTOR = ((size_t) 1 << BIT_PER_LIMB) - 1;   

    };

    template <class T, class T1, class T2>
    class ThirtyBitBucketMultiplicationOperator: public Operatable<ThirtyBitBucketMultiplicationOperator<T, T1, T2>>{

        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer;
            std::shared_ptr<operation_utility::Estimatable<T2>> estimator;
        public:

            ThirtyBitBucketMultiplicationOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                  std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                                  std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                this->allocator = allocator;
                this->result_trimmer = result_trimmer;
                this->estimator = estimator;
                               
            }
            
            template <class T4, class T5>
            auto ops(memory::sizet_linear::VectorReadable<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs){ // -> VectorReadable<>&

                this->allocator->enter_scope();

                size_t n = this->estimator->get(lhs, rhs);
                size_t flatten_n = n * this->LIMB_PER_SLOT;
                auto rs = this->allocator->vec_get_default(flatten_n);
                const size_t MAX_VAL_PER_LIMB = ((size_t) 1 << this->BIT_PER_LIMB) - 1;
                size_t carry = 0;
                size_t idx = 0; 
                size_t multiplier = 0; 

                for (size_t i = 0; i < lhs.length(); ++i){

                    idx = i * this->LIMB_PER_SLOT; 
                    multiplier = lhs.get(i) & MAX_VAL_PER_LIMB;

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

                        rs.set(idx, carry);
                        carry = 0;

                    }

                    idx = i * this->LIMB_PER_SLOT + 1; 
                    multiplier = lhs.get(i) >> BIT_PER_LIMB;

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

                        rs.set(idx, carry);
                        carry = 0;

                    }
                    
                }

                for (size_t i = 0; i < n; ++i){

                    idx = i *  this->LIMB_PER_SLOT;
                    rs.set(i, rs.get(idx) | (rs.get(idx + 1) << this->BIT_PER_LIMB));

                } 

                auto return_rs = memory::sizet_linear::StandardVectorView((size_t *) rs.get_data(), n);
                auto trimmed_rs = this->result_trimmer->trim(return_rs);

                return this->allocator->exit_scope(trimmed_rs); 

            }

        private:

            const uint8_t BIT_PER_LIMB = 15;
            const uint8_t LIMB_PER_SLOT = 2;
            const size_t EXTRACTOR = ((size_t) 1 << BIT_PER_LIMB) - 1;     

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6>
    class RecursiveKaratsubaMultiplicationOperator: public Operatable<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6>>{

        private:
            
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation_utility::VectorViewSplittable<T1>> splitter;
            std::shared_ptr<Operatable<T2>> plus_ops;
            std::shared_ptr<Operatable<T3>> minus_ops;
            std::shared_ptr<ShiftOperatable<T4>> lshifter;
            std::shared_ptr<Operatable<T5>> multiplier;
            std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T6>> caster;

            const uint8_t NAIVE_MUL_THRESHOLD = 50;            

        public:

            RecursiveKaratsubaMultiplicationOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                                     std::shared_ptr<operation_utility::VectorViewSplittable<T1>> splitter,
                                                     std::shared_ptr<Operatable<T2>> plus_ops,
                                                     std::shared_ptr<Operatable<T3>> minus_ops,
                                                     std::shared_ptr<ShiftOperatable<T4>> lshifter,
                                                     std::shared_ptr<Operatable<T5>> multiplier,
                                                     std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T6>> caster){
                
                this->allocator = allocator;
                this->splitter = splitter;
                this->plus_ops = plus_ops;
                this->minus_ops = minus_ops;
                this->lshifter = lshifter;
                this->multiplier = multiplier;
                this->caster = caster;

            }

            template <class T8, class T9>
            auto ops(memory::sizet_linear::VectorReadable<T8>& lhs, memory::sizet_linear::VectorReadable<T9>& rhs) -> decltype(this->caster->from_ptr(nullptr, 0)){ // -> VectorReadable<T>&

                if (lhs.length() < rhs.length()){

                    return this->ops(rhs, lhs);
                    
                }

                if (rhs.length() <= NAIVE_MUL_THRESHOLD){
                    
                    auto rs = this->multiplier->ops(lhs, rhs);

                    return this->caster->from_vec_readable(rs); 

                }

                this->allocator->enter_scope();

                size_t split_point = lhs.length() >> 1;                 

                auto lhs_splitted = this->splitter->split(lhs, split_point);
                auto rhs_splitted = this->splitter->split(rhs, split_point);

                size_t lower_bit_length = split_point * BIT_LENGTH_PER_SLOT;
                
                auto p1 = this->plus_ops->ops(lhs_splitted.first, lhs_splitted.second);
                auto p2 = this->plus_ops->ops(rhs_splitted.first, rhs_splitted.second);

                auto z1 = this->ops(lhs_splitted.first, rhs_splitted.first);
                auto z2 = this->ops(lhs_splitted.second, rhs_splitted.second);
                auto z3 = this->ops(p1, p2);    

                auto p3 = this->plus_ops->ops(z1, z2);
                auto z4 = this->minus_ops->ops(z3, p3);

                auto s1 = this->lshifter->ops(z2, lower_bit_length << 1);
                auto s2 = this->lshifter->ops(z4, lower_bit_length); 
                
                auto p4 = this->plus_ops->ops(z1, s1); 
                auto rs = this->plus_ops->ops(p4, s2); 

                auto exit_rs =  this->allocator->exit_scope(rs);

                return this->caster->from_vec_readable(exit_rs); 

            }

    };

    template <class T, class T1>
    class OptimizedMultiplicationOperator: public Operatable<OptimizedMultiplicationOperator<T, T1>>{

        private:

            std::shared_ptr<Operatable<T>> karatsuba;
            std::shared_ptr<Operatable<T1>> naive;

            static const uint8_t KARATSUBA_IDENTIFIER = 1;
            static const uint8_t NAIVE_IDENTIFIER = 2;

            const size_t NAIVE_THRESHOLD = 50;
            const size_t KARATSUBA_THRESHOLD = 1 << 15; 

        public:

            OptimizedMultiplicationOperator(std::shared_ptr<Operatable<T>> karatsuba,
                                            std::shared_ptr<Operatable<T1>> naive){
                
                this->karatsuba = karatsuba;
                this->naive = naive;

            }

            template <class T2, class T3>
            auto ops(memory::sizet_linear::VectorReadable<T2>& lhs, memory::sizet_linear::VectorReadable<T3>& rhs){ // -> VectorReadable<>&

                uint8_t identifer = this->get_identifier(lhs, rhs);

                switch (identifer){

                    case KARATSUBA_IDENTIFIER:

                        return this->karatsuba->ops(lhs, rhs);

                    case NAIVE_IDENTIFIER:

                        return this->naive->ops(lhs, rhs);
                    
                    default:

                        return this->naive->ops(lhs, rhs);

                }

            }

        private:

            template <class T2, class T3>
            uint8_t get_identifier(memory::sizet_linear::VectorReadable<T2>& lhs, memory::sizet_linear::VectorReadable<T3>& rhs){
                                
                if (std::min(lhs.length(), rhs.length()) < NAIVE_THRESHOLD){
                    
                    return NAIVE_IDENTIFIER; 

                }

                return KARATSUBA_IDENTIFIER; 

            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
    class IterativeModuloDivider: public Operatable<IterativeModuloDivider<T, T1, T2, T3, T4, T5, T6, T7, T8>>{
    
        private:

            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<Operatable<T1>> minus_ops; 
            std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever;
            std::shared_ptr<operation_utility::Estimatable<T3>> estimator;
            std::shared_ptr<operation_utility::ZeroDetectable<T4>> zero_detector;
            std::shared_ptr<comparer::Comparable<T5>> comparer;
            std::shared_ptr<operation_utility::VectorViewTrimmable<T6>> view_trimmer;
            std::shared_ptr<ShiftOperatable<T7>> lshifter;
            std::shared_ptr<caster::BoolVectorOpCastable<T8>> full_op_caster;

        public:

            IterativeModuloDivider(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                   std::shared_ptr<Operatable<T1>> minus_ops,
                                   std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever,
                                   std::shared_ptr<operation_utility::Estimatable<T3>> estimator,
                                   std::shared_ptr<operation_utility::ZeroDetectable<T4>> zero_detector,
                                   std::shared_ptr<comparer::Comparable<T5>> comparer,
                                   std::shared_ptr<operation_utility::VectorViewTrimmable<T6>> view_trimmer,
                                   std::shared_ptr<ShiftOperatable<T7>> lshifter,
                                   std::shared_ptr<caster::BoolVectorOpCastable<T8>> full_op_caster){
                
                this->allocator = allocator;
                this->minus_ops = minus_ops;
                this->bit_length_retriever = bit_length_retriever;
                this->estimator = estimator;
                this->zero_detector = zero_detector;
                this->lshifter = lshifter;
                this->view_trimmer = view_trimmer;
                this->comparer = comparer;
                this->full_op_caster = full_op_caster;

            }

            template <class T9, class T10>
            auto ops(memory::sizet_linear::VectorReadable<T9>& lhs, memory::sizet_linear::VectorReadable<T10>& rhs){ // -> VectorReadable&

                assert(!this->zero_detector->is_zero(rhs));

                this->allocator->enter_scope();

                auto rs = this->allocator->vec_get_default(this->estimator->get(lhs, rhs));
                auto casted_rs = this->full_op_caster->cast(rs); 
                size_t rhs_length = this->bit_length_retriever->get_bit_length(rhs); 

                this->divide(lhs, rhs, rhs_length, casted_rs);
                auto trimmed_rs = this->view_trimmer->trim(rs);

                return this->allocator->exit_scope(trimmed_rs);

            }

        private:

            template <class T9, class T10, class T11>
            void divide(memory::sizet_linear::VectorReadable<T9>& lhs, memory::sizet_linear::VectorReadable<T10>& rhs, size_t rhs_length, datastructure::UnalignedOperatableBoolVector<T11>& rs){

                size_t bit_dif = 0; 
                size_t lhs_length = this->bit_length_retriever->get_bit_length(lhs);

                if (lhs_length < rhs_length){

                    return;

                }
                
                if (lhs_length == rhs_length){

                    if (this->comparer->compare(lhs, rhs) >= 0){

                        this->bitwise_plus(rs, 0);

                    }

                    return;

                }

                bit_dif = lhs_length - rhs_length - 1;
                this->bitwise_plus(rs, bit_dif);

                auto shifted = this->lshifter->ops(rhs, bit_dif);
                auto remainder = this->minus_ops->ops(lhs, shifted);

                this->divide(remainder, rhs, rhs_length, rs);

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

    class StandardArithmeticOperatorGenerator{

        private:

            template <class T, class T1, class T2, class T3, class T4>
            std::shared_ptr<PairWiseOperator<T, T1, T2, T3, T4>> init_pairwise_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                        std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator,
                                        std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                                        std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                                        std::shared_ptr<vector::mutable_operation::Operatable<T4>> mutable_operator){
                
                return std::make_shared<PairWiseOperator<T, T1, T2, T3, T4>>(allocator, estimator, assign_operator, view_trimmer, mutable_operator); 

            }

            template <class T, class T1, class T2, class T3, class T4>
            std::shared_ptr<LShiftOperator<T, T1, T2, T3, T4>> init_lshift_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                      std::shared_ptr<operation_utility::ShiftEstimatable<T1>> estimator,
                                      std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> view_trimmer,
                                      std::shared_ptr<operation_utility::BitIteratorGeneratable<T3>> read_iter_gen,
                                      std::shared_ptr<operation_utility::BitIterWriterGeneratable<T4>> write_iter_gen,
                                      size_t write_delta){
                
                return std::make_shared<LShiftOperator<T, T1, T2, T3, T4>>(allocator, estimator, view_trimmer, read_iter_gen, write_iter_gen, write_delta); 

            }

            template <class T, class T1, class T2, class T3, class T4>
            std::shared_ptr<ShiftOperator<T, T1, T2, T3, T4>> init_shift_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                     std::shared_ptr<vector::operation_utility::ShiftEstimatable<T1>> estimator,
                                     std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                                     std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                                     std::shared_ptr<vector::mutable_operation::ShiftOperatable<T4>> mutable_operator){
                
                return std::make_shared<ShiftOperator<T, T1, T2, T3, T4>>(allocator, estimator, assign_operator, view_trimmer, mutable_operator);

            }

            template <class T, class T1, class T2, class T3, class T4>
            std::shared_ptr<CustomPairWiseOperator<T, T1, T2, T3, T4>> init_custom_pw_ops(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                   std::shared_ptr<vector::operation_utility::Estimatable<T1>> estimator,
                                   std::shared_ptr<vector::mutable_operation::Operatable<T2>> assign_operator,
                                   std::shared_ptr<vector::operation_utility::VectorViewTrimmable<T3>> view_trimmer,
                                   std::shared_ptr<vector::mutable_operation::PairWiseOperatable<T4>> pairwise_operator){
                
                return std::make_shared<CustomPairWiseOperator<T, T1, T2, T3, T4>>(allocator, estimator, assign_operator, view_trimmer, pairwise_operator); 

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6>
            std::shared_ptr<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6>> init_mul_karatsuba(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                    std::shared_ptr<operation_utility::VectorViewSplittable<T1>> splitter,
                                    std::shared_ptr<Operatable<T2>> plus_ops,
                                    std::shared_ptr<Operatable<T3>> minus_ops,
                                    std::shared_ptr<ShiftOperatable<T4>> lshifter,
                                    std::shared_ptr<Operatable<T5>> naive_mul_ops,
                                    std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable<T6>> readable_gen){
                
                return std::make_shared<RecursiveKaratsubaMultiplicationOperator<T, T1, T2, T3, T4, T5, T6>>(allocator, splitter, plus_ops, minus_ops, lshifter, naive_mul_ops, readable_gen);

            }

            template <class T, class T1>
            std::shared_ptr<OptimizedMultiplicationOperator<T, T1>> init_mul_standard(std::shared_ptr<Operatable<T>> karatsuba,
                                   std::shared_ptr<Operatable<T1>> naive){
                
                return std::make_shared<OptimizedMultiplicationOperator<T, T1>>(karatsuba, naive); 

            }
            
            template <class T, class T1, class T2>
            std::shared_ptr<SixtyTwoBitBucketMultiplicationOperator<T, T1, T2>> init_mul_62_bucket_two_limb(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                             std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                             std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                return std::make_shared<SixtyTwoBitBucketMultiplicationOperator<T, T1, T2>>(allocator, result_trimmer, estimator); 

            } 

            template <class T, class T1, class T2>
            auto init_mul_30_bucket_two_limb(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                             std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                             std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                return std::make_shared<ThirtyBitBucketMultiplicationOperator<T, T1, T2>>(allocator, result_trimmer, estimator); 

            } 

            template <class T, class T1, class T2>
            auto init_mul_naive(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                             std::shared_ptr<operation_utility::VectorViewTrimmable<T1>> result_trimmer,
                                             std::shared_ptr<operation_utility::Estimatable<T2>> estimator){
                
                return std::make_shared<NaiveMultiplication<T, T1, T2>>(allocator, result_trimmer, estimator); 

            } 

            template <class T, class T1, class T2, class T3, class T4, class T5>
            auto init_mul_generic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                  std::shared_ptr<operation_utility::Discretizable<T1>> discretizer,
                                  std::shared_ptr<operation_utility::VectorViewTrimmable<T2>> result_trimmer,
                                  std::shared_ptr<operation_utility::Estimatable<T3>> estimator,
                                  std::shared_ptr<operation_utility::BitIteratorGeneratable<T4>> read_iter_gen,
                                  std::shared_ptr<operation_utility::BitIterWriterGeneratable<T5>> write_iter_gen,
                                  size_t read_sz){
                
                return std::make_shared<PropagatedBucketMultiplicationOperator<T, T1, T2, T3, T4, T5>>(allocator, discretizer, result_trimmer, estimator, read_iter_gen, write_iter_gen, read_sz); 

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
            auto init_modulo_div(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                 std::shared_ptr<Operatable<T1>> minus_ops,
                                 std::shared_ptr<operation_utility::BitLengthRetrievable<T2>> bit_length_retriever,
                                 std::shared_ptr<operation_utility::Estimatable<T3>> estimator,
                                 std::shared_ptr<operation_utility::ZeroDetectable<T4>> zero_detector,
                                 std::shared_ptr<comparer::Comparable<T5>> comparer,
                                 std::shared_ptr<operation_utility::VectorViewTrimmable<T6>> view_trimmer,
                                 std::shared_ptr<ShiftOperatable<T7>> lshifter,
                                 std::shared_ptr<caster::BoolVectorOpCastable<T8>> full_op_caster){
                
                return std::make_shared<IterativeModuloDivider<T, T1, T2, T3, T4, T5, T6, T7, T8>>(allocator, minus_ops, bit_length_retriever, 
                                                                                                   estimator, zero_detector, comparer,
                                                                                                   view_trimmer, lshifter, full_op_caster);


            } 

            template <class T>
            auto get_mul_62_bucket_two_limb(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){
                
                // assert(allocator != nullptr);

                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_multiply_estimator();

                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_mul_62_bucket_two_limb(allocator, casted_view_trimmer, casted_estimator); 

            }

            template <class T>
            auto get_mul_30_bucket_two_limb(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ 
                
                // assert(allocator != nullptr);

                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_multiply_estimator();

                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_mul_30_bucket_two_limb(allocator, casted_view_trimmer, casted_estimator); 

            }

            template <class T>
            auto get_mul_naive(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ 
                
                // assert(allocator != nullptr);

                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_multiply_estimator();

                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);

                return this->init_mul_naive(allocator, casted_view_trimmer, casted_estimator); 

            }

            template <class T>
            auto get_mul_bucket_generic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){
                
                auto discretizer = operation_utility::StandardOperationUtilityGenerator().get_discretizer();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_multiply_estimator();
                auto read_iter_gen = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_gen();
                auto write_iter_gen = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_orwriter_gen();
                size_t read_sz = std::min(((size_t) ACTUAL_BIT_LENGTH_PER_SLOT >> 1) - 1, (size_t) BIT_LENGTH_PER_SLOT); 

                auto casted_discretizer = discretizer->to_discretizable_sp(discretizer);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);
                auto casted_read_iter_gen = read_iter_gen->to_bit_iterator_generatable_sp(read_iter_gen);
                auto casted_write_iter_gen = write_iter_gen->to_bit_iter_writer_generatable_sp(write_iter_gen);

                return this->init_mul_generic(allocator, casted_discretizer, casted_view_trimmer, casted_estimator, casted_read_iter_gen, casted_write_iter_gen, read_sz);

            }

        public:
            
            template <class T>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>
                
                // assert(allocator != nullptr);

                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_plus_estimator();
                auto assigner = mutable_operation::StandardArithmeticOperatorGenerator().get_assigner();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto plus_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_plus(); 
                
                auto casted_estimator = estimator->to_estimatable_sp(estimator);
                auto casted_assigner = assigner->to_operatable_sp(assigner);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_plus_ops = plus_ops->to_operatable_sp(plus_ops); 

                return this->init_pairwise_operator(allocator, casted_estimator, casted_assigner, casted_view_trimmer, casted_plus_ops);

            }

            template <class T>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_minus_estimator();
                auto assigner = mutable_operation::StandardArithmeticOperatorGenerator().get_assigner();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto minus_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_minus(); 

                auto casted_estimator = estimator->to_estimatable_sp(estimator);
                auto casted_assigner = assigner->to_operatable_sp(assigner);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops); 

                return this->init_pairwise_operator(allocator, casted_estimator, casted_assigner, casted_view_trimmer, casted_minus_ops);


            }

            template <class T>
            auto get_lshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<ShiftOperatable>

                // assert(allocator != nullptr);

                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_lshift_estimator();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto read_iter = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_gen();
                auto write_iter = operation_utility::StandardOperationUtilityGenerator().get_bit_iter_orwriter_gen();
                
                auto casted_estimator = estimator->to_shift_estimatable_sp(estimator);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_read_iter = read_iter->to_bit_iterator_generatable_sp(read_iter);
                auto casted_write_iter = write_iter->to_bit_iter_writer_generatable_sp(write_iter);

                auto rs = this->init_lshift_operator(allocator, casted_estimator, casted_view_trimmer, casted_read_iter, casted_write_iter, BIT_LENGTH_PER_SLOT);

                return rs;

            }

            template <class T>
            auto get_rshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<ShiftOperatable>

                // assert(allocator != nullptr);

                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_rshift_estimator();
                auto assigner = mutable_operation::StandardArithmeticOperatorGenerator().get_assigner();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto rshift_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_rshift(); 

                auto casted_estimator = estimator->to_shift_estimatable_sp(estimator);
                auto casted_assigner = assigner->to_operatable_sp(assigner);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_rshift_ops = rshift_ops->to_shift_operatable_sp(rshift_ops); 

                return this->init_shift_operator(allocator, casted_estimator, casted_assigner, casted_view_trimmer, casted_rshift_ops);

            }
            
            template <class T>
            auto get_custom_pairwise(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<PairWiseOperatable>

                // assert(allocator != nullptr);

                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_pairwise_estimator();
                auto assigner = mutable_operation::StandardArithmeticOperatorGenerator().get_assigner();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto pw_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_custom_pairwise(); 

                auto casted_estimator = estimator->to_estimatable_sp(estimator);
                auto casted_assigner = assigner->to_operatable_sp(assigner);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_pw_ops = pw_ops->to_pairwise_operatable_sp(pw_ops);

                return this->init_custom_pw_ops(allocator, casted_estimator, casted_assigner, casted_view_trimmer, casted_pw_ops);

            }

            template <class T>
            auto get_mul_bucket(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                #if defined(BIT_LENGTH_PER_SLOT_62)

                    return this->get_mul_62_bucket_two_limb(allocator);

                #elif defined(BIT_LENGTH_PER_SLOT_30) 

                    #if defined(QWORD_MACHINE)

                        return this->get_mul_naive(allocator);

                    #else
                        
                        return this->get_mul_30_bucket_two_limb(allocator);

                    #endif

                #else

                    return this->get_mul_bucket_generic(allocator);

                #endif

            }

            template <class T>
            auto get_mul_karatsuba(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto splitter = operation_utility::StandardOperationUtilityGenerator().get_zero_default_splitter();
                auto plus_ops = this->get_plus(allocator);
                auto minus_ops = this->get_minus(allocator);
                auto lshift_ops = this->get_lshift(allocator);
                auto mul_ops = this->get_mul_bucket(allocator);
                auto vec_caster = memory::sizet_linear::StandardGenerator().get_dangling_read_vec_gen(); 

                auto casted_splitter = splitter->to_view_splittable_sp(splitter);
                auto casted_plus_ops = plus_ops->to_operatable_sp(plus_ops);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops);
                auto casted_lshift_ops = lshift_ops->to_shift_operatable_sp(lshift_ops);
                auto casted_mul_ops = mul_ops->to_operatable_sp(mul_ops);
                auto casted_vec_caster = vec_caster->to_vector_readable_generatable_sp(vec_caster);
                
                return this->init_mul_karatsuba(allocator, casted_splitter, casted_plus_ops, casted_minus_ops, casted_lshift_ops, casted_mul_ops, casted_vec_caster);

            }

            template <class T>
            auto get_mul_standard(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                // auto karatsuba = this->get_mul_karatsuba(allocator);
                // auto naive = this->get_mul_bucket(allocator); 

                // auto casted_karatsuba = karatsuba->to_operatable_sp(karatsuba);
                // auto casted_naive = naive->to_operatable_sp(naive);

                // return this->init_mul_standard(casted_karatsuba, casted_naive);
                
                return this->get_mul_karatsuba(allocator); 

            }

            template <class T>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto minus_ops = this->get_minus(allocator);
                auto bit_length_retriever = operation_utility::StandardOperationUtilityGenerator().get_bit_length_retriever();
                auto estimator = operation_utility::StandardOperationUtilityGenerator().get_divide_estimator();
                auto zero_detector = operation_utility::StandardOperationUtilityGenerator().get_zero_detector();
                auto vec_comparer = comparer::StandardComparerGenerator().get_backward_comparer();
                auto view_trimmer = operation_utility::StandardOperationUtilityGenerator().get_default_guarded_vector_right_view_trimmer();
                auto lshift_ops = this->get_lshift(allocator);
                auto full_op_caster = caster::StandardBoolVectorCasterGenerator().get_full_opv_caster();
                
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops);
                auto casted_bit_length_retriever = bit_length_retriever->to_bit_length_retrievable_sp(bit_length_retriever);
                auto casted_estimator = estimator->to_estimatable_sp(estimator);
                auto casted_zero_detector = zero_detector->to_zero_detectable_sp(zero_detector);
                auto casted_vec_comparer = vec_comparer->to_comparable_sp(vec_comparer);
                auto casted_view_trimmer = view_trimmer->to_vector_view_trimmable_sp(view_trimmer);
                auto casted_lshift_ops = lshift_ops->to_shift_operatable_sp(lshift_ops);
                auto casted_full_op_caster = full_op_caster->to_boolvector_op_castable_sp(full_op_caster);

                return this->init_modulo_div(allocator, casted_minus_ops, casted_bit_length_retriever, casted_estimator, 
                                             casted_zero_detector, casted_vec_comparer, casted_view_trimmer, casted_lshift_ops,
                                             casted_full_op_caster);


            }


    };

}

namespace bignum::vector::mutable_extensible_operation{

    class AssignOperator: public Operatable<AssignOperator>{

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

    template <class T, class T1>
    class ImmutableOperator: public Operatable<ImmutableOperator<T, T1>>{

        private:

            AssignOperator assign_ops;
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation::Operatable<T1>> immutable_operator;

        public:

            ImmutableOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                              std::shared_ptr<operation::Operatable<T1>> immutable_operator){
                
                this->allocator = allocator;
                this->immutable_operator = immutable_operator;

            }

            template<class T2, class T3>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T2>& lhs, memory::sizet_linear::VectorReadable<T3>& rhs){
                
                this->allocator->enter_scope();

                auto rs = this->immutable_operator->ops(lhs, rhs);
                this->assign_ops.ops(lhs, rs);

                this->allocator->exit_scope();

            }

    };

    template <class T, class T1, class T2, class T3>
    class Operator: public Operatable<Operator<T, T1, T2, T3>>{

        private:

            AssignOperator assign_ops;
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation::Operatable<T1>> im_operator;
            std::shared_ptr<mutable_operation::Operatable<T2>> m_operator;
            std::shared_ptr<operation_utility::Judgable<T3>> judge;

        public:

            Operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                     std::shared_ptr<operation::Operatable<T1>> im_operator,
                     std::shared_ptr<mutable_operation::Operatable<T2>> m_operator,
                     std::shared_ptr<operation_utility::Judgable<T3>> judge){
                
                this->allocator = allocator;
                this->im_operator = im_operator;
                this->m_operator = m_operator;
                this->judge = judge;

            }

            template <class T4, class T5>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T4>& lhs, memory::sizet_linear::VectorReadable<T5>& rhs){

                if (this->judge->fit(lhs, rhs)){

                    this->m_operator->ops(lhs, rhs);
                    
                    return;

                } 

                this->allocator->enter_scope();

                auto rs = this->im_operator->ops(lhs, rhs);
                this->assign_ops.ops(lhs, rs);

                this->allocator->exit_scope();

            }

    };

    template <class T, class T1, class T2, class T3>
    class ShiftOperator: public ShiftOperatable<ShiftOperator<T, T1, T2, T3>>{

        private:

            AssignOperator assign_ops;
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation::ShiftOperatable<T1>> im_operator;
            std::shared_ptr<mutable_operation::ShiftOperatable<T2>> m_operator;
            std::shared_ptr<operation_utility::ShiftJudgable<T3>> judge;
        
        public:

            ShiftOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                          std::shared_ptr<operation::ShiftOperatable<T1>> im_operator,
                          std::shared_ptr<mutable_operation::ShiftOperatable<T2>> m_operator,
                          std::shared_ptr<operation_utility::ShiftJudgable<T3>> judge){
                
                this->allocator = allocator;
                this->im_operator = im_operator;
                this->m_operator = m_operator;
                this->judge = judge;

            }

            template <class T4>
            void ops(memory::sizet_linear::ReallocatableOperatableVector<T4>& lhs, size_t rhs){

                if (this->judge->fit(lhs, rhs)){

                    this->m_operator->ops(lhs, rhs);
                    return;

                } 

                this->allocator->enter_scope();

                auto rs = this->im_operator->ops(lhs, rhs);
                this->assign_ops.ops(lhs, rs);

                this->allocator->exit_scope();

            }

    };

    template <class T, class T1>
    class CustomPairWiseOperator: public PairWiseOperatable<CustomPairWiseOperator<T, T1>>{
        
        private:

            AssignOperator assign_ops;
            std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator;
            std::shared_ptr<operation::PairWiseOperatable<T1>> im_operator;
        
        public:

            CustomPairWiseOperator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                   std::shared_ptr<operation::PairWiseOperatable<T1>> im_operator){
                
                this->allocator = allocator;
                this->im_operator = im_operator;

            }

            template <class T2, class T3>
            void custom_ops(memory::sizet_linear::ReallocatableOperatableVector<T2>& lhs, memory::sizet_linear::VectorReadable<T3>& rhs, 
                            vector::pairwise_ops pw_ops){

                this->allocator->enter_scope();

                auto rs = this->im_operator->custom_ops(lhs, rhs, pw_ops);
                this->assign_ops.ops(lhs, rs);

                this->allocator->exit_scope();

            }

    };

    class StandardArithmeticOperatableGenerator{

        private:

            template <class T, class T1, class T2, class T3>
            auto init_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                               std::shared_ptr<operation::Operatable<T1>> im_operator,
                               std::shared_ptr<mutable_operation::Operatable<T2>> m_operator,
                               std::shared_ptr<operation_utility::Judgable<T3>> judge){
                
                return std::make_shared<Operator<T, T1, T2, T3>>(allocator, im_operator, m_operator, judge); 

            }  

            template <class T, class T1, class T2, class T3>
            auto init_shift_operator(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                     std::shared_ptr<operation::ShiftOperatable<T1>> im_operator,
                                     std::shared_ptr<mutable_operation::ShiftOperatable<T2>> m_operator,
                                     std::shared_ptr<operation_utility::ShiftJudgable<T3>> judge){
                
                return std::make_shared<ShiftOperator<T, T1, T2, T3>>(allocator, im_operator, m_operator, judge);

            }

            template <class T, class T1>
            auto init_custom_pairwise(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator,
                                      std::shared_ptr<operation::PairWiseOperatable<T1>> im_operator){
                
                return std::make_shared<CustomPairWiseOperator<T, T1>>(allocator, im_operator);

            }

        public:

            auto get_assign(){ // -> shared_ptr<Operatable>

                return std::make_shared<AssignOperator>();

            }

            template <class T>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>
                 
                // assert(allocator != nullptr);

                auto immutable_plus_ops = operation::StandardArithmeticOperatorGenerator().get_plus(allocator);
                auto mutable_plus_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_plus();
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_lightning_plus_judge();

                auto casted_immutable_plus_ops = immutable_plus_ops->to_operatable_sp(immutable_plus_ops);
                auto casted_mutable_plus_ops = mutable_plus_ops->to_operatable_sp(mutable_plus_ops);
                auto casted_judge = judge->to_judgable_sp(judge);

                return this->init_operator(allocator, casted_immutable_plus_ops, casted_mutable_plus_ops, casted_judge);

            }

            template <class T>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto immutable_minus_ops = operation::StandardArithmeticOperatorGenerator().get_minus(allocator);
                auto mutable_minus_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_minus();
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_minus_judge();

                auto casted_immutable_minus_ops = immutable_minus_ops->to_operatable_sp(immutable_minus_ops);
                auto casted_mutable_minus_ops = mutable_minus_ops->to_operatable_sp(mutable_minus_ops);
                auto casted_judge = judge->to_judgable_sp(judge); 

                return this->init_operator(allocator, casted_immutable_minus_ops, casted_mutable_minus_ops, casted_judge);

            }

            template <class T>
            auto get_lshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<ShiftOperatable>

                // assert(allocator != nullptr);

                auto immutable_lshift_ops = operation::StandardArithmeticOperatorGenerator().get_lshift(allocator);
                auto mutable_lshift_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_rshift(); //not working -- immutable
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_immutable_judge();

                auto casted_immutable_lshift_ops = immutable_lshift_ops->to_shift_operatable_sp(immutable_lshift_ops);
                auto casted_mutable_lshift_ops = mutable_lshift_ops->to_shift_operatable_sp(mutable_lshift_ops);
                auto casted_judge = judge->to_shift_judgable_sp(judge); 

                return this->init_shift_operator(allocator, casted_immutable_lshift_ops, casted_mutable_lshift_ops, casted_judge);

            }
            
            template <class T>
            auto get_rshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<ShiftOperatable>

                // assert(allocator != nullptr);

                auto immutable_rshift_ops = operation::StandardArithmeticOperatorGenerator().get_rshift(allocator);
                auto mutable_rshift_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_rshift();
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_rshift_judge();

                auto casted_immutable_rshift_ops = immutable_rshift_ops->to_shift_operatable_sp(immutable_rshift_ops);
                auto casted_mutable_rshift_ops = mutable_rshift_ops->to_shift_operatable_sp(mutable_rshift_ops);
                auto casted_judge = judge->to_shift_judgable_sp(judge); 

                return this->init_shift_operator(allocator, casted_immutable_rshift_ops, casted_mutable_rshift_ops, casted_judge);

            }

            template <class T>
            auto get_custom_pairwise(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<PairWiseOperatable>

                // assert(allocator != nullptr);

                auto immutable_custom_ops = operation::StandardArithmeticOperatorGenerator().get_custom_pairwise(allocator);
                auto casted_immutable_custom_ops = immutable_custom_ops->to_pairwise_operatable_sp(immutable_custom_ops);

                return this->init_custom_pairwise(allocator, casted_immutable_custom_ops);

            }

            template <class T>
            auto get_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto immutable_operator = operation::StandardArithmeticOperatorGenerator().get_mul_standard(allocator);
                auto mutable_operator = mutable_operation::StandardArithmeticOperatorGenerator().get_plus(); // not working -- immutable
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_immutable_judge();

                auto casted_immutable_operator = immutable_operator->to_operatable_sp(immutable_operator);
                auto casted_mutable_operator = mutable_operator->to_operatable_sp(mutable_operator);
                auto casted_judge = judge->to_shift_judgable_sp(judge);

                return this->init_operator(allocator, casted_immutable_operator, casted_mutable_operator, casted_judge);

            }

            template <class T>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable>

                // assert(allocator != nullptr);

                auto immutable_operator = operation::StandardArithmeticOperatorGenerator().get_divide(allocator);
                auto mutable_operator = mutable_operation::StandardArithmeticOperatorGenerator().get_plus(); // not working -- immutable
                auto judge = operation_utility::StandardOperationUtilityGenerator().get_immutable_judge();

                auto casted_immutable_operator = immutable_operator->to_operatable_sp(immutable_operator);
                auto casted_mutable_operator = mutable_operator->to_operatable_sp(mutable_operator);
                auto casted_judge = judge->to_shift_judgable_sp(judge);

                return this->init_operator(allocator, casted_immutable_operator, casted_mutable_operator, casted_judge);

            }


    };

}

namespace bignum::integer::usgn{ 

    class StandardBigNumView: public BigNumable<StandardBigNumView>, public memory::sizet_linear::SmallStackVectorView{

        public:

            StandardBigNumView() = default;

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
    
    template <class T>
    class SizeTLinearVectorViewCaster: public SizeTLinearVectorCastable<SizeTLinearVectorViewCaster<T>>{ // Type Unified Guarantee

        private:

            std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen;

        public:
            
            SizeTLinearVectorViewCaster(std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen){

                this->gen = gen;

            }

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
                
                auto casted = this->gen->from_vec_readable(std::forward<T1>(data));

                return StandardBigNumView(casted);

            }

    };

    template <class T, class T1>
    class UINTCaster: public UINTCastable<UINTCaster<T, T1>>{

        private:

            std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster;
            std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster;

            size_t MAX_VAL = std::numeric_limits<size_t>::max(); 

        public:
            
            UINTCaster(std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster,
                       std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster){
            
                this->vector_caster = vector_caster;
                this->bignum_caster = bignum_caster;

            }

            auto cast(size_t data){ // -> BigNumable&

                auto vec_casted = this->vector_caster->cast(&data, 1);
                
                return this->bignum_caster->cast(vec_casted);

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

    template <class T, class T1>
    class StandardUINTCaster: public UINTCastable<StandardUINTCaster<T, T1>>{ // Stable Addr Cast

        private:

            std::shared_ptr<UINTCastable<T>> runtime_caster;
            std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster;
            size_t precomputed_size;
            
        public:

            StandardUINTCaster(std::shared_ptr<UINTCastable<T>> runtime_caster,
                               std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster,
                               size_t precomputed_size){
                
                this->runtime_caster = runtime_caster;
                this->precomputed_caster = precomputed_caster;
                this->precomputed_size = precomputed_size;

            } 

            auto cast(size_t num){

                if (num < this->precomputed_size){

                    return this->precomputed_caster->cast(num);

                }

                return this->runtime_caster->cast(num);

            }

    };

    class StandardGenerator{ // - Type Guaranteed
 
        private:
            
            template <class T>
            auto init_sizet_linear_vector_view_caster(std::shared_ptr<memory::sizet_linear::StackVectorReadableGeneratable<T>> gen){

                return std::make_shared<SizeTLinearVectorViewCaster<T>>(gen);

            }   

            
            template <class T, class T1>
            auto init_slow_uint_caster(std::shared_ptr<vector::caster::SizeTLinearCastable<T>> vector_caster,
                                       std::shared_ptr<SizeTLinearVectorCastable<T1>> bignum_caster){
                
                return std::make_shared<UINTCaster<T, T1>>(vector_caster, bignum_caster); 

            } 

            template <class T, class T1>
            auto init_std_uint_caster(std::shared_ptr<UINTCastable<T>> runtime_caster,
                                      std::shared_ptr<PrecomputedUINTCaster<T1>> precomputed_caster,
                                      size_t precomputed_size){
                
                return std::make_shared<StandardUINTCaster<T, T1>>(runtime_caster, precomputed_caster, precomputed_size); 

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

};

namespace bignum::integer::usgn::caster{

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

    template <class T>
    class OOBMaxGuardUINTCaster: private RawUINTCaster, public UINTCastable<OOBMaxGuardUINTCaster<T>>{
        
        private:

            std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            
            const size_t BIT_MAX = sizeof(size_t) * 8;
            const size_t VAL_MAX = std::numeric_limits<size_t>::max();

        public:

            OOBMaxGuardUINTCaster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                this->bit_length_retriever = bit_length_retriever;

            }

            template <class T1>
            size_t to_size_t(BigNumable<T1>& val){

                if (this->bit_length_retriever->get_bit_length(val) > BIT_MAX){

                    return VAL_MAX;

                }

                return RawUINTCaster::raw_to_size_t(val);

            }

    };

    template <class T>
    class OOBErrorUINTCaster: private RawUINTCaster, public UINTCastable<OOBErrorUINTCaster<T>>{

        private:
            
            std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            const size_t BIT_MAX = sizeof(size_t) * 8;

        public:
            
            OOBErrorUINTCaster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                this->bit_length_retriever = bit_length_retriever;

            }

            template <class T1>
            size_t to_size_t(BigNumable<T1>& val){

                assert(this->bit_length_retriever->get_bit_length(val) <= BIT_MAX);

                return RawUINTCaster::raw_to_size_t(val);

            }

    };

    class StandardGenerator{

        private:
            
            template <class T>
            auto init_oob_max_guard_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                return std::make_shared<OOBMaxGuardUINTCaster<T>>(bit_length_retriever); 

            }

            template <class T>
            auto init_oob_error_uint_caster(std::shared_ptr<vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever){

                return std::make_shared<OOBErrorUINTCaster<T>>(bit_length_retriever); 

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

};

namespace bignum::integer::usgn::operation_utility{
    
    template <class T>
    class BitChecker: public OOBCheckable<BitChecker<T>>{

        private:

            std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever;
            size_t max_bits;

        public:

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

    class StandardGenerator{

        private:

            template <class T>
            auto init_bit_checker(std::shared_ptr<bignum::vector::operation_utility::BitLengthRetrievable<T>> bit_length_retriever,
                                  size_t max_bit_length){

                return std::make_shared<BitChecker<T>>(bit_length_retriever, max_bit_length);

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
};

namespace bignum::integer::usgn::operation{

    inline size_t bitwise_or(size_t lhs, size_t rhs){

        return lhs | rhs;

    }

    inline size_t bitwise_xor(size_t lhs, size_t rhs){
        
        return lhs ^ rhs;

    }

    inline size_t bitwise_and(size_t lhs, size_t rhs){

        return lhs & rhs;

    }

    template <class T, class T1, class T2>
    class Operator: public Operatable<Operator<T, T1, T2>>{

        private:

            std::shared_ptr<vector::operation::Operatable<T>> im_operator;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster;
            std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster;

        public:

            Operator(std::shared_ptr<vector::operation::Operatable<T>> im_operator,
                     std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                     std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster){
                
                this->im_operator = im_operator;
                this->result_caster = result_caster;
                this->bignum_caster = bignum_caster;

            }

            template <class T3, class T4>
            auto ops(BigNumable<T3>& lhs, BigNumable<T4>& rhs){ // -> BigNumable&
                
                auto rs = im_operator->ops(*lhs.to_vector_readable(), *rhs.to_vector_readable());
                                
                return this->result_caster->cast(rs);

            }

            template <class T3>
            auto ops(BigNumable<T3>& lhs, size_t rhs){ // ->BigNumable&
                
                auto casted = this->bignum_caster->cast(rhs);
                
                return this->ops(lhs, casted); 

            }


    };
    
    template <class T, class T1, class T2>
    class ShiftOperator: public Operatable<ShiftOperator<T, T1, T2>>{

        private:

            std::shared_ptr<vector::operation::ShiftOperatable<T>> shift_operator;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster;
            std::shared_ptr<caster::UINTCastable<T2>> uint_oob_guard_caster;
        
        public:

            ShiftOperator(std::shared_ptr<vector::operation::ShiftOperatable<T>> shift_operator,
                          std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                          std::shared_ptr<caster::UINTCastable<T2>> uint_oob_guard_caster){
                
                this->shift_operator = shift_operator;
                this->result_caster = result_caster;
                this->uint_oob_guard_caster = uint_oob_guard_caster;

            }

            template <class T3>
            auto ops(BigNumable<T3>& lhs, size_t rhs){ // -> BigNumable&
                
                auto rs = this->shift_operator->ops(*lhs.to_vector_readable(), rhs);
                
                return this->result_caster->cast(rs);

            }

            template <class T3, class T4>
            auto ops(BigNumable<T3>& lhs, BigNumable<T4>& rhs){ // BigNumable&
                
                auto casted = this->uint_oob_guard_caster->to_size_t(rhs);

                return this->ops(lhs, casted);

            }

    };
    
    template <class T, class T1, class T2>
    class PairWiseOperator: public Operatable<PairWiseOperator<T, T1, T2>>{

        private:

            std::shared_ptr<vector::operation::PairWiseOperatable<T>> custom_pairwise_operator;
            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster;
            std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster;
            vector::pairwise_ops pw_ops;

        public:

            PairWiseOperator(std::shared_ptr<vector::operation::PairWiseOperatable<T>> custom_pairwise_operator,
                             std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                             std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster,
                             vector::pairwise_ops pw_ops){
                
                this->custom_pairwise_operator = custom_pairwise_operator;
                this->result_caster = result_caster;
                this->bignum_caster = bignum_caster;
                this->pw_ops = pw_ops;

            }

            template <class T3, class T4>
            auto ops(BigNumable<T3>& lhs, BigNumable<T4>& rhs){ // -> BigNumable<>&
                
                auto rs = this->custom_pairwise_operator->custom_ops(*lhs.to_vector_readable(), *rhs.to_vector_readable(), this->pw_ops);

                return this->result_caster->cast(rs);;

            }

            template <class T3>
            auto ops(BigNumable<T3>& lhs, size_t rhs){

                auto casted = this->bignum_caster->cast(rhs);

                return this->ops(lhs, casted);

            }


    };
    
    template <class T, class T1, class T2>
    class BitwiseSizeTModuloOperator: public Operatable<BitwiseSizeTModuloOperator<T, T1, T2>>{

        private:
            
            std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> trueview_caster;
            std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster;
            std::shared_ptr<caster::UINTCastable<T2>> uint_caster;
        
        public:

            BitwiseSizeTModuloOperator(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> trueview_caster,
                                       std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                                       std::shared_ptr<caster::UINTCastable<T2>> uint_caster){
                
                this->trueview_caster = trueview_caster;
                this->bignum_caster = bignum_caster;
                this->uint_caster = uint_caster;

            }

            template <class T3>
            auto ops(BigNumable<T3>& lhs, size_t rhs){ // -> BigNumable<>&

                assert(rhs != 0);
                assert(algorithm::log2::log2_64(rhs) + 1 < sizeof(size_t) * 8);

                auto casted = this->trueview_caster->cast(lhs);
                size_t rs = 0;

                for (intmax_t i = (intmax_t) casted.length() - 1; i >= 0; --i){

                    rs <<= 1;

                    if (casted.get(i)){

                        rs += 1;

                    }

                    rs %= rhs;

                } 

                return this->bignum_caster->cast(rs);

            }
            
            template <class T3, class T4>
            auto ops(BigNumable<T3>& lhs, BigNumable<T4>& rhs){ // -> BigNumable<>&

                size_t casted_rs = this->uint_caster->to_size_t(rhs);

                return this->ops(lhs, casted_rs); 

            }

    };

    template <class T, class T1, class T2>
    class DiscretizedSizeTModuloOperator: public Operatable<DiscretizedSizeTModuloOperator<T, T1, T2>>{

        private:

            std::shared_ptr<vector::operation_utility::BitIteratorGeneratable<T>> reverse_read_iter_gen;
            std::shared_ptr<backward_caster::UINTCastable<T1>> result_caster;
            std::shared_ptr<caster::UINTCastable<T2>> uint_caster; 

        public:

            DiscretizedSizeTModuloOperator(std::shared_ptr<vector::operation_utility::BitIteratorGeneratable<T>> reverse_read_iter_gen,
                                           std::shared_ptr<backward_caster::UINTCastable<T1>> result_caster,
                                           std::shared_ptr<caster::UINTCastable<T2>> uint_caster){
                
                this->reverse_read_iter_gen = reverse_read_iter_gen;
                this->result_caster = result_caster;
                this->uint_caster = uint_caster;

            }

            template <class T3>
            auto ops(BigNumable<T3>& lhs, size_t rhs){
                
                assert(rhs != 0);
                uint8_t mod_bit_length = algorithm::log2::log2_64(rhs) + 1;
                assert(mod_bit_length < vector::BIT_LENGTH_PER_SLOT);

                uint8_t read_length = vector::BIT_LENGTH_PER_SLOT - mod_bit_length;
                size_t cur_read = 0;
                size_t rs = 0;

                auto read_iter = this->reverse_read_iter_gen->get(lhs, read_length);

                while (read_iter.next(cur_read)){

                    rs <<= read_length;
                    rs |= cur_read;
                    rs %= rhs; 

                }

                return this->result_caster->cast(rs);

            }

            template <class T3, class T4>
            auto ops(BigNumable<T3>& lhs, BigNumable<T4>& rhs){

                size_t casted = this->uint_caster->to_size_t(rhs); 
                
                return this->ops(lhs, casted);

            }

    };

    template <class T, class T1, class T2, class T3, class T4>
    class BruteForceModuloOperator: public Operatable<BruteForceModuloOperator<T, T1, T2, T3, T4>>{

        private:

            std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T>> result_caster;
            std::shared_ptr<backward_caster::UINTCastable<T1>> bvec_caster;
            std::shared_ptr<vector::operation::Operatable<T2>> mul_ops;
            std::shared_ptr<vector::operation::Operatable<T3>> divide_ops;
            std::shared_ptr<vector::operation::Operatable<T4>> minus_ops;

        public:

            BruteForceModuloOperator(std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T>> result_caster,
                                     std::shared_ptr<backward_caster::UINTCastable<T1>> bvec_caster,
                                     std::shared_ptr<vector::operation::Operatable<T2>> mul_ops,
                                     std::shared_ptr<vector::operation::Operatable<T3>> divide_ops,
                                     std::shared_ptr<vector::operation::Operatable<T4>> minus_ops){
                
                this->result_caster = result_caster;
                this->bvec_caster = bvec_caster;
                this->mul_ops = mul_ops;
                this->divide_ops = divide_ops;
                this->minus_ops = minus_ops;

            }

            template <class T5, class T6>
            auto ops(BigNumable<T5>& lhs, BigNumable<T6>& rhs){
                
                auto divide_rs = this->divide_ops->ops(lhs, rhs);
                auto mul_rs = this->mul_ops->ops(divide_rs, rhs);
                auto mod_rs = this->minus_ops->ops(lhs, mul_rs);

                return this->result_caster->cast(mod_rs);

            }

            template <class T5>
            auto ops(BigNumable<T5>& lhs, size_t rhs){

                auto casted = this->bvec_caster->cast(rhs);

                return this->ops(lhs, casted);

            }


    }; 

    template <class T, class T1, class T2, class T3, class T4>
    class ModuloOperator: public Operatable<ModuloOperator<T, T1, T2, T3, T4>>{

        private:

            std::shared_ptr<Operatable<T>> bitwise_modulo_ops;
            std::shared_ptr<Operatable<T1>> standard_modulo_ops;
            std::shared_ptr<operation_utility::OOBCheckable<T2>> bitwise_modulo_checker;
            std::shared_ptr<Operatable<T3>> discretized_modulo_ops;
            std::shared_ptr<operation_utility::OOBCheckable<T4>> discretized_modulo_checker;

        public:

            ModuloOperator(std::shared_ptr<Operatable<T>> bitwise_modulo_ops, 
                           std::shared_ptr<Operatable<T1>> standard_modulo_ops, 
                           std::shared_ptr<operation_utility::OOBCheckable<T2>> bitwise_modulo_checker,
                           std::shared_ptr<Operatable<T3>> discretized_modulo_ops,
                           std::shared_ptr<operation_utility::OOBCheckable<T4>> discretized_modulo_checker){
                
                this->bitwise_modulo_ops = bitwise_modulo_ops;
                this->standard_modulo_ops = standard_modulo_ops;
                this->bitwise_modulo_checker = bitwise_modulo_checker;
                this->discretized_modulo_ops = discretized_modulo_ops;
                this->discretized_modulo_checker = discretized_modulo_checker;

            }

            template <class T5, class T6>
            auto ops(BigNumable<T5>& lhs, BigNumable<T6>& rhs){ // -> BigNumable&

                if (this->discretized_modulo_checker->is_in_bound(rhs)){

                    return this->discretized_modulo_ops->ops(lhs, rhs);

                }

                if (this->bitwise_modulo_checker->is_in_bound(rhs)){

                    return this->bitwise_modulo_ops->ops(lhs, rhs);

                }

                return this->standard_modulo_ops->ops(lhs, rhs);

            }

            template <class T5>
            auto ops(BigNumable<T5>& lhs, size_t rhs){ // -> BigNumable&
                
                if (this->discretized_modulo_checker->is_in_bound(rhs)){

                    return this->discretized_modulo_ops->ops(lhs, rhs);

                }

                if (this->bitwise_modulo_checker->is_in_bound(rhs)){

                    return this->bitwise_modulo_ops->ops(lhs, rhs);

                }

                return this->standard_modulo_ops->ops(lhs, rhs);

            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
    class ArithmeticOperator: public UsgnOperatable<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9>>{

        private:

            std::shared_ptr<Operatable<T>> plus_ops;
            std::shared_ptr<Operatable<T1>> minus_ops;
            std::shared_ptr<Operatable<T2>> mul_ops;
            std::shared_ptr<Operatable<T3>> divide_ops;
        
            std::shared_ptr<Operatable<T4>> or_operator;
            std::shared_ptr<Operatable<T5>> and_operator;
            std::shared_ptr<Operatable<T6>> xor_operator;

            std::shared_ptr<Operatable<T7>> lshift_ops;
            std::shared_ptr<Operatable<T8>> rshift_ops;
            std::shared_ptr<Operatable<T9>> mod_ops;

        public:

            ArithmeticOperator() = default;
            
            ArithmeticOperator(std::shared_ptr<Operatable<T>> plus_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                               std::shared_ptr<Operatable<T2>> mul_ops, std::shared_ptr<Operatable<T3>> divide_ops,
                               std::shared_ptr<Operatable<T4>> or_operator, std::shared_ptr<Operatable<T5>> and_operator,
                               std::shared_ptr<Operatable<T6>> xor_operator, std::shared_ptr<Operatable<T7>> lshift_ops,
                               std::shared_ptr<Operatable<T8>> rshift_ops, std::shared_ptr<Operatable<T9>> mod_ops){
                
                this->plus_ops = plus_ops;
                this->minus_ops = minus_ops;
                this->mul_ops = mul_ops;
                this->divide_ops = divide_ops;
                
                this->or_operator = or_operator;
                this->and_operator = and_operator;
                this->xor_operator = xor_operator;
                
                this->lshift_ops = lshift_ops;
                this->rshift_ops = rshift_ops;
                this->mod_ops = mod_ops;

            }

            template<class T10, class T11>
            auto minus(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->minus_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto minus(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->minus_ops->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto plus(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->plus_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto plus(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->plus_ops->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto rshift(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->rshift_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto rshift(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->rshift_ops->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto lshift(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&
                
                return this->lshift_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto lshift(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->lshift_ops->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto multiply(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->mul_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto multiply(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->mul_ops->ops(lhs, rhs);

            }
            
            template<class T10, class T11>
            auto divide(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->divide_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto divide(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->divide_ops->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto and_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->and_operator->ops(lhs, rhs);

            }

            template<class T10>
            auto and_ops(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->and_operator->ops(lhs, rhs);

            } 

            template<class T10, class T11>
            auto or_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->or_operator->ops(lhs, rhs);

            }

            template<class T10>
            auto or_ops(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->or_operator->ops(lhs, rhs);
            }

            template<class T10, class T11>
            auto xor_ops(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->xor_operator->ops(lhs, rhs);

            }

            template<class T10>
            auto xor_ops(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->xor_operator->ops(lhs, rhs);

            }

            template<class T10, class T11>
            auto mod(BigNumable<T10>& lhs, BigNumable<T11>& rhs){ // -> BigNumable&

                return this->mod_ops->ops(lhs, rhs);

            }

            template<class T10>
            auto mod(BigNumable<T10>& lhs, size_t rhs){ // -> BigNumable&

                return this->mod_ops->ops(lhs, rhs);

            }

    }; 

    class StandardArithmeticOperatorGenerator{

        private:

            template <class T, class T1, class T2>
            auto init_discretized_modulo_operator(std::shared_ptr<vector::operation_utility::BitIteratorGeneratable<T>> reverse_read_iter_gen,
                                                  std::shared_ptr<backward_caster::UINTCastable<T1>> result_caster,
                                                  std::shared_ptr<caster::UINTCastable<T2>> uint_caster){
                
                return std::make_shared<DiscretizedSizeTModuloOperator<T, T1, T2>>(reverse_read_iter_gen, result_caster, uint_caster);

            }

            template <class T, class T1, class T2>
            auto init_operator(std::shared_ptr<vector::operation::Operatable<T>> im_operator,
                               std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                               std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster){
                
                return std::make_shared<Operator<T, T1, T2>>(im_operator, result_caster, bignum_caster);

            } 

            template <class T, class T1, class T2>
            auto init_shift_operator(std::shared_ptr<vector::operation::ShiftOperatable<T>> shift_operator,
                                     std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                                     std::shared_ptr<caster::UINTCastable<T2>> uint_oob_guard_caster){
                
                return std::make_shared<ShiftOperator<T, T1, T2>>(shift_operator, result_caster, uint_oob_guard_caster); 

            }

            template <class T, class T1, class T2>
            auto init_pairwise_operator(std::shared_ptr<vector::operation::PairWiseOperatable<T>> custom_pairwise_operator,
                                        std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T1>> result_caster,
                                        std::shared_ptr<backward_caster::UINTCastable<T2>> bignum_caster,
                                        vector::pairwise_ops pw_ops){
                
                return std::make_shared<PairWiseOperator<T, T1, T2>>(custom_pairwise_operator, result_caster, bignum_caster, pw_ops); 

            }
            
            template <class T, class T1, class T2>
            auto init_bitwise_sizet_modulo_operator(std::shared_ptr<vector::caster::BoolVectorViewCastable<T>> trueview_caster,
                                                    std::shared_ptr<backward_caster::UINTCastable<T1>> bignum_caster,
                                                    std::shared_ptr<caster::UINTCastable<T2>> oob_error_uint_caster){
                
                return std::make_shared<BitwiseSizeTModuloOperator<T, T1, T2>>(trueview_caster, bignum_caster, oob_error_uint_caster); 

            }

            template <class T, class T1, class T2, class T3, class T4>
            auto init_bruteforce_modulo_operator(std::shared_ptr<backward_caster::SizeTLinearVectorCastable<T>> result_caster,
                                                 std::shared_ptr<backward_caster::UINTCastable<T1>> bvec_caster,
                                                 std::shared_ptr<vector::operation::Operatable<T2>> mul_ops,
                                                 std::shared_ptr<vector::operation::Operatable<T3>> divide_ops,
                                                 std::shared_ptr<vector::operation::Operatable<T4>> minus_ops){
                
                return std::make_shared<BruteForceModuloOperator<T, T1, T2, T3, T4>>(result_caster, bvec_caster, mul_ops, divide_ops, minus_ops);

            }

            template <class T, class T1, class T2, class T3, class T4>
            auto init_standard_modulo_operator(std::shared_ptr<Operatable<T>> bitwise_modulo_ops, 
                                               std::shared_ptr<Operatable<T1>> bruteforce_modulo_ops, 
                                               std::shared_ptr<operation_utility::OOBCheckable<T2>> bitwise_modulo_checker,
                                               std::shared_ptr<Operatable<T3>> discretized_modulo_ops,
                                               std::shared_ptr<operation_utility::OOBCheckable<T4>> discretized_modulo_checker){
                
                return std::make_shared<ModuloOperator<T, T1, T2, T3, T4>>(bitwise_modulo_ops, bruteforce_modulo_ops, bitwise_modulo_checker, discretized_modulo_ops, discretized_modulo_checker); 

            } 

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
            auto init_arithmetic(std::shared_ptr<Operatable<T>> plus_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                                 std::shared_ptr<Operatable<T2>> mul_ops, std::shared_ptr<Operatable<T3>> divide_ops,
                                 std::shared_ptr<Operatable<T4>> or_operator, std::shared_ptr<Operatable<T5>> and_operator,
                                 std::shared_ptr<Operatable<T6>> xor_operator, std::shared_ptr<Operatable<T7>> lshift_ops,
                                 std::shared_ptr<Operatable<T8>> rshift_ops, std::shared_ptr<Operatable<T9>> mod_ops){
                
                return std::make_shared<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9>>(plus_ops, minus_ops, mul_ops, divide_ops, or_operator, and_operator, xor_operator, lshift_ops, rshift_ops, mod_ops);
            }

        public:

            template <class T>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto ops = vector::operation::StandardArithmeticOperatorGenerator().get_plus(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_result_caster, casted_caster); 

            }
            
            template <class T>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto ops = vector::operation::StandardArithmeticOperatorGenerator().get_minus(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_result_caster, casted_caster); 

            }

            template <class T>
            auto get_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto ops = vector::operation::StandardArithmeticOperatorGenerator().get_mul_standard(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_result_caster, casted_caster); 

            }

            template <class T>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto ops = vector::operation::StandardArithmeticOperatorGenerator().get_divide(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_operator = ops->to_operatable_sp(ops);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_operator(casted_operator, casted_result_caster, casted_caster); 

            }

            template <class T>
            auto get_bruteforce_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto bvec_caster = backward_caster::StandardGenerator().get_uint_caster();
                auto mul_ops = vector::operation::StandardArithmeticOperatorGenerator().get_mul_standard(allocator);
                auto divide_ops = vector::operation::StandardArithmeticOperatorGenerator().get_divide(allocator);
                auto minus_ops = vector::operation::StandardArithmeticOperatorGenerator().get_minus(allocator);

                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_bvec_caster = bvec_caster->to_uint_castable_sp(bvec_caster);
                auto casted_mul_ops = mul_ops->to_operatable_sp(mul_ops);
                auto casted_divide_ops = divide_ops->to_operatable_sp(divide_ops);
                auto casted_minus_ops = minus_ops->to_operatable_sp(minus_ops); 

                return this->init_bruteforce_modulo_operator(casted_result_caster, casted_bvec_caster, casted_mul_ops, casted_divide_ops, casted_minus_ops);


            }

            auto get_bitwise_mod(){
                
                auto trueview_caster = vector::caster::StandardBoolVectorCasterGenerator().get_true_view_caster();
                auto bignum_caster = backward_caster::StandardGenerator().get_uint_caster();
                auto oob_error_uint_caster = caster::StandardGenerator().get_uint_oob_error_caster();

                auto casted_trueview_caster = trueview_caster->to_boolvector_view_castable_sp(trueview_caster);
                auto casted_bignum_caster = bignum_caster->to_uint_castable_sp(bignum_caster);
                auto casted_oob_error_uint_caster = oob_error_uint_caster->to_uint_castable_sp(oob_error_uint_caster);

                return this->init_bitwise_sizet_modulo_operator(casted_trueview_caster, casted_bignum_caster, casted_oob_error_uint_caster);

            }
            
            auto get_discretized_mod(){

                auto bit_iter = vector::operation_utility::StandardOperationUtilityGenerator().get_reverse_bit_iter_gen();
                auto bignum_caster = backward_caster::StandardGenerator().get_uint_caster();
                auto oob_error_uint_caster = caster::StandardGenerator().get_uint_oob_error_caster(); 

                auto casted_bit_iter = bit_iter->to_bit_iterator_generatable_sp(bit_iter);
                auto casted_bignum_caster = bignum_caster->to_uint_castable_sp(bignum_caster);
                auto casted_oob_error_uint_caster = oob_error_uint_caster->to_uint_castable_sp(oob_error_uint_caster);

                return this->init_discretized_modulo_operator(casted_bit_iter, casted_bignum_caster, casted_oob_error_uint_caster);

            } 

            template <class T>
            auto get_standard_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                auto bitwise_modulo_checker = operation_utility::StandardGenerator().get_bitwise_modulo_check();
                auto bitwise_mod = this->get_bitwise_mod();
                auto bruteforce_mod = this->get_bruteforce_mod(allocator);
                auto discretized_mod = this->get_discretized_mod();
                auto discretized_check = operation_utility::StandardGenerator().get_discretized_modulo_check(); 

                auto casted_bitwise_modulo_checker = bitwise_modulo_checker->to_oob_checkable_sp(bitwise_modulo_checker);
                auto casted_bitwise_mod = bitwise_mod->to_operatable_sp(bitwise_mod);
                auto casted_bruteforce_mod = bruteforce_mod->to_operatable_sp(bruteforce_mod);
                auto casted_discretized_mod = discretized_mod->to_operatable_sp(discretized_mod);
                auto casted_discretized_check = discretized_check->to_oob_checkable_sp(discretized_check); 

                return this->init_standard_modulo_operator(casted_bitwise_mod, casted_bruteforce_mod, casted_bitwise_modulo_checker, casted_discretized_mod, casted_discretized_check);

            }

            template <class T>
            auto get_lshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto lshift_operator = vector::operation::StandardArithmeticOperatorGenerator().get_lshift(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = caster::StandardGenerator().get_uint_oob_guard_caster();
                
                auto casted_lshift_operator = lshift_operator->to_shift_operatable_sp(lshift_operator);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_shift_operator(casted_lshift_operator, casted_result_caster, casted_caster); 

            }

            template <class T>
            auto get_rshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto rshift_operator = vector::operation::StandardArithmeticOperatorGenerator().get_rshift(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = caster::StandardGenerator().get_uint_oob_guard_caster();

                auto casted_rshift_operator = rshift_operator->to_shift_operatable_sp(rshift_operator);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_shift_operator(casted_rshift_operator, casted_result_caster, casted_caster); 

            }

            template <class T>
            auto get_or(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){
                
                // assert(allocator != nullptr);

                auto pairwise_operator = vector::operation::StandardArithmeticOperatorGenerator().get_custom_pairwise(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_operator(casted_pairwise_operator, casted_result_caster, casted_caster, bitwise_or);

            }

            template <class T>
            auto get_xor(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto pairwise_operator = vector::operation::StandardArithmeticOperatorGenerator().get_custom_pairwise(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_operator(casted_pairwise_operator, casted_result_caster, casted_caster, bitwise_xor);
            }

            template <class T>
            auto get_and(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){

                // assert(allocator != nullptr);

                auto pairwise_operator = vector::operation::StandardArithmeticOperatorGenerator().get_custom_pairwise(allocator);
                auto result_caster = backward_caster::StandardGenerator().get_sizet_linear_caster();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_pairwise_operator = pairwise_operator->to_pairwise_operatable_sp(pairwise_operator);
                auto casted_result_caster = result_caster->to_sizet_linear_vector_castable_sp(result_caster);
                auto casted_caster = caster->to_uint_castable_sp(caster); 

                return this->init_pairwise_operator(casted_pairwise_operator, casted_result_caster, casted_caster, bitwise_and);
            }

            template <class T>
            auto get_arithmetic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){
                
                auto plus_ops = this->get_plus(allocator); 
                auto minus_ops = this->get_minus(allocator); 
                auto mul_ops = this->get_mul(allocator); 
                auto div_ops = this->get_divide(allocator); 
                auto or_ops = this->get_or(allocator); 
                auto and_ops = this->get_and(allocator); 
                auto xor_ops = this->get_xor(allocator); 
                auto rshift_ops = this->get_rshift(allocator); 
                auto lshift_ops = this->get_lshift(allocator); 
                auto mod_ops = this->get_standard_mod(allocator); 

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
                                             casted_rshift_ops, casted_mod_ops);

            }
    };

};

namespace bignum::integer::usgn::mutable_operation{

    template <class T, class T1>
    class Operator: public Operatable<Operator<T, T1>>{

        private:

            std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> op;
            std::shared_ptr<backward_caster::UINTCastable<T1>> caster;

        public:

            Operator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> op, 
                     std::shared_ptr<backward_caster::UINTCastable<T1>> caster){
                
                this->op = op;
                this->caster = caster;
        
            }

            template <class T2, class T3>
            void ops(MutableBigNumable<T2>& lhs, BigNumable<T3>& rhs){

                this->op->ops(*lhs.to_reallocatable_operatable_vector(), *rhs.to_vector_readable());

            }

            template <class T2>
            void ops(MutableBigNumable<T2>& lhs, size_t rhs){
                
                auto casted = this->caster->cast(rhs);
                this->ops(lhs, casted);

            }

    };

    template <class T, class T1>
    class ImmutableWrapper: public Operatable<ImmutableWrapper<T, T1>>{

        private:

            std::shared_ptr<usgn::operation::Operatable<T>> op;
            std::shared_ptr<vector::mutable_extensible_operation::Operatable<T1>> assign_op;
        
        public:

            ImmutableWrapper(std::shared_ptr<usgn::operation::Operatable<T>> op,
                             std::shared_ptr<vector::mutable_extensible_operation::Operatable<T1>> assign_op){
                
                this->op = op;
                this->assign_op = assign_op;

            }

            template <class T2, class T3>
            void ops(MutableBigNumable<T2>& lhs, BigNumable<T3>& rhs){

                auto rs = this->op->ops(lhs, rhs);
                this->assign_op->ops(*lhs.to_reallocatable_operatable_vector(), *rs.to_vector_readable());

            }

            template <class T2>
            void ops(MutableBigNumable<T2>& lhs, size_t rhs){

                auto rs = this->op->ops(lhs, rhs);
                this->assign_op->ops(*lhs.to_reallocatable_operatable_vector(), *rs.to_vector_readable());
                
            }

    };

    template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
    class ArithmeticOperator: public UsgnOperatable<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>>{

        private:

            std::shared_ptr<Operatable<T>> assign_ops;
            std::shared_ptr<Operatable<T1>> minus_ops;
            std::shared_ptr<Operatable<T2>> plus_ops;
            std::shared_ptr<Operatable<T3>> rshift_ops;
            std::shared_ptr<Operatable<T4>> lshift_ops;
            std::shared_ptr<Operatable<T5>> mul_ops;
            std::shared_ptr<Operatable<T6>> div_ops;
            std::shared_ptr<Operatable<T7>> and_operator;
            std::shared_ptr<Operatable<T8>> or_operator;
            std::shared_ptr<Operatable<T9>> xor_operator;
            std::shared_ptr<Operatable<T10>> mod_ops;
        
        public:

            ArithmeticOperator() = default;

            ArithmeticOperator(std::shared_ptr<Operatable<T>> assign_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                               std::shared_ptr<Operatable<T2>> plus_ops, std::shared_ptr<Operatable<T3>> rshift_ops,
                               std::shared_ptr<Operatable<T4>> lshift_ops, std::shared_ptr<Operatable<T5>> mul_ops,
                               std::shared_ptr<Operatable<T6>> div_ops, std::shared_ptr<Operatable<T7>> and_operator,
                               std::shared_ptr<Operatable<T8>> or_operator, std::shared_ptr<Operatable<T9>> xor_operator,
                               std::shared_ptr<Operatable<T10>> mod_ops){
                
                this->assign_ops = assign_ops;
                this->minus_ops = minus_ops;
                this->plus_ops = plus_ops;
                this->rshift_ops = rshift_ops;
                this->lshift_ops = lshift_ops;
                this->mul_ops = mul_ops;
                this->div_ops = div_ops;
                this->and_operator = and_operator;
                this->or_operator = or_operator;
                this->xor_operator = xor_operator;
                this->mod_ops = mod_ops;

            }

            template <class T12, class T13>
            void assign(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->assign_ops->ops(lhs, rhs);

            }

            template <class T12>
            void assign(MutableBigNumable<T12>& lhs, size_t rhs){
                
                this->assign_ops->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void minus(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->minus_ops->ops(lhs, rhs);

            }

            template <class T12>
            void minus(MutableBigNumable<T12>& lhs, size_t rhs){

                this->minus_ops->ops(lhs, rhs);
                
            }

            template <class T12, class T13>
            void plus(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->plus_ops->ops(lhs, rhs);

            }

            template <class T12>
            void plus(MutableBigNumable<T12>& lhs, size_t rhs){

                this->plus_ops->ops(lhs, rhs);
                
            }

            template <class T12, class T13>
            void rshift(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->rshift_ops->ops(lhs, rhs);

            }

            template <class T12>
            void rshift(MutableBigNumable<T12>& lhs, size_t rhs){

                this->rshift_ops->ops(lhs, rhs);

            }            

            template <class T12, class T13>
            void lshift(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->lshift_ops->ops(lhs, rhs);

            }

            template <class T12>
            void lshift(MutableBigNumable<T12>& lhs, size_t rhs){

                this->lshift_ops->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void multiply(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->mul_ops->ops(lhs, rhs);

            }

            template <class T12>
            void multiply(MutableBigNumable<T12>& lhs, size_t rhs){

                this->mul_ops->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void divide(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->div_ops->ops(lhs, rhs);

            }

            template <class T12>
            void divide(MutableBigNumable<T12>& lhs, size_t rhs){

                this->div_ops->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void and_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->and_operator->ops(lhs, rhs);

            }

            template <class T12>
            void and_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                this->and_operator->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void or_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->or_operator->ops(lhs, rhs);

            }

            template <class T12>
            void or_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                this->or_operator->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void xor_ops(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){
                this->xor_operator->ops(lhs, rhs);

            }

            template <class T12>
            void xor_ops(MutableBigNumable<T12>& lhs, size_t rhs){

                this->xor_operator->ops(lhs, rhs);

            }

            template <class T12, class T13>
            void mod(MutableBigNumable<T12>& lhs, BigNumable<T13>& rhs){

                this->mod_ops->ops(lhs, rhs);

            }

            template <class T12>
            void mod(MutableBigNumable<T12>& lhs, size_t rhs){

                this->mod_ops->ops(lhs, rhs);

            }

    };

    class StandardArithmeticOperatorGenerator{

        private:

            template <class T, class T1>
            std::shared_ptr<Operator<T, T1>> init_operator(std::shared_ptr<vector::mutable_extensible_operation::Operatable<T>> op, 
                                                           std::shared_ptr<backward_caster::UINTCastable<T1>> caster){
                
                return std::make_shared<Operator<T, T1>>(op, caster); 

            } 

            template <class T, class T1>
            std::shared_ptr<ImmutableWrapper<T, T1>> init_immutable_wrapper(std::shared_ptr<usgn::operation::Operatable<T>> op,
                                                                            std::shared_ptr<vector::mutable_extensible_operation::Operatable<T1>> assign_op){
                
                return std::make_shared<ImmutableWrapper<T, T1>>(op, assign_op);

            }

            template <class T, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
            auto init_arithmetic(std::shared_ptr<Operatable<T>> assign_ops, std::shared_ptr<Operatable<T1>> minus_ops,
                                std::shared_ptr<Operatable<T2>> plus_ops, std::shared_ptr<Operatable<T3>> rshift_ops,
                                std::shared_ptr<Operatable<T4>> lshift_ops, std::shared_ptr<Operatable<T5>> mul_ops,
                                std::shared_ptr<Operatable<T6>> div_ops, std::shared_ptr<Operatable<T7>> and_ops,
                                std::shared_ptr<Operatable<T8>> or_ops, std::shared_ptr<Operatable<T9>> xor_ops,
                                std::shared_ptr<Operatable<T10>> mod_ops){
                
                return std::make_shared<ArithmeticOperator<T, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>>(assign_ops, minus_ops, plus_ops, rshift_ops,
                                                                                                        lshift_ops, mul_ops, div_ops, and_ops,
                                                                                                        or_ops, xor_ops, mod_ops);

                
            }  

        public:

            auto get_assign(){ // -> shared_ptr<Operatable<>>
                
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster);
 
            }

            template <class T>
            auto get_plus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_plus(allocator);
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster);

            }

            template <class T>
            auto get_minus(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_minus(allocator);
                auto caster = backward_caster::StandardGenerator().get_uint_caster();

                auto casted_xmops = xmops->to_operatable_sp(xmops);
                auto casted_caster = caster->to_uint_castable_sp(caster);

                return this->init_operator(casted_xmops, casted_caster);

            }

            template <class T>
            auto get_rshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_rshift(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);            
            
            }

            template <class T>
            auto get_lshift(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_lshift(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);      

            }

            template <class T>
            auto get_mul(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_mul(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);                  
            }

            template <class T>
            auto get_divide(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_divide(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);                  
            }

            template <class T>
            auto get_and(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_and(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);                  
            }

            template <class T>
            auto get_or(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_or(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);                  
            }

            template <class T>
            auto get_xor(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_xor(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);                  
            }

            template <class T>
            auto get_mod(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<Operatable<>>

                // assert(allocator != nullptr);

                auto imops = operation::StandardArithmeticOperatorGenerator().get_standard_mod(allocator);
                auto xmops = vector::mutable_extensible_operation::StandardArithmeticOperatableGenerator().get_assign();

                auto casted_imops = imops->to_operatable_sp(imops);
                auto casted_xmops = xmops->to_operatable_sp(xmops);

                return this->init_immutable_wrapper(casted_imops, casted_xmops);

            }

            template <class T>
            auto get_arithmetic(std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> allocator){ // -> shared_ptr<UsgnOperatable<>>
                
                auto assign_ops = this->get_assign();
                auto plus_ops = this->get_plus(allocator);
                auto minus_ops = this->get_minus(allocator);
                auto rshift_ops = this->get_rshift(allocator);
                auto lshift_ops = this->get_lshift(allocator);
                auto mul_ops = this->get_mul(allocator);
                auto div_ops = this->get_divide(allocator);
                auto and_ops = this->get_and(allocator);
                auto or_ops = this->get_or(allocator);
                auto xor_ops = this->get_xor(allocator);
                auto mod_ops = this->get_mod(allocator);
                
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
                                             casted_or_ops, casted_xor_ops, casted_mod_ops);

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
            
            StandardStringParser() = default;

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

            StandardComparer() = default;

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

            StandardStringifier() = default;

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
    class TempStorageRetriever{

        private:

            inline static std::vector<std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>>> allocator;

        public:

            void init_data(std::vector<std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>>> allocator){

                this->allocator = allocator;

            }

            static inline std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>>& get(size_t idx){

                return TempStorageRetriever::allocator[idx];

            }

    };

    template <class T>
    class ParserRetriever{

        private:

            inline static std::vector<std::shared_ptr<parser::StringParsable<T>>> data; 

        public:

            void init_data(std::vector<std::shared_ptr<parser::StringParsable<T>>> data){

                this->data = data;

            }

            static inline std::shared_ptr<parser::StringParsable<T>>& get(size_t idx){

                return ParserRetriever::data[idx];

            }

    };

    template <class T>
    class ComparerRetriever{

        private:

            inline static std::vector<std::shared_ptr<Comparable<T>>> data; 

        public:

            void init_data(std::vector<std::shared_ptr<Comparable<T>>> data){

                this->data = data;

            }

            static inline std::shared_ptr<Comparable<T>>& get(size_t idx){

                return ComparerRetriever::data[idx];

            }

    };

    template <class T>
    class StringifierRetriever{

        private:

            inline static std::vector<std::shared_ptr<Stringifiable<T>>> data;

        public:

            void init_data(std::vector<std::shared_ptr<Stringifiable<T>>> data){
                
                this->data = data;

            }

            static inline std::shared_ptr<Stringifiable<T>>& get(size_t idx){

                return StringifierRetriever::data[idx];

            }


    };

    template <class T>
    class MutableOperationRetriever{

        private:

            inline static std::vector<std::shared_ptr<mutable_operation::UsgnOperatable<T>>> data; 

        public:

            void init_data(std::vector<std::shared_ptr<mutable_operation::UsgnOperatable<T>>> data){

                this->data = data;

            }

            static inline std::shared_ptr<mutable_operation::UsgnOperatable<T>>& get(size_t idx){

                return MutableOperationRetriever::data[idx];

            }

    };

    template <class T>
    class ImmutableOperationRetriever{

        private:

            inline static std::vector<std::shared_ptr<operation::UsgnOperatable<T>>> data; 

        public:

            void init_data(std::vector<std::shared_ptr<operation::UsgnOperatable<T>>> data){

                this->data = data;

            }

            static inline std::shared_ptr<operation::UsgnOperatable<T>>& get(size_t idx){

                return ImmutableOperationRetriever::data[idx];
                
            }

    };

    class ThreadSafeCallBack{

        private:

            static inline void empty_cb(size_t){};

            void (*cb_func) (size_t);
            size_t thread_id; 


        public:

            ThreadSafeCallBack(){

                this->cb_func = empty_cb;
                this->thread_id = 0;

            }

            ThreadSafeCallBack(void (*cb_func) (size_t), size_t thread_id){

                this->cb_func = cb_func;
                this->thread_id = thread_id;

            }
            
            ThreadSafeCallBack(const ThreadSafeCallBack&) = delete;
            
            ThreadSafeCallBack(ThreadSafeCallBack&& data){

                *this = std::move(data);

            }

            ThreadSafeCallBack& operator = (ThreadSafeCallBack&& data){

                this->cb_func = data.cb_func;
                this->thread_id = data.thread_id;
                data.cb_func = empty_cb;            

                return *this;

            }

            ThreadSafeCallBack& operator = (const ThreadSafeCallBack& data) = delete;

            ~ThreadSafeCallBack(){
                                    
                this->cb_func(this->thread_id);

            }

    };
    
    template <class T>
    class ScopeThreadSafeCallBack: public ThreadSafeCallBack{

        private:

            memory::sizet_linear::TempStorageGeneratable<T> * allocator;
            bool exit_allowed;

        public:

            ScopeThreadSafeCallBack(): ThreadSafeCallBack(){

                this->allocator = nullptr;
                this->exit_allowed = false;

            }

            ScopeThreadSafeCallBack(void (*cb_func) (size_t), size_t thread_id, memory::sizet_linear::TempStorageGeneratable<T> * allocator): ThreadSafeCallBack(cb_func, thread_id){

                this->allocator = allocator;
                this->allocator->enter_scope();
                this->exit_allowed = true;

            }

            ScopeThreadSafeCallBack(ScopeThreadSafeCallBack&& data){

                *this = std::move(data);

            }

            ScopeThreadSafeCallBack(const ScopeThreadSafeCallBack&) = delete;

            ScopeThreadSafeCallBack& operator = (ScopeThreadSafeCallBack&) = delete;

            ScopeThreadSafeCallBack& operator =(ScopeThreadSafeCallBack&& data){

                ThreadSafeCallBack::operator=(std::move(data));

                this->allocator = data.allocator;
                this->exit_allowed = data.exit_allowed;
                data.exit_allowed = false;

                return *this;

            }

            ~ScopeThreadSafeCallBack(){
                
                if (this->exit_allowed){
                    
                    this->allocator->exit_scope();

                }
                
            }

    };
    
    template <class T, class T1, class T2, class T3, class T4, class T5>
    class ThreadSafeRetriever{

        private:

            //inline static std::mutex mtx; 
            inline static std::vector<size_t> thread_ids;

            static void cb_func(size_t thread_id){

                //std::lock_guard<std::mutex> grd(ThreadSafeRetriever::mtx);

                ThreadSafeRetriever::thread_ids.push_back(thread_id);

            }

            template <class T6>
            inline constexpr ScopeThreadSafeCallBack<T6> init_scope_cb(void (*cb_func) (size_t), size_t thread_id, std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<T6>>& allocator){

                return ScopeThreadSafeCallBack<T6>(cb_func, thread_id, &*allocator); 

            } 

        public:

            void set_threads(size_t num_threads){

                //std::lock_guard<std::mutex> grd(this->mtx);

                this->thread_ids.resize(num_threads);
                
                for (size_t i = 0; i < num_threads; ++i){

                    this->thread_ids[i] = i; 

                }

            }

            inline constexpr auto get_comparer() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(this->thread_ids.size() != 0);
                
                size_t thread_id = this->thread_ids.back();
                this->thread_ids.pop_back();
                
                return std::make_pair(&*(ComparerRetriever<T>::get(thread_id)), ThreadSafeCallBack(cb_func, thread_id));


            }

            inline constexpr auto get_stringifier() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(this->thread_ids.size() != 0);

                size_t thread_id = this->thread_ids.back();
                this->thread_ids.pop_back();

                return std::make_pair(&*(StringifierRetriever<T1>::get(thread_id)), this->init_scope_cb(cb_func, thread_id, TempStorageRetriever<T5>::get(thread_id)));

            }

            inline constexpr auto get_mutable_operator() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(this->thread_ids.size() != 0);

                size_t thread_id = this->thread_ids.back();
                this->thread_ids.pop_back();

                return std::make_pair(&*(MutableOperationRetriever<T2>::get(thread_id)), ThreadSafeCallBack(cb_func, thread_id));
            }

            inline constexpr auto get_immutable_operator() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(this->thread_ids.size() != 0);

                size_t thread_id = this->thread_ids.back();
                this->thread_ids.pop_back();

                return std::make_pair(&*(ImmutableOperationRetriever<T3>::get(thread_id)), this->init_scope_cb(cb_func, thread_id, TempStorageRetriever<T5>::get(thread_id)));

            }

            inline constexpr auto get_parser() noexcept{

                //std::lock_guard<std::mutex> grd(this->mtx);

                assert(this->thread_ids.size() != 0);

                size_t thread_id = this->thread_ids.back();
                this->thread_ids.pop_back();

                return std::make_pair(&*(ParserRetriever<T4>::get(thread_id)), this->init_scope_cb(cb_func, thread_id, TempStorageRetriever<T5>::get(thread_id)));

            }

    };
    
    class ResourceTypeManager{

        protected:
            
            auto get_tstorage_type(){
                
                return memory::sizet_linear::StandardExponentialLinearTempStorageGenerator(); 

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

                auto immutable_ops = operation::StandardArithmeticOperatorGenerator().get_arithmetic<decltype(this->get_tstorage_type())>(nullptr);

                return immutable_ops->get_containee_type();

            }

            auto get_mutable_ops_type(){

                auto mutable_ops = mutable_operation::StandardArithmeticOperatorGenerator().get_arithmetic<decltype(this->get_tstorage_type())>(nullptr);

                return mutable_ops->get_containee_type();

            }

    };

    class RetrieverWrapper: public ResourceTypeManager{

        public:

            inline constexpr auto get_retriever() noexcept{

                return ThreadSafeRetriever<decltype(this->get_comparer_type()), decltype(this->get_stringifier_type()), 
                        decltype(this->get_mutable_ops_type()), decltype(this->get_immutable_ops_type()), decltype(this->get_parser_type()),
                        decltype(this->get_tstorage_type())>();


            }

    };

    class ResourceInitiator: public ResourceTypeManager{

        private:

            void init_temp_storage_retriever(size_t n) {

                std::vector<std::shared_ptr<memory::sizet_linear::TempStorageGeneratable<decltype(this->get_tstorage_type())>>> allocator;

                for (size_t i = 0; i < n; ++i){

                    auto temp_storage = std::make_shared<decltype(this->get_tstorage_type())>();
                    auto casted_temp_storage = temp_storage->to_temp_storage_generatable_sp(temp_storage);

                    allocator.push_back(casted_temp_storage);

                }

                TempStorageRetriever<decltype(this->get_tstorage_type())>().init_data(allocator);

            }

            void init_parser_retriever(size_t n){ 
                
                std::vector<std::shared_ptr<parser::StringParsable<decltype(this->get_parser_type())>>> data; 

                for (size_t i = 0; i < n; ++i){

                    auto allocator = TempStorageRetriever<decltype(this->get_tstorage_type())>().get(i);
                    data.push_back(parser::StandardGenerator().get_standard_parser(allocator));

                }

                ParserRetriever<decltype(this->get_parser_type())>().init_data(data);

            } 

            void init_comparer_retriever(size_t n){

                std::vector<std::shared_ptr<Comparable<decltype(this->get_comparer_type())>>> data;

                for (size_t i = 0; i < n; ++i){

                    data.push_back(UtilityGenerator().get_comparer());

                }

                ComparerRetriever<decltype(this->get_comparer_type())>().init_data(data);

            }

            void init_stringifier_retriever(size_t n){

                std::vector<std::shared_ptr<Stringifiable<decltype(this->get_stringifier_type())>>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = TempStorageRetriever<decltype(this->get_tstorage_type())>().get(i);
                    data.push_back(UtilityGenerator().get_stringifier(allocator));

                }

                StringifierRetriever<decltype(this->get_stringifier_type())>().init_data(data);

            }

            void init_mutable_ops_retriever(size_t n){

                std::vector<std::shared_ptr<mutable_operation::UsgnOperatable<decltype(this->get_mutable_ops_type())>>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = TempStorageRetriever<decltype(this->get_tstorage_type())>().get(i);
                    data.push_back(mutable_operation::StandardArithmeticOperatorGenerator().get_arithmetic(allocator));

                }

                MutableOperationRetriever<decltype(this->get_mutable_ops_type())>().init_data(data);


            }

            void init_immutable_ops_retriever(size_t n){

                std::vector<std::shared_ptr<operation::UsgnOperatable<decltype(this->get_immutable_ops_type())>>> data;

                for (size_t i = 0; i < n; ++i){

                    auto allocator = TempStorageRetriever<decltype(this->get_tstorage_type())>().get(i);
                    data.push_back(operation::StandardArithmeticOperatorGenerator().get_arithmetic(allocator));

                }

                ImmutableOperationRetriever<decltype(this->get_immutable_ops_type())>().init_data(data);

            }

        public:

            ResourceInitiator(size_t num_threads){

                num_threads *= 2;

                this->init_temp_storage_retriever(num_threads);
                this->init_parser_retriever(num_threads);
                this->init_comparer_retriever(num_threads);
                this->init_stringifier_retriever(num_threads);
                this->init_mutable_ops_retriever(num_threads);
                this->init_immutable_ops_retriever(num_threads);

                RetrieverWrapper().get_retriever().set_threads(num_threads);

            }

    };

    class ResourceDestructor: public ResourceTypeManager{

        public:

            ResourceDestructor(){

                TempStorageRetriever<decltype(this->get_tstorage_type())>().init_data({});
                ParserRetriever<decltype(this->get_parser_type())>().init_data({});
                ComparerRetriever<decltype(this->get_comparer_type())>().init_data({});
                MutableOperationRetriever<decltype(this->get_mutable_ops_type())>().init_data({});
                ImmutableOperationRetriever<decltype(this->get_immutable_ops_type())>().init_data({});
                RetrieverWrapper().get_retriever().set_threads({});

            }

    };
   
    class StandardMutableBigNum: public MutableBigNumable<StandardMutableBigNum>, public memory::sizet_linear::StandardReallocatableMemControlledVectorOperator, private RetrieverWrapper{
        
        public:

            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::get;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::length;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::resize;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::resize_no_copy;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::get_data;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::sizeof_slot;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::set;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::to_vector_readable;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::to_operatable_vector;
            using memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::to_reallocatable_operatable_vector;

        public:

            StandardMutableBigNum(){

                *this = 0;

            }

            StandardMutableBigNum(std::string obj){

                auto tup = this->get_retriever().get_parser();
                auto rs = tup.first->parse(obj);

                *this = rs;

            }

            StandardMutableBigNum(size_t val){

                *this = val;

            }

            template <class T>
            StandardMutableBigNum(BigNumable<T>& obj){
                                
                *this = obj;

            }

            StandardMutableBigNum(StandardMutableBigNum&& obj): memory::sizet_linear::StandardReallocatableMemControlledVectorOperator(std::move(obj)){}

            template <class T>
            bool operator > (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == 1;

            }

            bool operator > (size_t rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == 1;

            }
            
            bool operator > (StandardMutableBigNum&& rhs){
                
                return *this > (BigNumable&) rhs;

            }

            template <class T>
            bool operator < (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == -1;

            }

            bool operator < (size_t rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == -1;

            }

            bool operator < (StandardMutableBigNum&& rhs){
                
                return *this < (BigNumable&) rhs;

            }

            template <class T>
            bool operator == (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == 0;

            }

            bool operator == (size_t rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) == 0;

            }

            bool operator == (StandardMutableBigNum&& rhs){
                
                return *this == (BigNumable&) rhs;

            }

            template <class T>
            bool operator != (BigNumable<T>& rhs){

                return !(*this == rhs);

            }

            bool operator != (size_t rhs){

                return !(*this == rhs);

            }

            bool operator != (StandardMutableBigNum&& rhs){

                return *this != (BigNumable&) rhs;

            }

            template <class T>
            bool operator >= (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) >= 0;

            }

            bool operator >= (size_t rhs){
                
                return this->get_retriever().get_comparer().first->compare(*this, rhs) >= 0;

            }

            bool operator >= (StandardMutableBigNum&& rhs){

                return *this >= (BigNumable&) rhs;

            }

            template <class T>
            bool operator <= (BigNumable<T>& rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) <= 0;

            }

            bool operator <= (size_t rhs){

                return this->get_retriever().get_comparer().first->compare(*this, rhs) <= 0;

            }
            
            bool operator <= (StandardMutableBigNum&& rhs){

                return *this <= (BigNumable&) rhs;
                
            }

            std::string to_string(){

                return this->get_retriever().get_stringifier().first->stringify(*this);

            }

            StandardMutableBigNum& operator = (StandardMutableBigNum&& obj){

                memory::sizet_linear::StandardReallocatableMemControlledVectorOperator::operator=(std::move(obj));

                return *this;

            }

            template <class T>
            StandardMutableBigNum& operator = (BigNumable<T>& rhs){
                                
                if (this->length() != rhs.length()){

                    this->resize_no_copy(rhs.length());

                }

                for (size_t i = 0; i < this->length(); ++i){

                    this->set(i, rhs.get(i));

                }

                return *this;

            }
            
            StandardMutableBigNum& operator = (size_t rhs){
                
                this->get_retriever().get_mutable_operator().first->assign(*this, rhs);
                
                return *this;

            }
            
            template <class T>
            StandardMutableBigNum operator + (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->plus(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator + (size_t rhs){
                            
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->plus(*this, rhs);

                return StandardMutableBigNum(rs);
            
            }

            StandardMutableBigNum operator + (StandardMutableBigNum&& rhs){

                rhs += (BigNumable&) *this;

                return StandardMutableBigNum(std::move(rhs));
                
            }

            template <class T>
            StandardMutableBigNum operator - (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->minus(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator - (size_t rhs){
                
                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->minus(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator - (StandardMutableBigNum&& rhs){

                return *this - (BigNumable&) rhs; 

            }
            
            template <class T>
            StandardMutableBigNum operator * (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->multiply(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator * (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->multiply(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator * (StandardMutableBigNum&& rhs){
                
                return *this * (BigNumable&) rhs; 

            }
        
            template <class T>
            StandardMutableBigNum operator / (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->divide(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator / (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->divide(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator / (StandardMutableBigNum&& rhs){

                return *this / (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator % (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->mod(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator % (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->mod(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator % (StandardMutableBigNum&& rhs){

                return *this % (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator & (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->and_ops(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator & (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->and_ops(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator & (StandardMutableBigNum&& rhs){

                return *this & (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator | (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs  = tup.first->or_ops(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator | (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->or_ops(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator | (StandardMutableBigNum&& rhs){

                return *this | (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator ^ (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->xor_ops(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator ^ (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->xor_ops(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator ^ (StandardMutableBigNum&& rhs){

                return *this ^ (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator >> (BigNumable<T>& rhs){

               auto tup = this->get_retriever().get_immutable_operator();
               auto rs = tup.first->rshift(*this, rhs);

               return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator >> (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->rshift(*this, rhs);

                return StandardMutableBigNum(rs);

            }

            StandardMutableBigNum operator >> (StandardMutableBigNum&& rhs){
                
                return *this >> (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum operator << (BigNumable<T>& rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->lshift(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator << (size_t rhs){

                auto tup = this->get_retriever().get_immutable_operator();
                auto rs = tup.first->lshift(*this, rhs);

                return StandardMutableBigNum(rs);


            }

            StandardMutableBigNum operator << (StandardMutableBigNum&& rhs){

                return *this << (BigNumable&) rhs; 

            }

            template <class T>
            StandardMutableBigNum& operator += (BigNumable<T>& rhs){
                
                this->get_retriever().get_mutable_operator().first->plus(*this, rhs);

                return *this;
            
            }

            StandardMutableBigNum& operator += (size_t rhs){
                
               this->get_retriever().get_mutable_operator().first->plus(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator += (StandardMutableBigNum&& rhs){
                
                return *this += (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator -= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->minus(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator -= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->minus(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator -= (StandardMutableBigNum&& rhs){
                
                return *this -= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator *= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->multiply(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator *= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->multiply(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator *= (StandardMutableBigNum&& rhs){
                
                return *this *= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator /= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->divide(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator /= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->divide(*this, rhs);

                return *this;

            }
           
            StandardMutableBigNum& operator /= (StandardMutableBigNum&& rhs){
                
                return *this /= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator %= (BigNumable<T>& rhs){
                
                this->get_retriever().get_mutable_operator().first->mod(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator %= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->mod(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator %= (StandardMutableBigNum&& rhs){
                
                return *this %= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator &= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->and_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator &= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->and_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator &= (StandardMutableBigNum&& rhs){
                
                return *this &= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator |= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->or_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator |= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->or_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator |= (StandardMutableBigNum&& rhs){
                
                return *this |= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator ^= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->xor_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator ^= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->xor_ops(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator ^= (StandardMutableBigNum&& rhs){
                
                return *this ^= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator >>= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->rshift(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator >>= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->rshift(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator >>= (StandardMutableBigNum&& rhs){
                
                return *this >>= (BigNumable&) rhs;
            
            }

            template <class T>
            StandardMutableBigNum& operator <<= (BigNumable<T>& rhs){

                this->get_retriever().get_mutable_operator().first->lshift(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator <<= (size_t rhs){

                this->get_retriever().get_mutable_operator().first->lshift(*this, rhs);

                return *this;

            }

            StandardMutableBigNum& operator <<= (StandardMutableBigNum&& rhs){
                
                return *this <<= (BigNumable&) rhs;
            
            }

    };

}

namespace dgstd{

    using BigUINT = bignum::integer::usgn::StandardMutableBigNum; 

    static void big_uint_resource_init(size_t n = 1){

        assert(n != 0);

        bignum::integer::usgn::ResourceInitiator _init_bigint_resource(n);

    }
    
    static void big_uint_resource_destruct(){

        bignum::integer::usgn::ResourceDestructor();

    }

};

#endif