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
    //first menu
    Menu.push_back(Button("images/pencil.png", x, y, BUTTON_PENCIL));
    x += buttonSize;
    Menu.push_back(Button("images/line.png", x, y, BUTTON_LINE));
        x += buttonSize;



    std::cout << "Menu carregat amb " << Menu.size() << " botons" << std::endl;


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
	    // to try primittives implementation
		//painting = true;
        //drag_x0 = drag_x1 = event.x;
        //drag_y0 = drag_y1 = window_height -1-event.y; //flip Y do to behavior problms

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        //painting = false;
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
