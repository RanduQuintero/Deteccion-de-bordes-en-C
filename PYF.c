#include <stdio.h>
#include <stdlib.h>
#include "FreeImage.h" //BYTE tipo de variable para el manejo de imagenes

//Filtros Mx y My
int MX[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

int MY[3][3] = {
    { 1,  2,  1},
    { 0,  0,  0},
    {-1, -2, -1}
};

//Función para aplicar el filtro a un píxel
int applyFilter(int x, int y, BYTE *DatosImg, int Ancho, int Alto, int Filtro[3][3]) {
    int Suma = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int Px = x + j;
            int Py = y + i;
            if (Px >= 0 && Px < Ancho && Py >= 0 && Py < Alto) {
                int Pixel = (Py * Ancho + Px); //Píxel en escala de grises
                Suma += DatosImg[Pixel] * Filtro[i + 1][j + 1];
            }
        }
    }
    return Suma;
}

int main() {
	//Variales
	int Ancho, Alto;
    	//Ruta de la imagen
    	char *Archivo = "lena.jpg";

    	//Cargar la imagen
    	FIBITMAP *Imagen = FreeImage_Load(FIF_JPEG, Archivo, 0);

    	//Convertir la imagen a escala de grises
    	FIBITMAP *ImagenGris = FreeImage_ConvertToGreyscale(Imagen);
    
    	//Dimensiones de la imagen
    	Ancho = FreeImage_GetWidth(ImagenGris);
    	Alto = FreeImage_GetHeight(ImagenGris);
    	BYTE *DatosGrises = (BYTE*)FreeImage_GetBits(ImagenGris);

    	//Crear vector para almacenar los resultados
    	BYTE *B = (BYTE*)malloc((Ancho * Alto) * sizeof(BYTE)); // Para MX
    	BYTE *C = (BYTE*)malloc((Ancho * Alto) * sizeof(BYTE)); // Para MY

    	//Aplicar el filtro MX
    	for (int i = 0; i < Alto; i++) {
        	for (int j = 0; j < Ancho; j++) {
            		int Filtro = applyFilter(j, i, DatosGrises, Ancho, Alto, MX);
			if (Filtro < 0) {
            			B[i * Ancho + j] = 0;
        		}
        		else if (Filtro > 255) {
            			B[i * Ancho + j] = 255;
        		} 
        		else {
            			B[i * Ancho + j] = (BYTE)Filtro;
        		}
		}
    	}

    	//Aplicar el filtro MY
    	for (int i = 0; i < Alto; i++) {
        	for (int j = 0; j < Ancho; j++) {
            		int Filtro = applyFilter(j, i, DatosGrises, Ancho, Alto, MY);
            		if (Filtro < 0) {
            			C[i * Ancho + j] = 0;
        		}
        		else if (Filtro > 255) {
            			C[i * Ancho + j] = 255;
        		} 
        		else {
            			C[i * Ancho + j] = (BYTE)Filtro;
        		}
        	}
    	}
    	//Crear imágenes para almacenar los resultados
    	//MX
    	FIBITMAP *ImagenB = FreeImage_Allocate(Ancho, Alto, 8, 0, 0, 0); //Imagen en escala de grises
    	BYTE *DatosImgB = (BYTE*)FreeImage_GetBits(ImagenB);
    	for (int i = 0; i < Ancho * Alto; i++) {
      		DatosImgB[i] = B[i];
    	}
    	//Guardar la imagen con el filtro MX
    	FreeImage_Save(FIF_JPEG, ImagenB, "lena_MX.jpg", JPEG_QUALITYSUPERB);

    	//MY
    	FIBITMAP *ImagenC = FreeImage_Allocate(Ancho, Alto, 8, 0, 0, 0); //Imagen en escala de grises
    	BYTE *DatosImgC = (BYTE*)FreeImage_GetBits(ImagenC);
    	for (int i = 0; i < Ancho * Alto; i++) {
        	DatosImgC[i] = C[i];
    	}
    	//Guardar la imagen con el filtro MY
    	FreeImage_Save(FIF_JPEG, ImagenC, "lena_MY.jpg", JPEG_QUALITYSUPERB);
    
    
    	//Liberar memoria
    	FreeImage_Unload(Imagen);
    	FreeImage_Unload(ImagenGris);
    	FreeImage_Unload(ImagenB);
    	FreeImage_Unload(ImagenC);
    	FreeImage_DeInitialise();
    	free(B);
    	free(C);

    	return 0;
}
