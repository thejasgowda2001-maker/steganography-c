#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"


OperationType check_operation_type(char *argv[])
    {
        if(strcmp(argv[1], "-e") == 0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            return e_decode;
        }
        else{
            return e_unsupported;
        }
    }

int main(int argc, char **argv)
{
    if(check_operation_type(argv) == e_encode)
    {
        printf("--- Encoding Process Initiated ---\n");
        EncodeInfo encode;
        if(read_and_validate_encode_args(argv, &encode) == e_success)
        {
            printf("Arguments validated successfully\n");
            printf("---------Started Encoding--------\n");
            if(do_encoding(&encode) == e_success)
            {
                printf("----------------------------------\n");
                printf("|     Encoding is successful     |\n");
                printf("----------------------------------\n");
                printf("Output file : stego.bmp\n");
            }
            else
            {
                printf("Encoding failed\n");
                return -2;
            }
        }
        else
        {
            printf("Read and Validate Encoding Arguments has failed\n");
            return -1;
        }

    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("--- Decoding Process Initiated ---\n");
        DecodeInfo decode;
        if(read_and_validate_decode_args(argv, &decode) == e_success)
        {
            printf("Arguments validated successfully\n");
            printf("---------Started Decoding--------\n");
            if(do_decoding(&decode) == e_success)
            {
                printf("----------------------------------\n");
                printf("|     Decoding is successful     |\n");
                printf("---------------------------------\n");
                printf("Output file : decode.txt\n");
            }
            else
            {
                printf("Decoding failed\n");
                return -2;
            }
        }
        else
        {
            printf("Read and Validate Decoding Arguments has failed\n");
            return -1;
        }
    }
    
    else
    {
        printf("Invalid Input\n");
    }

    return 0;
}
