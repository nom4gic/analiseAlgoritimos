#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <stdio.h>

using namespace std;
using namespace chrono;

// --------------------------------------------------
// bubble Sort
// --------------------------------------------------
void bubbleSort(vector<int>& valores)
{
    int tamanho = valores.size();

    for (int i = 0; i < tamanho - 1; i++)
    {
        for (int j = 0; j < tamanho - 1 - i; j++)
        {
            if (valores[j] > valores[j + 1])
            {
                int temp = valores[j];
                valores[j] = valores[j + 1];
                valores[j + 1] = temp;
            }
        }
    }
}

// --------------------------------------------------
// insertion Sort
// --------------------------------------------------
void insertionSort(vector<int>& valores)
{
    int tamanho = valores.size();

    for (int i = 1; i < tamanho; i++)
    {
        int valor = valores[i];
        int j = i - 1;

        while (j >= 0 && valores[j] > valor)
        {
            valores[j + 1] = valores[j];
            j--;
        }

        valores[j + 1] = valor;
    }
}

// --------------------------------------------------
// converte tamanho para texto
// --------------------------------------------------
string nomeTamanho(int n)
{
    if (n == 1000)
        return "1k";

    if (n == 5000)
        return "5k";

    if (n == 10000)
        return "10k";

    if (n == 50000)
        return "50k";

    if (n == 100000)
        return "100k";

    if (n == 200000)
        return "200k";

    if (n == 300000)
        return "300k";

    if (n == 500000)
        return "500k";

    if (n == 1000000)
        return "1000k";

    return to_string(n);
}

// --------------------------------------------------
// estrutura para armazenar os resultados
// --------------------------------------------------
struct Resultado
{
    int n;

    double tempoBubbleDesordenado;
    double tempoBubbleOrdenado;

    double tempoInsertionDesordenado;
    double tempoInsertionOrdenado;
};

int main()
{
    // Tamanhos solicitados pelo experimento
    vector<int> tamanhos = {
        1000,
        5000,
        10000,
        50000,
        100000,
        500000,
        1000000,
        2000000,
        3000000,
        5000000,
        10000000
    };

    // Gerador aleatório
    random_device rd;
    mt19937 gerador(rd());

    uniform_int_distribution<int> distribuicao(0, 10500);

    vector<Resultado> resultados;

    cout << "=============================================\n";
    cout << "      Experimento - Bubble Sort | Insertion Sort\n";
    cout << "=============================================\n\n";

    for (int n : tamanhos)
    {
        cout << "---------------------------------------------\n";
        cout << "Tamanho: " << nomeTamanho(n) << "\n";
        cout << "---------------------------------------------\n";

        // =========================================
        // 1. geração do vetor
        // =========================================

        vector<int> valores(n);

        for (int i = 0; i < n; i++)
        {
            valores[i] = distribuicao(gerador);
        }

        // =========================================
        // 2. criação dos vetores para os testes
        // =========================================

        // Os dois algoritmos recebem os mesmos valores
        vector<int> valoresBubble = valores;
        vector<int> valoresInsertion = valores;

        // =========================================
        // 3. ordenação - Bubble Sort desordenado
        // =========================================

        auto inicioBubbleDes =
            high_resolution_clock::now();

        bubbleSort(valoresBubble);

        auto fimBubbleDes =
            high_resolution_clock::now();

        double tempoBubbleDes =
            duration<double, milli>(
                fimBubbleDes - inicioBubbleDes
            ).count();

        // =========================================
        // 4. ordenação - Insertion Sort desordenado
        // =========================================

        auto inicioInsertionDes =
            high_resolution_clock::now();

        insertionSort(valoresInsertion);

        auto fimInsertionDes =
            high_resolution_clock::now();

        double tempoInsertionDes =
            duration<double, milli>(
                fimInsertionDes - inicioInsertionDes
            ).count();

        // =========================================
        // 5. criação do vetor já ordenado
        // =========================================

        // O vetor já ordenado será utilizado
        // nos testes dos dois algoritmos
        vector<int> valoresOrdenados = valoresBubble;

        vector<int> valoresBubbleOrdenado =
            valoresOrdenados;

        vector<int> valoresInsertionOrdenado =
            valoresOrdenados;

        // =========================================
        // 6. ordenação - Bubble Sort ordenado
        // =========================================

        auto inicioBubbleOrd =
            high_resolution_clock::now();

        bubbleSort(valoresBubbleOrdenado);

        auto fimBubbleOrd =
            high_resolution_clock::now();

        double tempoBubbleOrd =
            duration<double, milli>(
                fimBubbleOrd - inicioBubbleOrd
            ).count();

        // =========================================
        // 7. ordenação - Insertion Sort ordenado
        // =========================================

        auto inicioInsertionOrd =
            high_resolution_clock::now();

        insertionSort(valoresInsertionOrdenado);

        auto fimInsertionOrd =
            high_resolution_clock::now();

        double tempoInsertionOrd =
            duration<double, milli>(
                fimInsertionOrd - inicioInsertionOrd
            ).count();

        // =========================================
        // 8. armazenar resultados
        // =========================================

        Resultado resultado;

        resultado.n = n;

        resultado.tempoBubbleDesordenado =
            tempoBubbleDes;

        resultado.tempoBubbleOrdenado =
            tempoBubbleOrd;

        resultado.tempoInsertionDesordenado =
            tempoInsertionDes;

        resultado.tempoInsertionOrdenado =
            tempoInsertionOrd;

        resultados.push_back(resultado);

        // =========================================
        // 9. mostrar resultados
        // =========================================

        cout << fixed << setprecision(4);

        cout << "Bubble Sort desordenado:       "
            << tempoBubbleDes << " ms\n";

        cout << "Insertion Sort desordenado:   "
            << tempoInsertionDes << " ms\n";

        cout << "Bubble Sort ordenado:          "
            << tempoBubbleOrd << " ms\n";

        cout << "Insertion Sort ordenado:      "
            << tempoInsertionOrd << " ms\n\n";
    }

    // ==================================================
    // tabela final
    // ==================================================

    cout << "\n\n";
    cout << "====================================================================================\n";
    cout << "                                  Resultados\n";
    cout << "====================================================================================\n";

    cout << left
        << setw(8) << "Cen."
        << setw(8) << "n"
        << setw(20) << "Bubble Des(ms)"
        << setw(22) << "Insertion Des(ms)"
        << setw(20) << "Bubble Ord(ms)"
        << setw(22) << "Insertion Ord(ms)"
        << "\n";

    cout << "------------------------------------------------------------------------------------\n";

    int cenario = 1;

    for (const auto& r : resultados)
    {
        cout << left
            << setw(8) << ("C" + to_string(cenario))
            << setw(8) << nomeTamanho(r.n)
            << setw(20) << fixed << setprecision(4)
            << r.tempoBubbleDesordenado
            << setw(22)
            << r.tempoInsertionDesordenado
            << setw(20)
            << r.tempoBubbleOrdenado
            << setw(22)
            << r.tempoInsertionOrdenado
            << "\n";

        cenario++;
    }

    cout << "\nFim dos testes.\n";

    return 0;
}