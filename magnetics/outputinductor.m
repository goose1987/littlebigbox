%%%%%%%%%%%%%%%%%

%output inductor design
%hoang pham

%%%%%%%%%%%%%%%%%



%assumed quantities
rho=1.0; %wire resistivity Ohm*cm
Imax=50; %peak winding current (A)
L=100e-6; %desired inductance (H)
R=1; %winding resistance (Ohm)
Ku=0.5;%winding fill factor
Bmax=1;%core maximum flux density

uo=4*pi*10^-7; % H/m
%core dimensions
%{
Ac=1; %core cross-sectional area (cm^2)
Wa=1; %core window area (cm^2)
MLT=1; %mean length per turn (cm)

%}

%Ac^2*Wa/MLT > Kg=rho*L^2*Imax^2*10^8/(Bmax^2*R*Ku) core geometry factor
Kg=rho*L^2*Imax^2*10^8/(Bmax^2*R*Ku);

%%choose a core based on Kg
Ac=1.0;
Wa=1.0;
MLT=1.0;
%calculate air gap length
lg=uo*L*Imax^2*10^4/(Bmax^2*Ac); %air gap in meter (m)
Al=10*Bmax^2*Ac^2/(L*Imax^2); %Al mH/1000 turns

n=L*Imax*10^4/(Bmax*Ac); %number of turn round to nearest integer


