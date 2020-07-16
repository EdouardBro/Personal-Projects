import java.io.*;
//import java.math.BigInteger;
import java.net.*;



public class Client {

    int port;
    int portServeur;
    InetAddress adr;
    String fichier;
    String mode = "octet";
    String chemin = "/"; //Définir le chemin vers le fichier à transférer

    // Requete RRQ et WRQ
    int RRQ = 1; // 1 correspond à RRQ
    int WRQ = 2; // 2 correspond à WRQ

    //Codes erreur
    int CrRv = 0; // Code erreur RRQ
    int CrEm = 0; // Code erreur WRQ



    Client(InetAddress adr, int port, String fichier){
        this.port = port;
        this.adr = adr;
        this.fichier = fichier;
    }


    private void sendRequest(int type, String mode, String fichier, InetAddress serveur, DatagramSocket socket, int port)
            throws IOException {
        // Cette classe va envoyer un message RRQ ou WRQ selon la demande du Client
        // Type correspond donc à RRQ ou WRQ, c'est un entier qui vaut 01 ou 02

        int taille = 2 + fichier.length() + 1 + mode.length() + 1;
        byte[] envoiRequete = new byte[taille];

        // On convertit notre requete en un tableau de bytes
        envoiRequete[0]=(byte) 0;
        envoiRequete[1]=(byte) type;
        int k=2;
        for(int i = 0; i<fichier.length();i++){
            envoiRequete[k]=(byte)fichier.charAt(i);
            k++;
        }
        envoiRequete[k] = (byte) 0;
        k++;

        for(int j=0; j<mode.length(); j++){
            envoiRequete[k]=(byte)mode.charAt(j);
            k++;
        }
        envoiRequete[k]=(byte)0;

        DatagramPacket envoi = new DatagramPacket(envoiRequete, envoiRequete.length, serveur, port);
        socket.send(envoi);

    }

    private void sendACK(byte[] numBloc, DatagramSocket socket, InetAddress serveur, int port) throws IOException {

        byte[] envoiACK = new byte[4];
        envoiACK[0]=(byte) 0;
        envoiACK[1]=(byte) 4;

        envoiACK[2]= numBloc[0];
        envoiACK[3]= numBloc[1];

        DatagramPacket ACK = new DatagramPacket(envoiACK,envoiACK.length,serveur,port);
        socket.send(ACK);

        //System.out.println("Ack envoyé : " + ACK);
        //System.out.println("Numéro de bloc" + numBloc[0] + numBloc[1]);

    }




    private int receivePacket(byte[] zoneReception, DatagramSocket socket, FileOutputStream fos) throws IOException {

        DatagramPacket receptionPacket = new DatagramPacket(zoneReception,zoneReception.length);
        socket.setSoTimeout(10000);
        try {
            socket.receive(receptionPacket);
        }catch (SocketTimeoutException ste){
            System.err.println("Temps expiré : 10s " + ste);
            CrRv= 2;
            return -1; // Si pas de réponse du serveur, on arrête l'exécution.
        }catch (SocketException e){
            System.err.println(e);
            CrRv=3;
        }


        // On va récupérer les 512 bytes de DATA (on enlève l'en-tête)
        byte[] tampon = new byte[512];
        for(int i=0; i<512; i++){
            tampon[i]=zoneReception[i+4];
        }

        fos.write(tampon);
        fos.flush();

        portServeur = receptionPacket.getPort();

        return receptionPacket.getLength();

    }



    int gestionErreur(int reception){
        switch((byte)reception){
            case 1 :
                return -1;
            case 2 :
                return -2;
            case 3 :
                return -3;
            case 4 :
                return -4;
            case 5 :
                return -5;
            case 6 :
                return -6;
            case 7 :
                return -7;
            default :
                return -8;
        }
    }



    public int ReceiveFile(InetAddress serveur,int port) throws IOException {

        // On définit une zone de reception pour les DATA reçus
        byte[] reception;

        // On crée le fichier de réception
        FileOutputStream fos=null;

        File file;
        file = new File(chemin + fichier);
        if(file.exists()){
            return 1;
        }

        try {
            fos = new FileOutputStream(chemin + fichier, true);
        }catch (SecurityException e){
            System.err.println(e);
            CrRv=8;
        }


        DatagramSocket sc = new DatagramSocket(); // Création d'un DTGSocket pour initier la connexion au serveur

        sendRequest(RRQ,mode,fichier,serveur,sc, port); // Envoi Requete RRQ


        //On va échanger avec le serveur tant qu'on recoit des paquets de 512octets
        int tailleRecu;
        byte num[];
        do {
            //Reception réponse requete RRQ (c'est un DATA)
            reception = new byte[516];
            tailleRecu = receivePacket(reception, sc, fos);

            //Gestion des paquets d'erreurs
            if(reception[0]==(byte)0 && reception[1]==(byte)5){
                CrRv = gestionErreur(reception[3]);
            }

            // Recupération du numéro de bloc du dernier paquet reçu
            num = new byte[2];
            num[0] = reception[2];
            num[1] = reception[3];
            

            //Envoi ACK
            //ACK = "04" + numBloc
            // Renvoie un ACK que si le dernier paquet reçu est un DATA
            if(reception[0]==(byte)0 && reception[1]==(byte)3)
            sendACK(num, sc, serveur, portServeur);


        }while(tailleRecu>=516);

        /* Attardement : attend pour être sûr que le dernier ACK n'est pas perdu */
        int taillePacket=0;
        do {
            sc.setSoTimeout(5000);
            reception=new byte[516];
            try {
                DatagramPacket receptionPacket = new DatagramPacket(reception, 516);
                sc.receive(receptionPacket);
                taillePacket = receptionPacket.getLength();
                num = new byte[2];
                num[0] = reception[2];
                num[1] = reception[3];
                sendACK(num, sc, serveur, portServeur);
            } catch (SocketTimeoutException ste) {
                System.out.println("Échange terminé");
            }
        }while (taillePacket!=0);


        fos.close();
        sc.close();
        return CrRv;
    }


}
