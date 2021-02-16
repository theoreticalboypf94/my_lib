//
// Created by alpharius on 17.02.2021.
//

#ifndef MATHEMATICA_PROJECT_MATRIX_IMPLEMENTATION_H
#define MATHEMATICA_PROJECT_MATRIX_IMPLEMENTATION_H

template<typename G>
Matrix<G>& Matrix<G>::operator^(int power) {
    if (power == -1){
        if(not this->is_square()) throw std::runtime_error("Error:  your matrix is not square matrix, so you cant get inverse it");

        Matrix<G> *result = new Matrix<G>(h,w);
        G deterinant = det(*this);
        if (deterinant == 0) throw std::runtime_error("Error:  determinant of matrix is 0");

        for(int i=0; i<h; i++)
            for(int j=0; j<w; j++)
                (*result)[i][j] = SIGN(i + j) * det(Minor(*this, i, j));

        *result = result->T();
        *result = *result / deterinant;
        // TODO определить операцию деления
        return *result;
    }
}

template<typename G>
Matrix<G>& Matrix<G>::operator*(Matrix<G>& another) {
    if (not is_multiplicable(another)) throw std::runtime_error("Error:  your matrix can't be multiplied, by size reason");

    Matrix<G> *result = new Matrix<G>(h, another.get_Width());
    for(int i=0; i<h; i++){
        for(int j=0; j<another.get_Width(); j++){
            G convolution = 0;
            for(int K=0; K<w; K++){
                convolution += (*this)[i][K] * another[K][j];
            }
            (*result)[i][j] = convolution;
        }
    }
    return *result;
}

template<typename G>
Matrix<G>& Matrix<G>::operator/(G devisor) {
    Matrix<G> *result = new Matrix<G>(*this);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++){
            (*result)[i][j] /= devisor;
        }
    return *result;
}

template <typename G>
Matrix<G> & Matrix<G>::T() {
    Matrix<G> *result = new Matrix(w, h);
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
            (*result)[j][i] = (*this)[i][j];
    return *result;
}

template <typename U>
Matrix<U> Minor(const Matrix<U>&m, size_t i, size_t j){
    Matrix<U>* result = new Matrix<U>(m.get_Height()-1, m.get_Width()-1);
    for(size_t I=0; I<m.get_Height(); I++) {
        if (i == I) continue;
        for (size_t J = 0; J < m.get_Width(); J++) {
            if (j == J) continue;
            int I1 = (I > i) ? I - 1 : I;
            int J1 = (J > j) ? J - 1 : J;
            (*result)[I1][J1] = m[I][J];
        }
    }
    return *result;
}

template<typename U>
U det(const Matrix<U>& m){
    if (not m.is_square()) throw std::runtime_error("Error:  your matrix is not square matrix, so you cant get det()");

    if(m.get_Width() == 1) return m[0][0];

    if (m.get_Width() == 2) return m[0][0] * m[1][1] - m[0][1] * m[1][0];

    double result = 0.;
    for(int j=0; j < m.get_Width(); j++){
        result += SIGN(j) * det(Minor(m,0,j)) * m[0][j];
    }
    return result;
}

template< typename G>
Matrix<G>& Matrix<G>::operator+(Matrix<G> &another) {
    Matrix<G> *result = new Matrix<G>(h, w);
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            (*result)[i][j] = m[i][j] + another[i][j];
        }
    }

    return *result;
}

template<typename T>
T* Matrix<T>::operator[](size_t row){
    return m[row];
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m){
    for(int i=0; i<m.h; ++i){
        for(int j=0; j<m.w; ++j){
            os << m.m[i][j] << "\t\t" ;
        }
        os << std::endl;
    }
    return os;
}

template <typename G>
bool Matrix<G>::check_size(const Matrix<G>& another){
    return h == another.get_Height() and w == another.get_Width();
}

template <typename G>
bool Matrix<G>::is_square(){
    return h == w;
}

template <typename G>
bool Matrix<G>::is_multiplicable(const Matrix<G> &another) {
    return true;
}



#endif //MATHEMATICA_PROJECT_MATRIX_IMPLEMENTATION_H
