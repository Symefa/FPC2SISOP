#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "fs.h"
#include "user.h"

char* fmtname(char *path);
char* strcat(char *s1,char *s2);
void rf(char *src);


int main(int argc, char *argv[])
{
  int i;
  if(argc < 2)
  {
    printf(2, "usage            : rm option files...\n");
    printf(2, "available option : -rf directory\n");
    exit();
  }

  if (argc==3 && argv[1][0]=='-' && argv[1][1]=='r' && argv[1][2]=='f')
  {
    int fd;
    fd=open(argv[2],O_WRONLY);
  if(fd>=0)
  {
    printf(1,"%s is not a directory\n", argv[3]);
    exit(); 
  }

  close(fd);

  rf(argv[2]);
  unlink(argv[2]);
  
  exit(); 
}
  for(i = 1; i < argc; i++)
  {
    if(unlink(argv[i]) < 0)
    {
      printf(2, "rm: %s failed to delete\n", argv[i]);
      break;
    }
  }

  exit();
}

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

char* strcat(char *s1,char *s2)
{
  char *b=s1;
  while(*s1)++s1;
  while(*s2)*s1++=*s2++;
  *s1=0;
  return b;
}

void rf(char *src)
{
  char buf[512], *p;
  int fd;
  struct dirent dr;
  struct stat st;
  
  stat(src,&st);
  fd=open(src,O_RDONLY);
  
  if(st.type==T_FILE)
  {
    unlink(src);  
  }

  else if(st.type==T_DIR)
  {
    strcpy(buf, src);
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &dr, sizeof(dr)) == sizeof(dr))
    {
      if(dr.inum == 0)
        continue;
      
      memmove(p, dr.name, DIRSIZ);
      p[DIRSIZ] = 0;
      
      if(stat(buf,&st)<0){
        printf(1,"rm -rf: file cannot be stats\n");
        continue; 
      }

      if(st.type==1)
      {
        if(fmtname(buf)[0]=='.')
        {
          continue;
        }
        
        char dsrc[512];
  
        strcpy(dsrc,src);
        strcat(dsrc,"/");
        strcat(dsrc,p);
  
        rf(dsrc); 
  
        unlink(dsrc);
      }
      else
      {        
        char dsrc[512];
    
        strcpy(dsrc,src);
        strcat(dsrc,"/");
        strcat(dsrc,p);
    
        unlink(dsrc);
      }
    }
  }
  close(fd);
}