#include "Welford.hpp"
#include <cmath>

double Welford::get_v_bar(){
    return v_bar/i;
}

double Welford::get_x_bar(){
    return x_bar;
}

void Welford::update(double x_i, double t_i){
    double delta_i = t_i - t_i_1;
    v_bar = v_bar + (delta_i * t_i_1 / t_i)*(pow(x_i-x_bar, 2.0));
    x_bar = x_bar + (delta_i / t_i)*(x_i - x_bar);
    t_i_1 = t_i;
    i++;
}

Welford::Welford(){
    i = 0;
    x_bar = 0.0;
    v_bar = 0.0;
    t_i_1 = 0.0;
}