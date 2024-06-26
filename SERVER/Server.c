#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sqlite3.h>
#include<ctype.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	
	int sockfd, newsockfd, portno, n;
	
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error While Creating Socket!");
	}
	
	portno = 8080;
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error While Binding!");
	}
	
	listen(sockfd, 4);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
	{
		error("Error While Accepting the Connection!");
	}
	
	//Actual code starts here...
	
	//Required Variables...
	char filename[15];
	char filedata[3000];
	int filesize;
	
	int choice;
	
	n = read(newsockfd,&choice,sizeof(int));
	
	if(n < 0)
	{
		error("Error While Reading the Choice!");
	}
	
	if(choice == 1)
	{
		bzero(filename,sizeof(filename));
		bzero(filedata,sizeof(filedata));
		
		int upload_choice;
		//receive the upload_choice from the client;	
		n = read(newsockfd,&upload_choice,sizeof(int));
		if(n < 0)
		{
			error("Error While Reading the Upload Choice");
		}
		
		if(upload_choice == 1)
		{
			read(newsockfd,filename,sizeof(filename));
			FILE *fp;
			int ch = 0;
			fp = fopen(filename, "w");		//Opening file in the write mode.
			int words;

			read(newsockfd, &words, sizeof(int));

			while(ch!=words)
			{
				read(newsockfd, filedata,255);
				fprintf(fp, "%s ", filedata);
				ch++;
			}
			fclose(fp);
			
			//Adding to the database...
			sqlite3 *db;		//connnection object
			//char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			const char *insert_sql = "INSERT INTO TextTable (FileName) VALUES (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			printf("\nFile Uploaded Successfully!");
			close(newsockfd);
			close(sockfd);
		}
		else if(upload_choice == 2)
		{	
			recv(newsockfd,filename,sizeof(filename),0);
			printf("\nName = %s",filename);
			
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(newsockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
	
			fclose(recfile);
			
			//Adding to the database...
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			const char *insert_sql = "INSERT INTO ImageTable (FileName) VALUES (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			printf("\nFile Uploaded Successfully!");
			close(newsockfd);
			close(sockfd);
		}
		else if(upload_choice == 3)
		{
			recv(newsockfd,filename,sizeof(filename),0);
			printf("\nName = %s",filename);
			
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(newsockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
			//close the received file...
			fclose(recfile);
			
			//Adding to the database...
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			const char *insert_sql = "INSERT INTO VideoTable (FileName) VALUES (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			printf("\nFile Uploaded Successfully!");
			close(newsockfd);
			close(sockfd);
		}
		else if(upload_choice == 4)
		{
			recv(newsockfd,filename,sizeof(filename),0);
			printf("\nName = %s",filename);
			
			FILE *recfile;
	
			recfile = fopen(filename,"wb");
			if(recfile == NULL)
			{
				perror("Error opening file");
				exit(1);
			}
			ssize_t bytesRead;
			while((bytesRead = recv(newsockfd,filedata,3000,0)) > 0)
			{
				fwrite(filedata,1,bytesRead,recfile);
			}
	
			//close the received file...
	
			fclose(recfile);
			
			//Adding to the database...
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			const char *insert_sql = "INSERT INTO AudioTable (FileName) VALUES (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return 1;
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			printf("\nFile Uploaded Successfully!");
			close(newsockfd);
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 2)
	{
		bzero(filename,sizeof(filename));
		bzero(filedata,sizeof(filedata));
		
		int download_choice;
		//receive the download_choice from the client;
		n = read(newsockfd,&download_choice,sizeof(int));
		if(n<0)
		{
			error("Error While Reading the Download Choice!");
		}

		if(download_choice == 1)
		{	
			char c;
			int words=0;
			read(newsockfd,filename,sizeof(filename));
			
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
	
			write(newsockfd, &words, sizeof(int));
	
			rewind(file);  //To make the file pointer 'f' to again point to the start of the file.
	
			char ch;
			while(ch!=EOF)
			{
				fscanf(file,"%s", filedata);
				write(newsockfd, filedata, 255);
				ch = fgetc(file);
			}
			
			fclose(file);
			printf("\nFile Send Successfully!");
			
			close(newsockfd);
			close(sockfd);
		}
		else if(download_choice == 2)
		{
			read(newsockfd,filename,sizeof(filename));
			
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
				send(newsockfd,filedata,bytesRead,0);
			}	
	
			printf("File Send successfully!");
	
			//closing the file and socket...
			fclose(file);
			close(newsockfd);
			close(sockfd);
		}
		else if(download_choice == 3)
		{
			read(newsockfd,filename,sizeof(filename));
			
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
				send(newsockfd,filedata,bytesRead,0);
			}	
	
			printf("File Send successfully!");
	
			//closing the file and socket...
			fclose(file);
			
			close(newsockfd);
			close(sockfd);
		}
		else if(download_choice == 4)
		{
			read(newsockfd,filename,sizeof(filename));
			
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
				send(newsockfd,filedata,bytesRead,0);
			}	
	
			printf("File Send successfully!");
	
			//closing the file and socket...
			fclose(file);
			
			close(newsockfd);
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 3)
	{
		bzero(filename,sizeof(filename));
		bzero(filedata,sizeof(filedata));
		
		int flag = 1;
		int delete_choice;
		//receive the delete_choice from the client;
		n = read(newsockfd,&delete_choice,sizeof(int));
		if(n<0)
		{
			error("Error While Reading the Delete Choice!");
		}
		
		if(delete_choice == 1)
		{
			read(newsockfd,filename,sizeof(filename));
			
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			const char *insert_sql = "DELETE FROM TextTable WHERE FileName = (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			//Deleting from the machine...
			if(remove(filename) == 0)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nUnable to Delete the File!");
				flag = 0;
			}
			write(newsockfd,&flag,sizeof(int));
			close(newsockfd);
			close(sockfd);
		}
		else if(delete_choice == 2)
		{
			read(newsockfd,filename,sizeof(filename));
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			const char *insert_sql = "DELETE FROM ImageTable WHERE FileName = (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			//Deleting from the machine...
			if(remove(filename) == 0)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nUnable to Delete the File!");
				flag = 0;
			}
			write(newsockfd,&flag,sizeof(int));
			close(newsockfd);
			close(sockfd);
		}
		else if(delete_choice == 3)
		{
			read(newsockfd,filename,sizeof(filename));
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			const char *insert_sql = "DELETE FROM VideoTable WHERE FileName = (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			//Deleting from the machine...
			if(remove(filename) == 0)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nUnable to Delete the File!");
				flag = 0;
			}
			write(newsockfd,&flag,sizeof(int));
			close(newsockfd);
			close(sockfd);
		}
		else if(delete_choice == 4)
		{
			read(newsockfd,filename,sizeof(filename));
			sqlite3 *db;		//connnection object
			char *errMessage = 0;

			int rc = sqlite3_open("storage.db",&db);	//connection establishing...

			if(rc)
			{
				fprintf(stderr,"Can't open database : %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			const char *insert_sql = "DELETE FROM AudioTable WHERE FileName = (?);";
	
			sqlite3_stmt *stmt;
			rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
			if(rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			
			sqlite3_bind_text(stmt, 1, filename, -1, SQLITE_STATIC);
			
			rc = sqlite3_step(stmt);

			if(rc != SQLITE_DONE)
			{
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			
			//Deleting from the machine...
			if(remove(filename) == 0)
			{
				printf("\nFile Delete Successfully!");
			}
			else
			{
				printf("\nUnable to Delete the File!");
				flag = 0;
			}
			write(newsockfd,&flag,sizeof(int));
			close(newsockfd);
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 4)
	{
		char buffer[1024]={0};
		int view_choice;
		//receive the view_choice from the client;
		n = read(newsockfd,&view_choice,sizeof(int));
		if(n<0)
		{
			error("Error While Reading the View Choice!");
		}
		
		if(view_choice == 1)
		{
			sqlite3 *db;
			if(sqlite3_open("storage.db",&db))
			{
				fprintf(stderr,"Cannot open Database: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			const char *sql = "SELECT Id,FileName FROM TextTable;";
			sqlite3_stmt *res;
	
			if(sqlite3_prepare_v2(db,sql,-1,&res,0)!=SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			//Fetch and send data to client...
			while(sqlite3_step(res) == SQLITE_ROW)
			{
				snprintf(buffer, 1024, "%d|%s\n",
				sqlite3_column_int(res, 0),
				sqlite3_column_text(res, 1));
		
				write(newsockfd, buffer, strlen(buffer));
			}
	
			sqlite3_finalize(res);
			sqlite3_close(db);
			
			close(newsockfd);
			close(sockfd);
		}
		else if(view_choice == 2)
		{
			sqlite3 *db;
			if(sqlite3_open("storage.db",&db))
			{
				fprintf(stderr,"Cannot open Database: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			const char *sql = "SELECT Id,FileName FROM ImageTable;";
			sqlite3_stmt *res;
	
			if(sqlite3_prepare_v2(db,sql,-1,&res,0)!=SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			//Fetch and send data to client...
			while(sqlite3_step(res) == SQLITE_ROW)
			{
				snprintf(buffer, 1024, "%d|%s\n",
				sqlite3_column_int(res, 0),
				sqlite3_column_text(res, 1));
		
				write(newsockfd, buffer, strlen(buffer));
			}
	
			sqlite3_finalize(res);
			sqlite3_close(db);
			
			close(newsockfd);
			close(sockfd);
		}
		else if(view_choice == 3)
		{
			sqlite3 *db;
			if(sqlite3_open("storage.db",&db))
			{
				fprintf(stderr,"Cannot open Database: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			const char *sql = "SELECT Id,FileName FROM VideoTable;";
			sqlite3_stmt *res;
	
			if(sqlite3_prepare_v2(db,sql,-1,&res,0)!=SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			//Fetch and send data to client...
			while(sqlite3_step(res) == SQLITE_ROW)
			{
				snprintf(buffer, 1024, "%d|%s\n",
				sqlite3_column_int(res, 0),
				sqlite3_column_text(res, 1));
		
				write(newsockfd, buffer, strlen(buffer));
			}
	
			sqlite3_finalize(res);
			sqlite3_close(db);
			
			close(newsockfd);
			close(sockfd);	
		}
		else if(view_choice == 4)
		{
			sqlite3 *db;
			if(sqlite3_open("storage.db",&db))
			{
				fprintf(stderr,"Cannot open Database: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			const char *sql = "SELECT Id,FileName FROM AudioTable;";
			sqlite3_stmt *res;
	
			if(sqlite3_prepare_v2(db,sql,-1,&res,0)!=SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n",sqlite3_errmsg(db));
				sqlite3_close(db);
				exit(EXIT_FAILURE);
			}
	
			//Fetch and send data to client...
			while(sqlite3_step(res) == SQLITE_ROW)
			{
				snprintf(buffer, 1024, "%d|%s\n",
				sqlite3_column_int(res, 0),
				sqlite3_column_text(res, 1));
		
				write(newsockfd, buffer, strlen(buffer));
			}
	
			sqlite3_finalize(res);
			sqlite3_close(db);
			
			close(newsockfd);
			close(sockfd);
		}
		else
		{
			exit(1);
		}
	}
	else if(choice == 5)
	{
		return 0;
	}
	else
	{
		exit(1);
	}
	
	return 0;
}
