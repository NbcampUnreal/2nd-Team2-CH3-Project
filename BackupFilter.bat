@echo off
echo Backing up .vcxproj.filters file...

:: 필터 파일 경로
set FILTER_FILE=Intermediate\ProjectFiles\InfectedCity.vcxproj.filters
:: 백업 파일 경로
set BACKUP_FILE=Backup\InfectedCity.vcxproj.filters

:: 백업 폴더 생성 (없으면 만들기)
if not exist Backup (
    mkdir Backup
)

:: 필터 파일 백업
if exist %FILTER_FILE% (
    copy /Y %FILTER_FILE% %BACKUP_FILE%
    echo Backup successful!
) else (
    echo Filters file not found!
)

pause
