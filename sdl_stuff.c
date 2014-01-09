#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT  600
#define SCREEN_BPP  32

int initGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL: %s", gluErrorString(error));
        return 1;
    }

    return 0;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();

    SDL_GL_SwapBuffers();
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Failed to init SDL\n");
        return 1;
    }

    if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL) == NULL) {
        printf("Failed to set video mode\n");
        return 1;
    }

    if (initGL() == 1) {
        printf("Failed to init OpenGL\n");
        return 1;
    }

    SDL_WM_SetCaption("Exciting Things", NULL);
   
    int running = 1;
    SDL_Event event;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                //pass
            }
        }
        render();
    }

    return 0;
}
