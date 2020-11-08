/***********************************************************/
/* ASD II лаба */
/* Автор:  Бубнов Евгений 19-ивт-2  */
/* Задача: Даны две разреженные ленточные матрицы. 
           Из одной матрицы вычесть другую и 
           результат занести в разреженную матрицу CRS.
*/    
/***********************************************************/
#include "SparseMatrix.h"

bool run(string& file_name_f_matrix, string& file_name_s_matrix, string& file_output);

int main()
{
    const int n_test = 2;
    string tests[n_test][3] = { {"test1/Matrix1.txt", "test1/Matrix2.txt", "test1/output.txt"}, {"test2/Matrix1.txt", "test2/Matrix2.txt", "test2/output.txt"} };

    try
    {
        for (int i = 0; i < n_test; i++)
        {
            cout << endl << "----------test" << i+1 << " begin----------" << endl;

            if (run(tests[i][0], tests[i][1], tests[i][2]))
                cout << endl << "----------test" << i+1 << " passed----------" << endl;
            else
                cout << endl << "----------test" << i+1 << " failed----------" << endl;
        }
    }
    catch(const std::exception& e)
    {
        cout << "Exception!" << endl << e.what() << endl;
    }
    
    
    return 0;
}

bool run(string& file_name_f_matrix, string& file_name_s_matrix, string& file_output)
{
    SS matrix1(file_name_f_matrix);
    cout << matrix1._is_string() << endl;
    
    SS matrix2(file_name_s_matrix);     
    cout << matrix2._is_string() << endl;
    
    //нахождение разницы двух матриц
    SS matrix = matrix1 - matrix2;
    cout << matrix._is_string() << endl;

    //преобразование ленточно-строчной матрицы в разреженно-строчный формат
    CRS crs_matrix(matrix);
    cout << crs_matrix._is_string() << endl; //вывод результата

    return crs_matrix.is_check_crs_matrix(file_output);
}







