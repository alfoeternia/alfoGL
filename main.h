#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <string>

enum { MODE_NONE,
       ROTATION_X, ROTATION_Y, ROTATION_Z,
       TRANSLATION_X, TRANSLATION_Y, TRANSLATION_Z,
       SYMMETRY_O, SYMMETRY_X, SYMMETRY_Y, SYMMETRY_Z,
       HOMOTHETY,
       SHEAR
       };

std::string getStringFromMode(int mode);

#endif // MAIN_H_INCLUDED
