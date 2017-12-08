#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "fs.h"
#include "user.h"

char buffer[512];
char* fmtname(char *path);
char* strcat(char *s1,char *s2);

void mv_all(char *src, char *dst);
void mv(char *src,char *dst);

int main(int argc, char *argv[]){
	char *src,*dst;
	if(argc<=2){
		printf(1," usage: mv src dst\n");
		exit();
	}
	if(argc==3){
		if(argv[1][0]=='*')
		{
			dst=argv[2];
			mv_all(".",dst);
			exit();	
		}
		else
		{
		   	src=argv[1];
			dst=argv[2];
			mv(src,dst);
			exit();	
		}
	}
	if(argc==4)
	{
		if(argv[1][0]=='*')
		{
			src=argv[2];
			dst=argv[3];
			mv_all(src,dst);
		}
		exit();
	}
}

char* fmtname(char *path){
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

char* strcat(char *s1,char *s2){
  char *b=s1;
  while(*s1)++s1;
  while(*s2)*s1++=*s2++;
  *s1=0;
  return b;
}

void mv_all(char *src, char *dst){
  char buf[512], *p;
  int fd;
  struct dirent dr;
  struct stat st;
  
  stat(src,&st);
  fd=open(src,O_RDONLY);
  
  if(st.type==T_FILE)
  {
    printf(1,"%s is a file\n", src);
    return;
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
        printf(1,"mv *: file cannot be stats\n");
        continue; 
      }

      if(st.type==1)
      {
        if(fmtname(buf)[0]=='.')
        {
          continue;
        }
        
        char dsrc[512];
  
        //select source
        strcpy(srcs,src);
        strcat(srcs,"/");
        strcat(srcs,p);
        
        //select dest
        strcpy(dsrc,dst);
        strcat(dsrc,"/");
        strcat(dsrc,p);
  		
  		mkdir(dsrc);
        mv_all(srcs,dsrc); 
  
        unlink(srcs);
      }
      else
      {        
        char srcs[512],dsrc[512];
    	
    	//select source
        strcpy(srcs,src);
        strcat(srcs,"/");
        strcat(srcs,p);
        
        //select dest
        strcpy(dsrc,dst);
        strcat(dsrc,"/");
        strcat(dsrc,p);
    	
    	//domove
    	mv(srcs,dsrc);
        unlink(srcs);
      }
    }
  }
  close(fd);
}

void mv(char *src,char *dst){
	int fdsrc, fddst, n,flag=1;
	char sec[512];
	
	fdsrc=open(src,O_RDWR);
	fddst=open(dst,O_RDWR);
	
	if(fddst>=0)
	{
		flag=0;
	}

	close(fddst);
	fddst=open(dst,O_RDWR | O_CREATE);

	if(fdsrc<0 && fddst>=0){
		printf(1,"%s does not exist or is a directory\n",src);
		if(flag)
		{
			unlink(dst);
		}

		close(fdsrc);
		close(fddst);
		return;
	}
	else if(fdsrc<0 && fddst<0){
		close(fdsrc);
		close(fddst);
		return;
	}
	else if(fddst<0 && fdsrc>=0){
		close(fddst);
		fddst=open(sec,O_CREATE | O_RDWR);
	}
	
	while((n=read(fdsrc,buffer,sizeof(buffer)))>0) 
		write(fddst,buffer,fdsrc);
	
	unlink(src);
	
	close(fdsrc);
	close(fddst);

}