#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define silent_duration  (20000)
#define silent_min_plus (5000)
#define silent_min_minus (-5000)

bool checkSlirent(uint32_t buf2) {

    short buf = (short) buf2;
    if ((buf < silent_min_plus ) && (buf > silent_min_minus )) {
        return true;
    }
    //printf("%x\n", buf2);
    return false;
}
int main(int argc , char **argv)
{
    
    printf("%s\n", argv[1]);
    FILE *in;

    in = fopen(argv[1], "r");
    if(in == NULL) {
        printf("param err %s\n",argv[1]);
        return -1;
    }

    int nextfilenumber = 1;
    for(;;) {
        uint32_t inbuf;
        size_t readsize= fread(&inbuf, 1, 4, in);
        //printf("%x\n", inbuf);
        if (readsize < 4) {
            printf("end of file %ld\n", readsize);
            return 0;
        }
        
        
        if (checkSlirent(inbuf)) {
            continue;
        }
        char nextfilename[256];
        char nextfilename1[256];
        memset(nextfilename1, 0, 256);
        int len = strlen(argv[1]);
        strncpy(nextfilename1,argv[1], len - 4);
        sprintf(nextfilename,"%s_%03d.cutraw",nextfilename1, nextfilenumber);
        nextfilenumber++;
        FILE *outfile = fopen(nextfilename, "w");
        int silent_counter = 0;
        for(;;) {
            size_t readsize= fread(&inbuf, 1,4, in);
            //printf("%x\n", inbuf);
            if (readsize < 4) {
                printf("end of file\n");
                fclose(outfile);
                return 0;
            }
            if (checkSlirent(inbuf)) {
                silent_counter++;
                if (silent_counter > silent_duration) {
                    break;
                }
            } else {
                silent_counter=0;
            }
            size_t write_size = fwrite(&inbuf, 1, 4, outfile);
            if (write_size < 4) {
                printf("error %d\n", __LINE__);
                return -1;
            }

        }
        fclose(outfile);
    }

    return 0;
}
