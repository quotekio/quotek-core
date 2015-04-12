#include "modules.h"
int __notice(struct evmio_array* evmio_a, char* name, char* subject,char* content);

#define  notice(_subject,_content)  ( __notice(io->evmio_a,"alert",_subject,_content) )
