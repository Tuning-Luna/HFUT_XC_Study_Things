@echo off
chcp 65001 >nul

D:
cd /d %~dp0

md docs 2>nul
python gen-md.py
copy /y README.md .\docs\index.md
if exist asset xcopy asset .\docs\asset /E /I /Y

rem 本地部署到 gh-pages（需要本地已登录 git）
mkdocs gh-deploy

rem 清理构建产物，不纳入提交
rmdir /s /q site docs

git add -A
git commit -m "Add files via upload"
git push -f origin main

pause
