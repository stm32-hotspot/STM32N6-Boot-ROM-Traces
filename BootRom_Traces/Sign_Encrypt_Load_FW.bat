@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Keys folder
set projectdir=%~dp0
pushd "%projectdir%"\..\BootROM\Keys
set keys_pem_dir=%cd%
set trusted_binary_dir=%projectdir%\Binary
set binary_dir=%projectdir%\..\..\Templates\Template_BootRom_traces\EWARM\FSBL\Template_BootRom_traces\Exe
popd


set auth_key_pair0_pem=%keys_pem_dir%\publicKey00.pem
set auth_key_pair1_pem=%keys_pem_dir%\publicKey01.pem
set auth_key_pair2_pem=%keys_pem_dir%\publicKey02.pem
set auth_key_pair3_pem=%keys_pem_dir%\publicKey03.pem
set auth_key_pair4_pem=%keys_pem_dir%\publicKey04.pem
set auth_key_pair5_pem=%keys_pem_dir%\publicKey05.pem
set auth_key_pair6_pem=%keys_pem_dir%\publicKey06.pem
set auth_key_pair7_pem=%keys_pem_dir%\publicKey07.pem
set auth_priv_key=%keys_pem_dir%\privateKey00.pem

set binary_file=%binary_dir%\Template_BootRom_traces.bin
set trusted_file=%trusted_binary_dir%\Template_signed.bin
set OEM_secret_file=%keys_pem_dir%\OEM_secret.bin
set second_trusted_file=%trusted_binary_dir%\Template_wH.bin

set connect_no_reset=-c port=SWD ap=1 speed=fast mode=Hotplug

:: Sign ans encrypt FSBL
set command="%stm32signingtoolcli%" -bin "%binary_file%" -pubk "%auth_key_pair0_pem%" "%auth_key_pair1_pem%" "%auth_key_pair2_pem%" "%auth_key_pair3_pem%" "%auth_key_pair4_pem%"  "%auth_key_pair5_pem%" "%auth_key_pair6_pem%" "%auth_key_pair7_pem%" -prvk "%auth_priv_key%" -pwd rot1 -iv 0x00000000 -of 0x00000003 -la 0x34180000 -t fsbl -o "%trusted_file%" -hv 2.3 -dump "%trusted_file%" -s
!command!
IF !errorlevel! NEQ 0 goto :error

:: Add a Header to FSBL that will be flashed at 0x70040000/ do not sign or encrypt
set command="%stm32signingtoolcli%" -bin "%binary_file%" -nk -iv 0x00000000 -of 0x80000000 -la 0x34180000 -t fsbl -o "%second_trusted_file%" -hv 2.3 -dump "%second_trusted_file%" -s
!command!
IF !errorlevel! NEQ 0 goto :error

::load trusted firmware
set command="%stm32programmercli%" %connect_no_reset% -w "%trusted_file%" 0x70000000 -el "%stm32ExtLoaderFlash%"
!command!
IF !errorlevel! NEQ 0 goto :error

::Load Second firmware
set command="%stm32programmercli%" %connect_no_reset% -w "%second_trusted_file%" 0x70040000 -el "%stm32ExtLoaderFlash%"
!command!
IF !errorlevel! NEQ 0 goto :error

cmd /k
exit 0

:error
echo Failed 
echo "%command% failed"
cmd /k
exit 1