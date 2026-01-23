#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>


//inicialitzacio del sistema de particules
//necessitem les dimensions de la pantalla
//per controlar els limits de les particules

void ParticleSystem::Init(int screenWidth, int screenHeight) {
    //guardeem les dimensions per utilitzarles despres
    width = screenWidth;
    height = screenHeight;
    //comencem amb la animacio de neu per defecte
    currentAnim = ANIM_SNOW;
    InitSnow();
}


//funcio per canviar entre animacions
//cada cop que lusuari prem la tecla 2
//passem a la seguent animacio del enum

void ParticleSystem::NextAnimation() {
    //utilitzem modul per ciclar entre animacions
    //ANIM_COUNT es el nombre total danimacions
    currentAnim = (AnimationType)((currentAnim + 1) % ANIM_COUNT);

    //inicialitzem lanimacio corresponent
    switch (currentAnim) {
        case ANIM_SNOW:      InitSnow();      break;
        case ANIM_STARFIELD: InitStarfield(); break;
        case ANIM_EXPLOSION: InitExplosion(); break;
        case ANIM_IMPLOSION: InitImplosion(); break;
        default: break;
    }
}


// ========/========= SN0W effect ================
//animacio de neu caient de dalt a baix
//les particules son blanques i cauen lentament
//sense deixar rastre (nomes el punt)

void ParticleSystem::InitSnow() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        //posicio aleatoria per tota la pantalla
        particles[i].position.x = rand() % width;
        particles[i].position.y = rand() % height;

        //velocitat cap avall amb una mica de deriva lateral
        //y negatiu perque en opengl y=0 es abaix
        particles[i].velocity.x = ((rand() % 10) - 5) * 0.1f;  //deriva lateral petita
        particles[i].velocity.y = -((rand() % 3) + 2);  //2-4 pixels per frame cap avall

        //color blanc per la neu
        particles[i].color = Color(255, 255, 255);
        particles[i].ttl = 9999;  //temps de vida molt llarg
        particles[i].inactive = false;

        //inicialitzem el trail encara que no lutilitzem per la neu
        for (int j = 0; j < TRAIL_LENGTH; j++) {
            particles[i].trail[j] = particles[i].position;
        }
        particles[i].trailIndex = 0;
    }
}


//-============= STARFIELD ==============/=
//animacio tipus velocitat de la llum
//les particules surten del centre i acceleren
//deixant una cua que sestiira com mes rapides van

void ParticleSystem::InitStarfield() {
    //calculem el centre de la pantalla
    float cx = width / 2.0f;
    float cy = height / 2.0f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        //angle aleatori en radians (0 a 360 graus)
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        //distancia inicial prop del centre
        float dist = rand() % 20 + 5;

        //posicio inicial al voltant del centre
        particles[i].position.x = cx + cos(angle) * dist;
        particles[i].position.y = cy + sin(angle) * dist;

        //velocitat inicial lenta en direccio radial
        //cos i sin per moures en la direccio del angle
        particles[i].velocity.x = cos(angle) * 0.5f;
        particles[i].velocity.y = sin(angle) * 0.5f;

        //acceleracio multiplicativa (1.05 = +5% cada frame)
        //aixo fa que les particules accelerin exponencialment
        particles[i].acceleration = 1.05f;

        //color blanc per les estrelles
        particles[i].color = Color(255, 134, 255);
        particles[i].ttl = 9999;
        particles[i].inactive = false;

        //inicialitzem el trail amb la posicio actual
        for (int j = 0; j < TRAIL_LENGTH; j++) {
            //particles[i].color = Color(134, 134, 255);
            particles[i].trail[j] = particles[i].position;
        }
        particles[i].trailIndex = 0;
    }
}


// ==================== EXPLOSION ==========//
//animacio dexplosio de foc des del centre
//les particules surten rapides i van frenant
//colors grocs i taronges com flames

void ParticleSystem::InitExplosion() {
    //centre de lexplosio
    float cx = width / 2.0f;
    float cy = height / 2.0f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        //angle aleatori per distribuir les particules
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        //velocitat inicial entre 1 i 4 pixels per frame
        float speed = ((rand() % 30) + 10) / 10.0f;

        //totes comencen al centre de lexplosio
        particles[i].position.x = cx;
        particles[i].position.y = cy;

        //velocitat en la direccio del angle
        particles[i].velocity.x = cos(angle) * speed;
        particles[i].velocity.y = sin(angle) * speed;

        //acceleracio menor que 1 per frenar (0.98 = -2% cada frame)
        particles[i].acceleration = 0.98f;

        //colors de foc (groc a taronja)
        //el vermell sempre a 255, el verd aleatori
        particles[i].color = Color(255, 100 + rand() % 155, 0);

        //temps de vida limitat per que mori lexplosio
        particles[i].ttl = 200;  //200 frames de vida
        particles[i].inactive = false;

        //inicialitzar trail al centre
        for (int j = 0; j < TRAIL_LENGTH; j++) {
            particles[i].trail[j] = particles[i].position;
        }
        particles[i].trailIndex = 0;
    }
}


// =========== IMPLOSION ====================/
//animacio contraria a lexplosio
//les particules venen dels marges i acceleren cap al centre
//efecte de forat negre o absorcio

void ParticleSystem::InitImplosion() {
    //el centre on convergeixen les particules
    float cx = width / 2.0f;
    float cy = height / 2.0f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        //angle aleatori per distribuir al voltant
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        //distancia lluny del centre (150 a 250 pixels)
        float dist = 150 + rand() % 100;

        //posicio inicial als marges de la pantalla
        particles[i].position.x = cx + cos(angle) * dist;
        particles[i].position.y = cy + sin(angle) * dist;

        //velocitat cap al centre (negativa respecte langle)
        //el signe negatiu fa que vagin cap endins
        particles[i].velocity.x = -cos(angle) * 0.5f;
        particles[i].velocity.y = -sin(angle) * 0.5f;

        //acceleracio per que vagin mes rapid quan sapropen
        particles[i].acceleration = 1.03f;

        //color cyan per diferenciar de lexplosio
        particles[i].color = Color(0, 255, 255);
        particles[i].ttl = 9999;
        particles[i].inactive = false;

        //inicialitzar trail
        for (int j = 0; j < TRAIL_LENGTH; j++) {
            particles[i].trail[j] = particles[i].position;
        }
        particles[i].trailIndex = 0;
    }
}

//dibuixem totes les particules al framebuffer
//la neu nomes es un punt, les altres tenen cua
//la cua es dibuixa mes fosca i petita que el cap

void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        //saltem les particules inactives
        if (particles[i].inactive) continue;

        Particle& p = particles[i];
        //convertim la posicio float a int per pintar pixels
        int x = (int)p.position.x;
        int y = (int)p.position.y;

        //la neu nomes dibuixa un punt blanc 5x5
        if (currentAnim == ANIM_SNOW) {
            //bucle per dibuixar un quadrat de 5x5 pixels
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    framebuffer->SetPixel(x + dx, y + dy, Color(255, 255, 255));
                }
            }
        }
        //les altres animacions dibuixen la cua (trail)
        else {
            //dibuixar el trail de mes antic a mes nou
            for (int t = 0; t < TRAIL_LENGTH; t++) {
                //calculem lindex circular del trail
                int idx = (p.trailIndex + t) % TRAIL_LENGTH;
                int tx = (int)p.trail[idx].x;
                int ty = (int)p.trail[idx].y;

                //el color es mes brillant quan mes nou es el punt
                //t=0 es el mes antic (fosc), t=TRAIL_LENGTH-1 es el mes nou (brillant)
                int bright = (t * 255) / TRAIL_LENGTH;
                Color trailColor(
                    (p.color.r * bright) / 255,
                    (p.color.g * bright) / 255,
                    (p.color.b * bright) / 255
                );

                //la cua es un punt 3x3
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        framebuffer->SetPixel(tx + dx, ty + dy, trailColor);
                    }
                }
            }

            //el cap de la particula es mes gran (5x5) i del color original
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    framebuffer->SetPixel(x + dx, y + dy, p.color);
                }
            }
        }
    }
}

//actualitzem la posicio de totes les particules
//cada animacio te el seu comportament especific
//les particules inactives es reinicialitzen (respawn)

void ParticleSystem::Update(float dt) {
    //ignorem dt perque el moviment es per frame
    //aixi es mes predictible i consistent
    (void)dt;

    //centre de la pantalla per calculs
    float cx = width / 2.0f;
    float cy = height / 2.0f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle& p = particles[i];

        //si la particula esta inactiva la reinicialitzem
        if (p.inactive) {
            //cada animacio te el seu respawn diferent
            switch (currentAnim) {
                case ANIM_SNOW:
                    //la neu reapareix a dalt de la pantalla
                    p.position.x = rand() % width;
                    p.position.y = height;  //a dalt de tot
                    p.velocity.x = ((rand() % 10) - 5) * 0.1f;
                    p.velocity.y = -((rand() % 3) + 2);
                    break;

                case ANIM_STARFIELD: {
                    //les estrelles reapareixen al centre
                    float angle = (rand() % 360) * 3.14159f / 180.0f;
                    p.position.x = cx + cos(angle) * 5;
                    p.position.y = cy + sin(angle) * 5;
                    p.velocity.x = cos(angle) * 0.5f;
                    p.velocity.y = sin(angle) * 0.5f;
                    p.acceleration = 1.05f;
                    break;
                }

                case ANIM_EXPLOSION: {
                    //lexplosio reapareix al centre amb nova direccio
                    float angle = (rand() % 360) * 3.14159f / 180.0f;
                    float speed = ((rand() % 30) + 10) / 10.0f;
                    p.position.x = cx;
                    p.position.y = cy;
                    p.velocity.x = cos(angle) * speed;
                    p.velocity.y = sin(angle) * speed;
                    p.acceleration = 0.98f;
                    p.ttl = 200;
                    break;
                }

                case ANIM_IMPLOSION: {
                    //la implosio reapareix als marges
                    float angle = (rand() % 360) * 3.14159f / 180.0f;
                    float dist = 150 + rand() % 100;
                    p.position.x = cx + cos(angle) * dist;
                    p.position.y = cy + sin(angle) * dist;
                    p.velocity.x = -cos(angle) * 0.5f;
                    p.velocity.y = -sin(angle) * 0.5f;
                    p.acceleration = 1.03f;
                    break;
                }
                default: break;
            }
            //reinicialitzem el trail amb la nova posicio
            for (int j = 0; j < TRAIL_LENGTH; j++) {
                p.trail[j] = p.position;
            }
            p.trailIndex = 0;
            p.inactive = false;
            continue;  //passem a la seguent particula
        }

        //guardem la posicio actual al trail (buffer circular)
        p.trail[p.trailIndex] = p.position;
        p.trailIndex = (p.trailIndex + 1) % TRAIL_LENGTH;

        //apliquem acceleracio multiplicativa
        //starfield i implosion acceleren, explosion frena
        if (currentAnim == ANIM_STARFIELD || currentAnim == ANIM_IMPLOSION || currentAnim == ANIM_EXPLOSION) {
            p.velocity.x *= p.acceleration;
            p.velocity.y *= p.acceleration;
        }

        //actualitzem la posicio amb la velocitat
        p.position.x += p.velocity.x;
        p.position.y += p.velocity.y;

        //reduim el temps de vida
        p.ttl -= 1;

        //comprovem si la particula ha sortit de la pantalla
        bool out = p.position.x < -10 || p.position.x >= width + 10 ||
                   p.position.y < -10 || p.position.y >= height + 10;

        //cas especial: implosion mor quan arriba al centre
        if (currentAnim == ANIM_IMPLOSION) {
            float dx = p.position.x - cx;
            float dy = p.position.y - cy;
            //si esta a menys de 5 pixels del centre la desactivem
            if (sqrt(dx*dx + dy*dy) < 5) out = true;
        }

        //desactivem la particula si ha sortit o ha mort inactiva
        if (out || p.ttl <= 0) {
            p.inactive = true;
        }
    }
}
