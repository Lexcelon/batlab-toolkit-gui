function Component() {
    component.addElevatedOperation("Execute", "mkdir", "-p", "@TargetDir@");
    
    component.addElevatedOperation("Execute", "cp", "/etc/shells", "@TargetDir@/Batlab-run");
    component.addElevatedOperation("Execute", "sed", "-i", 's/.*//', "@TargetDir@/Batlab-run");
    component.addElevatedOperation("Execute", "sed", "-i", '1s/.*/#!\\/bin\\/bash/g', "@TargetDir@/Batlab-run");
    component.addElevatedOperation("Execute", "sed", "-i", '$ acd @TargetDir@', "@TargetDir@/Batlab-run");
    component.addElevatedOperation("Execute", "sed", "-i", '$ a\.\/Batlab\.sh', "@TargetDir@/Batlab-run");
    component.addElevatedOperation("Execute", "chmod", "+x", "@TargetDir@/Batlab-run");
    
    component.addElevatedOperation("Execute", "ln", "-sf", "@TargetDir@/Batlab-run", "/usr/local/bin/batlab-toolkit-gui");
    
    
}

