@echo off
chcp 65001 >nul

REM 这是一个本地版的workflow，不需要gihub workflow进行构建，只需要在本地执行以下命令即可，效果和github workflow相同，但是不会在github workflow 上被使用
D:
cd /d %~dp0

md docs 2>nul
python gen-md.py
copy /y README.md .\docs\index.md
if exist assets xcopy assets .\docs\assets /E /I /Y

rem 本地部署到 gh-pages（需要本地已登录 git）
mkdocs gh-deploy

rem 清理构建产物，不纳入提交
rmdir /s /q site docs

git add -A
git commit -m "Add files via upload"
git push -f origin main

pause
