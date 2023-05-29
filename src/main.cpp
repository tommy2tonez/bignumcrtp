#include "MultithreadedFFT.h"
#include <iostream>
#include "limits.h"
#include <memory>
#include <chrono>
#include "time.h"


std::size_t sizetize(size_t * conv, size_t n, size_t bob){
    
    size_t rs = 0;
    size_t multiplier = 1;
    
    for (intmax_t i = bob; i < n; ++i){
        
        rs += conv[i] * multiplier;
        multiplier *= 10;

    }

    return rs; 

}

void randomize(size_t * arr, size_t n){

    size_t max = 1 << 31;

    
    for (size_t i = 0; i < n; ++i){

        arr[i] = rand() % max;

    }

} 

void qs(size_t * arr, size_t n){
    
    if (n <= 1){

        return;

    }

    size_t pivot = arr[n - 1];
    size_t split_point = 0;

    for (size_t i = 0; i < n; ++i){

        if (arr[i] < pivot){

            std::swap(arr[i], arr[split_point]);
            split_point++;

        }

    }

    std::swap(arr[split_point], arr[n - 1]);

    qs(arr, split_point);
    qs(arr + split_point + 1, n - split_point - 1);

}

int main(){

    std::srand(std::time(0));

    //auto fft = fft::StandardFastInPlaceTransformGenerator().get(new memory::linear::ExponentialLinearStorageBlockGenerator(1024));
    auto fft = multithreading_fft::MultithreadFFTGenerator().get(5, new memory::linear::ExponentialLinearStorageBlockGenerator(1024));
    
    size_t sz = 1 << 25; 
    size_t bound = 1 << 20; 
    size_t test_cases = 5;

    for (size_t j = 0; j < test_cases; ++j){

        fft::ComplexNumber * arr = (fft::ComplexNumber *) std::malloc(sz * sizeof(fft::ComplexNumber));

        for (size_t i = 0; i < sz; ++i){

            arr[i].real = rand() % bound;
            arr[i].im = 0;  

        }

        auto start = std::chrono::steady_clock::now();
        fft->transform(arr, sz);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

        std::cout << duration << std::endl;

    }

    size_t * arr = (size_t *) std::malloc(sz * sizeof(size_t));
    randomize(arr, sz);

    auto start = std::chrono::steady_clock::now();
    auto lambda = [](const void * lhs, const void * rhs){return (((size_t *) lhs)[0] > ((size_t *) rhs)[0]) ? 1 : (((size_t *) lhs)[0] == ((size_t *) rhs)[0]) ? 0 : -1;};

    std::sort(arr, arr + sz);

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    
    std::cout << duration << std::endl;


    // std::srand(std::time(0));

    // auto start = std::chrono::high_resolution_clock::now();
    // //double val = 1e-16;
    // size_t n = 10000000;
    // size_t * arr = (size_t *) std::malloc(sizeof(size_t) * n);
    
    // randomize(arr, n);
    // qs(arr, n);

    // auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

    // std::cout << dif;

    // memory::linear::TempStorageGeneratable * allocator = new memory::linear::ExponentialLinearStorageBlockGenerator(1024);
    // auto convolutor = polynomial::StandardUINTConvolutorGenerator().set_temp_storage(allocator).get();

    
    // std::vector<size_t> a = {6};
    // std::vector<size_t> b = {7, 5, 4};

    // void * rs = allocator->get(convolutor->get_computation_space_cost(a.size(), b.size())); 
    // size_t rs_length = 0;

    // convolutor->convolute(a.data(), a.size(), b.data(), b.size(), rs, rs_length);

    // for (size_t i = 0; i < rs_length; ++i){

    //     std::cout << ((size_t *) rs)[i] << std::endl;

    // }

    // std::cout << sizetize((size_t *) rs, rs_length, 1);

}