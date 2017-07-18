% test load cell

clear all

% Mass on LoadCell
Trial = 34;
Fre = 0;

sample_freq = 1000;                              % Frequency, Hz
duration = 7;                             % Duration of recording, s

Nsamples = sample_freq*duration;                % Number of samples


AI = daq.createSession ('ni');

AI.addAnalogInputChannel('cDAQ1Mod2',[0 8 1 2 5], 'Voltage');       % Create device object
%AI.addCounterInputChannel('Dev1',0,'EdgeCount');

%addAnalogInputChannel(AI(1),[17]);              % Add channels to analog input object

AI.Rate = sample_freq;                 % Set sampling frequency, Hz
AI.NumberOfScans=Nsamples;
%%Channels
AI.Channels(1).TerminalConfig=['SingleEnded']; % Encoder data
AI.Channels(2).TerminalConfig=['SingleEnded']; % Force data
AI.Channels(3).TerminalConfig=['SingleEnded']; % neg. motor ( post resistor)
AI.Channels(4).TerminalConfig=['SingleEnded']; % neg. resistor (ground)
AI.Channels(5).TerminalConfig=['SingleEnded']; % post. motor 
%AI.Channels(6).TerminalConfig=['Differential'];
%AI_InputType = 'Differential';        % Input type

% Set channel voltage limits
AI.Channels(1).Range = [-5 5];
AI.Channels(2).Range = [-5 5];
AI.Channels(3).Range = [-10 10];
AI.Channels(4).Range = [-10 10];
AI.Channels(5).Range = [-10 10];
%AI.Channels(6).Range = [-10 10];


%AI.resetCounters();
%AI.DurationInSeconds = 0.25;

% Power the Encoder with 5V


% Acquire data
data = AI.startForeground; 

%start(AI)
wait(AI,duration + 3)  % Add 2 seconds to complete 


%data = getdata(AI);
% Clean up
%delete(AI)
%clear AI

Rest = 0.1;    % Resistor, ohms
encd=data(:,1);   % Encoder, volts
Fvolt=data(:,2);  % Force, volts
Volt_motor= data(:,5)-data(:,3);
I_motor = (data(:,3)-data(:,4))/Rest;

% Low pass filter
filter_cutoff = 30; % Hz
run.sample_rate=sample_freq;
Wn=filter_cutoff/(run.sample_rate/2);
[b,a]=butter(2,Wn);

filt_Fvolt = filtfilt(b,a,Fvolt);
filt_Volt_motor=filtfilt(b,a,Volt_motor);
filt_I_motor=filtfilt(b,a,I_motor);
%%
Power = filt_Volt_motor.*filt_I_motor; 
Volt_motor_mean=mean(filt_Volt_motor);
Power_mean = mean(Power);


%% Saving
Fv_mean=mean(Fvolt)
FileName=['./data/Sequence',num2str(Trial,'%03d'),'.mat'];

save(FileName,'Fre','Fvolt','encd','filt_Fvolt','sample_freq','duration','Fv_mean',...
'Power','filt_Fvolt','filt_Volt_motor','filt_I_motor','Volt_motor','Power_mean',...
'Volt_motor_mean');

%% Plot
figure(1)
plot(Fvolt,'g'); hold on
plot(filt_Fvolt,'r')
xlabel('N sample')
ylabel('Force volts')
%%
figure(2)
plot(encd)
xlabel('N samples')
ylabel('Encoder')

figure(3)
plot(filt_Volt_motor); hold on;
plot(filt_I_motor,'r')
xlabel('N samples')
legend('Volts', 'Current')
