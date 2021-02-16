//
// Created by alpharius on 09.02.2021.
//


#pragma once

template <typename T>
Vector<T>::~Vector(void) {len = 0; free(ptr);}

template <typename T>
Vector<T>::Vector(int size) {
    len = size;
    orientation = 'h';
    ptr = (T*) calloc(len, sizeof(T));
}

template <typename T>
Vector<T>::Vector(Vector<T> &other) {
    len = other.len;
    orientation = 'h';
    ptr = (T*) calloc(len, sizeof(T));
    for(int i=0; i<len; ++i){
        ptr[i] = other.ptr[i];
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> l) {
    orientation = 'h';
    len = l.size();
    ptr = (T*) calloc(len, sizeof(T));
    auto *_ptr = l.begin();
    for(int i=0; _ptr != l.end(); i++){
        ptr[i] = *(_ptr++);
    }
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& other){
	assert(len == other.len);
	Vector<T> result(len);
	for(int i=0; i<len; i++)
		result[i] = ptr[i] + other[i];
	return result;
}

template <typename T>
T Vector<T>::operator*(Vector<T>& other){
	assert(len = other.len);
	T result;
	for(int i=0; i<len; i++){
		result += ptr[i] * other[i];
	}
	return result;
}

template <typename T>
T& Vector<T>::operator[](int index){
	return ptr[index];
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>& other){
	free(ptr);
	ptr = (T*) calloc(other.len, sizeof(T));
	for(int i=0; i<len; i++) {
        ptr[i] = other[i];
    }
	return *this;
}

template <typename T, typename U>
Vector<T> operator*(Vector<T>& v,U&& u){
    Vector<T> result(v.len);
    for(int i=0; i<v.len; i++)
        result[i] = v[i]*u;
    return result;
}

template <typename T, typename U>
Vector<T> operator*(U&& u, Vector<T>& v){
    return v*u;
}

//template <typename G>
//Vector<G> operator^(const Vector<G>& other){
//
//}
