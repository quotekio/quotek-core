//ODBC SQL HEADERS
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <vector>
#include <string>

using namespace std;

class odbc {

  private:
  	SQLHENV env;
    SQLHDBC dbc;
    bool connected;

  public:
  	odbc();
  	int connect(string);
  	bool isConnected();
  	SQLHDBC getHandler();
  	int freeStatement(SQLHSTMT* stmt);
  	int query(SQLHSTMT*,string);
  	int getTables(SQLHSTMT*,vector<string>*);
  	int fetch(SQLHSTMT*,vector<string>*);
  	int fetchAll(SQLHSTMT*,vector< vector<string> >*);
};

