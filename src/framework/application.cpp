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
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
}

//to compile cmake --build . -j"$(nproc)"
//to execute ./ComputerGraphics


// Render one frame
void Application::Render(void)
{
    framebuffer.Fill(Color::BLACK);
	// ...
	framebuffer.DrawLineDDA(150, 150, 150 + 100 * cos(time), 150 + 200 * sin(time), Color::CYAN);
	framebuffer.DrawRect(150, 150, 150 + 100 * cos(time), 150 + 200 * sin(time), Color::BLUE, 5, true , Color::GREEN );


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

void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

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
