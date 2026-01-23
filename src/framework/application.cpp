#include "application.h"
#include "framework.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	//bool isFilled = true;
	this->framebuffer.Resize(w, h);

	//for the final Menu
	this->currentTool = BUTTON_PENCIL;
    this->currentColor = Color::BLACK;
    this->paintingColor = Color::CYAN;
    this->painting = false;
    this->triangleClickCount = 0;
    this->menuHeight = 32;
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	//black screen by default
	Paint.Resize(window_width, window_height);
	Paint.Fill(currentColor);

	//uploading image to try see DrawImage working.
	//bool ok = imagen.LoadPNG("images/save.png", true);
	//std::cout << "Loadsed? " << ok << " size=" << imagen.width << "x" << imagen.height << "\n";
	//A veure si funciona un sol boto del Menu final

	///carreguem bottons del menu
	int buttonSize = 32;
    int x = 0;
    int y = 0;

    //MENU ICON LOAD
    //each loads the path we need for the icons img
    Menu.push_back(Button("images/pencil.png", x, y, BUTTON_PENCIL));
    x += buttonSize;//desplacem pel proxim button a renderitzar

    //lines, rectangles and triangles
    Menu.push_back(Button("images/line.png", x, y, BUTTON_LINE));
    x += buttonSize;//same as before
    Menu.push_back(Button("images/rectangle.png", x, y, BUTTON_RECTANGLE));
    x += buttonSize;
    Menu.push_back(Button("images/triangle.png", x, y, BUTTON_TRIANGLE));
    x += buttonSize;

    //clear the Painting Screen
    Menu.push_back(Button("images/eraser.png", x, y, BUTTON_ERASER));
    x += buttonSize;

    // loaad
    Menu.push_back(Button("images/load.png", x, y, BUTTON_LOAD));
    x += buttonSize;

    // save the painting
    Menu.push_back(Button("images/save.png", x, y, BUTTON_SAVE));
    x += buttonSize;

    //Colors avaliable in the menu.
    Menu.push_back(Button("images/black.png", x, y, BUTTON_COLOR_BLACK));
    x += buttonSize;
    Menu.push_back(Button("images/white.png", x, y, BUTTON_COLOR_WHITE));
    x += buttonSize;
    Menu.push_back(Button("images/pink.png", x, y, BUTTON_COLOR_PINK));
    x += buttonSize;
    Menu.push_back(Button("images/yellow.png", x, y, BUTTON_COLOR_YELLOW));
    x += buttonSize;
    Menu.push_back(Button("images/red.png", x, y, BUTTON_COLOR_RED));
    x += buttonSize;
    Menu.push_back(Button("images/blue.png", x, y, BUTTON_COLOR_BLUE));
    x += buttonSize;
    Menu.push_back(Button("images/cyan.png", x, y, BUTTON_COLOR_CYAN));
    x += buttonSize;

    std::cout << "Menu carregat amb " << Menu.size() << " botonss!!!" << std::endl;
}

//REMEMBER>>>!!!!
//to compile cmake --build . -j"$(nproc)"
//to execute ./ComputerGraphics

// Render one frame
void Application::Render(void)
{
    // el nostre paint conte tot el que el usuari
    // dibuixa a la screen, llavors li passem
    // al framebuffer perque el copi
    framebuffer.Fill(Color::BLACK);
    framebuffer.DrawImage(Paint, 0, 0);

    //si l'usuari esta pintant
    if (painting) {
        switch (currentTool) {
            //cas mode de dibbuixar lineees
            case BUTTON_LINE:
                framebuffer.DrawLineDDA(
                    startPoint.x, startPoint.y,
                    mouse_position.x, mouse_position.y,
                    paintingColor
                );
                break;
            //cas mode de dibuixar rectangles
            case BUTTON_RECTANGLE:
                framebuffer.DrawRect(
                    startPoint.x, startPoint.y,
                    mouse_position.x - startPoint.x,
                    mouse_position.y - startPoint.y,
                    // addint interactivity paintingColor, 1, false, paintingColor
                    //utilitznat nova variable bborderWidth
                    paintingColor, borderWidth, false, paintingColor
                );
                break;
            case BUTTON_TRIANGLE:
                //dibuixant triangle
                if (triangleClickCount >= 1) {
                    framebuffer.DrawLineDDA(startPoint.x, startPoint.y,
                        mouse_position.x, mouse_position.y, paintingColor);
                }
                if (triangleClickCount >= 2) {
                    framebuffer.DrawLineDDA(secondPoint.x, secondPoint.y,
                        mouse_position.x, mouse_position.y, paintingColor);
                }
                break;
            default:
                break;
        }
    }

    //dibuixem el menu del projecte
    for (int i = 0; i < Menu.size(); i++) {//iterem sobre el vector menu
            Menu[i].Render(framebuffer);
        }

    //quan el usuari activi
    // el mode animacio
    //hem de pasar el particle system
    // les smateixes coordenades de la pantalla

    if (animationMode) {
        particleSystem.Render(&framebuffer);
    }

    framebuffer.Render();//actualitzem el framebuffer amb icons i la resta
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    if (animationMode) {
        particleSystem.Update(seconds_elapsed);
    }
}


// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // TO ADD INTERACTIVITY
    //
    // KEYS NEEDED FOR LAB 1 ------------  ACTION EXPECTED
    //
    //       1                               Paint mode
    //       2                               Animation
    //       F                              Fill Shapes
    //       +                             Increase Border
    //       -                             DEcreease Border

    switch(event.keysym.sym) {
        case SDLK_ESCAPE:
            std::cout << "Tancant aplicacio..." << std::endl;
            exit(0); //ESC to kill the app..
            break;

        case SDLK_1:
            std::cout << "Mode: PAINT!!" << std::endl;
            // Ja estem en mode paint per defaultt
            break;

        case SDLK_2:
            if (!animationMode) {
                animationMode = true;
                particleSystem.Init(window_width, window_height);
                std::cout << "Mode: ANIMATION ON (Snow)" << std::endl;
            } else {
                particleSystem.NextAnimation();
                switch (particleSystem.GetCurrentAnimation()) {
                    case ANIM_SNOW:      std::cout << "Animacio: SNOW" << std::endl; break;
                    case ANIM_STARFIELD: std::cout << "Animacio: STARFIELD" << std::endl; break;
                    case ANIM_EXPLOSION: std::cout << "Animacio: EXPLOSION" << std::endl; break;
                    case ANIM_IMPLOSION: std::cout << "Animacio: IMPLOSION" << std::endl; break;
                    default: break;
                        }
                }
                break;

        case SDLK_f:
            fillShapes = !fillShapes;
            std::cout << "Fill shapes: " << (fillShapes ? "Omplint figures!" : "figurees vuides!!") << std::endl;
            break;

        case SDLK_PLUS:
        case SDLK_KP_PLUS:  // Teclat numèric
        case SDLK_EQUALS:  //incrementacio del gruixut ambb +
            borderWidth++;
            std::cout << "Border width: " << borderWidth << std::endl;
            break;

        //borderminus retraccio amb -
        // per si de cas altes tecles
        // o teclat en altres formats  K60/K75..
        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            if (borderWidth > 1) borderWidth--;
            std::cout << "Border width: " << borderWidth << std::endl;
            break;
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        //utilizarem un Vector2 per referenciar la psicio del mouse
        //tmbe haurem de convertir coordenades SDL a OpenGL
        Vector2 mousePos(event.x, window_height - event.y - 1);

        //necessitem mirar si el usueari a clicat algun boto concret
        //iterem sobre el menu disponible del user
        for (size_t i = 0; i < Menu.size(); i++) {
            if (Menu[i].IsMouseInside(mousePos)) {

                ButtonType temporal_type = Menu[i].buttonMenu; //boto temporal per trial menu
                //selecccio del color de la pantalla a traves del menu inferior
                if (temporal_type >= BUTTON_COLOR_BLACK) {
                    switch (temporal_type) {

                        case BUTTON_COLOR_BLACK:
                            //dinamica CASES primer triem el color
                            //que ha triat el usuari
                            //tornem a pintar el Paint amb el colo escollit
                            //missatge a CLI & break
                            paintingColor = Color::BLACK;
                           // Paint.Fill(currentColor);
                            std::cout << "Color canviat a negre" << std::endl;
                            break;
                        case BUTTON_COLOR_WHITE:
                            //dinamica igual al del primer cas
                            paintingColor = Color::WHITE;
                            //Paint.Fill(currentColor); //updating current color
                            std::cout << "Color canviat a blanc " << std::endl;
                            break;
                        case BUTTON_COLOR_RED:
                            paintingColor = Color::RED;
                            //Paint.Fill(currentColor);
                            std::cout << "Color canviat a vermell " << std::endl;
                            break;
                        case BUTTON_COLOR_BLUE:
                            paintingColor = Color::BLUE;
                           // Paint.Fill(currentColor);
                            std::cout << "Color canviat a bblau " << std::endl;
                            break;
                        case BUTTON_COLOR_YELLOW:
                            paintingColor = Color::YELLOW;
                            //Paint.Fill(currentColor);
                            std::cout << "Colour canviat a groc" << std::endl;
                            break;
                        case BUTTON_COLOR_CYAN:
                            paintingColor = Color::CYAN;
                            //Paint.Fill(currentColor);
                            std::cout << "Color canviatt a cyan" << std::endl;
                            break;
                        case BUTTON_COLOR_PINK:
                            paintingColor = Color(255,182,193);
                            //Paint.Fill(currentColor);//pink background
                            std::cout << "Color canviat a rosaa" << std::endl;
                            break;
                        default: break;
                    }
                }
                //Controlant els botons diferents
                // amb ambb funcionalitat mes complexes
                else if (temporal_type == BUTTON_PENCIL    ||
                        temporal_type == BUTTON_LINE       ||
                        temporal_type == BUTTON_RECTANGLE  ||
                        temporal_type == BUTTON_TRIANGLE) {
                    currentTool = temporal_type;
                    triangleClickCount = 0; // Reset triangle state
                    std::cout << "Eina seleccionada: " << temporal_type << std::endl;
                }
                //cleanse mode menuu
                else if (temporal_type == BUTTON_ERASER) {
                    Paint.Fill(currentColor); //Netejeem amb el color de fons actual
                    std::cout << "Canvas netejat" << std::endl;
                }
                //per guarda el paint realitzar
                // per l'usuari en format tga
                else if (temporal_type == BUTTON_SAVE) {
                    Paint.SaveTGA("output.tga");
                    std::cout << "Imatge guardada" << std::endl;
                }
                else if (temporal_type == BUTTON_LOAD) {
                    Paint.LoadTGA("");
                    std::cout << "Imatge carregada" << std::endl;
                }
                return;
            }
        }
        // si arribem aqui el usuari ja ha seleccionat el menu
        // per tant es pot pintar o es pot haver ralitzat una modificacio
        // del color de fons del paint a un altre color escollit
        // aquesta dinamica la gestionem amb un IF

        if (currentTool == BUTTON_TRIANGLE) {
            // x dibuixar un Triangle necessitarem 3 clicks
            if (triangleClickCount == 0) {
                startPoint = mousePos;
                triangleClickCount = 1;
                painting = true;
            } else if (triangleClickCount == 1) {
                secondPoint = mousePos;
                triangleClickCount = 2;
            } else if (triangleClickCount == 2) {
                //dibuixem el triangle ambb
                //el metode implementat a imatge.cpp
                Paint.DrawTriangle(startPoint, secondPoint, mousePos,
                    paintingColor, fillShapes, Color(255,105,180)); //fixed error ambb currenColour
                triangleClickCount = 0;
                painting = false;
                std::cout << "Triangle dibuixaat!!" << std::endl;
            }
        } else {
            //...
            startPoint = mousePos;
            mouse_position = mousePos; //importante per dibubixar ambb el llapis
            painting = true;
        }
	}
}



void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    //En cas que l'usuari hagi escollit fons
    //Paint.Fill(currentColor);// refresh the background

    if (event.button == SDL_BUTTON_LEFT && painting) {

	    //utilitzem vector2 per guarda la mouse position
        Vector2 mousePos(event.x, window_height - event.y - 1);
        switch (currentTool) {
            case BUTTON_LINE:
                Paint.DrawLineDDA(
                    startPoint.x, startPoint.y,
                    mousePos.x, mousePos.y,
                    paintingColor
                );

                //debugging purposes
                std::cout << "Linia dibuixaada!" << std::endl;
                painting = false;
                break;

            case BUTTON_RECTANGLE:
            //utilitzem la funcio desenvolupada per
            // el apartat 2.1.2 DrawRect
                Paint.DrawRect(
                    startPoint.x, startPoint.y,
                    mousePos.x - startPoint.x,
                    mousePos.y - startPoint.y,
                    //adding interactivity from --> currentColor, 1, true, paintingColor  /
                    paintingColor, borderWidth, fillShapes, paintingColor
                );
                std::cout << "Rectanggle dibuixat!" << std::endl;
                painting = false;
                break;

            case BUTTON_PENCIL:
                //parem de pintar quan el llapis
                // aixeca el cursor el usuari
                //el llapis para de pintar
                painting = false;
                break;
            case BUTTON_TRIANGLE:
                break;
            default:
                painting = false;
                break;
        }
	}
}


void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    Vector2 mousePos(event.x, window_height - event.y - 1);
    //pencil mode menu
    // el user continuara pintant mentre es mogui
    if (painting && currentTool == BUTTON_PENCIL) {
        Paint.DrawLineDDA(
            mouse_position.x, mouse_position.y,
            mousePos.x, mousePos.y,
            paintingColor
        );
    }

    mouse_position = mousePos;
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}
