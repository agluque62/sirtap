# ----------------------------------------------------------------------------
# Parametros necesarios para ejecutar el script:
#       Ficerho: Fichero a convertir
#       CodOUT: nueva codificación para el fichero de entrada
#
# ----------------------------------------------------------------------------
Param(
[string]$Fichero,
[string]$CodOUT
)



# ----------------------------------------------------------------------------
# 
#                        CUERPO DEL SCRIPT
# 
# ----------------------------------------------------------------------------
# Definimos las variables del script
$StartDate = Get-Date
$WorkingDir = Get-Location

$FicheroIN     = $(Get-ChildItem $Fichero | % {$_.FullName})
$FicheroOUT    = $FicheroIN

if ($Backup) 
{
    Copy-Item -Path $FicheroIN -Destination $FicheroBackup
}

# ENTRADA
try {
   $stream = New-Object System.IO.StreamReader($FicheroIN, $true)
   [char[]]$buffer = New-Object char[] 3
   $stream.Read($buffer, 0, 3) | Out-Null
   
   $CodIN = $stream.CurrentEncoding
   $stream.Close()
   $stream.Dispose()
}
catch {
   try {$stream.Close()  } catch {continue}
   try {$stream.Dispose()} catch {continue}
}

# Write-Host "     Leemos el contenido. Cod IN: $CodIN"
$Contenido = [System.IO.File]::ReadAllLines($FicheroIN, $CodIN)

Remove-Item $Fichero

# SALIDA
# Write-Host "     SALIDA. Cod OUT: $CodOUT"
switch ($CodOUT)
{
   'OEM'        { Out-File -FilePath $FicheroOUT -InputObject $Contenido -Encoding oem }
   'ASCII'      { Out-File -FilePath $FicheroOUT -InputObject $Contenido -Encoding ascii }
   'UTF8'       { Out-File -FilePath $FicheroOUT -InputObject $Contenido -Encoding utf8 }
   'ansi'       { Out-File -FilePath $FicheroOUT -InputObject $Contenido -Encoding default }
   'default'    { Out-File -FilePath $FicheroOUT -InputObject $Contenido -Encoding default }
   'Utf8SinBOM' { $Utf8NoBomEncoding = New-Object System.Text.UTF8Encoding $False
                  [System.IO.File]::WriteAllLines($FicheroOUT, $Contenido, $Utf8NoBomEncoding)
                  #[System.IO.File]::WriteAllText($FicheroOUT, $Contenido, $Utf8NoBomEncoding)
                }
}

