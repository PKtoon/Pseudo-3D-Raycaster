#ifndef SDL2_WRAP_H
#define SDL2_WRAP_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace SDL2Wrap
{
    class SDL
    {
        int error;  
    public:
        SDL(uint32_t SDL_flags,uint32_t IMG_flags)
        {
            error = SDL_Init(SDL_flags);
            IMG_Init(IMG_flags);
        }
    
        ~SDL()
        {
            IMG_Quit();
            SDL_Quit();
        }
    };

    class Window
    {
        SDL_Window* window = nullptr;
    public:
        Window(std::string name, int x, int y, int w, int h, uint32_t flags)
        {
            window = SDL_CreateWindow(name.c_str(), x, y, w, h, flags);
        }
        ~Window()
        {
            SDL_DestroyWindow(window);
        }
        SDL_Window* ptr() { return window; }
    };
    
    class Renderer
    {
        SDL_Renderer* renderer = nullptr;
    public:
        Renderer(Window& window, int index, uint32_t flags)
        {
            renderer = SDL_CreateRenderer(window.ptr(), index, flags);
        }
        ~Renderer()
        {
            SDL_DestroyRenderer(renderer);
        }
        
        SDL_Renderer* ptr() { return renderer; }
        int clear()
        {
            return SDL_RenderClear(renderer);
        }
        void present()
        {
            SDL_RenderPresent(renderer);
        }
        int clearNpresent()
        {
            int i = clear();
            present();
            return i;
        }
        void colour(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0)
        {
            SDL_SetRenderDrawColor(renderer,r,g,b,a);
        }
        void colour(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
        {
            SDL_GetRenderDrawColor(renderer,r,g,b,a);
        }
    };
    
    class Texture
    {
        SDL_Texture* texture = nullptr;
        Renderer& _renderer;
    public:
        Texture(Renderer& renderer, uint32_t format, int access, int w, int h) : _renderer{renderer}
        {
            SDL_CreateTexture(renderer.ptr(),format,access,w,h);
        }
        Texture(Renderer& renderer, std::string fileName) : _renderer{renderer}
        {
            load(fileName);
        }
        ~Texture()
        {
            SDL_DestroyTexture(texture);
        }
        SDL_Texture* ptr() { return texture; }
        void load(std::string fileName)
        {
            texture = IMG_LoadTexture(_renderer.ptr(),fileName.c_str());
        }
        void render(int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh)
        {
            SDL_Rect src;
            src.x = sx;
            src.y = sy;
            src.w = sw;
            src.h = sh;
            
            SDL_Rect dst;
            dst.x = dx;
            dst.y = dy;
            dst.w = dw;
            dst.h = dh;
            
            SDL_RenderCopy(_renderer.ptr(), texture, &src, &dst);
        }
            
        void render(int x, int y, int w, int h)
        {
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            dst.w = w;
            dst.h = h;
            SDL_RenderCopy(_renderer.ptr(), texture, nullptr, &dst);
        }
        void render(int x, int y)
        {
            int w,h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            render(x, y, w, h);
        }
    };
    
    class Pencil
    {
        Renderer& renderer;
    public:
        Pencil(Renderer& ren) : renderer{ren}{}
    
        int drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            uint8_t r1,g1,b1,a1;
            renderer.colour(&r1,&g1,&b1,&a1);
            renderer.colour(r,g,b,a);
            int i = SDL_RenderDrawLine(renderer.ptr(),x1,y1,x2,y2);
            renderer.colour(r1,g1,b1,a1);
            return i;
        }
        int drawLine(int x1, int y1, int x2, int y2)
        {
            return SDL_RenderDrawLine(renderer.ptr(),x1,y1,x2,y2);
        }
        void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            uint8_t r1,g1,b1,a1;
            renderer.colour(&r1,&g1,&b1,&a1);
            renderer.colour(r,g,b,a);
            drawLine(x1,y1,x2,y2);
            drawLine(x1,y1,x3,y3);
            drawLine(x3,y3,x2,y2);
            renderer.colour(r1,g1,b1,a1);
        }
        void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
        {
            drawLine(x1,y1,x2,y2);
            drawLine(x1,y1,x3,y3);
            drawLine(x3,y3,x2,y2);
        }
    };
}
#endif // SDL2_WRAP_H
