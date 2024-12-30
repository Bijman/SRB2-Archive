#ifndef __psp_header__
#define __psp_header__

static unsigned int size_psp_header = 336;
static unsigned char psp_header[] __attribute__((aligned(16))) = {
	0x7e, 0x50, 0x53, 0x50, 0x00, 0x02, 0x00, 0x00, 0x01, 0x01, 0x22, 0x74, 0x69, 0x66, 0x70, 0x73,
	0x70, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x33, 0x55, 0x00, 0x50, 0x34, 0x55, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x67, 0x3d, 0x00, 0x50, 0x55, 0x0a, 0x01, 0x10, 0x00, 0x40, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x6b, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x4c, 0x6b, 0x3d, 0x00, 0xcc, 0xbb, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
	0x90, 0x82, 0x4c, 0x48, 0xa3, 0x53, 0xb2, 0x1b, 0x13, 0x95, 0x2f, 0xf1, 0x0b, 0x90, 0x9c, 0x11,
	0x61, 0x40, 0x20, 0x67, 0xf8, 0xdb, 0xfc, 0x95, 0x5c, 0xbe, 0x8c, 0x80, 0xf3, 0x92, 0x03, 0x01,
	0xb0, 0xbe, 0xf5, 0xf8, 0xa1, 0xaf, 0xaf, 0xa8, 0x38, 0x26, 0x63, 0x09, 0x26, 0x0e, 0xb7, 0xd5,
	0x00, 0x33, 0x55, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x5c, 0x3e, 0x03, 0x22, 0xe5, 0x7d, 0xb9, 0xd1, 0x13, 0x67, 0x97, 0xa3, 0x5b, 0xd8, 0x77, 0x1f,
	0xf0, 0x05, 0xf3, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x4a, 0xd7, 0x37,
	0xc2, 0x8f, 0x15, 0x43, 0x33, 0x93, 0x4d, 0x5b, 0xc0, 0x6e, 0xe4, 0x00, 0xc6, 0x0a, 0x71, 0x11,
	0x98, 0xb6, 0xc3, 0xb7, 0x59, 0x66, 0x21, 0xa8, 0x65, 0xf6, 0x53, 0xa9, 0x7a, 0x48, 0x17, 0xb6,
};

#endif
