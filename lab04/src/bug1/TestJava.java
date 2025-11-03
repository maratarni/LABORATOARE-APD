public class TestJava {
    public static void main(String[] args) {
        System.out.println("Hello! Extensiile Java funcționează!");
        
        // Test variabile și tipuri
        int numar = 42;
        String text = "VS Code";
        
        // Test metode
        afiseazaInfo(numar, text);
        
        // Test structuri de control
        for (int i = 1; i <= 3; i++) {
            System.out.println("Iterația " + i);
        }
    }
    
    // Metodă separată pentru a testa IntelliSense
    public static void afiseazaInfo(int n, String s) {
        System.out.println("Număr: " + n);
        System.out.println("Text: " + s);
    }
}