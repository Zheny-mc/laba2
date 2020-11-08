#include "SparseMatrix.h"
int SS::count = 0;

void SS::input_data_matrix(std::ifstream& fin)
//ф-я ввода данных для создания разреженной ленточной матрицы  
{ 
    fin >> size; 
    cout << "size = " << size << endl;
    fin >> high_size_stip;
    cout << "high_size_stip = " << high_size_stip << endl;
    fin >> low_size_stip;
    cout << "low_size_stip = " << low_size_stip << endl;

    //проверка на корректность введенных парметров матрицы
    if (size <= 0 ||  
        high_size_stip >= size || high_size_stip < 0 ||
        low_size_stip  >= size || low_size_stip  < 0)
        throw std::invalid_argument("invalid data was entered for creating the matrix");
}

void SS::create_matrix(std::ifstream& fin)
//заполнения матрицы
{    
    int cur_value;

    for (int i = 0; i < low_size_stip; i++) 
    {
        for (int j = 0; j <= high_size_stip; j++)
        {
            fin >> cur_value;
            set_elem(i, j, cur_value);
        }
    }

    for (int i = low_size_stip, j = 0; i < size; i++, j++)
    {
        for (int j1 = j; (j1 < j + len_stip) && (j1 < size); j1++)
        {
            fin >> cur_value;
            set_elem(i, j1, cur_value);
        }
    }
}

void SS::check_indexes(int i, int j) const 
//проверка индексов для доступа к элементам матрицы
{
    if (i < 0 || i >= size || j < 0 || j >= size)
        throw std::invalid_argument("index error");
}

// конструктор по значению
SS::SS(const string& file_name)
{
    count++;
    cout << "Matrix " << count << ": "  << endl;

    //открывая файл
    std::ifstream fin(file_name);

    if ( fin.is_open() )
    {
        //считываю данные о матрице
        input_data_matrix(fin);
        len_stip = high_size_stip + low_size_stip + 1;

        //выделение памяти для ленты N-строк, len - столбцов
        stip = new List<int>[size];

        for (int i = 0; i < size; i++)
            for (int j = 0; j < len_stip; j++)
                stip[i].push_back(0);
        
        create_matrix(fin);

        fin.close();
    }
}

// конструктор по значению
SS::SS(const int _size, 
       const int _high_size_stip, 
       const int _low_size_stip) : size(_size), 
                                   high_size_stip(_high_size_stip), 
                                   low_size_stip (_low_size_stip),
                                   len_stip(_high_size_stip + _low_size_stip + 1)
{
    count++;

    stip = new List<int>[size];

    for (int i = 0; i < size; i++)
        for (int j = 0; j < len_stip; j++)
            stip[i].push_back(0);
}


void SS::set_elem(int i, int j, const int data)
// заполнение ячейки матрицы
{
    check_indexes(i, j);
    
    //если элемент находиться в ленте, то запиши значение в ячейку
    if ((i < j && (j - i <= high_size_stip)) || (i > j && (i - j <= low_size_stip)) || (i == j))
    {
        stip[i][j - i + low_size_stip] = data;
        return;
    }

    //элемент находиться не в ленте, выдай предупреждение
    throw std::invalid_argument("Еhe element is located outside the feed");
}


int SS::get_elem(int i, int j) const 
// получение значения ячейки матрицы
{
    check_indexes(i, j);
    
    //если элемент находиться в ленте, то верни значение
    if ((i < j && (j - i <= high_size_stip)) || (i > j && (i - j <= low_size_stip)) || (i == j))
        return stip[i][j - i + low_size_stip];

    //элемент находиться не в ленте, выдай 0
    return 0;
}


string SS::_is_string()
//вывод
{
    std::stringstream ss;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            ss << setw(3) << get_elem(i, j) << " ";
        }
        ss << "\n";
    }

    return ss.str();
}


SS operator-(const SS& lMatrix, const SS& rMatrix)
//вычитание матриц
{ 
    if (lMatrix.size != rMatrix.size) 
        throw std::invalid_argument("Matrices of different sizes");

    int size = lMatrix.size;

    int l_high_size_stip = lMatrix.get_high_size_stip();
    int l_low_size_stip  = lMatrix.get_low_size_stip(); 

    int r_high_size_stip = rMatrix.get_high_size_stip();
    int r_low_size_stip  = rMatrix.get_low_size_stip(); 

    int high_size_stip = (l_high_size_stip > r_high_size_stip)? l_high_size_stip : r_high_size_stip;
    int low_size_stip  = (l_low_size_stip  > r_low_size_stip)?  l_low_size_stip  : r_low_size_stip;


    SS Matrix(size, high_size_stip, low_size_stip);
    int cur_value;

    //заполнение
    for (int i = 0; i < low_size_stip; i++) 
    {
        for (int j = 0; j <= high_size_stip; j++)
        {
            cur_value = lMatrix.get_elem(i, j) - rMatrix.get_elem(i, j); // нахождение разницы элемнтов
            Matrix.set_elem(i, j, cur_value);                       // запись полученного значения
        }
    }

    for (int i = low_size_stip, j = 0; i < size; i++, j++)
    {
        for (int j1 = j; (j1 < j + Matrix.len_stip) && (j1 < size); j1++)
        {
            cur_value = lMatrix.get_elem(i, j1) - rMatrix.get_elem(i, j1); // запись полученного значения
            Matrix.set_elem(i, j1, cur_value);                        // запись полученного значения
        }
    }    
    
    return Matrix;
}
//**********************конец класса SS*******************************

//************************класс SRS**********************************
CRS::CRS(const SS& matrix)
// конструктор копирования
{
    int  n = 0;     //номер элемента
    int  elem;      //значение эелемнта
    bool is_elem; //флаг, показывающий есть ли хотя бы 1 элемент в строке
    
    for (int i = 0; i < matrix.get_low_size_stip(); ++i) 
    {
        is_elem = false; //изначально элемнта нет
        
        //запись элементов в матрицу
        for (int j = 0; j <= matrix.get_high_size_stip(); ++j)
        {
            elem = matrix.get_elem(i, j); 
            if (elem != 0)
            {
                value.push_back(elem);
                LJ.push_back(j);
                
                if (!is_elem)
                    LI.push_back(n);
                is_elem = true;
                ++n;
            }
        }

        //если элемента не нашлось, запиши пустую строку
        if (!is_elem)
            LI.push_back(NULL_STR);
    }
    

    for (int i = matrix.get_low_size_stip(), j = 0; i < matrix.get_size(); ++i, ++j)
    {
        is_elem = false; //изначально элемнта нет

        //запись элементов в матрицу
        for (int j1 = j; (j1 < j+matrix.get_len_stip()) && (j1 < matrix.get_size()); ++j1)
        {
            elem = matrix.get_elem(i, j1); 
            if (elem != 0)
            {
                value.push_back(elem);
                LJ.push_back(j1);
                
                if (!is_elem)
                    LI.push_back(n);
                is_elem = true;

                ++n;
            }
        }

        //если элемента не нашлось, запиши пустую строку
        if (!is_elem)
            LI.push_back(NULL_STR);
    }

    //запись кол-ва элементов
    n += 2; //количество NZ + 1, индекс +1
    LI.push_back(n);

    //обработка пустых строк LI
    for (int i = 0; i < LI.size()-1; i++)
    {
        //записываю пустые строки
        if (LI[i] == -1)
            LI[i] = LI[i+1];
    }

}

string CRS::_is_string()
// вывод 
{
    std::stringstream ss;

    //вывод номеров для опредления индекса
    ss << "    N: ";
    for (int i = 0; i < value.size(); i++)
    {
        ss << setw(3) << i;
    }
    ss << "\n";

    //вывод value
    ss << "value: ";
    for (int i = 0; i < value.size(); i++)
    {
        ss << setw(3) << value[i];
    }
    ss << "\n";

    //вывод LJ
    ss << "   LJ: ";
    for (int i = 0; i < LJ.size(); i++)
    {
        ss << setw(3) << LJ[i];
    }
    ss << "\n";
    
    //вывод LI
    ss << "   LI: ";
    for (int i = 0; i < LI.size(); i++)
    {
        ss << setw(3) << LI[i];
    }
    ss << "\n";

    return ss.str();
}

bool CRS::is_check_crs_matrix(const string& file_output)
{
    int elem;
    std::ifstream f(file_output);

    if (f.is_open())
    {   
        return is_find_incongruities(f, value) && 
               is_find_incongruities(f, LJ) && 
               is_find_incongruities(f, LI);
    }
    else
        throw std::invalid_argument("Error! File dont open(" + file_output + ")");
}


bool CRS::is_find_incongruities(std::ifstream& f, List<int>& array)
{
    int elem;
    for (int i = 0; i < array.size() && !f.eof(); i++)
    {
        f >> elem;
        if (array[i] != elem) 
            return false;
    }
    return true;
}
//******************конец класса CRS**************************************
