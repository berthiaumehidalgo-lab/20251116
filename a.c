#include<stdio.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<regex.h>

char *map_file(const char *file, int *psize)
{
    int fd = open(file, O_RDWR);
    int size = lseek(fd, 0, SEEK_END);
    char *ptr = NULL;
    ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    *psize = size;
    printf("ptr映射成功\n");
    return ptr;
}

int main(void){
	char * ptr = NULL;
	int psize;
	ptr = map_file("url.txt",&psize);
	char * copy_ptr = NULL;
	copy_ptr = ptr;
	regex_t preg;
	char * regex_str = "<a [^>]*href=\"\\([^\"]*\\)\"[^>]*>\\([^<]*\\)</a>";
	size_t nmatch = 3;
	regmatch_t pmatch[nmatch];
	char wwwname[1024];
	char titalname[1024];
	int err;
	if((err = regcomp(&preg,regex_str,0))>0){
		printf("regcomp error.\n");
	}
	while((regexec(&preg,ptr,nmatch,pmatch,0))==0){
		bzero(wwwname,sizeof(wwwname));
		snprintf(wwwname,pmatch[1].rm_eo-pmatch[1].rm_so+1,"%s",ptr+pmatch[1].rm_so);

		bzero(titalname,sizeof(titalname));
		snprintf(titalname,pmatch[2].rm_eo-pmatch[2].rm_so+1,"%s",ptr+pmatch[2].rm_so);
		ptr+=pmatch[0].rm_eo;
		printf("%s, %s\n",wwwname,titalname);
	}
	munmap(copy_ptr,psize);
	return 0;
	
}
