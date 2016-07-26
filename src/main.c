		// ./bin/adpcm -e -i file_mono.pcm -o file_mono.div4
// ./bin/adpcm -d -i file_mono.div4 -o file_mono1.pcm
// ./bin/adpcm -e -i file_stereo.pcm -o file_stereo.div4
// ./bin/adpcm -d -i file_stereo.div4 -o file_stereo1.pcm
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "adpcm.h"

typedef int (* CALLBACK) (const char* from, const char* to); 

int encode(const char* from, const char* to)
{
	FILE *pInFile, *pOutFile ;
	int iRead = 0;
	adpcm_state state;	
	unsigned char ucInBuff[512] = {0};
	unsigned char ucOutBuff[128] = {0};

	printf ("start encoding from %s to %s.\n", from, to);
    pInFile = fopen(from, "rb");
	if(pInFile == NULL)
	{
		printf("failed to open file %s\n", from);
		return -1;
	}
    pOutFile = fopen(to, "wb");
	if(pOutFile == NULL)
	{
		printf("failed to open file %s\n", to);
		fclose(pInFile);
		return -1;
	}

	while (1)
	{
		iRead = fread(ucInBuff, 1, 512, pInFile);
		if (iRead > 0)
		{
			adpcm_coder((short*)ucInBuff, (char*)ucOutBuff, (iRead/2), &state);
			printf("encode %d bytes\n", iRead);
			fwrite(ucOutBuff, 1, (iRead/4), pOutFile);
			memset(ucInBuff, 0, sizeof(ucInBuff));
			memset(ucOutBuff, 0, sizeof(ucOutBuff));
		}
		else
		{
			printf("read the end\n");
			break;
		}
	}
	
	fclose(pInFile);
	fclose(pOutFile);
	
}


int decode(const char* from, const char* to)
{
	FILE *pInFile, *pOutFile ;
	int iRead = 0;
	adpcm_state state;	
	unsigned char ucInBuff[64] = {0};
	unsigned char ucOutBuff[256] = {0};
	
	printf ("start decode from %s to %s.\n", from, to);
    pInFile = fopen(from, "rb");
	if(pInFile == NULL)
	{
		printf("failed to open file %s\n", from);
		return -1;
	}
    pOutFile = fopen(to, "wb");
	if(pOutFile == NULL)
	{
		printf("failed to open file %s\n", to);
		fclose(pInFile);
		return -1;
	}
	
	while (1)
	{
		iRead = fread(ucInBuff, 1, 64, pInFile);
		if (iRead > 0)
		{
			adpcm_decoder((char*)ucInBuff, (short*)ucOutBuff, (iRead*2), &state);
			printf("decode %d bytes\n", iRead);
			fwrite(ucOutBuff, 1, (iRead*4), pOutFile);
			memset(ucInBuff, 0, sizeof(ucInBuff));
			memset(ucOutBuff, 0, sizeof(ucOutBuff));
		}
		else
		{
			printf("read the end\n");
			break;
		}
	}
	
	fclose(pInFile);
	fclose(pOutFile);
	
	return 0;
	
}

int main (int argc, char ** argv)
{
	CALLBACK func;
	char *from, *to;
    char c;

	if(argc < 2)
	{
		printf ("Usage: %s [-e:pcm 2 dvi4][-d:dvi4 2 pcm][-i <inputpath>][-o <outputpath>].\n", argv[0]);
		return 1;
	}
	
    while (1) {
        c = getopt (argc, argv, "edi:o:");
        if (c == -1) {
            /* We have finished processing all the arguments. */
            break;
        }
        switch (c) {
        case 'e':
			func = encode;
            break;
        case 'd':
			func = decode;
            break;			
        case 'i':
			from = optarg;
            break;
        case 'o':
			to = optarg;
            break;			
        case '?':
        default:
			//break;
            printf ("Usage: %s [-e:pcm 2 dvi4][-d:dvi4 2 pcm][-i <inputpath>][-o <outputpath>].\n", argv[0]);
        }
    }
	
	func(from, to);

    return 0;
}

