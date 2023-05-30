
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class CountAction{

    public static void main(String[] args) throws FileNotFoundException {

        int[] channels = new int[16]; 
        File file = new File("learning/outputs/single-write-action.txt");
        Scanner scanner = new Scanner(file);

        while(scanner.hasNext()){
            channels[scanner.nextInt()]++;
        }
        scanner.close();

        for(int i = 0; i < channels.length; i++){
            System.out.print(channels[i] + ", ");
        }

    }
}