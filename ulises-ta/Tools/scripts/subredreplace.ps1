$pathin = "d:\Datos\Empresa\_SharedPrj\UlisesV5000i\ulises-ta\sources"
$subredin = "192.168.1"
$subredout = "10.35.70"

$files1 = dir -path $pathin -recurse U5ki.Nodebox.exe.config
$files2 = dir -path $pathin -recurse hmi.exe.config
$files3 = dir -path $pathin -recurse spread.conf

$files = $files1 + $files2 + $files3

foreach ($file in $files)
{
    $filepath = $file.FullName
    "Procesando Fichero: " + $filepath
    
    $con = Get-Content $filepath
    $con = $con -replace $subredin $subredout
    Set-Content -path $filepath -value $con
}

