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
    point *center;
    point *bottom_left;
    point *top_right;
    rgb_color *color;
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

void render(quad *quads, int size) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    printf("Render called: %d\n", sizeof(quads));
    int i;
    for (i = 0; i < size; i++) {
        printf("Render: %d\n", i);
        // loop over the given quads and render each one
        float cen_x = quads[i].center->x;
        float cen_y = quads[i].center->y;
        float left = quads[i].bottom_left->x;
        float bot = quads[i].bottom_left->y;
        float right = quads[i].top_right->x;
        float top = quads[i].top_right->y;
        rgb_color *color = quads[i].color;

        glTranslatef(cen_x, cen_y, 0.0f);
        // draw the quad
        glBegin(GL_QUADS);
            glColor3f(color->r, color->g, color->b);
            glVertex2f(left, top);
            glVertex2f(right, top);
            glVertex2f(right, bot);
            glVertex2f(left, bot);
        glEnd();
    }
    printf("Done rendering\n");

    SDL_GL_SwapBuffers();
}

int translate_quads(quad **quads, PyObject *pyobj) {
    printf("Translating quads\n");
    if (PySequence_Check(pyobj) == 0) {
        // pass 
    }

    int i;
    int num_quads = PySequence_Size(pyobj);
    *quads = malloc(sizeof(quad) * num_quads);
    printf("Memory allocated: %d\n", sizeof(quad)*num_quads);
    printf("Number of quads: %d\n", num_quads);
    for (i = 0; i < num_quads; i++) {
        printf("Parsing quad\n");
        PyObject *py_quad = PySequence_GetItem(pyobj, i);
        PyObject *py_center = PyObject_GetAttrString(py_quad, "center");
        PyObject *py_bottom_left = PyObject_GetAttrString(py_quad, "bottom_left");
        PyObject *py_top_right = PyObject_GetAttrString(py_quad, "top_right");
        PyObject *py_color = PyObject_GetAttrString(py_quad, "color");

        point *center, *bottom_left, *top_right;
        rgb_color *color;

        center = malloc(sizeof(point));
        bottom_left = malloc(sizeof(point));
        top_right = malloc(sizeof(point));
        color = malloc(sizeof(rgb_color));

        float *x1, *y1, *x2, *y2, *x3, *y3, *r, *g, *b;
        PyArg_ParseTuple(py_center, "ff", &(center->x), &(center->y));
        PyArg_ParseTuple(py_bottom_left, "ff", &(bottom_left->x), &(bottom_left->y));
        PyArg_ParseTuple(py_top_right, "ff", &(top_right->x), &(top_right->y));
        PyArg_ParseTuple(py_color, "fff", &(color->r), &(color->g), &(color->b));
        printf("Stuff parsed\n");

        printf("Center: %f %f\n", center->x, center->y);
        quads[i]->center = center;
        quads[i]->bottom_left = bottom_left;
        quads[i]->top_right = top_right;
        quads[i]->color = color;
    }

    printf("quad: %d\n", *quads);
    return num_quads;
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
    /*quad quads[NUM_QUADS];
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
    quads[0].color = color;*/


    // objects for running a python script
    PyObject *script_module, *ret_obj, *mod_func, *args;
    // pointer for array we will dump script results in
    quad *quads;
    int num_quads;

    printf("Initializing Python\n");
    Py_Initialize();
    PySys_SetPath(".");
    printf("Importing modules\n");
    script_module = PyImport_ImportModule("scripts");
    mod_func = PyObject_GetAttrString(script_module, "create_quads");
    // this is where we call the python script
    ret_obj = PyEval_CallObject(mod_func, NULL);
    // translate and store quad objects
    if (ret_obj == NULL) {
        printf("Failed to retrieve quads\n");
        return 1;
    }
    num_quads = translate_quads(&quads, ret_obj);

    // run the main loop
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
        render(quads, num_quads);
    }
    free(quads);
    Py_Finalize();


    return 0;
}
