#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//список
template<typename LT>
class List
{
private:
    //ячейка
    template<typename NT>
    class Node
    {
    public:
        NT data; //данные
        Node* next; //указатель на следующий элемент
        Node(NT _data=NT(), Node* _next=NULL) : data(_data), next(_next) {}
        ~Node(){}
    };

    Node<LT>* head; //указатель на говолу
    Node<LT>* end;  //указатель на конец
    int SIZE;       //размер списка

public:
    List() : head(NULL), end(NULL), SIZE(0) {}
    ~List();

    void push_back(const LT& _data); // добавление в конец списка
    void pop      (int index);       // удаление index элемнта
    void print    ();                //вывод всего списка
    LT& operator[](const int index); //дооступ к элементу

    bool empty() { return (SIZE == 0)? true : false; } // проверка на пустоту списка
    int  size()  { return SIZE; }                      // определение размера списка
};

template<typename LT>
List<LT>::~List()
/*Удаляем список*/
{
    Node<LT>* tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

template<typename LT>
void List<LT>::print()
/*выводим список*/
{
    Node<LT>* tmp = head; //создаем временнный объект для обхода

    //пока не конец списка выводи элемнты
    while (tmp != NULL)
    {
        cout << tmp->data << ' ';
        tmp = tmp->next;
    }
}

template<typename LT>
void List<LT>::push_back(const LT& _data)
/*Добавление в конец списка*/
{
    //если элемнтов нет
    if (SIZE == 0)
    {
        head = new Node<LT>(); //создаем елемент
        head->data = _data;    //заполняем
        end = head;            //присваем новый хвост
    }
    else
    {
        Node<LT>* tmp = new Node<LT>(); //создаем елемент
        tmp->data = _data;              //заполняем
        end->next = tmp;                //говорим что n-ый элемент указывает на наш новый элемент
        end = tmp;                      //присваем новый хвост
    }

    SIZE++; //увеливаем количтво элементов
}

template<typename LT>
void List<LT>::pop(int index)
/* Удаление index элемента */
{
    if ((head != NULL) && (index >= 0) && (index < SIZE)) // если по этому номеру что-то лежит и этот элемент внутри списка
    {
        // Mass - объекты, которые хранятся в списке
        Node<LT> *tmp = head, *helping = head;

        for (int i = 0; i < index; i++)
        {
            helping = tmp; // предыдущее значение temp
            tmp = tmp->next;
        }

        if (tmp == head) // если элемент который надо удалить первый
        {
            head = tmp->next;
        }
        else
        {
            helping->next = tmp->next;
        }

        delete(tmp);

        SIZE--; // уменьшаем размер списка
    }
    else
    {
        throw -1;
    }

}
template<typename LT>
LT& List<LT>::operator[](const int index)
{
    //если индекс элемента правильный
    if (index >= 0 && index < SIZE)
    {
        //создаю временный указатель на элемент для обхода списка
        Node<LT>* tmp = head;

        int count = 0; //счетчик
        while (count < index) //пока не найден нужный элемент
        {
            tmp = tmp->next; // переходи на следующий элемент
            count++;
        }

        return tmp->data; //возвращаю найденный данные
    }

    //индекс введен с ошибкой, возврат исключения
    throw std::invalid_argument("Error index");
}
