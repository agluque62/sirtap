# ----------------------------------------------------------------------------
# Parametros necesarios para ejecutar el script:
# ----------------------------------------------------------------------------
Param(
[string]$UsuarioBD,
[string]$PasswordBD,
[string]$ServidorBD,
[string]$IdLogProceso,
[int]$IdIncidencia=6000
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
[switch]$Existe=$false
[string]$BDOnline

[string]$cmdInsert = ""
[string]$cmdUpdate = ""
[string]$cmdWhere  = ""
[string]$DescBD = ""

if ($IdLogProceso -eq "") {
   LogError -Error "El parámetro IdLogProceso no puede ser nulo" 
}

if ($IdIncidencia -le 0) {
   $IdIncidencia = 6000
}

# Conectarse a la BD
$ConnBD=Conexion_BD -IPServidor $ServidorBD -Usuario $UsuarioBD -Password $PasswordBD

# Cual es la BD de producción (online)?
$Existe=$false
foreach ($ConfOnline in ConsultaBD -conexion $ConnBD -consulta "Select NOMBRE From CONFIGURACIONES Where TIPO = 'ONLINE'") {
   $Existe=$true
   $BDOnline = $ConfOnline.NOMBRE
   break
}

if (-not $Existe) {
   LogError -Error "No se encuentra ninguna BD configurada como ONLINE" 
}

# Existe la incidencia? Si no es así, se crea
$Existe=$false
foreach ($Incidencia in ConsultaBD -conexion $ConnBD -consulta "Select DESCRIPCION From $($BDOnline).incidencias Where IdIncidencia = $IdIncidencia") {
   $Existe=$true
   break
}

if (-not $Existe) {
   # Crear la incidencia por defecto para la Gestión de configuraciones Offline
   ComandoBD -conexion $ConnBD -comando "Insert into $($BDOnline).incidencias         (IdIncidencia, Incidencia, Descripcion, GeneraError) Values ($IdIncidencia, 'Conf.Offline', '{0}', 0)" | Out-Null
   ComandoBD -conexion $ConnBD -comando "Insert into $($BDOnline).incidencias_frances (IdIncidencia, Incidencia, Descripcion, GeneraError) Values ($IdIncidencia, 'Conf.Offline', '{0}', 0)" | Out-Null
   ComandoBD -conexion $ConnBD -comando "Insert into $($BDOnline).incidencias_ingles  (IdIncidencia, Incidencia, Descripcion, GeneraError) Values ($IdIncidencia, 'Offline Conf', '{0}', 0)" | Out-Null
}

if (-not $Existe) {
   LogError -Error "No se encuentra el Log de procesos con el Id $IdLogProceso" 
}

# Insertar el registro en HistoricoIncidencias (en el esquema de new_cd40_offline es una vista sobre la tabla $($BDOnline).HistoricoIncidencias
$cmdInsert = "Insert into HistoricoIncidencias ( IdSistema "
$cmdInsert = $cmdInsert +                    " , IdHw "
$cmdInsert = $cmdInsert +                    " , TipoHw "
$cmdInsert = $cmdInsert +                    " , IdIncidencia "
$cmdInsert = $cmdInsert +                    " , FechaHora "
$cmdInsert = $cmdInsert +                    " , Descripcion "
$cmdInsert = $cmdInsert +                    " , Usuario) "
$cmdInsert = $cmdInsert +               " Select t.IdSistema "
$cmdInsert = $cmdInsert +                     " , 'GOFF' "
$cmdInsert = $cmdInsert +                     " , 0 "
$cmdInsert = $cmdInsert +                     " , $IdIncidencia "
$cmdInsert = $cmdInsert +                     " , t.FechaHora "
$cmdInsert = $cmdInsert +                     " , CASE "
$cmdInsert = $cmdInsert +                          " WHEN LENGTH(t.Descripcion) < 200 THEN "
$cmdInsert = $cmdInsert +                             " t.Descripcion "
$cmdInsert = $cmdInsert +                          " WHEN LENGTH(t.Descripcion) > 200 AND INSTR(t.Descripcion, '...') > 0 THEN "
$cmdInsert = $cmdInsert +                             " CONCAT(t.DESC_1, ' [...] ', TRIM(SUBSTR(t.DESC_2, INSTR(t.DESC_2, ' ')))) "
$cmdInsert = $cmdInsert +                          " ELSE "
$cmdInsert = $cmdInsert +                             " SUBSTR(t.Descripcion, 1, 200) "
$cmdInsert = $cmdInsert +                       " END"
$cmdInsert = $cmdInsert +                     " , t.Usuario "
$cmdInsert = $cmdInsert +                " From (Select IdSistema "
$cmdInsert = $cmdInsert +                           " , FechaHora "
$cmdInsert = $cmdInsert +                           " , TRIM(SUBSTR(Descripcion, 1, INSTR(Descripcion, '...') - 1)) DESC_1"
$cmdInsert = $cmdInsert +                           " , TRIM(SUBSTR(Descripcion, -(193 - INSTR(Descripcion, '...')))) DESC_2"
$cmdInsert = $cmdInsert +                           " , Descripcion "
$cmdInsert = $cmdInsert +                           " , Usuario "
$cmdInsert = $cmdInsert +                        " From LogProcesos " 
$cmdInsert = $cmdInsert +                       " Where Id = '$IdLogProceso') t "
ComandoBD -conexion $ConnBD -comando $cmdInsert | Out-Null


FinScript -ExitCode 0
