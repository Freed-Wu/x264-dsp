#!/usr/bin/env octave
% a = -0.5
pkg load image;
% matlab is column-major order
img = transpose(reshape(0:15, 4, 4));
disp(imresize(img, 0.5, 'bilinear', 'AntiAliasing', false));
% 2.5000    4.5000
% 10.5000   12.5000
disp(imresize(img, 0.5, 'bicubic', 'AntiAliasing', false));
% 2.1875    4.3125
% 10.6875   12.8125
