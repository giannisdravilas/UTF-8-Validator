/* sdi1900053 */

#include <stdio.h>

int main(void){
	int counter1, counter2, ch, codepoint;
	counter1=0; 								/* counts 1-byte UTF-8 characters */
	counter2=0; 								/* counts multi-byte UTF-8 characters */
	while((ch=getchar())!=EOF){
    	if(ch>>7==0){ 							/* if the first bit of ch is 0 */
    		if((ch>=0x0000)&&(ch<=0x007F)){ 	/* if ch is a valid 1-byte UTF-8 character */
    			counter1++;
			}
		}else if(ch>>5==6){						/* if the 3 first bits of ch are 110 */
			codepoint=ch&0x1F; 					/* adds mask 00011111 to ch to get the 5 last bits of ch*/
			if((ch=getchar())==EOF){			/* gets 2nd byte */
				printf("Unexpected EOF\n");
				break;
			}else if(ch>>6==2){					/* if the 2 first bits of ch are 10 */
				codepoint<<=6;					/* shifts 6 bits left to receive the next bits from the 2nd byte */
				codepoint+=ch&0x3F;				/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
				if((codepoint>=0x0080)&&(codepoint<=0x07FF)){ 		/* if ch is a valid 2-byte UTF-8 character */
					counter2++;
				}else{
					printf("Oversized UTF-8 code point: U+%04X\n", codepoint);
					break;
				}
			}else{
				printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
				break;
			}
		}else if(ch>>4==14){					/* if the 4 first bits of ch are 1110 */
			codepoint=ch&0xF;					/* adds mask 00001111 to ch to get the 4 last bits ot ch */
			if((ch=getchar())==EOF){			/* gets 2nd byte */
				printf("Unexpected EOF\n");
				break;
			}else if(ch>>6==2){					/* if the 2 first bits of ch are 10 */
				codepoint<<=6;					/* shifts 6 bits left to receive the next bits from the 2nd byte */
				codepoint+=ch&0x3F;				/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
				if((ch=getchar())==EOF){		/* gets 3rd byte */
					printf("Unexpected EOF\n");
					break;
				}else if(ch>>6==2){				/* if the 2 first bits of ch are 10 */
					codepoint<<=6;				/* shifts 6 bits left to receive the next bits from the 3rd byte */
					codepoint+=ch&0x3F;			/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
					if(((codepoint>=0x0800)&&(codepoint<0xD800))||((codepoint>0xDFFF)&&(codepoint<=0xFFFF))){	/* if ch is a valid 3-byte UTF-8 character */
						counter2++;
					}else if(codepoint>=0xD800&&codepoint<=0xDFFF){		/* if ch belongs to the reserved range */
						printf("Invalid UTF-8 code point: U+%04X\n", codepoint);
						break;
					}else{
						printf("Oversized UTF-8 code point: U+%04X\n", codepoint);
						break;
					}
				}
				else{
					printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
					break;
				}
			}else{
				printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
				break;
			}
		}else if(ch>>3==30){					/* if the 5 first bits of ch are 11110 */
			codepoint=ch&0x7;					/* adds mask 00000111 to ch to get the 3 last bits ot ch */
			if((ch=getchar())==EOF){			/* gets 2nd byte */
				printf("Unexpected EOF\n");
				break;
			}else if(ch>>6==2){					/* if the 2 first bits of ch are 10 */
				codepoint<<=6;					/* shifts 6 bits left to receive the next bits from the 2nd byte */
				codepoint+=ch&0x3F;				/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
				if((ch=getchar())==EOF){		/* gets 3rd byte */
					printf("Unexpected EOF\n");
					break;
				}else if(ch>>6==2){				/* if the 2 first bits of ch are 10 (valid tail-byte) */
					codepoint<<=6;				/* shifts 6 bits left to receive the next bits from the 3rd byte */
					codepoint+=ch&0x3F;			/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
					if((ch=getchar())==EOF){	/* gets 4th byte */
						printf("Unexpected EOF\n");
						break;
					}else if(ch>>6==2){			/* if the 2 first bits of ch are 10 */
						codepoint<<=6;			/* shifts 6 bits left to receive the next bits from the 4th byte */
						codepoint+=ch&0x3F;		/* adds mask 00111111 to ch to get the 6 last bits of ch and adds them to the codepoint */
						if((codepoint>=0x010000)&&(codepoint<=0x10FFFF)){		/* if ch is a valid 4-byte UTF-8 character */
							counter2++;
						}else if(codepoint>=0xD800&&codepoint<=0xDFFF){			/* if ch belongs to the reserved range */
							printf("Invalid UTF-8 code point: U+%04X\n", codepoint);
							break;
						}else if(codepoint>=0x10FFFF){		/* if ch is out of the allowed range */
							printf("Invalid UTF-8 code point: U+%04X\n", codepoint);
							break;
						}else{
							printf("Oversized UTF-8 code point: U+%04X\n", codepoint);
							break;
						}
					}else{
						printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
						break;
					}
				}else{
					printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
					break;
				}
			}else{
				printf("Invalid UTF-8 tail byte: 0x%02X\n", ch);
				break;
			}
		}else{
			printf("Invalid UTF-8 header byte: 0x%02X\n", ch);
			break;
		}
  	}
	if(ch==EOF){
			printf("Found %d ASCII and %d multi-byte UTF-8 characters.\n", counter1, counter2);
	}
	return 0;
}
