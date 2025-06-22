#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

#define M_PI 3.14159265358979323846

// f(x) = sqrt((9-2x)/(2x-21))
double f(double x) {
    return sqrt((9 - 2 * x) / (2 * x - 21));
}

// КФ трапеций
double trapezoidalRule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    return h * sum;
}

// КФ Симпсона
double simpsonRule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x0 = a + i * h;
        double x1 = a + (i + 1) * h;
        double xm = (x0 + x1) / 2.0;

        sum += f(x0) + 4 * f(xm) + f(x1);
    }

    return (h / 6.0) * sum;
}

// Метод численного интегрирования с использованием правила Рунге
double rungeIntegration(double a, double b, double eps, int m, bool useTrapezoidal) {
    int n = 2; // Начальное количество разбиений
    int k = 0; // Счетчик итераций
    double h = (b - a) / n; // Начальный шаг

    // Вычисляем первое приближение интеграла
    double Q_h = useTrapezoidal ? trapezoidalRule(a, b, n) : simpsonRule(a, b, n);
    cout << "N=" << n << ", h=" << h << ", Q_h=" << setprecision(7) << fixed << Q_h << endl;

    double Q_h_div_2, R_h_div_2;

    do {
        n *= 2;
        k++;
        h /= 2;

        // Вычисляем следующее приближение интеграла с шагом h/2
        Q_h_div_2 = useTrapezoidal ? trapezoidalRule(a, b, n) : simpsonRule(a, b, n);

        // Оценка погрешности по правилу Рунге
        R_h_div_2 = (Q_h_div_2 - Q_h) / (pow(2, m) - 1);

        cout << n << "N=" << 2 * n << ", h/" << pow(2, k) << "=" << h << ", Q_h/2^" << k << "=" << Q_h_div_2
            << ", R_h/2^" << k << "=" << R_h_div_2 << endl;

        Q_h = Q_h_div_2;

    } while (fabs(R_h_div_2) > eps); 

    return Q_h_div_2;
}

// Вычисление интеграла с помощью КФ НАСТ с 5 узлами
double gaussLegendreIntegration(double a, double b) {
    // Узлы КФ НАСТ (корни многочлена Лежандра) на отрезке [-1, 1]
    const double x_k[5] = { -0.9061798459, -0.5384693101, 0.0, 0.5384693101, 0.9061798459 };

    // Веса КФ НАСТ
    const double A_k[5] = { 0.2369268851, 0.4786286705, 0.5688888889, 0.4786286705, 0.2369268851 };

    double sum = 0.0;

    // Вычисление интеграла
    double mid = (b + a) / 2.0;
    double half_length = (b - a) / 2.0;

    for (int i = 0; i < 5; i++) {
        // Преобразование координаты от [-1, 1] к [a, b]
        double x = mid + half_length * x_k[i];
        sum += A_k[i] * f(x);
    }
    return half_length * sum;
}

int main() {
    setlocale(LC_ALL, "ru");

    double a = 6.0;        
    double b = 9.0;        
    double eps = 1e-7;     
    double exact_value = M_PI;

    cout << "Численное интегрирование интеграла sqrt((9-2x)/(2x-21)) от " << a << " до " << b << endl;
    cout << "Требуемая точность: " << eps << endl;
    cout << "Точное значение интеграла: " << setprecision(7) << fixed << exact_value << endl << endl;

    cout << "Квадратурная формула трапеций:" << endl;
    double trap_result = rungeIntegration(a, b, eps, 2, true);
    cout << "Итоговое значение (КФ трапеций): " << trap_result << endl;
    cout << "Абсолютная погрешность: " << fabs(trap_result - exact_value) << endl << endl;

    cout << "Квадратурная формула Симпсона:" << endl;
    double simpson_result = rungeIntegration(a, b, eps, 4, false);
    cout << "Итоговое значение (КФ Симпсона): " << simpson_result << endl;
    cout << "Абсолютная погрешность: " << fabs(simpson_result - exact_value) << endl << endl;

    cout << "Квадратурная формула НАСТ с 5 узлами:" << endl;
    double gauss_result = gaussLegendreIntegration(a, b);
    cout << "Значение интеграла (КФ НАСТ): " << setprecision(7) << fixed << gauss_result << endl;

    return 0;
}