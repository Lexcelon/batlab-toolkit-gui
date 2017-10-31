Get-ChildItem Batlab-Toolkit-GUI-Installer-*.exe | % { Push-AppveyorArtifact $_.FullName -FileName $_.Name }

$root = Resolve-Path .\windows

[IO.Directory]::GetFiles($root.Path, '*.*', 'AllDirectories') | % { Push-AppveyorArtifact $_ -FileName $_.Substring($root.Path.Length + 1) -DeploymentName to-publish }

