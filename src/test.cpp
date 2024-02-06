#include<iostream>
#include<chrono>
#include "EncodedMatrix.hpp"

std::string fname = "D:\\Documents\\dataset(10000000_300).txt";

int main(){
    std::chrono::steady_clock::time_point begin, end;
    EncodedMatrix em(fname);
    //em.rows = 1;
    std::cout << em.rows << " " << em.cols << std::endl;
    //em.encode();
    //std::cout << "Encoding done\n 0th row decoding" << std::endl;
    begin = std::chrono::steady_clock::now();
    em.decode(0);
    end = std::chrono::steady_clock::now();
    
    std::cout << "Decoded in " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
}

/*int main(){
    std::cout << sizeof(int);
}*/