using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using HMI.Model.Module.BusinessEntities;

namespace HMI.Model.Module.Messages
{
	public sealed class LcDestination
	{
		public readonly string Dst = "";
		public readonly int Group = 0;

		public LcDestination(string dst, int group)
		{
			Dst = dst;
			Group = group;
		}

		public override string ToString()
		{
			return string.Format("[Dst={0}] [Group={1}]", Dst, Group); 
		}
	}

	public sealed class LcState
	{
		public LcRxState Rx;
		public LcTxState Tx;

		public LcState() 
			: this(LcRxState.Idle, LcTxState.Idle)
		{
		}

		public LcState(LcRxState rx, LcTxState tx)
		{
			Rx = rx;
			Tx = tx;
		}

		public override string ToString()
		{
			return string.Format("[Rx={0}] [Tx={1}]", Rx, Tx);
		}
	}

	public sealed class LcInfo
	{
		public readonly string Dst;
		public readonly LcRxState Rx;
		public readonly LcTxState Tx;
		public readonly int Group;

		public LcInfo(string dst, LcRxState rx, LcTxState tx, int group)
		{
			Dst = dst;
			Rx = rx;
			Tx = tx;
			Group = group;
		}

		public override string ToString()
		{
			return string.Format("[Dst={0}] [Rx={1}] [Tx={2}] [Group={3}]", Dst, Rx, Tx, Group); 
		}
	}

	public sealed class TlfDestination
	{
		public readonly string Dst;

		public TlfDestination(string dst)
		{
			Dst = dst;
		}

		public override string ToString()
		{
			return string.Format("[Dst={0}]", Dst);
		}
	}

	public sealed class TlfInfo
	{
		public readonly string Dst;
		public readonly TlfState St;
        public readonly bool _PriorityAllowed;
        public readonly TlfType _Type;
        public readonly bool _IsTop;
        public readonly bool _AllowsForward; //Point to point telephony destination CGW
		//LALM 211007
		//#2629 Presentar via utilizada en llamada saliente.
		public readonly string _recused;


        
        public TlfInfo(string dst, TlfState st, bool priorityAllowed, TlfType type, bool isTop, bool allowsForward, string recused = "")
		{
			Dst = dst;
			St = st;
            _PriorityAllowed = priorityAllowed;
            _Type = type;
            _IsTop = isTop;
            _AllowsForward = allowsForward;
			//LALM 211007
			//#2629 Presentar via utilizada en llamada saliente.
			// Este parametro ira en la proxima revision.
			if (recused != "" && recused!=null)

                if (recused.IndexOf("rs=") > 0)
                {


					_recused = StringUtil.ObtenerStringEntreMarcadores(recused);
                }
                else if (recused.IndexOf("sip:") > 0)
				{
					int begin = recused.IndexOf("sip:");
					_recused = "";
					// Si se quisiera presentar el abonado
					//_recused = " (" + recused.Substring(begin + 4).Split('@')[0] + ")";
				}
				else
                {
					_recused = "";
				}
					
		}

		public override string ToString()
		{
            return string.Format("[Dst={0}] [State={1}] [PrioAllowed={2}] [Type={3}] [IsTop={4}] [ForwAllowed={4}]", Dst, St, _PriorityAllowed, _Type, _IsTop, _AllowsForward);
        }
    }

	public sealed class RdDestination
	{
		public readonly string Dst;
		public readonly string Alias;
		public readonly string IdFrecuency;//RQF34 Nuevo parametro IdFrecuency, Dst es NameFrecuency

		// RQF34 Este constructor desaparecer�
		public RdDestination(string dst, string alias)
		{
			Dst = dst;
			Alias = alias;
		}

		// RQF34 
		public RdDestination(string idfrecuency, string dst, string alias)
		{
			Dst = dst;
			Alias = alias;
			IdFrecuency = idfrecuency;
		}

		public override string ToString()
		{
			return string.Format("[Dst={0}] [Alias={1}]", Dst, Alias);
		}
	}

	public sealed class RdAsignState
	{
		public bool Tx;
		public bool Rx;
		public RdRxAudioVia AudioVia;

		public RdAsignState()
			: this(false, false, RdRxAudioVia.NoAudio)
		{
		}

		public RdAsignState(bool tx, bool rx, RdRxAudioVia audio)
		{
			Tx = tx;
			Rx = rx;
			AudioVia = audio;
		}

		public override string ToString()
		{
			return string.Format("[Rx={0}] [Tx={1}] [AudioVia={2}]", Rx, Tx, AudioVia);
		}
	}

	public sealed class RdRtxGroup
	{
		public readonly int RtxGroup;

		public RdRtxGroup(int rtxGroup)
		{
			RtxGroup = rtxGroup;
		}

		public override string ToString()
		{
			return string.Format("[RtxGroup={0}]", RtxGroup);
		}
	}

	public sealed class selectable_frequencies
	{
        List <string> sel_frecuency;
		int index_to_selectable_frequencies;
		string SelectedFrecuency;

        public selectable_frequencies() {
			sel_frecuency = new List<string>();
            SelectedFrecuency = "";
		}

		public void setnewfrecuency(string value)
		{
			sel_frecuency.Add(value);
		}

        public List<string> getfrecuencies()
        {
            return sel_frecuency;
        }

        public void setSelectedFrecuency(string selected_frecuency)
        {
            SelectedFrecuency = selected_frecuency;
        }

        public string getSelectedFrecuency()
        {
			return SelectedFrecuency;
        }
		public bool multifrecuencia()
		{
			return (sel_frecuency.Count > 1); 
        }
    }

    public sealed class RdInfo
	{
		// Informacion para poder pintar la poscion de una radio.
		// Incluye el estado de la frecuencia (degradada, disponible o no).
		public readonly string Dst;
		public readonly string Alias;
		public string DescDestino;//RQF34
		public readonly bool Tx;
		public readonly bool Rx;
		public readonly PttState Ptt;
		public readonly SquelchState Squelch;
		public readonly RdRxAudioVia AudioVia;
		public readonly int RtxGroup;
		public readonly TipoFrecuencia_t TipoFrecuencia;
		public readonly bool Monitoring;
		public readonly bool FrecuenciaNoDesasignable;
		public readonly FrequencyState Estado;
		public readonly bool RxOnly;
		/** 20180321. AGL. ALIAS a mostrar en la tecla... */
		public string KeyAlias { get; set; }
		public int Priority { get; set; }
		public readonly string IdFrecuency;
		public readonly string selectable_frequency;
		//multifrecuencia
		public selectable_frequencies frecuencia_sel = new selectable_frequencies();
		public RdInfo(string dst, string alias, bool tx, bool rx, PttState ptt, SquelchState squelch,
			RdRxAudioVia audioVia, int rtxGroup, TipoFrecuencia_t tipoFrecuencia, bool monitoring, bool frecuencianodesasignable, FrequencyState estado, bool rxOnly, selectable_frequencies sf)
		{
			Dst = dst;
			Alias = alias;
			Tx = tx;
			Rx = rx;
			Ptt = ptt;
			Squelch = squelch;
			AudioVia = audioVia;
			RtxGroup = rtxGroup;
			TipoFrecuencia = tipoFrecuencia;
			Monitoring = monitoring;
			FrecuenciaNoDesasignable = frecuencianodesasignable;//RQf-14
			Estado = estado;
			RxOnly = rxOnly;
			frecuencia_sel = new selectable_frequencies();
			foreach (string s in sf.getfrecuencies())
			{
				frecuencia_sel.setnewfrecuency(s);
			}

        }

		public bool Multifrecuencia()
		{
            return frecuencia_sel.getfrecuencies().Count > 1;
        }

        public override string ToString()
		{
			//RQf-14
            return string.Format("[Dst={0}] [Alias={1}] [Rx={2}] [Tx={3}] [Ptt={4}] [Squelch={5}] [AudioVia={6}] [RxtGroup={7}] [TipoFrecuencia={8}] [Monitoring={9}] [FrecuenciaNoDesasignable={10}] [Estado={11}] [RxOnly={12}] [DescDestino={13}]", Dst, Alias, Rx, Tx, Ptt, Squelch, AudioVia, RtxGroup, TipoFrecuencia, Monitoring, FrecuenciaNoDesasignable, Estado, RxOnly,DescDestino);
		}
	}

	public sealed class RdState
	{
		public readonly bool Tx;
		public readonly bool Rx;
		public readonly PttState Ptt;
		public readonly SquelchState Squelch;
		public readonly RdRxAudioVia AudioVia;
		public readonly int RtxGroup;
        public readonly FrequencyState State;
        // BSS Information
        public readonly string QidxMethod;
        public readonly uint QidxValue;
        public readonly string QidxResource;
        /** 20190205 RTX Information */
        public readonly string PttSrcId;
		public string SelectedFrecuency;

		public RdState(bool tx, bool rx, string pttSrcId, PttState ptt, SquelchState squelch, RdRxAudioVia audioVia, int rtxGroup, FrequencyState state,
            string qidxMethod, uint qidxValue, string qidxResource,string selectedfrecuency)
		{
			Tx = tx;
			Rx = rx;
			Ptt = ptt;
			Squelch = squelch;
			AudioVia = audioVia;
			RtxGroup = rtxGroup;
            State = state;
		    // BSS Information
            QidxMethod = qidxMethod;
            QidxValue = qidxValue;
            QidxResource = qidxResource;

            PttSrcId = pttSrcId;
			SelectedFrecuency = selectedfrecuency;
        }

		public override string ToString()
		{
			return string.Format("[Rx={0}] [Tx={1}] [Ptt={2}] [Squelch={3}] [AudioVia={4}] [RxtGroup={5}] [FrequencyState={6}]", Rx, Tx, Ptt, Squelch, AudioVia, RtxGroup, State);
		}
	}

     public sealed class TlfIaDestination
	{
		public string Alias;
		public string Number;
		public TlfState State;
        public readonly bool _IsTop;
        public readonly bool _AllowsForward;
		//LALM 211008
		//#2629 Presentar via utilizada en llamada saliente.
		public string _recused;
        
        
		public TlfIaDestination(string alias, string number, TlfState state, bool isTop = true, bool allowsForward = true, string recused = "")
		{
			Alias = alias;
			Number = number;
			State = state;
            _IsTop = isTop;
            _AllowsForward = allowsForward;
			//LALM 211008
			//#2629 Presentar via utilizada en llamada saliente.
			if (recused != null && recused.IndexOf("cd40rs=") > 0)
			{
                _recused = StringUtil.ObtenerStringEntreMarcadores(recused);
            }
            else
				_recused = "";
		}

		public override string ToString()
		{
            return string.Format("[Alias={0}] [Number={1}] [State={2} [IsTop={3}]", Alias, Number, State, _IsTop);
		}
	}

	public class RangeMsg : EventArgs
	{
		public readonly int From;
		public readonly int Count;

		public RangeMsg(int from, int count)
		{
			From = from;
			Count = count;
		}

		public override string ToString()
		{
			return string.Format("[From={0}] [Count={1}]", From, Count);
		}
	}

	public class ParametrosReplay : EventArgs
	{
        private long tiempo;
        private bool _HaveFiles;
		public bool Refresco = false;
		public ParametrosReplay(long tiempo)
		{
			Tiempo= tiempo;
			HaveFiles = false;
		}

        public bool HaveFiles { get => _HaveFiles; set => _HaveFiles = value; }
        public long Tiempo
		{
			get => tiempo;
			set
			{
				tiempo = value;
				HaveFiles = (tiempo > 0);
			}
		}
		
        public int GetTiempo()
		{
			return (int)Tiempo;
		}
	}

	public sealed class RangeMsg<T> : RangeMsg
	{
		public readonly T[] Info;

		public RangeMsg(int from, int count) : base(from, count)
		{
			Info = new T[count];
		}

		public RangeMsg(int id, T info) : base(id, 1)
		{
			Info = new T[1] { info };
		}

		public RangeMsg(int from, int count, T info) : base(from, count)
		{
			Info = new T[count];

			for (int i = 0; i < count; i++)
			{
				Info[i] = info;
			}
		}

		public RangeMsg(T[] info) : base(0, info.Length)
		{
			Info = info;
		}

		public RangeMsg(int from, int count, IEnumerable<T> info) : base(from, count)
		{
			Info = new T[count];

			int i = 0;
			foreach (T el in info)
			{
				Info[i++] = el;
			}
		}

		public override string ToString()
		{
			if (Count == 1)
			{
				return string.Format("[Id={0}] {1}", From, Info[0]);
			}
			else
			{
				StringBuilder str = new StringBuilder(base.ToString());

				for (int i = 0; i < Count; i++)
				{
					str.AppendFormat("{0}\t[{1}] {2}", Environment.NewLine, i + From, Info[i]);
				}
				return str.ToString();
			}
		}
	}


 	public static class StringUtil
	{
        public static string ObtenerStringEntreMarcadores(string cadena)
        {
            if (string.IsNullOrEmpty(cadena))
                return string.Empty; // Si la cadena es null o est� vac�a, retorna una cadena vac�a directamente

            string marcadorInicio = "cd40rs=";
            char[] caracteresFinales = { ';', '>', ']' };
            int startIndex = cadena.IndexOf(marcadorInicio); // Busca el �ndice del inicio de la subcadena
            int subcadenaInicioIndex = startIndex + marcadorInicio.Length; // Calcula el �ndice de inicio de la subcadena real
            string subcadenaRecortada = cadena.Substring(subcadenaInicioIndex); // Recorta la cadena desde el �ndice de inicio
            int lastIndex = subcadenaRecortada.IndexOfAny(caracteresFinales); // Busca el �ndice del �ltimo car�cter final encontrado
            if (lastIndex != -1)
            {
                int subcadenaLength = lastIndex;
                string subcadena = cadena.Substring(subcadenaInicioIndex, subcadenaLength); // Obtiene la subcadena
                return "[" + subcadena + "]";
            }
            return string.Empty;
        }
    }
}
