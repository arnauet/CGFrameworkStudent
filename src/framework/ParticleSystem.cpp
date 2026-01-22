#include "ParticleSystem.h"
#include <cstdlib>

void ParticleSystem::Init(int screenWidth, int screenHeight) {

    //aixo serveix per delimitar la pantalla
    //necessitem width i height per controlar
    // la posicio de les particules i no sortnos fora
    for (int i=0; i<MAX_PARTICLES; i++){

        //posicio aleatoria tipica
        // utilitzant el modul i rand()
        particles[i].position.x = rand() % width;
        particles[i].position.y = rand() % height;

        //velocitat aleatoria
        //farem un efecte neeu llavors
        // la direccio sera cap aval
        particles[i].velocity.x = (rand() % 100 - 50) / 50.0f;  // -1 a 1
        particles[i].velocity.y = -((rand() % 50) + 20);

        // Color blanc-blau per neu
        int brightness = 200 + rand() % 35;
        particles[i].color = Color(brightness, brightness, 255);

        //accelleracio de les particules
        // i desacelaeracio
        particles[i].acceleration = 0.5f;
        //Temps de vida (TTL) de la particula
        particles[i].ttl = (rand() % 500) / 100.0f + 2.0f;  // 2-7 segons
        //finalment la particula mor i retorna
        // a fer el mateix q al pprincipi
        particles[i].inactive = false;

        }
    }


void ParticleSystem::Render(Image* framebuffer) {}



void ParticleSystem::Update(float dt) {}
