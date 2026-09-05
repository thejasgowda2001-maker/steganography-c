#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include <stdio.h>



typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *output_fname;
    FILE *fptr_output;

    char extn_secret_file[10];              // Example: ".txt"
    int size_secret_file;     // Size of the secret message
    char image_data[8];        // Buffer for reading LSBs
} DecodeInfo;

/* Function Prototypes */

/* Read and validaate decoding arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open decoding files */
Status open_decode_files(DecodeInfo *decInfo);

/* Performing decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/*Decode data from image */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

/* Decode a byte from LSB of the image */
Status decode_byte_from_lsb(char *image_buffer);

/* Decode four bytes to LSB of the image */
Status decode_int_from_lsb(DecodeInfo *decInfo, int *value);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo, int *extn_size);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
