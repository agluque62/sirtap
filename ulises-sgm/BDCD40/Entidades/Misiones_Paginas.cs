using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Misiones_Paginas : Tablas
	{
        #region Propiedades de Misiones_Paginas

        private uint _IdMision;
        public uint IdMision
        {
            get { return _IdMision; }
            set { _IdMision = value; }
        }

        private uint _IdPagina;
        public uint IdPagina
        {
            get { return _IdPagina; }
            set { _IdPagina = value; }
        }
        private uint _TipoDestino;
        public uint TipoDestino
        {
            get { return _TipoDestino; }
            set { _TipoDestino = value; }
        }
        // Modo de Operación
        private bool _Seguro;
        public bool Seguro
        {
            get { return _Seguro; }
            set { _Seguro = value; }
        }
        #endregion

        public Misiones_Paginas()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdMision != 0 &&  TipoDestino != 99)
                Consulta.Append("SELECT * FROM Misiones_Paginas WHERE IdMision=" + IdMision + " AND TipoDestino=" + TipoDestino);
            else if (IdMision != 0 )
                Consulta.Append("SELECT * FROM Misiones_Paginas WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones_Paginas");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Misiones_Paginas r = new Misiones_Paginas();

                    r.IdMision = (uint)dr["IdMision"];
                    r.IdPagina = (uint)dr["IdPagina"];
                    r.TipoDestino = (uint)dr["TipoDestino"];
                    r.Seguro = Convert.ToBoolean(dr["Seguro"]);
                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones_Paginas (IdMision,IdPagina,TipoDestino,Seguro)" +
                            " VALUES (" + IdMision + "," +
                                          IdPagina + "," +
                                          TipoDestino + "," +
                                          Seguro + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Paginas");
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
			if (IdMision != 0 && IdPagina != 0)
                Consulta.Append("DELETE FROM Misiones_Paginas WHERE IdMision=" + IdMision + " AND IdPagina=" + IdPagina);
			else if (IdMision != 0)
				Consulta.Append("DELETE FROM Misiones_Paginas WHERE IdMision=" + IdMision);

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Paginas");
			return consulta;
		}

	}
}
