//Sükséges könyvtárak becsatolása
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//állandók definiálása

#define matrixmeret 100
#define generacioszam 30000000
#define kiiratas 10000
#define kezdetielelem 100
#define kiindulasi_energia 100
#define kooperalofogyasztas 10
#define csalofogyasztas 8
#define kooperaloosztodas_valoszinuseg 70 
#define csaloosztodas_valoszinuseg 70 
#define osztodasfeluliras 30
//Saját függvények deklarálása
//Tóruszba hajtás sorok szerint
int torusz(int koordinata){
	if(koordinata < 0){
		return(matrixmeret+koordinata);
	}
	else{
		if(koordinata >= matrixmeret){
			return(koordinata%matrixmeret);
		}
		else{
			return(koordinata);
		}
	}
}

//Négyzetre emelés

int ipow(int base, int exp) {
    static const int highest_bit_set[] = {
        0, 1, 2, 2, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 255, // anything past 63 is a guaranteed overflow with base > 1
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
    };

    int result = 1;

    switch (highest_bit_set[exp]) {
    case 255: // we use 255 as an overflow marker and return 0 on overflow/underflow
        if (base == 1) {
            return 1;
        }
        
        if (base == -1) {
            return 1 - 2 * (exp & 1);
        }
        
        return 0;
    case 6:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 5:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 4:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 3:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 2:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 1:
        if (exp & 1) result *= base;
    default:
        return result;
    }
}

//százalék generálás
float szazalek() {
	return(100*(rand()/(float)RAND_MAX));
}

//asszociált szomszédok megszámlálása
int asszocialt_szomszedszamlalas(int matrix[matrixmeret][matrixmeret], int sor, int oszlop){
	int sorszam, oszlopszam, szomszedok_szama =0-matrix[sor][oszlop];
	for(sorszam = -1; sorszam <= 1; sorszam++) {
		for(oszlopszam = -1; oszlopszam <= 1; oszlopszam++) {
			szomszedok_szama += matrix[torusz(sor+sorszam)][torusz(oszlop+oszlopszam)];
		}
	}
	return(szomszedok_szama);
}

//asszociált szomszédok megszámlálása átoltáskor
int asszocialt_szomszedszamlalas_atoltas(int matrix[matrixmeret][matrixmeret], int sor, int oszlop){
	int sorszam, oszlopszam, szomszedok_szama =0-matrix[sor][oszlop];
	for(sorszam = -1; sorszam <= 1; sorszam++) {
		for(oszlopszam = -1; oszlopszam <= 1; oszlopszam++) {
			szomszedok_szama += matrix[torusz(sor+sorszam)][torusz(oszlop+oszlopszam)];
		}
	}
	return(szomszedok_szama);
}

// szomszédok megszámlálása
int szomszedszamlalas(int matrix[matrixmeret][matrixmeret], int sor, int oszlop){
	int sorszam, oszlopszam, szomszedok_szama =0;
	for(sorszam = -1; sorszam <= 1; sorszam++) {
		for(oszlopszam = -1; oszlopszam <= 1; oszlopszam++) {
			if(matrix[torusz(sor+sorszam)][torusz(oszlop+oszlopszam)]){
				szomszedok_szama++;
			}
		}
	}
	if(matrix[sor][oszlop]){
		szomszedok_szama--;
	}
	return(szomszedok_szama);
}

//sejttér feltöltése
void kiindulas_feltoltes(int matrix[matrixmeret][matrixmeret],int matrix2[matrixmeret][matrixmeret],int elelemmatrix[matrixmeret][matrixmeret],int energiamatrix[matrixmeret][matrixmeret],int asszocialtmatrix[matrixmeret][matrixmeret],int asszocialtfokmatrix[matrixmeret][matrixmeret]){
	for(int sor=0;sor<matrixmeret;sor++){
		for(int oszlop=0;oszlop<matrixmeret;oszlop++){
			float deklaralas=szazalek();
			if(deklaralas<=1){
				matrix[sor][oszlop]=2;
			}
			else if(deklaralas>=99){
				matrix[sor][oszlop]=1;
			}
			else{
				matrix[sor][oszlop]=0;
			}
			matrix2[sor][oszlop]=0;
			if(matrix[sor][oszlop]){
				energiamatrix[sor][oszlop]=kiindulasi_energia;
			}
			else{
				energiamatrix[sor][oszlop]=0;
			}
			asszocialtmatrix[sor][oszlop]=0;
			asszocialtfokmatrix[sor][oszlop]=0;
			elelemmatrix[sor][oszlop]=kezdetielelem;
		}
	}
}
//sejtek a mátrix egészére megszámolása
void teljes_sejtszamolas(int matrix[matrixmeret][matrixmeret],int *osszsejtszam,int *osszkooperalo,int *osszcsalo){
	for(int sor=0;sor<matrixmeret;sor++){
		for(int oszlop=0;oszlop<matrixmeret;oszlop++){
			if(matrix[sor][oszlop]==1){
				(*osszsejtszam)++;
				(*osszkooperalo)++;
			}
			if(matrix[sor][oszlop]==2){
				(*osszsejtszam)++;
				(*osszcsalo)++;
			}
		}
	}
}
int koordinatageneralas(){
	int koordinata=rand()%matrixmeret;
	return(koordinata);
}
void frissites(int *osszsejtszam,int *sejttípus){
	(*osszsejtszam)--;
	(*sejttípus)--;
}


//A sejtek osztódásának iránya
int* irany_onmaga_nelkul(int sorszam, int oszlopszam, int* kimenetikoordinata) {
	int irany = rand()%8;
	int celsor, celoszlop;
	switch(irany){
		case 0:  { celsor = sorszam;   celoszlop = oszlopszam-1; break; }
		case 1:  { celsor = sorszam+1; celoszlop = oszlopszam-1; break; }
		case 2:  { celsor = sorszam+1; celoszlop = oszlopszam;    break; }
		case 3:  { celsor = sorszam+1; celoszlop = oszlopszam+1; break; }
		case 4:  { celsor = sorszam;   celoszlop = oszlopszam+1; break; }
		case 5:  { celsor = sorszam-1; celoszlop = oszlopszam+1; break; }
		case 6:  { celsor = sorszam-1; celoszlop = oszlopszam;    break; }
		case 7:  { celsor = sorszam-1; celoszlop = oszlopszam-1; break; }
		default: { printf("Hiba van az iranygeneralasban!\n"); break; }
	}
	kimenetikoordinata[0] = torusz(celsor);
	kimenetikoordinata[1] = torusz(celoszlop);
	return(kimenetikoordinata);
}

//A sejtek mozgásának iránya
int* irany_mozgas(int sorszam, int oszlopszam, int* kimenetikoordinata) {
	int irany = rand()%9;
	int celsor, celoszlop;
	switch(irany){
		case 0:  { celsor = sorszam;   celoszlop = oszlopszam; break; }
		case 1:  { celsor = sorszam;   celoszlop = oszlopszam-1; break; }
		case 2:  { celsor = sorszam+1; celoszlop = oszlopszam-1; break; }
		case 3:  { celsor = sorszam+1; celoszlop = oszlopszam;    break; }
		case 4:  { celsor = sorszam+1; celoszlop = oszlopszam+1; break; }
		case 5:  { celsor = sorszam;   celoszlop = oszlopszam+1; break; }
		case 6:  { celsor = sorszam-1; celoszlop = oszlopszam+1; break; }
		case 7:  { celsor = sorszam-1; celoszlop = oszlopszam;    break; }
		case 8:  { celsor = sorszam-1; celoszlop = oszlopszam-1; break; }
		default: { printf("Hiba van az iranygeneralasban!\n"); break; }
	}
	kimenetikoordinata[0] = torusz(celsor);
	kimenetikoordinata[1] = torusz(celoszlop);
	return(kimenetikoordinata);
}


int main(int argc, char **argv){
//kimeneti fájlok megnyitása
	FILE *generaciok;
	FILE *tulajdonsagok;
	FILE *diagrammok;
	FILE *statisztika;
	FILE *atoltas_statisztika;
	FILE *debug;
	FILE *asszociacio_debug;
	FILE *asszocialtfok;
	FILE *elelem;
	FILE *atoltasok_kozti_ido_file;
	
	generaciok = fopen("generaciok.dat", "w");
	tulajdonsagok=fopen("tulajdonsagok.dat", "w");
	diagrammok=fopen("diagrammok.dat", "w");
	statisztika=fopen("statisztika.dat", "w");
	atoltas_statisztika=fopen("atoltas_statisztika.dat", "w");
	debug = fopen("debug.dat", "w");
	asszociacio_debug = fopen("asszociacio_debug.dat", "w");
	asszocialtfok = fopen("asszocialtfok.dat", "w");
	elelem = fopen("elelem.dat", "w");
	atoltasok_kozti_ido_file = fopen("atoltasok_kozti_ido.dat", "w");
	
	fprintf(statisztika,"ido,kooperalo,csalo\n");
	fprintf(atoltas_statisztika,"atoltas_sorszama,osztodasok_szama\n");
	fprintf(atoltasok_kozti_ido_file,"ket_atoltas_kozt_eltelt_ido\n");
	//megjegyzes: A tulajdonsagok fileben az elso sorban talalhato a matrix merete es a randomszam generator seedje.
	//deklarálom a változókat
	int atoltasmintazat=0;
	int seed=time(NULL);
	float kooperator_atoltas_szazalek=0.0;
	float asszocialasiertek=70,disszocialasiertek=30;
	int predacioerosseg=5;
	float kiind_koop_arany=50;
	if(argc>1){
		atoltasmintazat=atoi(argv[1]);
		seed=atoi(argv[2]);
		asszocialasiertek=atof(argv[3]);
		disszocialasiertek=atof(argv[4]);
		predacioerosseg=atoi(argv[5]);
		kiind_koop_arany=atof(argv[6]);
	}
	srand(seed);

	//kiiratással kapcsolatos változók
	//sejttér kiiratása
	int sejtter=0;
	int predacio=1;
	int menet_kozbeni_kiiratas=1;

	int randsor,randoszlop,ido;
	int matrix[matrixmeret][matrixmeret];
	int matrix2[matrixmeret][matrixmeret];
	int atoltassor,atoltasoszlop;
	int atoltaskezdes;
	int energiamatrix[matrixmeret][matrixmeret];
	int elelemmatrix[matrixmeret][matrixmeret];
	int celirany[2]={0,0};
	int Ujsor,Ujoszlop;
	int asszocialtmatrix[matrixmeret][matrixmeret],asszocialtsag=0,asszocialas;
	float asszocialasivaloszinuseg;
	float sporaarany;
	float deklaralas;
	int sor,oszlop,sor1,oszlop1;
	int osszelelem=0,osszsejtszam=0,osszcsalo=0,osszkooperalo=0,sporakooperalo=0,sporacsalo=0;
	int asszocialt_szomszedszam;
	int sporaciklus;
	int kiiratasertek[4];
	int asszocialtfokmatrix[matrixmeret][matrixmeret],legmagasabb_asszocialtsag=0,legmagasabb_sorok[matrixmeret*matrixmeret],legmagasabb_rosszak[matrixmeret*matrixmeret],oszrossz,legmagasabb_oszlopok[matrixmeret*matrixmeret],legmagasabb_koordinata=0,legmagasabb_koordinata_max;
	int atoltas_sorszama=0,osztodasok_szama=0;
	int atoltas_kozti_ido=0;
	
	//A szétszórós góckereső átoltás változói
	int ujsor;
	int ujoszlop;
	int ures_e=0;
	
	//Térbeliség elhanyagoló átoltás változói
	int sporasejtek;
	
	//Kiindulási mátrixok feltöltése
	for(sor=0;sor<matrixmeret;sor++){
		for(oszlop=0;oszlop<matrixmeret;oszlop++){
			//Annak eldöntése, hogy egy cellában legyen-e sejt
			deklaralas=szazalek();
			//Csak ezekben az esetekben lesz sejt a cellában
			if((deklaralas<=1)||(deklaralas>=99)){
				//Annak eldöntése, hogy csaló, vagy kooperáló legyen az adott sejt
				if(szazalek()<kiind_koop_arany){
					matrix[sor][oszlop]=1;
				}
				else{
					matrix[sor][oszlop]=2;
				}
			}
			//Ebben az esetben üres lesz az adott cella
			else{
				matrix[sor][oszlop]=0;
			}
			matrix2[sor][oszlop]=0;
			if(matrix[sor][oszlop]){
				energiamatrix[sor][oszlop]=kiindulasi_energia;
			}
			else{
				energiamatrix[sor][oszlop]=0;
			}
			asszocialtmatrix[sor][oszlop]=0;
			asszocialtfokmatrix[sor][oszlop]=0;
			elelemmatrix[sor][oszlop]=kezdetielelem;
		}
	}
	//A diagrammot tartalmazó adatatok táblázatának oszlopainak elnevezése
	fprintf(diagrammok,"ido,osszkooperalo,osszcsalo,osszelelem\n");
	//Az adott szimuláció tulajdonságainak kiírása
	fprintf(tulajdonsagok, "%d,%d,%d,%d\n", matrixmeret,seed,generacioszam,kiiratas);
	//megnézem mennyi élelem és sejt van a mátrixban,így létrehozom az összesített adatokat.
	osszelelem=kezdetielelem*matrixmeret*matrixmeret;
	teljes_sejtszamolas(matrix,&osszsejtszam,&osszkooperalo,&osszcsalo);
	//mátrix frissítése
	for(ido=0;ido<generacioszam;ido++){
		randsor=koordinatageneralas();
		randoszlop=koordinatageneralas();

		//Kooperáló sejtek frissítése
		if(matrix[randsor][randoszlop]==1){
			//energiafogyasztás
			energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]-kooperalofogyasztas;
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				frissites(&osszsejtszam,&osszkooperalo);
				matrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
			}
			//A sejt a táplálkozása során növelni tudja saját energiatartalmát
			if((matrix[randsor][randoszlop])&&(elelemmatrix[randsor][randoszlop]>0)){
				elelemmatrix[randsor][randoszlop]=elelemmatrix[randsor][randoszlop]-1;
				energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]+38;
				//Összesített adatok frissítése
				osszelelem=osszelelem-1;
			}
			/*A sejt osztódásának szimulálása
			* A sejt akkor osztódik, ha energiaértéke elér egy bizonyos értéket
			* Az osztódás során az útódsejtekbe a kiindulási energia fele kerül
			* Osztódás irányai:
			* [7][6][5]
			* [0][-][4]
			* [1][2][3]
			*/
		 
			//Kooperáló sejtek osztódása
			if((energiamatrix[randsor][randoszlop]>190)&&(szazalek()<kooperaloosztodas_valoszinuseg)){
				irany_onmaga_nelkul(randsor,randoszlop,celirany);
				Ujsor=celirany[0];
				Ujoszlop=celirany[1];
				if((matrix[Ujsor][Ujoszlop]==0)||(szazalek()<osztodasfeluliras)){
					//Összesített adatok frissítése
					if(matrix[Ujsor][Ujoszlop]==1){
						frissites(&osszsejtszam,&osszkooperalo);
					}
					if(matrix[Ujsor][Ujoszlop]==2){
						frissites(&osszsejtszam,&osszcsalo);
					}
					osztodasok_szama++;
					matrix[Ujsor][Ujoszlop]=matrix[randsor][randoszlop];
					energiamatrix[Ujsor][Ujoszlop]=energiamatrix[randsor][randoszlop]/2;
					energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]/2;
					asszocialtmatrix[Ujsor][Ujoszlop]=0;
					//Összesített adatok frissítése
					osszsejtszam++;
					osszkooperalo++;
					//Leelenőrzöm, hogy az utódsejtnek van-e elég energiája a túléléshez, ha nem, akkor meghal
					if((matrix[Ujsor][Ujoszlop])&&(energiamatrix[Ujsor][Ujoszlop]<1)){
						matrix[Ujsor][Ujoszlop]=0;
						energiamatrix[Ujsor][Ujoszlop]=0;
						asszocialtmatrix[Ujsor][Ujoszlop]=0;
						frissites(&osszsejtszam,&osszkooperalo);
					}
					//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a kooperáló sejtek osztódása után, és ha igen, akkor eltávolítom
					sor=torusz(Ujsor-1);
					oszlop=torusz(Ujoszlop-1);
					for(sor1=0;sor1<3;sor1++){
						for(oszlop1=0;oszlop1<3;oszlop1++){
							if(asszocialtmatrix[sor][oszlop]){
								if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
									asszocialtmatrix[sor][oszlop]=0;
								}
							}
							oszlop++;
							oszlop=torusz(oszlop);
						}
						sor++;
						sor=torusz(sor);
						oszlop=torusz(Ujoszlop-1);
					}
				}
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszkooperalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
			
			//Sejtek asszociációjának szimulalasa
			if((matrix[randsor][randoszlop])&&(szomszedszamlalas(matrix,randsor,randoszlop)>0)){
				asszocialasivaloszinuseg=szazalek();
				asszocialas=0;
				if((asszocialasivaloszinuseg<asszocialasiertek)&&(matrix[randsor][randoszlop]==1)){
					asszocialas=1;
				}
				if(asszocialas){
					while(asszocialtsag==0){
						irany_onmaga_nelkul(randsor,randoszlop,celirany);
						Ujsor=celirany[0];
						Ujoszlop=celirany[1];
						if((matrix[Ujsor][Ujoszlop]>0)){
							asszocialtmatrix[Ujsor][Ujoszlop]=1;
							asszocialtmatrix[randsor][randoszlop]=1;
							asszocialtsag++;
						}
					}
				}
			}
			//Sejtek disszocialasanak szimulalasa
			if((asszocialtmatrix[randsor][randoszlop])&&(asszocialas==0)){
				if((szazalek()<disszocialasiertek)&&(matrix[randsor][randoszlop])){
					asszocialtmatrix[randsor][randoszlop]=0;
				}
			}
			//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a disszociálás után, és ha igen, akkor eltávolítom
			sor=torusz(randsor-1);
			oszlop=torusz(randoszlop-1);
			for(sor1=0;sor1<3;sor1++){
				for(oszlop1=0;oszlop1<3;oszlop1++){
					if(asszocialtmatrix[sor][oszlop]){
						if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
							asszocialtmatrix[sor][oszlop]=0;
						}
					}
					oszlop++;
					oszlop=torusz(oszlop);
				}
				sor++;
				sor=torusz(sor);
				oszlop=torusz(randoszlop-1);
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszkooperalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
			//A predáció szimulálása
			if(predacio){
				//Ha a sejt asszociált, akkor az asszocialt szomszédok száma fogja eldönteni, hogy mennyire kitett az adott sejt a predációnak
				if((matrix[randsor][randoszlop])){
					//Megszámolom a sejtek asszociált szomszédainak a számát
					if(asszocialtmatrix[randsor][randoszlop]==0){
						asszocialt_szomszedszam=0;
					}
					else{
						asszocialt_szomszedszam=asszocialt_szomszedszamlalas(asszocialtmatrix,randsor,randoszlop);
						}
					//A predáció a szomszédszám függvényében hat a sejtre
					if((rand()/(float)RAND_MAX)<(float)1/ipow(asszocialt_szomszedszam+2,predacioerosseg)){
						//Összesített adatok frissítése
						frissites(&osszsejtszam,&osszkooperalo);
						matrix[randsor][randoszlop]=0;
						energiamatrix[randsor][randoszlop]=0;
						asszocialtmatrix[randsor][randoszlop]=0;
					}
				}
			}
			//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a predáció után, és ha igen, akkor eltávolítom
			sor=torusz(randsor-1);
			oszlop=torusz(randoszlop-1);
			for(sor1=0;sor1<3;sor1++){
				for(oszlop1=0;oszlop1<3;oszlop1++){
					if(asszocialtmatrix[sor][oszlop]){
						if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
							asszocialtmatrix[sor][oszlop]=0;
						}
					}
					oszlop++;
					oszlop=torusz(oszlop);
				}
				sor++;
				sor=torusz(sor);
				oszlop=torusz(randoszlop-1);
			}
			/*A sejt mozgásának szimulálása
			* A sejt akkor mozog, ha a mozgas változó nem 0.
			* Az irányok:
			* [8][7][6]
			* [1][0][5]
			* [2][3][4]
			*/
			irany_mozgas(randsor,randoszlop,celirany);
			Ujsor=celirany[0];
			Ujoszlop=celirany[1];
			if((matrix[Ujsor][Ujoszlop]==0)&&(matrix[randsor][randoszlop])&&(asszocialtmatrix[randsor][randoszlop]==0)){
				matrix[Ujsor][Ujoszlop]=matrix[randsor][randoszlop];
				energiamatrix[Ujsor][Ujoszlop]=energiamatrix[randsor][randoszlop]-8;
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
				if((matrix[Ujsor][Ujoszlop])&&(energiamatrix[Ujsor][Ujoszlop]<1)){
					frissites(&osszsejtszam,&osszkooperalo);
					matrix[Ujsor][Ujoszlop]=0;
					energiamatrix[Ujsor][Ujoszlop]=0;
				}
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszkooperalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
		}
		//Csaló sejtek frissítése
		if(matrix[randsor][randoszlop]==2){
			//energiafogyasztás
			energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]-csalofogyasztas;
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				frissites(&osszsejtszam,&osszcsalo);
				matrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
			}
			//A sejt a táplálkozása során növelni tudja saját energiatartalmát
			if((matrix[randsor][randoszlop])&&(elelemmatrix[randsor][randoszlop]>0)){
				elelemmatrix[randsor][randoszlop]=elelemmatrix[randsor][randoszlop]-1;
				energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]+38;
				//Összesített adatok frissítése
				osszelelem=osszelelem-1;
			}
			/*A sejt osztódásának szimulálása
			* A sejt akkor osztódik, ha energiaértéke elér egy bizonyos értéket
			* Az osztódás során az útódsejtekbe a kiindulási energia fele kerül
			* Osztódás irányai:
			* [7][6][5]
			* [0][-][4]
			* [1][2][3]
			*/
		 
			//Csaló sejtek osztódása
			if((energiamatrix[randsor][randoszlop]>190)&&(szazalek()<csaloosztodas_valoszinuseg)){
				irany_onmaga_nelkul(randsor,randoszlop,celirany);
				Ujsor=celirany[0];
				Ujoszlop=celirany[1];
				if((matrix[Ujsor][Ujoszlop]==0)||(szazalek()<osztodasfeluliras)){
					//Összesített adatok frissítése
					if(matrix[Ujsor][Ujoszlop]==1){
						frissites(&osszsejtszam,&osszkooperalo);
					}
					if(matrix[Ujsor][Ujoszlop]==2){
						frissites(&osszsejtszam,&osszcsalo);
					}
					osztodasok_szama++;
					matrix[Ujsor][Ujoszlop]=matrix[randsor][randoszlop];
					energiamatrix[Ujsor][Ujoszlop]=energiamatrix[randsor][randoszlop]/2;
					energiamatrix[randsor][randoszlop]=energiamatrix[randsor][randoszlop]/2;
					asszocialtmatrix[Ujsor][Ujoszlop]=0;
					//Összesített adatok frissítése
					osszsejtszam++;
					osszcsalo++;
					//Leelenőrzöm, hogy az utódsejtnek van-e elég energiája a túléléshez, ha nem, akkor meghal
					if((matrix[Ujsor][Ujoszlop])&&(energiamatrix[Ujsor][Ujoszlop]<1)){
						matrix[Ujsor][Ujoszlop]=0;
						energiamatrix[Ujsor][Ujoszlop]=0;
						asszocialtmatrix[Ujsor][Ujoszlop]=0;
						frissites(&osszsejtszam,&osszcsalo);
					}
					//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a csaló sejtek osztódása után, és ha igen, akkor eltávolítom
					sor=torusz(Ujsor-1);
					oszlop=torusz(Ujoszlop-1);
					for(sor1=0;sor1<3;sor1++){
						for(oszlop1=0;oszlop1<3;oszlop1++){
							if(asszocialtmatrix[sor][oszlop]){
								if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
									asszocialtmatrix[sor][oszlop]=0;
								}
							}
							oszlop++;
							oszlop=torusz(oszlop);
						}
						sor++;
						sor=torusz(sor);
						oszlop=torusz(Ujoszlop-1);
					}
				}
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszcsalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
			
			//Sejtek disszocialasanak szimulalasa
			if((asszocialtmatrix[randsor][randoszlop])){
				if((szazalek()<disszocialasiertek)&&(matrix[randsor][randoszlop])){
					asszocialtmatrix[randsor][randoszlop]=0;
				}
			}
			//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a predáció után, és ha igen, akkor eltávolítom
			sor=torusz(randsor-1);
			oszlop=torusz(randoszlop-1);
			for(sor1=0;sor1<3;sor1++){
				for(oszlop1=0;oszlop1<3;oszlop1++){
					if(asszocialtmatrix[sor][oszlop]){
						if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
							asszocialtmatrix[sor][oszlop]=0;
						}
					}
					oszlop++;
					oszlop=torusz(oszlop);
				}
				sor++;
				sor=torusz(sor);
				oszlop=torusz(randoszlop-1);
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszcsalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
			//A predáció szimulálása
			if(predacio){
				//Ha a sejt asszociált, akkor az asszocialt szomszédok száma fogja eldönteni, hogy mennyire kitett az adott sejt a predációnak
				if((matrix[randsor][randoszlop])){
					//Megszámolom a sejtek asszociált szomszédainak a számát
					if(asszocialtmatrix[randsor][randoszlop]==0){
						asszocialt_szomszedszam=0;
					}
					else{
						asszocialt_szomszedszam=asszocialt_szomszedszamlalas(asszocialtmatrix,randsor,randoszlop);
					}
					//A predáció a szomszédszám függvényében hat a sejtre
					if((rand()/(float)RAND_MAX)<(float)1/ipow(asszocialt_szomszedszam+2,predacioerosseg)){
						//Összesített adatok frissítése
						frissites(&osszsejtszam,&osszcsalo);
						matrix[randsor][randoszlop]=0;
						energiamatrix[randsor][randoszlop]=0;
						asszocialtmatrix[randsor][randoszlop]=0;
					}
				}
			}
			//Leelenőrzöm, hogy marad-e magányos asszociált sejt a mátrixba a predáció után, és ha igen, akkor eltávolítom
			sor=torusz(randsor-1);
			oszlop=torusz(randoszlop-1);
			for(sor1=0;sor1<3;sor1++){
				for(oszlop1=0;oszlop1<3;oszlop1++){
					if(asszocialtmatrix[sor][oszlop]){
						if(asszocialt_szomszedszamlalas(asszocialtmatrix,sor,oszlop)<1){
							asszocialtmatrix[sor][oszlop]=0;
						}
					}
					oszlop++;
					oszlop=torusz(oszlop);
				}
				sor++;
				sor=torusz(sor);
				oszlop=torusz(randoszlop-1);
			}
			/*A sejt mozgásának szimulálása
			* A sejt akkor mozog, ha a mozgas változó nem 0.
			* Az irányok:
			* [8][7][6]
			* [1][0][5]
			* [2][3][4]
			*/
			irany_mozgas(randsor,randoszlop,celirany);
			Ujsor=celirany[0];
			Ujoszlop=celirany[1];
			if((matrix[Ujsor][Ujoszlop]==0)&&(matrix[randsor][randoszlop])&&(asszocialtmatrix[randsor][randoszlop]==0)){
				matrix[Ujsor][Ujoszlop]=matrix[randsor][randoszlop];
				energiamatrix[Ujsor][Ujoszlop]=energiamatrix[randsor][randoszlop]-8;
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
				if((matrix[Ujsor][Ujoszlop])&&(energiamatrix[Ujsor][Ujoszlop]<1)){
					frissites(&osszsejtszam,&osszcsalo);
					matrix[Ujsor][Ujoszlop]=0;
					energiamatrix[Ujsor][Ujoszlop]=0;
				}
			}
			//Ha a sejt energiaszintje eléri a 0-t, akkor meghal
			if((matrix[randsor][randoszlop])&&(energiamatrix[randsor][randoszlop]<1)){
				//Összesített adatok frissítése
				frissites(&osszsejtszam,&osszcsalo);
				matrix[randsor][randoszlop]=0;
				energiamatrix[randsor][randoszlop]=0;
				asszocialtmatrix[randsor][randoszlop]=0;
			}
		}
		//Élelempótlás átoltás nélkül
		if((atoltasmintazat==0)&&(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))){
			
			//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
			fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
			atoltas_kozti_ido=0;
			
			osszelelem=kezdetielelem*matrixmeret*matrixmeret;
			for(sor=0;sor<matrixmeret;sor++){
				for(oszlop=0;oszlop<matrixmeret;oszlop++){
					elelemmatrix[sor][oszlop]=kezdetielelem;
				}
			}
			fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
			atoltas_sorszama++;
			osztodasok_szama=0;
		}
		//A rossz időszak szimulálása, ha nem hanyagoljuk el a mintázatot, az átoltás helyét pedig véletlenszerűen választom ki.
		if(atoltasmintazat==1){
			//A spóraképzés akkor indul meg, ha az élelemmenyiség az eredeti élelemmenyiség 40%-a alá esik, és még vannak kooperáló sejtek és 10 nél több sejt van összesen .
			if((osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))&&(osszkooperalo)&&(osszsejtszam>10)){
				
				//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
				fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
				atoltas_kozti_ido=0;
				
				//A spórában megtalálható a mintázat
				//A spórában mindenképen kell, hogy legyen sejt
				//Kiválasztom az átoltás helyét véletlenszerűen. A teljes mátrix 10%-át fogom átoltani.
				sporaciklus=0;
				while(sporaciklus<10){
					sporaciklus=0;
					atoltassor=rand()%matrixmeret;
					atoltasoszlop=rand()%matrixmeret;
					atoltaskezdes=atoltasoszlop;
					for(sor=0;sor<10;sor++){
						for(oszlop=0;oszlop<10;oszlop++){
							atoltassor=torusz(atoltassor);
							atoltasoszlop=torusz(atoltasoszlop);
							matrix2[atoltassor][atoltasoszlop]=matrix[atoltassor][atoltasoszlop];
							if(matrix2[atoltassor][atoltasoszlop]){
								sporaciklus++;
							}
							atoltasoszlop++;
						}//oszlop
						atoltassor++;
						atoltasoszlop=atoltaskezdes;
					}//sor
					if(sporaciklus<10){
						for(sor=0;sor<matrixmeret;sor++){
							for(oszlop=0;oszlop<matrixmeret;oszlop++){
								matrix2[atoltassor][atoltasoszlop]=matrix[atoltassor][atoltasoszlop];
							}//oszlop
						}//sor
					}//if
				}//while
				//A sejt, energia, asszocialt és élelemmátrix kiürítése (új hely létrehozása)
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=0;
						elelemmatrix[sor][oszlop]=kezdetielelem;
						energiamatrix[sor][oszlop]=0;
						asszocialtmatrix[sor][oszlop]=0;
					}
				}
				//Az új helyre kerülés szimulálása
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=matrix2[sor][oszlop];
						//kiürítem az átoltó mátrixot.
						matrix2[sor][oszlop]=0;
						//feltöltöm energiával a sejteket.
						if(matrix[sor][oszlop]){
							energiamatrix[sor][oszlop]=100;
						}
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
				//Összesített adatok frissítése
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				osszsejtszam=0;
				osszkooperalo=0;
				osszcsalo=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix[sor][oszlop]==1){
							osszsejtszam++;
							osszkooperalo++;
						}
						if(matrix[sor][oszlop]==2){
							osszsejtszam++;
							osszcsalo++;
						}
					}
				}
				atoltas_sorszama++;
				fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
				osztodasok_szama=0;
			}
			//Élelempótlás, ha elfogy valamelyik típus
			if(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4)){
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
			}
		}
		//A rossz időszak szimulálása, ha a góckereső átoltást használunk, és a sejtek egy tömbbe vannak az új élőhelyen.
		if(atoltasmintazat==2){
			//A spóraképzés akkor indul meg, ha az élelemmenyiség az eredeti élelemmenyiség 40%-a alá esik, és még vannak kooperáló sejtek
			if((osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))&&(osszkooperalo)){
				
				//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
				fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
				atoltas_kozti_ido=0;
				
				//Lenullázom az átoltással kapcsolatos tömböket
				for(legmagasabb_koordinata=0;legmagasabb_koordinata<(matrixmeret*matrixmeret);legmagasabb_koordinata++){
						legmagasabb_sorok[legmagasabb_koordinata]=0;
						legmagasabb_oszlopok[legmagasabb_koordinata]=0;
						legmagasabb_rosszak[legmagasabb_koordinata]=0;
				}
				//Megnézem a sejtek asszociáltsági fokait, és melyik a legnagyobb asszociáltsági fok
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						//Az asszocialt szomszédok számának megnézése
						if(asszocialtmatrix[sor][oszlop]){
							asszocialtfokmatrix[sor][oszlop]=asszocialt_szomszedszamlalas_atoltas(asszocialtmatrix,sor,oszlop);
						}
						else{
							asszocialtfokmatrix[sor][oszlop]=0;
						}
						if(asszocialtfokmatrix[sor][oszlop]>legmagasabb_asszocialtsag){
							legmagasabb_asszocialtsag=asszocialtfokmatrix[sor][oszlop];
						}
					}
				}
				//Mégegyszer végigmegyek az asszociáltsági fokokat tartalmazó mátrixon, és elmentem a legnagyobb asszociáltsági fokokat tartalmazó sejtek koordinátáit.
				legmagasabb_koordinata=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(asszocialtfokmatrix[sor][oszlop]>=legmagasabb_asszocialtsag){
							legmagasabb_sorok[legmagasabb_koordinata]=sor;
							legmagasabb_oszlopok[legmagasabb_koordinata]=oszlop;
							legmagasabb_koordinata++;
						}
					}
				}
				//Elmentem mennyi gócpont van
				legmagasabb_koordinata_max=legmagasabb_koordinata;
				sporaciklus=0;
				//kiválasztom a gócpontot, amit át fogok oltani. Addig választok, amíg a spórában találhatók csaló és kooperáló sejtek
				while(sporaciklus<1){
					legmagasabb_koordinata=rand()%legmagasabb_koordinata_max;
					if(legmagasabb_rosszak[legmagasabb_koordinata]==0){
						//Végrehajtom az átoltást. A teljes mátrix 10%-át fogom átoltani.
						atoltassor=torusz((legmagasabb_sorok[legmagasabb_koordinata])-(matrixmeret/100*5));
						atoltasoszlop=torusz((legmagasabb_oszlopok[legmagasabb_koordinata])-(matrixmeret/100*5));
						atoltaskezdes=atoltasoszlop;
						for(sor=0;sor<10;sor++){
							for(oszlop=0;oszlop<10;oszlop++){
								atoltassor=torusz(atoltassor);
								atoltasoszlop=torusz(atoltasoszlop);
								matrix2[atoltassor][atoltasoszlop]=matrix[atoltassor][atoltasoszlop];
								if(matrix[atoltassor][atoltasoszlop]==1){
									sporakooperalo++;
								}
								if(matrix[atoltassor][atoltasoszlop]==2){
									sporacsalo++;
								}
								atoltasoszlop++;
							}
							atoltassor++;
							atoltasoszlop=atoltaskezdes;
						}
						sporaarany=(float)sporakooperalo/(sporacsalo+sporakooperalo);
						if(sporaarany<kooperator_atoltas_szazalek){
							sporacsalo=0;
							sporakooperalo=0;
							legmagasabb_rosszak[legmagasabb_koordinata]=1;
							for(sor=0;sor<matrixmeret;sor++){
								for(oszlop=0;oszlop<matrixmeret;oszlop++){
									matrix2[sor][oszlop]=0;
								}
							}
						}
						else{
							sporaciklus=1;
						}
					}
					//Ha az összes átoltási góc rossz, leállítom a szimulációt.
					for(legmagasabb_koordinata=0;legmagasabb_koordinata<=legmagasabb_koordinata_max;legmagasabb_koordinata++){
						oszrossz=oszrossz+legmagasabb_rosszak[legmagasabb_koordinata];
					}
					if(oszrossz==legmagasabb_koordinata_max){
						kiiratasertek[0]=ido;
						ido=generacioszam;
						printf("\nVege a szimulacionak, mivel nem volt megfelo goc, amit at lehetett volna oltani");
						fclose(tulajdonsagok);
						FILE *tulajdonsagok;
						tulajdonsagok=fopen("tulajdonsagok.dat", "w");
						fprintf(tulajdonsagok, "%d,%d,%d,%d\n", matrixmeret,seed,kiiratasertek[0],kiiratas);
						sporaciklus=1;
					}
					oszrossz=0;
				}
				//A sejt, energia, asszocialt és élelemmátrix kiürítése (új hely létrehozása)
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=0;
						elelemmatrix[sor][oszlop]=kezdetielelem;
						energiamatrix[sor][oszlop]=0;
						asszocialtmatrix[sor][oszlop]=0;
						asszocialtfokmatrix[sor][oszlop]=0;
					}
				}
				for(legmagasabb_koordinata=0;legmagasabb_koordinata<=matrixmeret*matrixmeret;legmagasabb_koordinata++){
					legmagasabb_sorok[legmagasabb_koordinata]=0;
					legmagasabb_oszlopok[legmagasabb_koordinata]=0;
					legmagasabb_rosszak[legmagasabb_koordinata]=0;
				}
				//Az új helyre kerülés szimulálása
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=matrix2[sor][oszlop];
						//kiürítem az átoltó mátrixot.
						matrix2[sor][oszlop]=0;
						//feltöltöm energiával a sejteket.
						if(matrix[sor][oszlop]){
							energiamatrix[sor][oszlop]=100;
						}
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
				//Összesített adatok frissítése
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				osszsejtszam=0;
				osszkooperalo=0;
				osszcsalo=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix[sor][oszlop]==1){
							osszsejtszam++;
							osszkooperalo++;
						}
						if(matrix[sor][oszlop]==2){
							osszsejtszam++;
							osszcsalo++;
						}
					}
				}
				atoltas_sorszama++;
				fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
				osztodasok_szama=0;
			}
			//Élelempótlás, ha elfogy valamelyik típus
			if(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4)){
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
			}
		}
		//A rossz időszak szimulálása, ha a spórákat véletlenszerűen egy helyre gyűjtjük.
		//A spóraképzés akkor indul meg, ha az élelemmenyiség az eredeti élelemmenyiség 40%-a alá esik, és még vannak kooperáló sejtek
		if(atoltasmintazat==3){
			if((osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))&&(osszkooperalo)){
				
				//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
				fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
				atoltas_kozti_ido=0;
				
				//Változók meghatározása
				//Spóra mérete
				if(osszsejtszam>matrixmeret/10*matrixmeret/10){
					sporasejtek=matrixmeret/10*matrixmeret/10;
				}
				else{
					sporasejtek=osszsejtszam;
				}
				//Spórát alkotó sejtek átírása egy ideiglenes mátrixba
				while(sporasejtek){
					atoltassor=rand()%matrixmeret;
					atoltasoszlop=rand()%matrixmeret;
					if(matrix[atoltassor][atoltasoszlop]){
						matrix2[atoltassor][atoltasoszlop]=matrix[atoltassor][atoltasoszlop];
						matrix[atoltassor][atoltasoszlop]=0;
						sporasejtek--;
					}
				}
				//A sejt, energia, asszocialt és élelemmátrix kiürítése (új hely létrehozása)
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=0;
						elelemmatrix[sor][oszlop]=kezdetielelem;
						energiamatrix[sor][oszlop]=0;
						asszocialtmatrix[sor][oszlop]=0;
						asszocialtfokmatrix[sor][oszlop]=0;
					}
				}
				//A spórát alkotó sejtek visszahelyezése a kiindulási mátrixba
				//Itt a sor és az oszlop változó az ideiglenes mátrixba mozog, az atoltassor és atoltasoszlop pedig az új mátrixba mozog
				atoltassor=0;
				atoltasoszlop=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix2[sor][oszlop]){
							matrix[atoltassor][atoltasoszlop]=matrix2[sor][oszlop];
							//kiürítem az átoltó mátrixot.
							matrix2[sor][oszlop]=0;
							//feltöltöm energiával a sejteket.
							energiamatrix[atoltassor][atoltasoszlop]=100;
							atoltasoszlop++;
							if(atoltasoszlop==matrixmeret/10){
								atoltassor++;
								atoltasoszlop=0;
							}
						}

							
					}
				}
				//Összesített adatok frissítése
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				osszsejtszam=0;
				osszkooperalo=0;
				osszcsalo=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix[sor][oszlop]==1){
							osszsejtszam++;
							osszkooperalo++;
						}
						if(matrix[sor][oszlop]==2){
							osszsejtszam++;
							osszcsalo++;
						}
					}
				}
				atoltas_sorszama++;
				fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
				osztodasok_szama=0;
			}
			//Élelempótlás, ha elfogy valamelyik típus
			if(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4)){
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
			}
		}
		//A rossz időszak szimulálása, ha teljesen elhanyagoljuk a térbeliséget.
		//A spóraképzés akkor indul meg, ha az élelemmenyiség az eredeti élelemmenyiség 40%-a alá esik, és még vannak kooperáló sejtek
		if(atoltasmintazat==4){
			if((osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))&&(osszkooperalo)){
				
				//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
				fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
				atoltas_kozti_ido=0;
				
				//Változók meghatározása
				//Spóra mérete
				if(osszsejtszam>matrixmeret/10*matrixmeret/10){
					sporasejtek=matrixmeret/10*matrixmeret/10;
					}
				else{
					sporasejtek=osszsejtszam;
				}
				//Spórát alkotó sejtek átírása egy ideiglenes mátrixba
				while(sporasejtek){
					atoltassor=rand()%matrixmeret;
					atoltasoszlop=rand()%matrixmeret;
					if(matrix[atoltassor][atoltasoszlop]){
						matrix2[atoltassor][atoltasoszlop]=matrix[atoltassor][atoltasoszlop];
						matrix[atoltassor][atoltasoszlop]=0;
						sporasejtek--;
					}
				}
				//A sejt, energia, asszocialt és élelemmátrix kiürítése (új hely létrehozása)
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=0;
						elelemmatrix[sor][oszlop]=kezdetielelem;
						energiamatrix[sor][oszlop]=0;
						asszocialtmatrix[sor][oszlop]=0;
						asszocialtfokmatrix[sor][oszlop]=0;
					}
				}
				//A spórát alkotó sejtek visszahelyezése a kiindulási mátrixba
				//Itt a sor és az oszlop változó az ideiglenes mátrixba mozog, az atoltassor és atoltasoszlop pedig az új mátrixba mozog
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix2[sor][oszlop]){
							matrix[sor][oszlop]=matrix2[sor][oszlop];
							//kiürítem az átoltó mátrixot.
							matrix2[sor][oszlop]=0;
							//feltöltöm energiával a sejteket.
							energiamatrix[sor][oszlop]=100;
							atoltasoszlop++;
						}
					}
				}
				//Összesített adatok frissítése
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				osszsejtszam=0;
				osszkooperalo=0;
				osszcsalo=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix[sor][oszlop]==1){
							osszsejtszam++;
							osszkooperalo++;
						}
						if(matrix[sor][oszlop]==2){
							osszsejtszam++;
							osszcsalo++;
						}
					}
				}
				atoltas_sorszama++;
				fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
				osztodasok_szama=0;
			}
			//Élelempótlás, ha elfogy valamelyik típus
			if(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4)){
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
			}
		}
		
		//A rossz időszak szimulálása, ha a góckereső fragmentációt használunk.
		if(atoltasmintazat==5){
			//A spóraképzés akkor indul meg, ha az élelemmenyiség az eredeti élelemmenyiség 40%-a alá esik, és még vannak kooperáló sejtek
			if((osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4))&&(osszkooperalo)){
				
				//A két átoltási esemény eltelt idő kiiratása és a számláló nullázása
				fprintf(atoltasok_kozti_ido_file,"%d\n",atoltas_kozti_ido);
				atoltas_kozti_ido=0;
				
				//Lenullázom az átoltással kapcsolatos tömböket
				for(legmagasabb_koordinata=0;legmagasabb_koordinata<(matrixmeret*matrixmeret);legmagasabb_koordinata++){
						legmagasabb_sorok[legmagasabb_koordinata]=0;
						legmagasabb_oszlopok[legmagasabb_koordinata]=0;
						legmagasabb_rosszak[legmagasabb_koordinata]=0;
				}
				//Megnézem a sejtek asszociáltsági fokait, és melyik a legnagyobb asszociáltsági fok
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						//Az asszocialt szomszédok számának megnézése
						if(asszocialtmatrix[sor][oszlop]){
							asszocialtfokmatrix[sor][oszlop]=asszocialt_szomszedszamlalas_atoltas(asszocialtmatrix,sor,oszlop);
						}
						else{
							asszocialtfokmatrix[sor][oszlop]=0;
						}
						if(asszocialtfokmatrix[sor][oszlop]>legmagasabb_asszocialtsag){
							legmagasabb_asszocialtsag=asszocialtfokmatrix[sor][oszlop];
						}
					}
				}
				//Mégegyszer végigmegyek az asszociáltsági fokokat tartalmazó mátrixon, és elmentem a legnagyobb asszociáltsági fokokat tartalmazó sejtek koordinátáit.
				legmagasabb_koordinata=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(asszocialtfokmatrix[sor][oszlop]>=legmagasabb_asszocialtsag){
							legmagasabb_sorok[legmagasabb_koordinata]=sor;
							legmagasabb_oszlopok[legmagasabb_koordinata]=oszlop;
							legmagasabb_koordinata++;
						}
					}
				}
				//Elmentem mennyi gócpont van
				legmagasabb_koordinata_max=legmagasabb_koordinata;
				sporaciklus=0;
				//kiválasztom a gócpontot, amit át fogok oltani. Addig választok, amíg a spórában találhatók csaló és kooperáló sejtek
				while(sporaciklus<1){
					legmagasabb_koordinata=rand()%legmagasabb_koordinata_max;
					if(legmagasabb_rosszak[legmagasabb_koordinata]==0){
						//Végrehajtom az átoltást. A teljes mátrix 10%-át fogom átoltani.
						atoltassor=torusz((legmagasabb_sorok[legmagasabb_koordinata])-(matrixmeret/100*5));
						atoltasoszlop=torusz((legmagasabb_oszlopok[legmagasabb_koordinata])-(matrixmeret/100*5));
						atoltaskezdes=atoltasoszlop;
						for(sor=0;sor<10;sor++){
							for(oszlop=0;oszlop<10;oszlop++){
								atoltassor=torusz(atoltassor);
								atoltasoszlop=torusz(atoltasoszlop);
								//Kiválasztok egy véletlen koordinátát, és ha az nem foglalt, akkor belemásolom a kiválasztott sejtet
								while(ures_e==0){
									ujsor=koordinatageneralas();
									ujoszlop=koordinatageneralas();
									if(matrix2[ujsor][ujoszlop]==0){
										ures_e=1;
									}
								}
								matrix2[ujsor][ujoszlop]=matrix[atoltassor][atoltasoszlop];
								ures_e=0;
								if(matrix[atoltassor][atoltasoszlop]==1){
									sporakooperalo++;
								}
								if(matrix[atoltassor][atoltasoszlop]==2){
									sporacsalo++;
								}
								atoltasoszlop++;
							}
							atoltassor++;
							atoltasoszlop=atoltaskezdes;
						}
						sporaarany=(float)sporakooperalo/(sporacsalo+sporakooperalo);
						if(sporaarany<kooperator_atoltas_szazalek){
							sporacsalo=0;
							sporakooperalo=0;
							legmagasabb_rosszak[legmagasabb_koordinata]=1;
							for(sor=0;sor<matrixmeret;sor++){
								for(oszlop=0;oszlop<matrixmeret;oszlop++){
									matrix2[sor][oszlop]=0;
								}
							}
						}
						else{
							sporaciklus=1;
						}
					}
					//Ha az összes átoltási góc rossz, leállítom a szimulációt.
					for(legmagasabb_koordinata=0;legmagasabb_koordinata<=legmagasabb_koordinata_max;legmagasabb_koordinata++){
						oszrossz=oszrossz+legmagasabb_rosszak[legmagasabb_koordinata];
					}
					if(oszrossz==legmagasabb_koordinata_max){
						kiiratasertek[0]=ido;
						ido=generacioszam;
						printf("\nVege a szimulacionak, mivel nem volt megfelo goc, amit at lehetett volna oltani");
						fclose(tulajdonsagok);
						FILE *tulajdonsagok;
						tulajdonsagok=fopen("tulajdonsagok.dat", "w");
						fprintf(tulajdonsagok, "%d,%d,%d,%d\n", matrixmeret,seed,kiiratasertek[0],kiiratas);
						sporaciklus=1;
					}
					oszrossz=0;
				}
				//A sejt, energia, asszocialt és élelemmátrix kiürítése (új hely létrehozása)
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=0;
						elelemmatrix[sor][oszlop]=kezdetielelem;
						energiamatrix[sor][oszlop]=0;
						asszocialtmatrix[sor][oszlop]=0;
						asszocialtfokmatrix[sor][oszlop]=0;
					}
				}
				for(legmagasabb_koordinata=0;legmagasabb_koordinata<=matrixmeret*matrixmeret;legmagasabb_koordinata++){
					legmagasabb_sorok[legmagasabb_koordinata]=0;
					legmagasabb_oszlopok[legmagasabb_koordinata]=0;
					legmagasabb_rosszak[legmagasabb_koordinata]=0;
				}
				//Az új helyre kerülés szimulálása
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						matrix[sor][oszlop]=matrix2[sor][oszlop];
						//kiürítem az átoltó mátrixot.
						matrix2[sor][oszlop]=0;
						//feltöltöm energiával a sejteket.
						if(matrix[sor][oszlop]){
							energiamatrix[sor][oszlop]=100;
						}
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
				//Összesített adatok frissítése
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				osszsejtszam=0;
				osszkooperalo=0;
				osszcsalo=0;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						if(matrix[sor][oszlop]==1){
							osszsejtszam++;
							osszkooperalo++;
						}
						if(matrix[sor][oszlop]==2){
							osszsejtszam++;
							osszcsalo++;
						}
					}
				}
				atoltas_sorszama++;
				fprintf(atoltas_statisztika,"%d,%d\n",atoltas_sorszama,osztodasok_szama);
				osztodasok_szama=0;
			}
			//Élelempótlás, ha elfogy valamelyik típus
			if(osszelelem<(matrixmeret*matrixmeret*kezdetielelem*0.4)){
				osszelelem=kezdetielelem*matrixmeret*matrixmeret;
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						elelemmatrix[sor][oszlop]=kezdetielelem;
					}
				}
			}
		}
		//kiiratas
		//A sejttér kiiratása
		if((ido>0)&&(ido%kiiratas==0)){
			if(sejtter){
				for(sor=0;sor<matrixmeret;sor++){
					for(oszlop=0;oszlop<matrixmeret;oszlop++){
						fprintf(generaciok, "%d,",matrix[sor][oszlop]);
						fprintf(elelem,"%d,",elelemmatrix[sor][oszlop]);
						if(asszocialtmatrix[sor][oszlop]){
							fprintf(asszocialtfok,"%d,",asszocialt_szomszedszamlalas_atoltas(asszocialtmatrix,sor,oszlop));
						}
						else{
							fprintf(asszocialtfok,"%d,",0);
						}
						fprintf(asszociacio_debug, "%d,",asszocialtmatrix[sor][oszlop]);
					}
				}
				fprintf(generaciok, "\n");
				fprintf(elelem, "\n");
				fprintf(asszocialtfok, "\n");
				fprintf(asszociacio_debug, "\n");
			}
			if(menet_kozbeni_kiiratas){
				fprintf(statisztika,"%d,%d,%d\n",ido,osszkooperalo,osszcsalo);
				//Egyéb adatok kiiratása
				fprintf(diagrammok,"%d,%d,%d,%d\n",ido,osszkooperalo,osszcsalo,osszelelem);
			}
			
			
		}
		//Az átoltási esemény után eltelt idő számolása
		atoltas_kozti_ido++;
		//az adott változók lenullázása
		asszocialtsag=0;
		asszocialas=0;
		legmagasabb_asszocialtsag=0;
		oszrossz=0;
		sporacsalo=0;
		sporakooperalo=0;
		sporaciklus=0;
		asszocialt_szomszedszam=0;
		legmagasabb_koordinata=0;
		legmagasabb_koordinata_max=0;
	}
	//Kiiratom az utolsó állapot statisztikáját
	if(sejtter){
		for(sor=0;sor<matrixmeret;sor++){
			for(oszlop=0;oszlop<matrixmeret;oszlop++){
				fprintf(generaciok, "%d,",matrix[sor][oszlop]);
				fprintf(elelem,"%d,",elelemmatrix[sor][oszlop]);
				if(asszocialtmatrix[sor][oszlop]){
					fprintf(asszocialtfok,"%d,",asszocialt_szomszedszamlalas_atoltas(asszocialtmatrix,sor,oszlop));
				}
				else{
					fprintf(asszocialtfok,"%d,",0);
				}
				fprintf(asszociacio_debug, "%d,",asszocialtmatrix[sor][oszlop]);
			}
		}
	}
	fprintf(statisztika,"%d,%d,%d\n",ido,osszkooperalo,osszcsalo);
	//lezárom a file-ket
	fclose(tulajdonsagok);
	fclose(generaciok);
	fclose(elelem);
	fclose(asszocialtfok);
	fclose(diagrammok);
	fclose(statisztika);
	fclose(atoltas_statisztika);
	fclose(debug);
	fclose(asszociacio_debug);
	fclose(atoltasok_kozti_ido_file);
	return 0;
}
