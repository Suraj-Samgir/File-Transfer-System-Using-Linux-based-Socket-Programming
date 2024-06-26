#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<ctype.h>		//Contains functions like isspace();

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error While Creating Socket!");
	}
	
	portno = 8080;
	server = gethostbyname("127.0.0.1");	
	
	if(server == NULL)
	{
		fprintf(stderr, "Error, No Such Host!");
		exit(1);
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error While Connecting to the Server!");	
	}
	
	
	//Actual code starts here...
	int choice = 0;
	
	printf("What do you want to do ?\n\n");
	printf("(1) Upload File to the Server\n");
	printf("(2) Download File from the Server\n");
	printf("(3) Delete File from the Server\n");
	printf("(4) View Files in Database\n");
	printf("(5) Exit the Program\n");
	printf("\nChoose Any Option from Above : ");
	scanf("%d",&choice);
	// send the choice to the server...
	n = write(sockfd,&choice,sizeof(int));
		
	if(n < 0)
	{
		error("Error While Writing the Choice!");
	}
	
	//Required variables...
	
	char filename[15];
	char filedata[3000];	//dont change this...
	
	if(choice == 1)
	{
		bzero(filename,sizeof(filename));
		bzero(filedata,sizeof(filedata));
		
 		int upload_choice = 0;
		printf("(1) Text File\n");
		printf("(2) Image File\n");
		printf("(3) Video File\n");
		printf("(4) Audio File\n");
		printf("(5) Exit\n");
		printf("\nChoose a File Type to Upload : ");
		scanf("%d",&upload_choice);
		//send the upload_choice to the server...
		n = write(sockfd,&upload_choice,sizeof(int));
		if(n < 0)
		{
			error("Error While Writing the Upload Choice!");
		}
		
		if(upload_choice == 1) //Text File
		{
			char c;
			int words=0;
			
			printf("\nEnter the File Name with it's Extension: ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			if(strstr(filename,".txt") == NULL)
			{
				printf("\n\nFile Doesn't Match the Text Extension!");
				exit(1);
			}
			FILE *file;
			file = fopen(filename,"r");
			
			if(file == NULL)
			{
				printf("\n\nError in Opening File");
				exit(1);
			}
			
	
			while((c = getc(file)) != EOF) 		//getc(f) = getting the character pointed by the file pointer "f".
			{
				fscanf(file,"%s", filedata);
		
		
				if(isspace(c) || c=='\t')	
				{
					words++;		//Incrementing to count the number of words.
				}
		
			}
	
			write(sockfd, &words, sizeof(int));
	
			rewind(file);  //To make the file pointer 'f' to again point to the start of the file.
	
			char ch;
			while(ch!=EOF)
			{
				fscanf(file,"%s", filedata);
				write(sockfd, filedata, 255);
				ch = fgetc(file);
			}
			
			fclose(file);
			
			//write(sockfd,filename,strlen(filename));
			printf("\nFile Send Successfully!");
			
			close(sockfd);
			
		}
		else if(upload_choice == 2)
		{
			printf("\nNOTE : Supported extensions for Image File - (JPG, JPEG, PNG, PDF, GIF)");
			printf("\nEnter the File Name with it's Extension: ");
			scanf("%s",filename);
			send(sockfd,filename,sizeof(filename),0);
			
			if(strstr(filename,".jpg") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".jpeg") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".png") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".pdf") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".gif") != NULL)
			{
				//extension matched...
			}
			else
			{
				printf("\n\nFile Doesn't Match the Supported Extensions!");
				exit(1);
			}
			
			FILE *file;
			ssize_t bytesRead;
	
			file = fopen(filename,"rb");
			if(file == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			
			//Read and send the file in chunks...
			while((bytesRead = fread(filedata,1,3000,file))>0)
			{
				send(sockfd,filedata,bytesRead,0);
			}	
	
			printf("File Uploaded successfully!");
	
			//closing the file and socket...
			fclose(file);
			close(sockfd);
		}
		else if(upload_choice == 3)
		{
			printf("\nNOTE : Supported extension for Video File - (MP4, AVI, MOV)");
			printf("\nEnter the File Name with it's Extension: ");
			scanf("%s",filename);
			send(sockfd,filename,sizeof(filename),0);
			
			if(strstr(filename,".mp4") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".avi") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".mov") != NULL)
			{
				//extension matched...
			}
			else
			{
				printf("\n\nFile Doesn't Match the Supported Extensions!");
				exit(1);
			}
			
			FILE *file;
			ssize_t bytesRead;
	
			file = fopen(filename,"rb");
			if(file == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			
			//Read and send the file in chunks...
			while((bytesRead = fread(filedata,1,3000,file))>0)
			{
				send(sockfd,filedata,bytesRead,0);
			}	
	
			printf("File Uploaded successfully!");
	
			//closing the file and socket...
			fclose(file);
			close(sockfd);
		}
		else if(upload_choice == 4)
		{
			printf("\nNOTE : Supported extensions for Audio File - (MP3, WAV, AAC)");
			printf("\nEnter the File Name with it's Extension: ");
			scanf("%s",filename);
			send(sockfd,filename,sizeof(filename),0);
			
			if(strstr(filename,".mp3") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".wav") != NULL)
			{
				//extension matched...
			}
			else if(strstr(filename,".aac") != NULL)
			{
				//extension matched...
			}
			else
			{
				printf("\n\nFile Doesn't Match the Supported Extensions!");
				exit(1);
			}
			
			FILE *file;
			ssize_t bytesRead;
	
			file = fopen(filename,"rb");
			if(file == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			
			//Read and send the file in chunks...
			while((bytesRead = fread(filedata,1,3000,file))>0)
			{
				send(sockfd,filedata,bytesRead,0);
			}	
	
			printf("File Uploaded successfully!");
	
			//closing the file and socket...
			fclose(file);
			close(sockfd);
		}
		else
		{
			exit(1);
		}
 			
	}
	else if(choice == 2)
	{	
		int download_choice = 0;
		printf("(1) Text File\n");
		printf("(2) Image File\n");
		printf("(3) Video File\n");
		printf("(4) Audio File\n");
		printf("(5) Exit\n");
		printf("\nChoose a File Type to Download : ");
		scanf("%d",&download_choice);
		//send the download_choice to the server...
		n = write(sockfd, &download_choice, sizeof(int));
			
		if(n<0)
		{
			error("Error While Writing the Download Choice!");
		}
		
		if(download_choice == 1)
		{
			printf("\nEnter the Name of File to Download : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			FILE *fp;
			int ch = 0;
			fp = fopen(filename, "w");		//Opening file in the write mode.
			int words;

			read(sockfd, &words, sizeof(int));

			while(ch!=words)
			{
				read(sockfd, filedata,255);
				fprintf(fp, "%s ", filedata);
				ch++;
			}
			fclose(fp);
	
			printf("\nFile Downloaded Successfully!\n");
			close(sockfd);	
		}
		else if(download_choice == 2)
		{
			printf("\nEnter the Name of File to Download : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(sockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
			//close the received file...
	
			fclose(recfile);
			printf("\nFile Downloaded Successfully!\n");
			close(sockfd);
		}
		else if(download_choice == 3)
		{
			printf("\nEnter the Name of File to Download : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(sockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
			//close the received file...
	
			fclose(recfile);
			printf("\nFile Downloaded Successfully!\n");
			close(sockfd);
		}
		else if(download_choice == 4)
		{
			printf("\nEnter the Name of File to Download : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(sockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
			//close the received file...
	
			fclose(recfile);
			printf("\nFile Downloaded Successfully!\n");
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 3)
	{
		int flag;
		int delete_choice = 0;
		printf("(1) Text File\n");
		printf("(2) Image File\n");
		printf("(3) Video File\n");
		printf("(4) Audio File\n");
		printf("(5) Exit\n");
		printf("\nChoose a File Type to Delete : ");
		scanf("%d",&delete_choice);
		//send the delete_choice to the server...
		n = write(sockfd,&delete_choice,sizeof(int));
		if(n<0)
		{
			error("Error While Writing the Delete Choice!");
		}
		
		if(delete_choice == 1)
		{
			printf("\nEnter the Name of File to Delete : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			read(sockfd,&flag,sizeof(int));
			if(flag)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nSorry Can't Delete the File!");
			}
			close(sockfd);
		}
		else if(delete_choice == 2)
		{
			printf("\nEnter the Name of File to Delete : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			read(sockfd,&flag,sizeof(int));
			if(flag)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nSorry Can't Delete the File!");
			}
			close(sockfd);
		}
		else if(delete_choice == 3)
		{
			printf("\nEnter the Name of File to Delete : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			read(sockfd,&flag,sizeof(int));
			if(flag)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nSorry Can't Delete the File!");
			}
			close(sockfd);
		}
		else if(delete_choice == 4)
		{
			printf("\nEnter the Name of File to Delete : ");
			scanf("%s",filename);
			write(sockfd,filename,sizeof(filename));
			
			read(sockfd,&flag,sizeof(int));
			if(flag)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nSorry Can't Delete the File!");
			}
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 4)
	{
		char buffer[1024];
		int view_choice = 0;
		printf("(1) Text File\n");
		printf("(2) Image File\n");
		printf("(3) Video File\n");
		printf("(4) Audio File\n");
		printf("(5) Exit\n");
		printf("\nChoose a File Type to View : ");
		scanf("%d",&view_choice);
		//send the view_choice to the server...
		n = write(sockfd,&view_choice,sizeof(int));
		if(n<0)
		{
			error("Error While Writing the Delete Choice!");
		}
		
		if(view_choice == 1)
		{
			int valread;
	
			while((valread = read(sockfd, buffer, 1023))>0)
			{
				buffer[valread] = '\0';
				printf("%s",buffer);
			}
			
			close(sockfd);	
		}
		else if(view_choice == 2)
		{
			int valread;
	
			while((valread = read(sockfd, buffer, 1023))>0)
			{
				buffer[valread] = '\0';
				printf("%s",buffer);
			}
			
			close(sockfd);
		}
		else if(view_choice == 3)
		{
			int valread;
	
			while((valread = read(sockfd, buffer, 1023))>0)
			{
				buffer[valread] = '\0';
				printf("%s",buffer);
			}
			
			close(sockfd);
		}
		else if(view_choice == 4)
		{
			int valread;
	
			while((valread = read(sockfd, buffer, 1023))>0)
			{
				buffer[valread] = '\0';
				printf("%s",buffer);
			}
			
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 5)
	{
		exit(1);
	}
	else
	{
		printf("Error! Not a Valid Choice");
		exit(1);
	}
	return 0;	
}	
		
