
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgRecursoEnlaceInterno complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgRecursoEnlaceInterno"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Prefijo" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="NombreRecurso" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumeroAbonado" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgRecursoEnlaceInterno", propOrder = {
    "prefijo",
    "nombreRecurso",
    "numeroAbonado"
})
@XmlSeeAlso({
    CfgRecursoEnlaceInternoConInterface.class
})
public class CfgRecursoEnlaceInterno {

    @XmlElement(name = "Prefijo")
    @XmlSchemaType(name = "unsignedInt")
    protected long prefijo;
    @XmlElement(name = "NombreRecurso")
    protected String nombreRecurso;
    @XmlElement(name = "NumeroAbonado")
    protected String numeroAbonado;

    /**
     * Obtiene el valor de la propiedad prefijo.
     * 
     */
    public long getPrefijo() {
        return prefijo;
    }

    /**
     * Define el valor de la propiedad prefijo.
     * 
     */
    public void setPrefijo(long value) {
        this.prefijo = value;
    }

    /**
     * Obtiene el valor de la propiedad nombreRecurso.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombreRecurso() {
        return nombreRecurso;
    }

    /**
     * Define el valor de la propiedad nombreRecurso.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombreRecurso(String value) {
        this.nombreRecurso = value;
    }

    /**
     * Obtiene el valor de la propiedad numeroAbonado.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNumeroAbonado() {
        return numeroAbonado;
    }

    /**
     * Define el valor de la propiedad numeroAbonado.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNumeroAbonado(String value) {
        this.numeroAbonado = value;
    }

}
