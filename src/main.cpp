#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include "game.h"

Game g_game = Game(500);

void display(void) {
    g_game.display();
}

void keyboard(unsigned char key, int x, int y) {
    g_game.keyboard(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y) {
    g_game.keyboardUp(key, x, y);
}

void idle() {
    g_game.idle();
}

void mouse(int button, int state, int x, int y) {
    g_game.mouse(button, state, x, y);
}

void motion(int x, int y) {
    g_game.motion(x, y);
}

void init(void) {
    /* selecionar cor de fundo (azul rgb(158 158 255)) */
    glClearColor(0.620f, 0.620f, 1.0f, 1.0f);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, g_game.windowSideLength(), 0.0, g_game.windowSideLength(), -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_game.windowSideLength(), g_game.windowSideLength());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    if (argc < 2) {
        printf("Usage: %s <xml_path>\n", argv[0]);
        return 1;
    }

    if (!g_game.loadArena(argv[1])) {
        printf("Failed to load arena\n");
        return 1;
    }

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);

    glutMainLoop();

    return 0;
}
