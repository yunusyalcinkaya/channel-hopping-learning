import java.security.SecureRandom;

public class NumberGenerator{

    public static void main(String[] args) {
        SecureRandom random = new SecureRandom();

        double avg =0;

        for(int i =0; i < 1000; i++){
            double number = random.nextDouble(-90, -80);
            avg = (avg * i + number) / (i + 1);
            System.out.println(number);
        }
        System.out.println("average: "+avg);
    }
}