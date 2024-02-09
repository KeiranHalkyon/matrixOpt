#include <iostream>
#include <bitset>
#include <fstream>
#include <random>

int main() {
    // no of sellers
    const int columns = 10000;
    const int rows = 300;

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distribution(0.3);
    std::bernoulli_distribution dist_dense(0.6);
    std::bernoulli_distribution dist_sparse(0.01);
    std::bernoulli_distribution dist_fill(0.99);

    //dataset file
  	std::ofstream outFile("dataset_"+std::to_string(rows)+"_"+std::to_string(columns)+".txt");

    std::bitset<columns> bits;
    outFile <<rows<<"\n";
    outFile <<columns<<"\n";

    int i,j;
	//no of sellers
	for(j=0;j<rows;j++)
	{
	    for (i = 0; i < columns; ++i) 
        {
            if (j % 50==0)
                bits[i] = dist_sparse(gen);
            else if (j % 20 == 0)
                bits[i] = dist_dense(gen);
            else if (j % 10 == 0)
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
