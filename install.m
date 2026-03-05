% install.m
function install()
    disp('### Installing Real-Time Linux (rtlinux_ert) Custom Target...');  

    % 1. Add and save MATLAB paths
    currDir = pwd;
    addpath(genpath(fullfile(currDir, 'blocks')));
    addpath(genpath(fullfile(currDir, 'codegen')));
    
    if savepath() == 0
        disp(' - Paths saved successfully.');
    else
        warning(' - Could not save path. You may need to run MATLAB as Administrator.');
    end

    % 2. Prompt user for Yocto SDK Path
    disp(' ');
    disp('To cross-compile, we need the path to your Yocto SDK environment script in WSL.');
    disp('Example: /home/username/yocto-sdk/environment-setup-cortexa76-poky-linux');
    
    defaultSdk = '/home/user/yocto-sdk/environment-setup-cortexa76-poky-linux';
    sdkPath = input(['Enter Yocto SDK Path [' defaultSdk ']: '], 's');
    
    if isempty(sdkPath)
        sdkPath = defaultSdk;
    end

    % 3. Save to MATLAB preferences
    prefs.YoctoSDKPath = sdkPath;
    prefs.CameraWrapperDir = fullfile(currDir, 'codegen', 'camera_wrapper');
    
    setpref('RTLinux', 'Config', prefs); % Updated namespace
    disp(' - Configuration saved to MATLAB preferences.');

    sl_refresh_customizations;
    disp('### Installation Complete! You can now select Real-Time Linux (WSL/Yocto) in Simulink.');
end