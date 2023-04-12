using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Configuraciones : Tablas
	{
        #region Propiedades de Configuraciones

        private string _Nombre;
        public string Nombre
        {
            get { return _Nombre; }
            set { _Nombre = value; }
        }
        
        private string _Conf_Edicion;
        public string Conf_Edicion
        {
            get { return _Conf_Edicion; }
            set { _Conf_Edicion = value; }
        }
        private string _Tipo;
        public string Tipo
        {
            get { return _Tipo; }
            set { _Tipo = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }

        #endregion


        public Configuraciones()
        {

        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (Nombre != null)
                Consulta.Append("SELECT * FROM Configuraciones WHERE Nombre='" + Nombre + "'");
            else
                Consulta.Append("SELECT * FROM Configuraciones");

            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Configuraciones c = new Configuraciones();

                    c.Nombre = (string)dr["NOMBRE"];
                    c.Conf_Edicion = (string)dr["CONF_EDICION"];
                    c.Tipo = (string)dr["TIPO"];
                    c.Descripcion = (string)dr["DESCRIPCION"];
                    ListaResultado.Add(c);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO configuraciones (NOMBRE,CONF_EDICION,TIPO, DESCRIPCION)" +
                            " VALUES ('" + Nombre + "','" +
                                         Conf_Edicion + "','" +
                                         Tipo + "','" +
                                         Descripcion + "')");
                                        

			consulta[0] = Consulta.ToString();
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Configuraciones SET DESCRIPCION= '" + Descripcion.Replace("\'", "\\'") + "' WHERE NOMBRE= GetNombreConf('" + Nombre + "')");

			consulta[0] = Consulta.ToString();
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            if (Nombre != null)
				Consulta.Append("DELETE FROM Configuraciones WHERE NOMBRE='" + Nombre + "'");
			else
				Consulta.Append("DELETE FROM Configuraciones");

			consulta[0] = Consulta.ToString();

			return consulta;
		}

	}
}
