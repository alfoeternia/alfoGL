#include <iostream>
#include <cmath>
#include <ctime>
#include "alfoGL.h"

using namespace std;

alfoGL::alfoGL(SDL_Surface* window) : m_window(window)
{
    // Constructor
    m_aa_enabled = false;
    m_show_axis = false;
    m_show_z = false;
    m_show_help = true;
    m_show_mode = true;
    m_proj_orth = false;
    m_font = TTF_OpenFont("font.ttf", 9);
    m_last_fps = 0;
    m_drawn_pixels = 0;
    m_mode = "";
}

void alfoGL::setPerspective(double angle, double ratio, double near, double far)
{
    t_matrix4 projection = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    double f = 1 / tan((angle / 2) * M_PI / 180);

    projection.m[0] = f / ratio;
    projection.m[5] = f;

    projection.m[10] = (near + far) / (near - far);
    projection.m[11] = (2 * near * far) / (near - far);

    projection.m[14] = -1;

    t_matrix4 projectionOrth = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    projectionOrth.m[0] = 0.20;
    projectionOrth.m[5] = 0.20;
    projectionOrth.m[10] = -0.22;
    projectionOrth.m[11] = -1.22;
    projectionOrth.m[15] = 1;

    m_proj_matrix = projection;
    m_proj_matrix_orth = projectionOrth;
    m_near = near;
    m_far = far;

}

void alfoGL::lookAt(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ)
{
    // Creating Vertexs
    t_point axe = {upX, upY, upZ, 0};
    t_point regard = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ, 0};
    t_point normal = {0, 0, 0, 0};
    t_point newAxe = {0, 0, 0, 0};

    // Creating Matrix
    t_matrix4 matrice = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    // On effectue les deux produits vectoriels
    normal = regard*axe;
    newAxe = normal*regard;

    // Normalisation des vecteurs
    normal = normal.normalize();
    newAxe = newAxe.normalize();
    regard = regard.normalize();


    // Introduction des valeurs dans la matrice
    matrice.m[0] = normal.x;
    matrice.m[1] = normal.y;
    matrice.m[2] = normal.z;

    matrice.m[4] = newAxe.x;
    matrice.m[5] = newAxe.y;
    matrice.m[6] = newAxe.z;

    matrice.m[8] = -regard.x;
    matrice.m[9] = -regard.y;
    matrice.m[10] = -regard.z;

    matrice.m[15] = 1.0;


    // Multiplication des matrices, puis translation de la matrice view
    m_view_matrix = matrice;
    m_view_matrix = m_view_matrix.translate(-eyeX, -eyeY, -eyeZ);
}

t_point* alfoGL::addPoint(double x, double y, double z)
{
    // Converting of homogenous coords
    t_point point = {x, y, z, 1};
    m_scene_points.push_back(point);
    return &m_scene_points[m_scene_points.size()];
}

t_line* alfoGL::addLine(double x1, double y1, double z1, double x2, double y2, double z2, Uint8 R, Uint8 G, Uint8 B)
{
    t_point point1 = {x1, y1, z1, 1};
    t_point point2 = {x2, y2, z2, 1};

    t_line line;
    line.point1 = point1;
    line.point2 = point2;
    line.color = {R, G, B};
    m_scene_lines.push_back(line);

    return &m_scene_lines[m_scene_lines.size()];
}

void alfoGL::applyMatrix(t_matrix4 m)
{
    for(int i = 0; i < m_scene_lines.size(); i++)
    {
        m_scene_lines[i].point1 = m_scene_lines[i].point1*m;
        m_scene_lines[i].point2 = m_scene_lines[i].point2*m;
    }
}

t_point alfoGL::getProjected (t_point obj)
{
    // Applying the view matrix
    t_point eye = obj*m_view_matrix;

    // Applying the projection matrix
    t_point clip;
    if(m_proj_orth) clip = eye*m_proj_matrix_orth;
    else clip = eye*m_proj_matrix;

    // normalized Device Coordinates
    t_point ndc = {clip.x/clip.w, -clip.y/clip.w, clip.z/clip.w, 1};

    // Display Format
    t_point display = {(m_window->w/2)*ndc.x+m_window->w/2, (m_window->h/2)*ndc.y+m_window->h/2, (m_far-m_near)/2*ndc.z+(m_far+m_near)/2};

    return display;
}

void alfoGL::show()
{
    SDL_FillRect(m_window, 0, SDL_MapRGB(m_window->format, 50, 100, 200));
    SDL_GetError();

    if(m_show_axis)
    {
        Uint32 red = SDL_MapRGB(m_window->format, 255, 0, 0);
        Uint32 green = SDL_MapRGB(m_window->format, 0, 255, 0);
        Uint32 blue = SDL_MapRGB(m_window->format, 0, 0, 255);

        t_point center = {0,  0,  0,  1};
        t_point x_axis = {10, 0,  0,  1};
        t_point y_axis = {0,  10, 0,  1};
        t_point z_axis = {0,  0,  10, 1};


        t_point display_c = getProjected(center);
        t_point display_x = getProjected(x_axis);
        t_point display_y = getProjected(y_axis);
        t_point display_z = getProjected(z_axis);

        drawLine(display_c.x, display_c.y, display_x.x, display_x.y, red);
        drawLine(display_c.x, display_c.y, display_y.x, display_y.y, green);
        drawLine(display_c.x, display_c.y, display_z.x, display_z.y, blue);
    }

    for(int i = 0; i < m_scene_points.size(); i++)
    {
        t_point display = getProjected(m_scene_points[i]);
        setPixel(display.x, display.y, 255255255);
    }

    for(int i = 0; i < m_scene_lines.size(); i++)
    {
        t_point display1 = getProjected(m_scene_lines[i].point1);
        t_point display2 = getProjected(m_scene_lines[i].point2);

        Uint32 color = SDL_MapRGB(m_window->format, m_scene_lines[i].color.R, m_scene_lines[i].color.G, m_scene_lines[i].color.B);
        drawLine(display1.x, display1.y, display2.x, display2.y, color, 0);

        if(m_show_z)
        {
            SDL_Rect position1 = {display1.x+2, display1.y-13};
            SDL_Rect position2 = {display2.x+2, display2.y-13};

            char text[2][50];
            sprintf(text[0], "%f", display1.z);
            sprintf(text[1], "%f", display2.z);

            SDL_Surface *render1 = TTF_RenderText_Blended(m_font, text[0], {255, 255, 255});
            SDL_Surface *render2 = TTF_RenderText_Blended(m_font, text[1], {255, 255, 255});
            SDL_BlitSurface(render1, NULL, m_window, &position1);
            SDL_BlitSurface(render2, NULL, m_window, &position2);
            SDL_FreeSurface(render1);
            SDL_FreeSurface(render2);
        }
    }

    if(m_show_debug)
    {
        char aa[4] = "OFF";
        if(m_aa_enabled) sprintf(aa, "ON");

        char text[10][50];
        sprintf(text[0], "== alfoGL Rendering Engine ==");
        sprintf(text[1], "FPS: %d", m_last_fps);
        sprintf(text[2], "Anti Aliasing x2: %s", aa);
        sprintf(text[3], "Vectors: %d", m_scene_points.size()+m_scene_lines.size());
        sprintf(text[4], "Drawn Pixels: %d", m_drawn_pixels);
        sprintf(text[5], "Screen size: %dx%d", m_window->w, m_window->h);
        sprintf(text[6], "Pixel Occupation: %f", (m_drawn_pixels*100.0)/double(m_window->w*m_window->h));

        SDL_Rect position = {5, 0};
        for(int i = 0; i < 7; i++)
        {
            position.y = 10*i+5;
            SDL_Surface *render = TTF_RenderText_Blended(m_font, text[i], {255, 255, 255});
            SDL_BlitSurface(render, NULL, m_window, &position);
            SDL_FreeSurface(render);
        }
    }

    if(m_show_help)
    {
        char text[20][50];
        sprintf(text[0], "== Available Commands ==");
        sprintf(text[1], "W,S,A,D: Move on x/y");
        sprintf(text[2], "Up,Down: Move up/down");
        sprintf(text[3], "");
        sprintf(text[4], "F1: Toggle this help");
        sprintf(text[5], "F2: Toggle Antialiasing");
        sprintf(text[6], "F3: Toggle Debug");
        sprintf(text[7], "F4: Toggle Z-buffer");
        sprintf(text[8], "F5: Toggle Axis");
        sprintf(text[9], "F6: Toggle Projection Mode");
        sprintf(text[10], "");
        sprintf(text[11], "R: Rotations");
        sprintf(text[12], "T: Translations");
        sprintf(text[13], "V: Symmetries");
        sprintf(text[14], "H: Homothety");
        sprintf(text[15], "C: Shear");
        sprintf(text[16], "");
        sprintf(text[17], "");
        sprintf(text[18], "");
        sprintf(text[19], "");

        SDL_Rect position = {m_window->w-145, 0};
        for(int i = 0; i < 20; i++)
        {
            position.y = 10*i+5;
            SDL_Surface *render = TTF_RenderText_Blended(m_font, text[i], {255, 255, 255});
            SDL_BlitSurface(render, NULL, m_window, &position);
            SDL_FreeSurface(render);
        }
    }

    if(m_show_mode)
    {
        SDL_Rect position = {5, m_window->h-15};
        SDL_Surface *render = TTF_RenderText_Blended(m_font, (char*)m_mode.c_str(), {255, 255, 255});
        SDL_BlitSurface(render, NULL, m_window, &position);
        SDL_FreeSurface(render);
    }

    SDL_Surface *image;
    SDL_Rect positionImage;
    positionImage.x = m_window->w-132;
    positionImage.y = m_window->h-50;
    image = IMG_Load("ja3GL.gif");
    //image = SDL_LoadBMP("ja3GL.bmp");
    SDL_BlitSurface(image, NULL, m_window, &positionImage);
    //SDL_FreeSurface(image);

    m_drawn_pixels = 0;
    SDL_Flip(m_window);
}

bool alfoGL::getAAEnabled()
{
    return m_aa_enabled;
}

void alfoGL::setAAEnabled(bool mode)
{
    m_aa_enabled = mode;
}

void alfoGL::setShowAxis(bool mode)
{
    m_show_axis = mode;
}

void alfoGL::setShowDebug(bool mode)
{
    m_show_debug = mode;
}

void alfoGL::setLastFps(int fps)
{
    m_last_fps = fps;
}

void alfoGL::setShownMode(string mode)
{
    m_mode = mode;
}

void alfoGL::setPixel(int x, int y, Uint32 color)
{
    if(x>0 && y>0 && x<m_window->w && y<m_window->h)
    {
        *((Uint32*)(m_window->pixels) + x + y * m_window->w) = color;
        m_drawn_pixels++;
    }
}

void alfoGL::setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if(x>0 && y>0 && x<m_window->w && y<m_window->h)
  {
      *((Uint32*)(m_window->pixels) + x + y * m_window->w) = SDL_MapRGB(m_window->format, R, G, B);
      m_drawn_pixels++;
  }
}

void alfoGL::getPixelColor(int x, int y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
  if(x>0 && y>0 && x<m_window->w && y<m_window->h)
    SDL_GetRGBA(*((Uint32 *)m_window->pixels + x + y * m_window->w),m_window->format,r,g,b,a);
}

void alfoGL::drawLine(int x1, int y1, int x2, int y2, Uint32 color, int dotted)
{
    if((x1 < 0 || x1 >= m_window->w) && (x2 < 0 || x2 >= m_window->w) && (y1 < 0 || y1 >= m_window->h) && (y2 < 0 || y2 >= m_window->h))
       return;

    if(m_aa_enabled)
        drawLineAA(x1, y1, x2, y2, color, dotted);
    else
    {

        double x = x2 - x1;
        double y = y2 - y1;
        double length = sqrt( x*x + y*y );

        double addx = x / length;
        double addy = y / length;

        x = x1;
        y = y1;

        for(double i = 0; i < length; i += 1)
        {
          setPixel((int)x, (int)y, color);
          x += addx;
          y += addy;
        }
    }
}

void alfoGL::drawLineAA(int x1, int y1, int x2, int y2, Uint32 color, int dotted)
{
  int xd, yd, x, y, to_dot = 0;
  double grad,xf,yf;
  Uint8 r1,g1,b1,a1,r2,g2,b2,a2,r,g,b,a;
  Uint32 colorAA1,colorAA2;

  // Getting the r,g,b,a values of the specified color
  SDL_GetRGBA(color,m_window->format,&r1,&g1,&b1,&a1);

  xd = (x2-x1);
  yd = (y2-y1);

  if (abs(xd) > abs(yd)) // The line angle is smaller than 45°
  {
    if (x1 > x2) // Because of the FOR, going from the smallest to the greatest value
    {
      int_switch(&x1, &x2);
      int_switch(&y1, &y2);
      xd = (x2-x1);
      yd = (y2-y1);
    }

    grad = (double)yd/(double)xd; // Line slope
    yf = (double)y1+grad;

    // Start and arrival points
    setPixel(x1, y1, color);
    setPixel(x2, y2, color);

    for (x = x1+1; x <= x2; x++)
    {
     if((!dotted || (to_dot%dotted)) && x>0 && x<m_window->w && yf>0 && yf<m_window->h) // If not outside of the map surface
     {
         //cout << "DrawLine called with: (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")." << endl;
         // Getting the color of current (background) pixel
         getPixelColor(x, (int)yf, &r2,&g2,&b2,&a2);

         // For the antialising 2 pixels are drawn, one one top (x,y+1), the other below (x,y)
         // The color is a mix of:
         // - the color of background
         // - the color of the line
         // The amount of each color (bg & line color), on the 2 pixels is 100% (e.g. 10% on one, 90% on the other)

         r=(Uint8)(frac(yf)*r1+invfrac(yf)*r2);
         g=(Uint8)(frac(yf)*g1+invfrac(yf)*g2);
         b=(Uint8)(frac(yf)*b1+invfrac(yf)*b2);
         a=(Uint8)(frac(yf)*a1+invfrac(yf)*a2);
         colorAA1=SDL_MapRGBA(m_window->format,r,g,b,a); // Color for pixel one (x,y+1)

         r=(Uint8)(frac(yf)*r2+invfrac(yf)*r1);
         g=(Uint8)(frac(yf)*g2+invfrac(yf)*g1);
         b=(Uint8)(frac(yf)*b2+invfrac(yf)*b1);
         a=(Uint8)(frac(yf)*a2+invfrac(yf)*a1);
         colorAA2=SDL_MapRGBA(m_window->format,r,g,b,a); // Color for the 2nd pixel (x,y)

         // Blitting
         setPixel(x, (int)yf, colorAA2);
         setPixel(x, (int)yf+1, colorAA1);
     }
     to_dot++;

     yf += grad; // Go to next point (x => x+1 / yf => yf + grad)
    }
  }

 else // Same thing if the line angle is greater the 45°
  {
    if (y1 > y2)
    {
      int_switch(&x1, &x2);
      int_switch(&y1, &y2);
      xd = (x2-x1);
      yd = (y2-y1);
    }

    grad = (double)xd/(double)yd;
    xf = (double)x1+grad;

    setPixel(x1, y1, color);
    setPixel(x2, y2, color);

    for (y = y1+1; y <= y2; y++)
    {
     if((!dotted || (to_dot%dotted)) && xf>0 && xf<m_window->w && y>0 && y<m_window->h)
     {
         //cout << "DrawLine called with: (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ")." << endl;
         getPixelColor((int)xf, y, &r2,&g2,&b2,&a2);

         r=(Uint8)(frac(xf)*r1+invfrac(xf)*r2);
         g=(Uint8)(frac(xf)*g1+invfrac(xf)*g2);
         b=(Uint8)(frac(xf)*b1+invfrac(xf)*b2);
         a=(Uint8)(frac(xf)*a1+invfrac(xf)*a2);
         colorAA1=SDL_MapRGBA(m_window->format,r,g,b,a);

         r=(Uint8)(frac(xf)*r2+invfrac(xf)*r1);
         g=(Uint8)(frac(xf)*g2+invfrac(xf)*g1);
         b=(Uint8)(frac(xf)*b2+invfrac(xf)*b1);
         a=(Uint8)(frac(xf)*a2+invfrac(xf)*a1);
         colorAA2=SDL_MapRGBA(m_window->format,r,g,b,a);

         setPixel((int)xf, y, colorAA2);
         setPixel((int)xf+1, y, colorAA1);
     }
     to_dot++;

     xf += grad;
    }
  }
}

alfoGL::~alfoGL()
{
    // Destructor
}
