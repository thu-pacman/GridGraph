#include <stdio.h>
#include <stdlib.h>

typedef int VertexId;
typedef float Weight; // you may modify the weight type for different application requirements

struct xorshift128plus {
  unsigned long s[2];
  void init(int seed) {
    s[0] = +seed;
    s[1] = -seed;
  }
  unsigned long next() {
    unsigned long x = s[0];
    unsigned long const y = s[1];
    s[0] = y;
    x ^= x << 23;
    x ^= x >> 17;
    x ^= y ^ (y >> 26);
    s[1] = x;
    return x + y;
  }
};

int main(int argc, char ** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: [executable] [input] [output]\n");
    exit(-1);
  }
  FILE * fin = fopen(argv[1], "rb");
  FILE * fout = fopen(argv[2], "wb");
  xorshift128plus rng;
  rng.init(1);
  while (true) {
    VertexId src, dst;
    if (fread(&src, sizeof(src), 1, fin)==0) break;
    if (fread(&dst, sizeof(dst), 1, fin)==0) break;
    fwrite(&src, sizeof(src), 1, fout);
    fwrite(&dst, sizeof(dst), 1, fout);
    Weight wgh = rng.next() % 10000 / 100; // generate a random weight within [0, 100)
    fwrite(&wgh, sizeof(wgh), 1, fout);
  }
  fclose(fin);
  fclose(fout);
}
