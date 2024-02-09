#include<iostream>
#include<chrono>
#include "EncodedMatrix.hpp"

std::string fname = "D:\\Documents\\matrixOpt\\dataset_300_10000.txt";

int main(){
    std::chrono::steady_clock::time_point begin, end;
    EncodedMatrix em(fname);
    //em.rows = 1;
    std::cout << em.rows << " " << em.cols << std::endl;
    //em.encode8();
    //std::cout << "Encoding done\n 0th row decoding" << std::endl;
    
    begin = std::chrono::steady_clock::now();
    std::vector<signed int> vals = em.decode8(10);
    end = std::chrono::steady_clock::now();
    std::cout<< "Decode values : ";
    for(auto& elem:vals){
        std::cout << elem << " ";
    }
    std::cout << "Decoded in " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    
}

/*int main(){
    std::cout << sizeof(int);
}*/