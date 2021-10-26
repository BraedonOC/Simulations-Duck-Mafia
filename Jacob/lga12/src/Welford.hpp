#pragma once

class Welford {
    private:
        int i;
        double x_bar, v_bar, t_i_1;
    
    public:
        Welford();
        double get_x_bar();
        double get_v_bar();
        void update(double x_i, double t_i);
};