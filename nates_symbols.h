#ifndef NATESSYMBOLS
#define NATESSYMBOLS

/*....<..1.>
 *....^....^
 *....3....2
 *....2.....
 *....v....v
 *....<.64.>
 *....^....^
 *....1....4
 *....6.....
 *....v....v
 *....<..8.>*/
 
const byte ZERO  =127-32;
const byte ONE = 2+4;
const byte TWO = 1+2+32+16+8;
const byte THREE = 32+8+4+2+1;
const byte FOUR  =2+4+32+64;
const byte FIVE  =127-2-16;
const byte SIX   =127-2;
const byte SEVEN =1+2+4;
const byte EIGHT =127;
const byte NINE  =127-16;
const byte A = 1+ 2 + 4 + 16 + 32 + 64;
const byte B = 127 -1-2; //little b
const byte C = 1+8+16+64;
const byte D = 2+4+8+16+32; //litle d
const byte E = 1+8+16+32+64;
const byte F = 1+16+32+64;
const byte G = 1+2+4+8+32+64; //little g
const byte H = 127-1-2-8; //little h
const byte I = 2 + 4;
const byte J = 2+4+8;
const byte K = 127-2-8; //kind of a hacked up little k ('lil h bar)
const byte L = 64+16+8;
const byte M = 16+32+4 + 1; //like little n w/ a line over it
const byte N = 127-32-8;
const byte O = 32+16+4+8; //little o
const byte P = 127-4-8;
const byte Q = 127-16-8; //little q
const byte R = 16+32;  //little r
const byte S = 1+64+32+4+8;
const byte T = 64+32+16+8; //little t
const byte U = 127-1-32;
const byte V = 16+8+4; //like little u
const byte W = 1 + 16 + 8 + 4; //hack v with a line over it
const byte X = 127 - 1 - 8;
const byte Y = G - 1; //little y
const byte Z = 1 + 2 + 32 + 16 + 8;
const byte DASH = 32;
const byte VBARS = 1 + 32 + 8;
const byte HBARS = 64+16+2+4;
const byte UNDERSCORE = 8;
const byte COLON = 9;
const byte R1 = 1;
const byte R2 = 2;
const byte R3 = 4;
const byte R4 = 8;
const byte R5 = 16;
const byte R6 = 64;
const byte CR1 = 1+8;
const byte CR2 = 2+16;
const byte CR3 = 4+64;
const byte SPACE = 128;

const byte alphabet[] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
DASH,VBARS,HBARS,UNDERSCORE,R1,R2,R3,R4,R5,R6,CR1,CR2,CR3,
SPACE};
const byte alphabetLen = sizeof(alphabet);

#endif
