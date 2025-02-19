@echo off
echo Restoring .vcxproj.filters file...

:: 필터 파일 경로
set FILTER_FILE=Intermediate\ProjectFiles\InfectedCity.vcxproj.filters
:: 백업 파일 경로
set BACKUP_FILE=Backup\InfectedCity.vcxproj.filters

:: 필터 파일 복원
if exist %BACKUP_FILE% (
    copy /Y %BACKUP_FILE% %FILTER_FILE%
    echo Restore successful!
) else (
    echo Backup filters file not found!
)

pause
