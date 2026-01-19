/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include <vector>


//necessitem un structure enum pels diferents modes del menu
//tindrem diferents bottons
enum ButtonType {


    //
    //
    BUTTON_PENCIL,
    BUTTON_LINE,
    BUTTON_RECTANGLE,
    BUTTON_TRIANGLE,
    BUTTON_ERASER,

    //
    //
    BUTTON_CLEAR,
    BUTTON_LOAD,
    BUTTON_SAVE,

    //colors disponibles segons la
    // imatge del LAB1
    BUTTON_COLOR_BLACK,
    BUTTON_COLOR_WHITE,
    BUTTON_COLOR_PINK, //pink colour was missing
    BUTTON_COLOR_YELLOW,
    BUTTON_COLOR_RED,
    BUTTON_COLOR_BLUE,
    BUTTON_COLOR_CYAN,
    BUTTON_COLOR_GREEN
};

class Button {
    public://class necessitem que sigui publica
    //we need butttons for the menu
    //first load the image porperly of each button
    Image image;
    Vector2 position;
    ButtonType buttonMenu;


    Button(){} //constr

    //button constructor with path a la imatge
    //posicio del button al frame
    //button type mitjancant Enum per saber el tipus
    Button(const char* imagePath, int x, int y, ButtonType t) {
        //we fullfill each attribute
        image.LoadPNG(imagePath);
        position.set(x, y);
        buttonMenu = t; //from attributes
    }

    //checkpooint de si el mouse esta dins del button
    //per comprovar si el mouse esta hovering la imatge del ButtonMenu
    bool IsMouseInside(const Vector2& mousePos) {
        return
        //hovering mentres el mouse estigui sobre el button
            mousePos.x >= position.x &&
            mousePos.x < position.x + image.width &&
            mousePos.y >= position.y &&
            mousePos.y < position.y + image.height;
    }

    //dibuixa el button al framebuffer
    void Render(Image& framebuffer) {
        framebuffer.DrawImage(image, (int)position.x, (int)position.y);
    }

};

class Application
{
public:

    //to try primitives
    //bool painting = false;
    //Vector2 startPoint;
    //Vector2 secondPoint;  // Para triángulos (necesitas 3 puntos)


    //elements que necessitem per implementar
    //el Menu del nostre paintlike
    std::vector<Button> Menu; //list of buttons
    ButtonType currentTool;
    Color currentColor;
    int triangleClickCount = 0;
    int menuHeight;
    bool painting = false;

    //drawn points needed
    Vector2 startPoint;
    Vector2 secondPoint;

    //user painting screen>>
    Image Paint;


	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
		//apliquem l'event de modificar la pantalla
		// si el usuari modifica el seu tamany tambe
		//per la part interactiva del Paint-Like
		this->Paint.Resize(width,height);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
