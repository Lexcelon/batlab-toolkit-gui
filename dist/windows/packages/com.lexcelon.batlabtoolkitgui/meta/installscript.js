function Component()
{

}

Component.prototype.createOperations = function()
{
    component.createOperations();

    component.addOperation("CreateShortcut", "@TargetDir@/Batlab.exe", "@StartMenuDir@/@Name@.lnk");
}
