#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#define size 30  // 30 thanh pho co the thay doi

typedef struct{
	float do_dai;	// Do dai canh
	int dau, cuoi;	// Dinh dau dinh cuoi canh do
	int da_dung;	// Xac dinh canh da su dung hay chua?
}canh;				// Cau truc canh

bool dd[1000]; // Mang danh dau dinh
int dem = 1, dem2 = 1; // Dem so dinh

// Cac ham cua chuong trinh
void TextColor(int);
void getInfor();
void menuInput();
void menuFILE();
void menuOutput();
void getfile(canh a[][size], int *n);
void fileout(canh a[][size], canh PA[], int n, float *sum);
void get(canh a[][size], int *n);
void in_matran(canh a[][size], int);
void in_PA(canh PA[], int n, float *sum);
float canh_NN(canh a[][size], int);
float can_duoi(canh a[][size], float, int, int);
void Cap_Nhat_PA_TNTT(canh a[][size], int n, float TGT, float *GiaNNTT, canh x[], canh PA[]);
int co_chu_trinh(canh x[], int, int);
void Branch_and_Bound(canh a[][size], int n, int i, int dau, float *TGT, float *CD, float *GiaNNTT, canh x[], canh PA[]);
void Reset(canh a[][size], int n);
void check_main_diagonal(canh a[][size], int n);
void check_connection_graph(canh a[][size], int n, int i);

int main(){
	canh a[size][size];
	int n, tpxp;
	getInfor();
	char inp, outp;
	do{
		TextColor(14);
		menuInput();
		fflush(stdin);
		scanf("%c", &inp);
		printf("\n");
		switch (inp){
			case '1':{
				get(a, &n);
				if(n < 2){
					TextColor(4);
					printf("          Loi so dinh qua be!!!\n");
					printf("          So dinh phai 'lon hon' hoac 'bang' 2\n");
					return 0;
				}
				break;
			}		
			case '2':{
				menuFILE();
				getfile(a, &n);
				if(n < 2){
					TextColor(4);
					printf("         Loi so dinh qua be!!!\n");
					printf("          So dinh phai 'lon hon' hoac 'bang' 2\n");
					return 0;
				}
				break;
			}
			default:
				return 0;
		}
		check_main_diagonal(a, n);
		check_connection_graph(a, n, 0);
		if(dem == n){
			printf("         DO THI LIEN THONG!!!\n");
		}
		else{
			for(int i = 1; i < n; i++){
				if(dd[i] == 0){
					check_connection_graph(a, n, i);
					printf("\n");
					dem2++;
				}
			}
			TextColor(4);
			printf("          DO THI KHONG LIEN THONG!!!\n");
			printf("          So do thi con LIEN THONG la: %d", dem2);
			return 0;
		}
		canh PA[n];
		canh x[n];
		fflush(stdin);
		in_matran(a, n);
		float sum = 0.0, TGT = 0.0, CD = 0.0, GiaNNTT = 3.40282e+38;
		TextColor(6);
		printf("\n          Ban muon xuat phat tu thanh pho nao? (Mot trong cac thanh pho tu 1 den %d): ", n);
		again:
		scanf("%d", &tpxp);
		if(tpxp <= 0 || tpxp > n){
			printf("          Nhap lai: ");
			goto again;
		}
		Branch_and_Bound(a, n, 0, tpxp -1, &TGT, &CD, &GiaNNTT, x, PA);
		do{
			menuOutput();
			fflush(stdin);
			scanf("%c",&outp);
		    printf("\n");
		    switch(outp){
		    	case '1':{
		    		in_PA(PA, n, &sum);
		    		sum = 0.0;
		    		Reset(a, n);
					break;
				}
				case '2':{
					in_PA(PA, n, &sum);
					fileout(a, PA, n, &sum);
					Reset(a, n);
					break;
				}
				case '3':{
					system("pause");
			    	system("cls");
			    	Reset(a, n);
		            getInfor();
					break;
				}
				default:
					return 0;
			}
			printf("\n                                               -------\n\n");
		}while(outp == '1' || outp == '2');
	}while((inp == '1') || (inp == '2'));
	
	return 0;
}


void TextColor(int x){ //X là mã màu
    HANDLE h= GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, x);
}


void getInfor(){
	int i;
	TextColor(3);
	printf("\n%9c", 201);
	for (i = 0; i <= 85; i++) printf("%c", 205);
	printf("%c", 187);
	printf("\n        %c %86c", 186, 186);
	printf("\n        %c                             DO AN LAP TRINH TINH TOAN                                %c", 186, 186);
	printf("\n        %c        DE TAI: BAI TOAN NGUOI KHACH DI DU LICH TREN DO THI LIEN THONG G(V,E,W)       %c", 186, 186);
	printf("\n        %c                             %c THUAT TOAN: NHANH CAN %c                                %c", 186, 16, 17, 186);
	printf("\n        %c %86c", 186, 186);
	printf("\n        %c       Sinh vien thuc hien:                       Giang vien huong dan:               %c", 186, 186);
	printf("\n        %c           %c Dinh Duc                                   %c Nguyen Van Hieu             %c", 186, 4, 4, 186);
	printf("\n        %c           %c Truong Xuan Phuc %57c", 186, 4, 186);
	printf("\n        %c %86c", 186, 186);
	printf("\n        %c", 200);
	for(i = 0; i <= 85; i++) printf("%c", 205);
	printf("%c\n", 188);
}


void menuInput(){
	TextColor(14);
	printf("\n                                  %c   CHON CACH NHAP DU LIEU   %c\n", 175, 174);
	printf("\n                                      + Nhan 1: Tu ban phim");
	printf("\n                                      + Nhan 2: Tu File");
	printf("\n                                   Nhap phim bat ky de Ket thuc");
	printf("\n                                             -------\n");
	TextColor(15);
	printf("\n          %c Nhap yeu cau: ", 4);
}


void menuFILE(){
	TextColor(14);
	printf("\n                                  %c   CHON FILE DE LAY DU LIEU    %c\n", 16, 17);
	printf("\n                           Nhan 1 : TEST1                 Nhan 2 : TEST2 ");
	printf("\n                           Nhan 3 : TEST3                 Nhan 4 : TEST4 ");
	printf("\n                           Nhan 5 : TEST5                 Nhan 6 : TEST6 ");
	printf("\n                           Nhan 7 : TEST7                 Nhan 8 : TEST8 ");
	printf("\n                           Nhan 9 : TEST9                 Nhan 10: TEST10");
	printf("\n                                             -------\n");
	TextColor(15);
	printf("\n          %c Nhap yeu cau: ", 4);
}


void menuOutput(){
	TextColor(14);
	printf("\n                                       BAI TOAN NGUOI KHACH DI DU LICH\n");
	printf("\n                                       + Nhan 1: De in ket qua ra man hinh");
	printf("\n                                       + Nhan 2: De in ket qua vao File RESULT.OUT");
	printf("\n                                       + Nhan 3: De nhap du lieu moi");
	printf("\n                                   Nhap phim bat ky de ket thuc");
	printf("\n                                               -------\n");
	TextColor(15);
	printf("\n          %c Nhap yeu cau: ", 4);
}


void getfile(canh a[][size], int *n){
	int v, i, j;
	FILE *f;
	scanf("%d",&v);
	if(v==1)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST1.txt","r");
	if(v==2)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST2.txt","r");
	if(v==3)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST3.txt","r");
	if(v==4)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST4.txt","r");
	if(v==5)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST5.txt","r");
	if(v==6)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST6.txt","r");
	if(v==7)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST7.txt","r");
	if(v==8)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST8.txt","r");
	if(v==9)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST9.txt","r");
	if(v==10)	f=fopen("D:\\PBL1-Travelling Salesman Problem\\TEST 1 -10\\TEST10.txt","r");
	if(f == NULL){
		TextColor(4);
		printf("\n          Loi mo file hoac khong ton tai!\n");
		exit(0);
	}
	fscanf(f, "%d", n);	// Doc so thanh pho	
	for(i = 0; i < *n; i++){
		for(j = 0; j < *n; j++){
			fscanf(f, "%f", &a[i][j].do_dai);
			a[i][j].dau = i;
			a[i][j].cuoi = j;
			a[i][j].da_dung = 0;
		}
	}
	TextColor(11);
	printf("\n          Lay du lieu tu file TEST%d thanh cong!\n\n ", v);
	fclose(f);
}


void fileout(canh a[][size], canh PA[], int n, float *sum){
	int i;
	FILE *fo= fopen("F:\\HK2 2022-2023\\Project Based Learning\\Output RESULT.OUT\\RESULT.OUT","w");
	fprintf(fo,"Chu trinh toi uu nhat la :");
	for(i = 0; i < n; i++){
		fprintf(fo, " %d -> %d ", PA[i].dau +1, PA[i].cuoi +1);
	}
	fprintf(fo,"\nTong do dai(chi phi) cua chu trinh la : %f", *sum);
	fclose(fo);
	TextColor(11);
	printf("\n\n                                       Da luu ket qua vao file!\n\n");
}


void get(canh a[][size], int *n){
	int i, j;
	printf("          Nhap so thanh pho: ");
	scanf("%d", &*n);
	printf("\n          Nhap vao Ma tran TRONG SO chi phi cua do thi: \n\n");
	TextColor(3);
	for(int i = 0; i < *n; i++){
		for(j = 0; j < *n; j++){
			a[i][j].dau = i;
			a[i][j].cuoi = j;
			a[i][j].da_dung = 0;
			printf("\t  %c%d%c%d=", 177, a[i][j].dau +1, 45, a[i][j].cuoi +1);
			scanf("%f", &a[i][j].do_dai);
		}
		printf("\n");
	}
}


void in_matran(canh a[][size], int n){
	TextColor(3);
	int i, j;
	printf("\n                                      MA TRAN TRONG SO CHI PHI CUA DO THI: \n\n");
	for(int i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%c%d%c%d = %5.2f\t", 177, a[i][j].dau +1, 45, a[i][j].cuoi +1, a[i][j].do_dai);
		}
		printf("\n");
	}
}


void in_PA(canh PA[], int n, float *sum){	// Phuong an
	TextColor(7);
	int i, j;
	printf("\n          PHUONG AN TIM DUOC:\n");
	for(i = 0; i < n; i++){
		*sum += PA[i].do_dai;
		printf("          Canh %d%c%d = %5.2f\t\n", PA[i].dau +1, 45, PA[i].cuoi +1, PA[i].do_dai);
	}
	printf("\n          Chu trinh toi uu nhat:");
	for(i = 0; i < n; i++){
		printf(" %d %c %d ", PA[i].dau +1, 26,PA[i].cuoi +1);
	}
	printf("\n\n          Tong do dai(chi phi) cac canh cua chu trinh = %4.2f\n", *sum);
}


// Tim canh nho nhat trong so cac canh chua su dung
float canh_NN(canh a[][size], int n){
	float Cmin = 3.40282e+38;	// So thuc dau cham dong lon nhat trong 32-bit
	int i, j;
	for(i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i != j && !a[i][j].da_dung && a[i][j].do_dai < Cmin){
				Cmin = a[i][j].do_dai;
			}
		}
	}
	return Cmin;
}


// Ham tinh can duoi
float can_duoi(canh a[][size], float TGT, int n, int i){	// i la bac // TGT la tong gia tri
	return TGT + (n-i)*canh_NN(a, n);
}


// Ghi nhan phuong an tot nhat tam thoi
void Cap_Nhat_PA_TNTT(canh a[][size], int n, float TGT, float *GiaNNTT, canh x[], canh PA[]){
	int i;
	// Mang x da co n-1 canh tu x[0] den x[n-1]
	x[n-1] = a[x[n-2].cuoi][x[0].dau];
	TGT = TGT + x[n-1].do_dai;
	if(*GiaNNTT > TGT){
		*GiaNNTT = TGT;
		for(i = 0; i < n; i++){
			PA[i] = x[i];
		}
	}
}

/*  
	Gia su ta co mot phuong an thanh phan x, voi k canh
	Neu ta di toi dinh ke_tiep ma dinh ke_tiep nay trung voi mot dinh dau cua mot canh nao do thi tao thanh chu trinh
*/

int co_chu_trinh(canh x[], int k, int ke_tiep){
	int i = 0, co_CT = 0;
	while(i < k && !co_CT){
		if(ke_tiep == x[i].dau){
			co_CT = 1;
		}
		else
			i++;
	}
	return co_CT;
}


// Ham thuat toan Nhanh Can
void Branch_and_Bound(canh a[][size], int n, int i, int dau, float *TGT, float *CD, float *GiaNNTT, canh x[], canh PA[]){
	int j;	// j la dinh ke tiep
	for(j = 0; j < n; j++){
		if(a[dau][j].do_dai != -1 && dau != j && !a[dau][j].da_dung && !co_chu_trinh(x, i, j)){
			// Ung voi mot gia tri cua j, ta co mot nut tren cay
			// Tinh 2 thong so cua nut
			*TGT = *TGT + a[dau][j].do_dai;
			*CD = can_duoi(a, *TGT, n, i+ 1);
			if(*CD < *GiaNNTT){	// Neu CD <= Gia NNTT thi cat tia
			// Neu khong co dieu kien nay thi chuong trinh la phuong phap VET CAN
				x[i] = a[dau][j];
				a[dau][j].da_dung = 1;
				a[j][dau].da_dung = 1;
				if(i == n-2){	// Xet het tat ca cac dinh va nhung duong di moi co n -1 canh
					Cap_Nhat_PA_TNTT(a, n, *TGT, GiaNNTT, x, PA);
				}
				else
					Branch_and_Bound(a, n, i +1, j, TGT, CD, GiaNNTT, x, PA);
			}
			// Quay lui de xet nut khac (ung voi gia tri khac cua j)
			*TGT = *TGT - a[dau][j].do_dai;	// Tra lai tong gia tri cua nut cha
			a[dau][j].da_dung = 0;	// Tra lai canh vua xet la chua su dung
			a[j][dau].da_dung = 0;
		}
	}
}

// Ham Reset
void Reset(canh a[][size], int n){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			a[i][j].da_dung = 0;
		}
	}
}

// Ham gan gia tri -1 cho cac dinh khong co duong di (duong cheo chinh cua ma tran chi phi)
void check_main_diagonal(canh a[][size], int n){
	int i;
	for(int i = 0; i < n; i++){
		a[i][i].do_dai = -1;
	}
}

// Ham kiem tra lien thong cua ma tran chi phi (thuat toan BFS- duyet theo chieu rong)
void check_connection_graph(canh a[][size], int n, int i){
    dd[i]=1;	//	Danh dau dinh duoc duyet
    for(int j=0;j<n;j++){
        if(a[i][j].do_dai != 0 && dd[j] == 0){	// Neu 2 dinh co duong di va dinh chua duoc duyet qua
            dem++;	// Dem so dinh duoc duyet qua
            check_connection_graph(a,n,j); // De quy voi dinh j
        }
    }
}

