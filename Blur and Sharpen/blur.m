function out = blur(in) % function to blur an image

    if ~(isa(in, 'double')) %check to see if the class of input is double
        error('The data input must be a 2D Double Array') % return error if it's not double
    end
    
    out = in * 0; % pre-allocating the output with zeros
    w = 5; % set the parameter to 5

    for i = w + 1:size(in, 1) - w % for-loop for 1st dimension of input excluding the first and last 5 columns
        for j = w + 1:size(in, 2) - w % for-loop for 2nd dimension of input excluding the first and last 5 rows
            
            window = in(i-w:i+w, j-w:j+w); % window of x and y (5 to the left, right, up, and down)
            
            out(i, j) = mean(mean(window)); % output is the average of rows and columns of window

        end
    end     
end