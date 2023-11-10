// #######################################
/* 
postgresRemoveTable.cpp
This program shows how to use libpqxx to 
remove a table
To Compile:
g++ -Wall -Werror -O3 -std=c++17 postgresRemoveTable.cpp -lpqxx -lpq -o removeTable
To run:
./removeTable
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

#define ONLY_DELETE_TABLE true

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
      connection C(connectionOptions);  //connectionOptions.c_str()
      std::cout << "Opened database successfully: " << C.dbname() << std::endl;
      std::cout << "(An exception would have been thrown if this was not the case)" << std::endl;

      if(ONLY_DELETE_TABLE)
      {
         // Create a transactional object.
         work W_DROP(C);
         // DROP SQL statement
         // IF EXISTS prevents an error from being thrown if the table doesn't exist
         sql = "DROP TABLE IF EXISTS LOAD_CELL_DATA";
         // Execute SQL query
         W_DROP.exec( sql );
         W_DROP.commit();
         std::cout << "Table deleted/dropped successfully" << std::endl;
         return 0;
      }

   } 
   catch (const std::exception &e)
   {
      std::cerr << e.what() << std::endl;
      return 1;
   }
}
