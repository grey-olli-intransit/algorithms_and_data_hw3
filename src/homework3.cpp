#include <iostream>
#include <string>

#define BOARD_SIZE 8

// Реализовать функцию перевода чисел из десятичной системы в двоичную, используя рекурсию.
void to_binary(int x, std::string & result) {
    int y=x/2;

    if (x==0) {
        result.insert(0,"0");
	std::cout << " Это число в двоичном представлении:" << std::endl << " " << result << std::endl;
        return;
    }

    if (x==1) {
        result.insert(0,"1");
	std::cout << " Это число в двоичном представлении:" << std::endl << " " << result << std::endl;
        return;
    }

    if (y==1) {
	result.insert(0,std::__cxx11::to_string(x%2));
        result.insert(0,"1");
	std::cout << " Это число в двоичном представлении:" << std::endl << " " << result << std::endl;
	return;
    }
    else {
        result.insert(0,std::__cxx11::to_string(x%2));
	to_binary(y, result);
    }
}

// нерекурсивное
long int power(int a, int b) {
    long int pow=1;
    while(b--) 
      pow*=a;
    return pow;
}

// рекурсивное
long int power_recursion(int a, int b) {
    long int power=1;
    if (b==0)
	 return power;
    if (b==1)
	 return a;
    return a*power_recursion(a,b-1);
    
}

// рекурсивное с использованием чётности степени
long int power_recursion_even(int a, int b) {
    long int power=1;
    if (b==0)
	 return power;
    if (b==1)
	 return a;
    if (b%2 == 0)
	 return a*a*power_recursion_even(a,b-2);
    return a*power_recursion_even(a,b-1);
    
}

// Рассмотрим движение из левого верхнего угла в правый нижний.
// в ячейку можно попасть по диагонали S(m-1,n-1) и S(m+1,n-1), и из соседних ячеек
// S(m-1, n) , S(m, n-1). Диагональные дороги справа-налево-вверх,слева-направо-вверх
// и элементы справа и снизу не рассматриваем как часть возвратного движения.
// Пусть препятствие - дружественные пешки. Тогда нельзя попасть из
// занятых ими ячеек. Будем вычитать такой маршрут из S(m,n) обозначив его
// как Z(m-1,n) , Z(m,n-1), Z(m-1,n-1). Так как вычитается только один маршрут, а S(m,n)
// является суммой маршрутов, то надо умножать на Z соответствующий маршрут чтобы аннулировались
// все маршруты до занятой клетки.
// кроме того,не считая путей к клеткам источникам текущего шага, в кажду клетку есть 4 пути из
// клеток источников этого шага: с верхней, боковой и диагональных ячеек слева и справа,
// или менее, если соответствующие клетки заняты, чтобы вычесть занятые - вычитаем соответствующее Z.
// итого ходов в некоторую клектку:
//S(m, n) = 4 - Z(m-1,n) - Z(m,n-1) - Z(m-1,n-1) - Z(m+1,n-1) + S(m-1, n) + S(m, n-1) + S(m-1,n-1) + S(m+1,n-1) - Z(m-1,n)*S(m-1, n) - Z(m,n-1)*S(m, n-1) - Z(m-1,n-1)*S(m-1,n-1) -Z(m+1,n-1)*S(m+1,n-1) - не у края доски
//S(m, n) = 3 - Z(m-1,n) - Z(m,n-1) - Z(m-1,n-1) + S(m-1, n) + S(m, n-1) + S(m-1,n-1) - Z(m-1,n)*S(m-1, n) - Z(m,n-1)*S(m, n-1) - Z(m-1,n-1)*S(m-1,n-1) у правого края доски
//S(m, n) = 2 - Z(m,n-1) - Z(m+1,n-1) + S(m, n-1) + S(m+1,n-1) - Z(m,n-1)*S(m, n-1) - Z(m+1,n-1)*S(m+1,n-1) у левого края доски
//S(m, 0) = S(m-1,0) - Z(m-1,0)*S(m-1,0), если вторая координата равна нулю (верхний край), т.е. вообще 0, если хотя бы одна клетка занята
//(двигаться снизу вверх и по аналогичной диагонали считаем невозможным для упрощения задачи)
//S(m, n) = 0, если m и n это стартовые координаты 0,0

// для описания свободна ли клетка введём функцию busy с фиксированным количеством
// препятствий возвращающую 0 для не занятой и 1 для занятой клетки.
int busy(int x, int y) {
    // 4 пешки
    if (y==1 && (x==1 || x==2 || x==3 || x==4))
        return 1;
    // ещё 1 пешка
    if (y==0 && x==3)
        return 1;
    return 0;
}


int routes(int x, int y) {
    if (busy(x,y)) // считаем что 0 маршрутов у занятых клеток
        return 0;
    if (x == 0 && y == 0)
        return 0;
    else if (x == 0 ) {
        if (y==1) {
	   if (busy(0,1))
               return 0;
	   else 
	       return 1;
	}
        //S(m, n) = 2 - Z(m,n-1) - Z(m+1,n-1) + S(m, n-1) + S(m+1,n-1) - Z(m,n-1)*S(m, n-1) - Z(m+1,n-1)*S(m+1,n-1) у левого края доски
        if ( y - 1 > 0 )
           return 2 - busy(0,y-1) - busy(x+1,y-1) + routes(0,y-1) - busy(0,y-1)*routes(0,y-1) + routes(x+1,y-1) - busy(x+1,y-1)*routes(x+1,y-1); // левый край
	else 
	   return 2 - busy(x+1,y-1) + routes(x+1,y-1) - busy(x+1,y-1)*routes(x+1,y-1); // левый край следующая от 0 клетка
    } // x == 0
    else if (y == 0) {
	 if (x==1) {
	     if (busy(1,0))
                 return 0;
	     else 
	         return 1;
	 }
         //S(m, 0) = 1 + S(m-1,0) , если вторая координата равна нулю (верхний край), но вообще 0, если хотя бы одна клетка занята
         int has_busy=0;
         for(int c=x-1;c>0;c--)
	     if (busy(c,0))
	         has_busy=1;
	 if(has_busy)
	     return 0;
	 else 
             return 1 + routes(x-1,0);

    } // y == 0
    else if (x == BOARD_SIZE - 1) {
        //S(m, n) = 3 - Z(m-1,n) - Z(m,n-1) - Z(m-1,n-1) + S(m-1, n) + S(m, n-1) + S(m-1,n-1) - Z(m-1,n)*S(m-1, n) - Z(m,n-1)*S(m, n-1) - Z(m-1,n-1)*S(m-1,n-1) у правого края доски
	return 3 - busy(x-1,y) - busy(x,y-1) - busy(x-1,y-1) + routes(x-1,y) + routes(x,y-1) + routes(x-1,y-1) - busy(x-1,y)*routes(x-1,y) - busy(x,y-1)*routes(x,y-1) - busy(x-1,y-1)*routes(x-1,y-1);
    }
    else
        //S(m, n) = 4 - Z(m-1,n) - Z(m,n-1) - Z(m-1,n-1) - Z(m+1,n-1) + S(m-1, n) + S(m, n-1) + S(m-1,n-1) + S(m+1,n-1) - Z(m-1,n)*S(m-1, n) - Z(m,n-1)*S(m, n-1) - Z(m-1,n-1)*S(m-1,n-1) -Z(m+1,n-1)*S(m+1,n-1) - не у края доски
        return 4 - busy(x-1,y) - busy(x,y-1) - busy(x-1,y-1) - busy(x+1,y-1) + routes(x-1, y) + routes(x, y-1) + routes(x-1,y-1) + routes(x+1,y-1) - busy(x-1,y)*routes(x-1,y) - busy(x,y-1)*routes(x,y-1) - busy(x-1,y-1)*routes(x-1,y-1) - busy(x+1,y-1)*routes(x+1,y-1);

}

int board[BOARD_SIZE][BOARD_SIZE] = {};

void print_board() {
    for(int i=0;i<BOARD_SIZE;i++) {
        for(int j=0;j<BOARD_SIZE;j++)
            printf("%8d", routes(j, i));
        printf("\n");
    }
}

int main(int argc, char ** argv) {

    print_board();

    std::string result="";

    int x,y,z;
    std::cout << "Введите целое положительное число: ";
    std::cin >> x;

    to_binary(x, result);

    std::cout << "Введите целое число (основание): ";
    std::cin >> x;
    std::cout << "Введите целое число (степень): ";
    std::cin >> y;

    z=power(x,y);
    std::cout << " " << x << " в степени " << y << " = " << z << std::endl;

    z=power_recursion(x,y);
    std::cout << " " << x << " в степени " << y << " = " << z << std::endl;
    
    z=power_recursion_even(x,y);
    std::cout << " " << x << " в степени " << y << " = " << z << std::endl;
    return 0;
}
