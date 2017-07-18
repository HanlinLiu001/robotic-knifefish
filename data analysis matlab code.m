%% Velocity Calculation from tracking
clear; clc; close all

%% Data without pectoral fins
% Read data of all freqs and waves
Pos0_5w1 = csvread('Freq0_5w1');
Pos1_0w1 = csvread('Freq1_0w1');
Pos1_5w1 = csvread('Freq1_5w1');
Pos2_0w1 = csvread('Freq2_0w1');
Pos2_5w1 = csvread('Freq2_5w1');
Pos3_0w1 = csvread('Freq3_0w1');

Pos0_5w2 = csvread('Freq0_5w2');
Pos1_0w2 = csvread('Freq1_0w2');
Pos1_5w2 = csvread('Freq1_5w2');
Pos2_0w2 = csvread('Freq2_0w2');
Pos2_5w2 = csvread('Freq2_5w2');
Pos3_0w2 = csvread('Freq3_0w2');

% Time and Position
time0_5w1 = Pos0_5w1(:,1);
time1_0w1 = Pos1_0w1(:,1);
time1_5w1 = Pos1_5w1(:,1);
time2_0w1 = Pos2_0w1(:,1);
time2_5w1 = Pos2_5w1(:,1);
time3_0w1 = Pos3_0w1(:,1);

time0_5w2 = Pos0_5w2(:,1);
time1_0w2 = Pos1_0w2(:,1);
time1_5w2 = Pos1_5w2(:,1);
time2_0w2 = Pos2_0w2(:,1);
time2_5w2 = Pos2_5w2(:,1);
time3_0w2 = Pos3_0w2(:,1);

X0_5w1 = Pos0_5w1(:,2);
Y0_5w1 = Pos0_5w1(:,3);
X1_0w1 = Pos1_0w1(:,2);
Y1_0w1 = Pos1_0w1(:,3);
X1_5w1 = Pos1_5w1(:,2);
Y1_5w1 = Pos1_5w1(:,3);
X2_0w1 = Pos2_0w1(:,2);
Y2_0w1 = Pos2_0w1(:,3);
X2_5w1 = Pos2_5w1(:,2);
Y2_5w1 = Pos2_5w1(:,3);
X3_0w1 = Pos3_0w1(:,2);
Y3_0w1 = Pos3_0w1(:,3);

X0_5w2 = Pos0_5w2(:,2);
Y0_5w2 = Pos0_5w2(:,3);
X1_0w2 = Pos1_0w2(:,2);
Y1_0w2 = Pos1_0w2(:,3);
X1_5w2 = Pos1_5w2(:,2);
Y1_5w2 = Pos1_5w2(:,3);
X2_0w2 = Pos2_0w2(:,2);
Y2_0w2 = Pos2_0w2(:,3);
X2_5w2 = Pos2_5w2(:,2);
Y2_5w2 = Pos2_5w2(:,3);
X3_0w2 = Pos3_0w2(:,2);
Y3_0w2 = Pos3_0w2(:,3);
% Velocity calculation 1 wave
Velx0_5w1 = diff(X0_5w1)./diff(time0_5w1);
mean_Velx0_5w1 = mean(Velx0_5w1);
std_Velx0_5w1 = std(Velx0_5w1);

Velx1_0w1 = diff(X1_0w1)./diff(time1_0w1);
mean_Velx1_0w1 = mean(Velx1_0w1);
std_Velx1_0w1 = std(Velx1_0w1);

Velx1_5w1 = diff(X1_5w1)./diff(time1_5w1);
mean_Velx1_5w1 = mean(Velx1_5w1);
std_Velx1_5w1 = std(Velx1_5w1);

Velx2_0w1 = diff(X2_0w1)./diff(time2_0w1);
mean_Velx2_0w1 = mean(Velx2_0w1);
std_Velx2_0w1 = std(Velx2_0w1);

Velx2_5w1 = diff(X2_5w1)./diff(time2_5w1);
mean_Velx2_5w1 = mean(Velx2_5w1);
std_Velx2_5w1 = std(Velx2_5w1);

Velx3_0w1 = diff(X3_0w1)./diff(time3_0w1);
mean_Velx3_0w1 = mean(Velx3_0w1);
std_Velx3_0w1 = std(Velx3_0w1);

% Velocity calculation 2 waves
Velx0_5w2 = diff(X0_5w2)./diff(time0_5w2);
mean_Velx0_5w2 = mean(Velx0_5w2);
std_Velx0_5w2 = std(Velx0_5w2);

Velx1_0w2 = diff(X1_0w2)./diff(time1_0w2);
mean_Velx1_0w2 = mean(Velx1_0w2);
std_Velx1_0w2 = std(Velx1_0w2);

Velx1_5w2 = diff(X1_5w2)./diff(time1_5w2);
mean_Velx1_5w2 = mean(Velx1_5w2);
std_Velx1_5w2 = std(Velx1_5w2);

Velx2_0w2 = diff(X2_0w2)./diff(time2_0w2);
mean_Velx2_0w2 = mean(Velx2_0w2);
std_Velx2_0w2 = std(Velx2_0w2);

Velx2_5w2 = diff(X2_5w2)./diff(time2_5w2);
mean_Velx2_5w2 = mean(Velx2_5w2);
std_Velx2_5w2 = std(Velx2_5w2);

Velx3_0w2 = diff(X3_0w2)./diff(time3_0w2);
mean_Velx3_0w2 = mean(Velx3_0w2);
std_Velx3_0w2 = std(Velx3_0w2);

lamda_w1 = 300;
lamda_w2 = 150;
Freq = 0:0.5:3.0;
V_wave_w1 = Freq*lamda_w1;
V_wave_w2 = Freq*lamda_w2;
Vel_w1_mean = [0,mean_Velx0_5w1,mean_Velx1_0w1,mean_Velx1_5w1,mean_Velx2_0w1,mean_Velx2_5w1,mean_Velx3_0w1];
Vel_w1_std = [0,std_Velx0_5w1,std_Velx1_0w1,std_Velx1_5w1,std_Velx2_0w1,std_Velx2_5w1,std_Velx3_0w1];

Vel_w2_mean = [0,mean_Velx0_5w2,mean_Velx1_0w2,mean_Velx1_5w2,mean_Velx2_0w2,mean_Velx2_5w2,mean_Velx3_0w2];
Vel_w2_std = [0,std_Velx0_5w2,std_Velx1_0w2,std_Velx1_5w2,std_Velx2_0w2,std_Velx2_5w2,std_Velx3_0w2];

% Power data for each frequency
Pow_w1_mean = [0,0.2810,0.5859,1.1680,1.7052,2.1525,2.5352];
Pow_w1_std = [0,0.0261,0.0648,0.1088,0.1637,0.2169,0.2578];

Pow_w2_mean = [0,0.2562,0.4723,0.7851,1.2286,1.7289,2.2597];
Pow_w2_std = [0,0.0197,0.0410,0.0572,0.0817,0.1270,0.1657];

% COT calculation
COT_w1_mean = Pow_w1_mean(2:end)./(Vel_w1_mean(2:end)*0.001);
COT_w2_mean = Pow_w2_mean(2:end)./(Vel_w2_mean(2:end)*0.001);

% Pitch angle for each frequency
Pitch_w1_mean = [-1.2,-0.4019,0.1445,0.4597,0.7380,1.237,1.6161];
Pitch_w1_std = [0,0.3776,0.3021,0.3307,0.3993,0.4095,0.6714];

Pitch_w2_mean = [-1.2,-1.1874,-0.7522,-0.0739,0.3596,0.9026,1.2925];
Pitch_w2_std = [0,1.0210,0.2473,0.2716,0.2429,0.3659,0.6138];

% Yaw angle for each frequency
Yaw_w1_mean = [0,0.0015,-1.8776,-0.1970,-2.4921,1.4352,-0.5337];
Yaw_w1_std = [0,0.0238,1.3014,1.9304,0.6491,1.8241,4.0912];

Yaw_w2_mean = [0,-0.1495,0.0042,3.1633,-0.1278,0.7827,1.1802];
Yaw_w2_std = [0,0.0334,0.0063,1.3682,0.2805,1.2372,1.5324];

% Roll angle for each frequency
Roll_w1_mean = [85.5703,85.5286,85.7262,85.8180,85.3804,85.3804,84.3097]-85.5703;
Roll_w1_std = [0,0.6796,0.6830,1.1873,0.8587,0.8587,2.2177];

Roll_w2_mean = [85.5703,85.9704,85.8705,85.7620,85.7551,86.2970,85.5958]-85.5703;
Roll_w2_std = [0,0.6709,0.3350,0.7126,0.7740,0.7725,1.0489];
%% Data without pectoral fins
% Read data of all freqs and waves
Pos0_5w1_pec = csvread('Freq0_5w1_pec');
Pos1_0w1_pec = csvread('Freq1_0w1_pec');
Pos1_5w1_pec = csvread('Freq1_5w1_pec');
Pos2_0w1_pec = csvread('Freq2_0w1_pec');
Pos2_5w1_pec = csvread('Freq2_5w1_pec');
Pos3_0w1_pec = csvread('Freq3_0w1_pec');

Pos0_5w2_pec = csvread('Freq0_5w2_pec');
Pos1_0w2_pec = csvread('Freq1_0w2_pec');
Pos1_5w2_pec = csvread('Freq1_5w2_pec');
Pos2_0w2_pec = csvread('Freq2_0w2_pec');
Pos2_5w2_pec = csvread('Freq2_5w2_pec');
Pos3_0w2_pec = csvread('Freq3_0w2_pec');

% Time and Position
time0_5w1_pec = Pos0_5w1_pec(:,1);
time1_0w1_pec = Pos1_0w1_pec(:,1);
time1_5w1_pec = Pos1_5w1_pec(:,1);
time2_0w1_pec = Pos2_0w1_pec(:,1);
time2_5w1_pec = Pos2_5w1_pec(:,1);
time3_0w1_pec = Pos3_0w1_pec(:,1);

time0_5w2_pec = Pos0_5w2_pec(:,1);
time1_0w2_pec = Pos1_0w2_pec(:,1);
time1_5w2_pec = Pos1_5w2_pec(:,1);
time2_0w2_pec = Pos2_0w2_pec(:,1);
time2_5w2_pec = Pos2_5w2_pec(:,1);
time3_0w2_pec = Pos3_0w2_pec(:,1);

X0_5w1_pec = Pos0_5w1_pec(:,2);
Y0_5w1_pec = Pos0_5w1_pec(:,3);
X1_0w1_pec = Pos1_0w1_pec(:,2);
Y1_0w1_pec = Pos1_0w1_pec(:,3);
X1_5w1_pec = Pos1_5w1_pec(:,2);
Y1_5w1_pec = Pos1_5w1_pec(:,3);
X2_0w1_pec = Pos2_0w1_pec(:,2);
Y2_0w1_pec = Pos2_0w1_pec(:,3);
X2_5w1_pec = Pos2_5w1_pec(:,2);
Y2_5w1_pec = Pos2_5w1_pec(:,3);
X3_0w1_pec = Pos3_0w1_pec(:,2);
Y3_0w1_pec = Pos3_0w1_pec(:,3);

X0_5w2_pec = Pos0_5w2_pec(:,2);
Y0_5w2_pec = Pos0_5w2_pec(:,3);
X1_0w2_pec = Pos1_0w2_pec(:,2);
Y1_0w2_pec = Pos1_0w2_pec(:,3);
X1_5w2_pec = Pos1_5w2_pec(:,2);
Y1_5w2_pec = Pos1_5w2_pec(:,3);
X2_0w2_pec = Pos2_0w2_pec(:,2);
Y2_0w2_pec = Pos2_0w2_pec(:,3);
X2_5w2_pec = Pos2_5w2_pec(:,2);
Y2_5w2_pec = Pos2_5w2_pec(:,3);
X3_0w2_pec = Pos3_0w2_pec(:,2);
Y3_0w2_pec = Pos3_0w2_pec(:,3);
% Velocity calculation 1 wave
Velx0_5w1_pec = diff(X0_5w1_pec)./diff(time0_5w1_pec);
mean_Velx0_5w1_pec = mean(Velx0_5w1_pec);
std_Velx0_5w1_pec = std(Velx0_5w1_pec);

Velx1_0w1_pec = diff(X1_0w1_pec)./diff(time1_0w1_pec);
mean_Velx1_0w1_pec = mean(Velx1_0w1_pec);
std_Velx1_0w1_pec = std(Velx1_0w1_pec);

Velx1_5w1_pec = diff(X1_5w1_pec)./diff(time1_5w1_pec);
mean_Velx1_5w1_pec = mean(Velx1_5w1_pec);
std_Velx1_5w1_pec = std(Velx1_5w1_pec);

Velx2_0w1_pec = diff(X2_0w1_pec)./diff(time2_0w1_pec);
mean_Velx2_0w1_pec = mean(Velx2_0w1_pec);
std_Velx2_0w1_pec = std(Velx2_0w1_pec);

Velx2_5w1_pec = diff(X2_5w1_pec)./diff(time2_5w1_pec);
mean_Velx2_5w1_pec = mean(Velx2_5w1_pec);
std_Velx2_5w1_pec = std(Velx2_5w1_pec);

Velx3_0w1_pec = diff(X3_0w1_pec)./diff(time3_0w1_pec);
mean_Velx3_0w1_pec = mean(Velx3_0w1_pec);
std_Velx3_0w1_pec = std(Velx3_0w1_pec);

% Velocity calculation 2 waves
Velx0_5w2_pec = diff(X0_5w2_pec)./diff(time0_5w2_pec);
mean_Velx0_5w2_pec = mean(Velx0_5w2_pec);
std_Velx0_5w2_pec = std(Velx0_5w2_pec);

Velx1_0w2_pec = diff(X1_0w2_pec)./diff(time1_0w2_pec);
mean_Velx1_0w2_pec = mean(Velx1_0w2_pec);
std_Velx1_0w2_pec = std(Velx1_0w2_pec);

Velx1_5w2_pec = diff(X1_5w2_pec)./diff(time1_5w2_pec);
mean_Velx1_5w2_pec = mean(Velx1_5w2_pec);
std_Velx1_5w2_pec = std(Velx1_5w2_pec);

Velx2_0w2_pec = diff(X2_0w2_pec)./diff(time2_0w2_pec);
mean_Velx2_0w2_pec = mean(Velx2_0w2_pec);
std_Velx2_0w2_pec = std(Velx2_0w2_pec);

Velx2_5w2_pec = diff(X2_5w2_pec)./diff(time2_5w2_pec);
mean_Velx2_5w2_pec = mean(Velx2_5w2_pec);
std_Velx2_5w2_pec = std(Velx2_5w2_pec);

Velx3_0w2_pec = diff(X3_0w2_pec)./diff(time3_0w2_pec);
mean_Velx3_0w2_pec = mean(Velx3_0w2_pec);
std_Velx3_0w2_pec = std(Velx3_0w2_pec);

Vel_w1_mean_pec = [0,mean_Velx0_5w1_pec,mean_Velx1_0w1_pec,mean_Velx1_5w1_pec,mean_Velx2_0w1_pec,mean_Velx2_5w1_pec,mean_Velx3_0w1_pec];
Vel_w1_std_pec = [0,std_Velx0_5w1_pec,std_Velx1_0w1_pec,std_Velx1_5w1_pec,std_Velx2_0w1_pec,std_Velx2_5w1_pec,std_Velx3_0w1_pec];

Vel_w2_mean_pec = [0,mean_Velx0_5w2_pec,mean_Velx1_0w2_pec,mean_Velx1_5w2_pec,mean_Velx2_0w2_pec,mean_Velx2_5w2_pec,mean_Velx3_0w2_pec];
Vel_w2_std_pec = [0,std_Velx0_5w2_pec,std_Velx1_0w2_pec,std_Velx1_5w2_pec,std_Velx2_0w2_pec,std_Velx2_5w2_pec,std_Velx3_0w2_pec];

% Power data for each frequency
Pow_w1_mean_pec = [0,0.2612,0.5844,1.1713,1.7107,2.1293,2.4358];
Pow_w1_std_pec = [0,0.0217,0.0633,0.1092,0.1637,0.1933,0.2420];

Pow_w2_mean_pec = [0,0.2543,0.4654,0.7858,1.2247,1.7229,2.2521];
Pow_w2_std_pec = [0,0.0182,0.0388,0.0575,0.0792,0.1136,0.1646];

% COT calculation
COT_w1_mean_pec = Pow_w1_mean_pec(2:end)./(Vel_w1_mean_pec(2:end)*0.001);
COT_w2_mean_pec = Pow_w2_mean_pec(2:end)./(Vel_w2_mean_pec(2:end)*0.001);

% Pitch angle for each frequency
Pitch_w1_mean_pec = [-1.2,-1.5072,-1.3118,-1.0119,-0.5501,-0.2833,-0.0076];
Pitch_w1_std_pec = [0,0.9733,1.1021,1.0243,0.9508,0.6389,0.4660];

Pitch_w2_mean_pec = [-1.2,-1.8782,-1.4190,-1.2402,-0.9017,-0.6331,-0.2130];
Pitch_w2_std_pec = [0,1.0504,0.8267,1.0134,0.6045,1.1471,0.9173];

% Yaw angle for each frequency
Yaw_w1_mean_pec = [0,0.0609,-0.0967,0.2161,3.6598,-1.4782,-1.5311];
Yaw_w1_std_pec = [0,0.0077,0.0114,0.2871,1.7827,2.9086,2.2700];

Yaw_w2_mean_pec = [0,0.0046,-0.0183,-0.0347,-0.0087,0.3159,3.8770];
Yaw_w2_std_pec = [0,0.0291,0.0124,0.0093,0.0124,0.0167,-0.9963];

% Roll angle for each frequency
Roll_w1_mean_pec = [86.4710,86.2991,86.8773,86.6874,86.6264,86.0998,85.2875]-86.4710;
Roll_w1_std_pec = [0,0.3126,0.4721,0.4982,0.9029,1.2316,1.8288];

Roll_w2_mean_pec = [86.4710,86.7014,86.7245,86.3305,86.6527,86.3139,86.5629]-86.4710;
Roll_w2_std_pec = [0,0.6298,0.4572,0.3294,0.4600,0.5484,1.0064];

%% Plot
%%
fig_props.noYsubplots = 2;
fig_props.noXsubplots = 3;

fig_props.figW = 21;
fig_props.figH = 17;

fig_props.ml = 1.7;
fig_props.mt = 1.8;

create_figure
plotnoX = 1;
plotnoY = 1;
ha1 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 2;
plotnoY = 1;
ha2 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 3;
plotnoY = 1;
ha3 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 1;
plotnoY = 2;
ha4 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 2;
plotnoY = 2;
ha5 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 3;
plotnoY = 2;
ha6 = create_axes(plotnoX,plotnoY,fig_props);

colormap=[255/255, 235/255, 10/255;...  yellow (freq=0)
          255/255, 200/255, 0;...   light yellow (freq=0.5)
          255/255, 128/255, 0/255;... ligth red  (freq=1.0)
          255/255, 0/255, 0/255;...             red         (freq=1.5)
          215/255, 0/255, 0;...       dark red    (freq=2.0)
          164/255, 0, 0;...       darker red  (freq=2.5)
          90/255, 0/255, 0 ];            % blak        (freq=3.0) 
% Velocity plot
axes(ha1); hold on

for i=1:7
Vel_freq = errorbar(Freq(i),Vel_w1_mean(i),Vel_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(i,:);
Vel_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j=1:7
Vel_freq = errorbar(Freq(j),Vel_w2_mean(j),Vel_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(j,:);
Vel_freq.MarkerFaceColor = 'none'; hold on;
end

for i=1:7
Vel_freq_pec = errorbar(Freq(i),Vel_w1_mean_pec(i),Vel_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5); 
Vel_freq_pec.LineWidth = .85;
Vel_freq_pec.MarkerSize = 6.5; 
Vel_freq_pec.MarkerEdgeColor = colormap(i,:);
Vel_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j=1:7
Vel_freq_pec = errorbar(Freq(j),Vel_w2_mean_pec(j),Vel_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5); 
Vel_freq_pec.LineWidth = .85;
Vel_freq_pec.MarkerSize = 6.5; 
Vel_freq_pec.MarkerEdgeColor = colormap(j,:);
Vel_freq_pec.MarkerFaceColor = 'none'; hold on;
end

set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('Mean Thrust, N')
axis([0 3 0 350]);
set(gca,'xtick',0:0.5:3)
set(gca,'xticklabel',0:0.5:3)
set(gca,'ytick',0:50:350)
set(gca,'yticklabel',0:50:350)

axes(ha2); hold on

for i = 1:7
Pow_freq = errorbar(Freq(i),Pow_w1_mean(i),Pow_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Pow_freq.LineWidth = .85;
Pow_freq.MarkerSize = 6; 
Pow_freq.MarkerEdgeColor = colormap(i,:);
Pow_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j =1:7
Pow_freq = errorbar(Freq(j),Pow_w2_mean(j),Pow_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Pow_freq.LineWidth = .85;
Pow_freq.MarkerSize = 6; 
Pow_freq.MarkerEdgeColor = colormap(j,:);
Pow_freq.MarkerFaceColor = 'none'; hold on;
end

for i = 1:7
Pow_freq_pec = errorbar(Freq(i),Pow_w1_mean_pec(i),Pow_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5); 
Pow_freq_pec.LineWidth = .85;
Pow_freq_pec.MarkerSize = 6.5; 
Pow_freq_pec.MarkerEdgeColor = colormap(i,:);
Pow_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j =1:7
Pow_freq_pec = errorbar(Freq(j),Pow_w2_mean_pec(j),Pow_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5); 
Pow_freq_pec.LineWidth = .85;
Pow_freq_pec.MarkerSize = 6.5; 
Pow_freq_pec.MarkerEdgeColor = colormap(j,:);
Pow_freq_pec.MarkerFaceColor = 'none'; hold on;
end
set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('Mean Thrust, N')
axis([0 3 0 3]);
set(gca,'xtick',0:.5:3)
set(gca,'xticklabel',0:.5:3)
set(gca,'ytick',0:0.5:3)
set(gca,'yticklabel',0:0.5:3)
% COT plot
axes(ha3); hold on
% rectangle('Position',[0.75,2,0.5,6],'FaceColor',[183/255,244/255,1],'EdgeColor','none'); hold on;
% rectangle('Position',[1.25,4.5,2.1,5.3],'FaceColor',[193/255,197/255,1],'EdgeColor','none'); hold on;

for i = 1:6
ST_freq = plot(Freq(i+1),COT_w1_mean(i),'o','color',colormap(i+1,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(i+1,:);
ST_freq.MarkerFaceColor = colormap(i+1,:); hold on;
end

for j = 1:6
ST_freq = plot(Freq(j+1),COT_w2_mean(j),'o','color',colormap(j+1,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(j+1,:);
ST_freq.MarkerFaceColor = 'none'; hold on;
end  

for i = 1:6
COT_freq_pec = plot(Freq(i+1),COT_w1_mean_pec(i),'s','color',colormap(i+1,:));
COT_freq_pec.LineWidth = .85;
COT_freq_pec.MarkerSize = 6.5; 
COT_freq_pec.MarkerEdgeColor = colormap(i+1,:);
COT_freq_pec.MarkerFaceColor = colormap(i+1,:); hold on;
end

for j = 1:6
COT_freq_pec = plot(Freq(j+1),COT_w2_mean_pec(j),'s','color',colormap(j+1,:));
COT_freq_pec.LineWidth = .85;
COT_freq_pec.MarkerSize = 6.5; 
COT_freq_pec.MarkerEdgeColor = colormap(j+1,:);
COT_freq_pec.MarkerFaceColor = 'none'; hold on;
end  
set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('COT, 1/N')
axis([0 3 2 10]);
set(gca,'xtick',0:.5:3)
set(gca,'xticklabel',0:.5:3)
set(gca,'ytick',2:2:10)
set(gca,'yticklabel',2:2:10)

%Roll angle plot
axes(ha4); hold on
% rectangle('Position',[0,-2,3.2,4],'FaceColor',[183/255,244/255,1],'EdgeColor','none'); hold on;

for i = 1:7
Roll_freq = errorbar(Freq(i),Roll_w1_mean(i),Roll_w1_std(i),'o','color',colormap(i,:),'CapSize',5);
Roll_freq.LineWidth = .85;
Roll_freq.MarkerSize = 6; 
Roll_freq.MarkerEdgeColor = colormap(i,:);
Roll_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Roll_freq = errorbar(Freq(j),Roll_w2_mean(j),Roll_w2_std(j),'o','color',colormap(j,:),'CapSize',5);
Roll_freq.LineWidth = .85;
Roll_freq.MarkerSize = 6; 
Roll_freq.MarkerEdgeColor = colormap(j,:);
Roll_freq.MarkerFaceColor = 'none'; hold on;
end  

for i = 1:7
Roll_freq_pec = errorbar(Freq(i),Roll_w1_mean_pec(i),Roll_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5);
Roll_freq_pec.LineWidth = .85;
Roll_freq_pec.MarkerSize = 6.5; 
Roll_freq_pec.MarkerEdgeColor = colormap(i,:);
Roll_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Roll_freq_pec = errorbar(Freq(j),Roll_w2_mean_pec(j),Roll_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5);
Roll_freq_pec.LineWidth = .85;
Roll_freq_pec.MarkerSize = 6.5; 
Roll_freq_pec.MarkerEdgeColor = colormap(j,:);
Roll_freq_pec.MarkerFaceColor = 'none'; hold on;
end  
set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('Roll deg, N')
axis([0 3 -6 6]);
set(gca,'xtick',0:.5:3)
set(gca,'xticklabel',0:.5:3)
set(gca,'ytick',-6:2:6)
set(gca,'yticklabel',-6:2:6)
% Pitch angle plot
axes(ha5); hold on

for i = 1:7
Pitch_freq = errorbar(Freq(i),Pitch_w1_mean(i),Pitch_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Pitch_freq.LineWidth = .85;
Pitch_freq.MarkerSize = 6; 
Pitch_freq.MarkerEdgeColor = colormap(i,:);
Pitch_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Pitch_freq = errorbar(Freq(j),Pitch_w2_mean(j),Pitch_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Pitch_freq.LineWidth = .85;
Pitch_freq.MarkerSize = 6; 
Pitch_freq.MarkerEdgeColor = colormap(j,:);
Pitch_freq.MarkerFaceColor = 'none'; hold on;
end

for i = 1:7
Pitch_freq_pec = errorbar(Freq(i),Pitch_w1_mean_pec(i),Pitch_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5); 
Pitch_freq_pec.LineWidth = .85;
Pitch_freq_pec.MarkerSize = 6.5; 
Pitch_freq_pec.MarkerEdgeColor = colormap(i,:);
Pitch_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Pitch_freq_pec = errorbar(Freq(j),Pitch_w2_mean_pec(j),Pitch_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5); 
Pitch_freq_pec.LineWidth = .85;
Pitch_freq_pec.MarkerSize = 6.5; 
Pitch_freq_pec.MarkerEdgeColor = colormap(j,:);
Pitch_freq_pec.MarkerFaceColor = 'none'; hold on;
end
set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('Pitch, deg')
axis([0 3 -3 3]);
set(gca,'xtick',0:.5:3)
set(gca,'xticklabel',0:.5:3)
set(gca,'ytick',-3:1:3)
set(gca,'yticklabel',-3:1:3)
% Yaw angle plot
axes(ha6); hold on
% rectangle('Position',[0,-5,3.2,10],'FaceColor',[183/255,244/255,1],'EdgeColor','none'); hold on;

for i = 1:7
Yaw_freq = errorbar(Freq(i),Yaw_w1_mean(i),Yaw_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Yaw_freq.LineWidth = .85;
Yaw_freq.MarkerSize = 6; 
Yaw_freq.MarkerEdgeColor = colormap(i,:);
Yaw_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Yaw_freq = errorbar(Freq(j),Yaw_w2_mean(j),Yaw_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Yaw_freq.LineWidth = .85;
Yaw_freq.MarkerSize = 6; 
Yaw_freq.MarkerEdgeColor = colormap(j,:);
Yaw_freq.MarkerFaceColor = 'none'; hold on;
end

for i = 1:7
Yaw_freq_pec = errorbar(Freq(i),Yaw_w1_mean_pec(i),Yaw_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5); 
Yaw_freq_pec.LineWidth = .85;
Yaw_freq_pec.MarkerSize = 6.5; 
Yaw_freq_pec.MarkerEdgeColor = colormap(i,:);
Yaw_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
Yaw_freq_pec = errorbar(Freq(j),Yaw_w2_mean_pec(j),Yaw_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5); 
Yaw_freq_pec.LineWidth = .85;
Yaw_freq_pec.MarkerSize = 6.5; 
Yaw_freq_pec.MarkerEdgeColor = colormap(j,:);
Yaw_freq_pec.MarkerFaceColor = 'none'; hold on;
end
set(gca,'FontSize',10)
% xlabel('Frequency, Hz')
% ylabel('Yaw, deg')
axis([0 3 -10 10]);
set(gca,'xtick',0:.5:3)
set(gca,'xticklabel',0:.5:3)
set(gca,'ytick',-10:5:10)
set(gca,'yticklabel',-10:5:10)
hold off
%% saveas(gcf,'forward_pec_results','pdf')
A = 35;Lfin = 0.3;
Vel_lateral = 4*pi*Freq*A;
St_w1 = A*Freq./Vel_w1_mean;
Re_w1 = Vel_w1_mean*Lfin/1e-6;
St_w2 = A*Freq./Vel_w2_mean;
Re_w2 = Vel_w2_mean*Lfin/1e-6;
St_w1_pec = A*Freq./Vel_w1_mean_pec;
Re_w1_pec = Vel_w1_mean_pec*Lfin/1e-6;
St_w2_pec = A*Freq./Vel_w2_mean_pec;
Re_w2_pec = Vel_w2_mean_pec*Lfin/1e-6;

Wave_eff_w1 = Vel_w1_mean./V_wave_w1;
Wave_eff_w2 = Vel_w2_mean./V_wave_w2;

fig_props.noYsubplots = 2;
fig_props.noXsubplots = 3;

fig_props.figW = 21;
fig_props.figH = 16;

fig_props.ml = 1.7;
fig_props.mt = 1.8;

create_figure
plotnoX = 1.5;
plotnoY = 1;
ha7 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 2.5;
plotnoY = 1;
ha8 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 1;
plotnoY = 2;
ha9 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 2;
plotnoY = 2;
ha10 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 3;
plotnoY = 2;
ha11 = create_axes(plotnoX,plotnoY,fig_props);

colormap=[255/255, 235/255, 10/255;...  yellow (freq=0)
          255/255, 200/255, 0;...   light yellow (freq=0.5)
          255/255, 128/255, 0/255;... ligth red  (freq=1.0)
          255/255, 0/255, 0/255;...             red         (freq=1.5)
          215/255, 0/255, 0;...       dark red    (freq=2.0)
          164/255, 0, 0;...       darker red  (freq=2.5)
          90/255, 0/255, 0 ];            % blak        (freq=3.0) 

axes(ha7); hold on

for i=1:7
Vel_freq = errorbar(V_wave_w1(i),Vel_w1_mean(i),Vel_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(i,:);
Vel_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j=1:7
Vel_freq = errorbar(V_wave_w2(j),Vel_w2_mean(j),Vel_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(j,:);
Vel_freq.MarkerFaceColor = 'none'; hold on;
end

set(gca,'FontSize',10)
axis([0 1000 0 350]);
set(gca,'xtick',0:200:1000)
set(gca,'xticklabel',0:200:1000)
set(gca,'ytick',0:50:350)
set(gca,'yticklabel',0:50:350)

axes(ha8); hold on
for i = 2:7
ST_freq = plot(Vel_w1_mean(i),Pow_w1_mean(i),'o','color',colormap(i,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(i,:);
ST_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 2:7
ST_freq = plot(Vel_w2_mean(j),Pow_w2_mean(j),'o','color',colormap(j,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(j,:);
ST_freq.MarkerFaceColor = 'none'; hold on;
end  

set(gca,'FontSize',10)
axis([0 400 0 3]);
set(gca,'xtick',0:100:400)
set(gca,'xticklabel',0:100:400)
set(gca,'ytick',0:0.5:3)
set(gca,'yticklabel',0:0.5:3)

axes(ha9); hold on
for i = 1:7
ST_freq = plot(Vel_w1_mean(i),St_w1(i),'o','color',colormap(i,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(i,:);
ST_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
ST_freq = plot(Vel_w2_mean(j),St_w2(j),'o','color',colormap(j,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(j,:);
ST_freq.MarkerFaceColor = 'none'; hold on;
end  

set(gca,'FontSize',10)
axis([0 400 0 1]);
set(gca,'xtick',0:100:400)
set(gca,'xticklabel',0:100:400)
set(gca,'ytick',0:0.2:1)
set(gca,'yticklabel',0:0.2:1)

axes(ha10); hold on

for i = 1:7
Pow_freq = plot(Vel_w1_mean(i),Wave_eff_w1(i),'o','color',colormap(i,:)); 
Pow_freq.LineWidth = .85;
Pow_freq.MarkerSize = 6; 
Pow_freq.MarkerEdgeColor = colormap(i,:);
Pow_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j =1:7
Pow_freq = plot(Vel_w2_mean(j),Wave_eff_w2(j),'o','color',colormap(j,:)); 
Pow_freq.LineWidth = .85;
Pow_freq.MarkerSize = 6; 
Pow_freq.MarkerEdgeColor = colormap(j,:);
Pow_freq.MarkerFaceColor = 'none'; hold on;
end

set(gca,'FontSize',10)
axis([0 400 0 1]);
set(gca,'xtick',0:100:400)
set(gca,'xticklabel',0:100:400)
set(gca,'ytick',0:0.2:1)
set(gca,'yticklabel',0:0.2:1)

axes(ha11); hold on

for i = 1:6
ST_freq = plot(Vel_w1_mean(i+1),COT_w1_mean(i),'o','color',colormap(i+1,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(i+1,:);
ST_freq.MarkerFaceColor = colormap(i+1,:); hold on;
end

for j = 1:6
ST_freq = plot(Vel_w2_mean(j+1),COT_w2_mean(j),'o','color',colormap(j+1,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(j+1,:);
ST_freq.MarkerFaceColor = 'none'; hold on;
end  

set(gca,'FontSize',10)
axis([0 400 2 10]);
set(gca,'xtick',0:100:400)
set(gca,'xticklabel',0:100:400)
set(gca,'ytick',2:2:10)
set(gca,'yticklabel',2:2:10)

% saveas(gcf,'forward_wavespeed_v1','pdf')
% %%
%%
fig_props.noYsubplots = 1;
fig_props.noXsubplots = 2;

fig_props.figW = 17;
fig_props.figH = 9;

fig_props.ml = 1.7;
fig_props.mt = 1.8;

create_figure
plotnoX = 1;
plotnoY = 1;
ha12 = create_axes(plotnoX,plotnoY,fig_props);

plotnoX = 2;
plotnoY = 1;
ha13 = create_axes(plotnoX,plotnoY,fig_props);

axes(ha12); hold on
for i=1:7
Vel_freq = errorbar(V_wave_w1(i),Vel_w1_mean(i),Vel_w1_std(i),'o','color',colormap(i,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(i,:);
Vel_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j=1:7
Vel_freq = errorbar(V_wave_w2(j),Vel_w2_mean(j),Vel_w2_std(j),'o','color',colormap(j,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6; 
Vel_freq.MarkerEdgeColor = colormap(j,:);
Vel_freq.MarkerFaceColor = 'none'; hold on;
end

for i=1:7
Vel_freq_pec = errorbar(V_wave_w1(i),Vel_w1_mean_pec(i),Vel_w1_std_pec(i),'s','color',colormap(i,:),'CapSize',5); 
Vel_freq_pec.LineWidth = .85;
Vel_freq_pec.MarkerSize = 6.5; 
Vel_freq_pec.MarkerEdgeColor = colormap(i,:);
Vel_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j=1:7
Vel_freq = errorbar(V_wave_w2(j),Vel_w2_mean_pec(j),Vel_w2_std_pec(j),'s','color',colormap(j,:),'CapSize',5); 
Vel_freq.LineWidth = .85;
Vel_freq.MarkerSize = 6.5; 
Vel_freq.MarkerEdgeColor = colormap(j,:);
Vel_freq.MarkerFaceColor = 'none'; hold on;
end

set(gca,'FontSize',10)
axis([0 1000 0 350]);
set(gca,'xtick',0:200:1000)
set(gca,'xticklabel',0:200:1000)
set(gca,'ytick',0:50:350)
set(gca,'yticklabel',0:50:350)

axes(ha13); hold on
for i = 1:7
ST_freq = plot(Vel_w1_mean(i),St_w1(i),'o','color',colormap(i,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(i,:);
ST_freq.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
ST_freq = plot(Vel_w2_mean(j),St_w2(j),'o','color',colormap(j,:));
ST_freq.LineWidth = .85;
ST_freq.MarkerSize = 6; 
ST_freq.MarkerEdgeColor = colormap(j,:);
ST_freq.MarkerFaceColor = 'none'; hold on;
end  

for i = 1:7
ST_freq_pec = plot(Vel_w1_mean_pec(i),St_w1_pec(i),'s','color',colormap(i,:));
ST_freq_pec.LineWidth = .85;
ST_freq_pec.MarkerSize = 6.5; 
ST_freq_pec.MarkerEdgeColor = colormap(i,:);
ST_freq_pec.MarkerFaceColor = colormap(i,:); hold on;
end

for j = 1:7
ST_freq_pec = plot(Vel_w2_mean_pec(j),St_w2_pec(j),'s','color',colormap(j,:));
ST_freq_pec.LineWidth = .85;
ST_freq_pec.MarkerSize = 6.5; 
ST_freq_pec.MarkerEdgeColor = colormap(j,:);
ST_freq_pec.MarkerFaceColor = 'none'; hold on;
end  

set(gca,'FontSize',10)
axis([0 400 0 1]);
set(gca,'xtick',0:100:400)
set(gca,'xticklabel',0:100:400)
set(gca,'ytick',0:0.2:1)
set(gca,'yticklabel',0:0.2:1)
hold off

% saveas(gcf,'pec_vel_st','pdf');