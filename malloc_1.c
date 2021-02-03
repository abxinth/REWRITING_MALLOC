#include<assert.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

void *malloc(size_t size){
   void *ptr_current = sbrk(0);//current heap top
   void *ptr_prev = sbrk(size);// returns previous heap top after allocation
  if(ptr_prev == (void*)-1){
	return NULL;//failed
  }else{
	assert(ptr_current == ptr_prev);
	return ptr_current;
  }

}

int main(){
int *z = (int*)malloc(4);

return 0;
}
