#include <iostream>
#include <vector>
#include "src/memcrtp.h"
#include <iostream>

template <class T>
class A{

    private:

        T data;

    public:

        A(T data){

            this->data = data;

        }



};

class B{

    public:

        auto static get_type(){

            return 1;

        }

};

template <class T>
class C{

    public:

        inline static size_t val = 3;


};

constexpr memory::sizet_linear::StandardStackAllocatedVector<1> get_incrementor(){
    
    memory::sizet_linear::StandardStackAllocatedVector<1> inc;
    inc.resize_no_copy(1);
    inc.set(0, 1);

    return inc;

} 

int main(){

    auto rs = get_incrementor();

    std::cout << rs.to_vector_readable()->get(0) << std::endl;

}

