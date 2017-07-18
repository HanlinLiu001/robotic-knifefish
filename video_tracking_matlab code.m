%% Video processing of tracking the fish
close all
clear; clc;
vidObj = VideoReader('DSC_0335**.MOV');
vidHeight = get(vidObj,'Height');
vidWidth = get(vidObj,'Width');

k = 1;
while hasFrame(vidObj)
    s(k).cdata = readFrame(vidObj);
    k = k+1;
end

fps = get(vidObj,'FrameRate'); %frames per second 
dt = 1/fps;               % time between two frames
nFrames = size(s,2); %number of frames
%% show selected frames
pix2real = 462.3/776;     % pixel to real distance conversion
for i = 162:291
    I = s(i).cdata;
    cla;
    image(I);daspect([1 1 1])
    pause(.05);
end
%%
ii = 1;
level = 0.42;
for i = 165:291
    Im = s(i).cdata;        % load rgb frame
    Ig = rgb2gray(Im);      % change to gray
    bw = imbinarize(Ig,level); % change to black and white
    bw = ~bw;                  % invert black to white
     
    bw(1:420,:) = 0;           % blackout the upper part
    bw(840:end,:) = 0;         % blackout the lower part, with only the fin and the plug left
    
    bw = bwareaopen(bw, 200);  % remove small parts less than 200 pixels
    bw = imfill(bw,'holes');
    [B,L] = bwboundaries(bw,'noholes');
    stats = regionprops(L,'Area','Centroid','Extrema');
    xy1_pix(ii,:) = stats(1).Centroid;
    xy2_pix(ii,:) = stats(2).Centroid;
    finL(ii,:) = stats(2).Extrema(8,:);    % Fin edge left side coordinate
    finR(ii,:) = stats(2).Extrema(3,:);    % Fin edge right side coordinate
    pitch(ii) = atan2d(finR(ii,2) - finL(ii,2),finR(ii,1) - finL(ii,1));
    
    time(ii) = (ii-1)*dt;
    ii = ii+1;
end
xy1 = xy1_pix*pix2real;
xy2 = xy2_pix*pix2real;
for j = 1:ii-2
    V1x(j) = (xy1(j+1,1)-xy1(j,1))/dt;
    V1y(j) = (xy1(j+1,2)-xy1(j,2))/dt;
    V2x(j) = (xy2(j+1,1)-xy2(j,1))/dt;
    V2y(j) = (xy2(j+1,2)-xy2(j,2))/dt;
end

ToCGX = 441.0313;
ToCGY = -13.547; % transer the velocity point to the CG
% imwrite(s(165).cdata, 'Frame165.tif');
% 
% imwrite(s(226).cdata, 'Frame226.tif');
% Im2 = image(s(283).cdata);
% axis off; daspect([1 1 1]);
% Alpha = zeros(vidHeight,vidWidth);
% for m = 1350:1919
%     for n = 207:675
%     Alpha(n,m) = 1;
%     end
% end
% Im2.AlphaData = Alpha;

%% Quiver plot
imshow('FrameCom.tif','Border','loose');
hold on 
Velq = quiverwcolorbar2(xy1_pix(1:10:121,1)+ToCGX,xy1_pix(1:10:121,2)+ToCGY,V1x(1:10:121)',V1y(1:10:121)',1,'bounds',[0 350]);

figure(3)
plot(time,xy1(:,1),'LineStyle','-','LineWidth',2,'Color',[255/255, 128/255, 0/255]); hold on
% plot(time,xy2(:,1),'xb');
mean(V1x)
std(V1x)
