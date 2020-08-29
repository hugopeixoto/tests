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

std::vector<std::tuple<>> none;
std::vector<std::tuple<FixedMovementPosition>> fmp;
std::vector<std::tuple<PulsingRadius>> pr;
std::vector<std::tuple<FixedMovementPosition, PulsingRadius>> fmp_pr;
void gameloop() {
  for (int i = 0; i < 1000; i++) {
    for (auto& entity: fmp) { FixedMovementPositionSystem(i, std::get<0>(entity)); }
    for (auto& entity: fmp_pr) { FixedMovementPositionSystem(i, std::get<0>(entity)); }

    for (auto& entity: pr) { PulsingRadiusSystem(i, std::get<0>(entity)); }
    for (auto& entity: fmp_pr) { PulsingRadiusSystem(i, std::get<1>(entity)); }
  }
}

int main(int argc, char* argv[]) {
  srand(atoi(argv[1]));
  for (int i = 0; i < 10000; i++) {
    uint32_t mask = 0;
    if (rand() % 2) { mask |= 1; }
    if (rand() % 2) { mask |= 2; }

    switch (mask) {
      case 0: none.emplace_back(std::make_tuple()); break;
      case 1: fmp.emplace_back(std::make_tuple(FixedMovementPosition(0, 0, 1, 1))); break;
      case 2: pr.emplace_back(std::make_tuple(PulsingRadius(10, 10, 2))); break;
      case 3: fmp_pr.emplace_back(std::make_tuple(FixedMovementPosition(0, 0, 1, 1),
                                                  PulsingRadius(10, 10, 2))); break;
    }
  }

  auto startTime = std::chrono::steady_clock::now();

  gameloop();

  auto finishTime = std::chrono::steady_clock::now();

  auto delta = finishTime - startTime;

  int64_t checksum = 0;
  for (auto& entity: fmp) { checksum += std::get<0>(entity).checksum(); }
  for (auto& entity: pr) { checksum += std::get<0>(entity).checksum(); }
  for (auto& entity: fmp_pr) { checksum += std::get<0>(entity).checksum() + std::get<1>(entity).checksum(); }

  std::cout << delta.count() / 1000 / 1000 << " ms " << checksum << std::endl;
}
