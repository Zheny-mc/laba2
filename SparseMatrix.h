#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include "List.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::setw;

#define NULL_STR -1
#define MAX_BUFFER 32768

//---класс ленточно-строчного формата матрицы---
class SS
{
private:
    static int count;

    int size;  
    int high_size_stip;    
    int low_size_stip;    
    int len_stip;  
    List<int>* stip; 

    void input_data_matrix(std::ifstream& fin);
    void create_matrix(std::ifstream& fin);
    void check_indexes(int i, int j) const;
    
public:
    //создание объекта
    SS(const string& file_name);
    SS(const int _size, const int _high_size_stip, const int _low_size_stip);
    ~SS() { delete[] stip; count--;}                   

    int get_size()           const { return size; }
    int get_high_size_stip() const { return high_size_stip; }
    int get_low_size_stip()  const { return low_size_stip;  }
    int get_len_stip()       const { return len_stip; }

    //доступ к элементу
    void set_elem(int i, int  j, const int data);
    int  get_elem(int i, int j) const;   

    //вывод
    string _is_string();

    //вычитание матриц
    friend SS operator-(const SS& lMatrix, const SS& rMatrix);
};



//класс разреженно-строчного формата матрицы
class CRS
{
private:
    List<int> value; // элементы 
    List<int> LI;    // номер 1 элемента i строки
    List<int> LJ;    // номер столбца элемнта

public:
    CRS(const SS& matrix); 
    ~CRS(){}               
    string _is_string();
    
    bool is_check_crs_matrix(const string& file_output);
    
private:
    bool is_find_incongruities(std::ifstream& f, List<int>& array);
};
