#include <iostream>
#include <bitset>
#include <fstream>
#include <random>

int main() 
{
    //columns = sellers
    //rows = pincodes
    const int columns = 10;
    const int rows = 3;

    std::ofstream outFile("dataset.txt");

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distribution(0.3);
    //dataset file
  	
    std::bitset<columns> bits;
    outFile << "No. of rows(pincodes) = "<<rows<<"\n";
    outFile << "No. of columns(sellers) = "<<columns<<"\n";

    int i,j;
	for(j=0;j<rows;j++)
	{
	    for (i = 0; i < columns; ++i) 
            bits[i] = distribution(gen);
        outFile << bits;
		outFile << "\n";
	}
        outFile.close();
    return 0;
}
