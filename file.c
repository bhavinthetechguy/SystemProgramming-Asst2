//BHAVIN PATEL

#include "indexer.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "indexer.c"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void filePrint(char* filename, struct List* arr){
    
    FILE* fp;
    fp = fopen(filename, "r");
    
    if(fp != NULL){
        //error
        //then there is an overwrite, and ask them if they want to or not
        char * check = malloc(5);
        printf("Do you want to overwrite in this file?\nSay y/n\n");
        scanf("%s",check);
        while((strcmp(check,"y"))!=0 && (strcmp(check,"n"))!=0){
            printf("Enter either y/n\n");
            scanf("%s",check);
        }
        if((strcmp(check,"n"))==0){
            
            free(check);
            fclose(fp);
            return;
        }
        fclose(fp);
        free(check);
    }
    
    fp = fopen(filename,"w");
    
    //file exists
    
    int i;
    for(i=0; i< 36; i++){
        
        if(arr[i].head != NULL){
            
            //token list exists
            struct Token* temp;
            
            temp = arr[i].head;
            while(temp != NULL){
                
                fprintf(fp, "<list> %s\n", temp->word);
                
                if(temp->head != NULL){
                    //file list exists
                    
                    struct File* tempFile= temp->head;
                    int count = 0;
                    
                    while(tempFile!= NULL){
                        if(count==5){//it should be go onto the new line, and get printed there.
                            fprintf(fp,"\n");
                            count = 0;
                        }
                        fprintf(fp, "%s %d", tempFile->filename, tempFile->freq);
                        count++;
                        tempFile = tempFile->next;
                        if(tempFile != NULL){
                            fprintf(fp, " ");
                        }
                    }
                    fprintf(fp, "\n");
                    
                }
                
                temp = temp->next;
            }			
        }			
    }	
    
    fclose(fp);
    return;
    
}

int Dir_Check(char * name){ 
	
	char* test = strstr(name, ".txt");
	if(test == NULL){
		//chcking for directory
		return 1;
	}
	else
		return 0;
	
}



int Read_File(char* filepath, struct List* arr, char* filename){
	
	FILE* fp;
	long str_length;
	struct stream* line = (struct stream *) malloc(sizeof(struct stream));
	
	fp =fopen(filepath, "r");
	
	if(fp == NULL){
		// checking for error
		printf("file or directory does not exist\n");
		return -1;
	}
	
	else{
		
		fseek(fp, 0, SEEK_END);
		str_length = ftell(fp);
		if(str_length == 0)
		{
			printf("There is nothing in this file\n");
			return -1;
			}
		char buffer[str_length];
		rewind(fp);
		while(fscanf(fp, "%s", buffer) != EOF){
			line->contents = buffer;
			line->start = 0;
			
			char* word = getnextToken(line);
			int index = getIndex(word);
			while(index !=-1){
				
				insertToken(index, arr, word, filename, filepath);

				struct Token* temp = arr[index].head;
					
				word = getnextToken(line);
				index = getIndex(word);
			}
		}
	}
	
	return 0;
}






int RECURSION(char* dirname, struct List* arr){
	
	DIR *dir;
	struct dirent * directory;
	
	
	if((dir = opendir(dirname)) != NULL){
		
		while((directory = readdir(dir)) != NULL){

			if(strcmp(".",directory->d_name) !=0 && strcmp("..",directory->d_name)!=0){
				
				char* pathname;
					pathname = malloc(strlen(dirname)+ strlen(directory->d_name) + 2);
					strcat(pathname, dirname);
					strcat(pathname, "/");
					strcat(pathname, directory->d_name);
				
				if(Dir_Check(pathname) == 1){
				// checking for directory
			
					RECURSION(pathname,arr);
				
				}
				else{

					Read_File(pathname, arr, directory->d_name);
				}
			}
		}
	}
	
	else{
		int check = Read_File(dirname, arr, dirname);
		if(check == -1)
			return -1;
	}
	return 0;
}

int main(int argc, char* argv[]){
	
			if(argc==2&&strcmp(argv[1], "-h") == 0){
				printf("This program inversely indexes the contents of a file or directory.\n"
					"It lists every word and the files that contain it, with their frequencies.\n"
					"Arg 1: the file you want to output to. \n"
					"Arg 2: the file or directory you want to RECURSION.\n");
				return 0;
			}
			if(argc!=3)
			{
				printf("INVALID INPUT\n");
				return 0;
				}
	
	
	struct List* total_str = malloc(sizeof(struct List)*36); 
	int i;
	for(i=0;i<36;i++){
		total_str[i].head=NULL; 
	}
	
int check =	RECURSION(argv[2], total_str);
	if(check == -1){
	destroyList(total_str);
	return 0;
	
	}

	filePrint(argv[1], total_str);
	destroyList(total_str);
	return 0;
}
