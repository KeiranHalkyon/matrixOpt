#include <iostream>
#include <bitset>
#include <fstream>
#include <random>

int main() {
    // no of sellers
    const int columns = 10;
    const int rows = 3;

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distribution(0.3);
    std::bernoulli_distribution dist_dense(0.6);
    std::bernoulli_distribution dist_sparse(0.01);
    std::bernoulli_distribution dist_fill(0.9);

    //dataset file
  	std::ofstream outFile("dataset1.txt");

    std::bitset<columns> bits;
    outFile << "No. of rows(pincodes) = "<<rows<<"\n";
    outFile << "No. of columns(sellers) = "<<columns<<"\n";

    int i,j;
	//no of sellers
	for(j=0;j<rows;j++)
	{
	    for (i = 0; i < columns; ++i) 
        {
            if (i % 10==0)
                bits[i] = dist_sparse(gen);
            else if (i % 20 == 0)
                bits[i] = dist_dense(gen);
            else if (i % 50 == 0)
                bits[i] = dist_fill(gen);
        	else 
                bits[i] = distribution(gen);
        }
        outFile << bits;
		outFile << "\n";
	}
        outFile.close();
    return 0;
}
