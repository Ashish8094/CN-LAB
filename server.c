#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


int main()
{

    int server_fd,client_fd;

    struct sockaddr_in server,client;

    socklen_t len=sizeof(client);

    char msg[20];

    int h[13];

    int data[9];

    int i;

    int choice,pos;



    server_fd=socket(AF_INET,SOCK_STREAM,0);


    server.sin_family=AF_INET;
    server.sin_port=htons(9000);
    server.sin_addr.s_addr=INADDR_ANY;


    bind(server_fd,(struct sockaddr*)&server,sizeof(server));


    listen(server_fd,5);


    printf("Waiting for client...\n");


    client_fd=accept(server_fd,(struct sockaddr*)&client,&len);



    recv(client_fd,msg,sizeof(msg),0);



    printf("\nReceived Correct Hamming Code: %s\n",msg);



    for(i=1;i<=12;i++)
        h[i]=msg[i-1]-'0';



    printf("\n===== HAMMING CODE RECEIVER =====\n");
    printf("1. Receive Without Error\n");
    printf("2. Receive With Error\n");
    printf("3. Exit\n");

    printf("Enter your choice: ");

    scanf("%d",&choice);



    switch(choice)
    {

        case 1:

            printf("\nChecking without introducing error...\n");

            break;



        case 2:

            printf("\nEnter error position (1-12): ");
            scanf("%d",&pos);


            if(pos>=1 && pos<=12)
            {
                h[pos]^=1;

                printf("Error introduced at position %d\n",pos);
            }

            break;



        case 3:

            printf("Exiting...\n");

            close(client_fd);
            close(server_fd);

            return 0;



        default:

            printf("Invalid choice\n");

            return 0;

    }



    int p1=h[1]^h[3]^h[5]^h[7]^h[9]^h[11];

    int p2=h[2]^h[3]^h[6]^h[7]^h[10]^h[11];

    int p4=h[4]^h[5]^h[6]^h[7]^h[12];

    int p8=h[8]^h[9]^h[10]^h[11]^h[12];


    int error=p1*1+p2*2+p4*4+p8*8;



    if(error==0)

        printf("\nNo Error Detected\n");


    else
    {

        printf("\nError detected at bit position %d\n",error);


        h[error]^=1;


        printf("Error Corrected\n");

    }



    data[1]=h[3];
    data[2]=h[5];
    data[3]=h[6];
    data[4]=h[7];

    data[5]=h[9];
    data[6]=h[10];
    data[7]=h[11];
    data[8]=h[12];


    int value=0;


    for(i=1;i<=8;i++)
        value=value*2+data[i];


    printf("Corrected Character: %c\n",value);

    printf("ASCII Value: %d\n",value);



    close(client_fd);
    close(server_fd);


    return 0;
}
