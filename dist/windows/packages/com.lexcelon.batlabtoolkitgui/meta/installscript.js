function Component()
{
    console.log("Component");
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    console.log("prototype");
    console.log("@StartMenuDir@");
    console.log("@UserStartMenuProgramsPath@");
    component.addOperation("CreateShortcut", "@TargetDir@/Batlab.exe", "@UserStartMenuProgramsPath@/@StartMenuDir@/Batlab.lnk", "workingDirectory=@TargetDir@");
}


// function Component()
// {
// }
// Component.prototype.isDefault = function()
// {
//     return true;
// }
// Component.prototype.createOperations = function()
// {
//     try
//     {
// 	component.createOperations();
//     }
//     catch (e)
//     {
// 	print(e);
//     }
//     if (installer.value("os") === "win")
//     {
// 	component.addOperation("CreateShortcut", "@TargetDir@/YourApp.exe", "@DesktopDir@/YourApp_name.lnk");
//     }
//     if (installer.value("os") === "x11")
//     {
// 	component.addOperation("CreateDesktopEntry", "/usr/share/applications/YourApp.desktop", "Version=1.0\nType=Application\nTerminal=false\nExec=@TargetDir@/YourApp.sh\nName=YourApp_name\nIcon=@TargetDir@YourApp_icon.png\nName[en_US]=YourApp_name");
// 	component.addElevatedOperation("Copy", "/usr/share/applications/YourApp.desktop", "@HomeDir@/Desktop/YourApp.desktop");
//     }
// }
