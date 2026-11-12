// QUANTA 2 - Schrodinger's equation, live, on ATmega328P + SSD1306/SH1106 128x64 (I2C)
// Units: hbar = m = 1, dx = 1 cell, dt = 1/4 per step, V = pot/32, hard walls at both ends.
// 8 experiments | Born-rule measurement fed by physical noise | momentum view (FFT) | statistics
// Optional (nothing required): button D2->GND, buzzer D9->GND. The D13 LED is the detector.
#include <Wire.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define NX 128
#define BTN 2             // tap = measure now / next experiment, hold = wave <-> momentum view
#define BUZ 9             // detector click
#define COL0 0            // 0 = SSD1306, 2 = SH1106 (many 1.3" modules)
#define I2C_HZ 400000UL   // try 800000UL for a higher frame rate
#define TEVO 120          // frames of free evolution, then the measurement
#define TPOST 70          // frames after the collapse

int16_t re[NX], im[NX];                       // wavefunction psi = re + i*im        (512 B)
int8_t  pot[NX];                              // potential (negative = well)
uint8_t pc[NX], pg[NX], kb[64];               // terrain height | OLED page | momentum bars
int16_t twc[32];                              // FFT twiddles (cos, Q15)
union { int16_t f[128]; uint8_t b[256]; } S;  // scratch: FFT first, then envelope + top panel
#define hh (S.b)
#define tp (S.b+128)
volatile uint32_t pool;                       // entropy pool filled by physical noise
uint32_t rs=2463534242UL, ex;                 // PRNG state | expected 'right' hits (sum of %)
uint16_t t, nm, nr;                           // frame | measurements | found on the right
uint8_t ph, sc, fl, xm, pL, pR, spf=6, zo, ts=12, Eu=9, yE=50, kv, hold, adr=0x3C;
char buf[20];

ISR(WDT_vect){ uint32_t p=pool^TCNT1; p^=p<<13; p^=p>>17; p^=p<<5; pool=p; }   // watchdog RC jitter vs crystal
uint32_t rnd(){ uint32_t p; noInterrupts(); p=pool; interrupts(); rs^=p; rs^=rs<<13; rs^=rs>>17; rs^=rs<<5; return rs; }

const uint8_t INIT[] PROGMEM = {0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0,0x40,0x8D,0x14,0x20,0x02,0xA1,0xC8,
                                0xDA,0x12,0x81,0xCF,0xD9,0xF1,0xDB,0x40,0xA4,0xA6,0x2E,0xAF};
const char NM[8][16] PROGMEM = {"TUNNELING","RESONANCE","QUANTUM WELL","POTENTIAL STEP",
                                "OSCILLATOR","BRAGG CRYSTAL","DISORDER","QUANTUM BOUNCER"};
#define G(a,b,c,d,e) ((a<<12)|(b<<9)|(c<<6)|(d<<3)|e)          // 3x5 glyph, one octal digit per row
const char GL[] PROGMEM = " %.+-/:=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const uint16_t FNT[] PROGMEM = {
  G(0,0,0,0,0),G(5,1,2,4,5),G(0,0,0,0,2),G(0,2,7,2,0),G(0,0,7,0,0),G(1,1,2,4,4),G(0,2,0,2,0),G(0,7,0,7,0),
  G(7,5,5,5,7),G(2,6,2,2,7),G(6,1,2,4,7),G(6,1,2,1,6),G(5,5,7,1,1),G(7,4,6,1,6),G(3,4,7,5,7),G(7,1,2,2,2),G(7,5,7,5,7),G(7,5,7,1,6),
  G(2,5,7,5,5),G(6,5,6,5,6),G(3,4,4,4,3),G(6,5,5,5,6),G(7,4,6,4,7),G(7,4,6,4,4),G(3,4,5,5,3),G(5,5,7,5,5),G(7,2,2,2,7),G(1,1,1,5,2),G(5,5,6,5,5),G(4,4,4,4,7),G(5,7,7,5,5),
  G(6,5,5,5,5),G(2,5,5,5,2),G(6,5,6,4,4),G(2,5,5,6,3),G(6,5,6,5,5),G(3,4,2,1,6),G(7,2,2,2,2),G(5,5,5,5,7),G(5,5,5,5,2),G(5,5,7,7,5),G(5,5,2,5,5),G(5,5,2,2,2),G(7,1,2,4,7)};

void cmd(uint8_t c){ Wire.beginTransmission(adr); Wire.write((uint8_t)0); Wire.write(c); Wire.endTransmission(); }
bool probe(uint8_t a){ Wire.beginTransmission(a); return !Wire.endTransmission(); }

void packet(uint8_t x0, float s, float k){                 // gaussian packet with momentum k, norm = 5e7
  float n=0, d, g;
  for(uint8_t x=1; x<NX-1; x++){ d=(float)x-x0; n+=exp(-d*d/(2*s*s)); }
  float A=sqrt(5e7/n);                                     // exact normalisation (walls clip the tails)
  for(uint8_t x=1; x<NX-1; x++){ d=(float)x-x0; g=A*exp(-d*d/(4*s*s)); re[x]=g*cos(k*d); im[x]=g*sin(k*d); }
  re[0]=im[0]=re[NX-1]=im[NX-1]=0;
}

void step(){                                               // i d(psi)/dt = H psi, H = -1/2 d2/dx2 + V (energy zero shifted by 1)
  for(uint8_t x=1; x<NX-1; x++){
    int16_t a=(im[x-1]+im[x+1]+4)>>3; int8_t v=pot[x];
    if(v) a-=((int32_t)v*im[x]+64)>>7;
    re[x]-=a;
  }
  for(uint8_t x=1; x<NX-1; x++){
    int16_t a=(re[x-1]+re[x+1]+4)>>3; int8_t v=pot[x];
    if(v) a-=((int32_t)v*re[x]+64)>>7;
    im[x]+=a;
  }
}

void scene(){                                              // experiment sc, parameters drawn from physical noise
  uint32_t r=rnd(); uint8_t w=2+(r&1), h=9+((r>>1)&7), g=3+((r>>4)&3), s, i, X0=24; float sg=7, k=0.785;   // E = 1-cos(k) = 9.4 units
  memset(pot,0,NX); zo=0; ts=12; Eu=9; spf=6;
  switch(sc){
    case 0: s=64-w/2; for(i=0;i<w;i++) pot[s+i]=h; break;                                         // tunnelling
    case 1: s=64-(2*w+g)/2; for(i=0;i<w;i++) pot[s+i]=pot[s+w+g+i]=h; break;                      // resonant double barrier
    case 2: w=8+((r>>6)&7); h=6+((r>>9)&7); zo=h; s=64-w/2; for(i=0;i<w;i++) pot[s+i]=-(int8_t)h; break;   // potential well
    case 3: h=4+((r>>6)&15); for(i=64;i<NX;i++) pot[i]=h; break;                                  // potential step
    case 4: spf=8; Eu=17; X0=(r&0x40)?84:44; sg=3; k=0;                                           // harmonic oscillator, coherent state
            for(i=0;i<NX;i++){ int16_t d=i-64, v=d*d/24; pot[i]=v>127?127:v; } break;
    case 5: { uint8_t a=(r&0x40)?4:3+2*((r>>7)&1), n=6+((r>>8)&3); h=10+((r>>10)&3); s=68-n*a/2;   // crystal (a=4: Bragg gap)
              for(i=0;i<n;i++) pot[s+i*a]=h; } break;
    case 6: for(i=50;i<110;i++) pot[i]=rnd()%19; break;                                           // Anderson localisation
    case 7: spf=8; ts=6; X0=40+((r>>6)&31); sg=3; k=0; Eu=X0/3;                                   // quantum bouncer
            for(i=0;i<NX;i++) pot[i]=i/3; break;
  }
  for(i=0;i<NX;i++){ int16_t c=(((int16_t)pot[i]+zo)*ts)>>3; pc[i]=c<0?0:c>31?31:c; }
  int16_t e=63-((((int16_t)Eu+zo)*ts)>>3); yE=e<33?33:e;
  packet(X0,sg,k);
}

void measure(){                                            // Born rule: x ~ |psi|^2, then collapse
  uint32_t tot=0,c=0; uint8_t x;
  for(x=0;x<NX;x++) tot+=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x];
  uint32_t r=rnd()%(tot|1);
  for(x=0;x<NX-1;x++){ c+=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x]; if(c>r) break; }
  xm=x; fl=8; nm++; if(xm>=64) nr++; ex+=pR;
  for(uint8_t i=0;i<40;i++){ digitalWrite(BUZ,i&1); delayMicroseconds(150); } digitalWrite(BUZ,0);   // detector click
  packet(xm,3,0);
}

uint8_t rev6(uint8_t i){ uint8_t r=0; for(uint8_t b=0;b<6;b++){ r=(r<<1)|(i&1); i>>=1; } return r; }
void kspace(){                                             // |phi(k)|: 64-pt FFT of psi sampled every 2nd cell, k in [-pi/2,pi/2)
  int16_t *fr=S.f, *fi=S.f+64;
  for(uint8_t i=0;i<64;i++){ uint8_t j=rev6(i); fr[j]=re[2*i]; fi[j]=im[2*i]; }
  for(uint8_t n=2,w=32; n<=64; n<<=1,w>>=1)
    for(uint8_t b=0;b<64;b+=n) for(uint8_t j=0;j<n/2;j++){
      uint8_t k=j*w, a=b+j, c=a+n/2;
      int16_t wr=twc[k], ws=twc[k<16?16-k:k-16], ur=fr[a], ui=fi[a];
      int32_t tr=((int32_t)fr[c]*wr+(int32_t)fi[c]*ws)>>15, ti=((int32_t)fi[c]*wr-(int32_t)fr[c]*ws)>>15;
      fr[a]=(ur+tr)>>1; fi[a]=(ui+ti)>>1; fr[c]=(ur-tr)>>1; fi[c]=(ui-ti)>>1;       // >>1 per stage: no overflow
    }
  for(uint8_t m=0;m<64;m++){
    int16_t p=abs(fr[m]), q=abs(fi[m]), mx=p>q?p:q, mn=p>q?q:p; uint8_t h=(mx+(mn>>1)-(mn>>3))>>4;   // |z| ~ max+0.375 min
    kb[(m+32)&63]=h>22?22:h;
  }
}

uint8_t span(uint8_t p, int8_t a, int8_t b){               // bitmask of rows a..b inside page p
  int8_t lo=p*8, hi=lo+7; if(a<lo)a=lo; if(b>hi)b=hi;
  return a>b ? 0 : (0xFF<<(a-lo)) & (0xFF>>(hi-b));
}
void glyph(uint8_t x, char c){
  const char*q=strchr_P(GL,c); if(!q||x>NX-3) return;
  uint16_t g=pgm_read_word(FNT+(q-GL));
  for(uint8_t cx=0;cx<3;cx++){ uint8_t b=0; for(uint8_t r=0;r<5;r++) if((g>>(14-3*r-cx))&1) b|=2<<r; pg[x+cx]|=b; }
}
void text(uint8_t x, const char*s){ while(*s){ glyph(x,*s++); x+=4; } }
void hud(bool bad){
  snprintf_P(buf,sizeof buf,PSTR("%u%%"),pL); text(2,buf);
  snprintf_P(buf,sizeof buf,PSTR("%u%%"),pR); text(127-4*strlen(buf),buf);
  if(bad) strcpy_P(buf,PSTR("NORM ERR"));
  else if(ph==1) snprintf_P(buf,sizeof buf,PSTR("%S"),NM[sc]);
  else if(t<30) snprintf_P(buf,sizeof buf,PSTR("FOUND X=%u"),xm);
  else { uint32_t e=ex/10; snprintf_P(buf,sizeof buf,PSTR("R %u/%u EXP %lu.%u"),nr,nm,e/10,(uint16_t)(e%10)); }
  text(64-2*strlen(buf),buf);
}

void draw(){
  bool sk=kv^(ph==2);                                      // top panel: wave Re(psi) or momentum |phi(k)|
  if(sk) kspace();
  uint32_t L=0,R=0;
  for(uint8_t x=0;x<NX;x++){
    uint32_t d=(int32_t)re[x]*re[x]+(int32_t)im[x]*im[x];
    if(x<64) L+=d; else R+=d;
    float m=sqrt((float)d)*0.010; hh[x]=m>30?30:m;         // sqrt(P): makes tiny tunnelled parts visible
    if(sk) tp[x]=31-kb[x>>1];
    else { int16_t y=20-(((int32_t)re[x]*91)>>14); tp[x]=y<9?9:y>31?31:y; }
  }
  uint32_t u=(L+R)/100+1; pL=L/u; pR=R/u;
  bool bad=(L+R<40000000UL||L+R>60000000UL);               // probability must stay ~5e7: fail loudly, never silently
  for(uint8_t p=0;p<8;p++){
    for(uint8_t x=0;x<NX;x++){
      uint8_t b=0;
      if(p>3){
        b=span(p,64-hh[x],63);                             // |psi| envelope (solid)
        uint8_t c=pc[x], c1=x?pc[x-1]:c;
        if(c|c1){ b|=span(p,63-c,63)&((x&1)?0xAA:0x55);    // potential: dotted terrain + outline
                  if(c<31||c1<31){ int8_t a=63-c, e=63-c1; if(a>e){int8_t q=a;a=e;e=q;} b^=span(p,a,e); } }
        if(ph==1 && p==(yE>>3) && !(x&3)) b^=1<<(yE&7);    // packet energy (dashed)
      } else if(p){
        if(sk) b=span(p,tp[x],31);
        else { int8_t a=tp[x], c=x?tp[x-1]:a; if(a>c){int8_t q=a;a=c;c=q;} b=span(p,a,c); }
      }
      if(p==4 && !(x&3)) b|=1;
      if(fl && p && abs((int)x-xm)<fl) b^=0xFF;            // detector flash
      pg[x]=b;
    }
    if(!p) hud(bad); else if(p==3 && sk){ text(2,"-K"); text(118,"+K"); }
    Wire.beginTransmission(adr); Wire.write((uint8_t)0); Wire.write((uint8_t)(0xB0|p)); Wire.write((uint8_t)(COL0&15)); Wire.write((uint8_t)(0x10|(COL0>>4))); Wire.endTransmission();
    for(uint8_t x=0;x<NX;x+=16){ Wire.beginTransmission(adr); Wire.write((uint8_t)0x40); Wire.write(pg+x,16); Wire.endTransmission(); }
  }
  if(fl) fl--;
  digitalWrite(13,fl>0);
  if(bad) ph=0;
}

void setup(){
  Wire.begin(); Wire.setClock(I2C_HZ);
  pinMode(BTN,INPUT_PULLUP); pinMode(BUZ,OUTPUT); pinMode(13,OUTPUT);
  if(!probe(adr)) adr=0x3D;
  if(!probe(adr)) for(;;){ digitalWrite(13,!digitalRead(13)); delay(100); }   // no OLED found: LED blinks fast
  for(uint8_t i=0;i<sizeof INIT;i++) cmd(pgm_read_byte(INIT+i));
  for(uint8_t i=0;i<32;i++) twc[i]=32767*cos(i*M_PI/32);
  TCCR1A=0; TCCR1B=1;                                      // Timer1 free-running at 16 MHz
  cli(); MCUSR&=~_BV(WDRF); WDTCSR|=_BV(WDCE)|_BV(WDE); WDTCSR=_BV(WDIE); sei();  // WDT -> interrupt every 16 ms
  delay(300);
}

void loop(){
  uint8_t b=0;                                             // button: 1 = tap, 2 = hold
  if(!digitalRead(BTN)){ if(hold<255) hold++; if(hold==18) b=2; }
  else { if(hold>0 && hold<18) b=1; hold=0; }
  if(b==2) kv^=1;
  if(!ph){ scene(); ph=1; t=0; }
  for(uint8_t i=0;i<spf;i++) step();
  t++;
  if(b==1) t=(ph==1)?TEVO+1:TPOST+1;
  if(ph==1 && t>TEVO){ measure(); ph=2; t=0; }
  else if(ph==2 && t>TPOST){ ph=0; sc=(sc+1)&7; }
  draw();
}
