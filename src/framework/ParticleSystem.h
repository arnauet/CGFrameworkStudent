#pragma once
#include "image.h"
#include "framework.h"

enum AnimationType {
    ANIM_SNOW,
    ANIM_STARFIELD,
    ANIM_EXPLOSION,
    ANIM_IMPLOSION,
    ANIM_COUNT
};

class ParticleSystem {
    static const int MAX_PARTICLES = 150;
    static const int TRAIL_LENGTH = 10;  // Longitud de la cua

    struct Particle {
        Vector2 position;
        Vector2 velocity;
        Vector2 trail[TRAIL_LENGTH];  // Historial de posicions per la cua
        int trailIndex;
        Color color;
        float acceleration;
        float ttl;
        bool inactive;
    };

    Particle particles[MAX_PARTICLES];
    int width, height;
    AnimationType currentAnim;

    void InitSnow();
    void InitStarfield();
    void InitExplosion();
    void InitImplosion();

public:
    void Init(int screenWidth, int screenHeight);
    void Render(Image* framebuffer);
    void Update(float dt);
    void NextAnimation();
    AnimationType GetCurrentAnimation() { return currentAnim; }
};
