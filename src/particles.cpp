#include <particles.h>

ParticleGenerator::ParticleGenerator(Vector2 origin, 
    float explosionForce, float lifetime, float particleSpawnRegion, 
    unsigned int particleAmount,
    Color color) 
{
    this->lifetime = lifetime;
    this->color = color;

    // Generate the random particles in the particleSpawnRegion
    for (unsigned int i = 0; i < particleAmount; i++) {
        // consider a circular region with Radius R as the bounds.
        float R = particleSpawnRegion;
        // generate a random point in the bounds via polar coordinates.
        float theta = -(float)GetRandomValue( 0, 100 * PI ) / 100.0f * 180 / ( PI);
        float r = GetRandomValue(0, 100*R) / 100.0f;
        Vector2 right = (Vector2){1.0f, 0.0f};
        Vector2 velDir = Vector2Rotate(right, theta);
        Vector2 displacement = Vector2Scale(velDir, r);
        Vector2 newParticlePos = Vector2Add(origin, displacement);  

        /*int minX = -R/2; int maxX = R/2;
        int X = GetRandomValue(minX, maxX);
        int minY = -sqrtf( R*R - X*X );
        int maxY = sqrtf( R*R - X*X );
        int Y = GetRandomValue(minY, maxY);*/

        // so the explosion is basically a vector field of forces. and the forces decay as you move radially outward. It is also
        // symettric radially.
        float force = explosionForce - r;
        float pertubation = GetRandomValue(0.25 * -100 * explosionForce, 0.25 * 100 * explosionForce) / 200.0f;
        force += pertubation;
        if (force < 0.0f) force = 0.0f; 
        Vector2 newParticleVel = Vector2Scale(velDir, force);
        //Vector2 newParticleVel = Vector2Zero();

        this->particles.push_back((low_overhead_entity){
            newParticlePos,
            newParticleVel
        });
    }
}

void ParticleGenerator::update(Game *game) {

    // has the particle generator exceeed the lifetime?

    if (age > lifetime) {
        // Kill the particle generator!!!!
        // TODO: Make this even better. Because the ParticleGenerator object still exists....
        this->particles.clear();
        age = 0;
        //delete this; // NOTE: We must always create particle generators with new keyword.
    }
    float deltaTime = GetFrameTime();
    this->age += deltaTime;

    // TODO: Much less gravity.
    // Smaller amount of particles.
    // air friction

    for (unsigned int i = 0; i < this->particles.size(); i++) {
        // take the particle. // render it.
        // move it according to the current velocity.
        // then update the current velocity with gravity.
        low_overhead_entity particle = particles[i];
        DrawCircle(particle.position.x, particle.position.y, 2.0f, this->color);
        particles[i].position = Vector2Add(particle.position, Vector2Scale(particle.velocity,deltaTime));
        
        // Apply large amounts of air friction for "locking"
        {
            float currentVelLen = Vector2Length(particles[i].velocity);
            if (currentVelLen != 0) {
                float accelFactor = (Ground::airFriction) / 1.5f * currentVelLen;
                Vector2 accelVec = Vector2Scale( Vector2Negate(particles[i].velocity) , (accelFactor / currentVelLen * deltaTime) ); 
                particles[i].velocity = Vector2Add(particles[i].velocity, (Vector2){accelVec.x, 0.0f});
            }
        }

        particles[i].velocity.y += Entity::gravity / 10.0f * deltaTime; // make particles not so heavy...
    }
}