
#include <stdio.h>

int myputs(const char* x){
	printf("myputs:%s\n",x);
	return 0;
}
#include "local_hook.h"
LOCAL_HOOK(puts, myputs)

void output(long long x){
	printf("%lld\n",(long long)x);
}


int main(){
   typeof(&puts) y = &puts;
   output((long long)y);
   (*y)("1234567890");
   puts("0987654321");
   return 0;
}
