#include <iostream>
#include <string>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <typeinfo>
#include "../../src/EncodedMatrix.hpp"
//#include "hash_implement.h"
#include <sstream>

using namespace std;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

EncodedMatrix em("dataset(300_10000).txt");
std::unordered_map<int, int> pincodeMap;

void handle_request(const http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    if (req.method() == http::verb::get && req.target() == "/api/resource")
    {
        // Handle GET request to /api/resource
        res.result(http::status::ok);
        res.body() = "Response received.\n";
    }
    else if (req.method() == http::verb::post && req.target() == "/api/data")
    {
        res.result(http::status::created);
        
        std::string inputPincode;
    	//std::cout << "Enter the pincode: ";
    	//std::cin >> inputPincode;
    	inputPincode = req.body() ;
    	auto it = pincodeMap.find(stoi(inputPincode));
    	if (it != pincodeMap.end()) {
        //std::cout << "Pincode " << inputPincode << " found at line number: " << it->second << std::endl;
    	//vector<int> gyb = em.decode4(stoi(req.body()));
        vector<int> gyb = em.decode4(it->second);
        string str = "";
        if (gyb[0] == -1)
            str = "" ;
        else if (gyb[0] == -2){
            stringstream ss ;
            for(int i = 0 ; i < (em.cols) ; i ++){
                //str += to_string(i) + "," ;
            	ss << i << "," ;
            }
            str = ss.str() ;
        }
        else
        {
            stringstream ss ;
            for (int item : gyb)
            {
                //str = str + to_string(item) + ",";
            	ss << item << "," ;
            }
            str = ss.str() ;
        }
        res.body() = str;
    	} else {
        //std::cout << "Pincode " << inputPincode << " not found." << std::endl;
        res.body() = "-1" ;
    	}
        
        
        
        /*vector<int> gyb = em.decode4(stoi(req.body()));
        string str = "";
        if (gyb[0] == -1)
            str = "" ;
        else if (gyb[0] == -2){
            stringstream ss ;
            for(int i = 0 ; i < (em.cols) ; i ++){
                //str += to_string(i) + "," ;
            	ss << i << "," ;
            }
            str = ss.str() ;
        }
        else
        {
            stringstream ss ;
            for (int item : gyb)
            {
                //str = str + to_string(item) + ",";
            	ss << item << "," ;
            }
            str = ss.str() ;
        }
        res.body() = str;*/
    }
    else
    {
        // Handle invalid endpoint
        res.result(http::status::not_found);
        res.body() = "404 Not Found";
    }

    res.set(http::field::server, "Boost");
    res.set(http::field::content_type, "text/plain");
    res.prepare_payload();
}

int main()
{
    cout << "We are inside the API code." << endl;
    net::io_context ioc{1};
    //em.rows = 10;
    em.encode4();
    
    std::string fileName = "pincode-dataset.txt";
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open the file " << fileName << std::endl;
        return 1;
    }
    //std::unordered_map<int, int> pincodeMap;
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        pincodeMap[stoi(line)] = lineNumber;
        lineNumber++;
    }
    file.close();
    
    tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};

    while (true)
    {   
        try{
            tcp::socket socket{ioc};
            acceptor.accept(socket);
    
            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            handle_request(req, res);
            http::write(socket, res);
            socket.shutdown(tcp::socket::shutdown_send);
        }
        catch(...){
            std::exception_ptr p = std::current_exception();
            std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
        }
    }

    return 0;
}