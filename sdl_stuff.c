#include <Python.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT  600
#define SCREEN_BPP  32

#define NUM_QUADS 1

typedef struct point {
    float x, y;
} point;

typedef struct rgb_color {
    float r,g,b;
} rgb_color;

typedef struct quad {
    point center;
    point bottom_left;
    point top_right;
    rgb_color color;
} quad;


int initGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

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

void render(quad quads[], int size) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    int i;
    for (i = 0; i < size; i++) {
        // loop over the given quads and render each one
        float cen_x = quads[i].center.x;
        float cen_y = quads[i].center.y;
        float left = quads[i].bottom_left.x;
        float bot = quads[i].bottom_left.y;
        float right = quads[i].top_right.x;
        float top = quads[i].top_right.y;
        rgb_color color = quads[i].color;

        glTranslatef(cen_x, cen_y, 0.0f);
        // draw the quad
        glBegin(GL_QUADS);
            glColor3f(color.r, color.g, color.b);
            glVertex2f(left, top);
            glVertex2f(right, top);
            glVertex2f(right, bot);
            glVertex2f(left, bot);
        glEnd();
    }

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

    // set up the quads we will be rendering 
    quad quads[NUM_QUADS];
    int i = 0;
    point q1_center, q1_bl, q1_tr,
          q2_center, q2_bl, q2_tr,
          q3_center, q3_bl, q3_tr;
    rgb_color color;
    q1_center.x = 200; q1_center.y = 200;
    q1_bl.x = -50.0f; q1_bl.y = -50.0f;
    q1_tr.x = 50.0f; q1_tr.y = 50.0f;
    color.r = 1.0f; color.g = 0.0f; color.b = 0.0f;
    quads[0].center = q1_center;
    quads[0].bottom_left = q1_bl;
    quads[0].top_right = q1_tr;
    quads[0].color = color;

    // run the main loop
    int running = 1;
    SDL_Event event;
    Py_Initialize();
    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                //pass
            }
        }
        render(quads, NUM_QUADS);
    }
    Py_Finalize();

    return 0;
}
