#include "alert.h"

int __notice(struct evmio_array* evmio_a, char* name, char* subject,char* content) {
  char* wmail_content;

  wmail_content = (char*) malloc( (strlen(subject) + strlen(content) + 500 ) * sizeof(char) );
  sprintf(wmail_content,"%s\n%s",subject,content); 
  module_input(name,wmail_content);
}
