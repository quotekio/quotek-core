#include "adammodule.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct upload_status {
  int lines_read;
};

int plock = 0;

size_t curl_wh(void *ptr, size_t size, size_t nmemb, string stream)
{
    if (ptr != NULL) {

      string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}


static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  
  extern int plock;

  if (plock == 0) {

    char *payload = (char*) userp;
    size_t len = strlen(payload);
    memcpy(ptr,payload,len);
    plock = 1 ;
    return len;
  }

  else return 0;

}



class alert {

private:

    bool use_smtp;
    bool use_sms;
    string smtp_url;
    string smtp_from;
    vector<string> mail_recipients;
    string sms_sendurl_template;
    string sms_username;
    string sms_password;
    string sms_account;
    string sms_recipient;
    vector<string> alert_pnl_hours;

public:

  static const string cfgpath;

  alert() {
    
  };

  int readConf() {
    string line;
    vector<string> sline;
    string param;
    string arg;

    ifstream afh (alert::cfgpath.c_str());
    if (!afh.is_open() ) return 1;


    while(afh.good()) {

      getline(afh,line);

      sline = split(line,' ');

      if (sline.size() == 2) {
        param = trim(sline[0]);
        arg = trim(sline[1]);

        if (param == "use_smtp") {
          if (arg == "yes") use_smtp = true;
          else use_smtp = false;
        }

        else if (param == "use_sms") {
          if (arg == "yes") use_sms = true;
          else use_sms = false;
        }
      
        else if (param == "smtp_url") {
          smtp_url = arg;
        }  

        else if (param == "smtp_from") {
          smtp_from = arg;
        }  
        
        else if (param == "sms_url") {
          sms_sendurl_template = arg;
          
        } 
 
        else if (param == "sms_username") {
          sms_username = arg;
        } 
        
        else if (param == "sms_password") {
          sms_password = arg;
        } 
        
        else if (param == "sms_account") {
          sms_account = arg;
        } 

        else if (param == "sms_recipient") {
          sms_recipient = arg;
        }
        


        else if (param == "mail_recipients") mail_recipients = split(arg,',');
        else if (param == "alert_pnl_hours") {
          alert_pnl_hours = split(arg,',');
          for(int i=0;i<alert_pnl_hours.size();i++) {
            alert_pnl_hours[i] = trim(alert_pnl_hours[i]);
          }
          /*for(int i=0;i<alert_pnl_hours.size();i++) {
            cout << alert_pnl_hours[i] << endl;
          }*/
        }
      }
    }

  return 0;

  };

  int send(string from,string subject,string content) {

    if (use_sms) sendSMS(content); 
    if (use_smtp) sendEmail(from,subject,content);
    return 0;

  }

  int send(string subject,string content) {
    if (use_sms) sendSMS(content); 
    if (use_smtp) sendEmail(smtp_from,subject,content);
    return 0;
  }


  int sendEmail(string from, string subject,string content) {

    content = "From:" + from + "\nSubject:" + subject + "\n\n\n" + content + "\r\n.\r\n";

    CURL* mh = curl_easy_init();
    struct curl_slist *recipients = NULL;
    char *payload = (char*) malloc(10000 * sizeof(char)+1); 
    strncpy(payload,content.c_str(), 10000 * sizeof(char) );

    curl_easy_setopt(mh,CURLOPT_URL,smtp_url.c_str());
    curl_easy_setopt(mh, CURLOPT_MAIL_FROM,from.c_str());

    for(int i=0;i<mail_recipients.size();i++) {
      recipients = curl_slist_append(recipients, mail_recipients[i].c_str() );
    }
    curl_easy_setopt(mh, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(mh, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(mh, CURLOPT_READDATA, payload);
    //curl_easy_setopt(mh, CURLOPT_VERBOSE, 1);

    curl_easy_perform(mh);
    curl_slist_free_all(recipients);
    curl_easy_cleanup(mh);

    return 0;


  }


  int sendSMS(string content) {

    string temp;
 
    content = trim(content);
    sreplaceAll(content," ","%20");
    sreplaceAll(content,"\n","%10");

    string surl = sms_sendurl_template;
    surl = sreplace(surl,"%%SMS_USERNAME%%",sms_username);
    surl = sreplace(surl,"%%SMS_PASSWORD%%",sms_password);
    surl = sreplace(surl,"%%SMS_ACCOUNT%%",sms_account);
    surl = sreplace(surl,"%%SMS_RECIPIENT%%",sms_recipient);
    surl = sreplace(surl,"%%SMS_BODY%%",content);
    
    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,surl.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    //curl_easy_setopt(ch, CURLOPT_VERBOSE, 1);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);


    return 0; 
  }

  vector<string> getMailRecipients() {
    return mail_recipients;
  }

  vector<string> getAlertPNLHours() {
    return alert_pnl_hours;
  }

  string getSMTPFrom() {
    return smtp_from;
  }


};

const string alert::cfgpath = "/etc/adam/alert.conf";

void* module_fct(module_io mio) {

  alert* a = new alert();
  time_t t;
  struct tm* c_time;
  char* t_str = (char*) malloc(40*sizeof(char)+1);
  string prev_t_str;
  int mlock =0;

  if (a->readConf() != 0) {
    cout << "ALERT: cannot read configuration/configuration error, unloading module.." <<endl;
    return NULL;
  }


  while(1) {

    t = time(0);
    c_time = localtime(&t);   
    sprintf(t_str,
        "%02d:%02d",
        c_time->tm_hour,
        c_time->tm_min
    );
    
    if (std::string(t_str) != prev_t_str ) {
      mlock = 0;
      prev_t_str = std::string(t_str);

    }

    if (contains(a->getAlertPNLHours(),std::string(t_str) ) && mlock == 0 ) {
      string mct = "CUMULATIVE_PNL: " + float2string(*(mio.cumulative_pnl)) + "\n";
      mct += "CURRENT_PNL: " + float2string(*(mio.cur_pnl)) + "\n";
      a->send("Adam PNL Report",mct);
      mlock = 1;

    }

    if (! IsEmpty(*(mio.input)) ) {

      char* str = FrontAndDequeue(*(mio.input));

      string in = std::string(str);
      vector<string> lines = split(in,'\n');
      if (lines.size() > 0) {
        string subject = lines[0];
        in = sreplace(in,subject,"");
        a->send("notice@adambot.com",subject,in);
      }

      free(str);
    }

    sleep(2);
  }


}



extern "C" void* module(module_io mio) {
  module_fct(mio);
  return NULL;

}
