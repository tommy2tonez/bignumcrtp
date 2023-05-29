#include "BigNum.h"
#include "chrono"

using namespace bignum::integer::usgn;
using namespace std::chrono;
using namespace memory::sizet_linear; 

void test1(){

    auto start = steady_clock().now(); 

    StandardMutableBigNum num;
    StandardMutableBigNum inc(1);
    size_t n = 10000000;

    for (size_t i = 0; i < n; ++i){

        num += inc;

    }

    std::cout << "test1: " << duration_cast<milliseconds>(steady_clock().now() - start).count() << std::endl;

}


void test2(){

    auto allocator = new StandardExponentialLinearTempStorageGenerator(1024);
    auto start = steady_clock().now(); 
    auto ops = bignum::vector::mutable_operation::StandardArithmeticOperatorGenerator().set_temp_storage(allocator).get_plus(); 

    StandardMutableBigNum num;
    StandardMutableBigNum inc(1);

    size_t n = 10000000;

    for (size_t i = 0; i < n; ++i){

        ops->ops(num, inc);

    }

    std::cout << "test2: " << duration_cast<milliseconds>(steady_clock().now() - start).count() << std::endl;
    
}

void test3(){
    
    using namespace bignum::vector::mutable_operation;

    auto allocator = new StandardExponentialLinearTempStorageGenerator(1024);
    auto start = steady_clock().now(); 
    CRTPOperatable<CRTPPlusOperator> ops = CRTPPlusOperator(); 

    StandardMutableBigNum num;
    StandardMutableBigNum inc(1);

    size_t n = 100000000;
    size_t * num_ptr = (size_t *) num.get_data();
    size_t num_n = num.length();
    size_t * inc_ptr = (size_t *) inc.get_data();
    size_t inc_n = inc.length();

    for (size_t i = 0; i < n; ++i){

        ops.ops(num_ptr, num_n, inc_ptr, inc_n);

    }

    std::cout << "test3: " << duration_cast<milliseconds>(steady_clock().now() - start).count() << std::endl;
    
}

int main(){

    ResourceInitiator(5);

    test1();
    test2();
    test3();

}