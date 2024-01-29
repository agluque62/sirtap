using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Constants;
using HMI.Model.Module.Properties;
using Utilities;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Services;
using System.Linq;

namespace HMI.Model.Module.BusinessEntities
{
	public sealed class TftMision
	{
		private IEngineCmdManagerService _EngineCmdManager;

		private string _Mision = null;
		private List<(int, bool)> _pagrad;
		private List<(int, bool)> _pagtlf;
		private List<(bool, bool)> _paglc;
		Dictionary<uint, uint> _newpaglc = new Dictionary<uint, uint>();
		private int _NumberOfPagesRad = 0;
		private int _NumberOfPosLc = 0;
		public static int NumDestinations = Settings.Default.NumRdDestinations;
		public TftMision hinstance = null;
		private LcDst[] _Dst = new LcDst[NumDestinations];
		public event GenericEventHandler StChanged;

		private static TftMision _instance = null;
		public static TftMision Instance
		{
			get
			{
				if (_instance == null)
				{
					_instance = new TftMision();
				}
				return _instance;
			}
		}
		public TftMision()
		{
			_Mision = "";
			//Instance.Mision = _Mision;
			//var a = Instance;
		}
		public static TftMision Hinstance { get; set; }
		[EventPublication(EventTopicNames.RdPageClear, PublicationScope.Global)]
		public event EventHandler RdPageClear;
		[EventPublication(EventTopicNames.TftMisionChangedf2, PublicationScope.Global)]
		public event EventHandler TftMisionChangedf2;
		[EventPublication(EventTopicNames.RadioChanged, PublicationScope.Global)]
		public event EventHandler<RangeMsg> RadioChanged;

		public string Mision
		{

			get
			{
				if (_Mision==null)
					return null;
				return _Mision;
			}
			set
			{
				if (_Mision != value)
				{
					if (value == "")
						_Mision = null;
					else
						_Mision = value;
					string msg = _Mision;
					if (_Mision == "MISION 1")
					{
						SetPagRadio(new List<(int, bool)> { (0, true), (1, true), (2, true) });
						//SetPagTlf(new List<(int, bool)> { (0, true), (1, true), (2, true), (3, true), (4, true), (5, true), (6, true) });
						SetPagTlf(new List<(int, bool)> { (0, true) });
						SetPagLc(new List<(bool, bool)> { (false, true), (false, true), (true, true), (true, true), (true, true), (true, true) });
						General.SafeLaunchEvent(RadioChanged, this, new RangeMsg(0, NumDestinations));

						//_StateManager.Radio.ResetAssignatedState();
						//General.SafeLaunchEvent(RadioChanged, this);
					}
					else if (_Mision == "MISION 2")
					{
						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { (1, false), (3, true), (5, true), (7, true), (9, true) });
						SetPagLc(new List <(bool,bool)> { (true, true), (true, true), });
					}
					else if (_Mision == "MISION 3")
					{
						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { (0, true), (1, true), (2, true), (3, true), (4, true), (5, true), (6, true) });
						SetPagLc(new List<(bool, bool)> { (true, true),(true, true), (true, true), });
					}
					else if (_Mision == "MISION 4")
					{
						SetPagRadio(new List<(int, bool)> { (4, true) });
						SetPagTlf(new List<(int, bool)> { (4, true) });
						SetPagLc(new List<(bool, bool)> { (true, true), (true, true), (true, true), (true, true), });
					}
					else if (_Mision == "MISION 5")
					{
						SetPagRadio(new List<(int, bool)> { (4, true) });
						SetPagTlf(new List<(int, bool)> { (4, true) });
						SetPagLc(new List<(bool, bool)> { (true, true), (true, true), (true, true), (true, true), (true, true), });
					}
					else if (_Mision == "MISION 6")
					{
						SetPagRadio(new List<(int, bool)> { (4, true) });
						SetPagTlf(new List<(int, bool)> { (4, true) });
						SetPagLc(new List<(bool, bool)> { (true, true), (true, true), (true, true), (true, true), (true, true), (true, true), });
					}
					else if (_Mision == "MISION 7")
					{
						SetPagRadio(new List<(int, bool)> { (4, true) });
						SetPagTlf(new List<(int, bool)> { (4, true) });
						SetPagLc(new List<(bool, bool)> { (false, true), //1
														  (true, true), //2
														  (true, true), //3
														  (true, true), //4
														  (true, true), //5
														  (true, true), //6
							  							  (true, true), //7
						});
					}
					else if (_Mision == "MISION 8")
                    {

						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { });
						SetPagLc(new List<(bool, bool)> { (false, true), //1
														  (false, true), //2
														  (false, true), //3
														  (false, true), //4
														  (false, true), //5
														  (false, true), //6
							  							  (true, true), //7
						});
					}
					else
					{

						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { });
						SetPagLc(new List <(bool,bool)> { });

						General.SafeLaunchEvent(RdPageClear, this);

					}
					General.SafeLaunchEvent(TftMisionChangedf2, this);
				}
			}
		}
		public void SetUserClave(string nombre, string clave)
		{
			_EngineCmdManager?.EnviarLoginCorrectoEngine(nombre, clave);
		}

		private void SetPagRadio(List<(int, bool)> lista)
		{
			_pagrad = lista;
			NumberOfPagesRad = lista.Count;

		}
		private void SetPagTlf(List <(int, bool)>lista)
		{
			_pagtlf = lista;
		}
		private void SetPagLc(List <(bool,bool)> lista)
		{
			_paglc = lista;
			NumberOfPosLc = lista.Count(i => i.Item1);
			uint cont = 0;
			_newpaglc=new Dictionary<uint, uint>();
			for (int i = 0;i< lista.Count; i++)
            {
				if (lista[i].Item1)
				{
					_newpaglc.Add((uint)i+1, cont+1);
					cont++;
				}
			}
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
		public List<(bool, bool)> PagLC
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
					orden_pagina = i; 
					break;
				}
			}
			return orden_pagina;
		}
		public int NumberOfPosLc { get => _NumberOfPosLc; set => _NumberOfPosLc = value; }

		public uint ordenposlc(int numero_posicion_buscada)
		{
			int [] poslc= new int[18];
			uint orden_pos = 0;
			if (PagLC?.Count > 6)
			{
				for (int i = 0; i < PagLC?.Count; i++)
				{
					// si hay mas de 6 elementos reordeno la lista.
					if (PagLC[i].Item1)
					{
						poslc[i]=(int)orden_pos;
						orden_pos++;
					}
					// si hay 6 o menos elementos no reordeno la lista
					else if (PagLC?.Count <= 6)
						orden_pos++;
				}
			}
            else
            {
				// cargo su posicion natural del 1 al 6
				for (int i = 0; i < PagLC?.Count; i++)
				{
					poslc[i] = (int)orden_pos+1;
					orden_pos++;
				}

			}
			for (uint i = 0; i < poslc.Length; i++)
			{
				if (poslc[i] == numero_posicion_buscada)
				{
					orden_pos = i; 
					break;
				}
			}
			return orden_pos;
		}
		public int numero_pagina_mas_alto_rad()
		{
			int indice_mas_alto = Pagrad?.Count ?? -1;
			if (indice_mas_alto > 0)
				return Pagrad[indice_mas_alto-1].Item1;
			return indice_mas_alto;
		}
		public int numero_pagina_mas_alto_tlf()
		{
			int indice_mas_alto = Pagtlf?.Count ?? -1;
			if (indice_mas_alto > 0)
				return Pagtlf[indice_mas_alto-1].Item1;
			return indice_mas_alto;
		}
		public int paginaorden(int ordenpagina)
		{
			if (ordenpagina>0)
				return Pagrad[ordenpagina].Item1;
			return -1;
		}
		public bool IsPagTlf(uint pag)
		{
			for (int i = 0; i < Pagtlf?.Count; i++)
			{
				if (Pagtlf[i].Item1 == pag)
					return true;
			}
			return false;
		}
		public bool IsPagRad(uint pag)
		{
			for (int i = 0; i < Pagrad?.Count; i++)
			{
				if (Pagrad[i].Item1 == pag)
					return true;
			}
			return false;
		}
		public bool IsPosLC(uint pos)
		{
			for (int i = 0; i < PagLC?.Count; i++)
			{
				if (i==pos)
				if (PagLC[i].Item1)
					return true;
			}
			return false;
		}
		public uint OrdenLC(uint pos)
		{
			return _newpaglc[pos];
		}

		public uint NumFrecPagina = 6;
		public uint NumEnlacesInternosPag = 6;
		public uint NumPagEnlacesInt = 28;

		
	}
}
