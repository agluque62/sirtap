
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para GearCarrierOffStatus.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="GearCarrierOffStatus"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="Off"/&gt;
 *     &lt;enumeration value="kHz_7_5"/&gt;
 *     &lt;enumeration value="kHz_5_0"/&gt;
 *     &lt;enumeration value="kHz_2_5"/&gt;
 *     &lt;enumeration value="Hz_0_0"/&gt;
 *     &lt;enumeration value="kHz_minus_2_5"/&gt;
 *     &lt;enumeration value="kHz_minus_5_0"/&gt;
 *     &lt;enumeration value="kHz_minus_7_5"/&gt;
 *     &lt;enumeration value="kHz_8"/&gt;
 *     &lt;enumeration value="kHz_4"/&gt;
 *     &lt;enumeration value="kHz_minus_4"/&gt;
 *     &lt;enumeration value="kHz_minus_8"/&gt;
 *     &lt;enumeration value="kHz_7_3"/&gt;
 *     &lt;enumeration value="kHz_minus_7_3"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "GearCarrierOffStatus")
@XmlEnum
public enum GearCarrierOffStatus {

    @XmlEnumValue("Off")
    OFF("Off"),
    @XmlEnumValue("kHz_7_5")
    K_HZ_7_5("kHz_7_5"),
    @XmlEnumValue("kHz_5_0")
    K_HZ_5_0("kHz_5_0"),
    @XmlEnumValue("kHz_2_5")
    K_HZ_2_5("kHz_2_5"),
    @XmlEnumValue("Hz_0_0")
    HZ_0_0("Hz_0_0"),
    @XmlEnumValue("kHz_minus_2_5")
    K_HZ_MINUS_2_5("kHz_minus_2_5"),
    @XmlEnumValue("kHz_minus_5_0")
    K_HZ_MINUS_5_0("kHz_minus_5_0"),
    @XmlEnumValue("kHz_minus_7_5")
    K_HZ_MINUS_7_5("kHz_minus_7_5"),
    @XmlEnumValue("kHz_8")
    K_HZ_8("kHz_8"),
    @XmlEnumValue("kHz_4")
    K_HZ_4("kHz_4"),
    @XmlEnumValue("kHz_minus_4")
    K_HZ_MINUS_4("kHz_minus_4"),
    @XmlEnumValue("kHz_minus_8")
    K_HZ_MINUS_8("kHz_minus_8"),
    @XmlEnumValue("kHz_7_3")
    K_HZ_7_3("kHz_7_3"),
    @XmlEnumValue("kHz_minus_7_3")
    K_HZ_MINUS_7_3("kHz_minus_7_3");
    private final String value;

    GearCarrierOffStatus(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static GearCarrierOffStatus fromValue(String v) {
        for (GearCarrierOffStatus c: GearCarrierOffStatus.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
