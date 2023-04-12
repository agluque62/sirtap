
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_EM.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_EM"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="Type_I"/&gt;
 *     &lt;enumeration value="Type_II"/&gt;
 *     &lt;enumeration value="Type_III"/&gt;
 *     &lt;enumeration value="Type_IV"/&gt;
 *     &lt;enumeration value="Type_V"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_EM")
@XmlEnum
public enum TipoEM {

    @XmlEnumValue("Type_I")
    TYPE_I("Type_I"),
    @XmlEnumValue("Type_II")
    TYPE_II("Type_II"),
    @XmlEnumValue("Type_III")
    TYPE_III("Type_III"),
    @XmlEnumValue("Type_IV")
    TYPE_IV("Type_IV"),
    @XmlEnumValue("Type_V")
    TYPE_V("Type_V");
    private final String value;

    TipoEM(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoEM fromValue(String v) {
        for (TipoEM c: TipoEM.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
