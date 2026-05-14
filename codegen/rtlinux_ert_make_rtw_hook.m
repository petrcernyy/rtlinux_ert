function rtlinux_ert_make_rtw_hook(hookMethod, modelName, ~, ~, ~, ~)
    % RTLINUX_MAKE_RTW_HOOK
    % Handles compilation in WSL and deployment to a generic Linux/Raspberry Pi target.

    switch hookMethod
        case 'error'
            % No action needed on error cleanup currently
        case 'entrydo'
            disp(['### Starting build procedure for model: ', modelName]);
        case {'before_tlc', 'after_tlc'}
            % No action needed around TLC phase
            
        case 'before_make'
            disp('### Preparing WSL Build Environment...');
            
            % --- 1. GUARDRAILS ---
            [stWSL, ~] = system('wsl --status');
            if stWSL ~= 0
                error('WSL is not running or installed. Please install WSL2.');
            end
            
            [stSSH, ~] = system('wsl -e bash -c "command -v sshpass"');
            if stSSH ~= 0
                warning('sshpass not found in WSL. Deployment will fail. Run: wsl -e sudo apt-get install sshpass');
            end

            % --- 2. LOAD USER PREFERENCES ---
            if ~ispref('RTLinux', 'Config')
                error('Target not configured. Please run install.m first.');
            end
            prefs = getpref('RTLinux', 'Config');
            sdkEnv = prefs.YoctoSDKPath;
            camWrapperWin = prefs.CameraWrapperDir;
            
            % --- 3. SETUP DYNAMIC PATHS ---
            projWin  = pwd;
            mRootWin  = matlabroot;
            spRootWin = matlabshared.supportpkg.getSupportPackageRoot;
            
            % Convert to WSL Formats
            projWSL       = toWSL(projWin);
            mRootWSL      = toWSL(mRootWin);
            spRootWSL     = toWSL(spRootWin);
            camWrapperWSL = toWSL(camWrapperWin);
            
            % LET BASH HANDLE THE HOME DIRECTORY TO AVOID WSL WARNING INJECTIONS
            buildWSL = sprintf('$HOME/wsl_builds/%s_build', modelName);

            % Create the dynamic CMake command
            cmakeCmd = sprintf('cmake -S "$proj" -B . -DMATLAB_ROOT_WSL="%s" -DSP_ROOT_WSL="%s" -DCAMERA_WRAPPER_WSL="%s"', ...
                               mRootWSL, spRootWSL, camWrapperWSL);

            % --- 4. CONSTRUCT BUILD COMMANDS ---
            cmds = {
                'set -e'
                sprintf('bld="%s"', buildWSL)
                sprintf('proj="%s"', projWSL)
                'rm -rf "$bld" && mkdir -p "$bld"'
                sprintf('source %s', sdkEnv)
                'cd "$bld"'
                cmakeCmd
                'cmake --build . -j'
                'mkdir -p "$proj/build"'
                'cp -f "$bld/rt_main" "$proj/build/rt_main"'
            };

            % Cleanup Local Folders
            try
                deleteFolderIfExists(fullfile(pwd, 'slprj'));
            catch ME
                warning("Cleanup error: %s", ME.message);
            end

            % --- 5. EXECUTE BUILD IN WSL (BASE64 TRICK) ---
            disp('### Building in WSL (Yocto SDK Sourced)...');
            
            % Join commands with newlines and encode to Base64
            bashScript = strjoin(cmds, char(10));
            b64Script = char(matlab.net.base64encode(bashScript));
            wslCmd = sprintf('wsl --cd ~ -e bash -c "echo %s | base64 -d | bash"', b64Script);
            
            st = system(wslCmd);
            if st ~= 0
                error('Build failed in WSL. See log above.');
            end

        case 'after_make'
            disp('### Deployment Sequence (Using Pi-side run script)...');
            outFile = fullfile(pwd, 'build', 'rt_main');
            
            if ~isfile(outFile)
                warning('Artifact not found: %s. Skipping deployment.', outFile);
                return;
            end

            % --- 1. PARSE PARAMETERS ---
            IP_Address = strtrim(get_param(modelName, 'LINUX_IP_Address'));
            Username   = strtrim(get_param(modelName, 'LINUX_Username')); 
            Password   = strtrim(get_param(modelName, 'LINUX_Password')); 
            Path       = strtrim(get_param(modelName, 'LINUX_Path'));
            tfliteRaw  = get_param(modelName, 'TFLITE_Enable');
            coresRaw   = get_param(modelName, 'Linux_CoreIsolation');

            coresStr = regexprep(char(string(coresRaw)), '[\[\]]', '');
            cores = regexprep(strtrim(coresStr), '[\s,]+', ',');
            tfliteEnable = any(strcmpi(string(tfliteRaw), ["on", "true", "1"]));

            % --- 2. CONSTRUCT DEPLOYMENT COMMANDS ---
            sshFlags = '-n -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null';
            scpFlags = '-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null';
            
            cmds = {
                'set -ex' 
                'echo "1. Stopping old process and Uploading executable..."'
                
                % NEW LINE: Kill the old process first. "|| true" ensures the script doesn't fail if the app isn't running yet.
                sprintf('sshpass -p "%s" ssh %s %s@%s "echo \\"%s\\" | sudo -S pkill -9 rt_main || true"', Password, sshFlags, Username, IP_Address, Password)
                
                sprintf('sshpass -p "%s" ssh %s %s@%s "mkdir -p %s"', Password, sshFlags, Username, IP_Address, Path)
                sprintf('sshpass -p "%s" scp %s "%s" %s@%s:%s/rt_main', Password, scpFlags, toWSL(outFile), Username, IP_Address, Path)
                sprintf('sshpass -p "%s" ssh %s %s@%s "chmod +x %s/rt_main"', Password, sshFlags, Username, IP_Address, Path)
            };

            if tfliteEnable
                modelFile = fullfile(fileparts(pwd), get_param(modelName, 'TF_ModelName'));
                modelNameOnly = get_param(modelName, 'TF_ModelName');
                cmds{end+1} = sprintf('sshpass -p "%s" scp %s "%s" %s@%s:%s/%s', Password, scpFlags, toWSL(modelFile), Username, IP_Address, Path, modelNameOnly);
            end

            % --- 3. THE CLEAN LAUNCH ---
            % Instead of a long string, we just call the script you baked into Yocto
            % Syntax: /usr/bin/run_model.sh <cores> <app_working_dir> <app_binary_name>
            cmds{end+1} = 'echo "2. Launching via Pi-side script..."';
            launchCmd = sprintf('echo "%s" | sudo -S /usr/bin/run_model.sh "%s" "%s" "rt_main"', ...
                                Password, cores, Path);
            
            cmds{end+1} = sprintf('sshpass -p "%s" ssh %s %s@%s "%s"', Password, sshFlags, Username, IP_Address, launchCmd);
            cmds{end+1} = sprintf('echo "### Success! App is running on cores: %s"', cores);
            
            % --- 4. EXECUTE VIA WSL ---
            bashScript = strjoin(cmds, char(10));
            b64Script = char(matlab.net.base64encode(bashScript));
            wslCmd = sprintf('wsl --cd ~ -e bash -c "echo %s | base64 -d | bash -ex"', b64Script);
            
            runStatus = system(wslCmd);
            if runStatus ~= 0, error('Deployment failed.'); end

        case 'exit'
            disp(['### Successful completion of build procedure for model: ', modelName]);
    end
end

% -------- Helpers --------
function p = toWSL(winPath)
    % Convert Windows path to WSL path format (e.g., C:\dir -> /mnt/c/dir)
    winPath = strrep(winPath, '"', ''); % Strip quotes
    drive = lower(winPath(1));
    p = ['/mnt/' drive strrep(winPath(3:end), '\', '/')];
end

function deleteFolderIfExists(folderPath)
    if isfolder(folderPath)
        rmdir(folderPath, 's');
        fprintf("Deleted: %s\n", folderPath);
    end
end