function Robot = Robot_DH()
%   Returns D-H table of parameters for Robotic Arm
%   Robot=[d v a alpha;
%          d v a alpha;
%          . . .   .  ;
%          d v a alpha];
%   Use symbolic variables for each joint coordinate of the robot: in the d
%   column for a prismatic joint and in the v column for a rotational
%   joint. Name the variables from q1 to qn.


% ??? enter the links dimensions and references here ???
a_sh         = 0.8;    %-> shoulder link length
a_arm        = 0.8;     %-> arm link length
a_leg        = 0.8;     %-> leg length (to the EF)
d_sh         = 0;       %-> horizontal length of the shoulder-arm link
d_arm        = 0;       %-> horizontal length of the arm-leg link
d_leg        = 0;       %-> horizontal length of the leg-EF link
ref_shoulder = 0;       %-> reference for the shoulder senarvo
ref_arm      = 0;       %-> reference for the arm servo
ref_leg      = 0;       %-> reference for the leg servo
rc           = 0.8;     %-> center radius
h            = 0;     %-> height from the virtual center to the first servo
k            = 0;       %-> leg index starting in 0 ending in 5

syms t2 t3 t4 real;

%        d      teta                a       alfa   ref
Robot = [0      k*(pi/3)+(pi/6)     0       0      0;
         h      0                   rc      0      0;
         d_sh   t2                  a_sh    -pi/2  0;
         d_arm  t3                  a_arm   pi     0;
         d_leg  t4                  a_leg   0      0];

end

