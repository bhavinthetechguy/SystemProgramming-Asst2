#ifndef INDEXER_H 
#define INDEXER_H 
  
          
          
        struct File{ 
            char * filename; 
            int freq; 
            char * pathname; 
            struct File * next; 
                      
        }; 
          
        struct Token{
            char * word; //stores the name of the word 
            struct File * head;//pointer to the file 
            struct Token * next; //pointer to the next token 
              
        }; 
        struct List{
            struct Token * head;//pointing to the token 
          
        }; 
          
        struct stream{ 
            char* contents; 
            int start; 
		}; 
          
		/* 
		 *it would insert token into the linked list
		 */
          
        void insertToken(int index, struct List arr[], char* tok,  char* filename, char* path); 
/*
 free list
 */
void destroyList(struct List arr[]);

char* getnextToken(struct stream * words);

int Dir_Check(char * name);

int Read_File(char* filepath, struct List* arr, char* filename);
        /*
        it allocates memory for the token
         
        */
        struct Token* createToken(char* word);
          
        /* 
          it would allocate space for file struct
          
        */
          
        struct File* createFile(char* filename,char*path); 
          
        /* 
            checks if file exists 
                if it exists, update freq 
                else all createFile, add to end of list 
        */
        void File_Sort(char* filename, struct Token* tok,char*path);
          
          
        /* 
            gets index for list 
        */
        int getIndex(char* word); 
          

        
        /*
it would print out the whole file		*/

		void printFile(char* outFile, struct List* arr);
         
        /*
		 * itw ould go through the whole directory		 */

		int RECURSION(char* dirname, struct List* arr);
          
#endif 
