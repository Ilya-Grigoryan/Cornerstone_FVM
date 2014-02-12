md ..\bindebug\
md ..\libdebug\
md ..\include\

cd cip_fileproxy
msdev cip_fileproxy.dsw  /MAKE "cip_fileproxy - Win32 Debug"
xcopy /y /r /s input.h ..\include\*
xcopy /y /r /s EngineMSExcel.h ..\include\*
xcopy /y /r /s ..\tempdebug\cip_fileproxy\cip_fileproxy.lib  ..\libdebug\*
cd ..

cd cip_dbproxy
msdev cip_dbproxy.dsw  /MAKE "cip_dbproxy - Win32 Debug"
xcopy /y /r /s sqlcompany.h ..\include\*
xcopy /y /r /s sqluser.h ..\include\*
xcopy /y /r /s sqlmeetings.h ..\include\*
xcopy /y /r /s ..\tempdebug\cip_dbproxy\cip_dbproxy.lib  ..\libdebug\*
cd ..

cd cip_core
msdev cip_core.dsw  /MAKE "cip_core - Win32 Debug"
xcopy /y /r /s macroparameters.h  ..\include\*
xcopy /y /r /s basemath.h  ..\include\*
xcopy /y /r /s listex.h  ..\include\*
xcopy /y /r /s company.h  ..\include\*
xcopy /y /r /s history.h  ..\include\*
xcopy /y /r /s cipexception.h  ..\include\*
xcopy /y /r /s universe.h  ..\include\*
xcopy /y /r /s montecarlo.h  ..\include\*
xcopy /y /r /s datamanager.h  ..\include\*
xcopy /y /r /s report.h  ..\include\*
xcopy /y /r /s ..\tempdebug\cip_core\cip_core.lib  ..\libdebug\*
cd ..

cd CornerstoneFVModel
msdev CornerstoneFVModel.dsw  /MAKE "CornerstoneFVModel - Win32 Debug"
cd ..

cd cip_baselineproxy
msdev cip_baselineproxy.dsw  /MAKE "cip_baselineproxy - Win32 Debug"
cd ..

pause