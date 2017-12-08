#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char buf[521];

int main(int argc, char *argv[]){
 int fd0, fd1, n;

 if(argc <= 2){
   printf(1, "Butuh 2 argumen! format: cp [nama file yg ingin disalin] [nama file yang baru]\n");
   exit();
   }
 
 if((fd0 = open(argv[1], O_RDONLY)) < 0){
   printf(1, "cp: tidak dapat membuka %s\n", argv[1]);
   exit();
   }

 if((fd1 = open(argv[2], O_CREATE||O_RDWR)) < 0){
   printf(1, "cp: tidak dapat membuka %s\n", argv[2]);
   exit();
   }

 while((n = read(fd0, buf, sizeof(buf))) > 0){
   write(fd1, buf, n);
   }
 
  close (fd0);
  close (fd1);

exit();   
}
