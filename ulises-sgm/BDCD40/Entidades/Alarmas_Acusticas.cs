using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Alarmas_Acusticas : Tablas
	{
        #region Propiedades de Alarmas_Acusticas

        private uint _IdAlarmaAcustica;
        public uint IdAlarmaAcustica
        {
            get { return _IdAlarmaAcustica; }
            set { _IdAlarmaAcustica = value; }
        }

        private string _Descripcion;
        public string Descripcion
        {
            get { return _Descripcion; }
            set { _Descripcion = value; }
        }

		private uint _Tipo;
        public uint Tipo
        {
            get { return _Tipo; }
            set { _Tipo = value; }
        }

        #endregion


        public Alarmas_Acusticas()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdAlarmaAcustica != 0)
                Consulta.Append("SELECT * FROM Alarmas_Acusticas WHERE IdAlarmaAcustica=" + IdAlarmaAcustica);
            else 
                Consulta.Append("SELECT * FROM Alarmas_Acusticas");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Alarmas_Acusticas r = new Alarmas_Acusticas();

                    r.IdAlarmaAcustica = (uint)dr["IdAlarmaAcustica"];
                    r.Descripcion = (string)dr["Descripcion"];
                    r.Tipo = (uint)dr["Tipo"];

                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Alarmas_Acusticas (IdAlarmaAcustica,Descripcion,Tipo)" +
                            " VALUES (" + IdAlarmaAcustica + ",'" +
                                         Descripcion + "'," +
                                         Tipo + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Alarmas_Acusticas");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Alarmas_Acusticas SET Descripcion='" + Descripcion + "'," +
                                            "Tipo=" + Tipo + " " +
                                            " WHERE IdAlarmaAcustica=" + IdAlarmaAcustica
                                            );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Alarmas_Acusticas");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
			if (IdAlarmaAcustica != 0)
				Consulta.Append("DELETE FROM Alarmas_Acusticas WHERE IdAlarmaAcustica=" + IdAlarmaAcustica);
			else
				Consulta.Append("DELETE FROM Alarmas_Acusticas");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Alarmas_Acusticas");
			return consulta;
		}

	}
}
