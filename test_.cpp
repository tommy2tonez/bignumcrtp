#include <iostream>
#include <vector>
#include "src/memcrtp.h"
#include <iostream>
#include "src/BigNumOld.h"
#include <random>
#include <chrono> 

template <class T>
class A{

    private:

        T data;

    public:

        A(T data){

            this->data = data;

        }



};

inline static const size_t BIT_LENGTH_PER_SLOT = 60;
inline static const size_t MAX_VAL_PER_SLOT = ((size_t) 1 << BIT_LENGTH_PER_SLOT) - 1;
inline static const size_t LENGTH = 10;

struct _Virtual_Head{}; 

template <class Parent, class DoubleWord, unsigned LIMB_LENGTH, unsigned MAX_LIMB_LENGTH>
struct MulCarry: public MulCarry<MulCarry<Parent, DoubleWord, LIMB_LENGTH, MAX_LIMB_LENGTH>, DoubleWord, LIMB_LENGTH - 1, MAX_LIMB_LENGTH>{

    using Self = MulCarry<Parent, DoubleWord, LIMB_LENGTH, MAX_LIMB_LENGTH>;
    using Base = MulCarry<Self, DoubleWord, LIMB_LENGTH - 1, MAX_LIMB_LENGTH>;
    
    DoubleWord _val;
    DoubleWord _mul;
    
    constexpr MulCarry(): _val(0), _mul(0){}

    template <class T>
    MulCarry(memory::sizet_linear::VectorReadable<T>& muls): Base(muls){

        this->_val = 0;

        if (muls.length() >= LIMB_LENGTH){
            
            this->_mul = muls.get(LIMB_LENGTH - 1);

        } else{

            this->_mul = 0;

        }

    }  

    inline void plus(size_t val){

        static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->true_plus(val);

    }

    inline void true_plus(size_t val){

        this->_val += val;

    }

    template <template <unsigned> class MulCarryRetriever, unsigned ID>
    inline void mul(const MulCarryRetriever<ID>& carry_retriever, size_t val){

        static_cast<decltype(this->get_last()) *>(this)->rec_mul(carry_retriever, *this, val);

    }

    template <template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void rec_mul(const MulCarryRetriever<ID>& carry_retriever, const Head& head_val, size_t val){

        this->_val += static_cast<decltype(MulCarryRetriever<Self().get_cyclic_id(Head())>().get(this->get_last())) *>(this)->_mul * val;
        Base::rec_mul(carry_retriever, head_val, val);

    }

    inline auto shift(){

        static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->true_shift();

        return static_cast<decltype(this->get_parent_type()) *>(this);

    }

    inline void true_shift(){
        
        static_cast<decltype(this->get_parent_type()) *>(this)->true_plus(this->_val >> BIT_LENGTH_PER_SLOT);
        this->_val = 0;

    }
    
    inline size_t get_val(){

        return static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->get_true_val();

    }

    inline size_t get_true_val(){

        return this->_val & MAX_VAL_PER_SLOT;

    }

    template <class Head>
    inline bool is_zero(Head& _head){

        return (this->_val == 0) && (Base::is_zero(_head));

    }

    inline bool is_zero(Base&){

        return this->_val == 0;

    }

    constexpr auto get_parent_type(_Virtual_Head){

        return this->get_first(); 

    }

    template <class Parent_>
    constexpr auto get_parent_type(Parent_){

        return Parent_();

    }

    constexpr auto get_parent_type(){

        return this->get_parent_type(Parent()); 

    }

    template <class Parent_>
    constexpr auto get_last(Parent_){

        return static_cast<Parent_ *>(this)->get_last();

    } 

    constexpr auto get_last(_Virtual_Head){

        return Self(); 

    }

    constexpr auto get_last(){

        return this->get_last(Parent());

    }

    constexpr auto get_first(){

        return Base::get_first(); 

    }

    template <class Head>
    constexpr auto get_cyclic_tail(Head){

        return Base::get_cyclic_tail(Head()); 

    }

    constexpr auto get_cyclic_tail(Base){

        return Self();

    }

    template <class T, class T1, unsigned ID>
    constexpr unsigned get_cyclic_id(MulCarry<T, T1, ID, MAX_LIMB_LENGTH> head){

         if (LIMB_LENGTH <= ID){

            return MAX_LIMB_LENGTH - ID + LIMB_LENGTH; 

        }

        return LIMB_LENGTH - ID;

    }

};

template <class Parent, class DoubleWord, unsigned MAX_LIMB_LENGTH>
struct MulCarry<Parent, DoubleWord, 1, MAX_LIMB_LENGTH>{

    using Self = MulCarry<Parent, DoubleWord, 1, MAX_LIMB_LENGTH>;
    
    DoubleWord _val;
    DoubleWord _mul;
    
    constexpr MulCarry(): _val(0), _mul(0){}

    template <class T>
    MulCarry(memory::sizet_linear::VectorReadable<T>& muls){

        this->_val = 0;
        this->_mul = muls.get(0);

    }  

    inline void plus(size_t val){

        static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->true_plus(val);

    }

    inline void true_plus(size_t val){

        this->_val += val;

    }

    template <template <unsigned> class MulCarryRetriever, unsigned ID>
    inline void mul(const MulCarryRetriever<ID>& carry_retriever, size_t val){

        using Base = decltype(static_cast<Parent *>(this)->get_last());

        static_cast<Base *>(this)->rec_mul(carry_retriever, *this, val);

    }

    template <template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void rec_mul(const MulCarryRetriever<ID>& carry_retriever, const Head& head_val, size_t val){

        using Base = decltype(static_cast<Parent *>(this)->get_last());

        this->_val += static_cast<decltype(MulCarryRetriever<Self().get_cyclic_id(Head())>().get(Base())) *>(this)->_mul * val;

    }

    inline auto shift(){

        static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->true_shift();

        return static_cast<Parent *>(this);

    }

    inline void true_shift(){
        
        static_cast<Parent *>(this)->true_plus(this->_val >> BIT_LENGTH_PER_SLOT);
        this->_val = 0;

    }
    
    inline size_t get_val(){

        return static_cast<decltype(this->get_cyclic_tail(Self())) *>(this)->get_true_val();

    }

    inline size_t get_true_val(){

        return this->_val & MAX_VAL_PER_SLOT;

    }

    template <class Head>
    inline bool is_zero(Head& head){

        using Base = decltype(static_cast<Parent *>(this)->get_last());
        constexpr auto base = Base();

        return this->is_zero(head, base);

    }

    template <class Base>
    inline bool is_zero(Base&, const Base&){

        return this->_val == 0;

    }

    template <class Head, class Base>
    inline bool is_zero(Head& _head, const Base&){

        return (this->_val == 0) && (static_cast<Base *>(this)->is_zero(_head));

    }

    constexpr auto get_first(){

        return Self();

    }
    
    template <class Head>
    constexpr auto get_cyclic_tail(Head){

        using Base = decltype(static_cast<Parent *>(this)->get_last());

        return this->get_cyclic_tail(Head(), Base()); 

    }

    template <class Head, class Base>
    constexpr auto get_cyclic_tail(Head, Base){

        return static_cast<Base *>(this)->get_cyclic_tail(Head());

    }

    template <class Base>
    constexpr auto get_cyclic_tail(Base, Base){

        return Self();

    }

    template <class T, class T1, unsigned ID>
    constexpr unsigned get_cyclic_id(MulCarry<T, T1, ID, MAX_LIMB_LENGTH> head){

        const unsigned LIMB_LENGTH = 1;
        
        if (LIMB_LENGTH <= ID){

            return MAX_LIMB_LENGTH - ID + LIMB_LENGTH; 

        }

        return LIMB_LENGTH - ID;

    }

};

template <class Parent, class DoubleWord, unsigned CUR_LIMB_LENGTH, unsigned LOOKUP_LIMB_LENGTH, unsigned MAX_LIMB_LENGTH>
struct MulCarryRetriever{

    using Self = MulCarry<Parent, DoubleWord, CUR_LIMB_LENGTH, MAX_LIMB_LENGTH>;

    auto get(){

        return MulCarryRetriever<Self, DoubleWord, CUR_LIMB_LENGTH - 1, LOOKUP_LIMB_LENGTH, MAX_LIMB_LENGTH>().get();

    }

};

template <class Parent, class DoubleWord, unsigned LIMB_LENGTH, unsigned MAX_LIMB_LENGTH>
struct MulCarryRetriever<Parent, DoubleWord, LIMB_LENGTH, LIMB_LENGTH, MAX_LIMB_LENGTH>{

    using Self = MulCarry<Parent, DoubleWord, LIMB_LENGTH, MAX_LIMB_LENGTH>;
    
    auto get(){

        return Self();

    }
    
};

template <unsigned LOOKUP_LIMB_LENGTH>
struct MulCarryRetrieverInitiator{

    template <class Parent, class DoubleWord, unsigned MAX_LIMB_LENGTH>
    auto get(MulCarry<Parent, DoubleWord, MAX_LIMB_LENGTH, MAX_LIMB_LENGTH>){

        return MulCarryRetriever<Parent, DoubleWord, MAX_LIMB_LENGTH, LOOKUP_LIMB_LENGTH, MAX_LIMB_LENGTH>().get(); 

    }

};

template <unsigned Length>
struct CarryCycle{

    public:

        template <class MulCarry_, class T, class T1>
        auto rec_apply(MulCarry_& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, size_t inp_idx, 
                       memory::sizet_linear::OperatableVector<T1>& op_arr, size_t op_idx){
            
            return CarryCycle<Length - 1>().apply(mul_carry, mul_vec, inp_idx, op_arr, op_idx);
        
        }

        template <class MulCarry, class T, class T1>
        auto apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, size_t inp_idx, 
                   memory::sizet_linear::OperatableVector<T1>& op_arr, size_t op_idx){
            
            constexpr auto carry_retriever = MulCarryRetrieverInitiator<0>();
            
            mul_carry.mul(carry_retriever, mul_vec.get(inp_idx));
            mul_carry.plus(op_arr.get(op_idx));
            op_arr.set(op_idx, mul_carry.get_val());

            auto shifted = mul_carry.shift();
            
            return this->rec_apply(*shifted, mul_vec, inp_idx + 1, op_arr, op_idx + 1);

        }

};

template <>
struct CarryCycle<1>{

    public:

        template <class MulCarry, class T, class T1>
        auto apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, 
                   size_t inp_idx, memory::sizet_linear::OperatableVector<T1>& op_arr, size_t op_idx){

            constexpr auto carry_retriever = MulCarryRetrieverInitiator<0>();

            mul_carry.mul(carry_retriever, mul_vec.get(inp_idx));
            mul_carry.plus(op_arr.get(op_idx));
            op_arr.set(op_idx, mul_carry.get_val());
            
            return mul_carry.shift();

        }

};

template <class MulCarry, class T1>
void output_trailing(MulCarry& mul_carry, memory::sizet_linear::OperatableVector<T1>& op_arr,
                     size_t op_idx){
    
    if (mul_carry.is_zero(mul_carry)){

        return;

    }

    mul_carry.plus(op_arr.get(op_idx));
    op_arr.set(op_idx, mul_carry.get_val());
    
    auto shifted = mul_carry.shift();

    output_trailing(*shifted, op_arr, op_idx + 1);

}


//[Begin, End]
//[0..Length]

template <unsigned Begin, unsigned Length>
struct CarryRem{

    public:

        template <class MulCarry, class T, class T1>
        void apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, size_t inp_idx, 
                   memory::sizet_linear::OperatableVector<T1>& op_arr, size_t op_idx, size_t cyc_length){
            
            assert(cyc_length != 0);
            
            constexpr size_t End = Begin + Length - 1;
            constexpr size_t Mid = (Begin + End) >> 1; 
            constexpr size_t Lower_Length = Mid - Begin + 1;
            constexpr size_t Higher_Length = End - Mid + 1; 

            if (cyc_length > Mid){

                CarryRem<Mid, Higher_Length>().apply(mul_carry, mul_vec, inp_idx, op_arr, op_idx, cyc_length);

                return;

            } 

            CarryRem<Begin, Lower_Length>().apply(mul_carry, mul_vec, inp_idx, op_arr, op_idx, cyc_length);

        }

};

template <unsigned Begin>
struct CarryRem<Begin, 2>{

    public:

        template <class MulCarry, class T, class T1>
        void apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, size_t inp_idx, 
                   memory::sizet_linear::OperatableVector<T1>& op_arr, size_t op_idx, size_t cyc_length){
                        
            auto rs =  CarryCycle<Begin + 1>().apply(mul_carry, mul_vec, inp_idx, op_arr, op_idx);

            output_trailing(*rs, op_arr, op_idx + cyc_length);

        }
        
};


dgstd::BigUINT randomize(size_t sz){

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<size_t> dis;

    dgstd::BigUINT rs;
    rs.resize(sz);
    
    const size_t MAX_VAL = (1ULL << 60) - 1; 

    for (size_t i = 0; i < sz; ++i){

        rs.set(i, dis(gen) % MAX_VAL);

    }

    return rs;
    
} 

template <unsigned DEFAULT_SKIP_LENGTH = 2>
void sth(){

}

int main(){

    sth();

    using namespace std::chrono; 

    dgstd::big_uint_resource_init();
    size_t LHS_UPPER_BOUND = 100;
    size_t RHS_UPPER_BOUND = LENGTH; 

    for (size_t lhs_length = 1; lhs_length <= LHS_UPPER_BOUND; ++lhs_length){

        for (size_t rhs_length = 1; rhs_length <= RHS_UPPER_BOUND; ++rhs_length){

            dgstd::BigUINT lhs = randomize(lhs_length);
            dgstd::BigUINT rhs = randomize(rhs_length);
            dgstd::BigUINT rs; 

            rs.resize_no_copy(lhs.length() + rhs.length() + 2);

            for (size_t i = 0; i < rs.length(); ++i){

                rs.set(i, 0); 
            }

            auto start = steady_clock().now();

            MulCarry<_Virtual_Head, __uint128_t, LENGTH, LENGTH> mul_carry(*rhs.to_vector_readable());
            size_t cycle_n = lhs_length / LENGTH;

            for (size_t i = 0; i < cycle_n; ++i){
                
                CarryCycle<LENGTH>().apply(mul_carry, *lhs.to_vector_readable(), i * LENGTH, *rs.to_operatable_vector(), i * LENGTH);

            }

            size_t offset = cycle_n * LENGTH;
            size_t rem_n = lhs_length - offset;

            if (rem_n != 0){

                CarryRem<0, LENGTH>().apply(mul_carry, *lhs.to_vector_readable(), offset, *rs.to_operatable_vector(), offset, rem_n); 

            } else {
                
                output_trailing(mul_carry, *rs.to_operatable_vector(), lhs_length);

            }

            auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
            
            start = steady_clock().now();

            dgstd::BigUINT pre_rs = lhs * rhs;

            auto lapsed_ = duration_cast<milliseconds>(steady_clock().now() - start).count();

            for (size_t i = 0; i < pre_rs.length(); ++i){

                assert(rs.get(i) == pre_rs.get(i));

            }

        }

    }

    // while (true){
            
    //     size_t lhs_length = 0;
    //     size_t rhs_length = 0; 
        
    //     std::cout << "lhs_length: ";
    //     std::cin >> lhs_length;

    //     std::cout << "rhs_length: ";
    //     std::cin >> rhs_length;

    //     assert(rhs_length <= LENGTH);
        
    //     dgstd::BigUINT lhs = randomize(lhs_length);
    //     dgstd::BigUINT rhs = randomize(rhs_length);
    //     dgstd::BigUINT rs; 

    //     rs.resize_no_copy(lhs.length() + rhs.length() + 2);

    //     for (size_t i = 0; i < rs.length(); ++i){

    //         rs.set(i, 0); 
    //     }

    //     auto start = steady_clock().now();

    //     MulCarry<_Virtual_Head, __uint128_t, LENGTH, LENGTH> mul_carry(*rhs.to_vector_readable());
    //     size_t cycle_n = lhs_length / LENGTH;

    //     for (size_t i = 0; i < cycle_n; ++i){
            
    //         CarryCycle<LENGTH>().apply(mul_carry, *lhs.to_vector_readable(), i * LENGTH, *rs.to_operatable_vector(), i * LENGTH);

    //     }

    //     size_t offset = cycle_n * LENGTH;
    //     size_t rem_n = lhs_length - offset;

    //     if (rem_n != 0){

    //         CarryRem<0, LENGTH>().apply(mul_carry, *lhs.to_vector_readable(), offset, *rs.to_operatable_vector(), offset, rem_n); 

    //     } else {
            
    //         output_trailing(mul_carry, *rs.to_operatable_vector(), lhs_length);

    //     }

    //     auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
        
    //     start = steady_clock().now();

    //     dgstd::BigUINT pre_rs = lhs * rhs;

    //     auto lapsed_ = duration_cast<milliseconds>(steady_clock().now() - start).count();

    //     for (size_t i = 0; i < pre_rs.length(); ++i){

    //         assert(rs.get(i) == pre_rs.get(i));

    //     }

    //     //std::cout << "completed" << std::endl;
    //     //std::cout << lapsed << "<cyclic_milliseconds>" << lapsed_ << "<biguint_milliseconds>" << std::endl;

    // }

    std::cout << "completed" << std::endl;

    dgstd::big_uint_resource_destruct();

}

