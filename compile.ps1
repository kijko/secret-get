$Compiler="$env:SRG_COMPILER"

if ($Compiler -eq "") {
	$Compiler="gcc"
}

$GCCArch=& $Compiler -dumpmachine
Write-Host "Compiler: $Compiler"
Write-Host "Target: $GCCArch"

$SrcDir=$args[0]
$BuildDir=$args[1]
$ModuleName=$args[2]
$Version=$args[3]
$VersionArg="-DSRG_VERSION=$Version"
$LibHeaders=$args[4]

New-Item $BuildDir -ItemType "Directory" -Force

Write-Host "Compiling $ModuleName"

& $Compiler -c -std=c17 -x c -Wextra "-Wno-cast-function-type" -g $VersionArg "$SrcDir\$ModuleName.c" -I $LibHeaders -o "$BuildDir\$ModuleName.o"

if ($?) {
	Write-Host "Done"
} else {
  Write-Host "Error compiling"
}

