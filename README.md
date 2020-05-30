# TF_Qt

Integration of Tensorflow and Qt in C++

<ul> 
  <li>Tensorflow V2
  <li>Qt
  <li>Bazel
  <li>Visual Studio Code
  <li>C++
</ul>

This program is an updated version of the multibox_detector example included with the tensorflow distribution.  It is designed to run in the examples directory of the tensorflow tree and uses the same input files as the current multibox_detector example.  It is inspired in part by <a href=https://github.com/bbreslauer/qt-bazel-example>qt-bazel<a>
<p>  
There is some background work to be completed before the program will compile.  The project strives to be complete so these necessary background steps are described in detail.  Some familiarity with each of the bullet points listed above will be needed in order to be successful with the compilation.  This project was built on Windows, but uses all open source tools, so should be transferable to other platforms with minor adjustments.
<p>
Major steps:

<ol>
  <li> <a href=#A1>Compile Tensorflow</a>
  <li> <a href=#A2>Build multibox_detector example</a>
  <li> <a href=#A3>git project source code</a>
  <li> <a href=#A4>Set up an IDE using Visual Studio Code</a>
  <li> <a href=#A5>Import Qt components for compile</a>
  <li> <a href=#A6>Edit Tensorflow WORKSPACE file</a>
  <li> <a href=#A7>Compile program</a>
  <li> <a href=#A8>Configure run time environment</a>
  <li> <a href=#A9>Run the program</a>
</ol>

<b><a name=A1>Compile Tensorflow</a></b>
<p>
The official guide for compiling Tensorflow can be found <a href=https://www.tensorflow.org/install/source_windows>here</a>.  The process can be difficult, especially on Windows, so some patience is required.  It is suggested that you git the source code from github as described and use git checkout r2.2 for the current version as of this writing.
<p>
The version of tools used is very important as Tensorflow is sensitive to version changes.  For 2.2, it is necessary to use Visual Studio 2019 and the associated build tools.  Bazel version 2.0.0 is required, and will additionally require the environment variable BAZEL_VC to be set correctly.  Python version 3.6.8 is reccommended.  Pip the additional packages as described in the guide.  MSYS2 is also needed, with the path variable set accordingly.  If the use of GPU is desired, the CUDA packages, including cudnn, will need to be installed and the appropriate environment variables set.  Patience and persistence will be rewarded.
<p>
A legitimate question to be asked at this point is the necessity of compiling Tensorflow as opposed to using prebuilt binaries.  It is required to compile Tensorflow from source as the example project presented here will be compiled within the source tree of Tensorflow.  This is in contrast to most other types of integration where a library file and headers would linked to the project source code from a .lib file.  There is such a file for Tensorflow, however, it is not supported and has many issues.  Compiling the project inside the Tensorflow source tree is an accepted method.

<b><a name=A2>Build multibox_detector example</a></b>

The project is derived from the multibox_detector example and depends upon the same model files.  These files are not included with the Tensorflow distribution and must be downloaded seperately.  There are detailed instructions for obtaining these files and compiling the example in the README file included with the example.  The example can be found in the directory located at tensorflow\examples\multibox_detector in the tensorflow source tree.
<p>
The example program is built by running the bazel script as indicated in the README.  The correct operation of the example program should be confirmed before continuing with the project.
<p>
<b><a name=A3>git project source code</a></b>
<p>
The project source code can now be imported into the tensorflow source tree.  It belongs at the same level as the multibox_detector example compiled in the previous step.  To import the files, use the command prompt to navigate to tensorflow source code installation directory and then to tensorflow\examples.  Using the command 
<p>
<pre>
git clone https://github.com/sr99622/TF_Qt 
</pre>
<p>
will install the project from there into the correct location.
<p>
<b><a name=A4>Set up an IDE using Visual Studio</a></b>
<p>
Using an IDE will enhance the process of developing applications under these conditions.  Visual Studio Code is chosen for its versatility and availablity of add on design tools that will make working with this project much easier.  A project workspace is created that envelopes the entire tensorflow tree, so the workspace file will be located at the same directory level as the root of the tensorflow source code tree.  This position ensure that all header files and object code will be available to the project.
<p>
Visual Studio Code will need to be equipped with some extensions to enable development.  There are two C++ intellisense extensions that have been used successfully.  For some reason, it is necessary to have both.  C/C++ from Microsoft (ms-vscode.cpptools) and C++ Intellisense from austin (austin.code-gnu-global).  These two extensions working together will properly hightlight syntax and provide rudimentary command options during code editing.  It is important to note that the intellisense engine is a server that will take some time to initialize depending upon where file under consideration is located in the source code.  It may take a few minutes for the intellisense to properly identify correct syntax.  It is also important to note the limitations of this engine.  There are certain protobuf headers (.pb.h) located in the bazel-* subdirectories that will cause the intellisense engine to crash.  If one of those files is needed for reference, it is recommended that a seperate text editor is used.
<p>
There is also a need for a Python extension.  Python scripts will be used to invoke the compilation and execution of the program.  The extension provided by Microsoft (ms-python.python) is a good choice and will enable the launch of these scripts from within the development environment.  Lastly, a Bazel extension is useful.  The vscode-bazel extension provided by the Bazel team (bazelbuild.vscode-bazel) is suggested.
<p>
<b><a name=A5>Import Qt components for compile</a></b>
<p>
A Qt installation is necessary for this project.  It is necessary to use the 64 bit version, so the installer should be directed to include that configuration.  This particular build uses MSVC 2107.  The default location for the Qt installation is C:\Qt and this location is recommended for this project.  Once the Qt installation has completed, it is necessary to make the moc.exe file available to the Visual Studio Code environment so that it may be used during compilation.  The moc.exe program will generate intermediate moc_$.cc files based on the contents of .h files in the build.  It is recommended to install the moc.exe in the path.  A convenient location is the %LOCALAPPDATA%\Programs\Microsoft VS Code\bin directory created by Visual Studio Code.  Assuming a 2017 MSVC 64 bit Qt installation in the default directory, the moc.exe file can be found at C:\Qt\5.12.3\msvc2017_64\bin\moc.exe.  Note that there may be some varation based on the particular Qt version installed.
<p>
<b><a name=A6>Edit Tensorflow WORKSPACE file</a></b>
<p>
It is necessary to edit the tensorflow workspace in order that bazel may find the headers and BUILD files for the project.  The following code is added to the end of the tensorflow/WORKSPACE file to affect this change.  Note that bazel convention includes the comma following the last argument of the directive.  The location of the Qt path may differ in accordance with the particular installation, and forward slashes are used regardless of operating system.
<p>
<pre>
new_local_repository(
  name = "qt",
  build_file = "tensorflow/examples/TF_Qt/qt/qt.BUILD",
  path = "C:/Qt/5.12.3/msvc2017_64",
)
</pre>
<p>
<b><a name=A7>Compile the program</a></b>
<p>
The program may now be compiled.  There is a python script in the project named compile.py.  It can be invoked by right clicking over the file name the Visual Studio Code file navigation panel on the left hand side of the window.  The pop up menu will have a selection called "Run Python File In Terminal" which is the desired action.  The terminal pane should be displayed along the bottom of the window and show the compilation steps underway as the program compiles.  If all goes well, the build will complete successfully.
<p>
<b><a name=A8>Configure run time environment</a></b>
<p>
In order to run the executable, it is necessary that the .exe file has access to the appropriate Qt dll's.  Qt supplies a utility program for this purpose.  It is named windeployqt.exe and can be found at C:\Qt\5.12.3\msvc2017_64\bin, or other similar directory depending upon installation.  The proper use of the utility involves adding the directory location of the program executable to the PATH environment variable, then running windeployqt to populate that directory with the necessary run time files.
<p>
This is acheived by opening a command prompt and navigating to the appropriate Qt bin directory, and locating the windeployqt.exe file.  A temporary change to the PATH environment variable can be made within the command prompt by using the SET command.  The path is appended by the command SET PATH=%PATH%<i>full_path_to_program_executable</i>
<p>
The program directory executable is found within the tensorflow source tree at
<p>
<pre>
\bazel-out\x64_windows-opt\bin\tensorflow\examples\TF_Qt\qt\multibox;  
</pre>
<p>
Note that the full path name must be supplied, so the full path name will start with C:\ in most instances.
<p>
Once the program directory location has been added to the path, the windeployqt utility is run using the full path name of the main.exe program executable as the argument.  The utility will scan the executable for depedencies and populate the directory with the necessary files.
<p>
<b><a name=A9>Run the program</a></b>
<p>
If all has gone well, now is finally the time to run the program.  The program can be launched from Visual Studio Code in much the same way as it was compiled.  The run.py file in the navigation pane is right clicked "Run Python File In Terminal" selected. The program will present a window with a button that can be clicked to run the model on the sample picture the same manner as in the standard Tensorflow example program with the same results.  Feel free at this point to crack a brewski and bask in the glory of accomplishment.
<p>
Further information regarding the details of the implementation of the program can be found the README file in the multibox directory of the project.
