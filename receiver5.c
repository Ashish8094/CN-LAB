#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in server, client;

    socklen_t len = sizeof(client);

    char msg[20];

    int h[13] = {0};
    int data[9] = {0};

    int i;
    int choice;
    int error;
    int errorPosition;

    int p1, p2, p4, p8;
    int value = 0;

    printf("===== HAMMING CODE RECEIVER =====\n\n");

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;

    /* Bind socket */
    if(bind(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    /* Listen */
    listen(server_fd, 5);

    printf("Waiting for client...\n");

    /* Accept connection */
    client_fd = accept(server_fd,
                       (struct sockaddr*)&client,
                       &len);

    if(client_fd < 0)
    {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }

    /* Receive Hamming code */
    memset(msg, 0, sizeof(msg));

    recv(client_fd, msg, sizeof(msg) - 1, 0);

    printf("\nReceived Hamming Code: %s\n", msg);

    /* Convert received string into integer bits */
    for(i = 1; i <= 12; i++)
    {
        h[i] = msg[i - 1] - '0';
    }

    printf("\n===== HAMMING CODE RECEIVER =====\n");
    printf("1. Receive Without Error\n");
    printf("2. Receive With Error\n");
    printf("3. Exit\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:

            printf("\n----- DATA RECEIVED WITHOUT ERROR -----\n");

            printf("Received Hamming Code: ");

            for(i = 1; i <= 12; i++)
            {
                printf("%d", h[i]);
            }

            printf("\n");

            break;


        case 2:

            printf("\n----- DATA RECEIVED WITH ERROR -----\n");

            /*
             * Automatically select an error position.
             * User does NOT enter the position.
             */
            srand(time(NULL));

            errorPosition = (rand() % 12) + 1;

            h[errorPosition] ^= 1;

            printf("An error has been automatically introduced.\n");
            printf("Error occurred at bit position: %d\n",
                   errorPosition);

            printf("Hamming Code With Error: ");

            for(i = 1; i <= 12; i++)
            {
                printf("%d", h[i]);
            }

            printf("\n");

            break;


        case 3:

            printf("\nExiting...\n");

            close(client_fd);
            close(server_fd);

            return 0;


        default:

            printf("\nInvalid choice.\n");

            close(client_fd);
            close(server_fd);

            return 0;
    }


    /*
     * Calculate Hamming syndrome
     */

    p1 = h[1] ^ h[3] ^ h[5] ^ h[7] ^ h[9] ^ h[11];

    p2 = h[2] ^ h[3] ^ h[6] ^ h[7] ^ h[10] ^ h[11];

    p4 = h[4] ^ h[5] ^ h[6] ^ h[7] ^ h[12];

    p8 = h[8] ^ h[9] ^ h[10] ^ h[11] ^ h[12];

    error = p1 * 1 +
            p2 * 2 +
            p4 * 4 +
            p8 * 8;


    /*
     * Check error
     */

    if(error == 0)
    {
        printf("\nNo Error Detected\n");
    }
    else
    {
        printf("\nError Detected at Bit Position: %d\n", error);

        /* Correct the error */
        h[error] ^= 1;

        printf("Error Corrected Successfully\n");
    }


    /*
     * Display corrected Hamming code
     */

    printf("Corrected Hamming Code: ");

    for(i = 1; i <= 12; i++)
    {
        printf("%d", h[i]);
    }

    printf("\n");


    /*
     * Extract original data bits
     */

    data[1] = h[3];
    data[2] = h[5];
    data[3] = h[6];
    data[4] = h[7];

    data[5] = h[9];
    data[6] = h[10];
    data[7] = h[11];
    data[8] = h[12];


    /*
     * Display recovered binary data
     */

    printf("Recovered Binary Data: ");

    for(i = 1; i <= 8; i++)
    {
        printf("%d", data[i]);
    }

    printf("\n");


    /*
     * Convert binary data to ASCII
     */

    value = 0;

    for(i = 1; i <= 8; i++)
    {
        value = value * 2 + data[i];
    }


    printf("Corrected Character: %c\n", value);
    printf("ASCII Value: %d\n", value);


    close(client_fd);
    close(server_fd);

    return 0;
}
