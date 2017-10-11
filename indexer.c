#include "indexer.h" 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> 

void insertToken(int index, struct List *arr, char* tok,  char* filename, char* path){ 
    
            struct Token* tempTok = arr[index].head; 
            int exists = 0; 
            if(arr[index].head == NULL){ 
                // produced list is going to be emptylist
                tempTok = createToken(tok); 
                arr[index].head = tempTok; 
                tempTok->head = createFile(filename,path);
                
                return; 
            } 
            else{ 
               
                while(tempTok != NULL){ 
                    if(strcmp(tempTok->word, tok) == 0){ 
                 
                        File_Sort(filename, tempTok, path); 
                        exists = 1; 
                        return; 
                      
                    } 
                    tempTok = tempTok->next; 
                  
                } 
            } 
              
            if(exists == 0){ 
                //does not exist
                struct Token* newToken; 
                struct Token* temp; 
                newToken = createToken(tok); 
                tempTok = arr[index].head;
                
                if(strcmp(tempTok->word, newToken->word) > 0){ 
                    //newToken is added and in the pront
                    newToken->next = tempTok; 
                    arr[index].head = newToken; 
                    File_Sort(filename, newToken, path); 
                    return; 
                } 
                  
                while(tempTok != NULL){ 
                  
                  
                    if(tempTok->next ==NULL){ 
                        tempTok->next = newToken;
                        File_Sort(filename,newToken,path);
                        return; 
                    } 
  
                    if(strcmp(tempTok->next->word, newToken->word) > 0){ 
                    
  
                        temp = tempTok->next; 
                        tempTok->next = newToken; 
                        newToken->next = temp; 
                        File_Sort(filename, newToken, path); 
                        return; 
                          
                      
                    } 
                    tempTok = tempTok->next; 
                } 
  
            } 
              
            return; 
        } 

struct Token* createToken(char* word){ 

            if(word == NULL){ 
              
                return NULL; 
            } 
            else{ 
                struct Token* newTok = (struct Token*)malloc(sizeof(struct Token)); 
                newTok->word = word; 
                newTok->head = NULL; 
                newTok->next = NULL; 
                return newTok; 
              
            } 
            return NULL; 
          
        } 
          
/* 
it would check for the dynamiccaly allocated stuff and create file at this point
          
*/
          
struct File* createFile(char* filename, char * path){ 
          
    
            //error check
            if(filename == NULL){ 
                return NULL; 
            } 
              
            struct File* newFile = (struct File*)malloc(sizeof(struct File)); 
            newFile->filename=malloc(strlen(filename)+1);
            strcpy(newFile->filename,filename); 
            if(path==NULL) 
                newFile->pathname= NULL;
            else{
                char * fi = malloc(strlen(path)+1);
                strcpy(fi,path);
                newFile->pathname = fi;}
                newFile->next = NULL; 
                newFile->freq = 1; 

            return newFile; 
          
          
        }
      
     
/*
it would sort it and see if the word exists and it would increment 
 the freq
*/
void File_Sort(char* filename, struct Token* tok, char* path){ 
    
            int exists = 0; 
              
            struct File* temp; 
              
            //if there is no file 
              
            if(tok->head==NULL){ 
                tok->head=createFile(filename,path); 
                return; 
              
            } 
              
              
            if(tok->head != NULL){ 
           
                temp = tok->head; 
                while(temp != NULL){ 
                                    if(strcmp(temp->filename, filename) == 0 ){
 
              
                         
                        if(path!=NULL && (temp->pathname!=NULL)){
                 
                        if(strcmp(path,temp->pathname)==0){
                        temp -> freq = temp->freq +1; 
                    
                        exists = 1; 
                        break; 
                     }}
                        else if(path==NULL && temp->pathname==NULL){
                            temp->freq = temp->freq+1;
                            exists = 1;
                            break;
                             
                            }
                    }    
                    temp = temp->next; 
                } 
            } 
    
            if(exists == 0){ 
                struct File* newFile = createFile(filename, path);
              
                if(newFile == NULL){ 
              
                    return; 
                } 
                newFile->next = tok->head; 
                tok->head = newFile; 
              
                  
                  
            } 
    
                struct File* temp2 = tok->head->next;  
                temp = tok->head; 
                struct File * ptrc = NULL; 
                struct File * ptr; 
                struct File * hold = temp; 
            while(temp!= NULL){
                    hold = temp; 
                    temp2 = hold->next; 
                    while(temp2!=NULL){
                        if((temp2->freq)>(hold->freq)){ 
                          
                            hold = temp2; 
                        } 
                        temp2 = temp2->next; 
                    } 
                                        temp2 = tok->head;
                    if(hold==tok->head){//first node 
                        tok->head = hold->next; 
                        temp = tok->head; 
                        if(ptrc==NULL){//first node 
                            ptrc = hold; 
                            ptr = hold; 
                            //ptr = hold; 
                            ptr->next = NULL; 
                        } 
                        else{//not the first node 
                            ptr->next = hold; 
                            ptr = hold; 
                            ptr->next = NULL; 
                        } 
                    } 
                    else{
                        // pointer will be found before
                        temp2 = tok->head; 
                        while(temp2->next!=hold){ 
                            temp2 = temp2->next; 
                        } 
                        temp2->next = hold->next; 
                        if(ptrc==NULL){//node is first node
                            ptrc = hold; 
                         
                            ptr = hold; 
                            ptr->next = NULL; 
                        } 
                        else{//not the first node 
                            ptr->next = hold; 
                            ptr = hold; 
                            ptr->next = NULL; 
                        } 
                      
                    }    
                      
                temp = tok->head;     
                      
                } 
                  
                tok->head = ptrc; 
                  
                  
                  
                  

            return; 
          
        }        
/* 
    get the file, CALL tokenize, populate lists 
    repeat 
    (+ error checking) 
*/
  
char* LowerCase(char* word){ 
    
    if(word==NULL) return NULL; 
    if(strlen(word)==0)return ""; 
    int i;
    for(i=0; i<strlen(word); i++){ 
        word[i] = tolower(word[i]); 
    } 
  
    return word; 
} 
  
/* 
    gets index for list 
    and return -1 if its an error 
      
*/
int getIndex(char* word){ 
  
    if(strlen(word)==0 || word==NULL){
        return -1; 
    } 
    char placement; 
    int len = strlen(word); 
    int index = -1; 
    int i; 
    for(i =0; i<len;i++){ 
        strncpy(&placement,word+i,1); 
        if(isalnum(placement))break;     
          
    } 
    //gives the index according to its ascii value 
      
    if(isdigit(placement)){ 
          
        index = placement - 48; 
          
    } 
      
    else if(isalpha(placement)){ 
        index = placement - 87; 
    } 
      
    return index; 
  
  
} 
        /* 
            list will be freed
        */
void destroyList(struct List* arr){ 
  
    struct Token * ptr;
    struct Token * ptr2;
    struct File  * first;//iterating through file names
    struct File * next;//iterating through file names
      
    //loops, to delete all the allocated memory
    int i;
    for(i=0; i<36; i++){//iterate through the array
      
        //to iterate through tokens 
        if(arr[i].head!=NULL){ 
            ptr = arr[i].head; 
            ptr2 = ptr; 
            ptr = ptr->next; 
              
              
            while(ptr2!=NULL){//to iterate through tokens 
                      
                if(ptr2->head!=NULL){//to set iterate through file names 
                    first = ptr2->head; 
                    if(first!=NULL)next = first->next; 
                    else next = NULL; 
                    while(next!=NULL){ 
                        //free(first->filename); 
                        if(first->pathname!=NULL){
                            free(first->pathname);
                            }
                        free(first->filename);
                        free(first); 
                        first = next; 
                        next = next->next; 
                                      
                    }
                     
                if(first!=NULL){
                    if(first->pathname!=NULL)
                        free(first->pathname);
					free(first->filename);
                    free(first); 
                    }
                    }    
                free(ptr2->word); 
                free(ptr2); 
                ptr2 = ptr; 
                if(ptr!=NULL)ptr = ptr->next; 
            }
        } 
      
    } 
  
    free(arr); 
    return; 
  
} 
char* getnextToken(struct stream * words){ 
  
    if(words == NULL){
        return NULL; 
    } 
    if(words->start == strlen(words->contents)){ 
        return ""; 
    } 
    char* token; 
    char* lowertok; 
    int i; 
    int start2 = words->start;
      
    lowertok = LowerCase(words->contents); 
 
    for(i=start2;i<strlen(lowertok);i++){
 
        if(isalnum((unsigned char)lowertok[i])==0){
            if(start2==words->start){//the first word is not a letter or num
                start2++;
                words->start = words->start+1;
                 
                }
            else{//seperator
                break;
            }   
             
            }
        else{
 
            words->start = words->start+1;
        }
 
 
        } 
    words->start = words->start+1; 
    if((start2+1)==words->start){//this means that there is nothing left to be tokenized 
    return "";   
    } 
    token = malloc(sizeof(char)+(words->start - start2)+1); 
    if(token == NULL){//error checking 
        return NULL; 
    } 
      
    int toki= 0; 
    for(i = start2; i < (words->start-1); i++){ 
        token[toki] = words->contents[i]; 
        toki++; 
          
    } 
      
    token[toki] = '\0'; 
    return token;
          
}
