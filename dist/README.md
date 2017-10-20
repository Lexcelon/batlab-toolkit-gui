Make sure the installers are already built.

To build the installers, run:

```
~/Qt/QtIFW-3.0.1/linux/bin/binarycreator -p linux/packages/ -c linux/config/config.xml -n ../dist/linux/Batlab-Toolkit-GUI-Installer

wine ~/Qt/QtIFW-3.0.1/windows/bin/binarycreator.exe -p windows/packages/ -c windows/config/config.xml -n ../dist/windows/Batlab-Toolkit-GUI-Installer.exe
```

1. Build the Windows and Linux programs.
2. Update the package.xml files.
3. Run release.sh.
4. `git push`