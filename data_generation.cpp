#include <iostream>
#include <bitset>
#include <fstream>
#include <random>

int main() {
    // no of pincodes
    const int bitsetSize = 300;

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distribution(0.3);
    //dataset file
  	std::ofstream outFile("dataset(30*10000).txt");

    std::bitset<bitsetSize> bits;

	//no of sellers
	for(int j=0;j<10000;j++)
	{
	    for (int i = 0; i < bitsetSize; ++i) 
        	bits[i] = distribution(gen);
        outFile << bits;
		outFile << "\n";
	}
        outFile.close();

        std::cout << "Done" << std::endl;
    return 0;
}
