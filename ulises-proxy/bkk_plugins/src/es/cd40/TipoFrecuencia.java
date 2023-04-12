
package es.cd40;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tipo_Frecuencia.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * <p>
 * <pre>
 * &lt;simpleType name="Tipo_Frecuencia"&gt;
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string"&gt;
 *     &lt;enumeration value="Basica"/&gt;
 *     &lt;enumeration value="HF"/&gt;
 *     &lt;enumeration value="VHF"/&gt;
 *     &lt;enumeration value="UHF"/&gt;
 *   &lt;/restriction&gt;
 * &lt;/simpleType&gt;
 * </pre>
 * 
 */
@XmlType(name = "Tipo_Frecuencia")
@XmlEnum
public enum TipoFrecuencia {

    @XmlEnumValue("Basica")
    BASICA("Basica"),
    HF("HF"),
    VHF("VHF"),
    UHF("UHF");
    private final String value;

    TipoFrecuencia(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static TipoFrecuencia fromValue(String v) {
        for (TipoFrecuencia c: TipoFrecuencia.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
