;

function [clk_div, CPRD, T] = pwm_config(T_desired)
  MCK = 84e6;
  CPRD_MAX = 2^16;

  clk_div_ideal = MCK/CPRD_MAX/(1/T_desired);
  clk_div = ceil(clk_div_ideal);

  CPRD_ideal = MCK/clk_div * T_desired;
  CPRD = round(CPRD_ideal);

  T = 1/(MCK/clk_div/CPRD);
end


% Channel 0

T0_desired = 1/20e3;

[clk_div0, CPRD0, T0] = pwm_config(T0_desired)


% Channel 1
T1_desired = 20e-3;
t1_on_min = 0.9e-3;
t1_on_max = 2.1e-3;

[clk_div1, CPRD1, T1] = pwm_config(T1_desired)

CDTY1_min = ceil(t1_on_min/T1 * CPRD1)
CDTY1_max = floor(t1_on_max/T1 * CPRD1)
CDTY1_middle = round((CDTY1_max+CDTY1_min)/2)




