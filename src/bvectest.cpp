//#include "boolvectorcrtp.h"
//#include "BigNum.h"
#include "memcrtp.h"
#include <chrono>
#include "assert.h"
#include <vector>
#include "BigNum.h"

using namespace std::chrono;
using namespace bignum::integer::usgn;

template <unsigned T>
void print(){

    std::cout << T << std::endl;

}

class C{

};

class A{

    public:
        A(size_t data){

        }
        A() = default;

        ~A(){  
            std::cout << "destruct called" << std::endl;
        }
        A(A& data){
            std::cout << "copy called" << std::endl;
        }

        A(A&& data){
            std::cout << "moved called" << std::endl;
        }
        
        A& operator = (A&& data){
            std::cout << "moved called" << std::endl;

            return *this;
            
        }

        void print(){
        }

};

class B: public A{

    public:

        B(size_t data): A(data){

        }

};

template <class ...Args>
void test_(Args&& ...obj){

}

void test__(memory::pointer::stack_shared_ptr<C> temp){

} 

template <class T>
void test___(memory::pointer::stack_shared_ptr<memory::sizet_linear::TempStorageGeneratable<T>> casted){

    std::cout << "yay" << std::endl;

}

int main(){
        
    size_t n = 0;
    std::string inp;
    ResourceInitiator(1);

    auto allocator = memory::sizet_linear::StandardGenerator().get_temp_storage(1024);
    auto casted_allocator = allocator->to_temp_storage_generatable_sp(allocator);
    auto arith_ = bignum::vector::mutable_operation::IDArithmeticOperatorGenerator().get_plus(A());
    auto arith = bignum::integer::usgn::mutable_operation::IDArithmeticOperatorGenerator().get_arithmetic(casted_allocator, A());
    std::cout << &*allocator << "<temp storage addr>" << std::endl;

    dgstd::StackBigUINT<3> inc(1);

    while (true){
        
        dgstd::StackBigUINT<3> num(1);

        std::cout << "enter n: ";
        std::cin >> n;
        
        auto start = steady_clock().now();

        for (size_t i = 0; i < n; ++i){
            
            num += inc;

        }
        
        std::cout << num.to_string() << std::endl;
        std::cout << duration_cast<milliseconds>(steady_clock().now() - start).count() << "<lapsed>" << std::endl;
        
    }
    
    ResourceDestructor();

}