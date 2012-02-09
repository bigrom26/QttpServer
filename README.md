# node.native 

<b>node.native</b> is a [C++11](http://en.wikipedia.org/wiki/C%2B%2B11) (aka C++0x) port for [node.js](https://github.com/joyent/node). 

Please note that <b>node.native</b> project is <em>under heavy development</em>: currently experimental.

## Sample code

Simplest web-server example using node.native.

    #include <iostream>
    #include "http.h"
    using namespace native::http;
    
    int main()
    {
        http server;
        if(server.listen("0.0.0.0", 8080, [](request& req, response& res){
            res.set_status(200);
            res.set_header("Content-Type", "text/plain");
            res.end("C++ FTW\n");
        })) std::cout << "Server running at http://0.0.0.0:8080/" << std::endl;
    
        return native::run();
    }

## Getting started

<em>node.native</em> consists of header files(*.h) only, but requires [libuv](https://github.com/joyent/libuv) and [http-parser](https://github.com/joyent/http-parser) lib to use.

To compile included sample application(sample.cpp):

    export LIBUV_PATH=/path/to/libuv_dir
    export HTTP_PARSER_PATH=/path/to/http-parser_dir

then,

    make

I tested the code on Ubuntu 11.10 and GCC 4.6.1.

## Todos

- Match class and function names to those of Node.js as best as possible. 