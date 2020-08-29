#include <cmath>
#include <cstdio>

const double M_TAU = 2 * M_PI;

int main() {
  float period = 40;
  float amplitude = 30;
  int bob = 0;
  int bob2 = 0; float dy = 0;
  for (int i = 0; i < 50; i++) {
    bob = amplitude / 2.0 * sin(i / period * M_TAU);

    bob2 += amplitude / 2.0 * cos(dy * M_TAU) * M_TAU / period;
    dy += 1 / period; if (dy > 2.0) dy -= period;

    for (int j = 0; j <= amplitude + 4; j++) {
      fputc(j-(amplitude+4)/2.0 == bob ? '.' : ' ', stdout);
    }

    for (int j = 0; j <= amplitude + 4; j++) {
      fputc(j-(amplitude+4)/2.0 == bob2 ? '.' : ' ', stdout);
    }
    puts("");
  }
}
