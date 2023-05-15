/*****************************************************************************
 *  _        _______  ______        _______
 * ( \      (  ___  )(  ___ \      / ___   )
 * | (      | (   ) || (   ) )     \/   )  |
 * | |      | (___) || (__/ /          /   )
 * | |      |  ___  ||  __ (         _/   /
 * | |      | (   ) || (  \ \       /   _/
 * | (____/\| )   ( || )___) )     (   (__/\
 * (_______/|/     \||/ \___/      \_______/
 *
 *  _______  _______  _______           _______       _______
 * (  ____ \(  ____ )(  ___  )|\     /|(  ____ )     (  ____ \
 * | (    \/| (    )|| (   ) || )   ( || (    )|     | (    \/
 * | |      | (____)|| |   | || |   | || (____)|     | (____
 * | | ____ |     __)| |   | || |   | ||  _____)     (_____ \
 * | | \_  )| (\ (   | |   | || |   | || (                 ) )
 * | (___) || ) \ \__| (___) || (___) || )           /\____) )
 * (_______)|/   \__/(_______)(_______)|/            \______/
 *
 * Members:
 *    Jaylen Avtarovski
 *    Elie Habib
 *    Quoc Hao Vo-Nguyen
 *    Xiang Hong Ong
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/* Used to handle local timestamps for logging */
#include <time.h> /* time_t, strftime, localtime, mktime */
#include "main.h"

#define ENCRYPTION_KEY 13

int main(void)
{
  FILE *fps, *fpt;
  char filename[50];
  log_node *log_list = readLogsFromFile(LOG_FILE);
  printf("Welcome! \n");

  while (1)
  {
    int decision = mainMenu();
    int j,k = 0;
    char finalname[65];
    while(k<36) {
      finalname[k] = '\0';
      k++;
    }

    switch (decision)
    {
    case 1:
      printf("Enter Filename to be Encrypted: ");
      scanf("%s", filename);
      if ((fps = readFile(filename)) == NULL)
      {
        break;
      }
      encryptFile(fps, filename);
      /*FILE *fpu = fopen("encrypted.bin", "wb");*/
      j = 0;
      while(j < 50) {
        finalname[j] = filename[j];
        j++;
      }
      strcat(finalname, "_encrypted.bin");
      log_list = insertLogEntry(log_list, "encrypt",
                                filename, finalname);
      writeLogsToFile(log_list, LOG_FILE);
      break;
    case 2:
      printf("Enter Filename to be Decrypted: ");
      scanf("%s", filename);
      /*if ((fps = readFile(filename)) == NULL)
      {
        break;
      }*/
      if ((fps = fopen(filename, "rb")) == NULL) {
        printf("File Not Found");
        break;
      }
      fpt = decryptFile(fps);
      j = 0;
      while(j < 50) {
        finalname[j] = filename[j];
        j++;
      }
      strcat(finalname, "_decrypted.txt");
      writeFile(fpt, finalname);
      log_list = insertLogEntry(log_list, "decrypt",
                                filename, finalname);
      writeLogsToFile(log_list, LOG_FILE);
      break;
    case 3:
      printf("Enter Filename to be Compressed: ");
      scanf("%s", filename);
      if ((fps = readFile(filename)) == NULL)
      {
        break;
      }
      j = 0;
      while(j < 50) {
        finalname[j] = filename[j];
        j++;
      }
      strcat(finalname, "_compressed.txt");
      writeFile(fpt, finalname);
      log_list = insertLogEntry(log_list, "compress",
                                filename, finalname);
      writeLogsToFile(log_list, LOG_FILE);
      break;
    case 4:
      printf("Enter Filename to be Decompressed: ");
      scanf("%s", filename);
      if ((fps = readFile(filename)) == NULL)
      {
        break;
      }
      fpt = decompressFile(fps);
      j = 0;
      while(j < 50) {
        finalname[j] = filename[j];
        j++;
      }
      strcat(finalname, "_decompressed.txt");
      writeFile(fpt, finalname);
      log_list = insertLogEntry(log_list, "decompress",
                                filename, finalname);
      writeLogsToFile(log_list, LOG_FILE);
      break;
    case 5:
      printLogs(log_list);
      break;
    case 6:
      exit(0);
    default:
      int character;
      printf("Invalid choice.\n");
      while ((character = getchar()) != '\n' && character != EOF)
      {
      };
      /*Clear stdin*/
    }
  }
  return 0;
}

/* Main Menu Function to allow the user to choose what they want to do */
int mainMenu()
{
  int i = 0;

  printf("Choose an option:\n");
  printf("1) Encrypt a File\n");
  printf("2) Decrypt a File\n");
  printf("3) Compress a File\n");
  printf("4) Decompress a File\n");
  printf("5) Print logs\n");
  printf("6) Exit\n");
  printf("Choice: ");

  scanf("%d", &i);

  return i;
}

/******************************************************************************
 * This function encrypts a file by taking each characters ascii value and
 * modifying it through a predetermined equation, outputting the created ascii
 *
 * inputs:
 * file - the file to be encrypted
 * outputs:
 * file - the encrypted file
 *****************************************************************************/

/* Encrypts a file by shifting each character by ENCRYPTION_KEY */
FILE *encryptFile(FILE *file, char filename[20]) {
    char character, finalname[35];
    int charactercode, i = 0;
    int *fileContents = (int *)malloc(1024 * sizeof(int)); /* Allocate initial memory */
    FILE *encryptedFile;
    int j=0;
    while(j < 20) {
      finalname[j] = filename[j];
      j++;
    }
    strcat(finalname, "_encrypted.bin");

    if (fileContents == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    encryptedFile = fopen(finalname, "wb");
    if (encryptedFile == NULL) {
        perror("Failed to create temporary file");
        free(fileContents);
        return NULL;
    }

    while ((character = fgetc(file)) != EOF) {
        character += ENCRYPTION_KEY;
        charactercode = (int)character;
        fileContents[i++] = charactercode;
        if (i % 1024 == 0) { /* Allocate more memory if needed */
            fileContents = (int *)realloc(fileContents, (i + 1024) * sizeof(int));
            if (fileContents == NULL) {
                perror("Failed to allocate memory");
                fclose(encryptedFile);
                free(fileContents);
                return NULL;
            }
        }
    }

    fwrite(fileContents, sizeof(int), i, encryptedFile); /* Write only filled elements */

    free(fileContents);
    rewind(file);
    rewind(encryptedFile);
    return encryptedFile;
}


/*FILE *encryptFile(FILE *file)
{
  char character;
  / a rolling character to get the value of each character in the file 
  FILE *encryptedFile;
  character = fgetc(file);

  / create a temporary file which will automatically deleted later 
  encryptedFile = tmpfile();

  while (character != EOF)
  {
    character = character + 12;
    fputc(character, encryptedFile);
    character = fgetc(file);
  }
/ resets file position indicators back to the beginning for other processes 
  rewind(file);
  rewind(encryptedFile);
  return encryptedFile;
}*/

/******************************************************************************
 * This function decrypts a file by taking each binary value and modifying 
 * it through a predetermined equation, outputting the original characters
 *
 * inputs:
 * file - the file to be decrypted
 * outputs:
 * file - the decrypted file
 *****************************************************************************/
FILE *decryptFile(FILE *file) {
    int charactercode, i = 0;
    int *fileContents = (int *)malloc(1024 * sizeof(int)); /* Allocate initial memory */
    FILE *decryptedFile;

    if (fileContents == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    decryptedFile = tmpfile();
    if (decryptedFile == NULL) {
        perror("Failed to create temporary file");
        free(fileContents);
        return NULL;
    }

    while (fread(&charactercode, sizeof(int), 1, file) == 1) {
        charactercode -= ENCRYPTION_KEY;
        fputc((char)charactercode, decryptedFile);
        fileContents[i++] = charactercode;
        if (i % 1024 == 0) { /* Allocate more memory if needed */
            fileContents = (int *)realloc(fileContents, (i + 1024) * sizeof(int));
            if (fileContents == NULL) {
                perror("Failed to allocate memory");
                fclose(decryptedFile);
                free(fileContents);
                return NULL;
            }
        }
    }

    free(fileContents);
    rewind(file);
    rewind(decryptedFile);
    return decryptedFile;
}


/******************************************************************************
 * This function compresses a file by counting the number of characters
 * in a row and writing the count and character to a new file.
 * inputs:
 * file - the file to be compressed
 * outputs:
 * file - the compressed file
 *****************************************************************************/
FILE *compressFile(FILE *file)
{
  FILE *compressed;
  int current_char;

  /* create a temporary file which will automatically deleted later */
  compressed = tmpfile();

  if ((current_char = fgetc(file)) != EOF)
  {
    int count = 1;
    int next_char;

    /* loop through the file character by character until reaches the end */
    while ((next_char = fgetc(file)) != EOF)
    {
      if (next_char == current_char)
      {
        /* keep track of consecutive repeating characters */
        count++;
      }
      else
      {
        /* write the count and character to the compressed file */
        fprintf(compressed, "%d%c", count, current_char);
        current_char = next_char;
        count = 1;
      }
    }
    fprintf(compressed, "%d%c", count, current_char);
  }
/* resets file position indicators back to the beginning for other processes */
  rewind(file);
  rewind(compressed);
  return compressed;
}

/****************************************************************************
 * This function decompresses a file by reading the count of characters
 * in a row and writing the character to a new file count times.
 * inputs:
 * file - the file to be decompressed
 * outputs:
 * file - the decompressed file
 ***************************************************************************/
FILE *decompressFile(FILE *compressedFile)
{
  FILE *decompressed;
  int current_char;

  /* create a temporary file which will automatically deleted later */
  decompressed = tmpfile();

  /* loop through the file character by character until reaches the end */
  while ((current_char = fgetc(compressedFile)) != EOF)
  {
    int count = 0;
    char count_str[32];
    int count_position = 0;

    /* look for numbers to determine the count and build count string */
    while (current_char >= '0' && current_char <= '9')
    {
      count_str[count_position++] = current_char;
      current_char = fgetc(compressedFile);
    }
    /* add null terminator to count string */
    count_str[count_position] = '\0';

    /* convert the count string to integer */
    sscanf(count_str, "%d", &count);

    /* write the character to the decompressed file count times */
    int incrementer;
    for (incrementer = 0; incrementer < count; incrementer++)
    {
      fputc(current_char, decompressed);
    }
  }

/* resets file position indicators back to the beginning for other processes */
  rewind(compressedFile);
  rewind(decompressed);
  return decompressed;
}

/****************************************************************************
 * This function allow the program to read a file by reading the filename.
 * If there is an error when opening the file, it will return null and print
 * out the error.
 * If the name of the file does not exist, then it will also return null,
 * and print out stating that the file does not exist.
 * input:
 * filename - the name of the file
 * outputs:
 * filep - pointer to the file
 * or
 * null - any error that appear while reading the file
 ***************************************************************************/
FILE *readFile(char filename[])
{
  FILE *filep = NULL;

  if (filep = fopen(filename, "r"))
  {
    return filep;
  }
  else if (filep = NULL)
  {
    printf("Read Error.\n");
    return NULL;
  }
  printf("File does not exist.\n");
  return NULL;
}

/*****************************************************************************
 * This function allow the program to write a file by opening a new file
 * naming it with destinationName, and loop until reaching the end of the file
 * putting eveything into the new file from the file pointer filep.
 * If there is an error when opening a new file, it will stop the program
 * and print out the error.
 * inputs:
 * fpt - pointer to an existing file
 * destinationName - the name of the file
 * output:
 * nothing
 ****************************************************************************/
void writeFile(FILE *fpt, char destinationName[])
{
  char character;

  FILE *doneFile = fopen(destinationName, "w");
  if (doneFile == NULL)
  {
    printf("Write Error.\n");
    return;
  }

  while ((character = fgetc(fpt)) != EOF)
  {
    fprintf(doneFile, "%c", character);
  }

  fclose(doneFile);
}

/*****************************************************************************
 * This function creates a log entry and inserts it into the log linked list.
 * inputs:
 * head - the head of the log linked list
 * action - the action performed
 * input_file - the input filename
 * output_file - the output filename
 * outputs:
 * head - the new head of the log linked list
 *****************************************************************************/
log_node *insertLogEntry(log_node *head, const char *action,
                         const char *input_file, const char *output_file)
{
  /* Create a new log entry and insert it at the head of the linked list. */
  log_node *new_node = (log_node *)malloc(sizeof(log_node));
  /*  Adds the current time, action, input file, and output file to the new */
  new_node->entry.timestamp = time(NULL);
  strncpy(new_node->entry.action, action,
          sizeof(new_node->entry.action) - 1);
  strncpy(new_node->entry.input_file, input_file,
          sizeof(new_node->entry.input_file) - 1);
  strncpy(new_node->entry.output_file, output_file,
          sizeof(new_node->entry.output_file) - 1);
  /*  Set new node's next pointer to the current head of the linked list. */
  new_node->next = head;
  return new_node;
}

/*****************************************************************************
 * This function prints the log linked list.
 * inputs:
 * head - the head of the log linked list
 * outputs:
 * nothing
 *****************************************************************************/
void printLogs(log_node *head)
{
  log_node *current = head;
  char buffer[20];
  struct tm *time_info;
  /*
   * Loop through the linked list and print out the
   * timestamp, action, input file, and output file.
   */
  while (current != NULL)
  {
    time_info = localtime(&(current->entry.timestamp));
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", time_info);
    printf("Timestamp: %s\n", buffer);
    printf("Action: %s\n", current->entry.action);
    printf("Input file: %s\n", current->entry.input_file);
    printf("Output file: %s\n", current->entry.output_file);
    printf("\n");
    current = current->next;
  }
}

/*****************************************************************************
 * This function reads the log file and populates the log linked list.
 * inputs:
 * filename - the name of the log file
 * outputs:
 * head - the head of the log linked list
 *****************************************************************************/
log_node *readLogsFromFile(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return NULL;
  }

  log_node *head = NULL;
  char line[600];
  /* Loop through the log file and create a new log entry for each line. */
  while (fgets(line, sizeof(line), file) != NULL)
  {
    log_node *new_node = (log_node *)malloc(sizeof(log_node));
    if (new_node == NULL)
    {
      printf("Memory allocation failed.\n");
      break;
    }

    /* Parse the line and populate the log_entry fields */
    char timestamp_str[20];
    sscanf(line, "%[^,],%[^,],%[^,],%[^\n]",
           timestamp_str,
           new_node->entry.action,
           new_node->entry.input_file,
           new_node->entry.output_file);

    /* Initialize struct tm */
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    if (sscanf(timestamp_str, "%d/%d/%d %d:%d:%d",
               &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6)
    {
      /* Adjust month value to be zero-based */
      tm.tm_mon -= 1;
      /* Adjust year value to be years since 1900 */
      tm.tm_year -= 1900;
      new_node->entry.timestamp = mktime(&tm);
    }
    else
    {
      printf("Error parsing timestamp.\n");
      free(new_node);
      continue;
    }
/*  Move the current head of the linked list to the next pointer of the new */
    new_node->next = head;
    head = new_node;
  }

  fclose(file);
  return head;
}

/*****************************************************************************
 * This function writes the log linked list to the log file.
 * inputs:
 * head - the head of the log linked list
 * filename - the name of the log file
 * outputs:
 * nothing
 *****************************************************************************/
void writeLogsToFile(log_node *log_list, const char *filename)
{
  FILE *file = fopen(filename, "w");
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return;
  }

  /*
   * Loop through the linked list and print out the
   * timestamp, action, input file, and output file.
   */
  log_node *current = log_list;
  char buffer[20];
  int isFirstEntry = 1;

  while (current != NULL)
  {
    /* Generate formatted timestamp */
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", localtime(&(current->entry.timestamp)));
    if (!isFirstEntry)
    {
      fprintf(file, "\n");
    }
    else
    {
      isFirstEntry = 0;
    }
    fprintf(file, "%s", buffer);
    fprintf(file, ",%s", current->entry.action);
    fprintf(file, ",%s", current->entry.input_file);
    fprintf(file, ",%s", current->entry.output_file);
    /*  Move the current head of the linked list to the next pointer of the new */
    current = current->next;
  }

  fclose(file);
}

