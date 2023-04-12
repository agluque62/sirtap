
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para Tablas complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="Tablas"&gt;
 *   &lt;complexContent&gt;
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="InsertedId" type="{http://www.w3.org/2001/XMLSchema}long"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/restriction&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Tablas", propOrder = {
    "insertedId"
})
@XmlSeeAlso({
    SectoresAgrupacion.class,
    Agrupaciones.class,
    Internos.class,
    HFRangoFrecuencias.class,
    HFParams.class,
    ParametrosSectorSCV.class,
    TeclasSectorSCV.class,
    TroncalesRuta.class,
    SectoresSector.class,
    Altavoces.class,
    NivelesSCV.class,
    Radio.class,
    EstadosRecursos.class,
    DestinosRadioSector.class,
    DestinosInternosSector.class,
    GruposTelefonia.class,
    DestinosExternosSector.class,
    Externos.class,
    Destinos.class,
    UsuariosAbonados.class,
    RecursosSCV.class,
    ParametrosRecursoGeneral.class,
    Rutas.class,
    RangosSCV.class,
    Encaminamientos.class,
    SectoresSectorizacion.class,
    Troncales.class,
    PermisosRedesSCV.class,
    Redes.class,
    Sectorizaciones.class,
    Prefijos.class,
    Nucleos.class,
    Emplazamientos.class,
    Operadores.class,
    Sistema.class,
    Top.class,
    TifX.class,
    SectoresSCV.class
})
public abstract class Tablas {

    @XmlElement(name = "InsertedId")
    protected long insertedId;

    /**
     * Obtiene el valor de la propiedad insertedId.
     * 
     */
    public long getInsertedId() {
        return insertedId;
    }

    /**
     * Define el valor de la propiedad insertedId.
     * 
     */
    public void setInsertedId(long value) {
        this.insertedId = value;
    }

}
