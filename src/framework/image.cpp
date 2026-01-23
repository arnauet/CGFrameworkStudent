#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width; //wiwdth its x0 and x1
	this->height = height; //height its y0 and y1
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete[] pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels)
		delete[] pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height)
				result.SetPixelUnsafe( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

//Drawing lines by DDA method Primitive 2.1.2

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c)
{
    //cordenades pel DrawLineDDA
    int dx = x1-x0;
    int dy = y1-y0;
    int screen_steps = std::max(std::abs(dx), std::abs(dy));

    //cas unic de un solo punto
    if (screen_steps == 0) {
            SetPixel(x0, y0, c);
            return;
    }

    //computing direction step vectors to advance each cordinate
    float stepX = dx / (float)screen_steps; //normalitzacio del setpX respct el eix X
	float stepY = dy / (float)screen_steps; //normalitzacio del setpY respct el eix Y

	//coordenades d'on comenca la linea
	float x = x0;
	float y = y0;

	//drawiwng line
	for (int i = 0; i <= screen_steps; i++) {
	    SetPixel((int)std::round(x), (int)std::round(y), c); //fiquem color
	    x += stepX; // Increment x + step
		y += stepY; // Increment y + step
	}
}

//Draw rectangle method impleentation Primitive 2.1.2

void Image::DrawRect(int x, int y, int w, int h, const Color& borderColor,
    int borderWidth, bool isFilled, const Color& fillColor){

        if (w < 0) {
            x = x + w;
            w = -w;
        }
        if (h < 0) {
            y = y + h;
            h = -h;
        }

        if(isFilled){
            //si el interior esta coloreejat pintemm
            //faltava la y en la condicio del loop
            for(int j = y + borderWidth; j < y + h - borderWidth; j++){

                //slide 6: i= y*w+x memory acces using 2D cordinates
                for(int i = x + borderWidth; i< x +w - borderWidth; i++){

                    //pintem el pixel a la posicio que toc
                    SetPixel(i,j,fillColor);
                }
            }
        }

        //borderWidth color implementation
        for(int k=0; k< borderWidth; k++){
            //iterem segons la quantitat de pixel de 'border'
            //k es com una capa del gruixor del borde
            //part deel rectangle horitzontal -> y fixed
            //y constant amb x canviant -> linea horitzontal
            for(int i=x; i<x+w; i++){
                //iterem sobre x+w
                SetPixel(i, y+k, borderColor); // lines superiors
                //y+h-1-k la ultima linea d'abaix
                SetPixel(i, y+h-1-k,borderColor);
            }
            //part del rectangle vertical -> x fixed
            //x constant amb y canviat -> linea vertical
            for(int j=y; j<y+h; j++){
                //iterem sobre y+h
                SetPixel(x + k, j, borderColor);
                SetPixel(x + w - 1 - k, j, borderColor);
            }
        }
}

//dibuixar cercle amb algoritme midpoint
void Image::DrawCircle(int x0, int y0, int r, const Color& borderColor,
    int borderWidth, bool isFilled, const Color& fillColor) {

    if (r <= 0) return;

    //omplir el cercle si cal
    if (isFilled) {
        for (int y = -r; y <= r; y++) {
            int dx = (int)sqrt(r * r - y * y);
            for (int x = -dx; x <= dx; x++) {
                SetPixel(x0 + x, y0 + y, fillColor);
            }
        }
    }

    //dibuixar el contorn amb midpoint algorithm
    for (int i = 0; i < borderWidth; i++) {
        int rad = r - i;
        int x = 0;
        int y = rad;
        int d = 1 - rad;

        while (x <= y) {
            //dibuixem els 8 punts simetrics del cercle
            SetPixel(x0 + x, y0 + y, borderColor);
            SetPixel(x0 - x, y0 + y, borderColor);
            SetPixel(x0 + x, y0 - y, borderColor);
            SetPixel(x0 - x, y0 - y, borderColor);
            SetPixel(x0 + y, y0 + x, borderColor);
            SetPixel(x0 - y, y0 + x, borderColor);
            SetPixel(x0 + y, y0 - x, borderColor);
            SetPixel(x0 - y, y0 - x, borderColor);

            //actualitzacio midpoint
            if (d < 0) {
                d += 2 * x + 3;
            } else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
        }
    }
}

//Draw Triangle ratertization method implementation 2.1.3

// 1) We first need the implementation of ScanLinesDDA

void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table, int minY)
{
    //garantitza que sempre iterem cap abaix en la pantalla
    if (y0 > y1){
        //direccio cap abaix del processament del triangle
        // scanline omple la taula del triangle up down order
        ScanLineDDA(x1, y1, x0, y0, table, minY);  // punts intercambiats
        return;
    }

    //calculem el pendent (sobre y)
    int dy = y1-y0; //distance
    int dx = x1-x0;
    float screen_step = 0;

    // avoid zero division errors
    if (dy != 0){
        //calculem el pas a desplacarse per la pantalla
            screen_step = dx / (float)dy;
        }

    float x = (float)x0; //necessitem x per anar sumar el pendent en cada iteracio

    //fila per fila on esta el pixel extrem del triangle
    for (int y = y0; y <= y1; y++) {
        //index relatiu a la Taula!!! Sino sortim dels limits del triangle
        int idx = y - minY;
        if (idx >= 0 && idx < (int)table.size()) {
            int ix = (int)std::round(x);//per simplificar sintaxis
            //necessitem floats pels calculs decimals del pendent
            //pero quan necessitem el pixel real final fem recast (int)x
            table[idx].minX = std::min(table[idx].minX, ix);
            table[idx].maxX = std::max(table[idx].maxX, ix);

        }
        //per cada y estem moventnos horitzontalment una quantitat fixe
        x += screen_step;
    }
}


void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor,
        bool isFilled, const Color& fillColor)
{
    //trobem el punt mes i el mes alt
    //variables que defineixen el 'bounding box'
    int minY = std::min({ (int)p0.y, (int)p1.y, (int)p2.y });
	int maxY = std::max({ (int)p0.y, (int)p1.y, (int)p2.y });

	//per no ocupar la menys memoria posible
	//nomes les files que ocupa el triangle (relatiu y-minY)
	std::vector<Cell> table(maxY - minY + 1);

	// IMPORTANT: Inicialitzar cada Cell!
	//per evitar errors de comportaments estranys
	// amb la visualitzacio de triangles.
	for (int i = 0; i < table.size(); i++) {
	    //table[i].minX = 0;
	    //table[i].maxX = 0;
		//per no fer un #include extra
		table[i].minX = 2147483647;  //INTMAX de climits
	    table[i].maxX = -2147483648; //INTMAX
	}

	//una cell per cada fila per la taula AET
	//cada aresta registrara les coordenades X la taula
	//per cada trucada tindra el minim i maxim sobre X
	//la taulaa guardara la delimitacio per cada fila
	//limit esquerre i limit dret sobre l'eix horitzontal X
	ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table, minY); //p0-p1 edge per la taula
	ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table, minY); //p1-p2 edge..
	ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table, minY); //p2-p0 edge..

	//si sha de pintar el triangle>>>
	if (isFilled){

	    //iterem pel tamany de la taula creada
	    for (int y = 0; y < table.size(); ++y) {

			//coordenades totals de la pantalla
			int scanY = minY + y;
			int minX = table[y].minX; //limiits horitzontal left
			int maxX = table[y].maxX; //limiits horitzontal right

			if (minX <= maxX) { //No surtirse fora del rang
				for (int x = minX; x <= maxX; ++x) {
					SetPixel(x, scanY, fillColor); //pintem interior del triangle
				}
			}
		}
	}
	//Es dibuixa el contorn delimitat del triangle
	DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor); //dibuixa la aresta p0-p1
	DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor); //dibuixa la aresta p1-p2
	DrawLineDDA(p2.x, p2.y, p0.x, p0.y, borderColor); //dibuixa la aresta p2-p0

}

//We need to implement a method for drawwing an image
//explicat al enunciat del Lab 2.2
void Image::DrawImage(const Image& img, int x, int y){
    //implement draw image>>>
    //es podria hardcodejar perque seran simbols de 32 pixel (al LAB1)
    //ho fem ampliable a qualsevol imatge iterant sobre widht
    for(int j = 0; j<(int)img.height; j++){
        //iterem sobre height
        for(int i = 0; i<(int)img.width; i++){
            //calculem on va el pixel en les coredenades finals de la pantalla
            // seguint la formula dels slides sobre el framebuffer
            //int bigX = x + i;
            //int bigY = y + j;
            //dibuixem el pixel
            SetPixel(x + i, y + j, img.GetPixel(i, j));
        }
    }
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size){
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		return false;
	}

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0){
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		return false;
	}

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);

	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		if (pixels) delete[] pixels;
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {
		if (pixels) delete[] pixels;

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	std::cout << "+++ File loaded: " << sfullPath.c_str() << std::endl;

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "--- File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;

	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];

	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}

	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;

	tgainfo->data = new unsigned char[imageSize];

	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;

		if (tgainfo->data != NULL)
			delete[] tgainfo->data;

		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete[] pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixelUnsafe(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete[] tgainfo->data;
	delete tgainfo;

	std::cout << "+++ File loaded: " << sfullPath.c_str() << std::endl;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		std::cerr << "--- Failed to save file: " << fullPath.c_str() << std::endl;
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	delete[] bytes;

	std::cout << "+++ File saved: " << fullPath.c_str() << std::endl;

	return true;
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete[] pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete[] pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
