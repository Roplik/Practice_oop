import std;
//include<iostream>
using namespace std;


int main()
{
    //1
    /*
    int a       = 0;
    int summa   = 0;
    int product = 0;
    print("Введите трехзначное число: ");
    cin >> a;
    int first   = (a / 100);
    int second  = ((a / 10 ) % 10);
    int third   = (a % 10);
    summa    = first + second + third;
    product  = first * second * third;
    println("Сумма цифр: {}; Произведение цифр: {};", summa, product);
    */

    //2
    /*
    int second = 0;
    cin >> second;
    println("Прошло: {} минут;", (second % 3600) / 60);
    */

    //3
    /*
    int n   = 0;
    int k   = 0;
    int res = 0;
    print("Введите n: ");
    cin >> n;
    print("Введите k: ");
    cin >> k;
    res = (k + n - 1) % 7;
    switch (res)
    {
        case 1: println("Понедельник"); break;
        case 2: println("Вторник");     break;
        case 3: println("Среда");       break;
        case 4: println("Четверг");     break;
        case 5: println("Пятница");     break;
        case 6: println("Суббота");     break;
        case 7: println("Воскресенье"); break;
        default:println("Ошибка");      break;
    }
    */

    //4
    /*
    int a = 0;
    print("Введите трехзначное число: ");
    cin >> a;
    int first   = (a / 100);
    int second  = ((a / 10 ) % 10);
    int third   = (a % 10);

    if(first < second && second < third)
    {
        println("Цифры данного числа образуют возрастающую последовательность");
    }
    else
    {
        println("Цифры данного числа НЕ образуют возрастающую последовательность");
    }
    */

    //5
    /*
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    // Можно сделать проверку на корректность ввода координат доски
    print("Введите x1: ");
    cin >> x1;
    print("Введите y1: ");
    cin >> y1;
    print("Введите x2: ");
    cin >> x2;
    print("Введите y2: ");
    cin >> y2;
    if((x1 == x2 && y1 != y2) || (y1 == y2 && x1 != x2))
    {
        println("Перемещение возможно");
    }
    else
    {
        println("Пермещение невозможно");
    }
    */

    //6
    /*
    int year = 0;
    cin >> year;
    int res = (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0) ? 366 : 365;
    println("В этом году {} дня", res);
    */

    //7
    /*
    int num = 0;
    cin >> num;
    
    int count = 0;
    
    if ((num / 100) > 0)
    count = 3;
    else if((num / 10) > 0)
    count = 2;
    else
    count = 1;
    
    switch (count)
    {
        case 1:
        if(num % 2 == 0)
        println("Четное однозначное число");
        else
        println("Нечетное однозначное число");
        break;
        case 2:
        if(num % 2 == 0)
        println("Четное двухзначное число");
        else
        println("Нечетное двухзначное число");
        break;
        case 3:
        if(num % 2 == 0)
        println("Четное трехзначное число");
        else
        println("Нечетное трехзначное число");
        break;
        
        default:
        break;
    }
    */
    
    //8
    /*
    int number;
    cin >> number;
    
    int hundreds = number / 100;
    int tens = (number / 10) % 10;
    int ones = number % 10;
    
    
    switch (hundreds) 
    {
        case 1: print("сто"); break;
        case 2: print("двести"); break;
        case 3: print("триста"); break;
        case 4: print("четыреста"); break;
        case 5: print("пятьсот"); break;
        case 6: print("шестьсот"); break;
        case 7: print("семьсот"); break;
        case 8: print("восемьсот"); break;
        case 9: print("девятьсот"); break;
    }
    
    if(hundreds != 0)
    print(" ");
    
    if (tens == 1) 
    {
        switch (ones) 
        {
            case 0: print("десять"); break;
            case 1: print("одиннадцать"); break;
            case 2: print("двенадцать"); break;
            case 3: print("тринадцать"); break;
            case 4: print("четырнадцать"); break;
            case 5: print("пятнадцать"); break;
            case 6: print("шестнадцать"); break;
            case 7: print("семнадцать"); break;
            case 8: print("восемнадцать"); break;
            case 9: print("девятнадцать"); break;
        }
        return 0;
    } 
    else 
    {
        switch (tens) 
        {
            case 2: print("двадцать"); break;
            case 3: print("тридцать"); break;
            case 4: print("сорок"); break;
            case 5: print("пятьдесят"); break;
            case 6: print("шестьдесят"); break;
            case 7: print("семьдесят"); break;
            case 8: print("восемьдесят");break;
            case 9: print("девяносто"); break;
        }
    }
    
    if (tens != 0) 
    print(" ");
    
    switch (ones) 
    {
        case 1: print("один"); break;
        case 2: print("два"); break;
        case 3: print("три"); break;
        case 4: print("четыре"); break;
        case 5: print("пять"); break;
        case 6: print("шесть"); break;
        case 7: print("семь"); break;
        case 8: print("восемь"); break;
        case 9: print("девять"); break;
    }
    */
    
    
    //9
    /*
    int a = 0;
    int b = 0;
    cin >> a >> b;
    println("Все числа между {} и {};", a, b);
    for(int i = a; i <= b; ++i)
    {
        print("{} ", i);
    }
    println("\nВсего чисел: {}", b - a + 1);
    */
    
    //10
    /*
    int a = 0;
    int n = 0;
    cin >> a >> n;
    int res = a;
    println("Степени числа {}:", a);
    for(int i = 1; i <= n; ++i)
    {
        print("{} ", res);
        res *= a;
    }
    */
}
