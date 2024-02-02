using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Misiones_Alarmas_Acusticas : Tablas
	{
        #region Propiedades de Misiones_Alarmas_Acusticas

        private uint _IdMision;
        public uint IdMision
        {
            get { return _IdMision; }
            set { _IdMision = value; }
        }

        private uint _IdAlarmaAcustica;
        public uint IdAlarmaAcustica
        {
            get { return _IdAlarmaAcustica; }
            set { _IdAlarmaAcustica = value; }
        }
        #endregion

        public Misiones_Alarmas_Acusticas()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdMision != 0)
                Consulta.Append("SELECT * FROM Misiones_Alarmas_Acusticas WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones_Alarmas_Acusticas");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Misiones_Alarmas_Acusticas r = new Misiones_Alarmas_Acusticas();

                    r.IdMision = (uint)dr["IdMision"];
                    r.IdAlarmaAcustica = (uint)dr["IdAlarmaAcustica"];
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones_Alarmas_Acusticas (IdMision,Tipo)" +
                            " VALUES (" + IdMision + "," +
                                         IdAlarmaAcustica + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Alarmas_Acusticas");
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
			if (IdMision != 0 && IdAlarmaAcustica != 0)
                Consulta.Append("DELETE FROM Misiones_Alarmas_Acusticas WHERE IdMision=" + IdMision + " AND IdAlarmaAcustica=" + IdAlarmaAcustica);
			else if (IdMision != 0)
				Consulta.Append("DELETE FROM Misiones_Alarmas_Acusticas WHERE IdMision=" + IdMision);

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Alarmas_Acusticas");
			return consulta;
		}

	}
}
