using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace CD40.BD.Entidades
{
	public class Misiones_Operadores : Tablas
	{
        #region Propiedades de Misiones_Operadores

        private uint _IdMision;
        public uint IdMision
        {
            get { return _IdMision; }
            set { _IdMision = value; }
        }

        private string _IdOperador;
        public string IdOperador
        {
            get { return _IdOperador; }
            set { _IdOperador = value; }
        }

		private bool _Seguro;
        public bool Seguro
        {
            get { return _Seguro; }
            set { _Seguro = value; }
        }

        #endregion


        public Misiones_Operadores()
        {
        }

        public override string DataSetSelectSQL()
        {
            Consulta.Remove(0, Consulta.Length);
            if (IdMision != 0)
                Consulta.Append("SELECT * FROM Misiones_Operadores WHERE IdMision=" + IdMision);
            else 
                Consulta.Append("SELECT * FROM Misiones_Operadores");
            return Consulta.ToString();
        }

        public override System.Collections.Generic.List<Tablas> ListSelectSQL(DataSet ds)
        {
            ListaResultado.Clear();
            if (ds != null)
            {
                foreach (System.Data.DataRow dr in ds.Tables[0].Rows)
                {
                    Misiones_Operadores r = new Misiones_Operadores();

                    r.IdMision = (uint)dr["IdMision"];
                    r.IdOperador = (string)dr["IdOperador"];
                    r.Seguro = (bool)dr["Seguro"];

                    ListaResultado.Add(r);
                }
            }
            return ListaResultado;
        }

        public override string[] InsertSQL()
        {
			string[] consulta = new string[2];

            Consulta.Remove(0, Consulta.Length);
            Consulta.Append("INSERT INTO Misiones_Operadores (IdMision,IdOperador,Seguro)" +
                            " VALUES (" + IdMision +",'" + 
                                          IdOperador + "'," +
                                          Seguro + ")");
			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Operadores");
			return consulta;
        }

        public override string[] UpdateSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
            Consulta.Append("UPDATE Misiones_Operadores SET IdOperador='" + IdOperador + "'," +
                                            "Seguro=" + Seguro + " " +
                                            "WHERE IdMision=" + IdMision
                                            );

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Operadores");
			return consulta;
		}

        public override string[] DeleteSQL()
        {
			string[] consulta = new string[2];

			Consulta.Remove(0, Consulta.Length);
			if (IdMision != 0)
				Consulta.Append("DELETE FROM Misiones_Operadores WHERE IdMision=" + IdMision);
			else
				Consulta.Append("DELETE FROM Misiones_Operadores");

			consulta[0] = Consulta.ToString();
			consulta[1] = ReplaceSQL(null, "Misiones_Operadores");
			return consulta;
		}

	}
}
