#ifndef MODELS_H
#define MODELS_H

#include "vector.h"

using pp::vector;

// Right-hand-side ODE systems

vector harmonic(double x, vector y);
vector friction(double x, vector y);
vector lotka(double x, vector y);

vector orbit(double x, vector y, double epsilon);

vector threebody(double t, vector z);

#endif