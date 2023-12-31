# ----------------------------------------------------------------------------
# Parametros necesarios para ejecutar el script:
# ----------------------------------------------------------------------------
Param(
[string]$UsuarioBD,
[string]$PasswordBD,
[string]$ServidorBD,
[string]$GUID,
[string]$GUIDPadre,
[string]$Descripcion,
[switch]$ReplaceDesc,
[string]$Activo,
[string]$IdProceso,
[string]$Proceso,
[string]$Argumentos
)


# ----------------------------------------------------------------------------
# 
#                        FUNCIONES DEL SCRIPT
# 
# ----------------------------------------------------------------------------
function LogError {
    Param (
        [string]  $Error,
        [string]  $Procedure
    )

    if ($Error -ne "") {
        $Err = "ERROR: $Error"
        if($Procedure -ne "") {
           $Err = "$Err en $Procedure"
        }

        Write-Host ""
        Write-Host "   -----------------------------------------------------"
        Write-Host "   $Err"
        Write-Host "   -----------------------------------------------------"
    }

    FinScript -ExitCode 1
}

function FinScript {
    Param (
        [int] $ExitCode
    )

   try {
      $ConnBD.Close()
   } 
   catch {
      continue
   }

   if ($ExitCode -eq $null) {
     exit 0
   }
   else {
      exit $ExitCode
   }
}

function Conexion_BD {
    Param (
        [string]  $IPServidor,
        [string]  $Usuario,
        [string]  $Password
    )
    
    try {
       $ConnStr="server=$IPServidor;user id=$Usuario;password=$Password;pooling=false;database=new_cd40_offline"
       $Conn = New-Object MySql.Data.MySqlClient.MySqlConnection
       $Conn.ConnectionString = $ConnStr
       $Conn.Open()
    }
    catch {
       LogError -Error $_.Exception.Message -Procedure "Conexión BD"
    }

    return $Conn
}

function ConsultaBD {
    Param (
        [MySql.Data.MySqlClient.MySqlConnection] $conexion,
        [string] $consulta
    )
    
    try {
       $SQL = New-Object MySql.Data.MySqlClient.MySqlCommand
       $SQL.Connection  = $conexion
       $SQL.CommandText = $consulta
       $Reader = $SQL.ExecuteReader()
       $Resultado = New-Object Data.DataTable
       $Resultado.Load($Reader)
       $Reader.Close()
    }
    catch {
       $Reader.Close()
       LogError -Error "$consulta $($_.Exception.Message)" 
    }
    
    return $Resultado
}

function ComandoBD{
    Param (
        [MySql.Data.MySqlClient.MySqlConnection] $conexion,
        [string] $comando,
        [switch] $IgnorarError=$false
    )
    
    $SQL = New-Object MySql.Data.MySqlClient.MySqlCommand
    $SQL.Connection  = $conexion
    $SQL.CommandText = $comando

    # Write-Host "LogProceso.ComandoBD: $comando" 
    try   {
       $SQL.ExecuteNonQuery() | Out-Null
    }
    catch {
       if (-not $IgnorarError) {
          LogError -Error "$comando $($_.Exception.Message)" 
       }
    }
}



# ----------------------------------------------------------------------------
# 
#                        CUERPO DEL SCRIPT
# 
# ----------------------------------------------------------------------------
[void][Reflection.Assembly]::LoadWithPartialName('MySQL.Data')
# Definimos las variables del script
$StartDate = Get-Date
$WorkingDir = Get-Location

# Componer el comando a ejecutar
[string]$cmd = ""
[string]$cmdInsert = ""
[string]$cmdUpdate = ""
[string]$cmdWhere  = ""
[string]$DescBD = ""
[string]$DescPadre = ""
[switch]$Existe=$false

#Write-Host "UsuarioBD  : $UsuarioBD"
#Write-Host "PasswordBD : $PasswordBD"
#Write-Host "ServidorBD : $ServidorBD"
#Write-Host "GUID       : $GUID"
#Write-Host "GUIDPadre  : $GUIDPadre"
#Write-Host "Descripcion: $Descripcion"
#if ($ReplaceDesc) {
#   Write-Host "ReplaceDesc: SI"
#} else {
#   Write-Host "ReplaceDesc: NO"
#}
#Write-Host "Activo     : $Activo"
#Write-Host "IdProceso  : $IdProceso"
#Write-Host "Proceso    : $Proceso"
#Write-Host "Argumentos : $Argumentos"

if ($Descripcion -ne "") {
   $Descripcion = $Descripcion.Replace("\", "\\")
   $Descripcion = $Descripcion.Replace("'", "''")
}
$DescPadre = $Descripcion

if ($GUID -eq "") {
   LogError -Error "El parámetro GUID no puede ser nulo"
}
# Write-Host "LogProcesos: Arg $Argumentos"
# Conectarse a la BD
$ConnBD=Conexion_BD -IPServidor $ServidorBD -Usuario $UsuarioBD -Password $PasswordBD

if (($GUIDPadre -ne "") -and ($GUID -ne $GUIDPadre)) {
  # Si no existe, o las columnas no tienen valor, insertamos/modificamos el registro para el procreso que se va a registrar a partir de los datos del padre
  # Write-Host "LogProcesos: es un subproceso. GUID: $GUID, GUIDPadre: $GUIDPadre"
  foreach ($Log in ConsultaBD -conexion $ConnBD -consulta "Select PID, IdSistema, Usuario, Activo From LogProcesos Where Id = '$GUID'") {
     $Existe=$true
     break
  }
  
  foreach ($LogPadre in ConsultaBD -conexion $ConnBD -consulta "Select PID, IdSistema, Usuario, Activo From LogProcesos Where Id = '$GUIDPadre'") {
     if (-not $Existe) {
       ComandoBD -conexion $ConnBD -comando "Insert into LogProcesos (Id, PID, IdSistema, Usuario, Activo) Values ('$GUID', '$($LogPadre.PID)', '$($LogPadre.IdSistema)', '$($LogPadre.Usuario)', $($LogPadre.Activo))" | Out-Null
     } 
     break
  }
}

$cmdWhere  = "Where Id = '$GUID'"
$cmdInsert = "Insert into LogProcesos (Id, Descripcion, Activo) Values ('$GUID', '$Descripcion', $Activo)"
$cmdUpdate = "Update LogProcesos Set FechaHora = NOW()"

if ($Activo -ne "") {
  if ($Activo -ge 1) {
    $cmdUpdate = $cmdUpdate + ", Activo = 1"
  }
  else {
    $cmdUpdate = $cmdUpdate + ", Activo = 0"
  }
}

# Existe el registro?
$cmd = "Select ID, DESCRIPCION From LogProcesos " + $cmdWhere
$Existe=$false
foreach ($Log in ConsultaBD -conexion $ConnBD -consulta $cmd) {
   $DescBD=$Log.DESCRIPCION
   if (-not [string]::IsNullOrEmpty($DescBD)) {
     $DescBD=$DescBD.Replace("\", "\\");
     $DescBD=$DescBD.Replace("'", "''")
   }
   $Existe=$true
   break
}

if ($Existe) {
   if ($ReplaceDesc) {
      $cmdUpdate = $cmdUpdate + ", Descripcion = '$Descripcion' " + $cmdWhere
   }
   else {
      if ($Descripcion -ne "") {
         if ($DescBD.IndexOf("...") -ge 0) {
            # Write-Host "... Añadir a la desc de la BD: $Descripcion" 
            $Descripcion = $DescBD.Substring(0, $DescBD.IndexOf("...")).Trim(" ") + " ... " + $Descripcion
         }
         elseif ($Descripcion.Substring(0, 3) -ieq "ERR") {
            # Write-Host "... Se trata de un error: $Descripcion" 
            $Descripcion = $DescBD + ". " + $Descripcion
         }
         else {
            # Write-Host "... Añadir a la desc de la BD: $Descripcion" 
            $Descripcion = $DescBD + ". " + $Descripcion
         }
         $cmdUpdate = $cmdUpdate + ", Descripcion = '$Descripcion' " + $cmdWhere
      }
      else {
         $cmdUpdate = $cmdUpdate + " " + $cmdWhere
      }   
   }   
   $cmd = $cmdUpdate
}
else {
   $cmd = $cmdInsert
}

# Write-Host "LogProcesos: Modificamos el registro"
ComandoBD -conexion $ConnBD -comando $cmd | Out-Null

# Si se trata de un error, el proceso pasa a Activo = 0 y hay un proceso padre, entonces marcamos el error en el proceso padre y también lo desactivamos
if (($DescPadre.Substring(0, 3) -ieq "ERR") -and ($Activo -ne "") -and ($Activo -lt 1))
{
   if (($GUIDPadre -ne "") -and ($GUID -ne $GUIDPadre))
   {
      $cmdUpdate = "Update LogProcesos Set FechaHora = NOW(), Descripcion = CONCAT(Descripcion, '
       ', '$DescPadre'), Activo = 0 Where Id = '$GUIDPadre'"
      ComandoBD -conexion $ConnBD -comando $cmdUpdate | Out-Null
   }
}

FinScript -ExitCode 0
