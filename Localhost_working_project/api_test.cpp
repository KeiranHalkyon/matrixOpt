#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class HttpServer
{
public:
    HttpServer(io_service &io_service, short port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
          socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       cout << "Incoming connection from: " << socket_.remote_endpoint().address().to_string() << endl;
                                       handle_request();
                                   }

                                   do_accept();
                               });
    }

    void handle_request()
    {
        boost::system::error_code error;
        array<char, 8192> buffer;

        // Read request from client
        size_t length = socket_.read_some(buffer, error);
        if (error == boost::asio::error::eof)
        {
            cout << "Connection closed by client." << endl;
            return;
        }
        else if (error)
        {
            throw boost::system::system_error(error);
        }

        // Parse request
        string request(buffer.data(), length);
        cout << "Received request: " << request << endl;

        // Check if it's a GET or POST request
        string response;
        if (request.find("GET /hello HTTP/1.1") != string::npos)
        {
            response = "HTTP/1.1 200 OK\r\nContent-Length: 16\r\n\r\nHello, GET request!";
        }
        else if (request.find("POST /post HTTP/1.1") != string::npos)
        {
            // Extract key-value pairs from POST request
            size_t pos = request.find("\r\n\r\n");
            if (pos != string::npos)
            {
                string body = request.substr(pos + 4);
                size_t key_pos = body.find("key=");
                size_t value_pos = body.find("value=");
                if (key_pos != string::npos && value_pos != string::npos)
                {
                    string key = body.substr(key_pos + 4, value_pos - (key_pos + 4) - 1);
                    string value = body.substr(value_pos + 6);
                    response = "HTTP/1.1 200 OK\r\nContent-Length: " + to_string(key.length() + value.length() + 2) + "\r\n\r\nkey: " + key + ", value: " + value;
                }
                else
                {
                    response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
                }
            }
            else
            {
                response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            }
        }
        else
        {
            response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
        }

        // Send response to client
        boost::asio::write(socket_, boost::asio::buffer(response), error);
        if (error)
        {
            throw boost::system::system_error(error);
        }
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main()
{
    cout << "The Program has started executing" << endl;
    try
    {
        boost::asio::io_service io_service;
        HttpServer server(io_service, 8080);
        io_service.run();
    }
    catch (const std::exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}