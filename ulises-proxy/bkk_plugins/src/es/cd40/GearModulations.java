
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para GearModulations.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="GearModulations"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="AM"/&gt;
 *     &lt;enumeration value="Reserved"/&gt;
 *     &lt;enumeration value="ACARS"/&gt;
 *     &lt;enumeration value="VDL2"/&gt;
 *     &lt;enumeration value="AM_CT"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "GearModulations")
@XmlEnum
public enum GearModulations {

    AM("AM"),
    @XmlEnumValue("Reserved")
    RESERVED("Reserved"),
    ACARS("ACARS"),
    @XmlEnumValue("VDL2")
    VDL_2("VDL2"),
    AM_CT("AM_CT");
    private final String value;

    GearModulations(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static GearModulations fromValue(String v) {
        for (GearModulations c: GearModulations.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
