// QUANTA 2.0 - Quantum Lab on ATmega328P + SSD1306 128x64 (I2C: SDA=A4, SCL=A5; Mega: 20/21)
// Five live experiments from one Schrodinger solver: TUNNEL, BLOCH, HARMONIC, ANDERSON, BORN
#include <Wire.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define NX 128
int16_t re[NX], im[NX];                           // psi = re + i*im                    (512 B)
int8_t  pot[NX];                                  // potential V = pot/32               (128 B)
uint8_t hh[NX], wy[NX], ly[NX], pg[NX], cnt[NX];  // bars | wave y | line y | OLED page | Born histogram (640 B)
volatile uint32_t pool;                           // physical entropy (watchdog jitter)
uint32_t rng=1, Wsum;                             // xorshift state, Born weight sum
uint8_t kind, ph, fl, xm, pL, pR, agr;            // experiment, phase, flash, last click, %left, %right, Born agreement
uint16_t t, N, vsc;                               // frame counter, detections, potential-line scale

ISR(WDT_vect){ pool = (pool << 5) ^ (pool >> 27) ^ TCNT1L; }  // watchdog RC jitter vs crystal

const uint8_t INIT[] PROGMEM = {0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0,0x40,0x8D,0x14,0x20,0x02,0xA1,0xC8,
                                0xDA,0x12,0x81,0xCF,0xD9,0xF1,0xDB,0x40,0xA4,0xA6,0x2E,0xAF};
const uint8_t FNT[] PROGMEM = {
  0x1F,0x11,0x1F, 0x12,0x1F,0x10, 0x1D,0x15,0x17, 0x15,0x15,0x1F, 0x07,0x04,0x1F,   // 0 1 2 3 4
  0x17,0x15,0x1D, 0x1F,0x15,0x1D, 0x01,0x01,0x1F, 0x1F,0x15,0x1F, 0x17,0x15,0x1F,   // 5 6 7 8 9
  0x19,0x04,0x13,                                                                   // %
  0x01,0x1F,0x01, 0x1F,0x10,0x1F, 0x1F,0x01,0x1E, 0x1F,0x15,0x11, 0x1F,0x10,0x10,   // T U N E L
  0x1F,0x15,0x0A, 0x0E,0x11,0x0E, 0x0E,0x11,0x11, 0x1F,0x04,0x1F, 0x1E,0x05,0x1E,   // B O C H A
  0x1F,0x05,0x1A, 0x1F,0x06,0x1F, 0x11,0x1F,0x11, 0x1F,0x11,0x0E, 0x12,0x15,0x09};  // R M I D S
const char    AB[]  PROGMEM = "TUNELBOCHARMIDS";                           // letter -> glyph 11+index
const char    TT[]  PROGMEM = "TUNNEL  BLOCH   HARMONICANDERSONBORN    ";  // 8 chars per title
const uint8_t TEV[] PROGMEM = {110,140,140,130,46};                        // evolution frames per experiment

uint32_t rnd(){                                    // xorshift32 stirred by the physical pool
  noInterrupts(); uint32_t p=pool; interrupts();
  rng^=p; rng^=rng<<13; rng^=rng>>17; rng^=rng<<5; return rng;
}

void cmd(uint8_t c){ Wire.beginTransmission(0x3C); Wire.write((uint8_t)0); Wire.write(c); Wire.endTransmission(); }

void packet(float x0, float s, float k, float A){          // gaussian wave packet with momentum k
  for(int x=0; x<NX; x++){ float d=x-x0, g=A*exp(-d*d/(4*s*s)); re[x]=g*cos(k*d); im[x]=g*sin(k*d); }
}

void step(){                                               // i d(psi)/dt = H psi, dt=1/4, fixed-point
  for(uint8_t x=1; x<NX-1; x++){
    int16_t a = (im[x-1]+im[x+1]+4)>>3;
    if(pot[x]) a -= ((int32_t)pot[x]*im[x]+64)>>7;
    re[x] -= a;
  }
  for(uint8_t x=1; x<NX-1; x++){
    int16_t a = (re[x-1]+re[x+1]+4)>>3;
    if(pot[x]) a -= ((int32_t)pot[x]*re[x]+64)>>7;
    im[x] += a;
  }
}

void scene(){                                              // set up the current experiment
  uint32_t r=rnd(); uint8_t x; int16_t mx=1;
  ph=1; t=0; N=0; fl=0; memset(pot,0,NX);
  if(kind==0 || kind==4){                                  // TUNNEL / BORN: barrier(s)
    if(kind==4) r=11;                                      // Born: fixed single barrier w=3, h=14
    uint8_t w=2+(r&1), h=9+((r>>1)&7), g=3+((r>>4)&3), s;
    if(r&0x40){ s=64-(2*w+g)/2; for(x=0;x<w;x++) pot[s+x]=pot[s+w+g+x]=h; }   // double barrier
    else      { s=64-w/2;       for(x=0;x<w;x++) pot[s+x]=h; }                 // single barrier
    packet(24,7,0.785,1700);
  } else if(kind==1){                                      // BLOCH: constant force on a lattice
    int8_t sg=(r&1)?1:-1; uint8_t k=2+((r>>1)&1);
    for(x=0;x<NX;x++) pot[x]=sg*((((int16_t)x-64)*k)/2);
    packet(64+sg*(64/k),6,0,1830);
  } else if(kind==2){                                      // HARMONIC: parabolic trap, omega=1/16
    for(x=0;x<NX;x++){ int16_t d=(int16_t)x-64, v=(d*d+8)>>4; pot[x]=v>127?127:v; }
    if(r&1) packet(64+((r&2)?12:-12),2.83,0,2670);         // coherent state: oscillates, never spreads
    else    packet(64,5,0,2010);                           // squeezed state: breathes
  } else {                                                 // ANDERSON: random potential
    uint8_t W=32+((r&1)<<3);
    for(x=0;x<NX;x++) pot[x]=(int8_t)(rnd()%(W+1))-W/2;
    packet(56,7,0.785,1700);
  }
  for(x=0;x<NX;x++){ int16_t a=pot[x]<0?-pot[x]:pot[x]; if(a>mx) mx=a; }
  if(kind==2) mx=25;                                       // show only the bottom of the trap
  vsc=2304/mx;
}

void measure(){                                            // Born rule on the live wave, then collapse
  uint32_t tot=0,c=0,r; uint8_t x;
  for(x=0;x<NX;x++) tot+=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x];
  r=rnd()%tot;
  for(x=0;x<NX-1;x++){ c+=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x]; if(c>r) break; }
  xm=x; fl=8; packet(xm,3,0,2580);
}

void freeze(){                                             // BORN: keep |psi| (0..255) as weights in im[]
  uint8_t x; int16_t mx=1;
  for(x=0;x<NX;x++){ im[x]=sqrt((float)re[x]*re[x]+(float)im[x]*im[x]); if(im[x]>mx) mx=im[x]; }
  Wsum=0;
  for(x=0;x<NX;x++){ im[x]=(int32_t)im[x]*255/mx; Wsum+=(uint32_t)im[x]*im[x]; }
  memset(cnt,0,NX); N=0;
}

void born(){                                               // detections: x ~ weights, histogram builds up
  uint8_t k = t<40 ? 1 : t<80 ? 4 : t<120 ? 12 : 30, x;
  while(k-- && N<1500){
    uint32_t r=rnd()%Wsum, c=0;
    for(x=0; x<NX-1; x++){ c+=(uint32_t)im[x]*im[x]; if(c>r) break; }
    if(cnt[x]<255) cnt[x]++;
    N++; xm=x; fl=2;
  }
}

uint8_t span(uint8_t p, int8_t a, int8_t b){               // bitmask of rows a..b inside page p
  int8_t lo=p*8, hi=lo+7; if(a<lo)a=lo; if(b>hi)b=hi;
  return a>b ? 0 : (0xFF<<(a-lo)) & (0xFF>>(hi-b));
}
void glyph(uint8_t x, uint8_t g){ for(uint8_t c=0;c<3;c++) pg[x+c] |= pgm_read_byte(FNT+g*3+c)<<1; }
void pct(uint8_t x, uint8_t v){ if(v>99)v=99; glyph(x,v/10); glyph(x+4,v%10); glyph(x+8,10); }
void n4(uint8_t x, uint16_t v){ for(uint8_t i=4;i--;){ glyph(x+4*i,v%10); v/=10; } }
void wline(uint8_t x){ int16_t y=20-(((int32_t)re[x]*91)>>14); wy[x]=y<9?9:y>31?31:y; }

void prepWave(){                                           // envelope, potential line, left/right probability
  uint32_t L=0, Rt=0;
  for(uint8_t x=0; x<NX; x++){
    uint32_t d=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x];
    if(x<64) L+=d; else Rt+=d;
    float m=sqrt((float)d)*0.008; hh[x]=m>28?28:m;         // sqrt(P): makes tiny parts visible
    int16_t v=50-(((int32_t)pot[x]*vsc)>>8); ly[x]=v<38?38:v>62?62:v;
    wline(x);
  }
  uint32_t u=(L+Rt)/100+1; pL=L/u; pR=Rt/u;
}

void prepBorn(){                                           // histogram bars, theory curve, agreement
  uint16_t cm=4, hm, g, th; uint32_t Kn=((uint32_t)N<<20)/Wsum, ov=0;
  for(uint8_t x=0; x<NX; x++) if(cnt[x]>cm) cm=cnt[x];
  hm=((uint32_t)65025*Kn)>>20; if(hm<cm) hm=cm;            // peak of expectation vs peak of data
  g=(26UL<<8)/hm;                                          // pixels per count (8.8 fixed)
  for(uint8_t x=0; x<NX; x++){
    uint32_t w=(uint32_t)im[x]*im[x], e16=(w*Kn)>>16, c16=(uint32_t)cnt[x]<<4;
    hh[x]=((uint32_t)cnt[x]*g)>>8;
    th=(e16*g)>>12; if(th>27) th=27; ly[x]=63-th;
    ov+=c16<e16?c16:e16;
    wline(x);
  }
  agr=N?ov*100/((uint32_t)N<<4):0;
}

void draw(){
  if(ph==3) prepBorn(); else prepWave();
  for(uint8_t p=0; p<8; p++){
    for(uint8_t x=0; x<NX; x++){
      uint8_t b=0; int8_t a, c;
      if(p>3){                                             // lower panel: envelope/bars + XOR line
        b=span(p,64-hh[x],63);
        if(ph==3 || pot[x] || (x && pot[x-1]) || !(x&3)){
          a=ly[x]; c=x?ly[x-1]:a; if(a>c){int8_t q=a;a=c;c=q;}
          b^=span(p,a,c);
        }
      } else if(p){                                        // upper panel: Re(psi)
        a=wy[x]; c=x?wy[x-1]:a; if(a>c){int8_t q=a;a=c;c=q;}
        b=span(p,a,c);
      }
      if(p==4 && !(x&3)) b|=1;
      if(fl && abs((int)x-xm)<fl) b^=0xFF;                 // detector flash
      pg[x]=b;
    }
    if(!p){                                                // header: numbers + title
      if(ph==3){ n4(2,N); pct(114,agr); } else { pct(2,pL); pct(114,pR); }
      const char*s=TT+kind*8; uint8_t n=0;
      while(n<8 && pgm_read_byte(s+n)!=' ') n++;
      for(uint8_t j=0;j<n;j++){ uint8_t k=0; while(pgm_read_byte(AB+k)!=pgm_read_byte(s+j)) k++; glyph(64-2*n+4*j,11+k); }
    }
    Wire.beginTransmission(0x3C); Wire.write((uint8_t)0); Wire.write(0xB0|p); Wire.write((uint8_t)0); Wire.write(0x10); Wire.endTransmission();
    for(uint8_t x=0; x<NX; x+=16){
      Wire.beginTransmission(0x3C); Wire.write(0x40);
      for(uint8_t j=0;j<16;j++) Wire.write(pg[x+j]);
      Wire.endTransmission();
    }
  }
  if(fl) fl--;
}

void next(){ kind=(kind+1)%5; ph=0; }

void setup(){
  Wire.begin(); Wire.setClock(400000);
  for(uint8_t i=0;i<sizeof INIT;i++) cmd(pgm_read_byte(INIT+i));
  TCCR1A=0; TCCR1B=1;                                      // Timer1 free-running at 16 MHz
  cli(); MCUSR&=~_BV(WDRF); WDTCSR|=_BV(WDCE)|_BV(WDE); WDTCSR=_BV(WDIE); sei();  // WDT -> interrupt every 16 ms
  delay(300);
}

void loop(){
  if(!ph) scene();
  if(ph<3) for(uint8_t i=0;i<6;i++) step();
  t++;
  if(ph==1 && t>pgm_read_byte(TEV+kind)){
    if(kind==0){ measure(); ph=2; t=0; }                   // TUNNEL ends with a measurement
    else if(kind==4){ freeze(); ph=3; t=0; }               // BORN freezes psi and starts counting
    else next();
  } else if(ph==2 && t>70) next();
  else if(ph==3){ born(); if(t>210) next(); }
  if(ph) draw();
}
