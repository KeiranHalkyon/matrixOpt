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
        //std::vector<std::string> rowData;
        //std::vector<bool> isZero, isPresent;
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
            //isZero.resize(rows);
            //isPresent.resize(rows);
            finalized=false;
        }

        bool encode8(bool override=false){
            int rown=0, noz, noo, currLength;
            std::string currentRow;
            int *diffs; // store differences for each indices within a row
            std::string outerDir = filename.substr(0,filename.length()-4);
            std::filesystem::create_directory(outerDir);
            while(rown<rows){
                std::getline(fileIterator,currentRow);

                std::string innerDir = outerDir + "/" + std::to_string(rown/1000);
                if(!override && std::filesystem::exists(innerDir+"/"+"finalized")){
                    currentRow = "";
                    currentRow.shrink_to_fit();
                    delete[] diffs;
                    rown++;
                    continue;
                }

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
                //isZero[rown] = false;
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

                    
                    diffIndex++;
                    //std::cin.ignore();
                }
                //
                // now either write to file, or add to memory
                //
                std::string innerDir = outerDir + "\\" + std::to_string(rown/1000),
                    innerFile = innerDir+"\\"+std::to_string(rown%1000)+"_"+chToCheck;
                std::cout << innerDir << " " << innerFile << std::endl;
                std::filesystem::create_directory(innerDir);
                std::ofstream outfile(innerFile,std::ios::binary | std::ios::out);

                outfile << currentRow;
                outfile.close();

                //produce finalized file if last row of the dir or dataset
                if(rown%1000 == 999 || rown == (rows-1)){
                    std::ofstream finalizedFile(innerDir+"/"+"finalized");
                    finalizedFile.close();
                }

                //bookkeeping tasks to reduce memory footprint
                currentRow = "";
                currentRow.shrink_to_fit();
                delete[] diffs;
                rown++;
            }
            return true;
        }

        bool encode4(bool override=false){
            int rown=0, noz, noo, currLength;
            std::string currentRow;
            int *diffs; // store differences for each indices within a row
            std::string outerDir = filename.substr(0,filename.length()-4);
            std::filesystem::create_directory(outerDir);
            while(rown<rows){
                std::getline(fileIterator,currentRow);

                std::string innerDir = outerDir + "/" + std::to_string(rown/1000);
                if(!override && std::filesystem::exists(innerDir+"/"+"finalized")){
                    currentRow = "";
                    currentRow.shrink_to_fit();
                    delete[] diffs;
                    rown++;
                    continue;
                }

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
                //isZero[rown] = false;
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
                
                //std::cout << prev << std::endl;
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
                
                std::string innerFile = innerDir+"\\"+std::to_string(rown%1000)+"_"+chToCheck;
                std::cout << innerDir << " " << innerFile << std::endl;
                std::filesystem::create_directory(innerDir);
                std::ofstream outfile(innerFile,std::ios::binary | std::ios::out);

                outfile << currentRow;
                outfile.close();

                //produce finalized file if last row of the dir or dataset
                if(rown%1000 == 999 || rown == (rows-1)){
                    std::ofstream finalizedFile(innerDir+"/"+"finalized");
                    finalizedFile.close();
                }

                //bookkeeping tasks to reduce memory footprint
                currentRow = "";
                currentRow.shrink_to_fit();
                delete[] diffs;
                rown++;
            }
            return true;
        }

        const std::vector<signed int> decode8(int row){
            std::string rowFileName = filename.substr(0,filename.length()-4) + "/" + std::to_string(row/1000) + "\\" + std::to_string(row%1000);
            std::ifstream rowEncF;

            //check if 0 or 1 is the encoded char
            char encodedChar;

            if(std::filesystem::exists(rowFileName+"_1")){
                encodedChar = '1';
                rowFileName += "_1";
                rowEncF = std::ifstream(rowFileName, std::ios::binary);
            }
            else{
                encodedChar = '0';
                rowFileName += "_0";
                rowEncF = std::ifstream(rowFileName, std::ios::binary);
            }

            int byteNo = 0;
            int prevSum = -1;
            unsigned int buffer = 0;
            std::vector<signed int> indices;

            //std::cout <<"Encoded string length : "<< encodedRow.length() << std::endl;
            char currCh;

            while(rowEncF.get(currCh)){
                buffer |= (currCh & 0x7f) << (7*byteNo);
                if((currCh & 0x80) == 0){
                    int newValue = prevSum + buffer;
                    if(encodedChar == '1')
                        indices.push_back(newValue);
                    else{
                        for(int i=prevSum+1;i<newValue;i++)
                            indices.push_back(i);
                    }
                    prevSum = newValue;
                    //if (i<200) std::cout << newValue << " ";
                    buffer = byteNo = 0;
                }
                else byteNo++;
            }

            //add in remaining 0s if encoded char is 0

            if(encodedChar == '1' && indices.size()==0){
                //-1 indicates no 1s are present
                indices.push_back(-1);
            }
            else if(encodedChar == '0'){
                if(indices.size()==0){
                //-2 indicates all are 1s
                indices.push_back(-2);
                }
                else if(prevSum<(cols-1)){
                    for(int i = prevSum+1 ; i<cols; i++)
                        indices.push_back(i);
                }
            }

            indices.shrink_to_fit();
            //std::cout << "\n" << indices.back() << std::endl;
            /*
            for(int i=0;i<100;i++)
                    std::cout << indices[i] << " ";
            */
            return indices;
        }
    
        const std::vector<signed int> decode4(int row){
            std::string rowFileName = filename.substr(0,filename.length()-4) + "/" + std::to_string(row/1000) + "\\" + std::to_string(row%1000);
            std::ifstream rowEncF;

            //check if 0 or 1 is the encoded char
            char encodedChar;

            if(std::filesystem::exists(rowFileName+"_1")){
                encodedChar = '1';
                rowFileName += "_1";
                rowEncF = std::ifstream(rowFileName, std::ios::binary);
            }
            else{
                encodedChar = '0';
                rowFileName += "_0";
                rowEncF = std::ifstream(rowFileName, std::ios::binary);
            }

            int bit4No = 0;
            int prevSum = -1;
            unsigned int buffer = 0;
            std::vector<signed int> indices;

            //store current character/buffer
            char currCh;
            //whether currCh has been used before
            bool currChUsed = true;

            size_t i=0;
            while(true){
                signed int mask, currValue;
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
                    int newValue = prevSum + buffer;
                    if(encodedChar == '1')
                        indices.push_back(newValue);
                    else{
                        for(int i=prevSum+1;i<newValue;i++)
                            indices.push_back(i);
                    }
                    prevSum = newValue;
                    //if (i<200) std::cout << newValue << " ";
                    buffer = bit4No = 0;
                    
                }
                else bit4No++;
            }

            //add in remaining 0s if encoded char is 0

            if(encodedChar == '1' && indices.size()==0){
                //-1 indicates no 1s are present
                indices.push_back(-1);
            }
            else if(encodedChar == '0'){
                if(indices.size()==0){
                //-2 indicates all are 1s
                indices.push_back(-2);
                }
                else if(prevSum<(cols-1)){
                    for(int i = prevSum+1 ; i<cols; i++)
                        indices.push_back(i);
                }
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