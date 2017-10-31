# Publish installer artifact

Get-ChildItem Batlab-Toolkit-GUI-Installer-*.exe | % { Push-AppveyorArtifact $_.FullName -FileName $_.Name }

# Publish updated binary artifacts

cd windows

$root = Resolve-Path .

[IO.Directory]::GetFiles($root.Path, '*.*', 'AllDirectories') | % { Push-AppveyorArtifact $_ -FileName $_.Substring($root.Path.Length + 1) -DeploymentName to-publish }

cd ..