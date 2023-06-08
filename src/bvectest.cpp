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

    dgstd::BigUINT rs = 1;
    const size_t MAX_VAL = (1ULL << 62) - 1; 

    for (size_t i = 0; i < sz; ++i){

        rs <<= 62;
        rs += dis(gen) % MAX_VAL;

    }

    return rs;
    
} 

int main(){

    size_t x = 0;
    size_t x1 = 0;

    std::string inp;
    ResourceInitiator(1);

    while (true){
        
        std::cout << "x:";
        std::cin >> x;
        
        std::cout << "x1:";
        std::cin >> x1;

        dgstd::BigUINT val = 1;
        dgstd::BigUINT val2 = 1;

        val <<= (1ULL << x);
        val2 <<= (1ULL << x1);
        
        auto start = steady_clock().now();
        auto val3 = val / val2;
        auto lapsed = duration_cast<milliseconds>(steady_clock().now() - start).count();

        std::cout << val3.length() << "<length>" << lapsed << "<milliseconds>" << std::endl;
        
    }
    
    ResourceDestructor();

}