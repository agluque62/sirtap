
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para GearChannelSpacings.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="GearChannelSpacings"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="ChannelSpacingsDefault"/&gt;
 *     &lt;enumeration value="kHz_8_33"/&gt;
 *     &lt;enumeration value="kHz_12_5"/&gt;
 *     &lt;enumeration value="kHz_25_00"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "GearChannelSpacings")
@XmlEnum
public enum GearChannelSpacings {

    @XmlEnumValue("ChannelSpacingsDefault")
    CHANNEL_SPACINGS_DEFAULT("ChannelSpacingsDefault"),
    @XmlEnumValue("kHz_8_33")
    K_HZ_8_33("kHz_8_33"),
    @XmlEnumValue("kHz_12_5")
    K_HZ_12_5("kHz_12_5"),
    @XmlEnumValue("kHz_25_00")
    K_HZ_25_00("kHz_25_00");
    private final String value;

    GearChannelSpacings(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static GearChannelSpacings fromValue(String v) {
        for (GearChannelSpacings c: GearChannelSpacings.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
