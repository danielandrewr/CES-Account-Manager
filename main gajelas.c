/*#include <stdio.h>
#include <stdlib.h>
#include "parallel_string.h"


#include "md5.h"
//sumber: https://github.com/pod32g/MD5/blob/master/md5.c

 
int main(void) {
    register char *msg;
		register size_t len;
    register int i;
    unsigned char result[16];
 
    
    msg = "MD5 sucks ass";
 
    len = strlen(msg);
 
    
		md5(msg, len, result);
    
 
    // display result
    for (i = 0; i < 16; i++)
        printf("%2.2x", result[i]);
    puts("");
 
    return 0;
}
*/