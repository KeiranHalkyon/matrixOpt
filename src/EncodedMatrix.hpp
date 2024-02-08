#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include<chrono>
#include<algorithm>
#include<vector>
#include<filesystem>
//#include<boost/dynamic_bitset.hpp>
#include<boost/range/algorithm/count.hpp>

#include<bitset>
#include<assert.h>
#include<cstdio>

void skipBOM(std::ifstream &in)
{
    char test[3] = {0};
    in.read(test, 3);
    if ((unsigned char)test[0] == 0xEF && 
        (unsigned char)test[1] == 0xBB && 
        (unsigned char)test[2] == 0xBF)
    {
        return;
    }
    in.seekg(0);
}

void skipBOMfp(FILE *fp){
    if(fgetc(fp) == 0xEF && fgetc(fp) == 0xBB && fgetc(fp) == 0xBF)
        return;
    fseek(fp, 0, SEEK_SET);
}

class EncodedMatrix{
    public:
        std::string filename;
        int rows, cols;
        std::vector<std::string> rowData;
        std::vector<bool> isZero, isPresent;
        std::ifstream fileIterator;
        bool finalized;

        EncodedMatrix(std::string filename){
            //add file presence checker
            this->filename=filename;
            fileIterator=std::ifstream(filename);
            skipBOM(fileIterator);
            std::string temp;

            getline(fileIterator,temp);//extracting no. of rows from dataset
            this->rows = std::stoi(temp);

            getline(fileIterator,temp);//extracting no. of columns from dataset
            this->cols = std::stoi(temp);

            //rowData = new 
            isZero.resize(rows);
            isPresent.resize(rows);
            finalized=false;
        }

        bool encode8(){
            int rown=0, noz, noo, currLength;
            std::string currentRow;
            int *diffs; // store differences for each indices within a row
            std::string outerDir = filename.substr(0,filename.length()-4);
            std::filesystem::create_directory(outerDir);
            while(rown<rows){
                std::getline(fileIterator,currentRow);
                noz = boost::count(currentRow,'0');
                noo=cols-noz;
                char chToCheck;
                //make a check for noo or noz == 0
                if(noo<=noz){
                    chToCheck = '1';
                    currLength = noo;
                }
                else{
                    chToCheck = '0';
                    currLength = noz;
                }

                //count no. of 1's
                isZero[rown] = false;
                diffs = new int[currLength];
                int prev = 0, diffIndex=0;

                //check for consistency of counting b/w noo/noz and diffs[] size

                //cannot send zero to __builtin_clz()
                //so store all indices as +1
                //std::cout << "0th diffs"<<std::endl;
                while(true){
                    int currIndex = currentRow.find(chToCheck,prev)+1;
                    //if(diffIndex<100) std::cout<<currIndex-1<<" ";
                    if(currIndex == 0) break;
                    diffs[diffIndex++] = currIndex - prev;
                    prev = currIndex;
                }
                
                /*
                std::cout << rown << "th diffs" << std::endl;
                for(int i=0;i<100;i++)
                    std::cout << diffs[i] << " ";
                */

                currentRow.clear();
                //storing encoded data in currentRow
                // for now, considering sizeof(int) = 4
                diffIndex = 0;
                while(diffIndex<currLength){
                    unsigned int now = diffs[diffIndex];
                    //formula = ((size taken by int in bits) - no. of leading zero)/7
                    int noOfBytes = ceil((sizeof(int)*8 - __builtin_clz(now))*1.0/7);

                    //apply mask and then add to string
                    unsigned mask = (1 << 7) - 1;
                    char ch;
                    //std::cout<<"hi"<<now<<" "<<noOfBytes<<std::endl;
                    
                    //little endian
                    while(noOfBytes-->0){
                        ch = now & mask ;
                        now >>= 7;
                        if(noOfBytes)
                            ch |= 0x80;
                        currentRow+=ch;
                    }

                    //big endian logic
                    /* 
                    switch(noOfBytes){
                        case 5:ch = (now >> 28) & mask | 0x80;
                                currentRow+=ch;
                                assert(ch>0);
                                //std::cout << now << " " << std::bitset<8>(ch) << std::endl;
                        case 4:ch = (now >> 21) & mask | 0x80;
                                currentRow+=ch;
                                assert(ch>0);
                                //std::cout << now << " " << std::bitset<8>(ch) << std::endl;
                        case 3:ch = (now >> 14) & mask | 0x80;
                                currentRow+=ch;
                                assert(ch>0);
                                //std::cout << now << " " << std::bitset<8>(ch) << std::endl;
                        case 2:ch = (now >> 7) & mask | 0x80;
                                currentRow+=ch;
                                assert(ch>0);
                                std::cout << now << " " << std::bitset<8>(ch) << std::endl;
                        case 1:ch = now & mask;
                                currentRow+=ch;
                                assert(ch>0);
                                //std::cout << now << " " << std::bitset<8>(ch) << std::endl;
                    }
                    */
                    diffIndex++;
                    //std::cin.ignore();
                }
                //
                // now either write to file, or add to memory
                //
                std::string innerDir = outerDir + "\\" + std::to_string(rown/1000),
                    innerFile = innerDir+"\\"+std::to_string(rown%1000);
                std::cout << innerDir << " " << innerFile << std::endl;
                std::filesystem::create_directory(innerDir);
                std::ofstream outfile(innerFile,std::ios::binary | std::ios::out);

                outfile << currentRow;
                outfile.close();

                //bookkeeping tasks to reduce memory footprint
                currentRow = "";
                currentRow.shrink_to_fit();
                delete[] diffs;
                rown++;
            }
            return true;
        }

        bool encode4(){
            int rown=0, noz, noo, currLength;
            std::string currentRow;
            int *diffs; // store differences for each indices within a row
            std::string outerDir = filename.substr(0,filename.length()-4);
            std::filesystem::create_directory(outerDir);
            while(rown<rows){
                std::getline(fileIterator,currentRow);
                noz = boost::count(currentRow,'0');
                noo=cols-noz;
                char chToCheck;
                //make a check for noo or noz == 0
                if(noo<=noz){
                    chToCheck = '1';
                    currLength = noo;
                }
                else{
                    chToCheck = '0';
                    currLength = noz;
                }

                //count no. of 1's
                isZero[rown] = false;
                diffs = new int[currLength];
                int prev = 0, diffIndex=0;

                //check for consistency of counting b/w noo/noz and diffs[] size

                //cannot send zero to __builtin_clz()
                //so store all indices as +1
                //std::cout << "0th diffs"<<std::endl;
                while(true){
                    int currIndex = currentRow.find(chToCheck,prev)+1;
                    //if(diffIndex<100) std::cout<<currIndex-1<<" ";
                    if(currIndex == 0) break;
                    diffs[diffIndex++] = currIndex - prev;
                    prev = currIndex;
                }
                
                std::cout << prev << std::endl;
                /*
                std::cout << rown << "th diffs" << std::endl;
                for(int i=0;i<100;i++)
                    std::cout << diffs[i] << " ";
                */

                currentRow.clear();
                //storing encoded data in currentRow
                // for now, considering sizeof(int) = 4
                diffIndex = 0;

                char currCh = '\0';
                bool currChUsed = false;
                unsigned mask = 0x7;
                while(diffIndex<currLength){
                    unsigned int now = diffs[diffIndex];
                    //formula = ((size taken by int in bits) - no. of leading zero)/3
                    int noOfBytes = ceil((sizeof(int)*8 - __builtin_clz(now))*1.0/3);

                    //std::cout<<"hi"<<now<<" "<<noOfBytes<<std::endl;
                    
                    //little endian
                    while(noOfBytes-->0){
                        currCh |= now & mask ;
                        now >>= 3;
                        if(noOfBytes)
                            currCh |= 0x8;
                        if(currChUsed){
                            currentRow+=currCh;
                            currCh = '\0';
                            currChUsed = false;
                        }
                        else{
                            currCh <<= 4;
                            currChUsed = true;
                        }
                    }

                    diffIndex++;
                    //std::cin.ignore();
                }

                //if the last byte was only filled till the first half, manually push it into string
                if(currChUsed)
                    currentRow += currCh;

                //
                // now either write to file, or add to memory
                //
                std::string innerDir = outerDir + "\\" + std::to_string(rown/1000),
                    innerFile = innerDir+"\\"+std::to_string(rown%1000);
                std::cout << innerDir << " " << innerFile << std::endl;
                std::filesystem::create_directory(innerDir);
                std::ofstream outfile(innerFile,std::ios::binary | std::ios::out);

                outfile << currentRow;
                outfile.close();

                //bookkeeping tasks to reduce memory footprint
                currentRow = "";
                currentRow.shrink_to_fit();
                delete[] diffs;
                rown++;
            }
            return true;
        }

        bool decode8(int row){
            std::string rowFileName = filename.substr(0,filename.length()-4) + "\\" + std::to_string(row/1000) + "\\" + std::to_string(row%1000);
            std::ifstream rowEncF(rowFileName, std::ios::binary);
            skipBOM(rowEncF);

            //std::string encodedRow;
            //std::getline(rowEncF,encodedRow,'\0');

            /*
            FILE *rowEncFile = fopen(rowFileName.c_str(), "rb");
            if( rowEncFile == nullptr){
                perror("Error opening file");
                return 0;
            }
            */

            //skipBOMfp(rowEncFile);

            int byteNo = 0;
            int prevSum = -1;
            unsigned int buffer = 0;
            std::vector<unsigned int> indices;

            //std::cout <<"Encoded string length : "<< encodedRow.length() << std::endl;
            char currCh;
            //rowEncF.clear();
            //for(size_t i = 0;i<encodedRow.length();i++){
            
            //big endian
            /*
            for(size_t i = 0;(currCh = fgetc(rowEncFile)) != EOF;i++){
                if(isLeft) buffer<<7;
                buffer |= currCh & 0x7f;
                if((currCh & 0x80) == 0){
                    isLeft = false;
                    int newValue = prevSum += buffer;
                    if(i<200)std::cout << newValue << " ";

                    indices.push_back(newValue);
                    buffer = 0;
                }
                else isLeft = true;
            }
            */

            for(size_t i = 0;rowEncF.get(currCh);i++){
                buffer |= (currCh & 0x7f) << (7*byteNo);
                if((currCh & 0x80) == 0){
                    int newValue = prevSum += buffer;
                    indices.push_back(newValue);
                    //if (i<200) std::cout << newValue << " ";
                    buffer = byteNo = 0;
                }
                else byteNo++;
            }


            indices.shrink_to_fit();
            //std::cout << "\n" << indices.back() << std::endl;
            for(int i=0;i<100;i++)
                    std::cout << indices[i] << " ";
            return true;
        }
    
        const std::vector<unsigned int> decode4(int row){
            std::string rowFileName = filename.substr(0,filename.length()-4) + "\\" + std::to_string(row/1000) + "\\" + std::to_string(row%1000);
            std::ifstream rowEncF(rowFileName, std::ios::binary);

            int bit4No = 0;
            int prevSum = -1;
            unsigned int buffer = 0;
            std::vector<unsigned int> indices;

            //store current character/buffer
            char currCh;
            //whether currCh has been used before
            bool currChUsed = true;

            size_t i=0;
            while(true){
                unsigned int mask, currValue;
                if(currChUsed){
                    //if EOF has triggered
                    if(!rowEncF.get(currCh))
                        break;
                    currChUsed = false;
                    mask = 0x70;
                    currValue = (currCh & mask) >> 4;
                }
                else{
                    currChUsed = true;
                    mask = 0x7;
                    currValue = (currCh & mask);
                }

                buffer |= currValue << (3*bit4No);

                if((!currChUsed && ((currCh & 0x80) == 0)) || (currChUsed && ((currCh & 0x8) == 0))){
                    //detect last empty half i.e. 0010 0000, where the last half was unfilled
                    if(buffer == 0)
                        continue;
                    int newValue = prevSum += buffer;
                    indices.push_back(newValue);
                    //if (i<200) std::cout << newValue << " ";
                    buffer = bit4No = 0;
                    
                }
                else bit4No++;
            }


            indices.shrink_to_fit();
            //std::cout << "\n" << indices.back() << std::endl;
            /*
            for(int i=0;i<100;i++)
                    std::cout << indices[i] << " ";
            */
            return indices;
        }
};