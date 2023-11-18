// #######################################
/* 
postgresExampleAlt2.cpp

This program shows how to use libpqxx to stream multiple channels of simulated
sensor data to a postgresql database running on a container with a specific IP.
The data has been formatted so that a user can visualize the data in real time
in a Grafana dashboard.
To Compile:
g++ -Wall -Werror -O3 -std=c++17 postgresExampleAlt2.cpp -lpqxx -lpq -o exampleAlt2
To run:
./exampleAlt2
*/
// #######################################
#include <pqxx/pqxx> 
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

using namespace pqxx;

#define DATA_UPLOAD_RATE_HZ 10
#define DATA_UPLOAD_INTERVAL_MS (1000.0/DATA_UPLOAD_RATE_HZ)

void insert_load_cell_data(connection &C, std::time_t t, float runTime, float lc1, float lc2, float lc3, float lc4)
{
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

int main(int argc, char* argv[]) {
   std::string sql;

   if(argc < 2){
      std::cout << "must provide an IP Address in the format X.X.X.X" << std::endl;
      return 1;
   }

   std::string ip(argv[1]);

   if(ip.length() > 15){
      std::cout << "IP Address must be less than 15 characters!" << std::endl;
      return 1;
   }

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
         currentTime = std::chrono::system_clock::now();
         ttCurrentTime = std::chrono::system_clock::to_time_t(currentTime);
         timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
         runTime = timestamp/1000.0;
         timerTask1 = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevTimeTask1).count();

         // Insert Load Cell Data Task (Task1)
         if(timerTask1 > DATA_UPLOAD_INTERVAL_MS)
         {
            prevTimeTask1 = std::chrono::system_clock::now();
            float lc1 = 100 + runTime + (rand() % 10 - 5);
            float lc2 = 200 + runTime + (rand() % 10 - 5);
            float lc3 = 300 + runTime + (rand() % 10 - 5);
            float lc4 = 400 + runTime + (rand() % 10 - 5);
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
