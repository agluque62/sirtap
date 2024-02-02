using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Misiones_LCEN : Tablas
	{
        #region Propiedades de Misiones_LCEN

        private uint _IdMision;
        public uint IdMision
        {
            get { return _IdMision; }
            set { _IdMision = value; }
        }

        private uint _PosHMI;
        public uint PosHMI
        {
            get { return _PosHMI; }
            set { _PosHMI = value; }
        }
        #endregion

        public Misiones_LCEN()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdMision != 0)
                Consulta.Append("SELECT * FROM Misiones_LCEN WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones_LCEN");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Misiones_LCEN r = new Misiones_LCEN();

                    r.IdMision = (uint)dr["IdMision"];
                    r.PosHMI = (uint)dr["PosHMI"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones_LCEN (IdMision,PosHMI)" +
                            " VALUES (" + IdMision + "," +
                                         PosHMI + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_LCEN");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			return null;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
			if (IdMision != 0 && PosHMI != 0)
                Consulta.Append("DELETE FROM Misiones_LCEN WHERE IdMision=" + IdMision + " AND PosHMI=" + PosHMI);
			else if (IdMision != 0)
				Consulta.Append("DELETE FROM Misiones_LCEN WHERE IdMision=" + IdMision);

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_LCEN");
			return consulta;
		}

	}
}
