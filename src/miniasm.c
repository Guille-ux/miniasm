#include "../include/miniasm.h"
#include "../include/compat.h"
#include "../include/stream.h"
#include "../include/utils.h"
#include "../include/data_asm.h"
#include "../include/directives.h"
#include "../include/link.h"


void skipUntilThese(char *text, size_t *pos, const char *list) {
	while (1) {
		for (char *ch=list;ch!='\0';*ch++) {
			if (*ch==text[*pos]) {
				return;
			}
		}

		if (text[*pos]=='\0') {
			break;
		}

		*pos++;
	}
}

void skipThese(char *text, size_t *pos, const char *list) {
	bool found=false;

	while (1) {
		found=false;
		for (char *ch=list;ch!='\0';*ch++) {
			if (*ch==text[*pos]) {
				found=true;
				break;
			}
		}
		if (found==false) {
			break;
		}
		if (text[*pos]=='\0') {
			break;
		}
		*pos++;
	}	
}

bool assemble_text(ByteStream *outStream, const char *otext, ByteStream *linking) {
	if (outStream == NULL) return false;

	size_t pos=0;

	char *text = otext;

	/*
	 *
	 * Aqui iran cosas importantes, pero de momento no
	 *
	 */

	if(initStream(outStream)!=0) return false;


	directive cdir;
	size_t rel_cap=128;
	size_t rel_count=0;
	RelocationSymbol *rel=(RelocationSymbol*)malloc(sizeof(RelocationSymbol)*rel_cap);

	while (1) {
		if (rel_count >= rel_cap) {
			rel_cap *= 2;
			RelocationSymbol *new_rel=(RelocationSymbol*)malloc(sizeof(RelocationSymbol)*rel_cap);
			memcpy(new_rel, rel, rel_count);
			free(rel);
			rel=new_rel;
		}
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
				size_t n;
				if (text[pos]=='M' || text[pos]=='N' || text[pos]=='L') {
					char arch;
					uint8_t opcode;
					char ch=text[pos++];
	
					uint8_t a_reg_code = assemble_reg(&text[pos], &pos, &arch);					
					uint8_t b_reg_code;
					uint8_t modrm = 0;
					bool offset32=false;
					bool offset8=false;

					if (arch==8) {
						opcode = 0x88;
					} else if (arch!=DEFAULT_ARCH) {
						streamAppendByte(outStream, 0x66);
						opcode = 0x89;
					} else {
						opcode = 0x89;
					}	

					if (ch=='M') {
						// nothing
					} else if (ch=='N') {
						skipThese(text, &pos, " \t");
						modrm = 1 << 6;
						offset8=true;
					} else if (ch=='L') {
						skipThese(text, &pos, " \t");
						modrm = 2 << 6;
						offset32=true;
					}
					
					if (offset8 || offset32) {
						skipThese(text, &pos, " \t");	
						if (text[pos++]!='+') break; // Error 
						skipThese(text, &pos, " \t");
						n=getNum(&text[pos], &pos);	
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
						size_t number=getNum(&text[pos], &pos);
						if (arch==8) {
							opcode=0xB0+a_reg_code;
							streamAppendByte(outStream, opcode);
							streamAppendByte(outStream, number & 0xFF);
						} else if (arch!=DEFAULT_ARCH) {
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
						} else if (arch!=DEFAULT_ARCH){
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
						streamAppendByte(outStream, opcode);

						
						modrm |= a_reg_code << 3;
						uint8_t b_reg_code = assemble_reg(&text[pos], &pos, &arch);
						modrm |= b_reg_code;

						streamAppendByte(outStream, modrm);
						if (offset8 || offset32) {
							skipThese(text, &pos, " \t");
							if (text[pos]!='+') break; // Error
							skipThese(text, &pos, " \t");
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
				for (int i=0;i<rel_count;i++) {
					size_t start_pos=pos;
					skipUntilThese(text, &pos, ":");
					
					if (isPattern(&text[start_pos], rel[i].name)) {
						rel[i].direction = outStream->size;
						rel[i].ready = true;
						break;
					}
				}
				break;
			case DIRECTIVE_LABELDEF:
				RelocationSymbol *crel = &rel[rel_count++];
				size_t start_pos = pos;
				skipUntilThese(text, &pos, ":");
				char tmp = text[pos];
				text[pos]='\0';
				
				crel->name = strdup(&text[start_pos]);
				text[pos]=tmp;
				crel->id=rel_count-1;
				crel->direction=0;
				crel->ready=false;
				
				break;
			case DIRECTIVE_ADD:
				modrm_placeholder(0x01, outStream, text, &pos);
				break;
			case DIRECTIVE_ADC:
				modrm_placeholder(0x11, outStream, text, &pos);	
				break;
			case DIRECTIVE_SUB:
				modrm_placeholder(0x29, outStream, text, &pos);
				break;
			case DIRECTIVE_OR:
				modrm_placeholder(0x09, outStream, text, &pos);
				break;
			case DIRECTIVE_AND:
				modrm_placeholder(0x21, outStream, text, &pos);
				break;
			case DIRECTIVE_SBB:
				modrm_placeholder(0x19, outStream, text, &pos);
				break;
			case DIRECTIVE_XOR:
				modrm_placeholder(0x31, outStream, text, &pos);
				break;
			case DIRECTIVE_CMP:
				modrm_placeholder(0x39, outStream, text, &pos);
				break;
			case DIRECTIVE_TIMES:	
				break;
			case DIRECTIVE_TIMES_SIZE:
				break;
			case DIRECTIVE_MUL:
				i_placeholder(4, outStream, text, &pos);
				break;
			case DIRECTIVE_IMUL:
				i_placeholder(5, outStream, text, &pos);
				break;
			case DIRECTIVE_DIV:
				i_placeholder(6, outStream, text, &pos);
				break;
			case DIRECTIVE_IDIV:
				i_placeholder(7, outStream, text, &pos);
				break;
			case DIRECTIVE_SHL:
				shift_placeholder(4, 0xC1, outStream, text, &pos);
				break;
			case DIRECTIVE_SHR:
				shift_placeholder(5, 0xC1, outStream, text, &pos);
				break;
			case DIRECTIVE_SAR:
				shift_placeholder(7, 0xC1, outStream, text, &pos);
				break;
			case DIRECTIVE_ROL:
				shift_placeholder(0, 0xC1, outStream, text, &pos);
				break;
			case DIRECTIVE_ROR:
				shift_placeholder(1, 0xC1, outStream, text, &pos);
				break;
			case DIRECTIVE_NOT:
				i_placeholder(2, outStream, text, &pos);
				break;
			case DIRECTIVE_NEG:
				i_placeholder(3, outStream, text, &pos);
				break;
			case DIRECTIVE_STI:
				streamAppendByte(outStream, 0xFB);
				break;
			case DIRECTIVE_CLI:
				streamAppendByte(outStream, 0xFA);
				break;
			case DIRECTIVE_HLT:
				streamAppendByte(outStream, 0xF4);
				break;
			case DIRECTIVE_INT:
				size_t val = getNum(&text[pos], &pos);
				streamAppendByte(outStream, 0xCD);
				streamAppendByte(outStream, 0xFF & val);
				break;
			case DIRECTIVE_PUSH:
				char arch;
				uint8_t reg = assemble_reg(&text[pos], &pos, &arch);
				if (DEFAULT_ARCH!=arch) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0x50+reg);
				break;
			case DIRECTIVE_POP:
				char arch;
				uint8_t reg = assemble_reg(&text[pos], &pos, &arch);
				if (DEFAULT_ARCH!=arch) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0x58+reg);
				break;
			case DIRECTIVE_LOAX6:
				if (DEFAULT_ARCH!=16) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xB8+4);
				size_t start = pos;
				while (text[pos++]!=':');
				newRelocationRequest(linking, outStream->size, 2, &text[start], start-pos);
				streamAppendByte(outStream, 0x00);
				streamAppendByte(outStream, 0x00);
				break;
			case DIRECTIVE_LOAX:
				if (DEFAULT_ARCH!=32) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xB8+4);
				size_t start = pos;
				while (text[pos++]!=':');
				newRelocationRequest(linking, outStream->size, 4, &text[start], start-pos);
				for (int i=0;i<4;i++) {
					streamAppendByte(outStream, 0x00);
				}
				break;
			case DIRECTIVE_LOAX8:
				streamAppendByte(outStream, 0xB0+4);
				size_t start = pos;
				while (text[pos++]!=':');
				newRelocationRequest(linking, outStream->size, 1, &text[start], start-pos);
				streamAppendByte(outStream, 0x00);
				break;
			case DIRECTIVE_POPA:
				streamAppendByte(outStream, 0x61);
				break;
			case DIRECTIVE_PUSHA:
				streamAppendByte(outStream, 0x60);
				break;
			case DIRECTIVE_RET:
				streamAppendByte(outStream, 0xC3);
				break;
			case DIRECTIVE_IRET:
				streamAppendByte(outStream, 0xCF);
				break;
			case DIRECTIVE_CALL:
				char arch;
				uint8_t reg = assemble_reg(&text[pos], &pos, &arch);
				if (arch!= DEFAULT_ARCH) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xFF);
				uint8_t modrm = 3 << 6 | 2 << 3 | reg;
				streamAppendByte(outStream, modrm);
				break;
			case DIRECTIVE_JMP:
				char arch;
				uint8_t reg = assemble_reg(&text[pos], &pos, &arch);
				if (arch!= DEFAULT_ARCH) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xFF);
				uint8_t modrm = 3 << 6 | 4 << 3 | reg;
				streamAppendByte(outStream, modrm);
				break;
			case DIRECTIVE_RCALL:
				if (DEFAULT_ARCH==16) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xE8);
				size_t n = getNum(&text[pos], &pos);
				for (int i=0;i<4;i++) {
					streamAppendByte(outStream, (n >> i*8) & 0xFF);
				}	
				break;
			case DIRECTIVE_RJMP:
				if (DEFAULT_ARCH==16) {
					streamAppendByte(outStream, 0x66);
				}
				streamAppendByte(outStream, 0xE9);
				size_t n = getNum(&text[pos], &pos);
				for (int i=0;i<4;i++) {
					streamAppendByte(outStream, (n >> i*8) & 0xFF);
				}
				break;
			case DIRECTIVE_CMOVZ:
				break;
			case DIRECTIVE_CMOVNZ:
				break;
			case DIRECTIVE_CMOVC:
				break;
			default: break;
		}

		skipThese(text, &pos, " \n\t");
	}

	for (int i=0;i<rel_count;i++) {
		free(rel[i].name);
	}
	free(rel);

	Symbol endSym;
	endSym.type = SYM_END;
	streamAppendByte(linking, (uint8_t*)&endSym, sizeof(Symbol));

	return true;
}

uint8_t assemble_reg(char *text, size_t *pos, char *arch) {
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

void modrm_placeholder(uint8_t opcode, ByteStream *stream, char *text, size_t *pos) {
	
	char arch;
	uint8_t a_reg_code = assemble_reg(&text[*pos], pos, &arch);
	if (arch==8) {
		opcode--;
	} else if (arch!=DEFAULT_ARCH) {
		streamAppendByte(stream, 0x66);
	}
	streamAppendByte(stream, opcode);	
	*pos++;
	skipThese(text, pos, " \t");
	uint8_t b_reg_code = assemble_reg(&text[*pos], pos, &arch);
	uint8_t modrm = 3 << 6;
	modrm |= a_reg_code << 3;
	modrm |= b_reg_code;
	streamAppendByte(stream, modrm);
}

void i_placeholder(uint8_t extender, ByteStream *stream, char *text, size_t *pos) {
	char arch;
	uint8_t opcode = 0xF7;
	uint8_t a_reg_code = assemble_reg(&text[*pos], pos, &arch);
	if (arch==8) {
		opcode--;
	} else if (arch!=DEFAULT_ARCH) {
		streamAppendByte(stream, 0x66);
	}
	streamAppendByte(stream, opcode);	
	uint8_t modrm = 3 << 6;
	modrm |= extender << 3;
	modrm |= a_reg_code;
	streamAppendByte(stream, modrm);		
}

void shift_placeholder(uint8_t extender, uint8_t opcode, ByteStream *stream, char *text, size_t *pos) {
	
	char arch;
	uint8_t a_reg_code = assemble_reg(&text[*pos], pos, &arch);
	if (arch==8) {
		opcode--;
	} else if (arch!=DEFAULT_ARCH) {
		streamAppendByte(stream, 0x66);
	}
	streamAppendByte(stream, opcode);	
	*pos++;
	skipThese(text, pos, " \t");
	uint8_t modrm = 3 << 6;
	modrm |= extender << 3;
	modrm |= a_reg_code;	
	streamAppendByte(stream, modrm);
	streamAppendByte(stream, getNum(&text[*pos], pos));
}
