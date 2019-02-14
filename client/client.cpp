#include "csapp.cpp"
#include "stdio.h"

int main(int argc, char **argv)
{
	int clientfd, port;
	char *host, *image;
	rio_t rio;
    	FILE *fp;

	//error check
	if (argc != 4) {
		fprintf(stderr, "usage: %s <host> <port> <image>\n", argv[0]);
		exit(0);
	}

	//input arguments
	host = argv[1];
	port = atoi(argv[2]);
    	image = argv[3];

	//
	fp = fopen(image,"rb");//"rb" permission for non-text file(in our case it is an image)
	clientfd = Open_clientfd(host, port);
	//unsigned char array = (char *) malloc(sizeof (char) * 256);
	//array[256] = fp;
	Rio_readinitb(&rio, clientfd);
	fseek(fp, 0, SEEK_END);//setting the fp to end of file with an offset 0
	int size = ftell(fp);//gives the size of the image
    printf("size of the file is %u bytes\n", size);
	Rio_writen(clientfd, &size, sizeof(size));
	fseek(fp, 0, SEEK_SET);
	printf("Input image file reading has commenced \n");
	
	while(1)
	{
        /* First read file in chunks of 256 bytes */
        //printf("%d\n",size );
        unsigned char buff[256];

        //fseek(fp, 0, SEEK_SET);

        int nread = fread(buff,1,256,fp);
        printf(" No of bytes read from file :%d bytes\n", nread);

        /* If read was success, send data. */
        if(nread > 0)
        {
            Rio_writen(clientfd, buff, nread);
            printf("Sending %d bytes\n\n",nread);
            // write(clientfd, buff, nread);
        }

        /*
         * There is something tricky going on with read 
         * Either there was error, or we reached end of file.
         */
        if (nread < (256))
        {
            if (feof(fp))
                printf("End of file\n File sent successfully\n\n");
		fprintf(stdout, "------------Timestamp at which the color image file is sent to the server %lu seconds --------------\n\n",(unsigned long)time(NULL));
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }

    }
    fclose(fp);

printf("Waiting for the output image\n\n");

int bytesReceived = 0;
char recvBuff[256];
//char processed_image[20];
char processed_image[40];
sprintf(processed_image,"Output.jpg");

FILE *fp1;
fp1 = fopen(processed_image,"wb");

fseek(fp, 0, SEEK_SET);
int recData = 256;

/* Receive data in chunks of 256 bytes untill size data is received*/
    while(((bytesReceived = Rio_readnb(&rio,recvBuff,recData))>0) && size>0)
    {
        //printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp1);
        // printf("%s \n", recvBuff);
        size -= bytesReceived;
        printf("remaining: %d bytes\n\n",size);
        if (size<256)	//if remaining bytes is less than 256, read only remaining bytes of data
        	recData = size;
    }

    fclose(fp1);
    printf("----------File received successfully-----------\n");
    fprintf(stdout, "Timestamp at which the gray_scale output image file is recieved by the client %lu seconds\n\n",(unsigned long)time(NULL));

}
