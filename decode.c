#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(argv[1], "-d") != 0)
    {
        fprintf(stderr, "ERROR: Invalid decode option. Use -d\n");
        return e_failure;
    }

    if (argv[2] != NULL && strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        fprintf(stderr, "ERROR: Stego image file missing or not .bmp\n");
        return e_failure;
    }

    if (argv[3] != NULL && strlen(argv[3]) > 0)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "decoded.txt";
    }

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen - stego image");
        fprintf(stderr, "Unable to open stego image file\n");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen - output file");
        fprintf(stderr, "Unable to open output file\n");
        return e_failure;
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    //open stego image and output file
    if (open_decode_files(decInfo) == e_failure)
    {
        printf("ERROR: Failed to open required files\n");
        return e_failure;
    }
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);  // Skip BMP header

    if (decode_magic_string(decInfo) == e_success)
    {
        printf("[INFO]: Magic string decoded\n");
    }
    else
    {
        printf("[INFO]: Failed to decode magic string\n");
        return e_failure;
    }

    int extn_size;
    if (decode_secret_file_extn_size(decInfo, &extn_size) == e_success)
    {
        printf("[INFO]: Extension size decoded: %d\n", extn_size);
    }
    else
    {
        printf("[INFO]: Failed to decode extension size\n");
        return e_failure;
    }

    if (decode_secret_file_extn(decInfo, extn_size) == e_success)
    {
        printf("[INFO]: Extension decoded: %s\n", decInfo->extn_secret_file);
    }
    else
    {
        printf("[INFO]: Failed to decode extension\n");
        return e_failure;
    }

    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf("[INFO]: Secret file size: %d bytes\n", decInfo->size_secret_file);
    }
    else
    {
        printf("[INFO]: Failed to decode secret file size\n");
        return e_failure;
    }

    if (decode_secret_file_data(decInfo) == e_success)
    {
        printf("[INFO]: Secret file data decoded and written to %s\n", decInfo->output_fname);
    }
    else
    {
        printf("[INFO]: Failed to decode secret file data\n");
        return e_failure;
    }

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[strlen(MAGIC_STRING) + 1];
    decode_data_from_image(buffer, strlen(MAGIC_STRING), decInfo);
    buffer[strlen(MAGIC_STRING)] = '\0';

    if (strncmp(buffer, MAGIC_STRING, strlen(MAGIC_STRING)) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
}
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    if (decInfo == NULL || decInfo->fptr_stego_image == NULL)
    {
        printf("ERROR: decInfo or stego file is NULL\n");
        return e_failure;
    }

    for (int i = 0; i < size; i++)
    {
        fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
        data[i] = decode_byte_from_lsb(decInfo->image_data);
    }
    data[size] = '\0';
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo, int *extn_size)
{
    return decode_int_from_lsb(decInfo, extn_size);
}

Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size)
{
    if (extn_size >= sizeof(decInfo->extn_secret_file))
    {
        printf("ERROR: Extension size too large\n");
        return e_failure;
    }

    decode_data_from_image(decInfo->extn_secret_file, extn_size, decInfo);
    decInfo->extn_secret_file[extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    return decode_int_from_lsb(decInfo, &decInfo->size_secret_file);
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(decInfo->image_data);
        fputc(ch, decInfo->fptr_output);
    }
    return e_success;
}

Status decode_int_from_lsb(DecodeInfo *decInfo, int *value)
{
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    *value = 0;
    for (int i = 0; i < 32; i++)
    {
        *value |= (buffer[i] & 1) << i;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data |= (image_buffer[i] & 1) << i;
    }
    return data;
}
