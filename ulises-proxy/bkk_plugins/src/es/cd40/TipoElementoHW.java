
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_Elemento_HW.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_Elemento_HW"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="TEH_TOP"/&gt;
 *     &lt;enumeration value="TEH_TIFX"/&gt;
 *     &lt;enumeration value="TEH_EXTERNO_RADIO"/&gt;
 *     &lt;enumeration value="TEH_EXTERNO_TELEFONIA"/&gt;
 *     &lt;enumeration value="TEH_SISTEMA"/&gt;
 *     &lt;enumeration value="TEH_GRABADOR"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_Elemento_HW")
@XmlEnum
public enum TipoElementoHW {

    TEH_TOP,
    TEH_TIFX,
    TEH_EXTERNO_RADIO,
    TEH_EXTERNO_TELEFONIA,
    TEH_SISTEMA,
    TEH_GRABADOR;

    public String value() {
        return name();
    }

    public static TipoElementoHW fromValue(String v) {
        return valueOf(v);
    }

}
