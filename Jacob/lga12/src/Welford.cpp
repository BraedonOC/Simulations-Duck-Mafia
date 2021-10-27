#include "Welford.hpp"
#include <cmath>
#include <iostream>

double Welford::get_v_bar(){
    return this->v_bar/i;
}

double Welford::get_x_bar(){
    return this->x_bar;
}

void Welford::update(double x_i, double t_i){
    double delta_i = t_i - t_i_1;
    this->v_bar = v_bar + (delta_i * t_i_1 / t_i)*(pow(x_i - x_bar, 2.0));
    this->x_bar = x_bar + (delta_i / t_i)*(x_i - x_bar);
    this->t_i_1 = t_i;
    i++;
}

void Welford::simple_update(double x_i){
    this->i++;
    this->v_bar += (double)(i - 1.0)*(pow(x_i-this->x_bar, 2.0))/i;
    this->x_bar += (double)(1.0 / i)*(x_i - this->x_bar);
}

Welford::Welford(){
    i = 0;
    x_bar = 0.0;
    v_bar = 0.0;
    t_i_1 = 0.0;
}