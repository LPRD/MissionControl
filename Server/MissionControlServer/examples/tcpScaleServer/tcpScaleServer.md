## tcpScaleServer example
This example demonstrates the transfer of multiple data types (strings, integers, and floats) between 2 C++ programs on the network. A TCP socket client is hosted in the client program and this program is the server. A basic command word interface is set up and used. The code in this example can be expanded upon to allow for sensor data to be sent to another system on the network.

## Build
use the following commands to build this example:
(run commands from the tcpScaleClient directory)
```bash
make
make all
```

## Run
1. Start the client program

2. Start the executable for this example using:
```bash
make run
# or
make ARGS="<database-ip> <server-port>" run
# or
make run ARGS="<database-ip> <server-port>"
# or 
./tcpScaleServer database-ip server-port
# use the forwarded port of the server container
# ex:
make run ARGS="10.80.22.9 13056"    # individual container 
make run ARGS="10.80.23.9 13056"    # combined docker-compose container
# read Makefile to see available commands
```




