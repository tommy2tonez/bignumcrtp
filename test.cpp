#include <iostream>
#include <vector>

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

int main(){

    decltype(B::get_type()) b = 2;

    std::cout << C<decltype(B::get_type())>::val; 

}

