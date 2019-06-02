#include <Arduino.h>

/* Untitled1 (21/05/2019 20:30:34)
   StartOffset(h): 00000000, EndOffset(h): 0000000A, Length(h): 0000000B */

const PROGMEM byte PAYLOADBIN[11] = {
  0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x2E, 0x62, 0x69, 0x6E
};

const PROGMEM byte HKSECTION_1 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x31, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_2 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x32, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_3 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x33, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_4 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x34, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_5 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x35, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_6 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x36, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_7 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x37, 0x2E, 0x62, 0x69, 0x6E};
const PROGMEM byte HKSECTION_8 [12] = {0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x38, 0x2E, 0x62, 0x69, 0x6E};

//send hksection

/* C:\Users\Kitchen\Documents\GitHub\Fusee-UF2\Fusee-UF2_src\output\Fusee-UF2.bin (21/05/2019 20:10:39)
   StartOffset(d): 00041148, EndOffset(d): 00041171, Length(d): 00000024 */

const PROGMEM byte INBETWEENPL [24] = {
  0x00, 0x00, 0x00, 0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64,
  0x65, 0x72, 0x2F, 0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x73, 0x2F
};
const PROGMEM byte INBETWEENPL2[21] = {
  0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F,
  0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x73, 0x2F
};

const PROGMEM byte UNDER_LINE1[7] = {
  0x00, 0x00, 0x00, 0x0A, 0x0A, 0x00, 0x00
};

const PROGMEM byte FSAFTERPL1[141] = {
  0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F,
  0x73, 0x77, 0x69, 0x74, 0x63, 0x68, 0x62, 0x6F, 0x6F, 0x74, 0x2E, 0x62,
  0x69, 0x6E, 0x00, 0x00, 0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61,
  0x64, 0x65, 0x72, 0x2F, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65, 0x2E, 0x62,
  0x69, 0x6E, 0x00, 0x00, 0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61,
  0x64, 0x65, 0x72, 0x00, 0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61,
  0x64, 0x65, 0x72, 0x2F, 0x66, 0x75, 0x73, 0x65, 0x65, 0x00, 0x00, 0x00,
  0x00, 0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F,
  0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x73, 0x00, 0x62, 0x6F, 0x6F,
  0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x66, 0x75, 0x73, 0x65,
  0x65, 0x2F, 0x73, 0x74, 0x72, 0x61, 0x70, 0x73, 0x5F, 0x69, 0x6E, 0x66,
  0x6F, 0x2E, 0x74, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00
};






//hk again

const PROGMEM byte FSINBETWEENPL2[4] = {0x0A, 0x0A, 0x00, 0x00};



//fs straps

const PROGMEM byte SWPART2[1164] = {
  0x00, 0x00, 0x00, 0x00, 0x0A, 0x68, 0x65, 0x6B, 0x61, 0x74, 0x65, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x28, 0x43, 0x29, 0x20, 0x32, 0x30, 0x31, 0x38,
  0x20, 0x6E, 0x61, 0x65, 0x68, 0x72, 0x77, 0x65, 0x72, 0x74, 0x2C, 0x20,
  0x73, 0x74, 0x34, 0x72, 0x6B, 0x0A, 0x0A, 0x43, 0x54, 0x43, 0x61, 0x65,
  0x72, 0x20, 0x6D, 0x6F, 0x64, 0x20, 0x28, 0x43, 0x29, 0x20, 0x32, 0x30,
  0x31, 0x39, 0x20, 0x43, 0x54, 0x43, 0x61, 0x65, 0x72, 0x0A, 0x4C, 0x6F,
  0x63, 0x6B, 0x70, 0x69, 0x63, 0x6B, 0x20, 0x28, 0x43, 0x29, 0x20, 0x32,
  0x30, 0x31, 0x39, 0x20, 0x53, 0x68, 0x63, 0x68, 0x6D, 0x75, 0x65, 0x0A,
  0x41, 0x72, 0x67, 0x6F, 0x6E, 0x2D, 0x4E, 0x58, 0x20, 0x28, 0x43, 0x29,
  0x20, 0x32, 0x30, 0x31, 0x39, 0x20, 0x47, 0x75, 0x69, 0x6C, 0x6C, 0x65,
  0x6D, 0x39, 0x36, 0x2C, 0x20, 0x4D, 0x72, 0x44, 0x75, 0x64, 0x65, 0x0A,
  0x49, 0x6E, 0x69, 0x74, 0x69, 0x61, 0x6C, 0x20, 0x53, 0x41, 0x4D, 0x44,
  0x2D, 0x46, 0x75, 0x73, 0x65, 0x65, 0x20, 0x69, 0x6D, 0x70, 0x6C, 0x65,
  0x6D, 0x65, 0x6E, 0x74, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x41, 0x74,
  0x6C, 0x61, 0x73, 0x34, 0x34, 0x0A, 0x53, 0x77, 0x69, 0x74, 0x63, 0x68,
  0x62, 0x6F, 0x6F, 0x74, 0x20, 0x62, 0x72, 0x6F, 0x75, 0x67, 0x68, 0x74,
  0x20, 0x74, 0x6F, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x62, 0x79, 0x20, 0x4D,
  0x61, 0x74, 0x74, 0x79, 0x74, 0x72, 0x6F, 0x67, 0x0A, 0x0A, 0x00, 0x00,
  0x73, 0x61, 0x66, 0x65, 0x00, 0x00, 0x00, 0x00, 0x62, 0x61, 0x63, 0x6B,
  0x75, 0x70, 0x00, 0x00, 0x42, 0x41, 0x43, 0x4B, 0x55, 0x50, 0x5F, 0x00,
  0x33, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00,
  0x73, 0x63, 0x72, 0x65, 0x65, 0x6E, 0x73, 0x68, 0x6F, 0x74, 0x00, 0x00,
  0x2F, 0x00, 0x00, 0x00, 0x4D, 0x61, 0x69, 0x6E, 0x20, 0x4D, 0x65, 0x6E,
  0x75, 0x00, 0x00, 0x00, 0x42, 0x72, 0x6F, 0x77, 0x73, 0x65, 0x20, 0x66,
  0x6F, 0x72, 0x20, 0x61, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x00,
  0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x69,
  0x6E, 0x69, 0x00, 0x00, 0x42, 0x61, 0x63, 0x6B, 0x00, 0x00, 0x00, 0x00,
  0x63, 0x6F, 0x6E, 0x66, 0x69, 0x67, 0x00, 0x00, 0x4C, 0x61, 0x75, 0x6E,
  0x63, 0x68, 0x20, 0x6C, 0x69, 0x6E, 0x75, 0x78, 0x20, 0x2F, 0x20, 0x69,
  0x6E, 0x69, 0x20, 0x65, 0x6E, 0x74, 0x72, 0x79, 0x00, 0x00, 0x00, 0x00,
  0x25, 0x6B, 0x4E, 0x6F, 0x20, 0x65, 0x78, 0x74, 0x72, 0x61, 0x20, 0x63,
  0x6F, 0x6E, 0x66, 0x69, 0x67, 0x73, 0x20, 0x66, 0x6F, 0x75, 0x6E, 0x64,
  0x2E, 0x25, 0x6B, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x25, 0x6B, 0x43, 0x6F,
  0x75, 0x6C, 0x64, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x66, 0x69, 0x6E, 0x64,
  0x20, 0x61, 0x6E, 0x79, 0x20, 0x69, 0x6E, 0x69, 0x0A, 0x69, 0x6E, 0x20,
  0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x69,
  0x6E, 0x69, 0x21, 0x25, 0x6B, 0x0A, 0x00, 0x00, 0x25, 0x6B, 0x46, 0x61,
  0x69, 0x6C, 0x65, 0x64, 0x20, 0x74, 0x6F, 0x20, 0x6C, 0x61, 0x75, 0x6E,
  0x63, 0x68, 0x20, 0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x2E, 0x25,
  0x6B, 0x0A, 0x00, 0x00, 0x25, 0x6B, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64,
  0x20, 0x74, 0x6F, 0x20, 0x6C, 0x61, 0x75, 0x6E, 0x63, 0x68, 0x20, 0x66,
  0x69, 0x72, 0x6D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x25, 0x6B, 0x0A, 0x00,
  0x62, 0x6F, 0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x73,
  0x79, 0x73, 0x2F, 0x6C, 0x69, 0x62, 0x73, 0x79, 0x73, 0x5F, 0x6C, 0x70,
  0x30, 0x2E, 0x62, 0x73, 0x6F, 0x00, 0x00, 0x00, 0x62, 0x6F, 0x6F, 0x74,
  0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x68, 0x65, 0x6B, 0x61, 0x74,
  0x65, 0x5F, 0x69, 0x70, 0x6C, 0x2E, 0x69, 0x6E, 0x69, 0x00, 0x00, 0x00,
  0x4E, 0x6F, 0x20, 0x6D, 0x61, 0x69, 0x6E, 0x20, 0x63, 0x6F, 0x6E, 0x66,
  0x69, 0x67, 0x73, 0x20, 0x66, 0x6F, 0x75, 0x6E, 0x64, 0x2E, 0x2E, 0x2E,
  0x00, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x75, 0x6E, 0x63, 0x68, 0x20, 0x63,
  0x6F, 0x6E, 0x66, 0x69, 0x67, 0x75, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E,
  0x73, 0x00, 0x00, 0x00, 0x25, 0x6B, 0x43, 0x6F, 0x75, 0x6C, 0x64, 0x20,
  0x6E, 0x6F, 0x74, 0x20, 0x6F, 0x70, 0x65, 0x6E, 0x20, 0x27, 0x62, 0x6F,
  0x6F, 0x74, 0x6C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2F, 0x68, 0x65, 0x6B,
  0x61, 0x74, 0x65, 0x5F, 0x69, 0x70, 0x6C, 0x2E, 0x69, 0x6E, 0x69, 0x27,
  0x2E, 0x0A, 0x4D, 0x61, 0x6B, 0x65, 0x20, 0x73, 0x75, 0x72, 0x65, 0x20,
  0x69, 0x74, 0x20, 0x65, 0x78, 0x69, 0x73, 0x74, 0x73, 0x21, 0x25, 0x6B,
  0x0A, 0x00, 0x00, 0x00, 0x0A, 0x50, 0x72, 0x65, 0x73, 0x73, 0x20, 0x50,
  0x4F, 0x57, 0x45, 0x52, 0x20, 0x74, 0x6F, 0x20, 0x61, 0x74, 0x74, 0x65,
  0x6D, 0x70, 0x74, 0x20, 0x62, 0x6F, 0x6F, 0x74, 0x2E, 0x0A, 0x0A, 0x50,
  0x72, 0x65, 0x73, 0x73, 0x20, 0x56, 0x4F, 0x4C, 0x2D, 0x20, 0x74, 0x6F,
  0x20, 0x67, 0x6F, 0x20, 0x74, 0x6F, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6D,
  0x65, 0x6E, 0x75, 0x2E, 0x0A, 0x0A, 0x50, 0x72, 0x65, 0x73, 0x73, 0x20,
  0x56, 0x4F, 0x4C, 0x2B, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x72, 0x65, 0x61,
  0x74, 0x65, 0x20, 0x6E, 0x65, 0x77, 0x20, 0x69, 0x6E, 0x69, 0x0A, 0x00,
  0x0A, 0x55, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x64, 0x65, 0x66, 0x61, 0x75,
  0x6C, 0x74, 0x20, 0x6C, 0x61, 0x75, 0x6E, 0x63, 0x68, 0x20, 0x63, 0x6F,
  0x6E, 0x66, 0x69, 0x67, 0x75, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x2E,
  0x2E, 0x2E, 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x50, 0x6C, 0x65,
  0x61, 0x73, 0x65, 0x20, 0x77, 0x61, 0x69, 0x74, 0x2E, 0x2E, 0x2E, 0x20,
  0x4D, 0x61, 0x6B, 0x69, 0x6E, 0x67, 0x20, 0x6E, 0x65, 0x77, 0x20, 0x49,
  0x4E, 0x49, 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x2E, 0x69, 0x6E, 0x69,
  0x00, 0x00, 0x00, 0x00, 0x25, 0x6B, 0x50, 0x61, 0x79, 0x6C, 0x6F, 0x61,
  0x64, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x20, 0x69, 0x73, 0x20, 0x6D, 0x69,
  0x73, 0x73, 0x69, 0x6E, 0x67, 0x21, 0x0A, 0x28, 0x25, 0x73, 0x29, 0x25,
  0x6B, 0x0A, 0x00, 0x00, 0x25, 0x6B, 0x0A, 0x20, 0x4E, 0x6F, 0x20, 0x2E,
  0x62, 0x69, 0x6E, 0x20, 0x6F, 0x72, 0x20, 0x2E, 0x69, 0x6E, 0x69, 0x20,
  0x66, 0x69, 0x6C, 0x65, 0x73, 0x20, 0x68, 0x65, 0x72, 0x65, 0x21, 0x20,
  0x50, 0x72, 0x65, 0x73, 0x73, 0x20, 0x61, 0x20, 0x6B, 0x65, 0x79, 0x2E,
  0x25, 0x6B, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x63,
  0x72, 0x65, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x63, 0x6F, 0x6E, 0x66,
  0x69, 0x67, 0x20, 0x65, 0x6E, 0x74, 0x72, 0x79, 0x2E, 0x0A, 0x0A, 0x50,
  0x72, 0x65, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79,
  0x2E, 0x00, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6C, 0x65, 0x64, 0x20, 0x6F,
  0x70, 0x65, 0x6E, 0x69, 0x6E, 0x67, 0x20, 0x68, 0x65, 0x6B, 0x61, 0x74,
  0x65, 0x5F, 0x69, 0x70, 0x6C, 0x2E, 0x69, 0x6E, 0x69, 0x0A, 0x0A, 0x50,
  0x72, 0x65, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79,
  0x2E, 0x00, 0x00, 0x00, 0x5B, 0x00, 0x00, 0x00, 0x5D, 0x0A, 0x00, 0x00,
  0x70, 0x61, 0x79, 0x6C, 0x6F, 0x61, 0x64, 0x3D, 0x00, 0x00, 0x00, 0x00,
  0x41, 0x64, 0x64, 0x65, 0x64, 0x3A, 0x0A, 0x0A, 0x25, 0x73, 0x0A, 0x0A,
  0x74, 0x6F, 0x20, 0x68, 0x65, 0x6B, 0x61, 0x74, 0x65, 0x5F, 0x69, 0x70,
  0x6C, 0x2E, 0x69, 0x6E, 0x69, 0x0A, 0x0A, 0x50, 0x72, 0x65, 0x73, 0x73,
  0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79, 0x2E, 0x00, 0x00, 0x00,
  0x5B, 0x53, 0x74, 0x6F, 0x63, 0x6B, 0x5D, 0x0A, 0x00, 0x00, 0x00, 0x00,
  0x41, 0x64, 0x64, 0x65, 0x64, 0x20, 0x73, 0x74, 0x6F, 0x63, 0x6B, 0x20,
  0x65, 0x6E, 0x74, 0x72, 0x79, 0x2E, 0x20, 0x50, 0x72, 0x65, 0x73, 0x73,
  0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79, 0x2E, 0x00, 0x00, 0x00
};