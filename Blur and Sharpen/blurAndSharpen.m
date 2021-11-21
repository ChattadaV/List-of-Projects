img = imread('zoltan.jpg'); % load the image

img = double(img); % convert the image from uint8 to double class type

imshow(img,[]); % output the image; brackets are used so the image (double array) is not just a white square

blurredImg = blur(img); % calling 'blur' function

figure; % create a figure
imshow(blurredImg, []); % load the blurred image

sharpenedImg = img - (blurredImg .* 0.5); % equation to sharpen the image

figure; % create a figure
imshow(sharpenedImg, []); % load the sharpened image