#ifndef PARTICLES
#define PARTICLES
struct low_overhead_entity {
    Vector2 position;
    Vector2 velocity;
};
class ParticleGenerator : public Entity {
public:
    ParticleGenerator(Vector2, float, float, float, unsigned int,  Color);
    void update(Game *game) override;
private:
    std::vector<low_overhead_entity> particles;
    float lifetime;
    Color color;
    float age;
};
#endif