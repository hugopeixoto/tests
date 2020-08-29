#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cmath>

#include <vector>
#include <chrono>
#include <iostream>
#include <tuple>

struct FixedMovementPosition {
  int64_t x, y;
  int64_t dx, dy;

  FixedMovementPosition(int64_t ax, int64_t ay, int64_t adx, int64_t ady) : x(ax), y(ay), dx(adx), dy(ady) {}
  int64_t checksum() const { return x + y + dx + dy; }
};

void FixedMovementPositionSystem(int elapsedTime, FixedMovementPosition& fmp) {
  fmp.x += fmp.dx * elapsedTime;
  fmp.y += fmp.dy * elapsedTime;
}

struct PulsingRadius {
  int64_t radius;
  int64_t base;
  int64_t baseAmplitude;

  PulsingRadius(int64_t ar, int64_t ab, int64_t aba) : radius(ar), base(ab), baseAmplitude(aba) {}
  int64_t checksum() const { return radius + base + baseAmplitude; }
};

void PulsingRadiusSystem(int elapsedTime, PulsingRadius& pr) {
  pr.radius = pr.base + (int64_t)(pr.baseAmplitude * sin(elapsedTime / 1000.0));
}

std::vector<uint32_t> entities;
std::vector<FixedMovementPosition> fmp;
std::vector<PulsingRadius> pr;
void gameloop() {
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < entities.size(); j++) {
      if (entities[j] & 1) FixedMovementPositionSystem(i, fmp[j]);
      if (entities[j] & 2) PulsingRadiusSystem(i, pr[j]);
    }
  }
}

int main(int argc, char* argv[]) {
  srand(atoi(argv[1]));
  for (int i = 0; i < 10000; i++) {
    uint32_t mask = 0;
    if (rand() % 2) {
      mask |= 1;
    }
    fmp.emplace_back(FixedMovementPosition(0, 0, 1, 1));

    if (rand() % 2) {
      mask |= 2;
    }
    pr.emplace_back(PulsingRadius(10, 10, 2));

    entities.emplace_back(mask);
  }

  auto startTime = std::chrono::steady_clock::now();

  gameloop();

  auto finishTime = std::chrono::steady_clock::now();

  auto delta = finishTime - startTime;

  int64_t checksum = 0;
  for (int j = 0; j < entities.size(); j++) {
    if (entities[j] & 1) checksum += fmp[j].checksum();
    if (entities[j] & 2) checksum += pr[j].checksum();
  }

  std::cout << delta.count() / 1000 / 1000 << " ms " << checksum << std::endl;
}
