// #######################################
/* 
postgresExample0.cpp
This program is a basic example of how to use 
the libpqxx library to connect to a database.
To Compile:
g++ postgresExample0.cpp -lpqxx -lpq -o example0
To run:
./example0
*/
// #######################################
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
   try {
      connection C("dbname = missioncontroldb user = lprd password = tyJcV9oY1wvZU9IPpGgD \
         hostaddr = 127.0.0.1 port = 5432"
      );
      cout << "Opened database successfully: " << C.dbname() << endl;
      cout << "(An exception would have been thrown if this was not the case)" << endl;
   } 
   catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }
}
