#include "image.h"
#include "framework.h"



class ParticleSystem {

        static const int MAX_PARTICLES = 20000; //maxim de particules

        struct Particle {
                Vector2 position; //positio de les particule
                Vector2 velocity; //Veocitat de les particules
                Color color; //color rgb
                float acceleration; // acceleracio
                float ttl; // temps de vida fins q la particula reinicia
                bool inactive; //quan les particules eexpirem reinicien
        };

        Particle particles[MAX_PARTICLES];
        int width, height;

public:
        void Init(int screenWidth, int screenHeight);
        void Render(Image* framebuffer);
        void Update(float dt);
};
