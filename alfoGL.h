#ifndef ALFOGL_H
#define ALFOGL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
#include <map>
#include <string>
#include "structs.h"

class alfoGL
{
    public:
        alfoGL(SDL_Surface* window);
        ~alfoGL();

        // Config
        bool m_aa_enabled;
        bool m_show_axis;
        bool m_show_debug;
        bool m_show_z;
        bool m_show_help;
        bool m_show_mode;
        bool m_proj_orth;

        void setPerspective(double angle, double ratio, double near, double far);
        void lookAt(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ);

        void rotateX(double angle);
        void rotateY(double angle);
        t_point* addPoint(double x, double y, double z);
        t_line* addLine(double x1, double y1, double z1, double x2, double y2, double z2, Uint8 R, Uint8 G, Uint8 B);
        void applyMatrix(t_matrix4 m);

        t_point getProjected (t_point obj);
        void show();
        bool getAAEnabled();
        void setAAEnabled(bool mode);
        void setShowAxis(bool mode);
        void setShowDebug(bool mode);
        void setLastFps(int fps);
        void setShownMode(std::string mode);

        void setPixel(int x, int y, Uint32 color);
        void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
        void getPixelColor(int x, int y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
        void drawLine(int x1, int y1, int x2, int y2, Uint32 color, int dotted = 0);
        void drawLineAA(int x1, int y1, int x2, int y2, Uint32 color, int dotted = 0);

        // Getters
        SDL_Surface* getWindow() { return m_window; }
        t_matrix4 getProjectionMatrix() { return m_proj_matrix; }
        t_matrix4 getViewMatrix() { return m_view_matrix; }

        // Setters
        void setWindow(SDL_Surface *window) { m_window = window; }

    protected:

    private:
        SDL_Surface *m_window;
        TTF_Font *m_font;
        std::string m_mode;

        int m_last_fps;
        int m_drawn_pixels;

        // Scene
        std::vector<t_point> m_scene_points;
        std::vector<t_line> m_scene_lines;

        // View
        t_matrix4 m_view_matrix;

        // Projection
        double m_near;
        double m_far;
        t_matrix4 m_proj_matrix;
        t_matrix4 m_proj_matrix_orth;
};

#endif // ALFOGL_H
