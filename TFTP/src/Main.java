import java.io.IOException;
import java.net.InetAddress;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) throws IOException {

        System.out.println("________________________________________________________\n" +
                "--------------------------------------------------------\n" +
                "     ___________   _______   ___________   ______\n" +
                "    |____   ____| |  _____| |____   ____| |  __  \\\n" +
                "         | |      | |__          | |      | |__/  |\n" +
                "         | |      |  __/         | |      |  ____/\n" +
                "         | |      | |            | |      | |\n" +
                "         |_|      |_|            |_|      |_|\n" +
                "________________________________________________________\n" +
                "--------------------------------------------------------\n" +
                "\n" +
                "\n" +
                "+======================================================+\n" +
                "| Voulez-vous récupérer un fichier, ou en envoyer un ? |\n" +
                "+======================================================+\n" +
                "+------------------------------------------------------+\n" +
                "|    1 : Recevoir un fichier (ReceiveFile)             |\n" +
                "+------------------------------------------------------+\n" +
                "|    2 : Envoyer un fichier (SendFile)                 |\n" +
                "+------------------------------------------------------+\n" +
                "\n" +
                "\n");

        Scanner sc = new Scanner(System.in);
        int transfert = sc.nextInt();

        while(transfert!=1 && transfert!=2){
            System.out.println("Veuillez entrer 1 ou 2, et non pas un autre nombre !");
            transfert = sc.nextInt();
        }

        if(transfert==1){
            System.out.println("Vous avez choisi de recevoir un fichier !");
        }
        if(transfert==2){
            System.out.println("Vous avez choisi d'envoyer un fichier !");
        }

        Scanner sc2 = new Scanner(System.in);
        System.out.println("Veuillez entrer le nom du fichier avec son extension");
        String fichier = sc2.nextLine();

        System.out.println("Le fichier " + fichier + " va être échangé !");

        InetAddress adr = InetAddress.getByName("127.0.0.1");

        Client client = new Client(adr,69, fichier);

        if(transfert==1) {
            int CrRv = client.ReceiveFile(adr,69);

            switch (CrRv){
                case 1 :
                    System.out.println("Erreur locale : le fichier existe déjà !");
                    break;
                case 2 :
                    System.out.println("Erreur lors de l'échange : connexion au serveur impossible");
                    break;
                case 3 :
                    System.out.println("Erreur lors de l'échange : problème côté serveur");
                    break;
                case 0 :
                    System.out.println("Le transfert s'est bien déroulé !");
                    break;
                case -1 :
                    System.out.println("Le fichier demandé n'existe pas");
                    break;
                case -2 :
                    System.out.println("Violation de l'accès");
                    break;
                case -3 :
                    System.out.println("Disque plein");
                    break;
                case -4 :
                    System.out.println("Opération TFTP illégale");
                    break;
                case -5 :
                    System.out.println("TID inconnu");
                    break;
                case -6 :
                    System.out.println("Le fichier existe déjà");
                    break;
                case -7 :
                    System.out.println("Utilisateur inconnu");
                    break;
                default :
                    System.out.println("Erreur");
            }

            System.out.println("CrRv = " + CrRv);
        } else {

            System.out.println("L'envoi de fichier n'est pas encore disponible ! C'est pour bientôt ;)");
            //int CrEm =  client.ScendFile(adr,69);
            /*
            switch (CrEm){
                case 0 :
                    System.out.println("Le transfert s'est bien déroulé !");
                    break;
            }
            */

        }

    }
}
