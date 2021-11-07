#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROW 50
#define COL 50
#define DEFAULT_IMMUNE_PERIOD 45 
#define DEFAULT_INFECTION_PERIOD 2
#define DEFAULT_INFECT_RATE 0.2

FILE *Accu_Infection_Data;
FILE *Accu_Immune_Data;
FILE *By_Times_Immune_Data;
FILE *By_Times_Infection_Data;
FILE *By_Times_Infect_Rate_Data;

typedef struct _people {//초기값 지정 안됨 ㅡㅡ
    short Left_Infection_Period= 0;
    short Immune_Period = 0;//if(Immune_length>0) Immune_Flag = 1;
    double Infect_Rate = DEFAULT_INFECT_RATE;
} people;


void Init_Vaccine(people**func_field, double VaccinationRate);

void Sole_Act(people**func_field);

short Infect(double func_Infect_Rate, short Immune_Flag, short Left_Infection_Period);

void Proceed_Date(people**func_field);

void Accumulate_n_Print_Field_Left_Infection_Period_TF(people**func_field, short**Accu_Infection_Data);

void Accumulate_Field_Left_Infection_Period_TF(people**func_field,short**Accu_Infection_Data);

void Print_Field_Left_Infection_Period_TF(people**func_field);

void Fprint_Field_Left_Infection_Period_TF(people**func_field);

void Accumulate_n_Print_Field_Immune_Period_TF(people**func_field,short**Accu_Immune_Data);

void Accumulate_Field_Immune_Period_TF(people**func_field, short**Accu_Immune_Data);

void Print_Field_Immune_Period_TF(people**func_field);

void Fprint_Field_Immune_Period_TF(people**func_field);

void Print_Field_Infect_Rate_Figure(people**func_field);

void Fprint_Field_Infect_Rate_Figure(people**func_field);

void Print_Accumulated_Data(short**Data);

void fprint_Infection_Accumulated_Data(short**Data);

void fprint_Immune_Accumulated_Data(short**Data);

int main(int argc, const char * argv[])
{
    people**field = (people**)calloc(ROW, sizeof(people*));
    
    short **Accumulated_Infection_Data = (short **)calloc(ROW, sizeof(short *));

    short **Accumulated_Immune_Data = (short **)calloc(ROW, sizeof(short *));
    
    for(int i=0;i<ROW;i++){//초기값 지정
        *(field+i) = (people*)calloc(COL, sizeof(people));
        *(Accumulated_Infection_Data+i)=(short*)calloc(COL,sizeof(short));
        *(Accumulated_Immune_Data+i)=(short*)calloc(COL,sizeof(short));
    }
    
    
    
    srand((unsigned int)time(NULL));
    
    int i,j;
    
    
    printf("시행일수를 입력하세요: ");
    int days;
    scanf("%d",&days);
    days = abs(days);
    int origin_days = days;
    printf("시행횟수를 입력하세요: ");
    int times;
    scanf("%d",&times);
    
    
    printf("\n접종률을 입력하세요(0<=VaccRate<=100): ");
    double VaccinationRate;
    int VaccineRate;
    scanf("%d",&VaccineRate);
    VaccinationRate = (double)VaccineRate/100;
    
    
    char filename_By_Times_Immune[100];
    char filename_By_Times_Infection[100];
    char filename_By_Times_Infect_Rate[100];
    char filename_Infection_Accu[100];
    char filename_Immune_Accu[100];
    
    int t=0;
    while(times>t){//시행별 진행 루프
        //디렉터리 설정은 장비별로 설정 필요
        sprintf(filename_By_Times_Immune, "../../CalResult/By_Times_Immune-%d-%d.txt",(int)(VaccinationRate*100),t+1);
        sprintf(filename_By_Times_Infection, "../../CalResult/By_Times_Infection-%d-%d.txt",(int)(VaccinationRate*100),t+1);
        sprintf(filename_By_Times_Infect_Rate, "../../CalResult/By_Times_Infect_Rate-%d-%d.txt",(int)(VaccinationRate*100),t+1);
        sprintf(filename_Infection_Accu, "../../CalResult/Infection_Accu-%d-%d.txt",(int)(VaccinationRate*100),t+1);
        sprintf(filename_Immune_Accu, "../../CalResult/Immune_Accu-%d-%d.txt",(int)(VaccinationRate*100),t+1);
        By_Times_Immune_Data = fopen(filename_By_Times_Immune, "w");
        By_Times_Infection_Data = fopen(filename_By_Times_Infection, "w");
        By_Times_Infect_Rate_Data = fopen(filename_By_Times_Infect_Rate, "w");
        Accu_Immune_Data = fopen(filename_Immune_Accu, "w");
        Accu_Infection_Data= fopen(filename_Infection_Accu,"w");

        for(i=0;i<ROW;i++){//초기값 지정
            for(j=0;j<COL;j++){
                field[i][j].Left_Infection_Period=0;
                field[i][j].Immune_Period=0;
                field[i][j].Infect_Rate=DEFAULT_INFECT_RATE+ ((double)rand()/RAND_MAX);
                Accumulated_Infection_Data[i][j] = 0;
                Accumulated_Immune_Data[i][j] = 0;
            }
        }
        
        field[ROW/2][COL/2].Left_Infection_Period = DEFAULT_INFECTION_PERIOD;
        Init_Vaccine(field, VaccinationRate);//접종자 초기 설정
        while(days){//일별 진행 루프
//            printf("times %d/Date %d\n",t+1,days);
            Accumulate_Field_Left_Infection_Period_TF(field, Accumulated_Infection_Data);//감염 누적
            Accumulate_Field_Immune_Period_TF(field, Accumulated_Immune_Data);//면역 누적
            Fprint_Field_Left_Infection_Period_TF(field);//시행 일별 감염 필드 저장
            Fprint_Field_Immune_Period_TF(field);//시행 일별 면역 필드 저장
            Sole_Act(field);// 감염 1회 실행
            Proceed_Date(field);// 날짜 1일 진행
            fprintf(By_Times_Immune_Data, "\n");
            fprintf(By_Times_Infection_Data, "\n");
            days--;
        }
        fprint_Immune_Accumulated_Data(Accumulated_Immune_Data);//시행별 면역 누적 저장
        fprint_Infection_Accumulated_Data(Accumulated_Infection_Data);//시행별 감염 누적 저장
        Fprint_Field_Infect_Rate_Figure(field);//시행별 감염률 저장
        t++;
        fclose(By_Times_Immune_Data);
        fclose(By_Times_Infection_Data);
        fclose(By_Times_Infect_Rate_Data);
        fclose(Accu_Immune_Data);
        fclose(Accu_Infection_Data);
        days= origin_days;
    }
    
    printf("Calcuating Fishished!\n");
}

short Infect(double func_Infect_Rate, short Immune_Flag, short Left_Infection_Period)//감염률과 항체 여부에 따른 감염 시행 함수
{
    int Trans_Int_Infect_Rate=(int)(func_Infect_Rate*100);
    int randnum = rand() %200;
    if(Left_Infection_Period>0)
    {
        return Left_Infection_Period;
    }
    else if(Immune_Flag){
        Trans_Int_Infect_Rate/=3;
        if(Trans_Int_Infect_Rate>=randnum){//감염상태
            return DEFAULT_INFECTION_PERIOD+(rand()%3);//오차범위 2일
        }
        else{
            return 0;
        }
    }
    else{
        if(Trans_Int_Infect_Rate>=randnum){
            return DEFAULT_INFECTION_PERIOD+(rand()%3);
        }
        else{
            return 0;
        }
    }
}
    
void Init_Vaccine(people**func_field, double VaccinationRate) //초기 백신 접종자 설정(무작위로 선택)
{
    int VaccinatedPeople = VaccinationRate*COL*ROW;
    int X,Y;
    while(VaccinatedPeople--){
        X = rand() % ROW;
        Y = rand() % COL;
        if(func_field[X][Y].Immune_Period){
            VaccinatedPeople++;
            continue;
        }
        func_field[X][Y].Immune_Period = DEFAULT_IMMUNE_PERIOD+(rand()%30+1);
    }
}

void Sole_Act(people**func_field)
{
    int i,j;
    
    people origin_func_field[ROW][COL];
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            origin_func_field[i][j].Left_Infection_Period=func_field[i][j].Left_Infection_Period;
            origin_func_field[i][j].Infect_Rate =func_field[i][j].Infect_Rate;
            origin_func_field[i][j].Immune_Period=func_field[i][j].Immune_Period;
        }
    }
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
//            printf("ROW: %d COL: %d\n",i,j);
            if(origin_func_field[i][j].Left_Infection_Period>0){
                //infect around
                if(i!=0&&i!=ROW-1){
                    if(j!=0&&j!=COL-1){
                        //좌상
                        func_field[i-1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j-1].Immune_Period,func_field[i-1][j-1].Left_Infection_Period);
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //우상
                        func_field[i+1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j-1].Immune_Period,func_field[i+1][j-1].Left_Infection_Period);
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                        //좌하
                        func_field[i-1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j+1].Immune_Period,func_field[i-1][j+1].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                        //우하
                        func_field[i+1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j+1].Immune_Period,func_field[i+1][j+1].Left_Infection_Period);
                    }
                    else if(j==0){
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                        //좌하
                        func_field[i-1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j+1].Immune_Period,func_field[i-1][j+1].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                        //우하
                        func_field[i+1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j+1].Immune_Period,func_field[i+1][j+1].Left_Infection_Period);
                    }
                    else{//j==COL-1
                        //좌상
                        func_field[i-1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j-1].Immune_Period,func_field[i-1][j-1].Left_Infection_Period);
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //우상
                        func_field[i+1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j-1].Immune_Period,func_field[i+1][j-1].Left_Infection_Period);
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                    }
                }
                else if(i==0){
                    if(j!=0&&j!=COL-1){
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //우상
                        func_field[i+1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j-1].Immune_Period,func_field[i+1][j-1].Left_Infection_Period);
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                        //우하
                        func_field[i+1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j+1].Immune_Period,func_field[i+1][j+1].Left_Infection_Period);
                    }
                    else if(j==0){
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                        //우하
                        func_field[i+1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j+1].Immune_Period,func_field[i+1][j+1].Left_Infection_Period);
                    }
                    else{//j==COL-1
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //우상
                        func_field[i+1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j-1].Immune_Period,func_field[i+1][j-1].Left_Infection_Period);
                        //우
                        func_field[i+1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i+1][j].Immune_Period,func_field[i+1][j].Left_Infection_Period);
                    }
                }
                else{//i==ROW-1
                    if(j!=0&&j!=COL-1){
                        //좌상
                        func_field[i-1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j-1].Immune_Period,func_field[i-1][j-1].Left_Infection_Period);
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                        //좌하
                        func_field[i-1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j+1].Immune_Period,func_field[i-1][j+1].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                    }
                    else if(j==0){
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                        //좌하
                        func_field[i-1][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j+1].Immune_Period,func_field[i-1][j+1].Left_Infection_Period);
                        //하
                        func_field[i][j+1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j+1].Immune_Period,func_field[i][j+1].Left_Infection_Period);
                    }
                    else{//j==COL-1
                        //좌상
                        func_field[i-1][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j-1].Immune_Period,func_field[i-1][j-1].Left_Infection_Period);
                        //상
                        func_field[i][j-1].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i][j-1].Immune_Period,func_field[i][j-1].Left_Infection_Period);
                        //좌
                        func_field[i-1][j].Left_Infection_Period = Infect((*func_field)->Infect_Rate,func_field[i-1][j].Immune_Period,func_field[i-1][j].Left_Infection_Period);
                    }
                }
            }
        }
    }
}

void Proceed_Date(people** func_field)
{
    int i,j;
    
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            if((*(func_field+i)+j)->Immune_Period>0) ((*(func_field+i)+j)->Immune_Period)--;
            if((*(func_field+i)+j)->Left_Infection_Period>0){
                if((--((*(func_field+i)+j)->Left_Infection_Period))==0){
                    (*(func_field+i)+j)->Immune_Period = DEFAULT_IMMUNE_PERIOD+(rand()%30+1);
                }
            }
        }
    }
}

void Print_Field_Left_Infection_Period_TF(people**func_field)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Left_Infection_Period)>0;
            printf("%2d",Flag);
        }
        puts("");
    }
}

void Accumulate_Field_Left_Infection_Period_TF(people**func_field,short**Accu_Infection_Data)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Left_Infection_Period)>0;
            Accu_Infection_Data[i][j] +=Flag;
        }
    }
}

void Accumulate_n_Print_Field_Left_Infection_Period_TF(people**func_field,short**Accu_Infection_Data)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Left_Infection_Period)>0;
            printf("%2d",Flag);
            Accu_Infection_Data[i][j] +=Flag;
        }
        puts("");
    }
}

void Fprint_Field_Left_Infection_Period_TF(people**func_field)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            fprintf(By_Times_Infection_Data,"%d",(func_field[i][j].Left_Infection_Period)>0);
            if(j!=COL-1){
                fprintf(By_Times_Infection_Data,",");
            }
        }
        fprintf(By_Times_Infection_Data, "\n");
    }
}

void Accumulate_n_Print_Field_Immune_Period_TF(people**func_field, short**Accu_Immune_Data)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Immune_Period)>0;
            printf("%2d",Flag);
            Accu_Immune_Data[i][j] +=Flag;
        }
        puts("");
    }
}

void Print_Field_Immune_Period_TF(people**func_field)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Immune_Period)>0;
            printf("%2d",Flag);
        }
        puts("");
    }
}

void Accumulate_Field_Immune_Period_TF(people**func_field, short**Accu_Immune_Data)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            bool Flag = (func_field[i][j].Immune_Period)>0;
            Accu_Immune_Data[i][j] +=Flag;
        }
    }
}

void Fprint_Field_Immune_Period_TF(people**func_field)
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            fprintf(By_Times_Immune_Data, "%d",(func_field[i][j].Immune_Period)>0);
            if(j!=COL-1){
                fprintf(By_Times_Immune_Data,",");
            }
        }
        fprintf(By_Times_Immune_Data, "\n");
    }
}

void Print_Field_Infect_Rate_Figure(people**func_field) // This function must excute only one time
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            printf("%1.2lf ",func_field[i][j].Infect_Rate);
        }
        puts("");
    }
}

void Fprint_Field_Infect_Rate_Figure(people**func_field) // This function must excute only one time
{
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            fprintf(By_Times_Infect_Rate_Data, "%1.2lf ",func_field[i][j].Infect_Rate);
            if(j!=COL-1){
                fprintf(By_Times_Infect_Rate_Data,",");
            }
        }
        fprintf(By_Times_Infect_Rate_Data, "\n");
    }
}

void Print_Accumulated_Data(short**Data)
{
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++)
        {
            printf("%-3d",Data[i][j]);//정렬용
//            printf("%d ",Data[i][j]);//Excel용
        }
        puts("");
    }
}

void fprint_Infection_Accumulated_Data(short**Data)
{
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++)
        {
            fprintf(Accu_Infection_Data,"%d", Data[i][j]);
            if(j!=COL-1){
                fprintf(Accu_Infection_Data,",");
            }
        }
        fprintf(Accu_Infection_Data, "\n");
    }
}

void fprint_Immune_Accumulated_Data(short**Data)
{
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++)
        {
            fprintf(Accu_Immune_Data,"%d", Data[i][j]);
            if(j!=COL-1){
                fprintf(Accu_Immune_Data,",");
            }
        }
        fprintf(Accu_Immune_Data, "\n");
    }
}
