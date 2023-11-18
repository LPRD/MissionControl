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
#include <pqxx/pqxx>
#include <ctime>
#include <sys/time.h>

// #include <stdio.h>          //snprintf, fprintf, fopen, etc
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
#define DATA_UPLOAD_RATE_HZ 10
#define DATA_UPLOAD_INTERVAL_MS (1000.0/DATA_UPLOAD_RATE_HZ)

using namespace pqxx;

class DataPacket {
  public:
    char cmd[16];           //command variable, 4 byte alligned
    // char cmd2[30];          //additional text
    int data[NumCells];   //sensor data
    // float data[NumCells];   //sensor data float
    // std::string text;       //NEED TO BE 4n byte aligned, strings might be a bad idea
};

void insert_load_cell_data(connection &C, std::time_t t, float runTime, float lc1, float lc2, float lc3, float lc4)
{
    // TODO see if there is another way to get system time with millisecond accuracy
    try
    {
        std::tm* timeinfo;
        char buffer [80];

        int millisec;
        timeval tv;
        gettimeofday(&tv, NULL);

        millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
        if (millisec>=1000) { // Allow for rounding up to nearest second
            millisec -=1000;
            tv.tv_sec++;
        }

        timeinfo = localtime(&tv.tv_sec);

        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

        // Create a transactional object.
        work W_INSERT(C);

        // INSERT SQL statement
        std::string command = "INSERT INTO LOAD_CELL_DATA " \
            "(TIME_STR,RUN_TIME,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
            "VALUES ('" + 
            std::string(buffer) + "', " +
            std::to_string(runTime) + ", " + 
            std::to_string(lc1) + ", " + 
            std::to_string(lc2) + ", " + 
            std::to_string(lc3) + ", " + 
            std::to_string(lc4) + "); ";
        // Execute SQL query
        W_INSERT.exec( command );
        W_INSERT.commit();
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << std::endl;
      exit (1);
   }
}


int main(int argc, char* argv[])
{
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <database-ip> <server-port>" << std::endl;
        std::cout << "<database-ip> should be in the format X.X.X.X" << std::endl;
        std::cout << "<server-port> should be in the format XXXXX" << std::endl;
        exit(1);
    }

    std::string ip(argv[1]);

    if(ip.length() > 15){
        std::cout << "<database-ip> should be less than 16 characters!" << std::endl;
        exit(1);
    }

    std::string port(argv[2]);

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

    std::string sql;

    try 
    {
        std::string connectionOptions = "dbname = missioncontroldb user = lprd password = tyJcV9oY1wvZU9IPpGgD \
            hostaddr = " + ip + " port = 5432";
        connection C(connectionOptions);  //connectionOptions.c_str() also works
        std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        std::cout << "(An exception would have been thrown if this was not the case)" << std::endl;

        // Create a transactional object.
        work W_DROP(C);
        // DROP SQL statement
        // IF EXISTS prevents an error from being thrown if the table doesn't exist
        sql = "DROP TABLE IF EXISTS LOAD_CELL_DATA";
        // Execute SQL query
        W_DROP.exec( sql );
        W_DROP.commit();
        std::cout << "Table deleted/dropped successfully" << std::endl;

        // Create a transactional object.
        work W_TABLE(C);
        // CREATE SQL statement
        // will throw ERROR:  relation "load_cell_data" already exists if the table already exists
        // use IF NOT EXISTS after TABLE to prevent this error
        sql = "CREATE TABLE LOAD_CELL_DATA (TIME_STR TEXT NOT NULL," \
                "RUN_TIME   REAL  PRIMARY KEY NOT NULL," \
                "LC1_FORCE  REAL                      ," \
                "LC2_FORCE  REAL                      ," \
                "LC3_FORCE  REAL                      ," \
                "LC4_FORCE  REAL                       " \
                ");";
        // Execute SQL query
        W_TABLE.exec( sql );
        W_TABLE.commit();
        std::cout << "Table created successfully" << std::endl;

        auto startTime = std::chrono::system_clock::now();
        auto prevTimeTask1 = std::chrono::system_clock::now();
        auto currentTime = std::chrono::system_clock::now();
        std::time_t ttCurrentTime = std::chrono::system_clock::to_time_t(currentTime);
        auto timerTask1 = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTimeTask1).count();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        float runTime;

        auto timeSinceEpoch = startTime.time_since_epoch();
        auto epochSeconds = (timeSinceEpoch.count() * 
            std::chrono::system_clock::period::num / std::chrono::system_clock::period::den);
        std::cout << "start time: " << ctime(&ttCurrentTime) << std::endl;
        std::cout << "epoch seconds: " << epochSeconds << std::endl;
        srand(epochSeconds);

        while(1)
        {
            valread = read(new_socket, &packet, sizeof(packet));
            std::cout << packet.cmd << std::endl;
            for (int i = 0; i < NumCells; i++) {
                std::cout << "\t" << packet.data[i] << std::endl;
            }
            std::cout << std::endl;

            currentTime = std::chrono::system_clock::now();
            ttCurrentTime = std::chrono::system_clock::to_time_t(currentTime);
            timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
            runTime = timestamp/1000.0;
            timerTask1 = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTimeTask1).count();

            // Insert Load Cell Data Task (Task1)
            if(timerTask1 > DATA_UPLOAD_INTERVAL_MS)
            {
                prevTimeTask1 = std::chrono::system_clock::now();
                // float lc1 = 100 + runTime + (rand() % 10 - 5);
                // float lc2 = 200 + runTime + (rand() % 10 - 5);
                // float lc3 = 300 + runTime + (rand() % 10 - 5);
                // float lc4 = 400 + runTime + (rand() % 10 - 5);
                float lc1 = packet.data[0];
                float lc2 = packet.data[1];
                float lc3 = packet.data[2];
                float lc4 = packet.data[3];
                insert_load_cell_data(C, ttCurrentTime, runTime, lc1, lc2, lc3, lc4);
            }
        }
    } 
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
