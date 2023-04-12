
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para TipoInterface.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="TipoInterface"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="TI_Radio"/&gt;
 *     &lt;enumeration value="TI_LCEN"/&gt;
 *     &lt;enumeration value="TI_BC"/&gt;
 *     &lt;enumeration value="TI_BL"/&gt;
 *     &lt;enumeration value="TI_AB"/&gt;
 *     &lt;enumeration value="TI_ATS_R2"/&gt;
 *     &lt;enumeration value="TI_ATS_N5"/&gt;
 *     &lt;enumeration value="TI_ATS_QSIG"/&gt;
 *     &lt;enumeration value="TI_ISDN_2BD"/&gt;
 *     &lt;enumeration value="TI_ISDN_30BD"/&gt;
 *     &lt;enumeration value="TI_I_O"/&gt;
 *     &lt;enumeration value="TI_DATOS"/&gt;
 *     &lt;enumeration value="TI_RRC"/&gt;
 *     &lt;enumeration value="TI_EM_PP"/&gt;
 *     &lt;enumeration value="TI_EM_MARC"/&gt;
 *     &lt;enumeration value="TI_NOT_FOUND"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "TipoInterface")
@XmlEnum
public enum TipoInterface {

    @XmlEnumValue("TI_Radio")
    TI_RADIO("TI_Radio"),
    TI_LCEN("TI_LCEN"),
    TI_BC("TI_BC"),
    TI_BL("TI_BL"),
    TI_AB("TI_AB"),
    @XmlEnumValue("TI_ATS_R2")
    TI_ATS_R_2("TI_ATS_R2"),
    @XmlEnumValue("TI_ATS_N5")
    TI_ATS_N_5("TI_ATS_N5"),
    TI_ATS_QSIG("TI_ATS_QSIG"),
    @XmlEnumValue("TI_ISDN_2BD")
    TI_ISDN_2_BD("TI_ISDN_2BD"),
    @XmlEnumValue("TI_ISDN_30BD")
    TI_ISDN_30_BD("TI_ISDN_30BD"),
    TI_I_O("TI_I_O"),
    TI_DATOS("TI_DATOS"),
    TI_RRC("TI_RRC"),
    TI_EM_PP("TI_EM_PP"),
    TI_EM_MARC("TI_EM_MARC"),
    TI_NOT_FOUND("TI_NOT_FOUND");
    private final String value;

    TipoInterface(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoInterface fromValue(String v) {
        for (TipoInterface c: TipoInterface.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
