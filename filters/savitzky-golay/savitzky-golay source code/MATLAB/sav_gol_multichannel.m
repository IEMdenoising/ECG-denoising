function filtered_signal = sav_gol_multichannel (raw_signal, filter_type, ...
    window_width)
    %% Filter input signal with Savitzky-Golay filter

    % Parameters:
    % ---------------
    % raw_signal:       2D (or 1D) array, double
    %                   raw ECG signal from a few (at least one) channels
    % filter_type:      strictly specified uint: 1 or 2
    %                   1 - low-pass filter
    %                   2 - high-pass filter
    % window_width:     uint
    %                   width of filtration window
    %
    % Returns:
    % ---------------
    % filtered_signal:  2D (od 1D) array, double
    %                   filtered ECG signals returned in the same number of
    %                   channels as input signal

    %% Input data verification
   
    if (size(raw_signal,1)<1 || size(raw_signal,2)<1)
        error('Input signal must be non-empty array.')
    end
        
    if (filter_type ~= 1 && filter_type ~= 2)
        error('Incorrect filter type. Choose 1 for low-pass and 2 for high-pass filter.')
    end

    if (floor(window_width) ~= window_width || window_width <= 0)     
        error('Window width must be positive integer')
    end
    
    % window width must be odd value
    if (rem(window_width, 2) == 0)
        window_width = window_width + 1;
    end    
    
    %% calculate width of left/right artificial band
    bnd = floor(window_width / 2);
    
    %% iterate every channel
    for chan = 1 : size(raw_signal, 2)
        
        % expand signal
        left_side = repmat(raw_signal(1, chan), bnd, 1); 
        right_side = repmat(raw_signal(end, chan), bnd, 1);
        expanded_sig = [left_side; raw_signal(:, chan); right_side]; 
  
        
        for i=1:length(raw_signal(:,chan))

            % interpolation coefficients       
            coeffs = polyfit((i:i+window_width-1)', ...
                expanded_sig(i:i+window_width-1), 3);

            % output signal for every sample        
            filtered_signal(i, chan) = coeffs(1) * (i+bnd) ^3 + ...
                coeffs(2) * (i+bnd) ^2 + ...
                coeffs(3) * (i+bnd) + coeffs(4);
        end

        filtered_signal(:,chan) = filtered_signal(:,chan)';
    end
    
    %% in case of high-pass filter

    if filter_type == 2
        filtered_signal = raw_signal - filtered_signal;
    end
    
end


