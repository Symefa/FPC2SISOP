#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  int fd;

  if(argc < 2){
    printf(1, "Use this arguments : touch [namefile]\n");
    exit();
  }

  if((fd = open(argv[1], O_CREATE | O_RDWR)) < 0){
     printf(1, "failed create touch: %s \n", argv[1]);
  }
  close(fd);
  exit();
}
