#ifndef BULLETS
#define BULLETS

#include "raylib_wrapped.h"
#include "entity.h"

struct bullet_hit_info {
    Vector2 hitPos;
    Entity *entityHit;
};

typedef void (bullet_hit_callback)(bullet_hit_info);

class Bullets : public Entity {
    // so a bullets object is basically this beam thing.
    // and there are many bullets inside of an array that are tracked by this bullets object.
public:
    ~Bullets();
    // Setting this object to active means it can accept bullet generation requests.
    void SetActive(Vector2, Vector2, float, float, bullet_hit_callback *);
    // We call SetTarget to define where the bullets stream is directed. May be called multiple times during SetActive.
    void SetTarget(Vector2);
    void SetWidth(float);
    // spawns a new bullet in the stream
    void Shoot(); 
    void Deactivate();
    void update(Game *) override;
    Vector2 GetShootDir();
    bool CheckCollisionWithRectangle(Rectangle rect, Vector2 lastBulletPos, Vector2 currentBulletPos, Vector2 *collisionPoint);
private:
    // stores the list of bullets
    std::vector<Vector2> bullets;
    float width; // width of the bullet stream.
    Vector2 origin; // where the bullets are shot from
    Vector2 target;
    bullet_hit_callback *callback;
    bool active;
    float bulletVelocity; // Velocity dimensions are in pixels / second.
    #ifdef DEBUG
    FILE *debugFileLog;
    #endif
};
#endif 