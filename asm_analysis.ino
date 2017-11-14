// intRes = intIn1 * intIn2 >> 16
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 24 bit result
#define MultiU16X8toH16(intRes, charIn1, intIn2) \
asm volatile ( \
	"clr r26 \n\t" \
	"mul %A1, %B2 \n\t" \
	"movw %A0, r0 \n\t" \	
	"mul %A1, %A2 \n\t" \
	"add %A0, r1 \n\t" \
	"adc %B0, r26 \n\t" \
	"lsr r0 \n\t" \
	"adc %A0, r26 \n\t" \
	"adc %B0, r26 \n\t" \
	"clr r1 \n\t" \
	: \
	"=&r" (intRes) \
	: \
	"d" (charIn1), \
	"d" (intIn2) \
	: \
	"r26" \
	)

// intRes = longIn1 * longIn2 >> 24
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 48bit result
#define MultiU24X24toH16(intRes, longIn1, longIn2) \
asm volatile ( \
	"clr r26 \n\t" \
	"mul %A1, %B2 \n\t" \
	"mov r27, r1 \n\t" \
	"mul %B1, %C2 \n\t" \
	"movw %A0, r0 \n\t" \
	"mul %C1, %C2 \n\t" \
	"add %B0, r0 \n\t" \
	"mul %C1, %B2 \n\t" \
	"add %A0, r0 \n\t" \
	"adc %B0, r1 \n\t" \
	"mul %A1, %C2 \n\t" \
	"add r27, r0 \n\t" \
	"adc %A0, r1 \n\t" \
	"adc %B0, r26 \n\t" \
	"mul %B1, %B2 \n\t" \
	"add r27, r0 \n\t" \
	"adc %A0, r1 \n\t" \
	"adc %B0, r26 \n\t" \
	"mul %C1, %A2 \n\t" \
	"add r27, r0 \n\t" \
	"adc %A0, r1 \n\t" \
	"adc %B0, r26 \n\t" \
	"mul %B1, %A2 \n\t" \
	"add r27, r1 \n\t" \
	"adc %A0, r26 \n\t" \
	"adc %B0, r26 \n\t" \
	"lsr r27 \n\t" \
	"adc %A0, r26 \n\t" \
	"adc %B0, r26 \n\t" \
	"clr r1 \n\t" \
	: \
	"=&r" (intRes) \
	: \
	"d" (longIn1), \
	"d" (longIn2) \
	: \
	"r26" , "r27" \
	)

int timer;
int timer2;
uint8_t tmp_step_rate;
uint8_t tmp_step_rate2;
int  gain;
int  gain2;


void printBits(byte myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
}


uint16_t MultiU16X8toH16_emulation(uint8_t charIn1, uint16_t intIn2){
	uint8_t r26 = 0;
	uint8_t a0,b0;    // output
	uint8_t a1,a2,b2; //input
	a1 = charIn1;
	a2 = intIn2;
    b2 = intIn2 >> 8;
    uint16_t r1r0 = (uint16_t)a1*(uint16_t)b2;
    uint8_t r1 = r1r0 >> 8;
    uint8_t r0 = r1r0;
    a0 = r0;
    r1r0 = (int)a1*(int)a2;
    r1 = r1r0 >> 8;
    // r0 = r1r0; //not needed
    a0 = a0 + r1;
    b0 = r26;
    // r0 = r0 >> 1; //not needed
    // a0 = a0 + r26; //this does nothing
    // b0 = b0 + r26; //this does nothing
    // r1 = 0; //not needed
    uint16_t result = (b0<<8) + a0;
    return result;
}

uint16_t MultiU24X24toH16_emulation(uint32_t longIn1, uint32_t longIn2){
	uint8_t a0,b0;    // output 16bit
	uint8_t a1,b1,c1,a2,b2,c2; //input 32bit
	a1 = longIn1;
	b1 = longIn1>>8;
	c1 = longIn1>>16;
	a2 = longIn2;
    b2 = longIn2>>8;
    c2 = longIn2>>16;

	uint8_t r26 = 0;
	uint8_t r27;

    uint16_t r1r0 = (uint16_t)a1*(uint16_t)b2;
    uint8_t r1 = r1r0 >> 8;
    uint8_t r0 = r1r0;

    r27 = r1;

    r1r0 = (uint16_t)b1*(uint16_t)c2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    a0 = r0;
    b0 = r1;

    r1r0 = (uint16_t)c1*(uint16_t)c2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    b0 = b0 + r0;

    r1r0 = (uint16_t)c1*(uint16_t)b2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    a0 = a0 + r0;

    b0 = b0 + r1;

    r1r0 = (uint16_t)a1*(uint16_t)c2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    r27 = r27 + r0;

    a0 = a0 + r1;

    b0 = b0 + r26;

    r1r0 = (uint16_t)b1*(uint16_t)b2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    r27 = r27 + r0;

    a0 = a0 + r1;

    b0 = b0 + r26;

    r1r0 = (uint16_t)c1*(uint16_t)a2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    r27 = r27 + r0;

    a0 = a0 + r1;

    b0 = b0 + r26;

    r1r0 = (uint16_t)b1*(uint16_t)a2;
    r1 = r1r0 >> 8;
    r0 = r1r0;

    r27 = r27 + r1;

    a0 = a0 + r26;

    b0 = b0 + r26;

    r27 = r27 >> 1;

    a0 = a0 + r26;

    b0 = b0 + r26;

    r1 = 0;

    uint16_t result = (b0<<8) + a0;
    return result;
}

void setVar(){
	timer = 0;
	// tmp_step_rate = tmp_step_rate2 = B01001111; //8bit
	// gain = gain2 = (B01001111 * 256) + B00001111;//16bit
	tmp_step_rate = tmp_step_rate2 = 247; //8bit
	gain = gain2 = 117;//16bit
}

void simulation1(){
	//-----------------------------------
	Serial.println("******");
	// tmp_step_rate2 = B01111111; //8bit
	// gain2 = (B01111111 * 256) + B01111111;//16bit
	setVar();
	MultiU16X8toH16(timer2, tmp_step_rate2, gain2);
	Serial.println("A*B >> 16 (ASM)");
	printBits(timer2>>8);printBits(timer2);
	Serial.println();
	Serial.println(timer2,DEC);
	Serial.println("******");
	//-----------------------------------

	//-----------------------------------
	// assembler line-by-line emulation
	uint8_t r26 = 0;
	uint8_t a0,b0;    // output
	uint8_t a1,a2,b2; //input
	a1 = tmp_step_rate;
	a2 = gain;
    b2 = gain >> 8;
    int r1r0 = (int)a1*(int)b2;
    uint8_t r1 = r1r0 >> 8;
    uint8_t r0 = r1r0;
    a0 = r0;
    r1r0 = (int)a1*(int)a2;
    r1 = r1r0 >> 8;
    // r0 = r1r0;
    a0 = a0 + r1;
    b0 = r26;
    // r0 = r0 >> 1;
    // a0 = a0 + r26;
    // b0 = b0 + r26;
    // r1 = 0;
    int result = (b0<<8) + a0;
    printBits(b0);printBits(a0); 
    Serial.println();
    Serial.println(result,DEC);

	//-----------------------------------

	//-----------------------------------
	// tmp_step_rate = 0x0F; //8bit
	// gain = 0x0002;//16bit
	Serial.println("******");
	setVar();
	Serial.print("tmp_step_rate: ");Serial.print(tmp_step_rate,DEC);Serial.print(" ");printBits(tmp_step_rate);
	Serial.println();

	Serial.print("gain: ");Serial.print(gain);Serial.print(" ");
	printBits(gain/256);
	printBits(gain%256);
	Serial.println();

	long MultiU16X8toH16 = ((long) tmp_step_rate * (long) gain); //32bit
	Serial.print(tmp_step_rate,DEC);
	Serial.print("x");
	Serial.print(gain,DEC);
	Serial.print("=");
	Serial.println(MultiU16X8toH16);
	Serial.print("Full bits: ");
	printBits(MultiU16X8toH16 >> 16);
	printBits(MultiU16X8toH16 >> 8);
	printBits(MultiU16X8toH16);
	Serial.println();
	char moge = (MultiU16X8toH16) >> 16;
	Serial.print(">>16 bits shift: ");
	printBits(moge>>8);printBits(moge);
	Serial.println();
	Serial.println(moge,DEC);
	Serial.println("******");
	//-----------------------------------
}

long acc_time = 1000;
void setup(){
	Serial.begin(115200);
}

void loop() {
    // put your setup code here, to run once:
	// test for MultiU16X8toH16();
	// simulation1();
	// 値を調べる
	unsigned short acc_step_rate;
	// long acc_time = 12885; //32bit
	// long acc_time = 12885; //32bit
	long acc_rate = 671088; //32bit
	// acc_step_rate: 516, acc_time: 12885, acc_rate: 671088
	MultiU24X24toH16(acc_step_rate, acc_time, acc_rate);
	Serial.print(acc_time);Serial.print(",");
	Serial.print(acc_step_rate);Serial.print(",");
	// Serial.print("acc step rate: ");Serial.println(acc_step_rate);
	// Serial.print("acc time: ");Serial.println(acc_time); 
	// Serial.print("acc rate: ");Serial.println(acc_rate); 

	// Serial.println("**********");
	// assembler line-by-line emulation
	// uint8_t a0,b0;    // output 16bit
	// uint8_t a1,b1,c1,a2,b2,c2; //input 32bit
	// a1 = acc_time;
	// b1 = acc_time>>8;
	// c1 = acc_time>>16;
	// a2 = acc_rate;
 //    b2 = acc_rate>>8;
 //    c2 = acc_rate>>16;

	// uint8_t r26 = 0;
	// uint8_t r27;

 //    int r1r0 = (int)a1*(int)b2;
 //    uint8_t r1 = r1r0 >> 8;
 //    uint8_t r0 = r1r0;

 //    r27 = r1;

 //    r1r0 = (int)b1*(int)c2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    a0 = r0;
 //    b0 = r1;

 //    r1r0 = (int)c1*(int)c2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    b0 = b0 + r0;

 //    r1r0 = (int)c1*(int)b2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    a0 = a0 + r0;

 //    b0 = b0 + r1;

 //    r1r0 = (int)a1*(int)c2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    r27 = r27 + r0;

 //    a0 = a0 + r1;

 //    b0 = b0 + r26;

 //    r1r0 = (int)b1*(int)b2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    r27 = r27 + r0;

 //    a0 = a0 + r1;

 //    b0 = b0 + r26;

 //    r1r0 = (int)c1*(int)a2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    r27 = r27 + r0;

 //    a0 = a0 + r1;

 //    b0 = b0 + r26;

 //    r1r0 = (int)b1*(int)a2;
 //    r1 = r1r0 >> 8;
 //    r0 = r1r0;

 //    r27 = r27 + r1;

 //    a0 = a0 + r26;

 //    b0 = b0 + r26;

 //    r27 = r27 >> 1;

 //    a0 = a0 + r26;

 //    b0 = b0 + r26;

 //    r1 = 0;

 //    int result = (b0<<8) + a0;
    // Serial.print("a0: ");Serial.println(a0);
    // Serial.print("b0: ");Serial.println(b0);
    // Serial.println("acc step rate:");
    // printBits(b0);printBits(a0); 
    // Serial.println();
	int result = MultiU24X24toH16_emulation(acc_time, acc_rate);
    Serial.println(result,DEC);

    acc_time = acc_time + 10000;
    if(acc_time>500000){
    	acc_time = 1000;
    }
}
