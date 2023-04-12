
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para GearPowerLevels.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="GearPowerLevels"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="PowerLevelsDefault"/&gt;
 *     &lt;enumeration value="Low"/&gt;
 *     &lt;enumeration value="Normal"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "GearPowerLevels")
@XmlEnum
public enum GearPowerLevels {

    @XmlEnumValue("PowerLevelsDefault")
    POWER_LEVELS_DEFAULT("PowerLevelsDefault"),
    @XmlEnumValue("Low")
    LOW("Low"),
    @XmlEnumValue("Normal")
    NORMAL("Normal");
    private final String value;

    GearPowerLevels(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static GearPowerLevels fromValue(String v) {
        for (GearPowerLevels c: GearPowerLevels.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
