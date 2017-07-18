% Analyze_Thrust_Power, takes experimental data (thrust force, voltage
% motor, current, power, encoder) and generates some computed and analyzed
% results. 

clear all
close all

%% Load data
load_data_2
load('Power_System');
%% Call motor_frequency 
motor_frequency_2

%%
%% Convert Fvolt to Newton Force
slp1=.0918866;   % slope from calibration; (kg per volt)
slp2=.0914251;
slp3=.0929779;

% rigid ray case 1 (force was in tension)
Fo = Fv_mean_rigid_c1(1);  % Tare measurement; 
Fmean_rigid_c1= (Fv_mean_rigid_c1-Fo)*(-slp3)*9.81;  % Mean Force, Newtons
F_rigid_c1 = (Fvolt_rigid_c1-Fo)*(-slp3)*9.81;  % Force, Newtons;
F_mean_rig_c1(1) = Fmean_rigid_c1(1);
F_std_rig_c1(1) = 0;
for j=2:12
    F_mean_rig_c1(j)=mean([Fmean_rigid_c1(3*j-4) Fmean_rigid_c1(3*j-3) Fmean_rigid_c1(3*j-2)]);
    F_std_rig_c1(j)=std([Fmean_rigid_c1(3*j-4) Fmean_rigid_c1(3*j-3) Fmean_rigid_c1(3*j-2)]);
end

% rigid ray case 2 (robot was rotated, force was in compression)
Fo = Fv_mean_rigid_c1(1);  % Tare measurement; (Sequence 12; there was no flow)
Fmean_rigid_c2= (Fv_mean_rigid_c2-Fo)*(-slp3)*9.81;  % Mean Force, Newtons
F_rigid_c2 = (Fvolt_rigid_c2-Fo)*(-slp3)*9.81;  % Force, Newtons;
F_mean_rig_c2(1) = Fmean_rigid_c2(1);
F_std_rig_c2(1) = 0;
for j=2:12
    F_mean_rig_c2(j)=mean([Fmean_rigid_c2(3*j-4) Fmean_rigid_c2(3*j-3) Fmean_rigid_c2(3*j-2)]);
    F_std_rig_c2(j)=std([Fmean_rigid_c2(3*j-4) Fmean_rigid_c2(3*j-3) Fmean_rigid_c2(3*j-2)]);
end

% flexible ray1 case 1
Fo = Fv_mean_flex1_c1(1);  % Tare measurement; 
Fmean_flex1_c1= (Fv_mean_flex1_c1-Fo)*(-slp2)*9.81;  % Mean Force, Newtons
F_flex1_c1 = (Fvolt_flex1_c1-Fo)*(-slp2)*9.81;  % Force, Newtons;
F_mean_flex1_c1(1) = Fmean_flex1_c1(1);
F_std_flex1_c1(1) = 0;
for j=2:12
    F_mean_flex1_c1(j)=mean([Fmean_flex1_c1(3*j-4) Fmean_flex1_c1(3*j-3) Fmean_flex1_c1(3*j-2)]);
    F_std_flex1_c1(j)=std([Fmean_flex1_c1(3*j-4) Fmean_flex1_c1(3*j-3) Fmean_flex1_c1(3*j-2)]);
end

% flexible ray1 case 2
Fo = Fv_mean_flex1_c1(1);  % Tare measurement; 
Fmean_flex1_c2= (Fv_mean_flex1_c2-Fo)*(-slp2)*9.81;  % Mean Force, Newtons
F_flex1_c2 = (Fvolt_flex1_c2-Fo)*(-slp2)*9.81;  % Force, Newtons;
F_mean_flex1_c2(1) = Fmean_flex1_c2(1);
F_std_flex1_c2(1) = 0;
for j=2:12
    F_mean_flex1_c2(j)=mean([Fmean_flex1_c2(3*j-4) Fmean_flex1_c2(3*j-3) Fmean_flex1_c2(3*j-2)]);
    F_std_flex1_c2(j)=std([Fmean_flex1_c2(3*j-4) Fmean_flex1_c2(3*j-3) Fmean_flex1_c2(3*j-2)]);
end

% flexible ray2 case 1
Fo = Fv_mean_flex2_c1(1);  % Tare measurement; 
Fmean_flex2_c1= (Fv_mean_flex2_c1-Fo)*(-slp1)*9.81;  % Mean Force, Newtons
F_flex2_c1 = (Fvolt_flex2_c1-Fo)*(-slp1)*9.81;  % Force, Newtons;
F_mean_flex2_c1(1) = Fmean_flex2_c1(1);
F_std_flex2_c1(1) = 0;
for j=2:12
    F_mean_flex2_c1(j)=mean([Fmean_flex2_c1(3*j-4) Fmean_flex2_c1(3*j-3) Fmean_flex2_c1(3*j-2)]);
    F_std_flex2_c1(j)=std([Fmean_flex2_c1(3*j-4) Fmean_flex2_c1(3*j-3) Fmean_flex2_c1(3*j-2)]);
end

% flexible ray2 case 2
Fo = Fv_mean_flex2_c1(1);  % Tare measurement; 
Fmean_flex2_c2= (Fv_mean_flex2_c2-Fo)*(-slp1)*9.81;  % Mean Force, Newtons
F_flex2_c2 = (Fvolt_flex2_c2-Fo)*(-slp1)*9.81;  % Force, Newtons;
F_mean_flex2_c2(1) = Fmean_flex2_c2(1);
F_std_flex2_c2(1) = 0;
for j=2:12
    F_mean_flex2_c2(j)=mean([Fmean_flex2_c2(3*j-4) Fmean_flex2_c2(3*j-3) Fmean_flex2_c2(3*j-2)]);
    F_std_flex2_c2(j)=std([Fmean_flex2_c2(3*j-4) Fmean_flex2_c2(3*j-3) Fmean_flex2_c2(3*j-2)]);
end

% flexible ray3 case 1
Fo = Fv_mean_flex3_c1(1);  % Tare measurement; 
Fmean_flex3_c1= (Fv_mean_flex3_c1-Fo)*(-slp1)*9.81;  % Mean Force, Newtons
F_flex3_c1 = (Fvolt_flex3_c1-Fo)*(-slp1)*9.81;  % Force, Newtons;
F_mean_flex3_c1(1) = Fmean_flex3_c1(1);
F_std_flex3_c1(1) = 0;
for j=2:12
    F_mean_flex3_c1(j)=mean([Fmean_flex3_c1(3*j-4) Fmean_flex3_c1(3*j-3) Fmean_flex3_c1(3*j-2)]);
    F_std_flex3_c1(j)=std([Fmean_flex3_c1(3*j-4) Fmean_flex3_c1(3*j-3) Fmean_flex3_c1(3*j-2)]);
end

% flexible ray3 case 2
Fo = Fv_mean_flex3_c1(1);  % Tare measurement; 
Fmean_flex3_c2= (Fv_mean_flex3_c2-Fo)*(-slp1)*9.81;  % Mean Force, Newtons
F_flex3_c2 = (Fvolt_flex3_c2-Fo)*(-slp1)*9.81;  % Force, Newtons;
F_mean_flex3_c2(1) = Fmean_flex3_c2(1);
F_std_flex3_c2(1) = 0;
for j=2:12
    F_mean_flex3_c2(j)=mean([Fmean_flex3_c2(3*j-4) Fmean_flex3_c2(3*j-3) Fmean_flex3_c2(3*j-2)]);
    F_std_flex3_c2(j)=std([Fmean_flex3_c2(3*j-4) Fmean_flex3_c2(3*j-3) Fmean_flex3_c2(3*j-2)]);
end
% %% Net Power
% 
% P_net_mean_rig_c1 = P_mean_rig_c1 - 2.614*freq_mean_rig_c1;
% P_net_mean_rig_c2 = P_mean_rig_c2 - 2.614*freq_mean_rig_c2;
% P_net_mean_flex1_c1 = P_mean_flex1_c1 - 2.5*freq_mean_flex1_c1;
% P_net_mean_flex1_c2 = P_mean_flex1_c2 - 2.5*freq_mean_flex1_c2;
% P_net_mean_flex2_c1 = P_mean_flex2_c1 - 2.5*freq_mean_flex2_c1;
% P_net_mean_flex2_c2 = P_mean_flex2_c2 - 2.5*freq_mean_flex2_c2;
% P_net_mean_flex3_c1 = P_mean_flex3_c1 - 2.5*freq_mean_flex3_c1;
% P_net_mean_flex3_c2 = P_mean_flex3_c2 - 2.5*freq_mean_flex3_c2;

%% Time vector
dt=1/sample_freq;
time =0:dt:duration-dt;

%% Force coefficient
rho = 1000; lamda = 0.15; S = 0.05; U_char=0.166; C=0.15;

    V_wave_rig_c1 = freq_mean_rig_c1.*lamda;
    Cf_rig_c1 = F_mean_rig_c1./(0.5*rho*V_wave_rig_c1.^2*C*S);
    V_wave_flex1_c1 = freq_mean_flex1_c1.*lamda;
    Cf_flex1_c1 = F_mean_flex1_c1./(0.5*rho*V_wave_flex1_c1.^2*C*S);
    V_wave_flex2_c1 = freq_mean_flex2_c1.*lamda;
    Cf_flex2_c1 = F_mean_flex2_c1./(0.5*rho*V_wave_flex2_c1.^2*C*S);
    V_wave_flex3_c1 = freq_mean_flex3_c1.*lamda;
    Cf_flex3_c1 = F_mean_flex3_c1./(0.5*rho*V_wave_flex3_c1.^2*C*S);


for i=1:12
    V_wave_rig_c2 = freq_mean_rig_c2.*lamda - U_char;
    Cf_rig_c2 = F_mean_rig_c2./(0.5*rho*V_wave_rig_c2.^2*C*S);
    V_wave_flex1_c2 = freq_mean_flex1_c2.*lamda - U_char;
    Cf_flex1_c2 = F_mean_flex1_c2./(0.5*rho*V_wave_flex1_c2.^2*C*S);
    V_wave_flex2_c2 = freq_mean_flex2_c2.*lamda - U_char;
    Cf_flex2_c2 = F_mean_flex2_c2./(0.5*rho*V_wave_flex2_c2.^2*C*S);
    V_wave_flex3_c2 = freq_mean_flex3_c2.*lamda - U_char;
    Cf_flex3_c2 = F_mean_flex3_c2./(0.5*rho*V_wave_flex3_c2.^2*C*S);
end
%% Propulsive Efficiency 
FP_rigid_c1 = F_mean_rig_c1./P_mean_rig_c1;
FP_rigid_c2 = F_mean_rig_c2.*V_wave_rig_c2./P_mean_rig_c2;


FP_flex1_c1 = F_mean_flex1_c1./P_mean_flex1_c1;
FP_flex1_c2 = F_mean_flex1_c2.*V_wave_flex1_c2./P_mean_flex1_c2;


FP_flex2_c1 = F_mean_flex2_c1./P_mean_flex2_c1;
FP_flex2_c2 = F_mean_flex2_c2.*V_wave_flex2_c2./P_mean_flex2_c2;


FP_flex3_c1 = F_mean_flex3_c1./P_mean_flex3_c1;
FP_flex3_c2 = F_mean_flex3_c2.*V_wave_flex3_c2./P_mean_flex3_c2;
%% Power Coefficient

    CP_rig_c1 = P_mean_rig_c1./(0.5*rho*V_wave_rig_c1.^3*C*S);
    
    CP_flex1_c1 = P_mean_flex1_c1./(0.5*rho*V_wave_flex1_c1.^3*C*S);
    
    CP_flex2_c1 = P_mean_flex2_c1./(0.5*rho*V_wave_flex2_c1.^3*C*S);
    
    CP_flex3_c1 = P_mean_flex3_c1./(0.5*rho*V_wave_flex3_c1.^3*C*S);

    
    CP_rig_c2 = P_mean_rig_c2./(0.5*rho*V_wave_rig_c2.^3*C*S);
    
    CP_flex1_c2 = P_mean_flex1_c2./(0.5*rho*V_wave_flex1_c2.^3*C*S);
    
    CP_flex2_c2 = P_mean_flex2_c2./(0.5*rho*V_wave_flex2_c2.^3*C*S);
    
    CP_flex3_c2 = P_mean_flex3_c2./(0.5*rho*V_wave_flex3_c2.^3*C*S);

%%


%% Plot
figure(1)
P_rig_c1 = errorbarxy(freq_mean_rig_c1,P_mean_rig_c1,freq_std_rig_c1,P_std_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
P_flex1_c1 = errorbarxy(freq_mean_flex1_c1,P_mean_flex1_c1,freq_std_flex1_c1,P_std_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
P_flex2_c1 = errorbarxy(freq_mean_flex2_c1,P_mean_flex2_c1,freq_std_flex2_c1,P_std_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
P_flex3_c1 = errorbarxy(freq_mean_flex3_c1,P_mean_flex3_c1,freq_std_flex3_c1,P_std_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
P_tare_c1 = plot(freq_motor_c1(1:11),2.5*freq_motor_c1(1:11),'Color','g','LineStyle','none','Marker','^',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_motor_c1,2.5*freq_motor_c1-0.5,'-g','LineWidth',0.5);hold on
plot(freq_motor_c1,2.5*freq_motor_c1+0.5,'-g','LineWidth',0.5);hold off
legend([P_rig_c1(1) P_flex1_c1(1) P_flex2_c1(1) P_flex3_c1(1) P_tare_c1],{'rigid' 'flex 1' 'flex 2' 'flex 3' 'Tare'});
axis([0 6 0 25]);
xlabel('Frequency, Hz');
ylabel('Power, W');
title('Total Power');
set(gca,'FontSize',13)
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 0 mm/s');
%%
figure(2)
P_rig_c2 = errorbarxy(freq_mean_rig_c2,P_mean_rig_c2,freq_std_rig_c2,P_std_rig_c2,'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
P_flex1_c2 = errorbarxy(freq_mean_flex1_c2,P_mean_flex1_c2,freq_std_flex1_c2,P_std_flex1_c2,'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
P_flex2_c2 = errorbarxy(freq_mean_flex2_c2,P_mean_flex2_c2,freq_std_flex2_c2,P_std_flex2_c2,'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
P_flex3_c2 = errorbarxy(freq_mean_flex3_c2,P_mean_flex3_c2,freq_std_flex3_c2,P_std_flex3_c2,'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold on
% P_rig_c1 = errorbarxy(freq_mean_rig_c1,P_mean_rig_c1,freq_std_rig_c1,P_std_rig_c1,'Color','r','LineStyle','none','Marker','o',...
% 'LineWidth',1,'MarkerSize',10);hold on
% P_flex1_c1 = errorbarxy(freq_mean_flex1_c1,P_mean_flex1_c1,freq_std_flex1_c1,P_std_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
% 'LineWidth',1,'MarkerSize',10);hold on
P_tare_c2 = plot(freq_motor_c1(1:11),2.5*freq_motor_c1(1:11),'Color','k','LineStyle','none','Marker','^',...
'MarkerFaceColor','g','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_motor_c1,2.5*freq_motor_c1-0.5,'-g','LineWidth',0.5);hold on
plot(freq_motor_c1,2.5*freq_motor_c1+0.5,'-g','LineWidth',0.5);hold off
axis([0 6 0 25])
xlabel('Frequency, Hz')
ylabel('Power, W');
title('Total Power');
set(gca,'FontSize',13)
legend([P_rig_c2(1) P_flex1_c2(1) P_flex2_c2(1) P_flex3_c2(1) P_tare_c2(1)],{'rigid' 'flex 1' 'flex 2' 'flex 3' 'Tare'});
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 166 mm/s')
%%
figure(3)
F_rig_c1 = errorbarxy(freq_mean_rig_c1,F_mean_rig_c1,freq_std_rig_c1,F_std_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
F_flex1_c1 = errorbarxy(freq_mean_flex1_c1,F_mean_flex1_c1,freq_std_flex1_c1,F_std_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
F_flex2_c1 = errorbarxy(freq_mean_flex2_c1,F_mean_flex2_c1,freq_std_flex2_c1,F_std_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
F_flex3_c1 = errorbarxy(freq_mean_flex3_c1,F_mean_flex3_c1,freq_std_flex3_c1,F_std_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold off
axis([0 6 0 1.6])
xlabel('Frequency, Hz')
ylabel('Force, N')
set(gca,'FontSize',13)
legend([F_rig_c1(1) F_flex1_c1(1) F_flex2_c1(1) F_flex3_c1(1)],{'rigid' 'flex 1' 'flex 2' 'flex 3'});
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 0 mm/s')
%%
figure(4)
F_rig_c2 = errorbarxy(freq_mean_rig_c2,F_mean_rig_c2,freq_std_rig_c2,F_std_rig_c2,'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
F_flex1_c2 = errorbarxy(freq_mean_flex1_c2,F_mean_flex1_c2,freq_std_flex1_c2,F_std_flex1_c2,'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
F_flex2_c2 = errorbarxy(freq_mean_flex2_c2,F_mean_flex2_c2,freq_std_flex2_c2,F_std_flex2_c2,'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
F_flex3_c2 = errorbarxy(freq_mean_flex3_c2,F_mean_flex3_c2,freq_std_flex3_c2,F_std_flex3_c2,'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold on
axis([0 6 -0.2 1.1])
plot([0 7],[0,0],'color',[.5 .5 .5]); hold off
xlabel('Frequency, Hz')
ylabel('Force, N')
set(gca,'FontSize',13)
legend([F_rig_c2(1) F_flex1_c2(1) F_flex2_c2(1) F_flex3_c2(1)],{'rigid' 'flex 1' 'flex 2' 'flex 3'});
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 166 mm/s')
%%
figure(5)
F_P_rig_c1 = plot(freq_mean_rig_c1(2:end),FP_rigid_c1(2:end),'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
F_P_flex1_c1 = plot(freq_mean_flex1_c1(2:end),FP_flex1_c1(2:end),'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
F_P_flex2_c1 = plot(freq_mean_flex2_c1(2:end),FP_flex2_c1(2:end),'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
F_P_flex3_c1 = plot(freq_mean_flex3_c1(2:end),FP_flex3_c1(2:end),'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold off
xlabel('Frequency, Hz')
ylabel('Force/Power')
set(gca,'FontSize',13)
title('Propulsive Behavior vs. Frequency');
legend([F_P_rig_c1(1) F_P_flex1_c1(1) F_P_flex2_c1(1) F_P_flex3_c1(1)],{'rigid' 'flex 1' 'flex 2' 'flex 3'});
% axis([0 6 0 1.6])
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 0 mm/s')
%%
figure(6)

F_P_rig_c2 = plot(freq_mean_rig_c2(2:end),FP_rigid_c2(2:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',1,'MarkerSize',10);hold on
F_P_flex1_c2 = plot(freq_mean_flex1_c2(2:end),FP_flex1_c2(2:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',1,'MarkerSize',10);hold on
F_P_flex2_c2 = plot(freq_mean_flex2_c2(2:end),FP_flex2_c2(2:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',1,'MarkerSize',10);hold on
F_P_flex3_c2 = plot(freq_mean_flex3_c2(2:end),FP_flex3_c2(2:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',1,'MarkerSize',10);hold on
plot([0 6],[0,0],'color',[.5 .5 .5]);hold off
% axis([0 6 -0.2 1.2]); 
xlabel('Frequency, Hz')
ylabel('Net Propulsive efficiency')
set(gca,'FontSize',13)
title('Propulsive Efficiency vs. Frequency');
legend([F_P_rig_c2(1) F_P_flex1_c2(1) F_P_flex2_c2(1) F_P_flex3_c2],{'rigid' 'flex 1' 'flex 2' 'flex3'});
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 166 mm/s')

%%
figure(7)

plot(freq_mean_rig_c1,Cf_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex1_c1,Cf_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex2_c1,Cf_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex3_c1,Cf_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold off
xlabel('Frequency')
ylabel('C_F')
title('C_F using Relative Velocity')
set(gca,'FontSize',13)
legend('rigid','flex1','flex2','flex3');
%%
figure(8)
CF_rig_c2 = plot(freq_mean_rig_c2(5:end),Cf_rig_c2(5:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
CF_flex1_c2 = plot(freq_mean_flex1_c2(5:end),Cf_flex1_c2(5:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
CF_flex2_c2 = plot(freq_mean_flex2_c2(5:end),Cf_flex2_c2(5:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
CF_flex3_c2 = plot(freq_mean_flex3_c2(5:end),Cf_flex3_c2(5:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold on
% axis([0,12,-1,10])
plot([0 6],[0,0],'color',[.5 .5 .5]); 
hold off
xlabel('Frequency')
ylabel('C_F')
title('C_F using Relative Velocity')
set(gca,'FontSize',13)
legend([CF_rig_c2(1) CF_flex1_c2(1) CF_flex2_c2(1) CF_flex3_c2(1)],{'rigid' 'flex 1' 'flex 2' 'flex 3'});
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 166 mm/s')

%%
figure(9)
plot(freq_mean_rig_c1,CP_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex1_c1,CP_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex2_c1,CP_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex3_c1,CP_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold off
xlabel('Frequency')
ylabel('C_P')
title('C_P using Relative Velocity')
set(gca,'FontSize',13)
legend('rigid','flex1','flex2','flex3');

%%
figure(10)
plot(freq_mean_rig_c2(5:end),CP_rig_c2(5:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex1_c2(5:end),CP_flex1_c2(5:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex2_c2(5:end),CP_flex2_c2(5:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex3_c2(5:end),CP_flex3_c2(5:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
% axis([0 12 0 45])
xlabel('Frequency')
ylabel('C_P')
title('C_P using Relative Velocity')
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3');
% Create textbox
annotation('textbox', [0.3 0.8 0.25 0.07],'String','Water speed = 166 mm/s')
%%
figure(11)
plot(freq_mean_rig_c1,F_mean_rig_c1./(0.5*rho*(freq_mean_rig_c1.*lamda).^2*C*S),'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex1_c1,F_mean_flex1_c1./(0.5*rho*(freq_mean_flex1_c1.*lamda).^2*C*S),'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex2_c1,F_mean_flex2_c1./(0.5*rho*(freq_mean_flex2_c1.*lamda).^2*C*S),'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex3_c1,F_mean_flex3_c1./(0.5*rho*(freq_mean_flex3_c1.*lamda).^2*C*S),'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_rig_c2,F_mean_rig_c2./(0.5*rho*(freq_mean_rig_c2.*lamda).^2*C*S),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex1_c2,F_mean_flex1_c2./(0.5*rho*(freq_mean_flex1_c2.*lamda).^2*C*S),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex2_c2,F_mean_flex2_c2./(0.5*rho*(freq_mean_flex2_c2.*lamda).^2*C*S),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex3_c2,F_mean_flex3_c2./(0.5*rho*(freq_mean_flex3_c2.*lamda).^2*C*S),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
xlabel('Frequency (Hz)')
ylabel('C_T')
title('C_T using Wave Velocity')
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3', 'rigid with flow', 'flex1 with flow', 'flex2 with flow', 'flex3 with flow');
%%
figure(12)
plot(freq_mean_rig_c1,P_mean_rig_c1./(0.5*rho*(freq_mean_rig_c1.*lamda).^3*C*S),'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex1_c1,P_mean_flex1_c1./(0.5*rho*(freq_mean_flex1_c1.*lamda).^3*C*S),'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex2_c1,P_mean_flex2_c1./(0.5*rho*(freq_mean_flex2_c1.*lamda).^3*C*S),'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_flex3_c1,P_mean_flex3_c1./(0.5*rho*(freq_mean_flex3_c1.*lamda).^3*C*S),'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
plot(freq_mean_rig_c2,P_mean_rig_c2./(0.5*rho*(freq_mean_rig_c2.*lamda).^3*C*S),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex1_c2,P_mean_flex1_c2./(0.5*rho*(freq_mean_flex1_c2.*lamda).^3*C*S),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex2_c2,P_mean_flex2_c2./(0.5*rho*(freq_mean_flex2_c2.*lamda).^3*C*S),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(freq_mean_flex3_c2,P_mean_flex3_c2./(0.5*rho*(freq_mean_flex3_c2.*lamda).^3*C*S),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
xlabel('Frequency (Hz)')
ylabel('C_P')
title('C_P using Wave Velocity')
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3', 'rigid with flow', 'flex1 with flow', 'flex2 with flow', 'flex3 with flow');
%%
figure (13)
plot(V_wave_rig_c1(2:end),FP_rigid_c1(2:end),'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex1_c1(2:end),FP_flex1_c1(2:end),'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex2_c1(2:end),FP_flex2_c1(2:end),'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex3_c1(2:end),FP_flex3_c1(2:end),'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_rig_c2(2:end),FP_rigid_c2(2:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex1_c2(2:end),FP_flex1_c2(2:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex2_c2(2:end),FP_flex2_c2(2:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex3_c2(2:end),FP_flex3_c2(2:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
xlabel('Relative Velocity (m/s)')
ylabel('Propulsive Efficiency')
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3', 'rigid with flow', 'flex1 with flow', 'flex2 with flow', 'flex3 with flow');
%%
figure (14)
plot(V_wave_rig_c1,Cf_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex1_c1,Cf_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex2_c1,Cf_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex3_c1,Cf_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_rig_c2(5:end),Cf_rig_c2(5:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex1_c2(5:end),Cf_flex1_c2(5:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex2_c2(5:end),Cf_flex2_c2(5:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex3_c2(5:end),Cf_flex3_c2(5:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
xlabel('Relative Velocity (m/s)');
ylabel('C_F');
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3', 'rigid with flow', 'flex1 with flow', 'flex2 with flow', 'flex3 with flow');
%%
figure(15)
plot(V_wave_rig_c1,CP_rig_c1,'Color','r','LineStyle','none','Marker','o',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex1_c1,CP_flex1_c1,'Color','b','LineStyle','none','Marker','s',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex2_c1,CP_flex2_c1,'Color','m','LineStyle','none','Marker','d',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_flex3_c1,CP_flex3_c1,'Color','k','LineStyle','none','Marker','>',...
'LineWidth',1,'MarkerSize',10);hold on
plot(V_wave_rig_c2(5:end),CP_rig_c2(5:end),'Color','k','LineStyle','none','Marker','o',...
'MarkerFaceColor','r','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex1_c2(5:end),CP_flex1_c2(5:end),'Color','k','LineStyle','none','Marker','s',...
'MarkerFaceColor','b','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex2_c2(5:end),CP_flex2_c2(5:end),'Color','k','LineStyle','none','Marker','d',...
'MarkerFaceColor','m','LineWidth',0.5,'MarkerSize',10);hold on
plot(V_wave_flex3_c2(5:end),CP_flex3_c2(5:end),'Color','k','LineStyle','none','Marker','>',...
'MarkerFaceColor','k','LineWidth',0.5,'MarkerSize',10);hold off
xlabel('Relative Velocity (m/s)');
ylabel('C_P');
set(gca,'FontSize',13)
legend('rigid', 'flex 1', 'flex 2', 'flex 3', 'rigid with flow', 'flex1 with flow', 'flex2 with flow', 'flex3 with flow');
