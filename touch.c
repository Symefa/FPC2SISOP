#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  int fd,i;

  if(argc < 2){
    printf(2, "Use this arguments : touch [namefile]\n");
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_CREATE)) < 0){
      printf(2, "failed create touch: %s \n", argv[i]);
      break;
    }
  }
  close(fd);
  exit();
}
