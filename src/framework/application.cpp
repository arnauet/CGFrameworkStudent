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
	this->painting = false;
	this->drag_x0 = this->drag_y0 = 0;
	this->drag_x1 = this->drag_y1 = 0;

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	//bool isFilled = true;

	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	//uploading image to try see DrawImage working.
	bool ok = imagen.LoadPNG("images/save.png", true);
	std::cout << "Loaded? " << ok << " size=" << imagen.width << "x" << imagen.height << "\n";

}



//REMEMBER>>>!!!!
//to compile cmake --build . -j"$(nproc)"
//to execute ./ComputerGraphics

// Render one frame
void Application::Render(void)
{
    framebuffer.Fill(Color::BLACK);


    framebuffer.DrawImage(imagen, 50,50);
	// ...
	//framebuffer.DrawLineDDA(150, 150, 150 + 100 * cos(time), 150 + 200 * sin(time), Color::CYAN);
	///framebuffer.DrawRect(150, 150, 150 + 100 * cos(time), 150 + 200 * sin(time), Color::BLUE, 5, true , Color::GREEN );


	if (painting) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        drag_x1 = mx;
        drag_y1 = window_height-my-1;

        framebuffer.DrawLineDDA(drag_x0, drag_y0, drag_x1, drag_y1, Color::CYAN);
    }

	//to try image implementation
	//

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
		painting = true;
        drag_x0 = drag_x1 = event.x;
        drag_y0 = drag_y1 = window_height -1-event.y; //flip Y do to behavior problms

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        painting = false;
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
