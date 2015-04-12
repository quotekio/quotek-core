#include "odbc.h"
#include <iostream>

odbc::odbc() {
  connected = false;
  SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
  SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
}

int odbc::connect(string DSN) {
  SQLRETURN ret;
  SQLCHAR outstr[1024];
  SQLSMALLINT outstrlen;

  /* Allocate a connection handle */
  SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
  /* Connect to the DSN mydsn */
  ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*) DSN.c_str() , SQL_NTS,
         outstr, sizeof(outstr), &outstrlen,
         SQL_DRIVER_COMPLETE);
  if (SQL_SUCCEEDED(ret)) {
    connected = true;
  }
  else connected = false;
  return 0;
}


bool odbc::isConnected() {
     return connected;
}

SQLHDBC odbc::getHandler() {
     return dbc;
}


int odbc::freeStatement(SQLHSTMT* stmt) {
  return SQLFreeHandle(SQL_HANDLE_STMT,*stmt);
}

int odbc::query(SQLHSTMT* stmt,string q) {
      SQLRETURN ret;
      SQLAllocHandle(SQL_HANDLE_STMT, dbc, stmt);
      return  SQL_SUCCEEDED( (ret = SQLExecDirect(*stmt,  
                                                  (SQLCHAR*) q.c_str(),
                                                  SQL_NTS)) );
}


int odbc::getTables(SQLHSTMT* stmt,vector<string>* tables) {

     SQLSMALLINT columns;
     SQLRETURN ret;
     SQLAllocHandle(SQL_HANDLE_STMT, dbc, stmt);
     SQLTables(*stmt, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR*) "TABLE", SQL_NTS);

     SQLNumResultCols(*stmt, &columns);
     while (SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
         SQLUSMALLINT i;
         /* Loop through the columns */
         for (i = 1; i <= columns; i++) {
             SQLLEN indicator;
             char buf[512];
             /* retrieve column data as a string */
             ret = SQLGetData(*stmt, i, SQL_C_CHAR,
                              buf, sizeof(buf), &indicator);
             if (SQL_SUCCEEDED(ret)) {
                 /* Handle null columns */
                 if (indicator == SQL_NULL_DATA ) strcpy(buf, "NULL");
                 string tname = string(buf);
                 if (tname != "NULL" && tname != "TABLE") {
                   tables->push_back(tname);
                 }
             }
         }
     }
    SQLFreeHandle(SQL_HANDLE_STMT,*stmt); 
    return SQL_SUCCEEDED(ret);
}


int odbc::fetch(SQLHSTMT* stmt,vector<string>* result) {

  SQLRETURN ret;
  SQLSMALLINT columns;

  SQLNumResultCols(*stmt, &columns);

  if  ( SQL_SUCCEEDED( ( ret = SQLFetch(*stmt) ) ) ) {
    for (int i = 1; i <= columns; i++) {
      SQLLEN indicator;
      char buf[1024];
      /* retrieve column data as a string */
      ret = SQLGetData(*stmt, i, SQL_C_CHAR,
                       buf, sizeof(buf), &indicator);
      if (SQL_SUCCEEDED(ret)) {
        /* Handle null columns */
        if (indicator == SQL_NULL_DATA) strcpy(buf, "NULL");
        result->push_back(string(buf));
      }
    }
  }
  return SQL_SUCCEEDED(ret);
}


int odbc::fetchAll(SQLHSTMT* stmt,vector< vector<string> >* result) {
  return 0;
}

