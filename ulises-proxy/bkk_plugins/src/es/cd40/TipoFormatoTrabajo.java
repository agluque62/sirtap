
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_Formato_Trabajo.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_Formato_Trabajo"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="Principal"/&gt;
 *     &lt;enumeration value="Reserva"/&gt;
 *     &lt;enumeration value="Ambos"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_Formato_Trabajo")
@XmlEnum
public enum TipoFormatoTrabajo {

    @XmlEnumValue("Principal")
    PRINCIPAL("Principal"),
    @XmlEnumValue("Reserva")
    RESERVA("Reserva"),
    @XmlEnumValue("Ambos")
    AMBOS("Ambos");
    private final String value;

    TipoFormatoTrabajo(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoFormatoTrabajo fromValue(String v) {
        for (TipoFormatoTrabajo c: TipoFormatoTrabajo.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
