#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  if(argc < 2){
    printf(1, "Use this arguments: cd [path/dir]\n");
    exit();
  }
  else{
    chdir(argv[1]);
  }
  exit();
}
