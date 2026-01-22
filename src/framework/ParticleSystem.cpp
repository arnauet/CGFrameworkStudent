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

//recorrer totes les particules
// i dibuixa una per pixel
// si la particula esta activa
void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].inactive) {
            //casteig per convertir
            // la posicio de float a Int
            int x = (int)particles[i].position.x;
            int y = (int)particles[i].position.y;
            //dibuixa la particula al pixel q toca
            // controlem els limites de la pantalla
            if (x >= 0 && x < width && y >= 0 && y < height)
                framebuffer->SetPixel(x, y, particles[i].color);
        }
    }
}




void ParticleSystem::Update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].inactive) {
            //tornen a apareix les particules
            particles[i].position.x = rand() % width;
            particles[i].position.y = height;  // Dalt de tot screen
            particles[i].ttl = (rand() % 500) / 100.0f + 2.0f;
            particles[i].inactive = false;
        }

        // aactualitzar posició: P_t+1 = P_t + V * dt
        particles[i].velocity.y -= particles[i].acceleration * dt;
        particles[i].position.x += particles[i].velocity.x * dt * 60;
        particles[i].position.y += particles[i].velocity.y * dt * 60;

        //reuint temps de vida
        particles[i].ttl -= dt;

        //marquem inactiva si surt de la pantalla o mor
        if (particles[i].ttl <= 0 ||
            particles[i].position.y < 0 ||
            particles[i].position.x < 0 ||
            particles[i].position.x >= width) {
            particles[i].inactive = true;
        }
    }
}
