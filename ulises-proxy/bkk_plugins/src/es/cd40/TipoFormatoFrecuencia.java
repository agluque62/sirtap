
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_Formato_Frecuencia.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_Formato_Frecuencia"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="MHz"/&gt;
 *     &lt;enumeration value="Hz"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_Formato_Frecuencia")
@XmlEnum
public enum TipoFormatoFrecuencia {

    @XmlEnumValue("MHz")
    M_HZ("MHz"),
    @XmlEnumValue("Hz")
    HZ("Hz");
    private final String value;

    TipoFormatoFrecuencia(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoFormatoFrecuencia fromValue(String v) {
        for (TipoFormatoFrecuencia c: TipoFormatoFrecuencia.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
