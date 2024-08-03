# `bbwalld`

Small daemon that runs alongside [bbLean](https://bb4win.sourceforge.net/bblean/) to resize wallpaper source via NConvert and re-set wallpaper with `bsetroot` upon screen size change, useful for small WinPE environments.

Thanks to @penguin2233 for teaching me how sprintf works lol

## How to use?

1. Change `boot.wim[0]:\Windows\System32\winpe.jpg` to a black 16x16 bitmap and remove `boot.wim[0]:\Windows\System32\WallpaperHost.exe`
2. Change paths to `nconvert.exe`, `bsetroot.exe` and your actual background image on lines 79-81 in <main.cpp>
3. Build using any C++ compiler that will produce binaries that run in your WinPE environment
4. Add bbwalld to your winpeshl.ini w/ delay before launching bbLean:
```ini
[LaunchApps]
%systemroot%\system32\wpeinit.exe
%systemroot%\system32\netstart.exe
%systemroot%\system32\cmd.exe /c "start "" "%systemdrive%\Program Files\bbLean\bbwalld.exe""
%systemroot%\system32\sleep.exe 2s
%systemdrive%\Program Files\bbLean\blackbox.exe
```
