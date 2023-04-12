
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para CfgUsuario complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="CfgUsuario"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="Nombre" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="IdIdentificador" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/&gt;
 *         &lt;element name="ListaAbonados" type="{http://CD40.es/}ArrayOfNumerosAbonado" minOccurs="0"/&gt;
 *         &lt;element name="NumeroEnlacesInternos" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="NumeroEnlacesExternos" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="Sector" type="{http://CD40.es/}SectoresSCV" minOccurs="0"/&gt;
 *         &lt;element name="ParametrosDelSector" type="{http://CD40.es/}ParametrosSectorSCVKeepAlive" minOccurs="0"/&gt;
 *         &lt;element name="PermisosRedDelSector" type="{http://CD40.es/}ArrayOfPermisosRedesSCV" minOccurs="0"/&gt;
 *         &lt;element name="TeclasDelSector" type="{http://CD40.es/}TeclasSectorSCV" minOccurs="0"/&gt;
 *         &lt;element name="NivelesDelSector" type="{http://CD40.es/}NivelesSCV" minOccurs="0"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "CfgUsuario", propOrder = {
    "nombre",
    "idIdentificador",
    "listaAbonados",
    "numeroEnlacesInternos",
    "numeroEnlacesExternos",
    "sector",
    "parametrosDelSector",
    "permisosRedDelSector",
    "teclasDelSector",
    "nivelesDelSector"
})
public class CfgUsuario {

    @XmlElement(name = "Nombre")
    protected String nombre;
    @XmlElement(name = "IdIdentificador")
    protected String idIdentificador;
    @XmlElement(name = "ListaAbonados")
    protected ArrayOfNumerosAbonado listaAbonados;
    @XmlElement(name = "NumeroEnlacesInternos")
    protected int numeroEnlacesInternos;
    @XmlElement(name = "NumeroEnlacesExternos")
    protected int numeroEnlacesExternos;
    @XmlElement(name = "Sector")
    protected SectoresSCV sector;
    @XmlElement(name = "ParametrosDelSector")
    protected ParametrosSectorSCVKeepAlive parametrosDelSector;
    @XmlElement(name = "PermisosRedDelSector")
    protected ArrayOfPermisosRedesSCV permisosRedDelSector;
    @XmlElement(name = "TeclasDelSector")
    protected TeclasSectorSCV teclasDelSector;
    @XmlElement(name = "NivelesDelSector")
    protected NivelesSCV nivelesDelSector;

    /**
     * Obtiene el valor de la propiedad nombre.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNombre() {
        return nombre;
    }

    /**
     * Define el valor de la propiedad nombre.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNombre(String value) {
        this.nombre = value;
    }

    /**
     * Obtiene el valor de la propiedad idIdentificador.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getIdIdentificador() {
        return idIdentificador;
    }

    /**
     * Define el valor de la propiedad idIdentificador.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setIdIdentificador(String value) {
        this.idIdentificador = value;
    }

    /**
     * Obtiene el valor de la propiedad listaAbonados.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfNumerosAbonado }
     *     
     */
    public ArrayOfNumerosAbonado getListaAbonados() {
        return listaAbonados;
    }

    /**
     * Define el valor de la propiedad listaAbonados.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfNumerosAbonado }
     *     
     */
    public void setListaAbonados(ArrayOfNumerosAbonado value) {
        this.listaAbonados = value;
    }

    /**
     * Obtiene el valor de la propiedad numeroEnlacesInternos.
     * 
     */
    public int getNumeroEnlacesInternos() {
        return numeroEnlacesInternos;
    }

    /**
     * Define el valor de la propiedad numeroEnlacesInternos.
     * 
     */
    public void setNumeroEnlacesInternos(int value) {
        this.numeroEnlacesInternos = value;
    }

    /**
     * Obtiene el valor de la propiedad numeroEnlacesExternos.
     * 
     */
    public int getNumeroEnlacesExternos() {
        return numeroEnlacesExternos;
    }

    /**
     * Define el valor de la propiedad numeroEnlacesExternos.
     * 
     */
    public void setNumeroEnlacesExternos(int value) {
        this.numeroEnlacesExternos = value;
    }

    /**
     * Obtiene el valor de la propiedad sector.
     * 
     * @return
     *     possible object is
     *     {@link SectoresSCV }
     *     
     */
    public SectoresSCV getSector() {
        return sector;
    }

    /**
     * Define el valor de la propiedad sector.
     * 
     * @param value
     *     allowed object is
     *     {@link SectoresSCV }
     *     
     */
    public void setSector(SectoresSCV value) {
        this.sector = value;
    }

    /**
     * Obtiene el valor de la propiedad parametrosDelSector.
     * 
     * @return
     *     possible object is
     *     {@link ParametrosSectorSCVKeepAlive }
     *     
     */
    public ParametrosSectorSCVKeepAlive getParametrosDelSector() {
        return parametrosDelSector;
    }

    /**
     * Define el valor de la propiedad parametrosDelSector.
     * 
     * @param value
     *     allowed object is
     *     {@link ParametrosSectorSCVKeepAlive }
     *     
     */
    public void setParametrosDelSector(ParametrosSectorSCVKeepAlive value) {
        this.parametrosDelSector = value;
    }

    /**
     * Obtiene el valor de la propiedad permisosRedDelSector.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfPermisosRedesSCV }
     *     
     */
    public ArrayOfPermisosRedesSCV getPermisosRedDelSector() {
        return permisosRedDelSector;
    }

    /**
     * Define el valor de la propiedad permisosRedDelSector.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfPermisosRedesSCV }
     *     
     */
    public void setPermisosRedDelSector(ArrayOfPermisosRedesSCV value) {
        this.permisosRedDelSector = value;
    }

    /**
     * Obtiene el valor de la propiedad teclasDelSector.
     * 
     * @return
     *     possible object is
     *     {@link TeclasSectorSCV }
     *     
     */
    public TeclasSectorSCV getTeclasDelSector() {
        return teclasDelSector;
    }

    /**
     * Define el valor de la propiedad teclasDelSector.
     * 
     * @param value
     *     allowed object is
     *     {@link TeclasSectorSCV }
     *     
     */
    public void setTeclasDelSector(TeclasSectorSCV value) {
        this.teclasDelSector = value;
    }

    /**
     * Obtiene el valor de la propiedad nivelesDelSector.
     * 
     * @return
     *     possible object is
     *     {@link NivelesSCV }
     *     
     */
    public NivelesSCV getNivelesDelSector() {
        return nivelesDelSector;
    }

    /**
     * Define el valor de la propiedad nivelesDelSector.
     * 
     * @param value
     *     allowed object is
     *     {@link NivelesSCV }
     *     
     */
    public void setNivelesDelSector(NivelesSCV value) {
        this.nivelesDelSector = value;
    }

}
