// models.cc

#include "models.h"

// Simple harmonic oscillator: u''(t) + u(t) = 0 -->
//      u'(t) = v(t)
//      v'(t) = -u(t)
vector harmonic(double /*x*/, vector y){
    vector dy(2);
    dy[0] = y[1];
    dy[1] = -y[0];
    return dy;
}

// Damped oscillator: theta''(t) + b*omega(t) + c*sin(theta(t)) = 0 -->
//      theta'(t) =  omega(t)
//      omega'(t) = -b*omega(t) - c*sin(theta(t))
vector friction(double /*x*/, vector y){
    double b = 0.25;  // damping
    double c = 5.0;   // spring constant

    vector dy(2);
    dy[0] = y[1];
    dy[1] = -b*y[1] - c*std::sin(y[0]);
    return dy;
}

// Lotka–Volterra system:
//      x'(t) =  a*x(t) - b*x(t)*y(t)
//      y'(t) = -c*y(t) + d*x(t)*y(t)
vector lotka(double /*x*/, vector y){
    double a = 1.5, b = 1, c = 3, d = 1;  // parameters

    vector dy(2);
    dy[0] = a*y[0] - b*y[0]*y[1];
    dy[1] = -c*y[1] + d*y[0]*y[1];
    return dy;
}

// Planetary orbit in GR: u''(φ) + u(φ) = 1 + epsilon*u(φ)*u(φ) -->
//      u'(φ) = v(φ)
//      v'(φ) = 1 - u(φ) + epsilon*u(φ)*u(φ)
vector orbit(double /*phi*/, vector y, double epsilon){
    vector dy(2);

    dy[0] = y[1];
    dy[1] = 1.0 - y[0] + epsilon * y[0] * y[0]; // Newtonian + GR correction term

    return dy;
}


// Three-body problem
vector threebody(double /*t*/, vector z){

    vector dz(12);

    // positions
    double x1=z[6], y1=z[7];
    double x2=z[8], y2=z[9];
    double x3=z[10],y3=z[11];

    // velocities
    double vx1=z[0], vy1=z[1];
    double vx2=z[2], vy2=z[3];
    double vx3=z[4], vy3=z[5];


    auto force = [](double xi,double yi,
                    double xj,double yj){

        double dx=xj-xi;
        double dy=yj-yi;

        double r=sqrt(dx*dx+dy*dy);

        double r3=r*r*r;

        return std::pair(
            dx/r3,
            dy/r3
        );
    };


    auto [fx12,fy12]=force(x1,y1,x2,y2);
    auto [fx13,fy13]=force(x1,y1,x3,y3);

    auto [fx21,fy21]=force(x2,y2,x1,y1);
    auto [fx23,fy23]=force(x2,y2,x3,y3);

    auto [fx31,fy31]=force(x3,y3,x1,y1);
    auto [fx32,fy32]=force(x3,y3,x2,y2);


    // accelerations

    dz[0]=fx12+fx13;
    dz[1]=fy12+fy13;

    dz[2]=fx21+fx23;
    dz[3]=fy21+fy23;

    dz[4]=fx31+fx32;
    dz[5]=fy31+fy32;


    // positions evolve by velocity

    dz[6]=vx1;
    dz[7]=vy1;

    dz[8]=vx2;
    dz[9]=vy2;

    dz[10]=vx3;
    dz[11]=vy3;

    return dz;
}