/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tema2;

/**
 *
 * @author vsanz
 */
public class Ej03QueImprimeB {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Persona p1; 
        Persona p2;
        Persona p3;
        
        p1 = new Persona();
        p1.setNombre("Pablo Sotile");
        p1.setDNI(11200413);
        p1.setEdad(40);
        
        p2 = new Persona();
        p2.setNombre("Julio Toledo");
        p2.setDNI(22433516);
        p2.setEdad(51);
        
        p3 = new Persona();
        p3.setNombre("Julio Toledo");
        p3.setDNI(22433516);
        p3.setEdad(51);
        
        p1 = p2;
        
        System.out.println(p1.toString());
        System.out.println(p2.toString());
        System.out.println(p3.toString());
        System.out.println( (p1 == p2) );
        System.out.println((p3 == p2));
        System.out.println(p3.toString());
        System.out.println(p3.toString().equals(p2.toString()));
    }
    
}
