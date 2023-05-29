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
    ResourceInitiator(1);

    dgstd::BigUINT inc(1);

    while (true){
        
        dgstd::BigUINT num(1000);

        std::cout << "enter n: ";
        std::cin >> n;

        auto start = steady_clock().now();
        
        for (size_t i =  1; i < n; ++i){

            num -= inc;
            std::cout << num.to_string() << std::endl;

        }
        
        auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
        std::cout << lapsed << "----" << std::endl;
        
    }
    
    ResourceDestructor();

}