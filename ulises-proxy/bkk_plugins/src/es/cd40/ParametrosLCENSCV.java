
package es.cd40;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlSeeAlso;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Clase Java para ParametrosLCENSCV complex type.
 * 
 * <p>El siguiente fragmento de esquema especifica el contenido que se espera que haya en esta clase.
 * 
 * <pre>
 * &lt;complexType name="ParametrosLCENSCV"&gt;
 *   &lt;complexContent&gt;
 *     &lt;extension base="{http://CD40.es/}ParametrosRecursoGeneral"&gt;
 *       &lt;sequence&gt;
 *         &lt;element name="ListaEnlacesInternos" type="{http://CD40.es/}ArrayOfString" minOccurs="0"/&gt;
 *         &lt;element name="T1" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T1Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T2" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T2Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T3" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T4" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T4Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T5" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T5Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T6" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T6Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T8" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T8Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T9" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T9Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T10" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T10Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T11" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T11Max" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="T12" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="FrqTonoSQ" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoSQ" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="FrqTonoPTT" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="UmbralTonoPTT" type="{http://www.w3.org/2001/XMLSchema}int"/&gt;
 *         &lt;element name="RefrescoEstados" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="Timeout" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *         &lt;element name="LongRafagas" type="{http://www.w3.org/2001/XMLSchema}unsignedInt"/&gt;
 *       &lt;/sequence&gt;
 *     &lt;/extension&gt;
 *   &lt;/complexContent&gt;
 * &lt;/complexType&gt;
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ParametrosLCENSCV", propOrder = {
    "listaEnlacesInternos",
    "t1",
    "t1Max",
    "t2",
    "t2Max",
    "t3",
    "t4",
    "t4Max",
    "t5",
    "t5Max",
    "t6",
    "t6Max",
    "t8",
    "t8Max",
    "t9",
    "t9Max",
    "t10",
    "t10Max",
    "t11",
    "t11Max",
    "t12",
    "frqTonoSQ",
    "umbralTonoSQ",
    "frqTonoPTT",
    "umbralTonoPTT",
    "refrescoEstados",
    "timeout",
    "longRafagas"
})
@XmlSeeAlso({
    ParametrosRecursoLCEN.class
})
public class ParametrosLCENSCV
    extends ParametrosRecursoGeneral
{

    @XmlElement(name = "ListaEnlacesInternos")
    protected ArrayOfString listaEnlacesInternos;
    @XmlElement(name = "T1")
    @XmlSchemaType(name = "unsignedInt")
    protected long t1;
    @XmlElement(name = "T1Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t1Max;
    @XmlElement(name = "T2")
    @XmlSchemaType(name = "unsignedInt")
    protected long t2;
    @XmlElement(name = "T2Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t2Max;
    @XmlElement(name = "T3")
    @XmlSchemaType(name = "unsignedInt")
    protected long t3;
    @XmlElement(name = "T4")
    @XmlSchemaType(name = "unsignedInt")
    protected long t4;
    @XmlElement(name = "T4Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t4Max;
    @XmlElement(name = "T5")
    @XmlSchemaType(name = "unsignedInt")
    protected long t5;
    @XmlElement(name = "T5Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t5Max;
    @XmlElement(name = "T6")
    @XmlSchemaType(name = "unsignedInt")
    protected long t6;
    @XmlElement(name = "T6Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t6Max;
    @XmlElement(name = "T8")
    @XmlSchemaType(name = "unsignedInt")
    protected long t8;
    @XmlElement(name = "T8Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t8Max;
    @XmlElement(name = "T9")
    @XmlSchemaType(name = "unsignedInt")
    protected long t9;
    @XmlElement(name = "T9Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t9Max;
    @XmlElement(name = "T10")
    @XmlSchemaType(name = "unsignedInt")
    protected long t10;
    @XmlElement(name = "T10Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t10Max;
    @XmlElement(name = "T11")
    @XmlSchemaType(name = "unsignedInt")
    protected long t11;
    @XmlElement(name = "T11Max")
    @XmlSchemaType(name = "unsignedInt")
    protected long t11Max;
    @XmlElement(name = "T12")
    @XmlSchemaType(name = "unsignedInt")
    protected long t12;
    @XmlElement(name = "FrqTonoSQ")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoSQ;
    @XmlElement(name = "UmbralTonoSQ")
    protected int umbralTonoSQ;
    @XmlElement(name = "FrqTonoPTT")
    @XmlSchemaType(name = "unsignedInt")
    protected long frqTonoPTT;
    @XmlElement(name = "UmbralTonoPTT")
    protected int umbralTonoPTT;
    @XmlElement(name = "RefrescoEstados")
    @XmlSchemaType(name = "unsignedInt")
    protected long refrescoEstados;
    @XmlElement(name = "Timeout")
    @XmlSchemaType(name = "unsignedInt")
    protected long timeout;
    @XmlElement(name = "LongRafagas")
    @XmlSchemaType(name = "unsignedInt")
    protected long longRafagas;

    /**
     * Obtiene el valor de la propiedad listaEnlacesInternos.
     * 
     * @return
     *     possible object is
     *     {@link ArrayOfString }
     *     
     */
    public ArrayOfString getListaEnlacesInternos() {
        return listaEnlacesInternos;
    }

    /**
     * Define el valor de la propiedad listaEnlacesInternos.
     * 
     * @param value
     *     allowed object is
     *     {@link ArrayOfString }
     *     
     */
    public void setListaEnlacesInternos(ArrayOfString value) {
        this.listaEnlacesInternos = value;
    }

    /**
     * Obtiene el valor de la propiedad t1.
     * 
     */
    public long getT1() {
        return t1;
    }

    /**
     * Define el valor de la propiedad t1.
     * 
     */
    public void setT1(long value) {
        this.t1 = value;
    }

    /**
     * Obtiene el valor de la propiedad t1Max.
     * 
     */
    public long getT1Max() {
        return t1Max;
    }

    /**
     * Define el valor de la propiedad t1Max.
     * 
     */
    public void setT1Max(long value) {
        this.t1Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t2.
     * 
     */
    public long getT2() {
        return t2;
    }

    /**
     * Define el valor de la propiedad t2.
     * 
     */
    public void setT2(long value) {
        this.t2 = value;
    }

    /**
     * Obtiene el valor de la propiedad t2Max.
     * 
     */
    public long getT2Max() {
        return t2Max;
    }

    /**
     * Define el valor de la propiedad t2Max.
     * 
     */
    public void setT2Max(long value) {
        this.t2Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t3.
     * 
     */
    public long getT3() {
        return t3;
    }

    /**
     * Define el valor de la propiedad t3.
     * 
     */
    public void setT3(long value) {
        this.t3 = value;
    }

    /**
     * Obtiene el valor de la propiedad t4.
     * 
     */
    public long getT4() {
        return t4;
    }

    /**
     * Define el valor de la propiedad t4.
     * 
     */
    public void setT4(long value) {
        this.t4 = value;
    }

    /**
     * Obtiene el valor de la propiedad t4Max.
     * 
     */
    public long getT4Max() {
        return t4Max;
    }

    /**
     * Define el valor de la propiedad t4Max.
     * 
     */
    public void setT4Max(long value) {
        this.t4Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t5.
     * 
     */
    public long getT5() {
        return t5;
    }

    /**
     * Define el valor de la propiedad t5.
     * 
     */
    public void setT5(long value) {
        this.t5 = value;
    }

    /**
     * Obtiene el valor de la propiedad t5Max.
     * 
     */
    public long getT5Max() {
        return t5Max;
    }

    /**
     * Define el valor de la propiedad t5Max.
     * 
     */
    public void setT5Max(long value) {
        this.t5Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t6.
     * 
     */
    public long getT6() {
        return t6;
    }

    /**
     * Define el valor de la propiedad t6.
     * 
     */
    public void setT6(long value) {
        this.t6 = value;
    }

    /**
     * Obtiene el valor de la propiedad t6Max.
     * 
     */
    public long getT6Max() {
        return t6Max;
    }

    /**
     * Define el valor de la propiedad t6Max.
     * 
     */
    public void setT6Max(long value) {
        this.t6Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t8.
     * 
     */
    public long getT8() {
        return t8;
    }

    /**
     * Define el valor de la propiedad t8.
     * 
     */
    public void setT8(long value) {
        this.t8 = value;
    }

    /**
     * Obtiene el valor de la propiedad t8Max.
     * 
     */
    public long getT8Max() {
        return t8Max;
    }

    /**
     * Define el valor de la propiedad t8Max.
     * 
     */
    public void setT8Max(long value) {
        this.t8Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t9.
     * 
     */
    public long getT9() {
        return t9;
    }

    /**
     * Define el valor de la propiedad t9.
     * 
     */
    public void setT9(long value) {
        this.t9 = value;
    }

    /**
     * Obtiene el valor de la propiedad t9Max.
     * 
     */
    public long getT9Max() {
        return t9Max;
    }

    /**
     * Define el valor de la propiedad t9Max.
     * 
     */
    public void setT9Max(long value) {
        this.t9Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t10.
     * 
     */
    public long getT10() {
        return t10;
    }

    /**
     * Define el valor de la propiedad t10.
     * 
     */
    public void setT10(long value) {
        this.t10 = value;
    }

    /**
     * Obtiene el valor de la propiedad t10Max.
     * 
     */
    public long getT10Max() {
        return t10Max;
    }

    /**
     * Define el valor de la propiedad t10Max.
     * 
     */
    public void setT10Max(long value) {
        this.t10Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t11.
     * 
     */
    public long getT11() {
        return t11;
    }

    /**
     * Define el valor de la propiedad t11.
     * 
     */
    public void setT11(long value) {
        this.t11 = value;
    }

    /**
     * Obtiene el valor de la propiedad t11Max.
     * 
     */
    public long getT11Max() {
        return t11Max;
    }

    /**
     * Define el valor de la propiedad t11Max.
     * 
     */
    public void setT11Max(long value) {
        this.t11Max = value;
    }

    /**
     * Obtiene el valor de la propiedad t12.
     * 
     */
    public long getT12() {
        return t12;
    }

    /**
     * Define el valor de la propiedad t12.
     * 
     */
    public void setT12(long value) {
        this.t12 = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoSQ.
     * 
     */
    public long getFrqTonoSQ() {
        return frqTonoSQ;
    }

    /**
     * Define el valor de la propiedad frqTonoSQ.
     * 
     */
    public void setFrqTonoSQ(long value) {
        this.frqTonoSQ = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoSQ.
     * 
     */
    public int getUmbralTonoSQ() {
        return umbralTonoSQ;
    }

    /**
     * Define el valor de la propiedad umbralTonoSQ.
     * 
     */
    public void setUmbralTonoSQ(int value) {
        this.umbralTonoSQ = value;
    }

    /**
     * Obtiene el valor de la propiedad frqTonoPTT.
     * 
     */
    public long getFrqTonoPTT() {
        return frqTonoPTT;
    }

    /**
     * Define el valor de la propiedad frqTonoPTT.
     * 
     */
    public void setFrqTonoPTT(long value) {
        this.frqTonoPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad umbralTonoPTT.
     * 
     */
    public int getUmbralTonoPTT() {
        return umbralTonoPTT;
    }

    /**
     * Define el valor de la propiedad umbralTonoPTT.
     * 
     */
    public void setUmbralTonoPTT(int value) {
        this.umbralTonoPTT = value;
    }

    /**
     * Obtiene el valor de la propiedad refrescoEstados.
     * 
     */
    public long getRefrescoEstados() {
        return refrescoEstados;
    }

    /**
     * Define el valor de la propiedad refrescoEstados.
     * 
     */
    public void setRefrescoEstados(long value) {
        this.refrescoEstados = value;
    }

    /**
     * Obtiene el valor de la propiedad timeout.
     * 
     */
    public long getTimeout() {
        return timeout;
    }

    /**
     * Define el valor de la propiedad timeout.
     * 
     */
    public void setTimeout(long value) {
        this.timeout = value;
    }

    /**
     * Obtiene el valor de la propiedad longRafagas.
     * 
     */
    public long getLongRafagas() {
        return longRafagas;
    }

    /**
     * Define el valor de la propiedad longRafagas.
     * 
     */
    public void setLongRafagas(long value) {
        this.longRafagas = value;
    }

}
