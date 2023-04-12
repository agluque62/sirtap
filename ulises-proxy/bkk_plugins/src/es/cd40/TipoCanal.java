
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_Canal.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_Canal"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="Monocanal"/&gt;
 *     &lt;enumeration value="Multicanal"/&gt;
 *     &lt;enumeration value="Otros"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_Canal")
@XmlEnum
public enum TipoCanal {

    @XmlEnumValue("Monocanal")
    MONOCANAL("Monocanal"),
    @XmlEnumValue("Multicanal")
    MULTICANAL("Multicanal"),
    @XmlEnumValue("Otros")
    OTROS("Otros");
    private final String value;

    TipoCanal(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoCanal fromValue(String v) {
        for (TipoCanal c: TipoCanal.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
