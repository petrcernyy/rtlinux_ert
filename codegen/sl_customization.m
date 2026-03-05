function sl_customization(cm)
% SL_CUSTOMIZATION for RPI connectivity config

cm.registerTargetInfo(@loc_createSerialConfig);
cm.ExtModeTransports.add('rtlinux_ert.tlc', 'MyTCP', 'ext_comm', 'Level1');

% local function
function config = loc_createSerialConfig

config = rtw.connectivity.ConfigRegistry;
config.ConfigName = 'LINUX serial';
config.ConfigClass = 'RPI_ert.ConnectivityConfig';

% matching system target file
config.SystemTargetFile = {'rtlinux_ert.tlc'};

config.TemplateMakefile = {'rtlinux_ert.tmf'};

% match any hardware implementation
config.TargetHWDeviceType = {};
