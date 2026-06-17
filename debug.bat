net use Z: /delete
pushd \\Mac\Home\Desktop\Aidan
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
start /wait build_and_run.bat main.c --debug-rad
