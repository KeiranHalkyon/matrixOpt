#include <iostream>
#include <string>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <typeinfo>
#include "EncodedMatrix.hpp"


using namespace std ;
namespace beast = boost::beast; 
namespace http = beast::http; 
namespace net = boost::asio;
using tcp = net::ip::tcp;

EncodedMatrix em("dataset(300_10000).txt");

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    if (req.method() == http::verb::get && req.target() == "/api/resource") {
        // Handle GET request to /api/resource
        res.result(http::status::ok);
        res.body() = "Response received.\n";
    } else if (req.method() == http::verb::post && req.target() == "/api/data") {
        res.result(http::status::created);
        vector<unsigned int> gyb = em.decode4(stoi(req.body())) ;
        string str = "" ;
        for(unsigned int item:gyb){
          str = str + to_string(item) + ",";
        }
        //if(req.body == )
        
        res.body() = "Received POST request with data: " + str + "\n";
    } else {
        // Handle invalid endpoint
        res.result(http::status::not_found);
        res.body() = "404 Not Found";
    }

    res.set(http::field::server, "Boost");
    res.set(http::field::content_type, "text/plain");
    res.prepare_payload();
}

int main() {
    cout << "We are inside the API code." << endl ;
    // Step 1: Create the IO context
    net::io_context ioc{1};
    em.rows = 10;
    em.encode4();
    // Step 2: Create and bind the acceptor
    tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};

    while (true) {
        // Step 3: Accept incoming connections
        tcp::socket socket{ioc};
        acceptor.accept(socket);

        // Step 4: Read request
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // Step 5: Process request and generate response
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        handle_request(req, res);

        // Step 6: Send response
        http::write(socket, res);

        // Step 7: Gracefully close the socket
        socket.shutdown(tcp::socket::shutdown_send);
    }

    return 0;
}
