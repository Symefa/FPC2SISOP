#include "types.h"
#include "stat.h"
#include "user.h"

char *envp[] = environ;

int main(int argc, char *argv[]){

    for(int i=0;envp[i]>0;i++){

        printf(1,"%s\n",envp[i]);
    }

    exit();
}
