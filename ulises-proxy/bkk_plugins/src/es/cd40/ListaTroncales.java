
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ListaTroncales complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ListaTroncales"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="IdTroncal" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="NumeroTest" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ListaRecursos" type="{http://CD40.es/}ArrayOfPlanRecursos" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ListaTroncales", propOrder = {
    "idTroncal",
    "numeroTest",
    "listaRecursos"
})
public class ListaTroncales {

    @XmlElement(name = "IdTroncal")
    protected String idTroncal;
    @XmlElement(name = "NumeroTest")
    protected String numeroTest;
    @XmlElement(name = "ListaRecursos")
    protected ArrayOfPlanRecursos listaRecursos;

    /**
     * Obtiene el valor de la propiedad idTroncal.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdTroncal() {
        return idTroncal;
    }

    /**
     * Define el valor de la propiedad idTroncal.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdTroncal(String value) {
        this.idTroncal = value;
    }

    /**
     * Obtiene el valor de la propiedad numeroTest.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNumeroTest() {
        return numeroTest;
    }

    /**
     * Define el valor de la propiedad numeroTest.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNumeroTest(String value) {
        this.numeroTest = value;
    }

    /**
     * Obtiene el valor de la propiedad listaRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfPlanRecursos }
     *     
     */
    public ArrayOfPlanRecursos getListaRecursos() {
        return listaRecursos;
    }

    /**
     * Define el valor de la propiedad listaRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfPlanRecursos }
     *     
     */
    public void setListaRecursos(ArrayOfPlanRecursos value) {
        this.listaRecursos = value;
    }

}
