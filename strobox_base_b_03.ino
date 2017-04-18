// Strobox Base B - estou quase la

#define pinoSom 8
#define pinoTempo 10
#define pinoChaveEstado1 A0
#define pot1 A1
#define pot2 A2

byte numeroEscala = 0;
byte numeroEscalaAnterior = 0;
int tamanhoEscala = 0;
byte escala[11] = 0;

unsigned long microsAnterior = 0;
unsigned long microsAtual = 0;
unsigned long incremento = 0;
byte tocar = 0;
byte contadorSemiFusas = 0;
byte contadorSemiFusasAnterior = 16;
byte contadorSeminima = 0;


float bpm = 150;
float tempoBatida = (60.00*994517.72)/bpm; // um pulso em micro segundos Esse valor foi atingido ao fazer uma media
                                           // das oscilacoes de tempo durante 109 batidas. Foi o melhor que eu consegui
                                           // com esse codigo.


byte estadoChave1 = 1;
byte estadoChave1Anterior = 1;

//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------

// Tabela de frequencia das notas musicais temperadas da musica ocidental
//               C        C#/Db    D        D#/Eb    E        F        F#/Gb    G        G#/Ab    A        A#/Bb    B

/*float notas[] = {16.352,  17.324,  18.354,  19.445,  20.602,  21.827,  23.125,  24.500,  25.957,  27.500,  29.135,  30.868,
                 32.703,  34.648,  36.708,  38.891,  41.203,  43.654,  46.249,  48.999,  51.913,  55.000,  58.270,  61.735,
                 65.406,  69.296,  73.416,  77.782,  82.407,  87.303,  92.499,  97.999,  103.83,  110.00,  116.54,  123.47,
                 261.63,  277.18,  293.66,  311.13,  329.63,  349.23,  369.99,  392.00,  415.30,  440.00,  466.16,  493.88,
                 523.25,  554.37,  587.33,  622.25,  659.26,  698.46,  739.99,  783.99,  830.61,  880.00,  932.33,  987.77,
                 1046.5,  1108.7,  1174.7,  1244.5,  1318.5,  1396.9,  1480.0,  1568.0,  1661.2,  1760.0,  1864.7,  1975.5,
                 2093.0,  2217.5,  2349.3,  2489.0,  2637.0,  2793.8,  2960.0,  3136.0,  3322.4,  3520.0,  3729.3,  3951.1,
                 4186.0,  4434.9,  4698.6,  4978.0,  5274.0,  5587.7,  5919.9,  6271.9,  6644.9,  7040.0,  7458.6,  7902.1,
                 8372.0,  8869.8,  9397.3,  9956.1,  10548.1, 11175.3, 11839.8, 12543.9, 13289.8, 14080.0, 14917.2, 15804.3,
                 16744.0, 17739.7, 18794.5, 19912.1, 21096.2, 22350.6, 23679.6, 25087.7, 26579.5, 28160.0, 29834.5, 31608.5};
*/
//                         C        D        E       F#/Gb    G        A        B
float escalaMiMenor[] = {16.352,  18.354,  20.602,  23.125,  24.500,  27.500,  30.868,
                         32.703,  36.708,  41.203,  46.249,  48.999,  55.000,  61.735,
                         65.406,  73.416,  82.407,  92.499,  97.999,  110.00,  123.47,
                         261.63,  293.66,  329.63,  369.99,  392.00,  440.00,  493.88,
                         523.25,  587.33,  659.26,  739.99,  783.99,  880.00,  987.77,
                         1046.5,  1174.7,  1318.5,  480.0,  1568.0,  1760.0,  1975.5,
                         2093.0,  2349.3,  2637.0,  2960.0,  3136.0,  3520.0,  3951.1,
                         4186.0,  4698.6,  5274.0,  5919.9,  6271.9,  7040.0,  7902.1,
                         8372.0,  9397.3,  10548.1, 11839.8, 12543.9, 14080.0, 15804.3,
                         16744.0, 18794.5, 21096.2, 23679.6, 25087.7, 28160.0, 31608.5};
          
//                               D        E       G        A        B
float escalaPentaMiMenor[] =  {18.354,  20.602,  24.500,  27.500,  30.868,
                               36.708,  41.203,  48.999,  55.000,  61.735,
                               73.416,  82.407,  97.999,  110.00,  123.47,
                               293.66,  329.63,  392.00,  440.00,  493.88,
                               587.33,  659.26,  783.99,  880.00,  987.77,
                               1174.7,  1318.5,  1568.0,  1760.0,  1975.5,
                               2349.3,  2637.0,  3136.0,  3520.0,  3951.1,
                               4698.6,  5274.0,  6271.9,  7040.0,  7902.1,
                               9397.3,  10548.1, 12543.9, 14080.0, 15804.3,
                               18794.5, 21096.2, 25087.7, 28160.0, 31608.5};


//                             D        C#/Db    E        F#/Gb    G        G#/Ab    A        B

float escalaBluesMenor[] =  {18.354,  17.324,  20.602,  23.125,  24.500,  25.957,  27.500,  30.868,
                             36.708,  34.648,  41.203,  46.249,  48.999,  51.913,  55.000,  61.735,
                             73.416,  69.296,  82.407,  92.499,  97.999,  103.83,  110.00,  123.47,
                             293.66,  277.18,  329.63,  369.99,  392.00,  415.30,  440.00,  493.88,
                             587.33,  554.37,  659.26,  739.99,  783.99,  830.61,  880.00,  987.77,
                             1174.7,  1108.7,  1318.5,  1480.0,  1568.0,  1661.2,  1760.0,  1975.5,
                             2349.3,  2217.5,  2637.0,  2960.0,  3136.0,  3322.4,  3520.0,  3951.1,
                             4698.6,  4434.9,  5274.0,  5919.9,  6271.9,  6644.9,  7040.0,  7902.1,
                             9397.3,  8869.8,  10548.1, 11839.8, 12543.9, 13289.8, 14080.0, 15804.3,
                             18794.5, 17739.7, 21096.2, 23679.6, 25087.7, 26579.5, 28160.0, 31608.5};

// Lista de tonalidades ou raizes de acorde
              
const byte C  = 0;
const byte Csus = 1;
const byte Db = 1;
const byte D  = 2;
const byte Dsus = 3;
const byte Eb = 3;
const byte E  = 4;
const byte F  = 5;
const byte Fsus = 6;
const byte Gb = 6;
const byte G  = 7;
const byte Gsus = 8;
const byte Ab = 8;
const byte A  = 9;
const byte Asus = 10;
const byte Bb = 10; 
const byte B = 11;

// Mapeamento das escalas (os intervalos de uma escala)

/*
const byte escalaMaior[] =          {0, 2, 4 ,5, 7, 9, 11};
const byte escalaMenorNatural[] =   {0, 2, 3, 5, 7, 8, 10};
const byte escalaMenorHarmonica[] = {0, 2, 3, 5, 7, 8, 11};
const byte escalaMenorMelodica[] =  {0, 2, 3, 5, 7, 9, 11};
const byte escalaHexafonica[] =     {0, 2, 4, 6, 8, 10};
const byte petatonicaMenor[] =      {0, 3, 5, 7, 10};
const byte petatonicaMaior[] =      {0, 2, 4, 7, 9};
const byte EscalaDiminuta[] =       {0, 2, 3, 5, 6, 8, 9, 11};
const byte escalaCromatica[] =      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
//escala domdim
//escala dimdom





// Escalas modais

const byte modoJonico[] =    {0, 2, 4 ,5, 7, 9, 11};
const byte modoDorico[] =    {0, 2, 3, 5, 7, 9, 10};
const byte modoFrigio[] =    {0, 1, 3, 5, 7, 8, 10};
const byte modoLidio[] =     {0, 2, 4, 6, 7, 9, 11};
const byte modoMixolidio[] = {0, 2, 4, 5, 7, 9, 10};
const byte modoEolio[] =     {0, 2, 3, 5, 7, 8, 10};
const byte modoLocrio[] =    {0, 1, 3, 5, 6, 8, 10};

/*Acordes

const byte triadeMaior[] =    {};
const byte triadeMenor[] =    {};
const byte triadeAumentada[] =    {};
const byte triadeDiminuta[] =    {};
const byte triadeMaior[] =    {};
const byte triadeMaior[] =    {};
*/
*/

//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------



void setup() {
  
}

void loop() {
  ControleBatida();
  ControleTempo();
  Tocar();
  
}



//-------------------------------------------------------------------------------------------------------------
//Essa funcao controla a variavel microsAnterior para que o tempo se manenha constante mesmo com
//as variacoes do tempo dos loops

void ControleBatida() {
  microsAtual = micros();
  
  if ((microsAtual - microsAnterior) >= tempoBatida/16) {
    if (contadorSemiFusas <= 15) {
      contadorSemiFusas++;
      /*contadorSeminima++;*/
    }
    else if (contadorSemiFusas = 16) {
      contadorSemiFusas = 0;
    }
    /*else if (contadorSeminima == 16){
      contadorSeminima = 0;
    }*/
    
    if ((microsAtual - microsAnterior) > tempoBatida/16) {              // corrige o tempo que varia
      incremento = (microsAtual - microsAnterior) - tempoBatida/16;
    } 
    microsAnterior = microsAtual - incremento;
  }
}

//-------------------------------------------------------------------------------------------------------------

// Funcao que le a roda de estados e a apartir disso, divide em 6 valores, que sao os tempos das notas.
// Os valores sao referentes a escada de resistores:
// 0 ohms, 160 ohms, 630 ohms, 1000 ohms 2200 ohms, 10k ohms

void ChaveEstado1() {
  int leituraSensor = analogRead(pinoChaveEstado1);
    
    if(leituraSensor >= 1018) {                                  
       estadoChave1 = 1;
    }
    else if(leituraSensor >= 1001 && leituraSensor <= 1011) {   
      estadoChave1 = 2;
    }
    else if(leituraSensor >= 941 && leituraSensor <= 951) { 
       estadoChave1 = 3;
    }
    else if(leituraSensor >= 925 && leituraSensor <= 935) {  
       estadoChave1 = 4;
    }
    else if(leituraSensor >= 834 && leituraSensor <= 844) {  
       estadoChave1 = 5;
    }
    else if(leituraSensor >= 506 && leituraSensor <= 516) { 
       estadoChave1 = 6;
    }
}


//-------------------------------------------------------------------------------------------------------------
// Controle do tempo das notas

void ControleTempo() {
 
  ChaveEstado1();
  
  if (contadorSemiFusas != contadorSemiFusasAnterior) {
      switch (estadoChave1) {
      
        case 1:
          if (contadorSemiFusas == 0 /*&& contadorSeminima == 0*/) { // por causa da intermitencia desativei o tocar desta figura. Melhor o silencio.
              tocar = 0; 
          } 
        break;

        case 2:
          if (contadorSemiFusas == 0) {
            tocar = 1;
          }
        break;
           
        case 3:
          if (contadorSemiFusas == 0 || contadorSemiFusas == 8) {
            tocar = 1;
          }
        break;
  
        case 4:
          if (contadorSemiFusas == 0 || contadorSemiFusas == 4 || contadorSemiFusas == 8 || contadorSemiFusas == 12) {
            tocar = 1;
          }
        break;
  
        case 5:
          if (contadorSemiFusas == 0 || contadorSemiFusas == 2 || contadorSemiFusas == 4 || contadorSemiFusas == 6 || contadorSemiFusas == 8 || contadorSemiFusas == 10 || contadorSemiFusas == 12 || contadorSemiFusas == 14) {
            tocar = 1;
          }
        break;
  
        case 6:
          tocar = 1;
        break;
      } 
  }
  estadoChave1Anterior = estadoChave1;
  contadorSemiFusasAnterior = contadorSemiFusas;
}


//-------------------------------------------------------------------------------------------------------------
// Funcao resposavel por tocar as notas

void Tocar() {
  if (tocar == 1) {
    tone(pinoSom, notas[map(analogRead(pot1), 0, 1023, 0, 119)], 40);
  }
  tocar = 0;
}


//-------------------------------------------------------------------------------------------------------------
// Funcao que controla a selecao de escalas e o array escalas[]

void controleEscala() {
  
  numeroEscala = map(analogRead(pot2), 0, 1023, 0, 20);
  
  if (numeroEscala != numeroEscalaAnterior) {
  
    switch (numeroEscala) {
      case 1:
        tamanhoEscala = sizeof(escalaMenor)/sizeof(byte);
        for(int i = 0; i > tamanhoEscala; i++) {
         escala[i] = escalaMenor[i];
        }
        break;
        
        case 2:
          tamanhoEscala = sizeof(escalaPentatonicaMenor)/sizeof(byte);
          for(int i = 0; i > tamanhoEscala; i++) {
           escala[i] = escalaPentatonicaMenor[i];
          }
        break;  
        
        case 3:
          tamanhoEscala = sizeof(escalaHexafonica)/sizeof(byte);
          for(int i = 0; i > tamanhoEscala; i++) {
           escala[i] = escalaHexafonica[i];
          }
        break;
        
        case 4:
          tamanhoEscala = sizeof(escalaCromatica)/sizeof(byte);
          for(int i = 0; i > tamanhoEscala; i++) {
           escala[i] = escalaCromatica[i];
          }
        break;
    }
    numeroEscalaAnterior = numeroEscala;
  }
}

/*-------------------------------------------------------------------------------------------------------------
// so deve ser chamado uma vez, quando o encoder tiver sido modificado pois um for vai tomar tempo de execucao e pode atrasar o tempo
void ContadorNotasPadrao() {
  for (contadorNotas = 0; escala[contadorNotas] + tonalidade + (oitava*12) <= 119; oitava++) {
    for (int i=0; Escala[i] > (sizeof(Escala) -1); || escala[contadorNotas] + tonalidade + (oitava*12) = 119; i++) {
      contador++;
    }
  }
  Return NumeroNotas;
}
*/
