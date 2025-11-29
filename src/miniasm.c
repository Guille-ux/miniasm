#include "../include/miniasm.h"
#include "../include/compat.h"
#include "../include/stream.h"
#include "../include/utils.h"
#include "../include/data_asm.h"
#include "../include/directives.h"

void skipUntilThese(const char *text, int *pos, const char *list) {
	while (1) {
		for (char *ch=list;ch!='\0';*ch++) {
			if (*ch==text[pos]) {
				return;
			}
		}

		if (text[pos]=='\0') {
			break;
		}

		*pos++;
	}
}

void skipThese(const char *text, int *pos, const char *list) {
	bool found=false;

	while (1) {
		found=false;
		for (char *ch=list;ch!='\0';*ch++) {
			if (*ch==text[pos]) {
				found=true;
				break;
			}
		}
		if (found==false) {
			break;
		}
		if (text[pos]=='\0') {
			break;
		}
		*pos++;
	}	
}

bool assemble_text(ByteStream *outStream, const char *text) {
	if (outStream == NULL) return false;

	int pos=0;

	/*
	 *
	 * Aqui iran cosas importantes, pero de momento no
	 *
	 */

	if(initStream(outStream)!=0) return false;

	directive cdir;

	while (1) {
		if (text[pos]=='\n') {
			pos++;
			continue;
		}
		if (text[pos]=='\0') {
			break;
		}

		cdir = WtfDirectiveIs(&text[pos], &pos);

		switch (cdir) {
			case DIRECTIVE_DB:
			case DIRECTIVE_DW:
			case DIRECTIVE_DD:
				handle_data_directive(outStream, cdir, &text[pos], &pos);
				break;
			case DIRECTIVE_MOVI:
				if (text[pos]=='M' || text[pos]=='N' || text[pos]=='L') {
					char arch;
					uint8_t opcode;
					char ch=text[pos++];
	
					uint8_t a_reg_code = assemble_reg(&text[pos], &pos, &arch);					

					uint8_t modrm = 0;
					bool offset32=false;
					bool offset8=false;

					if (arch==8) {
						opcode = 0x88;
					} else if (arch=16) {
						streamAppendByte(outStream, 0x66);
						opcode = 0x89;
					} else {
						opcode = 0x89;
					}	

					if (ch=='M') {
						// nothing
					} else if (ch=='N') {
						skipThese(&text[pos], &pos, " \t");
						modrm = 1 << 6;
						offset8=true;
					} else if (ch=='L') {
						skipThese(&text[pos], &pos, " \t");
						modrm = 2 << 6;
						offset32=true;
					}
					if (offset8 || offset32) {
						skipThese(&text[pos], &pos, " \t");	
						if (&text[pos++]!='+') break; // Error 
						skipThese(&text[pos], &pos, " \t");
						size_t n=getNum(&text[pos], &pos);	
					}

					


					b_reg_code = assemble_reg(&text[pos], &pos, &arch);
					modrm |= b_reg_code << 3;
					modrm |= a_reg_code;

					if (offset8) {
						streamAppendByte(outStream, n & 0xFF);
					} else if (offset32) {
						for (int i=0;i<4;i++) {
							streamAppendByte(outStream, (n >> i*8) & 0xFF);
						}
					}
				} else {
					// is to a reg
					char arch;
					uint8_t opcode;
					uint8_t a_reg_code = assemble_reg(&text[pos], &pos, &arch);
					if (text[pos++]!=',') {
						break;
						// Error!
					}

					skipThese(text, &pos, " \t");

					if (text[pos]=='0') {
						size_t number=getNum(&text[pos]);
						if (arch==8) {
							opcode=0xB0+a_reg_code;
							streamAppendByte(outStream, opcode);
							streamAppendByte(outStream, number & 0xFF);
						} else if (arch==16) {
							streamAppendByte(outStream, 0x66);
							opcode=0xB8+a_reg_code;
							streamAppendByte(outStream, opcode);
							for (int i=0;i<2;i++) {
								streamAppendByte(outStream, (number >> (i*8)) & 0xFF);
							}
						} else {
							opcode=0xB8+a_reg_code;
							streamAppendByte(outStream, opcode);
							for (int i=0;i<4;i++) {
								streamAppendByte(outStream, (number >> (i*8)) & 0xFF);
							}	
						}
						
					} else {
									
						if (arch==8) {
							opcode = 0x88;
						} else if (arch==16){
							streamAppendByte(outStream, 0x66);
							opcode = 0x89;
						} else {
							opcode = 0x89;
						}

						uint8_t modrm=0;

						bool offset8 = false;
						bool offset32 = false;

						if (text[pos]=='M') {
							opcode += 2;
							pos++;
						} else if (text[pos]=='N') {
							pos++;
							opcode += 2;
							offset8 = true;
							modrm = 1 << 6;
						} else if (text[pos]=='L') {
							pos++;
							opcode += 2;
							offset32=true;
							modrm = 2 << 6;
						} else {
							modrm = 3 << 6;	
						}
						streamAppenByte(outStream, opcode);

						
						modrm |= a_reg_code << 3;
						uint8_t b_reg_code = assemble_reg(text[pos], &pos, &arch);
						modrm |= b_reg_code;

						streamAppendByte(outStream, modrm);
						if (offset8 || offset32) {
							skipThese(&text[pos], &pos, " \t");
							if (text[pos]!='+') break; // Error
							skipThese(&text[pos], &pos, " \t");
							size_t n = getNum(&text[pos], &pos);
						}

						if (offset8) {
							streamAppendByte(outStream, n & 0xFF);
						} else if (offset32) {
							for (int i=0;i<4;i++) {
								streamAppendByte(outStream, (n >> i*8) & 0xFF);
							}
						}
					}
				}
				break;
			case DIRECTIVE_LABEL:
				break;
			case DIRECTIVE_TIMES:	
				break;
			case DIRECTIVE_TIMES_SIZE:
				break;
			default: break;
		}

		pos++;
	}

	return true;
}

uint8_t assemble_reg(const char *text, int *pos, char *arch) {
	uint8_t ret=100;
	if (isPattern(text, "eax")) {
		ret=0;
	} else if (isPattern(text, "ecx")) {
		ret=1;
	} else if (isPattern(text, "edx")) {
		ret=2;
	} else if (isPattern(text, "ebx")) {
		ret=3;
	} else if (isPattern(text, "esp")) {
		ret=4;
	} else if (isPattern(text, "ebp")) {
		ret=5;
	} else if (isPattern(text, "esi")) {
		ret=6;
	} else if (isPattern(text, "edi")) {
		ret=7;
	}

	if (ret<100) {
		*pos+=3;
		*arch=32;
		return ret;
	}

	if (isPattern(text, "ax")) {
		ret=0;
	} else if (isPattern(text, "bx")) {
		ret=1;
	} else if (isPattern(text, "dx")) {
		ret=2;
	} else if (isPattern(text, "cx")) {
		ret=3;
	} else if (isPattern(text, "sp")) {
		ret=4;
	} else if (isPattern(text, "bp")) {
		ret=5;
	} else if (isPattern(text, "di")) {
		ret=6;
	} else if (isPattern(text, "si")) {
		ret=7;
	}

	if (ret<100) {
		*pos+=2;
		*arch=16;
		return ret;
	}

	if (isPattern(text, "al")) {
		ret=0;
	} else if (isPattern(text, "cl")) {
		ret=1;
	} else if (isPattern(text, "dl")) {
		ret=2;
	} else if (isPattern(text, "bl")) {
		ret=3;
	} else if (isPattern(text, "ah")) {
		ret=4;
	} else if (isPattern(text, "ch")) {
		ret=5;
	} else if (isPattern(text, "dh")) {
		ret=6;
	} else if (isPattern(text, "bh")) {
		ret=7;
	}

	if (ret<100) {
		*pos+=2;
		*arch=8;
		return ret;
	}
}
