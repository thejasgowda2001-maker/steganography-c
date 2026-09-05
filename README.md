# LSB Image Steganography

A C-based project that hides and retrieves secret text inside BMP images using the Least Significant Bit (LSB) technique.

## Features
- Encode secret text into a BMP image
- Decode hidden data from the stego image
- Preserve the BMP image structure
- File extension and secret data handling

## Technologies Used
- C Programming
- File Handling
- Bitwise Operations
- LSB Steganography

## Output
The project successfully encodes the secret file into a BMP image and generates stego.bmp.
gcc *.c
./a.out -e beautiful.bmp secret.txt stego_img.bmp
