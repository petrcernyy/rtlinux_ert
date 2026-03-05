function rtlinux_select_callback_handler(hDlg, hSrc)
  % The target is model reference compliant
  slConfigUISetVal(hDlg, hSrc, 'ModelReferenceCompliant', 'on');
  slConfigUISetEnabled(hDlg, hSrc, 'ModelReferenceCompliant', false);

  % Hardware being used is the production hardware
  slConfigUISetVal(hDlg, hSrc, 'ProdEqTarget', 'on');

  % Setup C++ as default language
  slConfigUISetVal(hDlg, hSrc, 'TargetLang', 'C');

  % Setup the hardware configuration
  slConfigUISetVal(hDlg, hSrc, 'ProdHWDeviceType', 'ARM Compatible->ARM Cortex-A (64-bit)');


  % Set the TargetLibSuffix
  slConfigUISetVal(hDlg, hSrc, 'TargetLibSuffix', '.a');

  % For real-time builds, we must generate ert_main.c
  slConfigUISetVal(hDlg, hSrc, 'ERTCustomFileTemplate', 'rtlinux_file_process.tlc');
  slConfigUISetVal(hDlg, hSrc, 'CustomSource', '');

  % switch on dialog to be able to configure custom step/init function names
  slConfigUISetVal(hDlg,hSrc,'ModelStepFunctionPrototypeControlCompliant','on');
  slConfigUISetEnabled(hDlg,hSrc,'ModelStepFunctionPrototypeControlCompliant',false);


  slConfigUISetVal(hDlg,hSrc,'GenerateReport','off');

  slConfigUISetVal(hDlg, hSrc, 'GenCodeOnly', 'off');
  slConfigUISetVal(hDlg, hSrc, 'GenerateMakefile', 'off');

  slConfigUISetVal(hDlg, hSrc, 'ExtMode', 'off');

  % set default solver config
  slConfigUISetVal(hDlg, hSrc, 'SolverType', 'Fixed-step');
  slConfigUISetVal(hDlg, hSrc, 'Solver', 'FixedStepDiscrete');
  slConfigUISetVal(hDlg, hSrc, 'EnableMultiTasking', 'off');
end