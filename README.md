# Practice project: webserver
The goal of the project is to write simple HTTP server with select and configuration file.

#### Command to compile: 
`make`

#### Command to run: 
`./webserv <config.conf>`

#### list of features:
* Non-blocking IO
* Can listen on multiple ports
* Dynamically generated error pages 
* Clients can upload files to the server 
* Support GET, POST and DELETE methods
* Accurate HTTP response status codes
* Support PHP-CGI
* Support redirection 
* Support autoindex
* Passes stress test by siege program with 100% availability

There are some configaration files and basic sites to test/demonstrate every feauture.
