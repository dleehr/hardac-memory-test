#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static const size_t BYTES_TO_GB = 1024*1024*1024;

void test_read(size_t bytes, char* ptr, int expected) {
  printf("Testing for %d...\n", expected);
  for(size_t b=0;b<bytes;b++) {
    if(ptr[b] == expected) {
      continue;
    } else {
      char *offset = ptr + b;
      int val = ptr[b];
      printf("Error: expected byte at %p to be %d, got %d\n", offset, expected, val);
      free(ptr);
      exit(-1);
    }
  }
}

void write(size_t bytes, char* ptr, int value) {
  printf("Writing %d...\n", value);
  memset(ptr, value, bytes);
}

int main(int argc, char * argv[]) {
  if(argc != 2) {
    printf("Usage: %s <GB to allocate>\n", argv[0]);
    exit(-1);
  }
  size_t chunk_size_gb = 1;
  size_t chunks = atoi(argv[1]);
  printf("Allocating %zu %zuGB chunks\n", chunks, chunk_size_gb);
  size_t bytes = chunk_size_gb * BYTES_TO_GB;
  char *ptr = calloc(chunks, bytes);

  if(ptr == NULL) {
    printf("Allocation of %zu chunks failed: %s\n", chunks, strerror(errno));
  } else {
    printf("Allocated %zu %zu GB chunks\n", chunks, chunk_size_gb);
  }

  // Memory allocated with calloc is initially zeroed. Let's make sure
  test_read(bytes, ptr, 0);

  // Now, write and read values over and over
  const int iterations = 10;
  for(int i=1;i<iterations;i++) {
    write(bytes, ptr, i);
    test_read(bytes, ptr, i);
  }
  printf("Freeing %zu %zu GB chunks\n", chunks, chunk_size_gb);
  free(ptr);
}
