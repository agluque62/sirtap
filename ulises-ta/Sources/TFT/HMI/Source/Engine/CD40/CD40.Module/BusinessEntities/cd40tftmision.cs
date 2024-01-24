using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Linq;

using HMI.Model.Module.BusinessEntities;
using HMI.CD40.Module.Properties;

using U5ki.Infrastructure;
using Utilities;
using NLog;
using conferencia;
using static System.Windows.Forms.LinkLabel;
using NAudio.SoundFont;
using static HMI.CD40.Module.BusinessEntities.TopRegistry;

namespace HMI.CD40.Module.BusinessEntities
{

	public partial class cd40tftmision
	{
		public event GenericEventHandler RdPageClear;
		public event GenericEventHandler<List<int>> TftMisionChangedf2;
		public event GenericEventHandler<int> RadioChanged;

		private string _Mision = "";
		private List<(int, bool)> _pagrad;
		private List<(int, bool)> _pagtlf;
		private bool[] _paglc;
		private int _NumberOfPagesRad = 0;
		public static int NumDestinations = 6;
		private LcDst[] _Dst = new LcDst[NumDestinations];
		public event GenericEventHandler StChanged;
		public cd40tftmision()
		{
			Mision = "MISION 1";
		}
		public string Mision
		{

			get { return _Mision; }
			set
			{
				if (_Mision != value)
				{
					_Mision = value;
					string msg = _Mision;
					if (_Mision == "MISION 1")
					{
						SetPagRadio(new List<(int, bool)> { (0, true), (1, true), (2, true) });
						SetPagTlf(new List<(int, bool)> { (0, true), (2, true), (4, true) });
						SetPagLc(new bool[] { true, false, true, false, false, false });
						//General.SafeLaunchEvent(RadioChanged, this, new RangeMsg(0, NumDestinations));
						General.SafeLaunchEvent(RadioChanged, this, NumDestinations);

						//_StateManager.Radio.ResetAssignatedState();
						//General.SafeLaunchEvent(RadioChanged, this);
					}
					else if (_Mision == "MISION 3")
					{
						SetPagRadio(new List<(int, bool)> { (2, true), (4, false), (6, true), (8, true) });
						SetPagTlf(new List<(int, bool)> { (1, false), (3, true), (5, true), (7, true), (9, true) });
						SetPagLc(new bool[] { true, true, true, false, false, false });
					}
					else
					{
						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { });
						SetPagLc(new bool[] { });

						General.SafeLaunchEvent(RdPageClear, this);

					}
					//General.SafeLaunchEvent(TftMisionChangedf2, this);
				}
			}
		}

		private void SetPagRadio(List<(int, bool)> lista)
		{
			_pagrad = lista;
			NumberOfPagesRad = lista.Count;

		}
		private void SetPagTlf(List<(int, bool)> lista)
		{
			_pagtlf = lista;
		}
		private void SetPagLc(bool[] lista)
		{
			_paglc = lista;
		}
		public List<(int, bool)> Pagrad
		{
			get
			{
				return _pagrad;
			}
			set => _pagrad = value;
		}
		public int GetNumberOfPagesRd()
		{
			return NumberOfPagesRad;
		}
		public List<(int, bool)> Pagtlf
		{
			get
			{
				return _pagtlf;
			}
			set => _pagtlf = value;
		}
		public bool[] PagLc
		{
			get
			{
				return _paglc;
			}
			set => _paglc = value;
		}
		public int NumberOfPagesRad { get => _NumberOfPagesRad; set => _NumberOfPagesRad = value; }
		public int ordenpaginaradio(int numero_pagina_buscada)
		{
			int orden_pagina = -1;
			for (int i = 0; i < Pagrad?.Count; i++)
			{
				if (Pagrad[i].Item1 == numero_pagina_buscada)
				{
					orden_pagina = i; // Sumamos 1 ya que los índices suelen empezar desde 0
					break;
				}
			}
			return orden_pagina;
		}
		public int ordenpaginatlf(int numero_pagina_buscada)
		{
			int orden_pagina = -1;
			for (int i = 0; i < Pagtlf?.Count; i++)
			{
				if (Pagtlf[i].Item1 == numero_pagina_buscada)
				{
					orden_pagina = i; // Sumamos 1 ya que los índices suelen empezar desde 0
					break;
				}
			}
			return orden_pagina;
		}
		public int numero_pagina_mas_alto_rad()
		{
			int indice_mas_alto = Pagrad?.Count ?? -1;
			if (indice_mas_alto > -1)
				return Pagrad[indice_mas_alto - 1].Item1;
			return indice_mas_alto;
		}
		public int numero_pagina_mas_alto_tlf()
		{
			int indice_mas_alto = Pagtlf?.Count ?? -1;
			if (indice_mas_alto > 0)
				return Pagtlf[indice_mas_alto - 1].Item1;
			return indice_mas_alto;
		}
		public int paginaorden(int ordenpagina)
		{
			if (ordenpagina > 0)
				return Pagrad[ordenpagina].Item1;
			return -1;
		}
	}
}
