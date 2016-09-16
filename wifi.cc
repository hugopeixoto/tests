#include <cstdint>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <syscall.h>
#include <omp.h>
#include <linux/random.h>

char *randoms;

uint64_t collisions(uint64_t n, uint32_t offset) {
  uint64_t* v = new uint64_t[n];

  for (uint64_t i = 0; i < n; i++) {
    v[i] = 0;
    for (uint32_t j = 0; j < 6; j++) {
      v[i] = v[i]*0x100+(randoms+offset+i*6)[j];
    }
  }

  std::sort(v, v+n);

  uint64_t c = 0;
  for (uint64_t i = 1; i < n; i++) {
    if (v[i] == v[i-1]) {
      c++;
    }
  }

  delete[] v;

  return c;
}

uint64_t total_collisions(const uint64_t iterations, const uint64_t n) {
  uint64_t t = 0;

  #pragma omp parallel for reduction(+:t)
  for (uint64_t i = 0; i < iterations; i++) {
    t += collisions(n, i*n*6);
  }

  return t;
}

int main(int argc, char* argv[]) {
  FILE* fp = fopen("totally-random.txt", "rb");
  randoms = new char[1ULL<<31];
  fread(randoms, 1ULL<<31, 1, fp);
  fclose(fp);

  uint64_t n = atoi(argv[1]);
  uint64_t i = atoi(argv[2]);
  std::cout << total_collisions(i, n) << std::endl;
  return 0;
}
