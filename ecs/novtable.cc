#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cmath>

#include <vector>
#include <chrono>
#include <iostream>

struct Component {
  virtual void update(int elapsedTime) {}
  virtual int64_t checksum() const = 0;
};

struct FixedMovementPosition : Component {
  int64_t x, y;
  int64_t dx, dy;

  FixedMovementPosition(int64_t ax, int64_t ay, int64_t adx, int64_t ady) : x(ax), y(ay), dx(adx), dy(ady) {}

  void update(int elapsedTime) {
    x += dx * elapsedTime;
    y += dy * elapsedTime;
  }

  int64_t checksum() const { return x + y + dx + dy; }
};

struct PulsingRadius : Component {
  int64_t radius;
  int64_t base;
  int64_t baseAmplitude;

  PulsingRadius(int64_t ar, int64_t ab, int64_t aba) : radius(ar), base(ab), baseAmplitude(aba) {}
  void update(int elapsedTime) {
    radius = base + (int64_t)(baseAmplitude * sin(elapsedTime / 1000.0));
  }

  int64_t checksum() const { return radius + base + baseAmplitude; }
};

struct Entity {
  void update(int elapsedTime) {
    if (mask & 1) ((FixedMovementPosition*)components[0])->update(elapsedTime);
    if (mask == 2) ((PulsingRadius*)components[0])->update(elapsedTime);
    if (mask == 3) ((PulsingRadius*)components[1])->update(elapsedTime);
  }

  uint32_t mask;
  std::vector<Component*> components;
};

std::vector<Entity> entities;
void gameloop() {
  for (int i = 0; i < 1000; i++) {
    for (auto& entity: entities) entity.update(i);
  }
}

int main(int argc, char* argv[]) {
  srand(atoi(argv[1]));
  for (int i = 0; i < 10000; i++) {
    auto& entity = entities.emplace_back(Entity());

    entity.mask = 0;
    if (rand() % 2) {
      entity.mask |= 1;
      entity.components.emplace_back(new FixedMovementPosition(0, 0, 1, 1));
    }
    if (rand() % 2) {
      entity.mask |= 2;
      entity.components.emplace_back(new PulsingRadius(10, 10, 2));
    }
  }

  auto startTime = std::chrono::steady_clock::now();

  gameloop();

  auto finishTime = std::chrono::steady_clock::now();

  auto delta = finishTime - startTime;

  int64_t checksum = 0;
  for (const auto& entity: entities) {
    for (const auto& component : entity.components) {
      checksum += component->checksum();
    }
  }

  std::cout << delta.count() / 1000 / 1000 << " ms " << checksum << std::endl;
}
