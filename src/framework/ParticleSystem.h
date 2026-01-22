#include "image.h"
#include "framework.h"



class ParticleSystem {

        static const int MAX_PARTICLES = 20000;

        struct Particle {
                Vector2 position;
                Vector2 velocity; // Normalized speed and direction of the particle
                Color color;
                float acceleration;
                float ttl; // Time left until the particle expires
                bool inactive; // Particle is not used/expired, so it can be recreated
        };

        Particle particles[MAX_PARTICLES];
        int width, height;

public:
        void Init(int screenWidth, int screenHeight);
        void Render(Image* framebuffer);
        void Update(float dt);
};
