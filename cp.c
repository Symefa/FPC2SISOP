#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

#define BUF_SIZE 521

void ls(char *path);
char* fmtname(char *path);

int main(int argc, char *argv[]){
int fd0, fd1;
	
if (argc <= 2){
   printf(1, "Butuh 2 argumen! format: cp [nama file yg ingin disalin] [nama file yang baru]\n");
   exit();
   }
 
 if((fd0 = open(argv[1], O_RDONLY)) < 0){
   printf(1, "cp: tidak dapat membuka %s\n", argv[1]);
   exit();
   }

struct stat st;
	fstat(fd0, &st);
	if (st.type == T_DIR)
	{	printf(1, "source file merupakan directori, file-file yg ada dalam direktori tersebut adalah:\n");
		ls(argv[1]);
		printf(1, "Karena ini merupakan cp saja, jadi hanya bisa meyalin satu persatu file\n");
		exit();
	}


char com[128] = {};
	strcpy(com, argv[2]);
	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);
	if (argv[2][len2-1] == '/')
	{
	
		int i = len1 - 1;
		for (; i >= 0; i--)
			if (argv[1][i] == '/')
				break;
		i++;
		strcpy(&com[len2], &argv[1][i]);
	}

 if((fd1 = open(com, O_CREATE|O_WRONLY)) < 0){
   printf(1, "cp: tidak dapat membuka %s\n", argv[2]);
   exit();
   }

char buf[BUF_SIZE] = {};
	int len = 0;
	while((len = read(fd0, buf, BUF_SIZE)) > 0)
		write(fd1, buf, len);
 
  close (fd0);
  close (fd1);

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

void ls(char *path)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	if((fd = open(path, 0)) < 0)
	{
		printf(2, "tidak dapat membuka %s\n", path);
		return;
	}
	
	if(fstat(fd, &st) < 0)
	{
		printf(2, "tidak dapat stat %s\n", path);
		close(fd);
		return;
	}
	
	switch(st.type)
	{
		case T_FILE:
		printf(1, "name = %s, type = file, size = %d\n", fmtname(path), st.size);
		break;
		
		case T_DIR:
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
		{
			printf(1, "path terlalu panjang\n");
			break;
		}
		strcpy(buf, path);
		p = buf+strlen(buf);
		*p++ = '/';
		
		while(read(fd, &de, sizeof(de)) == sizeof(de))
		{
			if(de.inum == 0)
				continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if(stat(buf, &st) < 0)
			{
				printf(1, "tidak dapat stat %s\n", buf);
				continue;
			}
			printf(1, "name = %s, type = directory, size = %d\n", fmtname(buf), st.size);
		}
		break;
	}
	close(fd);
}
