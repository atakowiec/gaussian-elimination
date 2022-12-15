#include <iostream>

using namespace std;

void readMatrix(double**, int);
void printMatrix(double **matrix, int n);
void printResult(double *array, int n);
void printResultInOrder(double *array, int n, int* order);
void roundMatrix(double **matrix, int n);
double* postepowanieOdwrotne(double **matrix, int n);
int* findMax(double **matrix, int n, int x, int y);
void swapColumns(double **matrix, int n, int column1, int column2);
void swapRows(double **matrix, int row1, int row2);
void podstawowy();
void pojedynczyWybor();
void pelnyWybor();

int main() {
    int wybor;
    do {
        cout << "\n\nWybierz metode: \n"
                "0 - wyjscie\n"
                "1 - podstawowa\n"
                "2 - z wyborem elementu maksymalnego w kolumnie\n"
                "3 - z pełnym wyborem elementu maksymalnego\n"
                "\n"
                "Twoj wybor: ";
        cin >> wybor;
        switch (wybor) {
            case 0: break;
            case 1: {
                podstawowy();
                break;
            }
            case 2: {
                pojedynczyWybor();
                break;
            }
            case 3: {
                pelnyWybor();
                break;
            }
            default: {
                cout << "Niepoprawna opcja! " << endl;
            }
        }
    } while(wybor != 0);

    return 0;
}

// funkcja pobierajaca wszystkie potrzebne elementy macierzy o rozmiarze n
void readMatrix(double **matrix, int n) {
    for(int i=0; i<n; i++) {
        double *row = new double[n+1];
        for(int j=0; j<n+1; j++) {
            if(j == n)
                cout << "Wyraz wolny dla "<<i<<" wiersza: ";
            else
                cout << "matrix[" << i << "][" << j << "] = ";
            cin >> row[j];
        }
        matrix[i] = row;
    }
}

void roundMatrix(double **matrix, int n) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<n+1; j++) {
            matrix[i][j] = abs(matrix[i][j]) < 1e-7 ? 0 : matrix[i][j];
        }
    }
}

int lengthOfDouble(double d) {
    string str = std::to_string (d);
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    return str.length();
}

void printResult(double *array, int n) {
    cout << "------------------------------------------\nRozwiazanie: \n";
    for(int i=0; i<n; i++)
        cout << " x" << (i+1) << " = " << array[i] << "\n";
    cout << endl;
}

void printResultInOrder(double *array, int n, int* order) {
    cout << "------------------------------------------\nRozwiazanie: \n";
    for(int i=0; i<n; i++) {
        for (int j = 0; j < n; j++) {
            if (order[j] == i) {
                cout << " x" << (i + 1) << " = " << array[j] << "\n";
                break;
            }
        }
    }
    cout << endl;
}

void printMatrix(double **matrix, int n) {
    int maxLength = 0;
    for(int i=0; i<n; i++) {
        for (int j = 0; j < n; j++) {
            int len = lengthOfDouble(matrix[i][j]);
            maxLength = maxLength > len ? maxLength : len;
        }
    }
    for(int i=0; i<n; i++) {
        cout << "|";
        for(int j=0; j<n+1; j++) {
            cout << " " <<matrix[i][j];
            for(int k=0; k<maxLength-lengthOfDouble(matrix[i][j])+1; k++)
                cout << " ";
        }
        cout << " |" << endl;
    }
}

void podstawowy() {
    double **matrix;
    int n = 0;
    int wybor = 0;
    // petla obslugujaca wybor danych do zadania
    do {
        cout << "Wybor danych:\n"
                "1 - predefiniowane dane\n"
                "2 - podaj wlasne dane\n"
                "\nTwoj wybor: ";
        cin >> wybor;
        if(wybor == 2) {
            do {
                cout << "Podaj ilosc zmiennych w ukladzie: ";
                cin >> n;
                if(n <= 0) cout << "Liczba zmiennych musi byc wieksza od 0!" << endl;
            } while (n <= 0);
            matrix = new double *[n];
            readMatrix(matrix, n);
        } else if(wybor == 1) {
            n = 4;
            matrix = new double *[n];
            matrix[0] = new double[n+1]{1,1,0,-3,1};
            matrix[1] = new double[n+1]{1,4,-1,-4,-2};
            matrix[2] = new double[n+1]{0.5, 0.5, -3, -5.5, 1.5};
            matrix[3] = new double[n+1]{1.5, 3, -5, -9, -0.5};
        }
    } while(!(wybor == 1 || wybor == 2));

    cout << "\n------------------------------------------"
            "\n\n Obliczanie metoda podstawowa ukladu: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------";

    // podstawowa eliminacja gauss'a
    for(int k=0; k<n-1; k++) {
        if(matrix[k][k] == 0) {
            cout << "Podstawowa eliminacja gaussa zawiodla, matrix[" << k << "][" << k << "] = 0\n\n";
            printMatrix(matrix, n);
            return;
        }
        for(int i=k+1; i<n; i++) {
            double p = matrix[i][k]/matrix[k][k];
            for(int j=0; j<n+1; j++) {
                matrix[i][j] = matrix[i][j] - (p * matrix[k][j]);
            }
            roundMatrix(matrix, n);
        }
    }
    cout << "\n------------------------------------------";
    cout << "\n\n Po dokonaniu podstawowej elimincaji Gauss'a: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------\n";

    double last = matrix[n-1][n-1];
    if(last == 0) {
        if(matrix[n-1][n] == 0) {
            cout << "Uklad rownan nieoznaczony - nieskonczona ilosc rozwiazan\n";
        } else {
            cout << "Uklad rownan sprzeczny - brak rozwiazan\n";
        }
    } else {
        double* result = postepowanieOdwrotne(matrix, n);
        printResult(result, n);
    }
}

void pojedynczyWybor() {
    double **matrix;
    int n = 0;
    int wybor = 0;
    // petla obslugujaca wybor danych do zadania
    do {
        cout << "Wybor danych:\n"
                "1 - predefiniowane dane\n"
                "2 - podaj wlasne dane\n"
                "\nTwoj wybor: ";
        cin >> wybor;
        if(wybor == 2) {
            do {
                cout << "Podaj ilosc zmiennych w ukladzie: ";
                cin >> n;
                if(n <= 0) cout << "Liczba zmiennych musi byc wieksza od 0!" << endl;
            } while (n <= 0);
            matrix = new double *[n];
            readMatrix(matrix, n);
        } else if(wybor == 1) {
            n = 4;
            matrix = new double *[n];
            matrix[0] = new double[n+1]{2.25, -2.5, 4, -5.25, -1};
            matrix[1] = new double[n+1]{-3, -7.5, 6.5, 0, 17};
            matrix[2] = new double[n+1]{-6.25, -12.5, 0.25, 5.25, 24.25};
            matrix[3] = new double[n+1]{9, 10, 7, -21, -33};
        }
    } while(!(wybor == 1 || wybor == 2));

    int *columns = new int[n];
    for(int i=0; i<n; i++)
        columns[i] = i;


    cout << "\n------------------------------------------"
            "\n\n Obliczanie metoda z wyborem elementu maksymalnego w kolumnie ukladu: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------";

    // eliminacja gauss'a z wyborem elementu maksymalnego w kolumnie
    for(int k=0; k<n-1; k++) {
        int* maxPos = findMax(matrix, n, k, k);
        if(maxPos[0] != maxPos[1] && maxPos[1] != k) {
            swapRows(matrix, maxPos[1], k);
            int temp = columns[k];
            columns[k] = columns[maxPos[1]];
            columns[maxPos[1]] = temp;
        }

        if(matrix[k][k] == 0) {
            cout << "Eliminacja gaussa zawiodla, matrix[" << k << "][" << k << "] = 0\n\n";
            printMatrix(matrix, n);
            return;
        }
        for(int i=k+1; i<n; i++) {
            double p = matrix[i][k]/matrix[k][k];
            for(int j=0; j<n+1; j++) {
                matrix[i][j] = matrix[i][j] - (p * matrix[k][j]);
            }
            roundMatrix(matrix, n);
        }
    }
    cout << "\n------------------------------------------";
    cout << "\n\n Po dokonaniu  elimincaji Gauss'a z wyborem elementu maksymalnego w kolumnie;: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------\n";

    double last = matrix[n-1][n-1];
    if(last == 0) {
        if(matrix[n-1][n] == 0) {
            cout << "Uklad rownan nieoznaczony - nieskonczona ilosc rozwiazan\n";
        } else {
            cout << "Uklad rownan sprzeczny - brak rozwiazan\n";
        }
    } else {
        double* result = postepowanieOdwrotne(matrix, n);
        printResultInOrder(result, n, columns);
    }
}

void pelnyWybor() {
    double **matrix;
    int n = 0;
    int wybor = 0;
    // petla obslugujaca wybor danych do zadania
    do {
        cout << "Wybor danych:\n"
                "1 - predefiniowane dane\n"
                "2 - podaj wlasne dane\n"
                "\nTwoj wybor: ";
        cin >> wybor;
        if(wybor == 2) {
            do {
                cout << "Podaj ilosc zmiennych w ukladzie: ";
                cin >> n;
                if(n <= 0) cout << "Liczba zmiennych musi byc wieksza od 0!" << endl;
            } while (n <= 0);
            matrix = new double *[n];
            readMatrix(matrix, n);
        } else if(wybor == 1) {
            n = 4;
            matrix = new double *[n];
//            matrix[0] = new double[n+1]{2.25, -2.5, 4, -5.25, -1};
//            matrix[1] = new double[n+1]{-3, -7.5, 6.5, 0, 17};
//            matrix[2] = new double[n+1]{-6.25, -12.5, 0.25, 5.25, 24.25};
//            matrix[3] = new double[n+1]{9, 10, 7, -21, -33};
            matrix[0] = new double[n+1]{1,2,-1,2, 0};
            matrix[1] = new double[n+1]{1, 0, -2, 4, 4};
            matrix[2] = new double[n+1]{0, -3, 1.5, 7, 0};
            matrix[3] = new double[n+1]{0, -1, 1, 6, -1};
        }
    } while(!(wybor == 1 || wybor == 2));

    int *columns = new int[n];
    for(int i=0; i<n; i++)
        columns[i] = i;


    cout << "\n------------------------------------------"
            "\n\n Obliczanie metoda z pelnym wyborem elementu maksymalnego ukladu: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------";

    // eliminacja gauss'a z pelnym wyborem elementu maksymalnego
    for(int k=0; k<n-1; k++) {
        int* maxPos = findMax(matrix, n, k, k);
        if(maxPos[0] != maxPos[1] && maxPos[1] != k) {
            swapRows(matrix, maxPos[0], k);
            swapColumns(matrix, n, maxPos[1], k);

            int temp = columns[k];
            columns[k] = columns[maxPos[1]];
            columns[maxPos[1]] = temp;
        }

        if(matrix[k][k] == 0) {
            cout << "\nEliminacja gaussa zawiodla, matrix[" << k << "][" << k << "] = 0\n\n";
            printMatrix(matrix, n);
            return;
        }
        for(int i=k+1; i<n; i++) {
            double p = matrix[i][k]/matrix[k][k];
            for(int j=0; j<n+1; j++) {
                matrix[i][j] = matrix[i][j] - (p * matrix[k][j]);
            }
            roundMatrix(matrix, n);
        }
    }
    cout << "\n------------------------------------------";
    cout << "\n\n Po dokonaniu  elimincaji Gauss'a z pelnym wyborem elementu maksymalnego: \n\n";
    printMatrix(matrix, n);
    cout << "\n------------------------------------------\n";

    double last = matrix[n-1][n-1];
    if(last == 0) {
        if(matrix[n-1][n] == 0) {
            cout << "Uklad rownan nieoznaczony - nieskonczona ilosc rozwiazan\n";
        } else {
            cout << "Uklad rownan sprzeczny - brak rozwiazan\n";
        }
    } else {
        double* result = postepowanieOdwrotne(matrix, n);
        printResultInOrder(result, n, columns);
    }
}

double* postepowanieOdwrotne(double **matrix, int n) {
    double *res = new double[n];
    for(int i=n-1; i>=0; i--) {
        res[i] = matrix[i][n];
        for(int j=i+1; j<n; j++) {
            res[i] -= (matrix[i][j] * res[j]);
        }
        res[i] /= matrix[i][i];
        if(abs(res[i]) < 1e-7)
            res[i] = 0;
    }
    return res;
}

int *findMax(double **matrix, int n, int x, int y) {
    int *res = new int[2];
    double max = 0;
    for(int i=x; i<n; i++) {
        for (int j = y; j < n; j++) {
            if(abs(matrix[i][j]) > max) {
                res[0] = i;
                res[1] = j;
                max = abs(matrix[i][j]);
            }
        }
    }
    return res;
}

void swapColumns(double **matrix, int n, int column1, int column2) {
    for(int i=0; i<n; i++) {
        double temp = matrix[i][column1];
        matrix[i][column1] = matrix[i][column2];
        matrix[i][column2] = temp;
    }
}

void swapRows(double **matrix, int row1, int row2) {
    double *temp = matrix[row1];
    matrix[row1] = matrix[row2];
    matrix[row2] = temp;
}