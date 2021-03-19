/*
 * Include the provided hashtable library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * this hashtable stores the dictionary.
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process, in the same way which this does.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /* main in C should always return 0 as a way of telling
     whatever program invoked this that everything went OK
     */
  return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience.
 */
unsigned int stringHash(void *s) {
	//djb2 algorithm
	//cited from https://stackoverflow.com/questions/1579721/why-are-5381-and-33-so-important-in-the-djb2-algorithm
  char *string = (char *) s;
  unsigned int hash = 5381;
  int c;
  while ((c = *string++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  if(strcmp(string1,string2)==0){
    return 1;
  }
  else{
    return 0;
  }
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with returns in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *filename) {
  FILE *fdict=fopen(filename,"r");
  if(fdict==NULL){
    fprintf(stderr,"The file requested does not exist.");
    exit(0);
  }
  int malsize = 60;
  char *strtemp = (char *)malloc(malsize);
    //TODO
  int charget=fgetc(fdict);
  int counter = 0;
  while(charget!=EOF){
    if(counter==malsize){
        malsize*=2;
        char *strtemp2 = (char*)malloc(malsize);
        for(int i =0;i<malsize;i++){
            if(i<counter){
                strtemp2[i]=strtemp[i];
            }
            else{
                strtemp2[i]=0;
            }
        }
        free(strtemp);//?
        strtemp = strtemp2;
    }
    if(charget=='\n') {
        strtemp[counter] ='\0';
        char *keyget = (char*)malloc((strlen(strtemp)+1)*sizeof(char));
        strcpy(keyget,strtemp);
        if(findData(dictionary,keyget)==NULL){
            insertData(dictionary,keyget,keyget);
        }
        malsize = 60;
        memset(strtemp,0,strlen(strtemp));
        counter = 0;
        charget = fgetc(fdict);
        continue;
    }
    if((char)charget!='\r'){
		strtemp[counter]=(char)charget;
    }
    counter++;
    charget = fgetc(fdict);
  }
  free(strtemp);
  fclose(fdict);
}

/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".  All words should be checked
 * against the dictionary as they are input, again with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the
 * error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 60 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 60 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */
void processInput() {
  int charget=0;
  int mallocsize = 60;
  char * strget = (char*)malloc(mallocsize);
  int counter = 0;
  charget = fgetc(stdin);
  while((char)charget!=EOF){
	if(counter==mallocsize){
		mallocsize*=2;
		char *strnew = (char*)malloc(mallocsize);
		for(int i =0;i<mallocsize;i++){
			if(i<counter){
				strnew[i]=strget[i];
			}
			else{
				strnew[i]=0;
			}
		}
		free(strget);
		strget = strnew;
	}
    if(isalpha((char)charget)==0){
		//construct another two string
		char *stralllower = (char*)malloc(mallocsize);
		char *strallloweref = (char*)malloc(mallocsize);
		for(int i =0;i<mallocsize;i++){
			if(i<counter){
				if(i==0){
					stralllower[i]=tolower(strget[i]);
					strallloweref[i]=strget[i];
				}
				else{
					stralllower[i]=tolower(strget[i]);
					strallloweref[i]=tolower(strget[i]);
				}
			}
		}
		stralllower[counter]='\0';
		strallloweref[counter]='\0';
		if((char)charget=='\n'){
			if(isalpha(strget[0])){
				strget[counter]='\0';
				if(findData(dictionary,strget)==NULL&&
					findData(dictionary,stralllower)==NULL&&
					findData(dictionary,strallloweref)==NULL){
					fprintf(stdout,"%s [sic]%c",strget,(char)charget);
				}
				else{
					if(strget[0]!='\0'){
						fprintf(stdout,"%s%c",strget,(char)charget);
					}
				}
			}
			else{
				fprintf(stdout,"%c",(char)charget);
			}
//insert
		}
		else if(isspace((char)charget)){
			//strget[counter]='\0';
			//if(strget[0]!='\0'){
			if(isalpha(strget[0])){
				strget[counter]='\0';
				if(strget[0]!='\0'){
					fprintf(stdout,"%s",strget);
			    }
				if(findData(dictionary,strget)==NULL&&
					findData(dictionary,stralllower)==NULL&&
					findData(dictionary,strallloweref)==NULL){
					fprintf(stdout," [sic]");
				}
			}
			fprintf(stdout," ");
		}
		else{
			if(isalpha(strget[0])){
				strget[counter]='\0';
				if(strget[0]!='\0'){
					fprintf(stdout,"%s",strget);
				}
				if(findData(dictionary,strget)==NULL&&
					findData(dictionary,stralllower)==NULL&&
					findData(dictionary,strallloweref)==NULL){
					fprintf(stdout," [sic]%c",(char)charget);
				}
				else{
					fprintf(stdout,"%c",(char)charget);
				}
		    }
		    else{
				fprintf(stdout,"%c",(char)charget);
			}
			//
		}
		free(stralllower);
		free(strallloweref);
		counter=0;
		memset(strget,0,strlen(strget));
    }
    else{
		strget[counter]=(char)charget;
		counter++;
    }
	charget = fgetc(stdin);
  }
  //
  if (isalpha(strget[0])) {
	  //
	  char *stralllower = (char*)malloc(mallocsize);
		char *strallloweref = (char*)malloc(mallocsize);
		for(int i =0;i<mallocsize;i++){
			if(i<counter){
				if(i==0){
					stralllower[i]=tolower(strget[i]);
					strallloweref[i]=strget[i];
				}
				else{
					stralllower[i]=tolower(strget[i]);
					strallloweref[i]=tolower(strget[i]);
				}
			}
		}
		stralllower[counter]='\0';
		strallloweref[counter]='\0';
	  //
        strget[counter] = '\0';
        if(findData(dictionary,strget)==NULL&&
					findData(dictionary,stralllower)==NULL&&
					findData(dictionary,strallloweref)==NULL){
					fprintf(stdout,"%s [sic]",strget);
		} 
		else {
			if (strlen(strget) != 0) {
				fprintf(stdout, "%s", strget);
			}
		}
		free(stralllower);
		free(strallloweref);
	}
   //
  free(strget);
}
//
