#ifndef __VECTOR_UTILITY_H__
#define __VECTOR_UTILITY_H__

#include "functional"
#include <vector>

namespace utility::vector{

    template <typename T>
    void filter(std::vector<T>& data, std::function<bool(T&)> filter_lambda){

        if (data.size() == 0){
            
            return;
       
        }

        uint64_t back_ptr = data.size() - 1;
        
        for (int64_t i = data.size() - 1; i >= 0; --i){

            if (filter_lambda(data[i])){

                std::swap(data[i], data[back_ptr--]);

            }

        }

        while (data.size() != back_ptr + 1){

            data.pop_back();

        }

    }

    template <typename T, typename Functor>
    void filter(std::vector<T>& data, Functor filter_lambda){

        if (data.size() == 0){
            
            return;
       
        }

        uint64_t back_ptr = data.size() - 1;
        
        for (int64_t i = data.size() - 1; i >= 0; --i){

            if (filter_lambda(data[i])){

                std::swap(data[i], data[back_ptr--]);

            }

        }

        while (data.size() != back_ptr + 1){

            data.pop_back();

        }

    }

    template <typename T>
    void combine(std::vector<T>& appendee, const std::vector<T>& appender){

        appendee.reserve(appendee.size() + appender.size());
        appendee.insert(appendee.end(), appender.begin(), appender.end());

    }

    template<typename R, typename L, typename T>
    void map(std::vector<R>& transformer, std::vector<L>& transformee, T& performer){

        transformee.reserve(transformer.size());

        for (auto it = transformer.begin(); it != transformer.end(); ++it){

            transformee.push_back(performer(*it));

        }
    
    }

    template<typename T>
    std::vector<T> pop_back(std::vector<T>& data, uint64_t n){

        std::vector<T> rs;

        if (n >= data.size()){

            rs = std::move(data);
            data = {};

            return rs;

        }

        rs.reserve(n);
        rs.insert(rs.end(), data.end() - n, data.end());

        for (uint64_t i = 0; i < n; ++i){

            data.pop_back();

        }

        return rs;

    }

    template<typename CTN, typename T>
    std::vector<T> convert(CTN& container){

        std::vector<T> rs;

        rs.reserve(container.size());

        for (auto it = container.begin(); it != container.end(); ++it){

            rs.push_back(*it);
        
        }

        return rs;
        
    };

    std::vector<uint64_t> range_inclusive(uint64_t l, uint64_t r, uint64_t delta){

    };

}

#endif