// pai-back w10xh15
const unsigned char img_back[] PROGMEM = {
	0xfe, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
	0x01, 0xfe, 0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 
	0x40, 0x40, 0x40, 0x3f, 
};

// pai w8xh11
const unsigned char img_manzi[] PROGMEM = {
// 萬子
	0x00, 0x44, 0xc4, 0x44, 0x44, 0x44, 0x44, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x50, 0xd2, 0x52, 0x52, 0x52, 0x50, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x50, 0xd5, 0x55, 0x55, 0x55, 0x50, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 	
	0x00, 0x5f, 0xd5, 0x57, 0x57, 0x55, 0x5f, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x51, 0xd5, 0x5f, 0x55, 0x5d, 0x51, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 	
	0x00, 0x52, 0xca, 0x43, 0x42, 0x4a, 0x52, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x44, 0xc4, 0x4f, 0x52, 0x52, 0x52, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x58, 0xc7, 0x40, 0x40, 0x47, 0x58, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 
	0x00, 0x42, 0xda, 0x47, 0x42, 0x5e, 0x50, 0x00, 
	0x00, 0x00, 0x07, 0x01, 0x01, 0x01, 0x07, 0x00, 	

// 筒子
	0x00, 0x70, 0xa8, 0x54, 0x54, 0xa8, 0x70, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x8c, 0xde, 0xde, 0x8c, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 
	0x00, 0x06, 0x06, 0x30, 0x30, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 
	0x00, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00, 
	0x00, 0x03, 0x03, 0x00, 0x00, 0x03, 0x03, 0x00, 
	0x00, 0x06, 0x06, 0x30, 0x30, 0x06, 0x06, 0x00, 
	0x00, 0x03, 0x03, 0x00, 0x00, 0x03, 0x03, 0x00, 
	0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 
	0x00, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00, 
	0x00, 0xc3, 0xc3, 0x04, 0x04, 0xd8, 0xd8, 0x00, 
	0x00, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00, 
	0x00, 0xdb, 0xdb, 0x00, 0x00, 0xdb, 0xdb, 0x00, 
	0x00, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00, 
	0x66, 0x66, 0x00, 0x66, 0x66, 0x00, 0x66, 0x66, 
	0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 

// 索子
	0x08, 0xec, 0x57, 0xfc, 0x7a, 0xd9, 0x64, 0x82, 
	0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xde, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0xc0, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
	0x00, 0xde, 0x00, 0x00, 0x00, 0x00, 0xde, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
	0x00, 0xde, 0x00, 0x78, 0x00, 0x00, 0xde, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
	0x00, 0xde, 0x00, 0xde, 0x00, 0x00, 0xde, 0x00, 
	0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 
	0x00, 0x70, 0x00, 0x77, 0x00, 0x00, 0x70, 0x00, 
	0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x07, 0x00, 
	0x00, 0xde, 0x04, 0x88, 0x88, 0x04, 0xde, 0x00, 
	0x00, 0x03, 0x01, 0x00, 0x00, 0x01, 0x03, 0x00, 
	0x00, 0x77, 0x00, 0x77, 0x00, 0x00, 0x77, 0x00, 
	0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x07, 0x00, 


// 風牌
	0x00, 0x3a, 0xaa, 0xff, 0xaa, 0x3a, 0x00, 0x00, 
	0x02, 0x01, 0x00, 0x03, 0x00, 0x01, 0x02, 0x00, 
	0x00, 0xf4, 0x74, 0xde, 0x74, 0x54, 0xf0, 0x00, 
	0x00, 0x03, 0x01, 0x03, 0x01, 0x01, 0x03, 0x00, 
	0x00, 0xf4, 0x7c, 0x14, 0x74, 0x5c, 0xf4, 0x00, 
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 
	0x10, 0x10, 0xfc, 0x80, 0xfc, 0x10, 0x88, 0x00, 
	0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 

// 三元牌
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x20, 0x90, 0xea, 0xa4, 0xa4, 0xea, 0xb4, 0x20, 
	0x00, 0x02, 0x01, 0x00, 0x00, 0x03, 0x02, 0x00, 
	0x00, 0x1c, 0x24, 0xfe, 0x24, 0x34, 0x0c, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 

};
