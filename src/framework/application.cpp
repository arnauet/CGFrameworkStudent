#include "application.h"
#include "framework.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	//to try primitives
	//utilizant moouse states
	//this->painting = false;
	//this->drag_x0 = this->drag_y0 = 0;
	//this->drag_x1 = this->drag_y1 = 0;

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
    this->currentColor = Color::WHITE;
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
	Paint.Fill(Color::BLACK);

	//uploading image to try see DrawImage working.
	//bool ok = imagen.LoadPNG("images/save.png", true);
	//std::cout << "Loaded? " << ok << " size=" << imagen.width << "x" << imagen.height << "\n";
	//A veure si funciona un sol boto del Menu final

	///carreguem bottons del menu
	int buttonSize = 32;
    int x = 0;
    int y = 0;

    //MENU ICON LOAD

    //each loads the path nedeed
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
    // al framebuffer
    framebuffer.DrawImage(Paint, 0, 0);

    //si l'usuari esta pintant

    if(painting){
        //procedure
    }



    //Actualitzem i dibuixem el menu del projecte

    for (int i = 0; i < Menu.size(); i++) {
            Menu[i].Render(framebuffer);
        }
        framebuffer.Render();



}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//     keyboard press event
//
// TO ADD INTERACTIVITY
//
// KEYS NEEDED FOR LAB 1  ----- ACTION EXPECTED
//
//    1                             Paint mode
//    2                              Animation
//    F                             Fill Shapes
//    +                            Increase Border
//    -                            DEcreease Border
//

// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    switch(event.keysym.sym) {

        case SDLK_ESCAPE: exit(0);
        std::cout << "Tancant aplicacio..." << std::endl;
        break; // ESC key, kill the app

        //Requested for Lab 1.
        //interactvity ONKEYPRESSED differentt scenarios
    	case SDLK_1:break;
    	case SDLK_2:break;
        case SDLK_f:break;
        case SDLK_MINUS: break;




    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
    //utilizarem un Vector2 per referenciar la psicio del mouse
    //tmbe haurem de convertir coordenades SDL a OpenGL
        Vector2 mousePos(event.x, window_height - event.y - 1);

        //necessitem mirar si el usueari a clicat algun boto
        //iterem sobre el menu disponible del user
        for (size_t i = 0; i < Menu.size(); i++) {
            if (Menu[i].IsMouseInside(mousePos)) {
                ButtonType temporal_type = Menu[i].buttonMenu; //boto temporal per trial menu
                //selecccio del color de la pantalla a traves del menu inferior
                if (temporal_type >= BUTTON_COLOR_BLACK) {
                    switch (temporal_type) {
                        case BUTTON_COLOR_BLACK:
                            currentColor = Color::BLACK;
                            std::cout << "Color canviat a negre" << std::endl;
                            break;

                        case BUTTON_COLOR_WHITE:
                            currentColor = Color::WHITE;
                            std::cout << "Color canviat a blanc " << std::endl;
                            break;

                        case BUTTON_COLOR_RED:
                            currentColor = Color::RED;
                            std::cout << "Color canviat a vermell " << std::endl;
                            break;

                        case BUTTON_COLOR_BLUE:
                            currentColor = Color::BLUE;
                            std::cout << "Color canviat a bblau " << std::endl;
                            break;

                        case BUTTON_COLOR_YELLOW:
                            currentColor = Color::YELLOW;
                            std::cout << "Color canviat a groc" << std::endl;
                            break;

                        case BUTTON_COLOR_CYAN:
                            currentColor = Color::CYAN;
                            std::cout << "Color canviat a cyan" << std::endl;
                            break;

                        case BUTTON_COLOR_PINK:
                            currentColor = Color::PURPLE;
                            break;

                        default: break;
                    }
                    //KEYLINE necessaria pel canvi de fons
                    Paint.Fill(currentColor);
                }
                return;
            }
        }
	}
}



void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        //painting = false then is not working
        //

        Vector2 mousePos(event.x, window_height - event.y - 1);
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{

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
