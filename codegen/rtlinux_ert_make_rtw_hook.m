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

            [stWSL, ~] = system('wsl --status');
            if stWSL ~= 0
                error('WSL is not running or installed. Please install WSL2.');
            end
            
            [stSSH, ~] = system('wsl -e bash -c "command -v sshpass"');
            if stSSH ~= 0
                warning('sshpass not found in WSL. Deployment will fail. Run: wsl -e sudo apt-get install sshpass');
            end

            if ~ispref('RTLinux', 'Config')
                error('Target not configured. Please run install.m first.');
            end
            prefs = getpref('RTLinux', 'Config');
            sdkEnv = prefs.YoctoSDKPath;
            camWrapperWin = prefs.CameraWrapperDir;

            projWin  = pwd;
            mRootWin  = matlabroot;
            spRootWin = matlabshared.supportpkg.getSupportPackageRoot;

            projWSL       = toWSL(projWin);
            mRootWSL      = toWSL(mRootWin);
            spRootWSL     = toWSL(spRootWin);
            camWrapperWSL = toWSL(camWrapperWin);

            buildWSL = sprintf('$HOME/wsl_builds/%s_build', modelName);

            cmakeCmd = sprintf('cmake -S "$proj" -B . -DMATLAB_ROOT_WSL="%s" -DSP_ROOT_WSL="%s" -DCAMERA_WRAPPER_WSL="%s"', ...
                               mRootWSL, spRootWSL, camWrapperWSL);

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

            try
                deleteFolderIfExists(fullfile(pwd, 'slprj'));
            catch
                warning("Cleanup error");
            end

            disp('### Building in WSL (Yocto SDK Sourced)...');

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

            IP_Address = strtrim(get_param(modelName, 'LINUX_IP_Address'));
            Username   = strtrim(get_param(modelName, 'LINUX_Username')); 
            Password   = strtrim(get_param(modelName, 'LINUX_Password')); 
            Path       = strtrim(get_param(modelName, 'LINUX_Path'));
            tfliteRaw  = get_param(modelName, 'TFLITE_Enable');
            coresRaw   = get_param(modelName, 'Linux_CoreIsolation');

            coresStr = regexprep(char(string(coresRaw)), '[\[\]]', '');
            cores = regexprep(strtrim(coresStr), '[\s,]+', ',');
            tfliteEnable = any(strcmpi(string(tfliteRaw), ["on", "true", "1"]));

            sshFlags = '-n -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null';
            scpFlags = '-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null';
            
            cmds = {
                'set -ex' 
                'echo "1. Stopping old process and Uploading executable..."'

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

            cmds{end+1} = 'echo "2. Launching via Pi-side script..."';
            launchCmd = sprintf('echo "%s" | sudo -S /usr/bin/run_model.sh "%s" "%s" "rt_main"', ...
                                Password, cores, Path);
            
            cmds{end+1} = sprintf('sshpass -p "%s" ssh %s %s@%s "%s"', Password, sshFlags, Username, IP_Address, launchCmd);
            cmds{end+1} = sprintf('echo "### Success! App is running on cores: %s"', cores);

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
    winPath = strrep(winPath, '"', '');
    drive = lower(winPath(1));
    p = ['/mnt/' drive strrep(winPath(3:end), '\', '/')];
end

function deleteFolderIfExists(folderPath)
    if isfolder(folderPath)
        rmdir(folderPath, 's');
        fprintf("Deleted: %s\n", folderPath);
    end
end