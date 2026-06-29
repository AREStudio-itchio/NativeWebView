# NativeWebView
NativeWebView is a non-required attribution alternative to the WebView2 package made by Microsoft.

Of all the HTML wrapper versions I've programmed, this is the best. You can launch Edge with any HTML file you want, and you can add flags!

And it's available in DLL form!

# Build
MinGW static library:
```cmd
clang++ -c NativeWebView.cpp -o NativeWebView.o && llvm-ar rcs libNativeWebView.a NativeWebView.o
```
MinGW dynamic library:
```cmd
clang++ -shared NativeWebView.cpp -o NativeWebView.dll "-Wl,--out-implib,libNativeWebView.dll.a"
```
MSVC library:
```cmd
clang++ -shared NativeWebView.cpp -o NativeWebView.dll -fuse-ld=lld "-Wl,/IMPLIB:NativeWebView.lib"
```
Execute the ninja command in the CMD o PS prompt to compile the library:
```cmd
ninja
```
and for the program:
```cmd
g++ main.cpp -o main.exe -static-libgcc -static-libstdc++ -lNativeWebView
```
