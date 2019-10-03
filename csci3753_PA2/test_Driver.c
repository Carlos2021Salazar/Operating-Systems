#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define DEVICE "/dev/simple_character_device"
#define BUFFER_SIZE 1024

int main () 
{
    char cases; 
    char buffer[BUFFER_SIZE];
    int fileIO = open(DEVICE, O_RDWR);
    int offset;
    int whence;

    while (1) {
        printf("\nr) Read from device\nw) )Write to device\ns) Offset value\ne) Exit device\nAnything else to continue reading and writing\n\nEnter command: ");
        scanf("%c", &cases);
        switch (cases) { 
            case 'r':
                read(fileIO, buffer, BUFFER_SIZE);
                printf("Device output: %s\n", buffer);
                while (getchar() != '\n');
                break;
            case 'w':
                printf("Enter data you want to write to the device: ");
                scanf("%s", buffer);
                write(fileIO, buffer, BUFFER_SIZE);
                while (getchar() != '\n');
                break;
            case 's':
                printf("Enter an offset value: ");
                scanf("%d", &offset);
                printf("Enter a value for whence: (0, 1, 2)");
                scanf("%d", &whence);
                lseek(fileIO, offset, whence);
                break;  
            case 'e':
            	printf("Goodbye friend:\n");
                return 0;
            default:
                while (getchar() != '\n');
        }
    }
    close(fileIO);
    return 0;
}