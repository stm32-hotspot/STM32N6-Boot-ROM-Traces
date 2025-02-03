@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Keys folder
set projectdir=%~dp0
pushd "%projectdir%"\..\BootROM\Keys
set keys_pem_dir=%cd%
set binary_dir=%projectdir%\NOR_Binary
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

set FSBL_binary_file=%binary_dir%\binary.bin
set FSBL_trusted_file=%binary_dir%\binary_trusted.bin

set OpenBL_binary_file=%binary_dir%\OpenBootloader_STM32N6570-DK.bin
set OpenBL_trusted_file=%binary_dir%\OpenBootloader_STM32N6570-DK-trusted.stm32


set version=0x00000000

:: Sign the firmware to install
set command="%stm32signingtoolcli%" -bin "%FSBL_binary_file%" -pubk "%auth_key_pair0_pem%" "%auth_key_pair1_pem%" "%auth_key_pair2_pem%" "%auth_key_pair3_pem%" "%auth_key_pair4_pem%"  "%auth_key_pair5_pem%" "%auth_key_pair6_pem%" "%auth_key_pair7_pem%" -prvk "%auth_priv_key%" -pwd rot1 -iv %version% -of 0x00000001 -la 0x34180000 -t fsbl -o "%FSBL_trusted_file%" -hv 2.3 -dump "%FSBL_trusted_file%" -s
!command!
IF !errorlevel! NEQ 0 goto :error

:: Sign the OpenBL to install
set command="%stm32signingtoolcli%" -bin "%OpenBL_binary_file%" -pubk "%auth_key_pair0_pem%" "%auth_key_pair1_pem%" "%auth_key_pair2_pem%" "%auth_key_pair3_pem%" "%auth_key_pair4_pem%"  "%auth_key_pair5_pem%" "%auth_key_pair6_pem%" "%auth_key_pair7_pem%" -prvk "%auth_priv_key%" -pwd rot1 -iv %version% -of 0x00000001 -la 0x34180000 -t fsbl -o "%OpenBL_trusted_file%" -hv 2.3 -dump "%OpenBL_trusted_file%" -s
!command!
IF !errorlevel! NEQ 0 goto :error

cmd /k
exit 0

:error
echo Failed 
echo "%command% failed"
cmd /k
exit 1