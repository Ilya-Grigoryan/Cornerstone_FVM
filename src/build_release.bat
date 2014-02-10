md s:\bin\
md s:\lib\
md s:\include\

cd cip_fileproxy
msdev cip_fileproxy.dsw  /MAKE "cip_fileproxy - Win32 Release"
xcopy /y /r /s input.h s:\include\*
xcopy /y /r /s EngineMSExcel.h s:\include\*
xcopy /y /r /s s:\temp\cip_fileproxy\cip_fileproxy.lib  s:\lib\*
cd ..

cd cip_dbproxy
msdev cip_dbproxy.dsw  /MAKE "cip_dbproxy - Win32 Release"
xcopy /y /r /s sqlcompany.h s:\include\*
xcopy /y /r /s sqluser.h s:\include\*
xcopy /y /r /s sqlmeetings.h s:\include\*
xcopy /y /r /s s:\temp\cip_dbproxy\cip_dbproxy.lib  s:\lib\*
cd ..

cd cip_core
msdev cip_core.dsw  /MAKE "cip_core - Win32 Release"
xcopy /y /r /s macroparameters.h  s:\include\*
xcopy /y /r /s basemath.h  s:\include\*
xcopy /y /r /s listex.h  s:\include\*
xcopy /y /r /s company.h  s:\include\*
xcopy /y /r /s history.h  s:\include\*
xcopy /y /r /s cipexception.h  s:\include\*
xcopy /y /r /s universe.h  s:\include\*
xcopy /y /r /s montecarlo.h  s:\include\*
xcopy /y /r /s datamanager.h  s:\include\*
xcopy /y /r /s report.h  s:\include\*
xcopy /y /r /s s:\temp\cip_core\cip_core.lib  s:\lib\*
cd ..

cd CornerstoneFVModel
msdev CornerstoneFVModel.dsw  /MAKE "CornerstoneFVModel - Win32 Release"
cd ..

cd cip_baselineproxy
msdev cip_baselineproxy.dsw  /MAKE "cip_baselineproxy - Win32 Release"
cd ..

cd ..\bin
xcopy /y /r /s libmySQL.dll t:\Model-2010\*
xcopy /y /r /s cip_fileproxy.dll t:\Model-2010\*
xcopy /y /r /s cip_dbproxy.dll t:\Model-2010\*
xcopy /y /r /s cip_core.dll t:\Model-2010\*
xcopy /y /r /s CornerstoneFVModel.exe t:\Model-2010\*
xcopy /y /r /s cip_baselineproxy.exe t:\Model-2010\*
cd s:\src

pause
