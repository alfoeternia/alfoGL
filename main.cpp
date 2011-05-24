/**
 * alfoGL
 * ------------------
 * 3D Graphic Library
 *
 * @author alfo
 * @version 1.0
 */

#define cinfo cout << "INFO: "
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "main.h"
#include "alfoGL.h"
#include "structs.h"

using namespace std;

void displayMatrix (t_matrix4 matrix)
{
    cout.precision(3);
    cout << fixed;

    for(int i = 0; i < 4; i++)
    {
        cinfo << "|";
        for(int j = 0; j < 4; j++)
        {
            if (matrix[4*i+j] >= 0.0) cout << " ";
            cout << matrix[4*i+j] << " ";
        }
        cout << "|" << endl;
    }
}

int main (int argc, char **argv)
{
    // Reopen outputs
    freopen("CON", "w", stderr);
    freopen("CON", "w", stdout);

    cout << "==============================================================================" << endl;
    cout << "|                             alfo GL :: Demo                                |" << endl;
    cout << "==============================================================================" << endl << endl;
    cinfo << "Loading 3D Engine ..." << endl;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        cout << "FATAL: Unable to init SDL." << endl;
        cout << "FATAL: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    cinfo << "SDL loaded successfully." << endl;

    atexit(SDL_Quit);

    if(TTF_Init() == -1)
    {
        cout << "FATAL: Unable to init SDL_ttf." << endl;
        cout << "FATAL: " <<  TTF_GetError() << endl;
        return EXIT_FAILURE;
    }
    cinfo << "SDL_ttf loaded successfully." << endl;

    // Create a new Window
    SDL_WM_SetCaption("alfoGL Demo", NULL);
    SDL_Surface* screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    //SDL_Surface* screen = SDL_SetVideoMode(1680, 1050, 32, SDL_FULLSCREEN|SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (!screen)
    {
        cout << "FATAL: Unable to set video." << endl;
        cout << "FATAL: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    cinfo << "SDL Window created successfully." << endl;

    // Init 3D renderer
    alfoGL *gl = new alfoGL(screen);
    int a = 0;
    int b = 0;
    gl->lookAt(10,-10,10,0,0,0,0,0,1);
    //gl->lookAt(0,34,0,0,0,0,0,0,-1);
    gl->setAAEnabled(true);
    gl->setShowAxis(true);
    gl->setShowDebug(true);
    gl->setPerspective(55, (double) screen->w/screen->h, 1, 5);

    //gl->setPerspective(55, 1, 1, 5);
    cinfo << "3D renderer initialized." << endl;

    cinfo << "=== View matrix ========================================================" << endl;
    t_matrix4 view = gl->getViewMatrix();
    displayMatrix(view);
    cinfo << "========================================================================" << endl;

    cinfo << "=== Projection matrix ==================================================" << endl;
    t_matrix4 proj = gl->getProjectionMatrix();
    displayMatrix(proj);
    cinfo << "========================================================================" << endl;

    t_point target;
    t_point position = {10, -10, 10, 0};
    double _phi = 0, _theta = 0;
    displayMatrix(view*proj);

    // E
    /*gl->addLine(0,0,-3, 0,5,-3, 50,50,50);
    gl->addLine(1,1,-3, 1,2,-3, 50,50,50);
    gl->addLine(1,3,-3, 1,4,-3, 50,50,50);
    gl->addLine(2,2,-3, 2,3,-3, 50,50,50);
    gl->addLine(4,0,-3, 4,1,-3, 50,50,50);
    gl->addLine(4,4,-3, 4,5,-3, 50,50,50);
    gl->addLine(0,0,-3, 4,0,-3, 50,50,50);
    gl->addLine(1,1,-3, 4,1,-3, 50,50,50);
    gl->addLine(1,2,-3, 2,2,-3, 50,50,50);
    gl->addLine(1,3,-3, 2,3,-3, 50,50,50);
    gl->addLine(1,4,-3, 4,4,-3, 50,50,50);
    gl->addLine(0,5,-3, 4,5,-3, 50,50,50);

    // F
    gl->addLine(5,0,-3, 5,5,-3, 50,50,50);
    gl->addLine(6,1,-3, 6,2,-3, 50,50,50);
    gl->addLine(6,3,-3, 6,4,-3, 50,50,50);
    gl->addLine(7,2,-3, 7,3,-3, 50,50,50);
    gl->addLine(9,4,-3, 9,5,-3, 50,50,50);
    gl->addLine(5,0,-3, 6,0,-3, 50,50,50);
    gl->addLine(6,1,-3, 6,0,-3, 50,50,50);
    gl->addLine(6,2,-3, 7,2,-3, 50,50,50);
    gl->addLine(6,3,-3, 7,3,-3, 50,50,50);
    gl->addLine(6,4,-3, 9,4,-3, 50,50,50);
    gl->addLine(5,5,-3, 9,5,-3, 50,50,50);

    // R
    gl->addLine(10,0,-3, 10,5,-3, 50,50,50);
    gl->addLine(11,0,-3, 11,2,-3, 50,50,50);
    gl->addLine(11,3,-3, 11,4,-3, 50,50,50);
    gl->addLine(13,0,-3, 13,4,-3, 50,50,50);
    gl->addLine(14,0,-3, 14,2,-3, 50,50,50);
    gl->addLine(14,3,-3, 14,5,-3, 50,50,50);
    gl->addLine(10,0,-3, 11,0,-3, 50,50,50);
    gl->addLine(13,0,-3, 14,0,-3, 50,50,50);
    gl->addLine(11,2,-3, 14,2,-3, 50,50,50);
    gl->addLine(11,3,-3, 14,3,-3, 50,50,50);
    gl->addLine(11,4,-3, 13,4,-3, 50,50,50);
    gl->addLine(10,5,-3, 14,5,-3, 50,50,50);

    // E
    gl->addLine(15,0,-3, 15,5,-3, 50,50,50);
    gl->addLine(16,1,-3, 16,2,-3, 50,50,50);
    gl->addLine(16,3,-3, 16,4,-3, 50,50,50);
    gl->addLine(17,2,-3, 17,3,-3, 50,50,50);
    gl->addLine(19,0,-3, 19,1,-3, 50,50,50);
    gl->addLine(19,4,-3, 19,5,-3, 50,50,50);
    gl->addLine(15,0,-3, 19,0,-3, 50,50,50);
    gl->addLine(16,1,-3, 19,1,-3, 50,50,50);
    gl->addLine(16,2,-3, 17,2,-3, 50,50,50);
    gl->addLine(16,3,-3, 17,3,-3, 50,50,50);
    gl->addLine(16,4,-3, 19,4,-3, 50,50,50);
    gl->addLine(15,5,-3, 19,5,-3, 50,50,50);

    // I
    gl->addLine(20,0,-3, 21,0,-3, 50,50,50);
    gl->addLine(20,5,-3, 21,5,-3, 50,50,50);
    gl->addLine(20,0,-3, 20,5,-3, 50,50,50);
    gl->addLine(21,0,-3, 21,5,-3, 50,50,50);*/

    /**
     * Maison
     */

    // Socle
    gl->addLine(0,0,0, 4,0,0, 200,200,200);
    gl->addLine(0,0,0, 0,3,0, 200,200,200);
    gl->addLine(4,0,0, 4,1,0, 200,200,200);
    gl->addLine(4,2,0, 4,3,0, 200,200,200);
    gl->addLine(0,3,0, 4,3,0, 200,200,200);

    // Murs
    gl->addLine(0,0,0, 0,0,3, 200,200,200);
    gl->addLine(0,3,0, 0,3,3, 200,200,200);
    gl->addLine(4,0,0, 4,0,3, 200,200,200);
    gl->addLine(4,3,0, 4,3,3, 200,200,200);

    // Porte
    gl->addLine(4,1,0, 4,1,2, 200,200,200);
    gl->addLine(4,2,0, 4,2,2, 200,200,200);
    gl->addLine(4,1,2, 4,2,2, 200,200,200);

    // Toit
    gl->addLine(0,0,3, 4,0,3, 200,200,200);
    gl->addLine(0,0,3, 0,3,3, 200,200,200);
    gl->addLine(4,0,3, 4,3,3, 200,200,200);
    gl->addLine(0,3,3, 4,3,3, 200,200,200);

    gl->addLine(0,0,3, 0,1.5,4.5, 200,200,200);
    gl->addLine(0,3,3, 0,1.5,4.5, 200,200,200);
    gl->addLine(4,0,3, 4,1.5,4.5, 200,200,200);
    gl->addLine(4,3,3, 4,1.5,4.5, 200,200,200);

    gl->addLine(0,1.5,4.5, 4,1.5,4.5, 200,200,200);

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/3, SDL_DEFAULT_REPEAT_INTERVAL);
    // Main Loop
    int FPS = 0;
    int pastFPS = 0;
    int past = 0;
    t_matrix4 matrix;
    int mode = MODE_NONE;
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    done = true;
                    exit;
                    break;
                case SDLK_F1:
                    gl->m_show_help = !gl->m_show_help;
                    break;
                case SDLK_F2:
                    gl->setAAEnabled(!gl->getAAEnabled());
                    break;
                case SDLK_F3:
                    gl->m_show_debug = !gl->m_show_debug;
                    break;
                case SDLK_F4:
                    gl->m_show_z = !gl->m_show_z;
                    break;
                case SDLK_F5:
                    gl->m_show_axis = !gl->m_show_axis;
                    break;
                case SDLK_F6:
                    gl->m_proj_orth = !gl->m_proj_orth;
                    break;

                case SDLK_w:
                    position.x -= 0.5;
                    target.x -= 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;
                case SDLK_s:
                    position.x += 0.5;
                    target.x += 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;
                case SDLK_a:
                    position.y -= 0.5;
                    target.y -= 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;
                case SDLK_d:
                    position.y += 0.5;
                    target.y += 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;
                case SDLK_PAGEUP:
                    position.z += 0.5;
                    target.z += 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;
                case SDLK_PAGEDOWN:
                    position.z -= 0.5;
                    target.z -= 0.5;
                    gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                    break;

                // Set Mode
                case SDLK_r:
                    if(mode == ROTATION_X) mode = ROTATION_Y;
                    else if (mode == ROTATION_Y) mode = ROTATION_Z;
                    else mode = ROTATION_X;
                    break;
                case SDLK_t:
                    if(mode == TRANSLATION_X) mode = TRANSLATION_Y;
                    else if (mode == TRANSLATION_Y) mode = TRANSLATION_Z;
                    else mode = TRANSLATION_X;
                    break;
                case SDLK_v:
                    if(mode == SYMMETRY_O) mode = SYMMETRY_X;
                    else if (mode == SYMMETRY_X) mode = SYMMETRY_Y;
                    else if (mode == SYMMETRY_Y) mode = SYMMETRY_Z;
                    else mode = SYMMETRY_O;
                    break;
                case SDLK_h:
                    mode = HOMOTHETY;
                    break;
                case SDLK_c:
                    mode = SHEAR;
                    break;

                // Plus or Minus
                case SDLK_KP_PLUS:
                    switch(mode)
                    {
                        // Rotations
                        case ROTATION_X:
                            matrix.m = {1, 0,            0,           0,
                                        0, cos(M_PI/12), -sin(M_PI/12), 0,
                                        0, sin(M_PI/12),  cos(M_PI/12), 0,
                                        0, 0,            0,           1};
                            break;
                        case ROTATION_Y:
                            matrix.m = {cos(M_PI/12), 0, sin(M_PI/12), 0,
                                        0, 1, 0, 0,
                                        -sin(M_PI/12), 0, cos(M_PI/12), 0,
                                        0, 0, 0, 1};
                            break;
                        case ROTATION_Z:
                            matrix.m = {cos(M_PI/12), -sin(M_PI/12), 0, 0,
                                        sin(M_PI/12), cos(M_PI/12), 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;

                        // Translations
                        case TRANSLATION_X:
                            matrix.m = {1, 0, 0, 0.5,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;
                        case TRANSLATION_Y:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, 0.5,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;
                        case TRANSLATION_Z:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0.5,
                                        0, 0, 0, 1};
                            break;

                        // Symmetries
                        case SYMMETRY_O:
                            matrix.m = {-1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_X:
                            matrix.m = {1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_Y:
                            matrix.m = {-1, 0, 0, 0,
                                        0,  1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_Z:
                            matrix.m = {-1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0,  1, 0,
                                        0, 0, 0, 1};
                            break;

                        // Homothety
                        case HOMOTHETY:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1./1.1};
                            break;

                        // Shear
                        case SHEAR:
                            matrix.m = {1, 0, 0, 0,
                                        1, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;

                        default:
                            matrix.m = {1,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
                    }
                    gl->applyMatrix(matrix);
                    break;

                case SDLK_KP_MINUS:
                    switch(mode)
                    {
                        // Rotations
                        case ROTATION_X:
                            matrix.m = {1, 0,             0,            0,
                                        0,  cos(M_PI/12), sin(M_PI/12), 0,
                                        0, -sin(M_PI/12), cos(M_PI/12), 0,
                                        0,  0,            0,            1};
                            break;
                        case ROTATION_Y:
                            matrix.m = {cos(M_PI/12), 0, -sin(M_PI/12), 0,
                                        0, 1, 0, 0,
                                        sin(M_PI/12), 0, cos(M_PI/12), 0,
                                        0, 0, 0, 1};
                            break;
                        case ROTATION_Z:
                            matrix.m = {cos(M_PI/12), sin(M_PI/12), 0, 0,
                                        -sin(M_PI/12), cos(M_PI/12), 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;

                        // Translations
                        case TRANSLATION_X:
                            matrix.m = {1, 0, 0, -0.5,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;
                        case TRANSLATION_Y:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, -0.5,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;
                        case TRANSLATION_Z:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, -0.5,
                                        0, 0, 0, 1};
                            break;

                        // Symmetries
                        case SYMMETRY_O:
                            matrix.m = {-1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_X:
                            matrix.m = {1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_Y:
                            matrix.m = {-1, 0, 0, 0,
                                        0,  1, 0, 0,
                                        0, 0, -1, 0,
                                        0, 0, 0, 1};
                            break;
                        case SYMMETRY_Z:
                            matrix.m = {-1, 0, 0, 0,
                                        0, -1, 0, 0,
                                        0, 0,  1, 0,
                                        0, 0, 0, 1};

                        // Homothety
                        case HOMOTHETY:
                            matrix.m = {1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1.1};
                            break;

                        // Shear
                        case SHEAR:
                            matrix.m = {1, 0, 0, 0,
                                        -1, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1};
                            break;

                        default:
                            matrix.m = {1,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
                    }
                    gl->applyMatrix(matrix);
                    break;
                }
                break;
            }

            case SDL_MOUSEMOTION:
            {
                //cinfo << "Mouse moved by " << event.motion.xrel << "," << event.motion.yrel << " to (" << event.motion.x << "," << event.motion.y << ")." << endl;

                _theta -= event.motion.xrel*0.5;
                _phi -= event.motion.yrel*0.5;

                t_point _forward = {0, 0, 0, 0};

                if (_phi > 89)
                    _phi = 89;
                else if (_phi < -89)
                    _phi = -89;
                //passage des coordonnées sphériques aux coordonnées cartésiennes
                double r_temp = cos(_phi*M_PI/180);
                _forward.z = sin(_phi*M_PI/180);
                _forward.x = r_temp*cos(_theta*M_PI/180);
                _forward.y = r_temp*sin(_theta*M_PI/180);

                //avec la position de la caméra et la direction du regard, on calcule facilement ce que regarde la caméra (la cible)

                target.x = position.x + _forward.x;
                target.y = position.y + _forward.y;
                target.z = position.z + _forward.z;

                gl->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,0,0,1);
                break;
            }
            }
        }

        int currentTime = SDL_GetTicks();

        if ( currentTime - past >= 16 )
        {
            // Display
            gl->setShownMode(getStringFromMode(mode));
            gl->show();
            FPS++;
        }

        if ( currentTime - pastFPS >= 1000 )
        {
            static char buffer[30] = {0};
            sprintf(buffer, "alfoGL Demo :: %d FPS", FPS);
            gl->setLastFps(FPS);
            SDL_WM_SetCaption(buffer, 0);

            FPS = 0;
            pastFPS = currentTime;
        }
    }

    // All is well ;)
    cout << "Good Bye!" << endl;
    SDL_Quit();
    return EXIT_SUCCESS;
}

string getStringFromMode(int mode)
{
    switch(mode)
    {
        case ROTATION_X:
            return "Mode: Rotation around X-axis.";
            break;
        case ROTATION_Y:
            return "Mode: Rotation around Y-axis.";
            break;
        case ROTATION_Z:
            return "Mode: Rotation around Z-axis.";
            break;

        case TRANSLATION_X:
            return "Mode: Translation on the X-axis.";
            break;
        case TRANSLATION_Y:
            return "Mode: Translation on the Y-axis.";
            break;
        case TRANSLATION_Z:
            return "Mode: Translation on the Z-axis.";
            break;

        case SYMMETRY_O:
            return "Mode: Symmetry by O point.";
            break;
        case SYMMETRY_X:
            return "Mode: Symmetry by X-axis";
            break;
        case SYMMETRY_Y:
            return "Mode: Symmetry by Y-axis.";
            break;
        case SYMMETRY_Z:
            return "Mode: Symmetry by Z-axis.";
            break;

        case HOMOTHETY:
            return "Mode: Homothety.";
            break;

        case SHEAR:
            return "Mode: Shear.";
            break;

        default:
            return "Mode: none.";
    }
}
