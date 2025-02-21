@echo off
echo Backing up .vcxproj.filters file...

:: ���� ���� ���
set FILTER_FILE=Intermediate\ProjectFiles\InfectedCity.vcxproj.filters
:: ��� ���� ���
set BACKUP_FILE=Backup\InfectedCity.vcxproj.filters

:: ��� ���� ���� (������ �����)
if not exist Backup (
    mkdir Backup
)

:: ���� ���� ���
if exist %FILTER_FILE% (
    copy /Y %FILTER_FILE% %BACKUP_FILE%
    echo Backup successful!
) else (
    echo Filters file not found!
)

pause
