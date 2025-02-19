@echo off
echo Restoring .vcxproj.filters file...

:: ���� ���� ���
set FILTER_FILE=Intermediate\ProjectFiles\InfectedCity.vcxproj.filters
:: ��� ���� ���
set BACKUP_FILE=Backup\InfectedCity.vcxproj.filters

:: ���� ���� ����
if exist %BACKUP_FILE% (
    copy /Y %BACKUP_FILE% %FILTER_FILE%
    echo Restore successful!
) else (
    echo Backup filters file not found!
)

pause
