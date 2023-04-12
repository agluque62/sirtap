
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ConfiguracionSistema complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ConfiguracionSistema"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ParametrosGenerales" type="{http://CD40.es/}ParametrosGeneralesSistema" minOccurs="0"/&gt;
 *         &lt;element name="PlanNumeracionATS" type="{http://CD40.es/}ArrayOfNumeracionATS" minOccurs="0"/&gt;
 *         &lt;element name="PlanDireccionamientoIP" type="{http://CD40.es/}ArrayOfDireccionamientoIP" minOccurs="0"/&gt;
 *         &lt;element name="PlanTroncales" type="{http://CD40.es/}ArrayOfListaTroncales" minOccurs="0"/&gt;
 *         &lt;element name="PlanRedes" type="{http://CD40.es/}ArrayOfListaRedes" minOccurs="0"/&gt;
 *         &lt;element name="PlanAsignacionUsuarios" type="{http://CD40.es/}ArrayOfAsignacionUsuariosTV" minOccurs="0"/&gt;
 *         &lt;element name="PlanAsignacionRecursos" type="{http://CD40.es/}ArrayOfAsignacionRecursosGW" minOccurs="0"/&gt;
 *         &lt;element name="PlanDireccionamientoSIP" type="{http://CD40.es/}ArrayOfDireccionamientoSIP" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ConfiguracionSistema", propOrder = {
    "parametrosGenerales",
    "planNumeracionATS",
    "planDireccionamientoIP",
    "planTroncales",
    "planRedes",
    "planAsignacionUsuarios",
    "planAsignacionRecursos",
    "planDireccionamientoSIP"
})
public class ConfiguracionSistema {

    @XmlElement(name = "ParametrosGenerales")
    protected ParametrosGeneralesSistema parametrosGenerales;
    @XmlElement(name = "PlanNumeracionATS")
    protected ArrayOfNumeracionATS planNumeracionATS;
    @XmlElement(name = "PlanDireccionamientoIP")
    protected ArrayOfDireccionamientoIP planDireccionamientoIP;
    @XmlElement(name = "PlanTroncales")
    protected ArrayOfListaTroncales planTroncales;
    @XmlElement(name = "PlanRedes")
    protected ArrayOfListaRedes planRedes;
    @XmlElement(name = "PlanAsignacionUsuarios")
    protected ArrayOfAsignacionUsuariosTV planAsignacionUsuarios;
    @XmlElement(name = "PlanAsignacionRecursos")
    protected ArrayOfAsignacionRecursosGW planAsignacionRecursos;
    @XmlElement(name = "PlanDireccionamientoSIP")
    protected ArrayOfDireccionamientoSIP planDireccionamientoSIP;

    /**
     * Obtiene el valor de la propiedad parametrosGenerales.
     * 
     * @return
     *     possible object is
     *     {@link ParametrosGeneralesSistema }
     *     
     */
    public ParametrosGeneralesSistema getParametrosGenerales() {
        return parametrosGenerales;
    }

    /**
     * Define el valor de la propiedad parametrosGenerales.
     * 
     * @param value
     *     allowed object is
     *     {@link ParametrosGeneralesSistema }
     *     
     */
    public void setParametrosGenerales(ParametrosGeneralesSistema value) {
        this.parametrosGenerales = value;
    }

    /**
     * Obtiene el valor de la propiedad planNumeracionATS.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfNumeracionATS }
     *     
     */
    public ArrayOfNumeracionATS getPlanNumeracionATS() {
        return planNumeracionATS;
    }

    /**
     * Define el valor de la propiedad planNumeracionATS.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfNumeracionATS }
     *     
     */
    public void setPlanNumeracionATS(ArrayOfNumeracionATS value) {
        this.planNumeracionATS = value;
    }

    /**
     * Obtiene el valor de la propiedad planDireccionamientoIP.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfDireccionamientoIP }
     *     
     */
    public ArrayOfDireccionamientoIP getPlanDireccionamientoIP() {
        return planDireccionamientoIP;
    }

    /**
     * Define el valor de la propiedad planDireccionamientoIP.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfDireccionamientoIP }
     *     
     */
    public void setPlanDireccionamientoIP(ArrayOfDireccionamientoIP value) {
        this.planDireccionamientoIP = value;
    }

    /**
     * Obtiene el valor de la propiedad planTroncales.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfListaTroncales }
     *     
     */
    public ArrayOfListaTroncales getPlanTroncales() {
        return planTroncales;
    }

    /**
     * Define el valor de la propiedad planTroncales.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfListaTroncales }
     *     
     */
    public void setPlanTroncales(ArrayOfListaTroncales value) {
        this.planTroncales = value;
    }

    /**
     * Obtiene el valor de la propiedad planRedes.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfListaRedes }
     *     
     */
    public ArrayOfListaRedes getPlanRedes() {
        return planRedes;
    }

    /**
     * Define el valor de la propiedad planRedes.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfListaRedes }
     *     
     */
    public void setPlanRedes(ArrayOfListaRedes value) {
        this.planRedes = value;
    }

    /**
     * Obtiene el valor de la propiedad planAsignacionUsuarios.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfAsignacionUsuariosTV }
     *     
     */
    public ArrayOfAsignacionUsuariosTV getPlanAsignacionUsuarios() {
        return planAsignacionUsuarios;
    }

    /**
     * Define el valor de la propiedad planAsignacionUsuarios.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfAsignacionUsuariosTV }
     *     
     */
    public void setPlanAsignacionUsuarios(ArrayOfAsignacionUsuariosTV value) {
        this.planAsignacionUsuarios = value;
    }

    /**
     * Obtiene el valor de la propiedad planAsignacionRecursos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfAsignacionRecursosGW }
     *     
     */
    public ArrayOfAsignacionRecursosGW getPlanAsignacionRecursos() {
        return planAsignacionRecursos;
    }

    /**
     * Define el valor de la propiedad planAsignacionRecursos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfAsignacionRecursosGW }
     *     
     */
    public void setPlanAsignacionRecursos(ArrayOfAsignacionRecursosGW value) {
        this.planAsignacionRecursos = value;
    }

    /**
     * Obtiene el valor de la propiedad planDireccionamientoSIP.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfDireccionamientoSIP }
     *     
     */
    public ArrayOfDireccionamientoSIP getPlanDireccionamientoSIP() {
        return planDireccionamientoSIP;
    }

    /**
     * Define el valor de la propiedad planDireccionamientoSIP.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfDireccionamientoSIP }
     *     
     */
    public void setPlanDireccionamientoSIP(ArrayOfDireccionamientoSIP value) {
        this.planDireccionamientoSIP = value;
    }

}
