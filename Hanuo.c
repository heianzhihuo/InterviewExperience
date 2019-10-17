#include <stdio.h>
int main(){
	register char w,x = 'A', y = 'B', z = 'C';
	register int n = 31,i;
	register long long m = 987654321,s = (1<<n)-1,t;
	while(s>3){
		t = (s+1)/2;
		if(m<t){
			w = y;
			y = z;
			z = w;
			s = (s-1)>>1;
		}else if(m>t){
			w = y;
			y = z;
			z = w;
			s = (s-1)>>1;
		}else{
			printf("%c-->%c\n",x,y);
			break;
		}
	}
	if(m==1)
		printf("%c-->%c",x,z);
	else if(m==2)
		printf("%c-->%c",x,y);
	else
		printf("%c-->%c",z,y);
	return 0;
}