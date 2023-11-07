#include <iostream>
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

    void inicializar() {
        ocupado = false;
        qtdePendentes = 0;
        tipo = "";
    }

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

        // Quando está no absoluto inicio
        if (c->tamanhoDaFila() == 1 && c->qtdePendentes == 0) {
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
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes != 0) {
            c->qtdePendentes--;
            return;
        }

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

int* traduzirInput(string input, string tipo_referencia) {

    istringstream stream(input);

    string firstLine;
    getline(stream, firstLine);
    int tamanho = stoi(firstLine);

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
        } else {
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


int main() {

    fila fila_caixa1;
    fila fila_caixa2;
    Agencia agencia;
    agencia.caixa1.inicializar();
    agencia.caixa2.inicializar();
    agencia.caixa1.f = fila_caixa1;
    agencia.caixa2.f = fila_caixa2;
    agencia.definirTempo(20);

    string texto = "20\n1 J 4\n2 J 5\n3 N 0\n4 F 3\n5 J 6\n6 J 2\n7 J 4\n8 F 5\n9 N 0\n10 N 0\n11 F 3\n12 J 1\n13 N 0\n14 N 0\n15 N 0\n16 N 0\n17 F 1\n18 N 0\n19 J 2\n20 J 3";


    int* timingCaixa1 = new int [agencia.tempoTotal]();
    timingCaixa1 = traduzirInput(texto, "F");

    int* timingCaixa2 = new int [agencia.tempoTotal]();
    timingCaixa2 = traduzirInput(texto, "J");

    agencia.caixa1.f.frente = NULL;
    agencia.caixa1.f.re = NULL;
    agencia.caixa1.f.tamanho = 0;
    agencia.caixa2.f.frente = NULL;
    agencia.caixa2.f.re = NULL;
    agencia.caixa2.f.tamanho = 0;

    agencia.caixa1.qtdePendentes = 0;

    for (int i=0; i<agencia.tempoTotal; i++) {

        agencia.caixa1.adicionarNaFila(timingCaixa1[i]);
        agencia.caixa2.adicionarNaFila(timingCaixa2[i]);

        agencia.operarCaixa(&agencia.caixa1);
        agencia.operarCaixa(&agencia.caixa2);
        agencia.passarMinuto();
        agencia.gerarLinha(&agencia.caixa1, &agencia.caixa2);
    }
    
}


