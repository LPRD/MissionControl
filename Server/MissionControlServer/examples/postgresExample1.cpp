// #######################################
/* 
postgresExample1.cpp
This program shows how to use libpqxx to 
create and delete a table in postgresql
as well as how to insert rows and select 
data from a table
To Compile:
g++ -Wall -Werror -std=c++17 postgresExample1.cpp -lpqxx -lpq -o example1
To run:
./example1
*/
// #######################################
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

#define ONLY_DELETE_TABLE false

int main(int argc, char* argv[]) {
   // char * sql;
   std::string sql;

   try {
      connection C("dbname = missioncontroldb user = lprd password = tyJcV9oY1wvZU9IPpGgD \
         hostaddr = 127.0.0.1 port = 5432"
      );
      cout << "Opened database successfully: " << C.dbname() << endl;
      cout << "(An exception would have been thrown if this was not the case)" << endl;

      if(!ONLY_DELETE_TABLE)
      {
         // Create a transactional object.
         work W_1(C);
         // CREATE SQL statement
         // will throw ERROR:  relation "load_cell_data_example" already exists if the table already exists
         // use IF NOT EXISTS after TABLE to prevent this error
         sql = "CREATE TABLE LOAD_CELL_DATA_EXAMPLE(" \
               "ID INT PRIMARY KEY     NOT NULL," \
               "TIMESTAMP      REAL    NOT NULL," \
               "LC1_FORCE      REAL    NOT NULL," \
               "LC2_FORCE      REAL    NOT NULL," \
               "LC3_FORCE      REAL    NOT NULL," \
               "LC4_FORCE      REAL    NOT NULL" \
               ");";
         // Execute SQL query
         W_1.exec( sql );
         W_1.commit();
         cout << "Table created successfully" << endl;

         // Create a transactional object.
         work W_2(C);
         // INSERT SQL statement
         sql = "INSERT INTO LOAD_CELL_DATA_EXAMPLE (ID,TIMESTAMP,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
               "VALUES (1, 0.0, 0.0, 0.0, 0.0, 0.0); " \
               "INSERT INTO LOAD_CELL_DATA_EXAMPLE (ID,TIMESTAMP,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
               "VALUES (2, 1.0, 67.079, 107.594, 103.611, 94.080); " \
               "INSERT INTO LOAD_CELL_DATA_EXAMPLE (ID,TIMESTAMP,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
               "VALUES (3, 2.0, 107.836, 163.807, 107.178, 174.063); " \
               "INSERT INTO LOAD_CELL_DATA_EXAMPLE (ID,TIMESTAMP,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
               "VALUES (4, 3.0, 230.689, 245.293, 210.162, 295.917); " \
               "INSERT INTO LOAD_CELL_DATA_EXAMPLE (ID,TIMESTAMP,LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE) " \
               "VALUES (5, 4.0, 318.931, 314.253, 352.248, 362.885); ";
         // Execute SQL query
         W_2.exec( sql );
         W_2.commit();
         cout << "Rows inserted successfully" << endl;

         // SELECT SQL statement
         sql = "SELECT * from LOAD_CELL_DATA_EXAMPLE";
         // Create a non-transactional object.
         nontransaction N(C);
         // Execute SQL query
         result R( N.exec( sql ));
         N.commit();
         // List down all the records
         for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            cout << "ID = " << c[0].as<int>() << ", ";
            cout << "TIMESTAMP = " << c[1].as<float>() << ", ";
            cout << "LC1_FORCE = " << c[2].as<float>() << ", ";
            cout << "LC2_FORCE = " << c[3].as<float>() << ", ";
            cout << "LC3_FORCE = " << c[4].as<float>() << ", ";
            cout << "LC4_FORCE = " << c[4].as<float>() << endl;
         }
         cout << "Select operation done successfully" << endl;
      }

      // Create a transactional object.
      work W_4(C);
      // DROP SQL statement
      // IF EXISTS prevents an error from being thrown if the table doesn't exist
      sql = "DROP TABLE IF EXISTS LOAD_CELL_DATA_EXAMPLE";
      // Execute SQL query
      W_4.exec( sql );
      W_4.commit();
      cout << "Table deleted/dropped successfully" << endl;
   } 
   catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }
}



