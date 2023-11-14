#include <iostream>
#include <fstream>
#include <thread>
#include <cctype>
#include <string>
#include <sstream>
using namespace std;

struct nodo {
    nodo *elo;
    int min;
    int qtde;
};

struct fila {
    nodo *frente;
    nodo *re;
    int tamanho;
};

class Caixa {
public:
    bool ocupado;
    int qtdePendentes;
    string tipo;
    fila f;

    int tamanhoDaFila () {
        return f.tamanho;
    }

    void adicionarNaFila(int qtde) {

        if (qtde == 0) {
            return;
        }

        nodo *novo;
        novo = new (nothrow) nodo;
        novo->qtde = qtde;
        novo->elo = NULL;

        if (f.frente == NULL) {
            f.frente = novo;
            f.re = novo;
            f.tamanho++;
            return;
        } else {
            if (f.re != NULL) {
                f.re->elo = novo;
            }
            f.re = novo;
            f.tamanho++;
            return;
        }
    }

    void removerPrimeiroDaFila() {

        if (f.frente == NULL || f.tamanho <= 0) {
            return;
        }

        if (f.frente->elo != NULL) {
            f.frente = f.frente->elo;
            ocupado = false;
            f.tamanho--;
            return;
        }

        if (f.frente != NULL) {
            f.frente = NULL;
            ocupado = false;
            f.tamanho--;
            return;
        }
        return;
    }

    int totalDeDocumentos () {

        if (f.tamanho <= 0) {
            return 0;
        }
        int documentos = 0;
        nodo *index;
        index = f.frente;
        for (int i = 0; i < f.tamanho; i++) {
            documentos += index->qtde;
            if (index->elo == NULL) {
                 return documentos;
            } else {
                index = index->elo;
            }
        }
        return 0;
    }
};

class Agencia {
public:
    Caixa caixa1;
    Caixa caixa2;
    int tempoRestante;
    int tempoTotal;

    void gerarLinha(Caixa *c1, Caixa *c2) {
        string output_c1 = (c1->ocupado ? " A " : " L ") + to_string(c1->tamanhoDaFila());
        string output_c2 = (c2->ocupado ? " A " : " L ") + to_string(c2->tamanhoDaFila());
        cout << (tempoTotal-tempoRestante) << output_c1 << output_c2 << "\n";
    }

    void passarMinuto() {
        tempoRestante--;
    }

    void operarCaixa(Caixa *c) {

        // Quando está no absoluto inicio da Fila com nenhum dcumento pendente
        if (c->tamanhoDaFila() == 1 && c->qtdePendentes == 0) {
            c->qtdePendentes += c->f.frente->qtde;
            c->removerPrimeiroDaFila();
            c->ocupado = true;
            return;
        }

        // Quando você ESTÁ ATENDENDO uma pessoa e TEM APENAS UM documento pendente
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes == 1) {
            c->qtdePendentes--;
            c->qtdePendentes += c->f.frente->qtde;
            c->removerPrimeiroDaFila();
            c->ocupado = true;
            return;
        }

        // Quando você NÃO precisa atender uma pessoa e NÃO tem documentos pendentes
        if (c->tamanhoDaFila() == 0 && c->qtdePendentes <= 0) {
            c->ocupado = false;
            return;
        }

        // Quando você PRECISA atender uma pessoa e NÃO tem documentos pendentes
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes <= 0) {
            c->qtdePendentes += c->f.frente->qtde;
            c->removerPrimeiroDaFila();
            c->ocupado = true;
            return;
        }

        // Quando você ESTÁ ATENDENDO uma pessoa e TEM documentos pendentes
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes > 0) {
            c->ocupado = true;
            c->qtdePendentes--;
            return;
        }

        // Se for seu ultimo documento pendente, e a fila estiver vazia
        if (c->qtdePendentes-1 == 0 && c->tamanhoDaFila() == 0) {
            c->qtdePendentes--;
            c->ocupado = false;
        }

        // Caso nenhuma das condições acima seja ativada
        if (c->qtdePendentes > 0) {
            c->qtdePendentes--;
            return;
        }
    }

    void definirTempo(int tempo) {
        tempoRestante = tempo;
        tempoTotal = tempo;
    }
};

int coletarMinutosDoInput(string input) {
    istringstream stream(input);
    string firstLine;
    getline(stream, firstLine);
    return (stoi(firstLine)+1);
}

int* traduzirInput(string input, string tipo_referencia) {

    istringstream stream(input);
    int tamanho = coletarMinutosDoInput(input);

    int* array = new int[tamanho]();

    string linha;
    int numeroLinha = 0;
    int i = 0;

    while (getline(stream, linha)) {

        if (i >= tamanho) {
            break;
        }

        int min = 0;
        string tipo = "N";
        int qtde = 0;

        if (numeroLinha >= 10) {
            string minutoEmString = "";
            minutoEmString += linha[0];
            minutoEmString += linha[1];
            min = stoi(minutoEmString);

            string qtdeEmString = "";
            qtdeEmString += linha[5];
            qtdeEmString += linha[6];
            qtde = stoi(qtdeEmString);

            tipo = linha[3];
        } else if (numeroLinha != 0) {
            string minutoEmString = "";
            minutoEmString += linha[0];
            min = stoi(minutoEmString);

            string qtdeEmString = "";
            qtdeEmString += linha[4];
            qtdeEmString += linha[5];
            qtde = stoi(qtdeEmString);

            tipo = linha[2];
        }

        if (tipo == tipo_referencia) {
            array[min-1] = qtde;
        }

        i++;
        numeroLinha++;
    }
    return array;
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "No Terminal, use: \\.nome_do_arquivo.cpp entrada.txt > saida.txt" << endl;
        return 1;
    }

    //const char* arquivoDeEntrada = argv[1];
    const char* arquivoDeEntrada = "entrada.txt";
    ifstream entrada(arquivoDeEntrada);

    string entradaEmString = "";

    string linhaDaEntrada;
    while (getline(entrada, linhaDaEntrada)) {
        entradaEmString += (linhaDaEntrada + "\n");
    }

    // Declara as Filas do Caixa
    fila fila_caixa1;
    fila fila_caixa2;

    // Cria instancia da Classe Agencia
    Agencia agencia;

    // Define as filas de cada caixa
    agencia.caixa1.f = fila_caixa1;
    agencia.caixa2.f = fila_caixa2;

    // Define o tempo maximo para observar a Agencia (em minutos)
    agencia.definirTempo(coletarMinutosDoInput(entradaEmString));

    // Cria os Arrays que serão armazenados os QTDE no Index que representa MIN
    // Uso Prático: array[MIN] = [QTDE]
    int* timingCaixa1 = new int [agencia.tempoTotal]();
    timingCaixa1 = traduzirInput(entradaEmString, "F");
    int* timingCaixa2 = new int [agencia.tempoTotal]();
    timingCaixa2 = traduzirInput(entradaEmString, "J");

    // Inicializa as variáveis das Filas
    agencia.caixa1.f.frente = NULL;
    agencia.caixa1.f.re = NULL;
    agencia.caixa1.f.tamanho = 0;
    agencia.caixa2.f.frente = NULL;
    agencia.caixa2.f.re = NULL;
    agencia.caixa2.f.tamanho = 0;

    // Inicia o Loop que finaliza no Tempo Total (minutos)
    for (int i=0; i<(agencia.tempoTotal-1); i++) {

        // Verifica os QTDE daquele minuto especifico pelo array
        agencia.caixa1.adicionarNaFila(timingCaixa1[i]);
        agencia.caixa2.adicionarNaFila(timingCaixa2[i]);

        // Realiza as ações de operar o caixa
        agencia.operarCaixa(&agencia.caixa1);
        agencia.operarCaixa(&agencia.caixa2);

        // Passa 1 minuto na agência (afetando todos os caixas)
        agencia.passarMinuto();

        // Gera uma Linha com base na situação atual
        agencia.gerarLinha(&agencia.caixa1, &agencia.caixa2);
    }
}

// LEMBRETES:
// to_string - Converte de int para string
// stoi - Converte de string para int
// stol - Converte de string para long
// stof - Converte de string para float
// stod - Converte de string para double
// to_string - Converte de qualquer tipo numérico para string
// stoi, stol, stoll, stoul, stoull, stof, stod, stold - Podem lançar std::invalid_argument se a string não for um número válido
// stoi, stol, stoll - Podem lançar std::out_of_range se o valor convertido estiver fora do intervalo representável pelo tipo
// stoi, stol, stoll, stoul, stoull - Aceitam uma segunda argumento opcional para indicar a posição do primeiro caractere não convertível na string
// to_string - Funciona para int, long, long long, unsigned int, unsigned long, unsigned long long, float, double, long double
// to_string - Pode ser usado para converter tipos personalizados se houver um operador de inserção sobrecarregado
// char - Pode ser convertido para int subtraindo '0'
// int - Pode ser convertido para char adicionando '0'
