#include <stdlib.h>
#include <stdio.h>
/* Used for the timestamp struct */
#include <time.h> /* timestamp */

#define LOG_FILE "log.txt"

typedef struct log_entry
{
  time_t timestamp;
  char action[32];
  char input_file[256];
  char output_file[256];
} log_entry;

typedef struct log_node
{
  log_entry entry;
  struct log_node *next;
} log_node;

/* Main Menu Function */
int mainMenu();
/* Read Data from a File */
FILE *readFile(char filename[]);
/* Write Data to a File */
void writeFile(FILE *fpt, char destinationName[]);

/* Encrypts Data */
FILE *encryptFile(FILE *file, char filename[20]);
/* Decrypts Encrypted Data */
FILE *decryptFile(FILE *file);
/* Compresses File */
FILE *compressFile(FILE *file);
/* Decompresses File */
FILE *decompressFile(FILE *file);
/* Inserts a new log entry to the linked list */
log_node *insertLogEntry(log_node *head, const char *action, const char *input_file, const char *output_file);
/* Prints linked list of logs */
void printLogs(log_node *head);
/* Reads logs from file and stores in linked list */
log_node *readLogsFromFile(const char *filename);
/* Writes linked list into a file */
void writeLogsToFile(log_node *log_list, const char *filename);
