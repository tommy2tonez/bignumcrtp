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

    // StandardMutableBigNum a("0b1000000001");
    // StandardMutableBigNum b("0b101010110");
    // // std::cout << "rs parsing" <<
    // StandardMutableBigNum rs("0b1000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    
    // auto c = a << b;

    // for (size_t i = 0; i < rs.length(); ++i){

    //     std::cout << rs.get(i) << std::endl;

    // }


    // std::cout << c.length() << " " << rs.length() << std::endl;
    // std::cout << (c.to_string() == rs.to_string()) << std::endl;
    // std::cout << c.get_data() << " " << rs.get_data() << std::endl;

    // for (size_t i = 0; i < c.length(); ++i){
        
    //     std::cout << c.get(i) << " " << rs.get(i) << std::endl;
        
    //     assert(((size_t *)c.get_data())[i] == ((size_t *) rs.get_data())[i]);

    // }

    // auto mutable_ops = bignum::vector::mutable_operation::StandardArithmeticOperatorGenerator().get_plus();

    while (true){
        
        std::cout << "enter n: ";
        std::cin >> n;

        StandardMutableBigNum num(1);
        StandardMutableBigNum inc(1);

        auto start = steady_clock().now();
        
        for (size_t i =  1; i < n; ++i){

            num += inc;

        }

        std::cout << num.to_string() << std::endl;
        auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();
        std::cout << lapsed << std::endl;

    }
    
    ResourceDestructor();

}