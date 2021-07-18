@echo off

set ProjectName=Kessler_Syndrome

call misc\shell.bat %ProjectName%
call misc\sublime.bat code\%ProjectName%.cpp
