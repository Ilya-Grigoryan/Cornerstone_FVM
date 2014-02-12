md ..\..\bin\
md ..\..\lib\
md ..\..\include\

cd cip_fileproxy
msdev cip_fileproxy.dsw  /MAKE "cip_fileproxy - Win32 Release"
xcopy /y /r /s input.h ..\..\include\*
xcopy /y /r /s EngineMSExcel.h ..\..\include\*
xcopy /y /r /s ..\..\temp\cip_fileproxy\cip_fileproxy.lib  ..\..\lib\*
cd ..

cd cip_dbproxy
msdev cip_dbproxy.dsw  /MAKE "cip_dbproxy - Win32 Release"
xcopy /y /r /s sqlcompany.h ..\..\include\*
xcopy /y /r /s sqluser.h ..\..\include\*
xcopy /y /r /s sqlmeetings.h ..\..\include\*
xcopy /y /r /s ..\..\temp\cip_dbproxy\cip_dbproxy.lib  ..\..\lib\*
cd ..

cd cip_core
msdev cip_core.dsw  /MAKE "cip_core - Win32 Release"
xcopy /y /r /s macroparameters.h  ..\..\include\*
xcopy /y /r /s basemath.h  ..\..\include\*
xcopy /y /r /s listex.h  ..\..\include\*
xcopy /y /r /s company.h  ..\..\include\*
xcopy /y /r /s history.h  ..\..\include\*
xcopy /y /r /s cipexception.h  ..\..\include\*
xcopy /y /r /s universe.h  ..\..\include\*
xcopy /y /r /s montecarlo.h  ..\..\include\*
xcopy /y /r /s datamanager.h  ..\..\include\*
xcopy /y /r /s report.h  ..\..\include\*
xcopy /y /r /s ..\..\temp\cip_core\cip_core.lib  ..\..\lib\*
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
cd ..\..\src

pause
