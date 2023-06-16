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

inline static const size_t BIT_LENGTH_PER_SLOT = 50;
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

    template <class Head>
    inline void plus(Head& _head, size_t val){

        Base::plus(_head, val);

    }

    inline void plus(Base&, size_t val){

        this->_val += val;
        
    }

    inline void true_plus(size_t val){

        this->_val += val;

    }

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void mul(MulIDRetriever& id_retriever, MulCarryRetriever<ID>& carry_retriever, Head& _head, size_t val){

        constexpr unsigned mul_id = MulIDRetriever().get(Self(), Head());

        this->_val += static_cast<decltype(MulCarryRetriever<mul_id>().get(this->get_last())) *>(this)->_mul * val;
        Base::rec_mul(id_retriever, carry_retriever, _head, val);

    }

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID>
    inline void rec_mul(MulIDRetriever&, MulCarryRetriever<ID>&, Self&, size_t val){}

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void rec_mul(MulIDRetriever& id_retriever, MulCarryRetriever<ID>& carry_retriever, Head& _head, size_t val){

        constexpr unsigned mul_id = MulIDRetriever().get(Self(), Head());

        this->_val += static_cast<decltype(MulCarryRetriever<mul_id>().get(this->get_last())) *>(this)->_mul * val;
        Base::rec_mul(id_retriever, carry_retriever, _head, val);

    }

    template <class Head>
    inline auto shift(Head& _head){

        Base::rec_shift(_head);

        return static_cast<decltype(this->get_parent_type()) *>(this);

    }

    template <class Head>
    inline void rec_shift(Head& _head){
        
        Base::rec_shift(_head);
        
    }

    inline void rec_shift(Base&){
        
        static_cast<decltype(this->get_parent_type()) *>(this)->true_plus(this->_val >> BIT_LENGTH_PER_SLOT);
        this->_val = 0;

    }
    
    template <class Head>
    inline size_t get_val(Head& _head){

        return Base::get_val(_head);

    }

    inline size_t get_val(Base&){

        return this->_val & MAX_VAL_PER_SLOT;

    }

    template <class Head>
    inline bool is_zero(Head& _head){

        return (this->_val == 0) && (Base::is_zero(_head));

    }

    inline bool is_zero(Base&){

        return this->_val == 0;

    }

    static auto get_first(){

        return Base::get_first(); 

    }

    auto get_parent_type(_Virtual_Head){

        return this->get_first(); 

    }

    template <class Parent_>
    auto get_parent_type(Parent_){

        return Parent_();

    }

    auto get_parent_type(){

        return this->get_parent_type(Parent()); 

    }

    template <class Parent_>
    static auto get_last(Parent_){

        return Parent_::get_last(); 

    } 

    static auto get_last(_Virtual_Head){

        return Self(); 

    }

    static auto get_last(){

        return Self::get_last(Parent());

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

    template <class Head>
    inline void plus(Head& _head, size_t val){

        using Base = decltype(Parent::get_last());
        constexpr auto _base = Base();

        this->plus(_head, _base, val);

    }
    
    template <class Base>
    void plus(Base&, const Base&, size_t val){

        this->_val += val;

    }

    template <class Head, class Base>
    void plus(Head& _head, const Base&, size_t val){

        static_cast<Base *>(this)->plus(_head, val);

    } 

    inline void true_plus(size_t val){

        this->_val += val;

    }

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void mul(MulIDRetriever& id_retriever, MulCarryRetriever<ID>& carry_retriever, Head& _head, size_t val){

        using Base = decltype(Parent::get_last());

        constexpr unsigned mul_id = MulIDRetriever().get(Self(), Head());

        this->_val += static_cast<decltype(MulCarryRetriever<mul_id>().get(Base())) *>(this)->_mul * val;
        static_cast<Base *>(this)->rec_mul(id_retriever, carry_retriever, _head, val);

    }

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID>
    inline void rec_mul(MulIDRetriever&, MulCarryRetriever<ID>&, Self&, size_t val){}

    template <class MulIDRetriever, template <unsigned> class MulCarryRetriever, unsigned ID, class Head>
    inline void rec_mul(MulIDRetriever& id_retriever, MulCarryRetriever<ID>& carry_retriever, Head& _head, size_t val){

        using Base = decltype(Parent::get_last());

        constexpr unsigned mul_id = MulIDRetriever().get(Self(), Head());

        this->_val += static_cast<decltype(MulCarryRetriever<mul_id>().get(Base())) *>(this)->_mul * val;
        static_cast<Base *>(this)->rec_mul(id_retriever, carry_retriever, _head, val);

    }

    template <class Head>
    inline auto shift(Head& _head){

        using Base = decltype(Parent::get_last());
        static_cast<Base *>(this)->rec_shift(_head);

        return static_cast<Parent *>(this);

    }

    template <class Head>
    inline void rec_shift(Head& _head){
        
        using Base = decltype(Parent::get_last());
        constexpr auto _base = Base(); 

        this->rec_shift(_head, _base);
        
    }
    
    template <class Base>
    inline void rec_shift(Base&, const Base&){
        
        static_cast<Parent *>(this)->true_plus(this->_val >> BIT_LENGTH_PER_SLOT);
        this->_val = 0;

    }
    
    template <class Head, class Base>
    inline void rec_shift(Head& _head, const Base&){

        static_cast<Base *>(this)->rec_shift(_head);

    } 
    
    template <class Head>
    inline size_t get_val(Head& _head){

        using Base = decltype(Parent::get_last());
        constexpr auto _base = Base(); 

        return this->get_val(_head, _base);

    }

    template <class Base>
    inline size_t get_val(Base&, const Base&){
  
        return this->_val & MAX_VAL_PER_SLOT;

    }

    template <class Head, class Base>
    inline size_t get_val(Head& _head, const Base&){

        return static_cast<Base *>(this)->get_val(_head);

    }

    template <class Head>
    inline bool is_zero(Head& _head){

        using Base = decltype(Parent::get_last());
        constexpr auto _base = Base();

        return this->is_zero(_head, _base);

    }

    template <class Base>
    inline bool is_zero(Base&, const Base&){

        return this->_val == 0;

    }

    template <class Head, class Base>
    inline bool is_zero(Head& _head, const Base&){

        return (this->_val == 0) && (static_cast<Base *>(this)->is_zero(_head));

    }

    static auto get_first(){

        return Self();

    }
    
};

struct MulIDRetriever{

    template <class T, class T1, unsigned ID, class T2, class T3, unsigned ID1, unsigned MAX_ID>
    constexpr unsigned get(MulCarry<T, T1, ID, MAX_ID> cur, MulCarry<T2, T3, ID1, MAX_ID> head){

        if (ID <= ID1){

            return MAX_ID - ID1 + ID; 

        }

        return ID - ID1;

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

template <class MulCarry, class T, class T1, unsigned Length>
struct CarryCycle{

    public:

        template <class MulCarry_>
        void rec_apply(MulCarry_& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, 
                       size_t inp_idx, size_t op_idx, memory::sizet_linear::OperatableVector<T1>& op_arr){
            
            CarryCycle<MulCarry_, T, T1, Length - 1>().apply(mul_carry, mul_vec, inp_idx, op_idx, op_arr);
        
        }

        void apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, 
                   size_t inp_idx, size_t op_idx, memory::sizet_linear::OperatableVector<T1>& op_arr){
            
            auto id_rev = MulIDRetriever();
            auto carry_rev = MulCarryRetrieverInitiator<0>();
            
            mul_carry.mul(id_rev, carry_rev, mul_carry, mul_vec.get(inp_idx));
            mul_carry.plus(mul_carry, op_arr.get(op_idx));
            op_arr.set(op_idx, mul_carry.get_val(mul_carry));

            auto shifted = mul_carry.shift(mul_carry);
            
            this->rec_apply(*shifted, mul_vec, inp_idx + 1, op_idx + 1, op_arr);

        }

};

template <class MulCarry, class T, class T1>
struct CarryCycle<MulCarry, T, T1, 1>{

    public:

        void apply(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, 
                   size_t inp_idx, size_t op_idx, memory::sizet_linear::OperatableVector<T1>& op_arr){

            auto id_rev = MulIDRetriever();
            auto carry_rev = MulCarryRetrieverInitiator<0>();

            mul_carry.mul(id_rev, carry_rev, mul_carry, mul_vec.get(inp_idx));
            mul_carry.plus(mul_carry, op_arr.get(op_idx));
            op_arr.set(op_idx, mul_carry.get_val(mul_carry));
            
            mul_carry.shift(mul_carry);

        }

};

template <class MulCarry, class T, class T1>
void init_cycle(MulCarry& mul_carry, memory::sizet_linear::VectorReadable<T>& mul_vec, 
                size_t inp_idx, size_t op_idx, memory::sizet_linear::OperatableVector<T1>& op_arr){
    
    CarryCycle<MulCarry, T, T1, LENGTH>().apply(mul_carry, mul_vec, inp_idx, op_idx, op_arr);
    
}

template <class MulCarry, class T1>
void output_trailing(MulCarry& mul_carry, memory::sizet_linear::OperatableVector<T1>& op_arr,
                     size_t op_idx){
    
    if (mul_carry.is_zero(mul_carry)){

        return;

    }

    mul_carry.plus(mul_carry, op_arr.get(op_idx));
    op_arr.set(op_idx, mul_carry.get_val(mul_carry));
    
    auto shifted = mul_carry.shift(mul_carry);

    output_trailing(*shifted, op_arr, op_idx + 1);

}

dgstd::BigUINT randomize(size_t sz){

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<size_t> dis;

    dgstd::BigUINT rs;
    rs.resize(sz);
    
    const size_t MAX_VAL = (1ULL << 50) - 1; 

    for (size_t i = 0; i < sz; ++i){

        rs.set(i, dis(gen) % MAX_VAL);

    }

    return rs;
    
} 

int main(){
    
    using namespace std::chrono; 

    dgstd::big_uint_resource_init();

    while (true){
            
        size_t magnifier = 0;

        std::cout << "magnifier: ";
        std::cin >> magnifier;
        
        size_t lhs_length = LENGTH * magnifier;
        size_t rhs_length = LENGTH; 

        dgstd::BigUINT lhs = randomize(lhs_length);
        dgstd::BigUINT rhs = randomize(rhs_length);
        dgstd::BigUINT rs; 


        rs.resize_no_copy(lhs.length() + rhs.length() + 2);

        for (size_t i = 0; i < rs.length(); ++i){

            rs.set(i, 0); 
        }

        auto start = steady_clock().now();

        MulCarry<_Virtual_Head, __uint128_t, LENGTH, LENGTH> mul_carry(*rhs.to_vector_readable());

        for (size_t i = 0; i < magnifier; ++i){
            
            init_cycle(mul_carry, *lhs.to_vector_readable(), i * LENGTH, i * LENGTH, *rs.to_operatable_vector());

        }

        output_trailing(mul_carry, *rs.to_operatable_vector(), lhs_length);
        auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
        
        start = steady_clock().now();

        dgstd::BigUINT pre_rs = lhs * rhs;

        auto lapsed_ = duration_cast<milliseconds>(steady_clock().now() - start).count();

        for (size_t i = 0; i < pre_rs.length(); ++i){

            assert(rs.get(i) == pre_rs.get(i));

        }

        std::cout << "completed" << std::endl;
        std::cout << lapsed << "<cyclic_milliseconds>" << lapsed_ << "<biguint_milliseconds>" << std::endl;

    }

    dgstd::big_uint_resource_destruct();

}

