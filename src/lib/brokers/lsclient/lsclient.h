

class LSClient {

  public:

    LSClient(string url, 
    		 string uname, 
    		 string passwd, 
    		 vector<string>* il);

    int connect();

  private:

  	string url;
  	string uname;
  	string passwd;
  	vector<string>* ilist;


};