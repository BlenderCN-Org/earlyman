
///**** CUSTOM BMP READER *****////
static int ReadBMP (std::string image_path, GLsizei& width, GLsizei& height, unsigned char * bmp_pixels)
{
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int bmp_width, bmp_height, bits_per_pixel;
  unsigned int imageSize;   // = width*height*3
  // Actual RGB data
  unsigned char * data;

  FILE * file = fopen (image_path . c_str (),"rb");
  if (!file){printf("Image could not be opened\n"); return 0;}

  if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return 1;
  }
  if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
    return 1;
  }
  
  // Read ints from the byte array
  dataPos    = *(int*)&(header[0x0A]);
  imageSize  = *(int*)&(header[0x22]);
  bmp_width      = *(int*)&(header[0x12]);
  bmp_height     = *(int*)&(header[0x16]);
  bits_per_pixel = *(int*)&(header[0x1C]);

  printf ("imageSize: %d\n", imageSize);
  printf ("bits per pixel: %d\n", bits_per_pixel);
  printf ("data pos: %d\n", dataPos);

  // Some BMP files are misformatted, guess missing information
  if (imageSize==0)    imageSize=bmp_width*bmp_height*3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos==0)      dataPos=54; // The BMP header is done that way
  // Create a buffer
  data = new unsigned char [imageSize];

  // since the header is (probably) only 54 bytes, but the data may not start immediately
  // we need to skip the difference
  fseek (file, dataPos - 54, SEEK_CUR);
  // Read the actual data from the file into the buffer
  fread (data, 1, imageSize, file);
  //Everything is in memory now, the file can be closed
  fclose (file);

  // flip the RGB to m to BGR
  /*
  int i;
  for(i = 0; i < imageSize; i += 3)
  {
    unsigned char tmp = data[i];
    data[i] = data[i+2];
    data[i+2] = tmp;
  }
  */
  ///**** CUSTOM BMP READER *****////

  width = bmp_width;
  height = bmp_height;
  memcpy (bmp_pixels, data, 32 * 32 * 3);
  
  return 0;
}
