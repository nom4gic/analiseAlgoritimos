#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <algorithm>

using namespace std;
using namespace chrono;

// --------------------------------------------------
// shell Sort
// --------------------------------------------------
void shellSort(vector<int>& valores)
{
    int tamanho = valores.size();

    for (int gap = tamanho / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < tamanho; i++)
        {
            int valor = valores[i];
            int j = i;

            while (j >= gap && valores[j - gap] > valor)
            {
                valores[j] = valores[j - gap];
                j -= gap;
            }

            valores[j] = valor;
        }
    }
}

// --------------------------------------------------
// quick Sort
// --------------------------------------------------
// escolhe o pivo pela mediana entre o primeiro, o do meio e o ultimo
// isso evita o pior caso (O(n) de profundidade) quando o vetor ja
// esta ordenado, que era o que estava causando o stack overflow
void escolhePivoMediana(vector<int>& valores, int inicio, int fim)
{
    int meio = inicio + (fim - inicio) / 2;

    if (valores[meio] < valores[inicio])
    {
        swap(valores[meio], valores[inicio]);
    }

    if (valores[fim] < valores[inicio])
    {
        swap(valores[fim], valores[inicio]);
    }

    if (valores[fim] < valores[meio])
    {
        swap(valores[fim], valores[meio]);
    }

    // coloca o valor do meio (a mediana) na posicao fim,
    // que e onde a particao espera encontrar o pivo
    swap(valores[meio], valores[fim]);
}

int particionaQuick(vector<int>& valores, int inicio, int fim)
{
    escolhePivoMediana(valores, inicio, fim);

    int pivo = valores[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++)
    {
        if (valores[j] <= pivo)
        {
            i++;

            int temp = valores[i];
            valores[i] = valores[j];
            valores[j] = temp;
        }
    }

    int temp = valores[i + 1];
    valores[i + 1] = valores[fim];
    valores[fim] = temp;

    return i + 1;
}

void quickSortRecursivo(vector<int>& valores, int inicio, int fim)
{
    if (inicio < fim)
    {
        int posicaoPivo = particionaQuick(valores, inicio, fim);

        quickSortRecursivo(valores, inicio, posicaoPivo - 1);
        quickSortRecursivo(valores, posicaoPivo + 1, fim);
    }
}

void quickSort(vector<int>& valores)
{
    int tamanho = valores.size();

    if (tamanho > 1)
    {
        quickSortRecursivo(valores, 0, tamanho - 1);
    }
}

// --------------------------------------------------
// merge Sort
// --------------------------------------------------
void intercalaMerge(vector<int>& valores, int inicio, int meio, int fim)
{
    vector<int> esquerda(valores.begin() + inicio, valores.begin() + meio + 1);
    vector<int> direita(valores.begin() + meio + 1, valores.begin() + fim + 1);

    int i = 0;
    int j = 0;
    int k = inicio;

    while (i < (int)esquerda.size() && j < (int)direita.size())
    {
        if (esquerda[i] <= direita[j])
        {
            valores[k] = esquerda[i];
            i++;
        }
        else
        {
            valores[k] = direita[j];
            j++;
        }

        k++;
    }

    while (i < (int)esquerda.size())
    {
        valores[k] = esquerda[i];
        i++;
        k++;
    }

    while (j < (int)direita.size())
    {
        valores[k] = direita[j];
        j++;
        k++;
    }
}

void mergeSortRecursivo(vector<int>& valores, int inicio, int fim)
{
    if (inicio < fim)
    {
        int meio = inicio + (fim - inicio) / 2;

        mergeSortRecursivo(valores, inicio, meio);
        mergeSortRecursivo(valores, meio + 1, fim);

        intercalaMerge(valores, inicio, meio, fim);
    }
}

void mergeSort(vector<int>& valores)
{
    int tamanho = valores.size();

    if (tamanho > 1)
    {
        mergeSortRecursivo(valores, 0, tamanho - 1);
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

    if (n == 250000)
        return "250k";

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

    double tempoShellDesordenado;
    double tempoShellOrdenado;

    double tempoQuickDesordenado;
    double tempoQuickOrdenado;

    double tempoMergeDesordenado;
    double tempoMergeOrdenado;
};

int main()
{
    // Tamanhos solicitados pelo experimento (aula 4 pede 250k a mais)
    vector<int> tamanhos = {
        1000,
        5000,
        10000,
        50000,
        100000,
        250000,
        500000,
        1000000
    };

    // Gerador aleatório
    random_device rd;
    mt19937 gerador(rd());

    uniform_int_distribution<int> distribuicao(0, 10500);

    vector<Resultado> resultados;

    cout << "=============================================\n";
    cout << "  Experimento - Shell Sort | Quick Sort | Merge Sort\n";
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

        // Os tres algoritmos recebem os mesmos valores
        vector<int> valoresShell = valores;
        vector<int> valoresQuick = valores;
        vector<int> valoresMerge = valores;

        // =========================================
        // 3. ordenação - Shell Sort desordenado
        // =========================================

        auto inicioShellDes =
            high_resolution_clock::now();

        shellSort(valoresShell);

        auto fimShellDes =
            high_resolution_clock::now();

        double tempoShellDes =
            duration<double, milli>(
                fimShellDes - inicioShellDes
            ).count();

        // =========================================
        // 4. ordenação - Quick Sort desordenado
        // =========================================

        auto inicioQuickDes =
            high_resolution_clock::now();

        quickSort(valoresQuick);

        auto fimQuickDes =
            high_resolution_clock::now();

        double tempoQuickDes =
            duration<double, milli>(
                fimQuickDes - inicioQuickDes
            ).count();

        // =========================================
        // 5. ordenação - Merge Sort desordenado
        // =========================================

        auto inicioMergeDes =
            high_resolution_clock::now();

        mergeSort(valoresMerge);

        auto fimMergeDes =
            high_resolution_clock::now();

        double tempoMergeDes =
            duration<double, milli>(
                fimMergeDes - inicioMergeDes
            ).count();

        // =========================================
        // 6. criação do vetor já ordenado
        // =========================================

        // O vetor já ordenado será utilizado
        // nos testes dos tres algoritmos
        vector<int> valoresOrdenados = valoresShell;

        vector<int> valoresShellOrdenado =
            valoresOrdenados;

        vector<int> valoresQuickOrdenado =
            valoresOrdenados;

        vector<int> valoresMergeOrdenado =
            valoresOrdenados;

        // =========================================
        // 7. ordenação - Shell Sort ordenado
        // =========================================

        auto inicioShellOrd =
            high_resolution_clock::now();

        shellSort(valoresShellOrdenado);

        auto fimShellOrd =
            high_resolution_clock::now();

        double tempoShellOrd =
            duration<double, milli>(
                fimShellOrd - inicioShellOrd
            ).count();

        // =========================================
        // 8. ordenação - Quick Sort ordenado
        // =========================================

        auto inicioQuickOrd =
            high_resolution_clock::now();

        quickSort(valoresQuickOrdenado);

        auto fimQuickOrd =
            high_resolution_clock::now();

        double tempoQuickOrd =
            duration<double, milli>(
                fimQuickOrd - inicioQuickOrd
            ).count();

        // =========================================
        // 9. ordenação - Merge Sort ordenado
        // =========================================

        auto inicioMergeOrd =
            high_resolution_clock::now();

        mergeSort(valoresMergeOrdenado);

        auto fimMergeOrd =
            high_resolution_clock::now();

        double tempoMergeOrd =
            duration<double, milli>(
                fimMergeOrd - inicioMergeOrd
            ).count();

        // =========================================
        // 10. armazenar resultados
        // =========================================

        Resultado resultado;

        resultado.n = n;

        resultado.tempoShellDesordenado =
            tempoShellDes;

        resultado.tempoShellOrdenado =
            tempoShellOrd;

        resultado.tempoQuickDesordenado =
            tempoQuickDes;

        resultado.tempoQuickOrdenado =
            tempoQuickOrd;

        resultado.tempoMergeDesordenado =
            tempoMergeDes;

        resultado.tempoMergeOrdenado =
            tempoMergeOrd;

        resultados.push_back(resultado);

        // =========================================
        // 11. mostrar resultados
        // =========================================

        cout << fixed << setprecision(4);

        cout << "Shell Sort desordenado:        "
            << tempoShellDes << " ms\n";

        cout << "Quick Sort desordenado:        "
            << tempoQuickDes << " ms\n";

        cout << "Merge Sort desordenado:        "
            << tempoMergeDes << " ms\n";

        cout << "Shell Sort ordenado:           "
            << tempoShellOrd << " ms\n";

        cout << "Quick Sort ordenado:           "
            << tempoQuickOrd << " ms\n";

        cout << "Merge Sort ordenado:           "
            << tempoMergeOrd << " ms\n\n";
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
        << setw(18) << "Shell Des(ms)"
        << setw(18) << "Quick Des(ms)"
        << setw(18) << "Merge Des(ms)"
        << setw(18) << "Shell Ord(ms)"
        << setw(18) << "Quick Ord(ms)"
        << setw(18) << "Merge Ord(ms)"
        << "\n";

    cout << "------------------------------------------------------------------------------------\n";

    int cenario = 1;

    for (const auto& r : resultados)
    {
        cout << left
            << setw(8) << ("C" + to_string(cenario))
            << setw(8) << nomeTamanho(r.n)
            << setw(18) << fixed << setprecision(4)
            << r.tempoShellDesordenado
            << setw(18)
            << r.tempoQuickDesordenado
            << setw(18)
            << r.tempoMergeDesordenado
            << setw(18)
            << r.tempoShellOrdenado
            << setw(18)
            << r.tempoQuickOrdenado
            << setw(18)
            << r.tempoMergeOrdenado
            << "\n";

        cenario++;
    }

    cout << "\nFim dos testes.\n";

    return 0;
}