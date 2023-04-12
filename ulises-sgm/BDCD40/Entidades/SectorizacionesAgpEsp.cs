using System;
using System.Data;
using System.Collections.Generic;
using System.Text;
using System.Web.Configuration;
using System.Configuration;

namespace CD40.BD.Entidades
{
    public class SectorizacionesAgpEsp :Tablas
    {
        #region Propiedades de Sectorizaciones Agrupaciones Especiales
        private string _idSistema;
        public string IdSistema
        {
            get { return _idSistema; }
            set { _idSistema = value; }
        }

        private string _IdSectorizacion;
        public string IdSectorizacion
        {
            get { return _IdSectorizacion; }
            set { _IdSectorizacion = value; }
        }

        private string _IdAgrupacion;
        public string IdAgrupacion
        {
            get { return _IdAgrupacion; }
            set { _IdAgrupacion = value; }
        }

        private string _idTop;
        public string IdTop
        {
            get { return _idTop; }
            set { _idTop = value; }
        }
        #endregion

        public SectorizacionesAgpEsp()
        {

        }

        public override string DataSetSelectSQL()
        {
            StringBuilder strCadena = new StringBuilder();

            if (IdSistema != null && IdSectorizacion != null && IdAgrupacion != null)
                strCadena.AppendFormat("SELECT * FROM sectorizacionesagpesp WHERE IdSistema='{0}' AND IdSectorizacion='{1}' AND IdAgrupacion='{2}'", IdSistema, IdSectorizacion, IdAgrupacion);
            else if (IdSistema != null && IdSectorizacion != null)
                strCadena.AppendFormat("SELECT * FROM sectorizacionesagpesp WHERE IdSistema='{0}' AND IdSectorizacion='{1}'", IdSistema, IdSectorizacion);
            else if (IdSistema != null)
                strCadena.AppendFormat("SELECT * FROM sectorizacionesagpesp WHERE IdSistema='{0}' ", IdSistema);
            else
                strCadena.Append("SELECT * FROM Sectorizaciones ");
            return strCadena.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();

            if (ds != null)
            {
                if (ds.Tables.Count > 0)
                {
                    foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                    {
                        SectorizacionesAgpEsp sae = new SectorizacionesAgpEsp();

                       
                        sae.IdSistema = (string)dr["IdSistema"];
                        sae.IdSectorizacion = (string)dr["IdSectorizacion"];
                        sae.IdAgrupacion = (string)dr["IdAgrupacion"];
                        sae.IdTop = (string)dr["IdTOP"];
                        ListaResultado.Add(sae);
                    }
                }
            }
            
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
            string[] consulta = new string[2];

            StringBuilder strCadena = new StringBuilder();

            strCadena.AppendFormat("INSERT INTO sectorizacionesagpesp (IdSistema, IdSectorizacion, IdAgrupacion, IdTop) VALUES ('{0}','{1}','{2}','{3}')", 
                                    IdSistema, IdSectorizacion, IdAgrupacion, IdTop);
            consulta[0] = strCadena.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "sectorizacionesagpesp");
            
            strCadena.Clear();
            
            return consulta;
        }

        public override string[] UpdateSQL()
        {
            return null;
        }

        public override string[] DeleteSQL()
        {
            string[] consulta = new string[2];

            StringBuilder strCadena = new StringBuilder();

            if (IdSistema != null && IdSectorizacion != null && IdAgrupacion != null && IdTop != null)
                strCadena.Append("DELETE FROM sectorizacionesagpesp WHERE IdSistema='" + IdSistema + "' AND IdSectorizacion='" + IdSectorizacion + "' AND IdAgrupacion ='" + IdAgrupacion + "´ AND IdTop='" + IdTop +"'");
            else if (IdSistema != null && IdSectorizacion != null && IdAgrupacion != null)
                strCadena.Append("DELETE FROM sectorizacionesagpesp WHERE IdSistema='" + IdSistema + "' AND IdSectorizacion='" + IdSectorizacion + "' AND IdAgrupacion ='" + IdAgrupacion + "'");
            else if (IdSistema != null && IdSectorizacion != null)
                strCadena.Append("DELETE FROM sectorizacionesagpesp WHERE IdSistema='" + IdSistema + "' AND IdSectorizacion='" + IdSectorizacion + "'");
            else if (IdSectorizacion != null)
                strCadena.Append("DELETE FROM sectorizacionesagpesp WHERE IdSistema='" + IdSistema + "'");
            else
                strCadena.Append("DELETE FROM sectorizacionesagpesp");

            consulta[0] = strCadena.ToString();
            consulta[1] = ReplaceSQL(IdSistema, "sectorizacionesagpesp");

            strCadena.Clear();

            return consulta;
        }

    }
}
