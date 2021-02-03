# REWRITING_MALLOC
Re-writing malloc(),calloc(),realloc(),free() using sbrk()

sbrk(0) -> returns the top of the current heap reserved by os for the program
sbrk(size) -> increases the heap pointer by size and returns
              pointer to the previous top


!!!may not work on some versions of mac as sbrk() is depreceated

