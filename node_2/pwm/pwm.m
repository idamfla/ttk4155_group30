MCK = 84e6;
CPRD_MAX = 2^16;
T_desired = 20e-3;
t_on_min = 0.9e-3;
t_on_max = 2.1e-3;

clk_div_ideal = MCK/CPRD_MAX/(1/T_desired);
clk_div = ceil(clk_div_ideal)

CPRD_ideal = MCK/clk_div * T_desired;
CPRD = round(CPRD)

T = 1/(MCK/clk_div/CPRD)

CDTY_min = ceil(t_on_min/T * CPRD)
CDTY_max = floor(t_on_max/T * CPRD)
CDTY_middle = round((CDTY_max+CDTY_min)/2)

