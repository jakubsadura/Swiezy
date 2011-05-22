
CMAKE_MINIMUM_REQUIRED (VERSION 2.2)

INCLUDE( "GIMIASTestMacro.cmake" )

TEST_GIMIAS( 
  "demo.machine.cistib" 
  "WinXP64-vs90-64" 
  "C:/Python26/python.exe" 
  "E:/development/CSnake/src/csnConsole.py"
  "E:/Temp/Subversion Client/svn.exe"
  "E:/development/src/gimias_devel_2008_64.CSnakeGUI"
  "E:/development/src/gimias_devel/Resources"
  "E:/development/bin/gimias_devel_2008_64/executable/Gimias"
  "E:/development/src/gimias_devel"
  )

TEST_GIMIAS( 
  "demo.machine.cistib" 
  "WinXP64-vs90-32" 
  "C:/Python26/python.exe" 
  "E:/development/CSnake/src/csnConsole.py"
  "E:/Temp/Subversion Client/svn.exe"
  "E:/development/src/gimias_devel_2008_32.CSnakeGUI"
  "E:/development/src/gimias_devel/Resources"
  "E:/development/bin/gimias_devel_2008_32/executable/Gimias"
  "E:/development/src/gimias_devel"
  )  

TEST_GIMIAS( 
  "demo.machine.cistib" 
  "WinXP64-vs71-32" 
  "C:/Python26/python.exe" 
  "E:/development/CSnake/src/csnConsole.py"
  "E:/Temp/Subversion Client/svn.exe"
  "E:/development/src/gimias_devel_2003_32.CSnakeGUI"
  "E:/development/src/gimias_devel/Resources"
  "E:/development/bin/gimias_devel_2003_32/executable/Gimias"
  "E:/development/src/gimias_devel"
  )  
  
  