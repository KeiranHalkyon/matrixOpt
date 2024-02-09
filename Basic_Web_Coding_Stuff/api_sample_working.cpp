#include <iostream>
#include <string>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <typeinfo>

using namespace std;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void handle_request(const http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    string response ;
    if (req.method() == http::verb::get && req.target() == "/api/resource")
    {
        // Handle GET request to /api/resource
        res.result(http::status::ok);
        res.body() = "Response received.\n";
    }
    else if (req.method() == http::verb::post && req.target() == "/api/data")
    {
        // Handle POST request to /api/data
        res.result(http::status::created);

        res.body() = "Received POST request with data: " + req.body() + "\n";
    }
    // else if (req.find("POST /post HTTP/1.1") != string::npos)
    //     {
    //         //string response;
    //         // Extract key-value pairs from POST request
    //         size_t pos = req.find("\r\n\r\n");
    //         if (pos != string::npos)
    //         {
    //             string body = req.substr(pos + 4);
    //             size_t key_pos = body.find("key=");
    //             size_t value_pos = body.find("value=");
    //             if (key_pos != string::npos && value_pos != string::npos)
    //             {
    //                 string key = body.substr(key_pos + 4, value_pos - (key_pos + 4) - 1);
    //                 string value = body.substr(value_pos + 6);
    //                 response = "HTTP/1.1 200 OK\r\nContent-Length: " + to_string(key.length() + value.length() + 2) + "\r\n\r\nkey: " + key + ", value: " + value;
    //             }
    //             else
    //             {
    //                 response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
    //             }
    //         }
    //         else
    //         {
    //             response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
    //         }
    //     }
    
    
    else
    {
        // Handle invalid endpoint
        res.result(http::status::not_found);
        res.body() = "404 Not Found";
    }
    // boost::asio::write(socket_, boost::asio::buffer(response), error);
    res.set(http::field::server, "Boost");
    res.set(http::field::content_type, "text/plain");
    res.prepare_payload();
}

int main()
{
    cout << "We are inside the API code." << endl;
    // Step 1: Create the IO context
    net::io_context ioc{1};

    // Step 2: Create and bind the acceptor
    tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};

    while (true)
    {
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