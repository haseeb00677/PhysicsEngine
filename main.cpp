#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <optional>  // Required for SFML 3.0
#include <vector>

// --- Configuration ---
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_OBJECTS = 60;
const float GRAVITY = 980.0f;
const float BOUNCE_FACTOR = 0.7f;

// --- Physics Object Class ---
class PhysicsObject {
 public:
  sf::CircleShape shape;
  sf::Vector2f velocity;
  float mass;

  PhysicsObject(float x, float y, float radius) {
    shape.setPosition({x, y});  // SFML 3 requires Vector2f({x,y})
    shape.setRadius(radius);
    shape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
    shape.setOrigin({radius, radius});

    velocity = sf::Vector2f((rand() % 400 - 200), (rand() % 200 - 100));
    mass = radius;
  }

  void update(float deltaTime) {
    velocity.y += GRAVITY * deltaTime;
    shape.move({velocity.x * deltaTime, velocity.y * deltaTime});

    sf::Vector2f pos = shape.getPosition();
    float r = shape.getRadius();

    // Floor
    if (pos.y + r >= WINDOW_HEIGHT) {
      shape.setPosition({pos.x, WINDOW_HEIGHT - r});
      velocity.y = -velocity.y * BOUNCE_FACTOR;
    }
    // Ceiling
    if (pos.y - r <= 0) {
      shape.setPosition({pos.x, r});
      velocity.y = -velocity.y * BOUNCE_FACTOR;
    }
    // Walls
    if (pos.x + r >= WINDOW_WIDTH) {
      shape.setPosition({WINDOW_WIDTH - r, pos.y});
      velocity.x = -velocity.x * BOUNCE_FACTOR;
    }
    if (pos.x - r <= 0) {
      shape.setPosition({r, pos.y});
      velocity.x = -velocity.x * BOUNCE_FACTOR;
    }
  }
};

// --- Main Engine ---
int main() {
  srand(static_cast<unsigned>(time(0)));

  // SFML 3.0: VideoMode now takes a vector {w, h}
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          "2D Physics Simulation Engine");
  window.setFramerateLimit(60);

  std::vector<PhysicsObject> objects;

  // Initialize Objects
  for (int i = 0; i < NUM_OBJECTS; i++) {
    float r = 10.0f + (rand() % 20);
    float x = r + (rand() % (WINDOW_WIDTH - 100));
    float y = r + (rand() % (WINDOW_HEIGHT / 2));
    objects.emplace_back(x, y, r);
  }

  sf::Clock clock;

  while (window.isOpen()) {
    // SFML 3.0: New Event Handling Loop
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      // Add object on click
      else if (const auto* mouse =
                   event->getIf<sf::Event::MouseButtonPressed>()) {
        objects.emplace_back(static_cast<float>(mouse->position.x),
                             static_cast<float>(mouse->position.y), 15.0f);
      }
    }

    float dt = clock.restart().asSeconds();
    if (dt > 0.1f) dt = 0.1f;

    // Update Physics
    for (size_t i = 0; i < objects.size(); i++) {
      objects[i].update(dt);

      for (size_t j = i + 1; j < objects.size(); j++) {
        sf::Vector2f delta =
            objects[j].shape.getPosition() - objects[i].shape.getPosition();
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        float minDistance =
            objects[i].shape.getRadius() + objects[j].shape.getRadius();

        if (distance < minDistance) {
          float overlap = 0.5f * (distance - minDistance);
          sf::Vector2f offset = delta * (overlap / distance);

          objects[i].shape.move(offset);
          objects[j].shape.move(-offset);

          sf::Vector2f normal = delta / distance;
          sf::Vector2f relativeVelocity =
              objects[j].velocity - objects[i].velocity;
          float velocityAlongNormal =
              relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

          if (velocityAlongNormal > 0) continue;

          float j_val = -(1 + BOUNCE_FACTOR) * velocityAlongNormal;
          j_val /= (1 / objects[i].mass + 1 / objects[j].mass);

          sf::Vector2f impulse = j_val * normal;
          objects[i].velocity -= (1 / objects[i].mass) * impulse;
          objects[j].velocity += (1 / objects[j].mass) * impulse;
        }
      }
    }

    window.clear(sf::Color::Black);
    for (const auto& obj : objects) {
      window.draw(obj.shape);
    }
    window.display();
  }

  return 0;
}
