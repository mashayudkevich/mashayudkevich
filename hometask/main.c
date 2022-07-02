#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#define STBIW_WINDOWS_UTF8
#define STBI_NO_SIMD





//void fGauss(unsigned char *od, unsigned char *dc, int ih, int iw){
     //for(int i=2;i<ih-1;i++)
        //for(int j=2;j<iw-1;j++){
            //dc[iw*i+j]=0.12*od[iw*i+j]+0.12*od[iw*(i+1)+j]+0.12*od[iw*(i-1)+j];
            //dc[iw*i+j]=dc[iw*i+j]+0.12*od[iw*i+(j+1)]+0.12*od[iw*i+(j-1)];
            //dc[iw*i+j]=dc[iw*i+j]+0.095*od[iw*(i+1)+(j+1)]+0.095*od[iw*(i+1)+(j-1)];
            //dc[iw*i+j]=dc[iw*i+j]+0.095*od[iw*(i-1)+(j+1)]+0.095*od[iw*(i-1)+(j-1)];
        //}
//return;
//}



//function dfs
void dfs(int i,int j,int iw,int ih,unsigned char*odata, int*comp,int num){
    comp[iw*i+j]=num;
    if((i>=1) && (i<=ih-1) && (j-2>=1) && (j-2<=iw-1)){
        if((abs(odata[iw*i+j]-odata[iw*(i)+(j-2)])<=59)&&(comp[iw*(i)+(j-2)]==0)){
            dfs(i,j-2,iw,ih,odata,comp,num);
        }
    }
    if((i-2>=1) && (i-2<=ih-1) && (j+1>=1) && (j+1<=iw-1)){
        if((abs(odata[iw*i+j]-odata[iw*(i-2)+(j+1)])<=51)&&(comp[iw*(i-2)+(j+1)]==0)){
            dfs(i-2,j+1,iw,ih,odata,comp,num);
        }
    }
    if((i+2>=1) && (i+2<=ih-1) && (j+1>=1) && (j+1<=iw-1)){
        if((abs(odata[iw*i+j]-odata[iw*(i+2)+(j+1)])<=51)&&(comp[iw*(i+2)+(j+1)]==0)){
            dfs(i+2,j+1,iw,ih,odata,comp,num);
        }
    }
    return;
}


int main(){
     // строка, путь к файлу
    char* inputPath = "homyak.png";



    int iw, ih, n,t=0,num=0;
    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }


    //Переходим от 4 канального изображения к 1 канальному

    unsigned char* odata = (unsigned char*)malloc(iw*ih*sizeof(unsigned char));
    int k=0;
    for(int i=0;i<ih*iw*n;i=i+n){
         odata[k]=0.299*idata[i]+0.587*idata[i+1]+0.114*idata[i+2];
         k++;
    }

    //contrast
     for(int i=2;i<ih-2;i++){
        for(int j=2;j<iw-2;j++){
            if(odata[iw*i+j]<100){
                odata[iw*i+j]=0;}
            if(odata[iw*i+j]>160){
                odata[iw*i+j]=255;}}}




    int* comp=(int*)malloc((iw*ih)*sizeof(int));
    for(int i=0;i<iw*ih;i++){
            comp[i]=0;}

    //dfs
    for (int i=1;i<=ih-1;i++){
        for (int j=1;j<=iw-1;j++){
            if(comp[iw*i+j]==0){
                num++;
                dfs(i,j,iw,ih,odata,comp,num);}}}



    //final array
    unsigned char* nextdata=(unsigned char*)malloc(iw*ih*n*sizeof(unsigned char));
    k=0; int y;

    //y-coefficient
    for(int i=0;i<iw*ih*n;i=i+n) {
        y=comp[k]%43+comp[k]%15;
        nextdata[i]=13+y*75;
        nextdata[i+1]=55+y*3;
        nextdata[i+2]=71+y*31;
        nextdata[i+3] = 255;
        k++;}


    //выгружаем полученное изображение
    char *outputPath="output.png";
    stbi_write_png(outputPath, iw, ih, n, nextdata, 0);
    stbi_image_free(idata);
    stbi_image_free(odata);
    stbi_image_free(nextdata);
    return 0;
}
