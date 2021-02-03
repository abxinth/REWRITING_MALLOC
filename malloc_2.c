#include<assert.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
///MAKING FREE() WORK
struct meta_block{
	size_t size;
	struct meta_block *next;
	int free;
	int debug;
};
void *global_base = NULL;
#define META_SIZE sizeof(struct meta_block)

struct meta_block *find_free_block(struct meta_block** last, size_t size){
	struct meta_block *current = global_base;
	while(current && !(current->free && current->size >= size)){
	 *last = current;
     current = current->next;
	}
	return current;
}

struct meta_block *request_space(struct meta_block* last, size_t size){
	struct meta_block *block;
	block = sbrk(0);
	void *request = sbrk(size + META_SIZE);
	assert((void*)block == request);
	if(request == (void*)-1){
		return NULL;
	}
	if(last){
		last->next = block;
	}
	block->size = size;	
	block->next = NULL;
	block->free = 0;
	block->debug = 0011;
	return block;
}

void *malloc(size_t size){
	struct meta_block *block;
	if(size <=0){
		return NULL;
	}
	if(!global_base){
		block = request_space(NULL, size);	
		if(!block){
			return NULL;
		}
		global_base = block;
	}else{
		struct meta_block *last = global_base;
		block = find_free_block(&last, size);
		if(!block){
			block = request_space(last, size);
			if(!block){
			return NULL;
			}
		}else{
			block->free = 0;
			block->debug = 123;
		
		}
	}
	return (block+1);
	
}
struct meta_block *get_block_ptr(void *ptr){
	struct meta_block *ptr_meta = (struct meta_block*)ptr;
	return (ptr_meta-1);
}

void free(void *ptr){
	if(!ptr){
		return;
	}
	struct meta_block* block_ptr = get_block_ptr(ptr);
	assert(block_ptr->free == 0);
	block_ptr->free = 1;
}
void *realloc(void *ptr,size_t size){
	if(!ptr){
		return malloc(size);
	}
	struct meta_block* block_ptr = get_block_ptr(ptr);
	if(block_ptr->size >= size){
		return ptr;
	}
	void *new_ptr;
	new_ptr = malloc(size);
	if(!new_ptr){
		return NULL;
	}
	memcpy(new_ptr, ptr,block_ptr->size);
	free(ptr);
	return new_ptr;
}
void *calloc(size_t nelem, size_t elsize){
	size_t size = nelem * elsize;
	void *ptr = malloc(size);
	memset(ptr, 0,size);
	return ptr;
}
int main(){
/*
int *z = (int*)malloc(111);
struct meta_block *x = (struct meta_block*)z - 1;
printf("%d", x->size);
*/

return 0;
}
