#ifndef CUSTOMMATRIX_H
#define CUSTOMMATRIX_H

#include <iostream>
#include <cstring> // Diperlukan untuk membandingkan array, tapi lebih baik pakai operator==

using namespace std;

class CustomMatrix{
public:
    bool mat[3][3]={0};
    CustomMatrix(){

    }

    // Perbaikan: Menggunakan batas 3x3 yang pasti, karena array C-style akan 'decay' saat dilewatkan
    CustomMatrix(const bool m[3][3]){
        for (int i=0;i< 3;i++) {
            for(int j=0; j < 3;j++){
                mat[i][j] = m[i][j];
            }
        }
    }

    // Perbaikan: Menggunakan batas 3x3 yang pasti
    void fillMatrix(const bool m[3][3]){
        for (int i=0;i< 3;i++) {
            for(int j=0; j < 3;j++){
                mat[i][j] = m[i][j];
            }
        }
    }

    // NEW: Operator perbandingan untuk mendeteksi kecocokan pola
    bool operator==(const CustomMatrix& other) const {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (mat[i][j] != other.mat[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif // CUSTOMMATRIX_H
