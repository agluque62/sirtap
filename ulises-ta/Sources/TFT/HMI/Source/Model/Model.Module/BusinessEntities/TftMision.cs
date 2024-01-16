﻿using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Constants;
using HMI.Model.Module.Properties;
using Utilities;

namespace HMI.Model.Module.BusinessEntities
{
    public sealed class TftMision
    {
		private string _Mision = "";

		//[EventPublication(EventTopicNames.TftMisionChangedf2, PublicationScope.Global)]
		//public event EventHandler TftMisionChangedf2;

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
						SetPagRadio(new List<(int, bool)> { (1, true), (3, true), (5, true), (7, true), (9, true) });
						SetPagTlf(new List<(int, bool)> { (2, true), (4, true), (6, true) });
						SetPagLc(new bool[] { true, false, true, false, false, false });
					}
					else if (_Mision == "MISION 3")
					{
						SetPagRadio(new List<(int, bool)> { (2, true), (4,false), (6, true), (8, true) });
						SetPagTlf(new List<(int, bool)> { (1, false), (3, true), (5, true), (7, true), (9, true) });
						SetPagLc(new bool[] { true, true, true, false, false, false });
					}
					else
					{
						SetPagRadio(new List<(int, bool)> { });
						SetPagTlf(new List<(int, bool)> { });
						SetPagLc(new bool[] { });
					}
					//General.SafeLaunchEvent(TftMisionChangedf2, this);
				}
			}
		}

		private List<(int, bool)> _pagrad;
		private List<(int, bool)> _pagtlf;
		private bool[] _paglc;
		private int _NumberOfPagesRad =0;
		private void SetPagRadio(List<(int, bool)> lista)
		{
			_pagrad = lista;
			NumberOfPagesRad = lista.Count;

		}
		private void SetPagTlf(List <(int, bool)>lista)
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
		public int paginaorden(int ordenpagina)
		{
			if (ordenpagina>0)
				return Pagrad[ordenpagina].Item1;
			return -1;
		}
	}
}
