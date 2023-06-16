//#include "boolvectorcrtp.h"
//#include "BigNum.h"
#include "memcrtp.h"
#include <chrono>
#include "assert.h"
#include <vector>
#include "BigNum.h"
#include <random>

using namespace std::chrono;
using namespace bignum::integer::usgn;
using namespace memory::sizet_linear;

dgstd::BigUINT randomize(size_t sz){

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<size_t> dis;

    dgstd::BigUINT rs;
    rs.resize(sz);
    
    const size_t MAX_VAL = (1ULL << 62) - 1; 

    for (size_t i = 0; i < sz; ++i){

        rs.set(i, dis(gen) % MAX_VAL);

    }

    return rs;
    
} 



int main(){

    size_t x = 0;
    size_t x1 = 0;
    size_t n = 0;
    size_t i = 0;

    std::string inp;
    ResourceInitiator(1);

    // auto gen = memory::sizet_linear::StandardGenerator().get_temp_storage(1024);
    // auto casted_gen = gen->to_temp_storage_generatable_sp(gen);
    // auto mul_ops = bignum::vector::immutable_operation::IDArithmeticGenerator().get_mul_std(casted_gen, 0);
    // auto mul_ops_ = bignum::vector::immutable_operation::IDArithmeticGenerator().get_inplace_generic_mul<__uint128_t, int, 10>(); 
    // auto slow_std_div = bignum::vector::immutable_operation::IDArithmeticGenerator().get_slow_std_divide(casted_gen, mul_ops_->to_inplace_operatable_sp(mul_ops_), 0);

    std::random_device rd;
    std::mt19937_64 rgen(rd());
    std::uniform_int_distribution<size_t> dis;

    while (true){

        std::cout << "loops:";
        std::cin >> n;

        std::cout << "x:";
        std::cin >> x;

        std::cout << "x1:";
        std::cin >> x1;
        
        auto _val = randomize(x);
        auto _val1 = randomize(x1);

        // dgstd::BigUINT rs; 
        // rs.resize_no_copy(_val.length() + _val1.length() + 1);

        // std::cout << "<start>" << std::endl;
        // auto start = steady_clock().now();                    
        // slow_std_div->ops(*_val.to_vector_readable(), *_val1.to_vector_readable(), *rs.to_reallocatable_operatable_vector());
        // // for (size_t i = 0; i < rs.length(); ++i){

        // //     rs.set(i, 0);

        // // }

        // // std::cout << "<start>" << std::endl;
        // // auto start = steady_clock().now();                    

        // // if (i++ % 2 == 0){

        // //     mul_ops->ops(*_val.to_vector_readable(), *_val1.to_vector_readable(), *rs.to_reallocatable_operatable_vector());

            
        // // } else{

        // //     mul_ops_->ops(*_val.to_vector_readable(), *_val1.to_vector_readable(), *rs.to_operatable_vector());

        // // }

        // auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();

        // std::cout << rs.length() << "<length>" << lapsed << "<milliseconds>" << std::endl;

        __uint128_t total = 0;

        // // dgstd::StackBigUINT<1000> val = _val;
        // // dgstd::StackBigUINT<100> val2 = _val1;
        
        std::cout << "<start>" << std::endl;
        auto start = steady_clock().now();                    

        for (size_t i = 0; i < n; ++i){
            auto rs = _val * _val1;
            total += rs.get(0);
        }

        auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();

        std::cout << (size_t) (total & std::numeric_limits<size_t>::max()) << "<val>" << lapsed << "<milliseconds>" << std::endl;
        
        // for (size_t i = 0; i < x; ++i){

        //     for (size_t j = 1; j < x; ++j){

        //         size_t rval = dis(rgen);
        //         size_t lval = dis(rgen);
        //         // size_t rval = i;
        //         // size_t lval = j;

        //         dgstd::BigUINT val = rval;
        //         dgstd::BigUINT val1 = lval;

        //         dgstd::BigUINT pre_rs = rval / lval;
        //         auto rs = val / val1;

        //         assert(rs == pre_rs);


        //     }
        // }

        // std::cout << "all clear" << std::endl;

        // dgstd::BigUINT val_ = randomize(x);
        // dgstd::BigUINT val2_ = randomize(x1);
        // dgstd::BigUINT rs;

        // start = steady_clock().now();

        // div_ops->inplace_ops(*val_.to_reallocatable_operatable_vector(), *val2_.to_vector_readable(), *rs.to_reallocatable_operatable_vector());
        
        // lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
        // std::cout << lapsed << "<milliseconds>" << std::endl;

        // for (size_t i = 0; i < x; ++i){

        //     for (size_t j = 1; j < x; ++j){

        //         dgstd::BigUINT lhs = i;
        //         dgstd::BigUINT rhs = j;

        //         auto rs = lhs / rhs;
        //         dgstd::BigUINT real_rs = i / j;

        //         assert(rs == real_rs);
        //         std::cout << i << " " << j << std::endl;

        //     }
        // }
    }
    
    ResourceDestructor();

}