#include <stdlib.h>         //exit()
#include <chrono>           //time
#include <thread>           //std::this_thread::sleep_for
#include <math.h>
#include <arpa/inet.h>
#include <stdio.h>          //printf
#include <string.h>         //strcpy()
#include <string>           //std::string
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> 

// #include <stdio.h>          //snprintf, fprintf, fopen, etc
// #include <chrono>           //time
// #include <thread>           //std::this_thread::sleep_for
//To debug, add the following lines in the desired location
// FILE *fp;
// fp = fopen("log.txt","a");
// auto begin = std::chrono::steady_clock::now();
// library task
// auto end = std::chrono::steady_clock::now();
// auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
// fprintf(fp, "it took %d us to collect %u readings!\n", diff.count(), avg);
// fclose(fp);

#define NumCells 4

class DataPacket {
  public:
    char cmd[16];           //command variable, 4 byte alligned
    // char cmd2[30];          //additional text
    int data[NumCells];   //sensor data
    // float data[NumCells];   //sensor data float
    // std::string text;       //NEED TO BE 4n byte aligned, strings might be a bad idea
};


int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <server-port>" << std::endl;
        std::cout << "<server-port> should be in the format XXXXX" << std::endl;
        exit(1);
    }

    std::string port(argv[1]);

    if(port.length() > 5){
        std::cout << "<server-port> should be less than 6 characters!" << std::endl;
        exit(1);
    }

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    std::string hello = "Hello from server";
    // char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(stoi(port));

    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    DataPacket packet;
    char spaces[16] = "               ";
    strcpy(packet.cmd, spaces);
    for (int i = 0; i < NumCells; i++) {
        packet.data[i] = 0;
    }

    while(1)
    {
        // valread = read(new_socket, buffer, 1024);
        // printf("%s\n", buffer);
        // send(new_socket, hello, strlen(hello), 0);
        // printf("Hello message sent\n");

        valread = read(new_socket, &packet, sizeof(packet));
        std::cout << packet.cmd << std::endl;
        for (int i = 0; i < NumCells; i++) {
            std::cout << "\t" << packet.data[i] << std::endl;
        }
        std::cout << std::endl;







        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // int bytesToSend = NumCells*4;

        // strcpy(packet.cmd, "data:int32:");

        // bytesToSend += strlen(packet.cmd); //does not work, must use whole buffer length
        // bytesToSend += 16;  //need to send an amount of bytes that is 4byte aligned
        //NEED TO BE 4 BYTE ALIGNED

        // if(!CalculateZeroOffset && RunSocket){
        //     // printf("sent %d bytes!\n", bytesToSend);
        //     send(client_fd, &packet, bytesToSend, 0);

        //     // valread = read(client_fd, buffer, 1024);    //Move this to a separate task
        //     // printf("%s\n", buffer);
        //     // need to add a way to escape from read after a certain amount of time

        //     int iBuf[1024/4] = { 0 };
        //     valread = read(client_fd, iBuf, 1024);   
        //     for (int i = 0; i < NumCells; i++){
        //         printf("%d\t", iBuf[i]);
        //     }
        //     printf("\n");
        
        //     // float fBuf[1024/4] = { 0 };
        //     // valread = read(client_fd, fBuf, 1024);
        //     // for (int i = 0; i < NumCells; i++){
        //     //     printf("%f\t", fBuf[i]);
        //     // }
        //     // printf("\n");
        // }


    }


}




