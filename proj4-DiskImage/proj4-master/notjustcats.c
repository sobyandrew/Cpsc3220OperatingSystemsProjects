//Daniel Smith dnsmith
//Andrew Soby asoby
//notjustcats.c Project 4 CPSC 3220
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>

//Section to define numbers we need to use.
#define NUMSECTORS 2880
#define SECTORSIZE 512
#define ROOTDIR 19
#define ENTRIES_IN_SUBDIRECTORY 16
#define DATA 33
#define NUMENTRIES 224
#define ENTRYSIZE 32
#define LASTCLUSTER 0xFFF
#define RELATIVE_DIRECTORY 0x2E
#define DELETED_FILE 0xE5
#define FILENAME_LENGTH 8
#define FILE_EXT 3
#define FLC_OFFSET 26
#define SIZE_OFFSET 28
#define LARGE 1000

// retrieve the FAT entry of a cluster given its # and a disk image
int getFATEntry(int cluster, char * image){
  int numIndex, fatEntry;
  uint16_t firstByte, middleByte, lastByte; // FAT Entry is 1.5 bytes, so get 3 bytes and bit mask

  //use this to get correct 12 bits for the FAT Entry
  numIndex = ((cluster/2)*3 + SECTORSIZE);

  firstByte = (uint8_t)image[numIndex];
  middleByte = (uint8_t)image[numIndex + 1];
  lastByte = (uint8_t)image[numIndex + 2];

  if(cluster % 2 == 0){ // if even, get last middle nibble and 1st byte
    fatEntry = ((middleByte & 0x0F) << 8) | firstByte;
    return fatEntry;
  }
  else{ // if odd, get the last byte + 1st middle nibble
    fatEntry = ((middleByte & 0xF0) >> 4) | (lastByte << 4);
    return fatEntry;
  }
}

// write file contents to output directory
void writeFile(char *fileext, char *argv2, int size, char *contents) {
  //static int to continue the correct file number across multiple calls
  static int i = 0;
  char outputName[LARGE];
  //initialize first character in array to null so strcat will work.
  outputName[0] = '\0';

  // convert integer size to a string
  int len = snprintf(NULL, 0, "%d", i);
  char snum[len];
  snprintf(snum, len + 1, "%d", i++); // sneaky post-increment
  
  // form the path of the output file
  strcat(outputName, argv2);
  strcat(outputName, "/file");
  strcat(outputName, snum);
  strcat(outputName, ".");
  strcat(outputName, fileext);

  // write to output file
  FILE *outputDirectory = fopen(outputName, "w");
  if(contents != NULL) fwrite(contents, sizeof(char), size, outputDirectory);
  fclose(outputDirectory);
}

// return contents of a file given a first logical cluster, file size, and disk image
char *getFileContents(uint16_t flc, uint32_t size, char *image) {
  if(size == 0) return NULL; // handle file of size 0
  char *contents = malloc(size*sizeof(char));
  int remainingBytes = size;
  int offset = 0; // tracks byte position in contents array
  int sector = (DATA + flc - 2) * SECTORSIZE; //get sector #
  int FAT = getFATEntry(flc, image);

  // cluster is free (deleted)
  if(FAT == 0) {
    for(int i = 0; i < size; i++) { //iterate thru size and add to contents
      contents[i] = image[sector+i];
    }
  }

  // cluster is used (normal)
  else {
    // iterate through all but last cluster of a file
    while(FAT != LASTCLUSTER) {
        for(int i = 0; i < SECTORSIZE; i++) {
          contents[offset++] = image[sector+i]; // sneaky post-increment
        }
        remainingBytes -= SECTORSIZE;
        sector = (DATA + FAT - 2) * SECTORSIZE;
        FAT = getFATEntry(FAT, image);
    }
	 // account for last cluster of file when FAT == LASTCLUSTER (0xfff)
    for(int i = 0; i < remainingBytes; i++) {
      contents[offset++] = image[sector+i]; // sneaky post-increment
    }
  }
  return contents;
}

// recursive function to go through the entries in a directory
void goThroughDirectory(int numEntries, int sector, char * path, char *argv2, char *image) {
  uint8_t filename_firstbyte = (uint8_t)image[SECTORSIZE*sector]; // first byte of an entry
  int iter = 0; // this makes sure we don't go outside directory

  // while there are more entries in the directory and not outside the directory
  while(filename_firstbyte != 0 && iter < numEntries) {

    // SKIP relative directories '.' and '..'
    if(filename_firstbyte == RELATIVE_DIRECTORY) {
      iter++;
      filename_firstbyte = (uint8_t)image[SECTORSIZE*sector + iter*ENTRYSIZE];
      continue;
    }

	 // GET the filename
    char filename[FILENAME_LENGTH+1];
    for(int i = 0; i < FILENAME_LENGTH; i++) {
      filename[i] = image[SECTORSIZE*sector + iter*ENTRYSIZE+i];
      if(filename[i] == ' ') filename[i] = '\0'; // if space, terminate str w/ null char
    }

    // make sure to terminate str w/ null char if no spaces
    filename[FILENAME_LENGTH] = '\0';
	 // ...and check again for good measure?
    for(int i = 0; i < FILENAME_LENGTH+1; i++){
      if(filename[i] == ' '){
        filename[i] = '\0';
        break;
      }
      else if(filename[i] =='\0'){
        break;
      }
    }

	 // GET the file extension
    char fileext[FILE_EXT+1];
    for(int i = 0; i < FILE_EXT; i++) {
      fileext[i] = image[SECTORSIZE*sector + iter*ENTRYSIZE+FILENAME_LENGTH+i];
      if(fileext[i] == ' ') fileext[i] = '\0'; // if space, null terminate str
    }

    fileext[FILE_EXT] = '\0'; // if no space, still null terminate str
	 // ...and check again for some reason.
    for(int i = 0; i < FILE_EXT+1; i++){
      if(fileext[i] == ' '){
        fileext[i] = '\0';
        break;
      }
      else if(fileext[i] =='\0'){
        break;
      }
    }

	 // GET the file attributes bit
    uint8_t attributes = (uint8_t)image[SECTORSIZE*sector + iter*ENTRYSIZE+FILENAME_LENGTH+FILE_EXT];

    // GET the first logical cluster (2 bytes)
	 int flc = 0;
    for(int i = 0; i < 2; i++) {
      flc += (uint8_t)image[SECTORSIZE*sector + iter*ENTRYSIZE+FLC_OFFSET+i] << (8*i); //shift over by 1 byte
    }

	 // GET the size of the file (4 bytes)
    int size = 0;
    for(int i = 0; i < 4; i++) {
      size += (uint8_t)image[SECTORSIZE*sector + iter*ENTRYSIZE+SIZE_OFFSET+i] << (8*i); //shift over by 1 byte
    }

    // if directory entry is a directory
    if((attributes & 0x10) >> 4) {
      char bigEnough[LARGE];
      bigEnough[0] = '\0';
      strcat(bigEnough, path);
      strcat(bigEnough, filename);
      strcat(bigEnough, "/");
      goThroughDirectory(ENTRIES_IN_SUBDIRECTORY, (DATA + flc - 2), bigEnough, argv2, image);
    }

	 // else the directory entry is a file
    else {
      if(filename_firstbyte == DELETED_FILE) {
        printf("FILE\tDELETED\t");
        filename[0] = '_'; // insert an underscore as first char of a deleted file
      }

      else printf("FILE\tNORMAL\t");

      printf("%s%s.%s\t%d\n", path, filename, fileext, size);
      char *contents = getFileContents(flc, size, image); // get contents of file
      writeFile(fileext, argv2, size, contents); // write contents of file
    }

    iter++;
    filename_firstbyte = (uint8_t)image[SECTORSIZE*sector + iter*ENTRYSIZE];
  }
}

// main gets cmd line args, mmaps disk image into mem, and calls a function to
// go thru all entries in root directory
int main(int argc, char ** argv){

  if(argc != 3){
    printf("\nthere were not enough arguments passed in\n");
    return -1;
  }

  int file_descriptor = open(argv[1],O_RDONLY, S_IRUSR | S_IWUSR);

  struct stat fileStat;

	if (fstat(file_descriptor,&fileStat) == -1)
	{
		perror("couldn't get file size.\n");
	}

	char *image = mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, file_descriptor, 0);

  goThroughDirectory(NUMENTRIES, ROOTDIR, "/", argv[2], image);

  return 0;
}
