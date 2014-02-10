md s:\bindebug\
md s:\libdebug\
md s:\include\

cd cip_fileproxy
msdev cip_fileproxy.dsw  /MAKE "cip_fileproxy - Win32 Debug"
xcopy /y /r /s input.h s:\include\*
xcopy /y /r /s EngineMSExcel.h s:\include\*
xcopy /y /r /s s:\tempdebug\cip_fileproxy\cip_fileproxy.lib  s:\libdebug\*
cd ..

cd cip_dbproxy
msdev cip_dbproxy.dsw  /MAKE "cip_dbproxy - Win32 Debug"
xcopy /y /r /s sqlcompany.h s:\include\*
xcopy /y /r /s sqluser.h s:\include\*
xcopy /y /r /s sqlmeetings.h s:\include\*
xcopy /y /r /s s:\tempdebug\cip_dbproxy\cip_dbproxy.lib  s:\libdebug\*
cd ..

cd cip_core
msdev cip_core.dsw  /MAKE "cip_core - Win32 Debug"
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
xcopy /y /r /s s:\tempdebug\cip_core\cip_core.lib  s:\libdebug\*
cd ..

cd CornerstoneFVModel
msdev CornerstoneFVModel.dsw  /MAKE "CornerstoneFVModel - Win32 Debug"
cd ..

cd cip_baselineproxy
msdev cip_baselineproxy.dsw  /MAKE "cip_baselineproxy - Win32 Debug"
cd ..

pause