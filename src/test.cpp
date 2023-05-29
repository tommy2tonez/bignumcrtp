#include "BigNum.h"
#include "chrono" 
#include <fstream>
#include <unordered_map>

bool read(std::string &file_path, char *& buffer, size_t& size){

    std::ifstream file(file_path, std::ios::binary);

    file.seekg(0, std::ios::end);
    
    size = file.tellg();
    buffer = (char *) malloc(size * sizeof(char));

    file.seekg(0, std::ios::beg);

    if (file.read(buffer, size))
    {
        file.close();

        return true; 
    }

    file.close();

    return false;

}

std::vector<std::string> delim(char * buffer, size_t size, char delimiter){

    std::vector<std::string> rs;
    std::string cur = "";

    for (size_t i = 0; i < size; ++i){

        if (buffer[i] == delimiter){
            
            rs.push_back(cur);
            cur = "";
        
        } else{

            cur += buffer[i];
        
        }

    }

    if (cur != ""){

        rs.push_back(cur);

    }

    return rs;

}

bool test_plus(std::string lhs, std::string rhs, std::string rs){

    return dgstd::BigUINT(lhs) + dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

bool test_minus(std::string lhs, std::string rhs, std::string rs){

    return dgstd::BigUINT(lhs) - dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

bool test_mul(std::string lhs, std::string rhs, std::string rs){
    
    return dgstd::BigUINT(lhs) * dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

bool test_div(std::string lhs, std::string rhs, std::string rs){

    return dgstd::BigUINT(lhs) / dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);
 
} 

bool test_and(std::string lhs, std::string rhs, std::string rs){

    return (dgstd::BigUINT(lhs) & dgstd::BigUINT(rhs)) == dgstd::BigUINT(rs);

} 

bool test_or(std::string lhs, std::string rhs, std::string rs){
    
    return (dgstd::BigUINT(lhs) | dgstd::BigUINT(rhs)) == dgstd::BigUINT(rs);

} 

bool test_xor(std::string lhs, std::string rhs, std::string rs){
    
    return (dgstd::BigUINT(lhs) ^ dgstd::BigUINT(rhs)) == dgstd::BigUINT(rs);

} 

bool test_lshift(std::string lhs, std::string rhs, std::string rs){

    return dgstd::BigUINT(lhs) << dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

bool test_rshift(std::string lhs, std::string rhs, std::string rs){
    

    return dgstd::BigUINT(lhs) >> dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

bool test_mod(std::string lhs, std::string rhs, std::string rs){
    
    return dgstd::BigUINT(lhs) % dgstd::BigUINT(rhs) == dgstd::BigUINT(rs);

} 

int main(){

    bignum::integer::usgn::ResourceInitiator(5);

    typedef bool (*lambda)(std::string, std::string, std::string);
    std::unordered_map<std::string, lambda> test_map;

    test_map["plus"] = test_plus;
    test_map["minus"] = test_minus;
    test_map["div"] = test_div;
    test_map["mul"] = test_mul;
    test_map["and"] = test_and;
    test_map["or"] = test_or;
    test_map["xor"] = test_xor;
    test_map["lshift"] = test_lshift;
    test_map["rshift"] = test_rshift;
    test_map["mod"] = test_mod;

    std::string fname = "bignum_test/bignum_test.data";

    char * buf = nullptr;
    size_t buf_length = 0;
    size_t passed = 0;

    read(fname, buf, buf_length);

    std::vector<std::string> lines = delim(buf, buf_length, '\t');
    
    for (size_t i = 0; i < lines.size(); ++i){

        std::vector<std::string> splitted = delim(lines[i].data(), lines[i].size(), ',');

        //std::cout << splitted[0] << std::endl;

        if (test_map[splitted[0]](splitted[1], splitted[2], splitted[3])){

            ++passed;

        } else{
            
            assert(false);
            std::cout << splitted[0] << std::endl;
            std::cout << splitted[1] << " " << splitted[2] << " " << splitted[3] << std::endl;

        }

        if (passed % 10000 == 0){

            std::cout << passed << std::endl;

        }
        
    }

    std::cout << passed << "/" << lines.size() << std::endl;

}