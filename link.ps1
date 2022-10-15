$Compiler="$env:SRG_COMPILER"

if ($Compiler -eq "") {
	$Compiler="gcc"
}

$GCCArch=& $Compiler -dumpmachine
Write-Host "Compiler: $Compiler"
Write-Host "Target: $GCCArch"

$Target=$args[0]
$ObjectFiles=$args[1]

$i = 0;
Foreach ($Arg in $args) {
	if ($i -gt 1) {
		$ObjectFiles = "$ObjectFiles,$Arg"
	}

	$i = $i + 1
}

Write-Host "Linking"
& $Compiler $ObjectFiles.Split(",") -o $Target

if ($?) {
	  Write-Host "Done"
} else {
    Write-Host "Linking error"
}


