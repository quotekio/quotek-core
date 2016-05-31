#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>  
#include <fcntl.h>      /* O_flags */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <quotek/record.hpp>
#include <map>
#include <iostream>

#include "salloc.hpp"


#define SHM_SIZE 256 * 1024^2


int main(int argc, char** argv) {

  key_t key;
  int shmid;
  void *data;
  int mode;

  std::map<std::string, quotek::data::records>* rdest;

  if ((key = ftok("/dev/null", 201 )) == -1) {
          perror("ftok");
          exit(1);
  }

  std::cout << "KEY:" << key << std::endl;

  /*  create the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
         perror("shmget");
         exit(1);
   }

     /* attach to the segment to get a pointer to it: */
     rdest =  reinterpret_cast< std::map<std::string, quotek::data::records>* > (shmat(shmid, (void *)0, 0));

     //std::map<std::string, quotek::data::records> rfrom;
     //rfrom["foo"].append(8);
     //memcpy(rdest,&rfrom, 100);
     
     (*rdest)["foo"] = quotek::data::records();
     (*rdest)["foo"].append(9);


     while(true ) {
       sleep(1);
       std::cout <<  (*rdest)["foo"][0].value << std::endl;

     }


}